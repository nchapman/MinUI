###########################################################
# Emus - Emulator paks
# Downloads libretro cores from minarch-cores repository
#
# Hook interface:
#   setup   - Download cores for both architectures
#   build   - (not implemented - cores are pre-built)
#   install - (not implemented - cores already in place)
#   clean   - Remove downloaded cores
###########################################################

MINARCH_CORES_VERSION ?= 20251119
CORES_BASE = https://github.com/nchapman/minarch-cores/releases/download/$(MINARCH_CORES_VERSION)

# Setup hook - download cores (runs once)
# DESTDIR is absolute path to build/SYSTEM/common/bin
# Cores go in build/.system/cores (3 levels up from SYSTEM, then down to .system)
setup:
	@CORES_DIR="$(DESTDIR)/../../../.system/cores"; \
	mkdir -p "$$CORES_DIR/a7" "$$CORES_DIR/a53"; \
	if [ -f cores-override/linux-cortex-a7.zip ]; then \
		echo "Using local a7 cores from cores-override/..."; \
		unzip -o -j -q cores-override/linux-cortex-a7.zip -d "$$CORES_DIR/a7"; \
	else \
		echo "Downloading a7 cores (ARM32) from minarch-cores $(MINARCH_CORES_VERSION)..."; \
		curl -sL $(CORES_BASE)/linux-cortex-a7.zip -o /tmp/lessui-cores-a7.zip; \
		unzip -o -j -q /tmp/lessui-cores-a7.zip -d "$$CORES_DIR/a7"; \
		rm /tmp/lessui-cores-a7.zip; \
	fi; \
	if [ -f cores-override/linux-cortex-a53.zip ]; then \
		echo "Using local a53 cores from cores-override/..."; \
		unzip -o -j -q cores-override/linux-cortex-a53.zip -d "$$CORES_DIR/a53"; \
	else \
		echo "Downloading a53 cores (ARM64) from minarch-cores $(MINARCH_CORES_VERSION)..."; \
		curl -sL $(CORES_BASE)/linux-cortex-a53.zip -o /tmp/lessui-cores-a53.zip; \
		unzip -o -j -q /tmp/lessui-cores-a53.zip -d "$$CORES_DIR/a53"; \
		rm /tmp/lessui-cores-a53.zip; \
	fi; \
	echo "Cores deployed successfully:"; \
	echo "  a7:  $$(ls "$$CORES_DIR/a7"/*.so 2>/dev/null | wc -l | tr -d ' ') cores (ARM32)"; \
	echo "  a53: $$(ls "$$CORES_DIR/a53"/*.so 2>/dev/null | wc -l | tr -d ' ') cores (ARM64)"

# Build hook - not needed (cores are pre-built)
build:
	@true

# Install hook - not needed (cores installed during setup)
install:
	@true

# Clean hook
clean:
	@# Cores are in build/.system/cores, cleaned by main makefile
	@true

.PHONY: setup build install clean
