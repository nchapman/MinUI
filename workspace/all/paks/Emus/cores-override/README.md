# Cores Override Directory

Place prerelease or custom core builds here to override the normal GitHub download.

## Usage

1. Copy your prerelease core zips here:
   ```bash
   cp ~/Downloads/linux-cortex-a7.zip workspace/all/paks/Emus/cores-override/
   cp ~/Downloads/linux-cortex-a53.zip workspace/all/paks/Emus/cores-override/
   ```

2. Run your build as normal:
   ```bash
   make all
   ```

The build system will automatically detect and use your local zips instead of downloading from GitHub.

## Expected Files

- `linux-cortex-a7.zip` - ARMv7 cores (cortex-a7/a9)
- `linux-cortex-a53.zip` - ARMv8+ cores (cortex-a53/a55)

You can provide one or both files. Any missing files will be downloaded from the configured `MINARCH_CORES_VERSION` release.

## Cleanup

When you're done testing, just delete the zip files to return to normal GitHub downloads:

```bash
rm workspace/all/paks/Emus/cores-override/*.zip
```
