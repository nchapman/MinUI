/**
 * ubuntu/platform/platform.h - Platform definitions for CI/linting
 *
 * This is a CI platform for static analysis and linting:
 * - 640x480 display (standard VGA resolution)
 * - Uses SDL2 for compatibility
 * - All button mappings defined for complete code coverage
 * - Minimal configuration suitable for clang-tidy analysis
 *
 * @note This platform is for CI/testing only, not for actual deployment
 */

#ifndef PLATFORM_H
#define PLATFORM_H

///////////////////////////////
// Build Information
///////////////////////////////

#define PLATFORM "ubuntu"
#define USE_SDL2 1

///////////////////////////////
// SDL Keyboard Button Mappings
// Ubuntu CI platform does not use SDL keyboard input
///////////////////////////////

#define BUTTON_UP		BUTTON_NA
#define BUTTON_DOWN		BUTTON_NA
#define BUTTON_LEFT		BUTTON_NA
#define BUTTON_RIGHT	BUTTON_NA

#define BUTTON_SELECT	BUTTON_NA
#define BUTTON_START	BUTTON_NA

#define BUTTON_A		BUTTON_NA
#define BUTTON_B		BUTTON_NA
#define BUTTON_X		BUTTON_NA
#define BUTTON_Y		BUTTON_NA

#define BUTTON_L1		BUTTON_NA
#define BUTTON_R1		BUTTON_NA
#define BUTTON_L2		BUTTON_NA
#define BUTTON_R2		BUTTON_NA
#define BUTTON_L3		BUTTON_NA
#define BUTTON_R3		BUTTON_NA

#define BUTTON_MENU		BUTTON_NA
#define BUTTON_MENU_ALT	BUTTON_NA
#define	BUTTON_POWER	BUTTON_NA
#define	BUTTON_PLUS		BUTTON_NA
#define	BUTTON_MINUS	BUTTON_NA

///////////////////////////////
// Evdev/Keyboard Input Codes
// All defined for complete code coverage
///////////////////////////////

#define CODE_UP			CODE_NA
#define CODE_DOWN		CODE_NA
#define CODE_LEFT		CODE_NA
#define CODE_RIGHT		CODE_NA

#define CODE_SELECT		CODE_NA
#define CODE_START		CODE_NA

#define CODE_A			CODE_NA
#define CODE_B			CODE_NA
#define CODE_X			CODE_NA
#define CODE_Y			CODE_NA

#define CODE_L1			CODE_NA
#define CODE_R1			CODE_NA
#define CODE_L2			CODE_NA
#define CODE_R2			CODE_NA
#define CODE_L3			CODE_NA
#define CODE_R3			CODE_NA

#define CODE_MENU		CODE_NA
#define CODE_POWER		CODE_NA
#define CODE_PLUS		CODE_NA
#define CODE_MINUS		CODE_NA

///////////////////////////////
// Joystick Button Mappings
// All defined for complete code coverage
///////////////////////////////

#define JOY_UP			JOY_NA
#define JOY_DOWN		JOY_NA
#define JOY_LEFT		JOY_NA
#define JOY_RIGHT		JOY_NA

#define JOY_SELECT		JOY_NA
#define JOY_START		JOY_NA

#define JOY_A			JOY_NA
#define JOY_B			JOY_NA
#define JOY_X			JOY_NA
#define JOY_Y			JOY_NA

#define JOY_L1			JOY_NA
#define JOY_R1			JOY_NA
#define JOY_L2			JOY_NA
#define JOY_R2			JOY_NA
#define JOY_L3			JOY_NA
#define JOY_R3			JOY_NA

#define JOY_MENU		JOY_NA
#define JOY_POWER		JOY_NA
#define JOY_PLUS		JOY_NA
#define JOY_MINUS		JOY_NA

///////////////////////////////
// Function Button Mappings
// System-level button combinations
///////////////////////////////

#define BTN_RESUME			BTN_X       // Button to resume from save state
#define BTN_SLEEP 			BTN_POWER   // Button to enter sleep mode
#define BTN_WAKE 			BTN_POWER   // Button to wake from sleep
#define BTN_MOD_VOLUME 		BTN_NONE    // Modifier for volume control
#define BTN_MOD_BRIGHTNESS 	BTN_NONE    // Modifier for brightness control
#define BTN_MOD_PLUS 		BTN_PLUS    // Increase button
#define BTN_MOD_MINUS 		BTN_MINUS   // Decrease button

///////////////////////////////
// Display Specifications
///////////////////////////////

#define FIXED_SCALE 	2              // 2x scaling factor for UI
#define FIXED_WIDTH		640            // Screen width in pixels
#define FIXED_HEIGHT	480            // Screen height in pixels (VGA)
#define FIXED_BPP		2              // Bytes per pixel (RGB565)
#define FIXED_DEPTH		(FIXED_BPP * 8) // Bit depth (16-bit color)
#define FIXED_PITCH		(FIXED_WIDTH * FIXED_BPP)  // Row stride in bytes
#define FIXED_SIZE		(FIXED_PITCH * FIXED_HEIGHT) // Total framebuffer size

///////////////////////////////
// UI Layout Configuration
///////////////////////////////

#define MAIN_ROW_COUNT 6           // Number of rows visible in menu
#define PADDING 10                 // Padding for UI elements in pixels

///////////////////////////////
// Platform-Specific Paths and Settings
///////////////////////////////

#define SDCARD_PATH "/tmp/lessui-ci"
#define MUTE_VOLUME_RAW 0          // Standard volume scale

///////////////////////////////
// Platform-Specific Functions (stubs for linting)
///////////////////////////////

// These are platform-specific functions that may be called
// For CI/linting, we provide inline stubs

static inline int GetHDMI(void) { return 0; }
static inline int GetBrightness(void) { return 50; }
static inline int GetVolume(void) { return 10; }
static inline int GetMute(void) { return 0; }
static inline void SetRawVolume(int val) { (void)val; }
static inline void InitSettings(void) { }
static inline void QuitSettings(void) { }

#endif
