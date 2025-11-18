#!/bin/sh
#
# update-functions.sh - LessUI Update/Install Functions
#
# SOURCE LOCATION: skeleton/SYSTEM/common/update-functions.sh
# BUILD DESTINATION (copied during make):
#   .tmp_update/install/update-functions.sh (update time - sourced by boot.sh)
#
# Provides atomic update logic with automatic rollback for all platforms.
# Requires log.sh to be sourced first.
#

# Source logging functions (will be in same directory during update)
. "$(dirname "$0")/install/log.sh"

#######################################
# Atomic update with automatic rollback
#
# Backs up .system before update, restores on failure.
# This prevents orphaned files and partial updates.
#
# Args:
#   $1 - UPDATE_PATH (path to LessUI.zip)
#   $2 - SDCARD_PATH (SD card mount point)
#   $3 - SYSTEM_PATH (.system directory path)
#   $4 - LOG_FILE (log file path)
#
# Returns:
#   0 on success, 1 on failure
#######################################
atomic_system_update() {
	local update_zip="$1"
	local sdcard="$2"
	local system_dir="$3"
	local log="$4"

	# Backup old system if it exists
	if [ -d "$system_dir" ]; then
		log_info "Backing up existing .system to .system-prev..."
		# Remove any stale backup first
		rm -rf "$system_dir-prev"
		# Create fresh backup
		if ! mv "$system_dir" "$system_dir-prev"; then
			log_error "Failed to backup .system - aborting update"
			return 1
		fi
	fi

	# Backup old .tmp_update if it exists
	if [ -d "$sdcard/.tmp_update" ]; then
		log_info "Backing up existing .tmp_update to .tmp_update-prev..."
		# Remove any stale backup first
		rm -rf "$sdcard/.tmp_update-prev"
		# Create fresh backup
		mv "$sdcard/.tmp_update" "$sdcard/.tmp_update-prev" 2>/dev/null
	fi

	# Extract update
	if unzip -o "$update_zip" -d "$sdcard" >> "$log" 2>&1; then
		log_info "Unzip complete"
		# Success - remove backups and cleanup
		rm -rf "$system_dir-prev"
		rm -rf "$sdcard/.tmp_update-prev"
		rm -f "$update_zip"
		return 0
	else
		local exit_code=$?
		log_error "Unzip failed with exit code $exit_code"

		# Failure - restore backups
		if [ -d "$system_dir-prev" ]; then
			log_info "Restoring .system from backup..."
			# Force remove partial .system before restore
			rm -rf "$system_dir"
			if mv "$system_dir-prev" "$system_dir"; then
				log_info ".system backup restored successfully"
			else
				log_error "CRITICAL: Failed to restore .system backup!"
			fi
		fi

		if [ -d "$sdcard/.tmp_update-prev" ]; then
			log_info "Restoring .tmp_update from backup..."
			# Force remove partial .tmp_update before restore
			rm -rf "$sdcard/.tmp_update"
			if mv "$sdcard/.tmp_update-prev" "$sdcard/.tmp_update"; then
				log_info ".tmp_update backup restored successfully"
			else
				log_error "WARNING: Failed to restore .tmp_update backup"
			fi
		fi

		rm -f "$update_zip"
		return 1
	fi
}

#######################################
# Run platform-specific install.sh script
#
# Executes the platform's install.sh script if it exists.
# Logs success or failure.
#
# Args:
#   $1 - INSTALL_SCRIPT (full path to install.sh)
#   $2 - LOG_FILE (log file path)
#
# Returns:
#   0 on success or if script doesn't exist, 1 on failure
#######################################
run_platform_install() {
	local install_script="$1"
	local log_file="$2"

	if [ -f "$install_script" ]; then
		log_info "Running install.sh..."
		if "$install_script" >> "$log_file" 2>&1; then
			log_info "Installation complete"
			return 0
		else
			local exit_code=$?
			log_error "install.sh failed with exit code $exit_code"
			return 1
		fi
	fi
	return 0
}
