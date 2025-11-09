# MinUI Test Suite

This directory contains the test suite for MinUI, organized to mirror the source code structure.

## Quick Start

```bash
make test   # Run all tests in Docker (recommended)
```

Tests run in a Debian Buster ARM64 container that matches the platform toolchains exactly. This ensures consistency across development environments and catches platform-specific issues.

## Test Environment

**Docker Container Specifications:**
- Base: Debian Buster (10) ARM64
- Compiler: GCC 8.3.0-6 (matches platform toolchains)
- C Library: GLIBC 2.28
- Architecture: aarch64 (falls back to C implementations for 32-bit ARM assembly)
- Dockerfile: `tests/Dockerfile`

**Why Docker?**
- Eliminates macOS ARM architecture differences
- Matches the exact GCC/libc versions used by platform toolchains
- Consistent test results across all development machines
- No need to install native build tools on macOS

## Directory Structure

```
tests/
├── unit/                       # Unit tests (mirror workspace/ structure)
│   └── all/
│       └── common/
│           ├── test_utils.c      # Tests for workspace/all/common/utils.c
│           └── test_date_utils.c # Tests for workspace/all/common/date_utils.c
├── integration/                # Integration tests (end-to-end tests)
├── fixtures/                   # Test data, sample ROMs, configs
├── support/                    # Test infrastructure
│   ├── unity/                  # Unity test framework
│   └── platform.h              # Platform stubs for testing
├── Dockerfile                  # Test environment (Debian Buster ARM64)
└── README.md                   # This file
```

## Organization Philosophy

### Mirror Structure
Tests mirror the source code structure under `workspace/`:

```
workspace/all/common/utils/utils.c        →  tests/unit/all/common/test_utils.c
workspace/all/common/utils/string_utils.c →  tests/unit/all/common/test_string_utils.c
workspace/all/common/utils/file_utils.c   →  tests/unit/all/common/test_file_utils.c
workspace/all/common/utils/name_utils.c   →  tests/unit/all/common/test_name_utils.c
workspace/all/common/utils/date_utils.c   →  tests/unit/all/common/test_date_utils.c
workspace/all/common/utils/math_utils.c   →  tests/unit/all/common/test_math_utils.c
workspace/all/minui/minui.c               →  tests/unit/all/minui/test_minui.c
```

This makes it easy to:
- Find tests for any source file
- Maintain consistency as the codebase grows
- Understand test coverage at a glance

### Test Types

**Unit Tests** (`unit/`)
- Test individual functions in isolation
- Fast execution
- Mock external dependencies
- Example: Testing string manipulation in `utils.c`

**Integration Tests** (`integration/`)
- Test multiple components working together
- Test real workflows (launch a game, save state, etc.)
- May be slower to execute

**Fixtures** (`fixtures/`)
- Sample ROM files
- Test configuration files
- Expected output data

**Support** (`support/`)
- Test frameworks (Unity)
- Shared test utilities
- Platform stubs

## Running Tests

### Docker-Based Testing (Default)

Tests run in a Debian Buster ARM64 container that matches the platform toolchains exactly (GCC 8.3.0, GLIBC 2.28). This eliminates macOS-specific build issues and ensures consistency with the actual build environment.

```bash
# Run all tests (uses Docker automatically)
make test

# Enter Docker container for debugging
make -f Makefile.qa docker-shell

# Rebuild Docker image
make -f Makefile.qa docker-build
```

### Native Testing (Advanced)

Run tests directly on your host system (not recommended on macOS due to architecture differences):

```bash
# Run all tests natively
make -f Makefile.qa test-native

# Run individual test executables
./tests/utils_test         # Timing tests (2 tests)
./tests/string_utils_test  # String tests (35 tests)
./tests/file_utils_test    # File I/O tests (10 tests)
./tests/name_utils_test    # Name processing tests (10 tests)
./tests/date_utils_test    # Date/time tests (30 tests)
./tests/math_utils_test    # Math tests (13 tests)

# Run with verbose output
./tests/string_utils_test -v

# Run specific test
./tests/utils_test -n test_getMicroseconds_non_zero
./tests/string_utils_test -n test_prefixMatch_exact
./tests/file_utils_test -n test_exists_file_exists
./tests/name_utils_test -n test_getDisplayName_simple
./tests/date_utils_test -n test_isLeapYear_divisible_by_4
./tests/math_utils_test -n test_gcd_common_divisor
```

### Clean and Rebuild
```bash
make -f Makefile.qa clean-tests
make test
```

## Writing New Tests

### 1. Mirror the Source Structure

If adding tests for `workspace/all/minui/minui.c`:

```bash
# Create directory
mkdir -p tests/unit/all/minui

# Create test file
touch tests/unit/all/minui/test_minui.c
```

