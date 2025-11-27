# Paks Migration TODO

This document tracks the migration of existing paks to the new unified architecture (`workspace/all/paks/`).

## Completed ✅

- [x] **Clock** - Migrated to `workspace/all/paks/Clock/`
  - Single source of truth for launch.sh (was duplicated 11 times)
  - Native C code in `src/`
  - Builds and deploys successfully

- [x] **Utils Integration** - Added Jose's utilities to `workspace/all/utils/`
  - minui-keyboard (text input widget)
  - minui-list (settings/menu widget) - upgraded to DP system
  - minui-presenter (message display widget) - upgraded to DP system
  - jq (JSON processor) - downloads prebuilt binaries per platform
  - All deployed to `build/SYSTEM/<platform>/bin/` (available to all paks)

- [x] **Input** - Migrated to `workspace/all/paks/Input/`
  - Native C code in `src/`
  - Single launch.sh (was duplicated 11 times)
  - Successfully validates the cross-platform paks pattern
  - Builds and deploys successfully

- [x] **Files** - Migrated to `workspace/all/paks/Files/`
  - Platform-specific binaries (DinguxCommander, 351Files, system fileM)
  - Launch.sh has platform branching using case/esac
  - Successfully validates `bin/<platform>/` and `res/<platform>/` pattern
  - Removed old skeleton copies (10 platforms)
  - Removed platform-specific makefile.copy rules
  - Builds and deploys successfully

- [x] **Bootlogo** - Migrated to `workspace/all/paks/Bootlogo/`
  - Cross-platform launch.sh with platform branching (7 platforms)
  - Platform-specific resources in `<platform>/` directories
  - Hybrid pak: native code for miyoomini, shell-only for others
  - miyoomini: logoread.elf, logomake.elf, logowrite.elf (compiled)
  - my282: bootlogo.bmp + res/ with firmware offsets
  - my355: payload/bin/ with mkbootimg, unpackbootimg, rsce_tool
  - tg5040: brick/ and default bootlogo.bmp variants
  - zero28, m17, trimuismart: bootlogo.bmp files
  - Enhanced with minui-presenter for user feedback
  - Successfully validates hybrid pak pattern (native + shell)
  - Removed old skeleton copies (7 platforms)
  - Removed platform-specific makefile.copy rules

- [x] **Wifi** - Migrated to `workspace/all/paks/Wifi/`
  - Based on Jose Gonzalez's minui-wifi-pak (third-party integration)
  - Supports 5 platforms: miyoomini, my282, my355, rg35xxplus, tg5040
  - Full UI using minui-keyboard, minui-list, minui-presenter, jq (all system utils)
  - Helper scripts: service-on, service-off, wifi-enabled, on-boot
  - Platform-specific assets:
    - `bin/miyoomini/iw` - WiFi scanning tool
    - `lib/miyoomini/` - Required shared libraries (libnl, libssl, etc.)
    - `res/miyoomini/8188fu.ko` - WiFi kernel module for Miyoo Mini Plus
  - Platform-specific wpa_supplicant.conf templates
  - rg35xxplus uses netplan.yaml for systemd-networkd
  - Boot-time auto-start via auto.sh hook
  - Successfully validates third-party pak integration pattern

## Pending Migrations

### High Priority - Native Code Paks

None remaining!

### Medium Priority - Shell Script Paks

None remaining!

### Low Priority - Platform-Specific Paks

- [ ] **ADBD** - `skeleton/EXTRAS/Tools/miyoomini/ADBD.pak/`
  - miyoomini-specific (WiFi/USB debug)

- [ ] **Enable SSH** - `skeleton/EXTRAS/Tools/rg35xxplus/Enable SSH.pak/`
  - rg35xxplus-specific

- [ ] **Splore** - `skeleton/EXTRAS/Tools/rgb30/Splore.pak/`
  - rgb30-specific (PICO-8 launcher)

- [ ] **Other platform-specific paks** - Various Remove Loading, IP, etc.

## Future Enhancements

- [x] **Emulator paks** - Migrated from `skeleton/TEMPLATES/minarch-paks/` to `workspace/all/paks/Emus/`
  - Template system preserved (works well for uniform emulator paks)
  - All pak sources now unified in `workspace/all/paks/`
  - cores-override also moved to `workspace/all/paks/Emus/cores-override/`

- [ ] **Third-party pak support** - Make it easy to integrate external paks
  - Document pak.json schema
  - Provide template/example paks
  - Consider making `workspace/all/common/` available to external paks

- [ ] **Dependency management** - Add support for downloading external binaries
  - Implement `dependencies` section in pak.json
  - Auto-fetch jq, syncthing, sftpgo, etc.
  - Reference: Jose's sftpgo pak Makefile

## Next Recommended Action

**All high and medium priority tool paks are now migrated, including WiFi!**

Remaining paks are platform-specific (ADBD, SSH, Splore, etc.). These can be migrated as needed or left as platform-specific since they only apply to 1-2 platforms each.

Consider:
1. Third-party pak documentation - Make it easy for external developers to contribute paks
2. Dependency management - Auto-download binaries like jq, syncthing, etc.
