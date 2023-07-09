#pragma once

#include <string>

#include "SDL.h"

/*
 * Helper class for drawing text using SDL. It works by loading in a sprite sheet
 * contaning glyphs for the visible ascii characters. It assumes that all the
 * glyphs are arranged in a single row on the sprite sheet with each glyph having
 * the same width and height.
 */
class TextRenderer
{
private:

	/* Width and height of the character glyphs in pixels. */
	unsigned int glyph_width, glyph_height;

	SDL_Texture* glyph_sheet_ptr;

	bool ready;

	void cleanup();

public:
	TextRenderer(std::string glyph_path, SDL_Renderer* render_ptr, unsigned int glyph_width, unsigned int glyph_height);
	~TextRenderer();

	bool get_glyph_bbox(char c, SDL_Rect* rect_ptr);
	SDL_Texture* get_glyph_sheet_texture();
	unsigned int get_glyph_width();
	unsigned int get_glyph_height();

	bool is_ready();

};