### 2. Use Unity Framework

```c
#include "../../../../workspace/all/minui/minui.h"
#include "../../../support/unity/unity.h"

void setUp(void) {
    // Run before each test
}

void tearDown(void) {
    // Run after each test
}

void test_something(void) {
    TEST_ASSERT_EQUAL_INT(42, my_function());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_something);
    return UNITY_END();
}
```

### 3. Update Makefile.qa

Add your test to the build:

```makefile
tests/unit_tests: tests/unit/all/minui/test_minui.c ...
    @$(CC) -o $@ $^ ...
```

### 4. Common Assertions

```c
TEST_ASSERT_TRUE(condition)
TEST_ASSERT_FALSE(condition)
TEST_ASSERT_EQUAL_INT(expected, actual)
TEST_ASSERT_EQUAL_STRING("expected", actual)
TEST_ASSERT_NOT_NULL(pointer)
TEST_ASSERT_NULL(pointer)
```

See `support/unity/unity.h` for full list.

## SDL Function Mocking

MinUI uses SDL extensively for graphics, input, and audio. Testing SDL-dependent code requires **mocking** SDL functions. We use the **Fake Function Framework (fff)** for this.

### Infrastructure Overview

```
tests/support/
├── fff/
│   └── fff.h              # Fake Function Framework (MIT licensed)
├── sdl_stubs.h            # Minimal SDL type definitions
├── sdl_fakes.h            # fff-based SDL function fakes (declarations)
├── sdl_fakes.c            # fff-based SDL function fakes (definitions)
├── platform_mocks.h       # Mock PLAT_* interface
└── platform_mocks.c       # Mock PLAT_* implementations
```

### What is fff?

The **Fake Function Framework** is a header-only C mocking library that generates mockable versions of functions using macros. It provides:

- **Call tracking** - Counts how many times a function was called
- **Argument history** - Records arguments from the last 50 calls
- **Return value control** - Set return values or sequences
- **Custom implementations** - Provide custom fake behavior

### Using fff for SDL Mocking

#### Example: Mocking SDL_PollEvent

```c
#include "../../../support/unity/unity.h"
#include "../../../support/fff/fff.h"
#include "../../../support/sdl_stubs.h"
#include "../../../support/sdl_fakes.h"

DEFINE_FFF_GLOBALS;

void setUp(void) {
    // Reset fakes before each test
    RESET_FAKE(SDL_PollEvent);
    FFF_RESET_HISTORY();
}

void test_event_handling(void) {
    // Configure mock to return "has event"
    SDL_PollEvent_fake.return_val = 1;

    SDL_Event event;
    int result = SDL_PollEvent(&event);

    // Verify behavior
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent_fake.call_count);
}
```

#### Example: Custom Fake Implementation

```c
// Custom implementation that calculates text width
int mock_TTF_SizeUTF8(TTF_Font* font, const char* text, int* w, int* h) {
    if (w) *w = strlen(text) * 10;  // 10 pixels per character
    if (h) *h = font->point_size;
    return 0;
}

void test_text_width_calculation(void) {
    // Use custom fake
    TTF_SizeUTF8_fake.custom_fake = mock_TTF_SizeUTF8;

    TTF_Font font = {.point_size = 16};
    int width, height;

    TTF_SizeUTF8(&font, "Hello", &width, &height);

    TEST_ASSERT_EQUAL_INT(50, width);   // 5 chars * 10px
    TEST_ASSERT_EQUAL_INT(16, height);  // Font size
}
```

#### Example: Return Value Sequences

```c
void test_multiple_events(void) {
    // Configure sequence: event, event, no more events
    static int return_sequence[] = {1, 1, 0};
    SET_RETURN_SEQ(SDL_PollEvent, return_sequence, 3);

    SDL_Event event;
    TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent(&event));  // First event
    TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent(&event));  // Second event
    TEST_ASSERT_EQUAL_INT(0, SDL_PollEvent(&event));  // No more
}
```

### Available SDL Fakes

**Currently defined:**
- `SDL_PollEvent` - Event polling
- `TTF_SizeUTF8` - Text size calculation

**To add more fakes:**

1. Add declaration in `tests/support/sdl_fakes.h`:
```c
DECLARE_FAKE_VALUE_FUNC(int, SDL_BlitSurface,
                        SDL_Surface*, SDL_Rect*, SDL_Surface*, SDL_Rect*);
```

2. Add definition in `tests/support/sdl_fakes.c`:
```c
DEFINE_FAKE_VALUE_FUNC(int, SDL_BlitSurface,
                       SDL_Surface*, SDL_Rect*, SDL_Surface*, SDL_Rect*);
```

3. Reset in your test's `setUp()`:
```c
RESET_FAKE(SDL_BlitSurface);
```

