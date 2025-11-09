/**
 * test_sdl_mocking_example.c - Proof of concept for SDL mocking with fff
 *
 * This file demonstrates how to use the Fake Function Framework (fff) to mock
 * SDL functions for unit testing MinUI code.
 *
 * This is a DEMONSTRATION of the mocking infrastructure, showing:
 * - How to set up fff fakes
 * - How to configure mock behavior
 * - How to verify function calls
 * - How to use custom fake implementations
 *
 * For actual GFX/SND/PWR testing, functions would need to be extracted to
 * separate modules (similar to pad.c and collections.c) to avoid the complex
 * dependencies in api.c.
 */

#include "../../../support/unity/unity.h"
#include "../../../support/fff/fff.h"
#include "../../../support/sdl_stubs.h"
#include "../../../support/sdl_fakes.h"

DEFINE_FFF_GLOBALS;

void setUp(void) {
	// Reset all fakes before each test
	RESET_FAKE(SDL_PollEvent);
	RESET_FAKE(TTF_SizeUTF8);
	FFF_RESET_HISTORY();
}

void tearDown(void) {
	// Nothing to clean up
}

///////////////////////////////
// Example: SDL Event Mocking
///////////////////////////////

void test_SDL_PollEvent_mock_returns_configured_value(void) {
	// Configure the fake to return "has event"
	SDL_PollEvent_fake.return_val = 1;

	SDL_Event event;
	int result = SDL_PollEvent(&event);

	// Verify return value
	TEST_ASSERT_EQUAL_INT(1, result);

	// Verify function was called once
	TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent_fake.call_count);
}

void test_SDL_PollEvent_mock_no_events(void) {
	// Configure the fake to return "no events"
	SDL_PollEvent_fake.return_val = 0;

	SDL_Event event;
	int result = SDL_PollEvent(&event);

	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent_fake.call_count);
}

///////////////////////////////
// Example: TTF Function Mocking
///////////////////////////////

// Custom fake implementation for TTF_SizeUTF8
// Simulates font rendering by approximating width based on string length
int mock_TTF_SizeUTF8(TTF_Font* font, const char* text, int* w, int* h) {
	if (w)
		*w = strlen(text) * 10; // Assume 10 pixels per character
	if (h)
		*h = font->point_size; // Height = font size
	return 0; // Success
}

void test_TTF_SizeUTF8_mock_calculates_text_width(void) {
	// Use custom fake implementation
	TTF_SizeUTF8_fake.custom_fake = mock_TTF_SizeUTF8;

	TTF_Font font = {.point_size = 16, .style = 0};
	int width, height;

	int result = TTF_SizeUTF8(&font, "Hello", &width, &height);

	// Verify results
	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(50, width); // 5 characters * 10 pixels
	TEST_ASSERT_EQUAL_INT(16, height); // Font point size

	// Verify function was called
	TEST_ASSERT_EQUAL_INT(1, TTF_SizeUTF8_fake.call_count);
}

void test_TTF_SizeUTF8_mock_with_long_text(void) {
	TTF_SizeUTF8_fake.custom_fake = mock_TTF_SizeUTF8;

	TTF_Font font = {.point_size = 12, .style = 0};
	int width, height;

	TTF_SizeUTF8(&font, "This is a longer string", &width, &height);

	TEST_ASSERT_EQUAL_INT(230, width); // 23 characters * 10 pixels
	TEST_ASSERT_EQUAL_INT(12, height);
}

///////////////////////////////
// Example: Multiple Calls and History
///////////////////////////////

void test_fff_tracks_multiple_calls(void) {
	SDL_PollEvent_fake.return_val = 1;

	SDL_Event event;

	// Make multiple calls
	SDL_PollEvent(&event);
	SDL_PollEvent(&event);
	SDL_PollEvent(&event);

	// Verify call count
	TEST_ASSERT_EQUAL_INT(3, SDL_PollEvent_fake.call_count);
}

void test_fff_reset_clears_history(void) {
	SDL_PollEvent_fake.return_val = 1;

	SDL_Event event;
	SDL_PollEvent(&event);

	TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent_fake.call_count);

	// Reset the fake
	RESET_FAKE(SDL_PollEvent);

	// Call count should be back to zero
	TEST_ASSERT_EQUAL_INT(0, SDL_PollEvent_fake.call_count);
}

///////////////////////////////
// Example: Return Value Sequences
///////////////////////////////

void test_fff_return_value_sequence(void) {
	// Configure a sequence of return values
	static int return_sequence[] = {1, 1, 0}; // Two events, then no more
	SET_RETURN_SEQ(SDL_PollEvent, return_sequence, 3);

	SDL_Event event;

	TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent(&event)); // First event
	TEST_ASSERT_EQUAL_INT(1, SDL_PollEvent(&event)); // Second event
	TEST_ASSERT_EQUAL_INT(0, SDL_PollEvent(&event)); // No more events
}

///////////////////////////////
// Test Runner
///////////////////////////////

int main(void) {
	UNITY_BEGIN();

	// SDL Event mocking examples
	RUN_TEST(test_SDL_PollEvent_mock_returns_configured_value);
	RUN_TEST(test_SDL_PollEvent_mock_no_events);

	// TTF mocking examples
	RUN_TEST(test_TTF_SizeUTF8_mock_calculates_text_width);
	RUN_TEST(test_TTF_SizeUTF8_mock_with_long_text);

	// fff framework features
	RUN_TEST(test_fff_tracks_multiple_calls);
	RUN_TEST(test_fff_reset_clears_history);
	RUN_TEST(test_fff_return_value_sequence);

	return UNITY_END();
}
