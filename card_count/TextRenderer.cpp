#include "TextRenderer.h"

#include "stb_image.h"

/*
 * Cleanup all data related to the class.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Nothing.
 */
void TextRenderer::cleanup() {
	SDL_DestroyTexture(this->glyph_sheet_ptr);
}

/*
 * Get the bounding box on the glyph sheet for a given character.
 * 
 * Parameters:
 *	c: Character to get the bounding box for.
 *	rect_ptr: Pointer to a SDL_Rect object that will be populated with the bounding box
 *			  data for the requested character.
 * 
 * Return:
 *	True if the character that the bounding box is requested for is a visible character.
 *	and false otherwise.
 */
bool TextRenderer::get_glyph_bbox(char c, SDL_Rect* rect_ptr) {
	/* Ignore non-visible characters. */
	if (c < 32 || c > 126) {
		rect_ptr = NULL;
		return false;
	}

	/* The first visible character is at decimal 32 so down shift by that amount. */
	c -= 32;

	rect_ptr->w = this->glyph_width;
	rect_ptr->h = this->glyph_height;
	rect_ptr->x = c * this->glyph_width;
	rect_ptr->y = 0;

	return true;
}

/*
 * Constructor for the TextRenderer class.
 * 
 * Parameters:
 *	glyph_path: Path to the sprite sheet that contains the character glyphs.
 *	render_ptr: Pointer to the main SDL_Renderer object for the program. This is used for
 *				creating a texture for the sprite sheet.
 *	glyph_width: Width in pixels of the glyphs on the sprite sheet.
 *	glyph_height: Height in pixels of the glyphs on the sprite sheet.
 */
TextRenderer::TextRenderer(std::string glyph_path, SDL_Renderer* render_ptr, unsigned int glyph_width, unsigned int glyph_height) :
	glyph_width(glyph_width), glyph_height(glyph_height), glyph_sheet_ptr(NULL), ready(false)
{
	/* Load in the image data. */
	int w, h, c, status;
	unsigned char* image_buffer = stbi_load(glyph_path.c_str(), &w, &h, &c, 0);
	if (image_buffer == NULL) {
		goto IMAGE_LOAD_ERROR;
	}
	/* Make sure that the image has for channels for RGBA. */
	else if (c != 4) {
		goto CHANNEL_COUNT_ERROR;
	}

	/* Create a texture for the image data. */
	this->glyph_sheet_ptr = SDL_CreateTexture(render_ptr, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);
	if (this->glyph_sheet_ptr == NULL) {
		goto CREATE_TEXTURE_ERROR;
	}

	/* Set the correct blend mode to support the alpha channel. */
	SDL_SetTextureBlendMode(this->glyph_sheet_ptr, SDL_BLENDMODE_BLEND);

	/* Load the image data into the texture. */
	status = SDL_UpdateTexture(this->glyph_sheet_ptr, NULL, image_buffer, w * c);
	if (status != 0) {
		goto CREATE_TEXTURE_ERROR;
	}
	stbi_image_free(image_buffer);

	this->ready = true;
	return;

CREATE_TEXTURE_ERROR:
	this->cleanup();
CHANNEL_COUNT_ERROR:
	stbi_image_free(image_buffer);
IMAGE_LOAD_ERROR:
	this->ready = false;
	return;

}

/*
 * Destructor for the class. Uses the internal cleanup function.
 */
TextRenderer::~TextRenderer() {
	this->cleanup();
}

/*
 * Return the status of the object.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	True if the object is in a ready state and false otherwise.
 */
bool TextRenderer::is_ready() {
	return this->ready;
}

/*
 * Get a pointer to the internal sprite sheet texture.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Constant pointer to the internal SDL_Texture object.
 */
SDL_Texture* TextRenderer::get_glyph_sheet_texture() {
	return this->glyph_sheet_ptr;
}

/* 
 * Get the width of the glyphs on the sprite sheet.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Width of the glyphs as an unsigned integer.
 */
unsigned int TextRenderer::get_glyph_width() {
	return this->glyph_width;
}

/*
 * Get the height of the glyphs on the sprite sheet.
 *
 * Parameters:
 *	None
 *
 * Return:
 *	Height of the glyphs as an unsigned integer.
 */
unsigned int TextRenderer::get_glyph_height() {
	return this->glyph_height;
}