### Platform Mocks

For mocking platform-specific `PLAT_*` functions, use `platform_mocks.c`:

```c
#include "../../../support/platform_mocks.h"

void test_battery_status(void) {
    // Configure mock battery state
    mock_set_battery(75, 1);  // 75% charge, charging

    // Call code that uses PLAT_getBatteryStatus()
    update_battery_display();

    // Verify behavior
    // ...
}
```

### Complete Example

See `tests/unit/all/common/test_sdl_mocking_example.c` for a comprehensive demonstration of fff usage.

### When to Extract vs. Mock

**Extract to separate module** (like `pad.c`, `collections.c`) when:
- Logic is pure (no SDL dependencies)
- Functions can be reused across components
- Code is tightly coupled to complex global state

**Use SDL mocking** when:
- Testing functions that directly call SDL
- Verifying interaction with SDL APIs
- Testing error handling (simulating SDL failures)

**Skip testing** when:
- Function is trivial (simple getter/setter)
- Logic is entirely SDL rendering (test visually instead)
- Would require massive extraction effort for minimal value

### fff Documentation

For more fff features, see:
- [fff GitHub](https://github.com/meekrosoft/fff)
- `tests/support/fff/fff.h` (inline documentation)
- `tests/unit/all/common/test_sdl_mocking_example.c` (examples)

## Test Guidelines

### Good Test Characteristics
1. **Fast** - Unit tests should run in milliseconds
2. **Isolated** - No dependencies on other tests
3. **Repeatable** - Same result every time
4. **Self-checking** - Asserts verify correctness
5. **Timely** - Written alongside the code

### What to Test
- **Happy paths** - Normal, expected usage
- **Edge cases** - Boundary conditions (empty strings, NULL, max values)
- **Error cases** - Invalid input, file not found, etc.
- **Regression tests** - Known bugs that were fixed

### Example Test Coverage

From `test_utils.c`:
```c
// Happy path
void test_prefixMatch_exact(void) {
    TEST_ASSERT_TRUE(prefixMatch("hello", "hello"));
}

// Edge case
void test_prefixMatch_empty(void) {
    TEST_ASSERT_TRUE(prefixMatch("", "anything"));
}

// Error case
void test_exactMatch_null_strings(void) {
    TEST_ASSERT_FALSE(exactMatch(NULL, "hello"));
}

// Regression test
void test_getEmuName_with_parens(void) {
    // This previously crashed due to overlapping memory
    char out[512];
    getEmuName("test (GB).gb", out);
    TEST_ASSERT_EQUAL_STRING("GB", out);
}
```

## Current Test Coverage

### workspace/all/common/utils/utils.c - ✅ 2 tests
**File:** `tests/unit/all/common/test_utils.c`

- Timing (getMicroseconds)

**Coverage:** Timing functions tested for non-zero values and monotonicity.

### workspace/all/common/utils/string_utils.c - ✅ 35 tests
**File:** `tests/unit/all/common/test_string_utils.c`

- String matching (prefixMatch, suffixMatch, exactMatch, containsString, hide)
- String manipulation (normalizeNewline, trimTrailingNewlines, trimSortingMeta)
- Text parsing (splitTextLines)

**Coverage:** All functions tested with happy paths, edge cases, and error conditions.

### workspace/all/common/utils/file_utils.c - ✅ 10 tests
**File:** `tests/unit/all/common/test_file_utils.c`

- File existence checking (exists)
- File creation (touch)
- File I/O (putFile, getFile, allocFile)
- Integer file I/O (putInt, getInt)

**Coverage:** All file I/O functions tested including edge cases and error conditions.

### workspace/all/common/utils/name_utils.c - ✅ 10 tests
**File:** `tests/unit/all/common/test_name_utils.c`

- Display name processing (getDisplayName) - strips paths, extensions, region codes
- Emulator name extraction (getEmuName) - extracts from ROM paths

**Coverage:** All name processing functions tested with various input formats.

### workspace/all/common/utils/date_utils.c - ✅ 30 tests
**File:** `tests/unit/all/common/test_date_utils.c`

- Leap year calculation (isLeapYear)
- Days in month logic with leap year support (getDaysInMonth)
- Date/time validation and normalization (validateDateTime)
  - Month wrapping (1-12)
  - Year clamping (1970-2100)
  - Day validation (handles varying month lengths and leap years)
  - Time wrapping (hours, minutes, seconds)
- 12-hour time conversion (convertTo12Hour)

**Coverage:** Complete coverage of date/time validation logic.

**Note:** Logic was extracted from `clock.c` into a proper utility library.

### workspace/all/common/utils/math_utils.c - ✅ 13 tests
**File:** `tests/unit/all/common/test_math_utils.c`

- Greatest common divisor (gcd) - Euclidean algorithm
- 16-bit color averaging (average16) - RGB565 pixel blending
- 32-bit color averaging (average32) - RGBA8888 pixel blending with overflow handling

**Coverage:** Pure math functions with edge cases and real-world scenarios.

**Note:** Extracted from `api.c` for reusability and testability.

### workspace/all/common/pad.c - ✅ 21 tests
**File:** `tests/unit/all/common/test_api_pad.c`

- Button state management (PAD_reset, PAD_setAnalog)
- Analog stick deadzone handling
- Opposite direction cancellation (left/right, up/down)
- Button repeat timing
- Query functions (anyJustPressed, justPressed, isPressed, justReleased, justRepeated)
- Menu tap detection (PAD_tappedMenu)

**Coverage:** Complete coverage of input state machine logic.

**Note:** Extracted from `api.c` for testability without SDL dependencies.

### workspace/all/common/collections.c - ✅ 30 tests
**File:** `tests/unit/all/common/test_collections.c`

- Array lifecycle (Array_new, Array_free)
- Array operations (push, pop, unshift, reverse)
- Capacity growth (doubling when full)
- StringArray operations (indexOf, StringArray_free)
- Hash map operations (Hash_new, Hash_set, Hash_get, Hash_free)
- Integration tests (ROM alias maps, recent games lists)

**Coverage:** Complete coverage of dynamic array and hash map data structures.

**Note:** Extracted from `minui.c` for reusability across all components.

### SDL Mocking Infrastructure - ✅ 7 tests
**File:** `tests/unit/all/common/test_sdl_mocking_example.c`

- SDL event mocking (SDL_PollEvent)
- TTF function mocking (TTF_SizeUTF8 with custom implementations)
- fff framework features (call tracking, return sequences, history)

**Coverage:** Demonstrates SDL mocking with Fake Function Framework (fff).

**Infrastructure:**
- `tests/support/fff/fff.h` - Fake Function Framework (header-only)
- `tests/support/sdl_stubs.h` - Minimal SDL type definitions
- `tests/support/sdl_fakes.h/c` - SDL function fakes
- `tests/support/platform_mocks.h/c` - Platform function mocks

**Note:** Provides foundation for future testing of SDL-dependent code.

### Todo
- [ ] workspace/all/common/api.c GFX functions (requires extraction or complex mocking)
- [ ] workspace/all/minui/minui.c (integration tests)
- [ ] workspace/all/minarch/minarch.c (integration tests)
- [ ] Integration tests for full launch workflow

## Continuous Integration

Tests run automatically on:
- Pre-commit hooks (optional)
- Pull request validation
- Release builds

Configure CI to run:
```bash
make lint   # Static analysis
make test   # All tests in Docker
```

CI systems should have Docker available. The test environment will automatically:
- Pull/build the Debian Buster ARM64 test image
- Compile and run all tests
- Report any failures

## Debugging Test Failures

### Debug in Docker Container
```bash
# Enter the test container
make -f Makefile.qa docker-shell

# Inside container, build and run tests
make -f Makefile.qa clean-tests test-native

# Build with debug symbols
gcc -g -o tests/utils_test_debug tests/unit/all/common/test_utils.c \
    workspace/all/common/utils/utils.c \
    tests/support/unity/unity.c \
    -I tests/support -I tests/support/unity -I workspace/all/common \
    -std=c99

# Run with gdb (lldb not available in Debian Buster)
gdb tests/utils_test_debug
(gdb) run
(gdb) bt  # backtrace when crash occurs
```

### Debug Natively (macOS/Linux)
```bash
# Build with debug symbols
gcc -g -o tests/utils_test_debug tests/unit/all/common/test_utils.c \
    workspace/all/common/utils/utils.c \
    tests/support/unity/unity.c \
    -I tests/support -I tests/support/unity -I workspace/all/common \
    -std=c99

# macOS
lldb tests/utils_test_debug
(lldb) run
(lldb) bt

# Linux
gdb tests/utils_test_debug
(gdb) run
(gdb) bt
```

### Verbose Output
```bash
# Run individual test with verbose mode
./tests/utils_test -v
./tests/string_utils_test -v
```

### Run Single Test
```bash
# Filter by test name
./tests/utils_test -n test_getMicroseconds_non_zero
./tests/string_utils_test -n test_prefixMatch_exact
```

## References

- **Unity Framework**: https://github.com/ThrowTheSwitch/Unity
- **Test Organization Best Practices**: See this README's structure section
- **C Testing Tutorial**: support/unity/README.md

## Questions?

If you're adding tests and need help:
1. Look at existing tests in `unit/all/common/test_utils.c`
2. Check Unity documentation in `support/unity/`
3. Ask in discussions or open an issue
