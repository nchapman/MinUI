# LessUI Pak Templates

This directory contains templates for generating platform-specific `.pak` directories.

## Directory Structure

```
TEMPLATES/
├── minarch-paks/           # Template system for libretro core paks
│   ├── platforms.json      # Platform metadata (nice prefix, default settings)
│   ├── cores.json          # Core definitions (emu_exe, bundled status)
│   ├── launch.sh.template  # Launch script template (shared by all cores)
│   └── configs/            # Config templates (one per core)
│       ├── GB.cfg
│       ├── GBA.cfg
│       └── ...
└── paks/                   # Direct paks (copied as-is to all platforms)
    └── PAK.pak/
        └── launch.sh
```

## Two Types of Paks

### 1. MinArch Paks (Template-Based)
Emulator paks that launch libretro cores using `minarch.elf`. These are **generated** from templates.

- **Location**: `minarch-paks/`
- **How it works**: One template + per-core configs → generates 19 cores × 12 platforms
- **Purpose**: Eliminates duplication for emulators

### 2. Direct Paks (Copied As-Is)
Special-purpose paks that don't use the template system. These are **copied** directly.

- **Location**: `paks/`
- **How it works**: Complete pak directories copied to all platforms
- **Purpose**: For special cases like PAK.pak (native app launcher)

## Usage

Paks are automatically generated during `make setup`:

```bash
make setup  # Generates all paks for all platforms
```

Manual generation:

```bash
# Generate all paks for all platforms
./scripts/generate-paks.sh all

# Generate for specific platform
./scripts/generate-paks.sh miyoomini

# Generate specific cores for platform
./scripts/generate-paks.sh miyoomini GB GBA
```

## Adding a New Core

1. Add entry to `minarch-paks/cores.json`:
   ```json
   "NEWCORE": {
     "emu_exe": "newcore_name"
   }
   ```

2. Create config template `minarch-paks/configs/NEWCORE.cfg`:
   ```ini
   {{PLATFORM_MINARCH_SETTING}}

   newcore_option = value

   bind Up = UP
   bind Down = DOWN
   ...
   ```

3. Run generation:
   ```bash
   ./scripts/generate-paks.sh all
   ```

That's it! The core will be generated for all 12 platforms.

## Adding a Direct Pak

1. Create pak directory in `paks/`:
   ```bash
   mkdir -p paks/NEWPAK.pak
   ```

2. Add files (will be copied as-is):
   ```
   paks/NEWPAK.pak/
   ├── launch.sh
   └── default.cfg (optional)
   ```

3. Run generation - it will be copied to all platforms automatically

## Template Placeholders

### launch.sh.template (MinArch paks)
- `{{EMU_EXE}}` - Core library name (becomes `${EMU_EXE}_libretro.so`)
- `{{NICE_PREFIX}}` - CPU priority prefix (`nice -20 ` or empty)
- `{{CORES_PATH_OVERRIDE}}` - Sets `CORES_PATH=$(dirname "$0")` for bundled cores

### configs/*.cfg (MinArch paks)
- `{{PLATFORM_MINARCH_SETTING}}` - Platform-specific first-line setting

## Files

- **Source of truth**: This directory (`skeleton/TEMPLATES/`)
- **Generated** (during build): `build/SYSTEM/*/paks/`, `build/EXTRAS/Emus/*/`
- **Never committed**: Generated paks (see `.gitignore`)

## Why Two Systems?

**MinArch paks** all share the same structure:
- Same launch script (only `emu_exe` varies)
- Platform-specific configs
- Generate 228 paks from 19 configs ✅ DRY

**Direct paks** are unique:
- Different structure (like PAK.pak with no config)
- Not worth templating (only 12 copies total)
- Easier to understand as complete directories ✅ Explicit

See `docs/PAK-TEMPLATES.md` for comprehensive documentation.
