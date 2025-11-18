#!/bin/bash
# Extract config templates from existing paks
# This is a one-time script to populate skeleton/TEMPLATES/paks/configs/

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SOURCE_DIR="$PROJECT_ROOT/skeleton/SYSTEM/miyoomini/paks/Emus"
SOURCE_EXTRAS="$PROJECT_ROOT/skeleton/EXTRAS/Emus/miyoomini"
TEMPLATE_DIR="$PROJECT_ROOT/skeleton/TEMPLATES/paks/configs"

mkdir -p "$TEMPLATE_DIR"

echo "Extracting config templates from miyoomini paks..."

# Extract SYSTEM cores
for pak in "$SOURCE_DIR"/*.pak; do
    if [ -d "$pak" ]; then
        pak_name=$(basename "$pak" .pak)
        config_file="$pak/default.cfg"

        if [ -f "$config_file" ]; then
            output_file="$TEMPLATE_DIR/${pak_name}.cfg"
            echo "  Extracting $pak_name.cfg"

            # Replace the first line with template variable
            # (first line is always the platform-specific minarch setting)
            tail -n +2 "$config_file" > "$output_file.tmp"
            echo "{{PLATFORM_MINARCH_SETTING}}" > "$output_file"
            if [ -s "$output_file.tmp" ]; then
                cat "$output_file.tmp" >> "$output_file"
            fi
            rm "$output_file.tmp"
        fi
    fi
done

# Extract EXTRAS cores
if [ -d "$SOURCE_EXTRAS" ]; then
    for pak in "$SOURCE_EXTRAS"/*.pak; do
        if [ -d "$pak" ]; then
            pak_name=$(basename "$pak" .pak)
            config_file="$pak/default.cfg"

            if [ -f "$config_file" ]; then
                output_file="$TEMPLATE_DIR/${pak_name}.cfg"

                # Skip if already extracted from SYSTEM
                if [ -f "$output_file" ]; then
                    continue
                fi

                echo "  Extracting $pak_name.cfg (from EXTRAS)"

                # For EXTRAS, the first line is usually minarch_cpu_speed = Performance
                # Replace with template variable
                tail -n +2 "$config_file" > "$output_file.tmp"
                echo "{{PLATFORM_MINARCH_SETTING}}" > "$output_file"
                if [ -s "$output_file.tmp" ]; then
                    cat "$output_file.tmp" >> "$output_file"
                fi
                rm "$output_file.tmp"
            fi
        fi
    done
fi

echo "Config template extraction complete!"
echo "Templates saved to: $TEMPLATE_DIR"
