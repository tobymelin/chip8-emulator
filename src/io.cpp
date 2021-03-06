#include "io.h"


// Initialise SDL window and renderer
IO::IO() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CHIP-8 Emulator", 0, 0, VID_WIDTH * RES_MULT, VID_HEIGHT * RES_MULT, SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Reset all screen pixels to avoid junk data sometimes appearing on screen
	clear_screen();

	// Initialise screen pixel
	pixel.h = 1*RES_MULT;
	pixel.w = 1*RES_MULT;
}

IO::~IO() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void IO::clear_screen() {
	for (int i = 0; i < sizeof(screen_pixels); i++) {
		screen_pixels[i] = 0;
	}
}

// Helper function for drawing screen pixels
void IO::draw_rect(uint16_t pos_x, uint16_t pos_y) {
	pixel.x = pos_x * RES_MULT;
	pixel.y = pos_y * RES_MULT;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(renderer, &pixel);
}

// Render loop
bool IO::emulate() {
	bool retval = true;

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// Draw the pixel buffer to screen
	for (int i = 0; i < sizeof(screen_pixels); i++) {
		if (screen_pixels[i]) {
			draw_rect(i % 64, (i - i % 64) / 64);
		}
	}

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			retval = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			// Testing whether keys are recognised
			draw_rect(5, 10);
			draw_rect(20, 10);
		}
	}

	SDL_RenderPresent(renderer);
	SDL_Delay( 50 );

	return retval;
}

// XOR for pixels, return value = 1 if pixel changes from 1 to 0 (ie. pixel clash)
bool IO::change_pixel(uint16_t pos_x, uint16_t pos_y, bool val) {
	bool retval = screen_pixels[pos_x + pos_y * 64] & val;

	// XOR between current pixel value and requested
	screen_pixels[pos_x + pos_y * 64] = screen_pixels[pos_x + pos_y * 64] ^ val;

	return retval;
}
