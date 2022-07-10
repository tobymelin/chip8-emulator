#ifndef EMU_IO_H
#define EMU_IO_H

#include <SDL.h>

#define RES_MULT 10 // Resolution multiplier
#define VID_HEIGHT 32 // Screen height in pixels
#define VID_WIDTH 64 // Screen width in pixels

class IO {
	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Event event;

		SDL_Rect pixel;

		// Keep the pixel buffer in-memory to allow for on-off operations of
		// pixels based on CHIP-8 opcodes.
		bool screen_pixels[VID_HEIGHT * VID_WIDTH];
	public:
		IO();
		~IO();

		bool emulate();
		void clear_screen();
		void draw_rect(uint16_t pos_x, uint16_t pos_y);
};

#endif

