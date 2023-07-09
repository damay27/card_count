#include "SDLFrontEnd.h"
#include "Deck.h"
#include "TextRenderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "SDL.h"

#include <string>

#include <Windows.h>

/* Constant data for the textbox boarder. */
#define TEXTBOX_BOARDER_R 0
#define TEXTBOX_BOARDER_G 0
#define TEXTBOX_BOARDER_B 0
#define TEXTBOX_BOARDER_A 255

/* Constant data for the textbox center. */
#define TEXTBOX_BACKGROUND_R 64
#define TEXTBOX_BACKGROUND_G 128
#define TEXTBOX_BACKGROUND_B 64
#define TEXTBOX_BACKGROUND_A 255

#define GLYPH_SHEET_PATH		"resources/glyph_sheet.png"
#define GLYPH_SCALE_FACTOR		.3f
#define GLYPH_WIDTH				38
#define GLYPH_HEIGHT			71

/*
 * Draw the given string on the screen using the location and scale factor provided.
 *
 * Parameters:
 *	message: The string value to be printed on screen.
 *	horixontal_position: Location along the x axis in pixels where the text should be drawn.
 *	vertial_position: Location along the y axis in pixels where the text should be drawn.
 *	scale_factor: Factor by which to scale the indevidual glyphs by. This is used to adjust
 *				  the text size.
 * Return:
 *	Nothing
 */
void SDLFrontEnd::draw_string(std::string message, int horizontal_position, int vertical_position, float scale_factor) {
	SDL_Rect dst_rect;
	unsigned int glyph_draw_width = (unsigned int) (this->textrenderer_ptr->get_glyph_width() * scale_factor);
	unsigned int glyph_draw_height = (unsigned int) (this->textrenderer_ptr->get_glyph_width() * scale_factor);

	/* Loop[ over the entire message string and draw it one character at a time. */
	for (int i = 0; i < message.size(); i++) {

		/* If the character is a newline the move to the next line. */
		if (message.at(i) == '\n') {
			horizontal_position = this->textrenderer_ptr->get_glyph_width();
			vertical_position += glyph_draw_height;
			continue;
		}

		/* Get the bounding box on the glyph sheet for the character we are trying to draw. */
		SDL_Rect glyph_rect;
		this->textrenderer_ptr->get_glyph_bbox(message.at(i), &glyph_rect);

		dst_rect.x = horizontal_position;
		dst_rect.y = vertical_position;
		dst_rect.w = glyph_draw_width;
		dst_rect.h = glyph_draw_height;

		/* 
		 * Before drawing the character erase what ever was previously at that
		 * location by drawing a rectangle over it with the background color.
		 */
		SDL_SetRenderDrawColor(this->render_ptr, TEXTBOX_BACKGROUND_R, TEXTBOX_BACKGROUND_G,
			TEXTBOX_BACKGROUND_B, TEXTBOX_BACKGROUND_A);
		SDL_RenderFillRect(this->render_ptr, &dst_rect);
		SDL_RenderCopy(this->render_ptr, this->textrenderer_ptr->get_glyph_sheet_texture(), &glyph_rect, &dst_rect);
		SDL_SetRenderDrawColor(this->render_ptr, 0, 0, 0, 255);

		horizontal_position += glyph_draw_width;
	}
	/*
	 * Draw over one more chracter slot past the end of the string with the
	 * background color. This special case is needed to accomodate the backspace
	 * operation. For example if there are initially two characters being drawn
	 * but then backspace is pressed we need to make sure that the character
	 * which was just deleted gets removed from the screen.
	 */
	dst_rect.x = horizontal_position;
	dst_rect.y = vertical_position;
	dst_rect.w = glyph_draw_width;
	dst_rect.h = glyph_draw_height;
	SDL_SetRenderDrawColor(this->render_ptr, TEXTBOX_BACKGROUND_R, TEXTBOX_BACKGROUND_G,
		TEXTBOX_BACKGROUND_B, TEXTBOX_BACKGROUND_A);
	SDL_RenderFillRect(this->render_ptr, &dst_rect);
	SDL_SetRenderDrawColor(this->render_ptr, 0, 0, 0, 255);
	SDL_RenderPresent(this->render_ptr);
}

/*
 * Poll/handle events. Window close events are handled right away while
 * key presses are stored in a queue to be processed later on.
 * 
 * NOTE: This is meant to be run in its own thread.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	nothing
 */
void SDLFrontEnd::handle_events() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
		else if (event.type == SDL_KEYUP) {
			this->key_press_queue.push_back(event.key.keysym.sym);
		}
	}
}

/*
 * Free all resources associated with the class.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Nothing
 */
void SDLFrontEnd::cleanup() {
	delete this->textrenderer_ptr;

	/* Destroy all the card textures. */
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			SDL_DestroyTexture(this->card_textures[i][j]);
		}
	}

	if (this->render_ptr != NULL) {
		SDL_DestroyRenderer(this->render_ptr);
	}

	if (this->window_ptr != NULL) {
		SDL_DestroyWindow(this->window_ptr);
	}
	
	SDL_Quit();
}

/*
 * Constructor for the SDLFrontEnd class.
 *
 * Parameters:
 *	deck: Deck object. This is used when loading the card textures.
 */
SDLFrontEnd::SDLFrontEnd(Deck &deck) :
	card_textures(), ready(false) {

	/* Initialize SDL and then setup a window. */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		this->ready = false;
	}

	this->window_ptr = SDL_CreateWindow(
		"CardCounting",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		500,
		725,
		0
	);
	if (this->window_ptr == NULL) {
		this->ready = false;
	}

	this->render_ptr = SDL_CreateRenderer(this->window_ptr, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(this->render_ptr, 0, 0, 0, 255);

	/* Load card textures. */
	Card card = deck.draw();
	while (card.is_valid()) {
		std::string file_name = "resources/cards_png/" + card.ascii() + ".png";

		int width, height, channels;
		unsigned char* image_buffer = stbi_load(file_name.c_str(),
			&width, &height, &channels, 0);
		if (image_buffer == NULL) {
			this->ready = false;
			this->cleanup();
			return;
		}

		SDL_Texture* texture = SDL_CreateTexture(this->render_ptr,
			SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
		if (texture == NULL) {
			this->ready = false;
			this->cleanup();
			return;
		}
		int status = SDL_UpdateTexture(texture, NULL, image_buffer, width * channels);
		if (status != 0) {
			this->ready = false;
			this->cleanup();
			return;
		}

		int suit_index = (int)card.get_card_suit();
		/* The rank type starts from an integer value of two to make converting
		 * between rank and a numeric value easier. However, this means we can't
		 * use rank directly when indexing so we need to subtract to get things
		 * back to being zero indexed.
		 */
		int rank_index = (int)card.get_card_rank() - 2;

		this->card_textures[suit_index][rank_index] = texture;

		stbi_image_free(image_buffer);

		deck.discard(card);
		card = deck.draw();
	}

	/* 
	 * The magic number for widthand height will vary based on the glyph sheet.If you generated
	 * the glyph sheet using the ttf2bitmap.py script these values should have been printed out.
	 */
	this->textrenderer_ptr = new TextRenderer(GLYPH_SHEET_PATH, this->render_ptr, GLYPH_WIDTH, GLYPH_HEIGHT);
	if (this->textrenderer_ptr->is_ready() == false) {
		this->ready = false;
		this->cleanup();
		return;
	}

	this->ready = true;
	return;
	
}

/*
 * Destructor for the SDLFrondEnd class.
 * This simply calls the internal cleanup method.
 */
SDLFrontEnd::~SDLFrontEnd() {
	this->cleanup();
}

void SDLFrontEnd::draw_card(Card& card) {
	int suit_index = (int)card.get_card_suit();
	/* The rank type starts from an integer value of zero to make converting
	 * between rank and a numeric value easier. However, this means we can't
	 * use rank directly when indexing so we need to subtract to get things
	 * back to being zero indexed.
	 */
	int rank_index = (int)card.get_card_rank() - 2;

	SDL_Texture *card_texture = this->card_textures[suit_index][rank_index];
	SDL_RenderClear(this->render_ptr);
	SDL_RenderCopy(this->render_ptr, card_texture, NULL, NULL);
	SDL_RenderPresent(this->render_ptr);

	SDL_ShowWindow(window_ptr);

}

/*
 * Let the user input their guess for the current count.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	The count value entered by the user as an integer.
 */
int SDLFrontEnd::get_count_input() {
	int window_width, window_height;
	SDL_GetWindowSize(this->window_ptr, &window_width, &window_height);

	/* Pre-compute the starting position of the text. */
	unsigned int glyph_draw_width = (unsigned int) (this->textrenderer_ptr->get_glyph_width() * GLYPH_SCALE_FACTOR);
	unsigned int glyph_draw_height = (unsigned int) (this->textrenderer_ptr->get_glyph_width() * GLYPH_SCALE_FACTOR);
	int horizontal_position = this->textrenderer_ptr->get_glyph_width();
	int vertical_position = (window_height / 2) + (glyph_draw_height / 2);

	/* Drop all previous key presses. */
	this->key_press_queue.clear();

	bool done = false;
	int return_value;
	do {

		/* Read key presses out of the queue. */
		while (this->key_press_queue.size() != 0) {

			/* Convert from SDL Keycode to a character and append it to the input string. */
			switch (this->key_press_queue.back()) {
				case SDLK_0:
					this->input_string.push_back('0');
					break;
				case SDLK_1:
					this->input_string.push_back('1');
					break;
				case SDLK_2:
					this->input_string.push_back('2');
					break;
				case SDLK_3:
					this->input_string.push_back('3');
					break;
				case SDLK_4:
					this->input_string.push_back('4');
					break;
				case SDLK_5:
					this->input_string.push_back('5');
					break;
				case SDLK_6:
					this->input_string.push_back('6');
					break;
				case SDLK_7:
					this->input_string.push_back('7');
					break;
				case SDLK_8:
					this->input_string.push_back('8');
					break;
				case SDLK_9:
					this->input_string.push_back('9');
					break;
				case SDLK_MINUS:
					this->input_string.push_back('-');
					break;
				case SDLK_BACKSPACE:
					this->input_string.pop_back();
					break;
				case SDLK_RETURN:
				case SDLK_RETURN2:
					/*
					 * Convert the input string to an integer. Its possible
					 * that the user input a nonsense value such as "93-".
					 */
					try {
						return_value = stoi(this->input_string);
						this->input_string = "";
						done = true;
					}
					catch (...) {
						done = false;
					}
					done = true;
					break;
				default:
					break;
			}
			/* Once an event has been handled remove it from the queue. */
			this->key_press_queue.pop_back();

			/*
			 * Don't allow the input string to grow to more than three characters.
			 * This allows for up to two digits and a minus sign.
			*/
			if (this->input_string.size() > 3) {
				this->input_string.pop_back();
			}

		}

		this->handle_events();

		/* Keep redrawing the string so it updates as the user types. */
		int horizontal_position = this->textrenderer_ptr->get_glyph_width();
		int vertical_position = (window_height / 2) + (this->textrenderer_ptr->get_glyph_height() / 4);
		draw_string(this->input_string, horizontal_position, vertical_position, .5);
	} while (!done);

	return return_value;
}

/*
 * Print the given string in a textbox.
 *
 * Parameteres:
 *	message: String object containing the message to be printed.
 * Return:
 *	Nothing.
 */
void SDLFrontEnd::print_message(std::string message) {
	SDL_Rect textbox_rect;
	int window_width, window_height;
	SDL_GetWindowSize(this->window_ptr, &window_width, &window_height);

	/*
	 * Before drawing the text draw a background that takes up the width of the screen and
	 * is centered vertically. This box is draw it two steps the first step draws a background
	 * that serves as a boarder and the secnd is the actual fill color of the textbox.
	 */

	/* 
	 * Draw the back rectangle of the textbox. This will actually end up being the boarder
	 * of the box once the middle of the box is drawn.
	 */
	textbox_rect.x = 0;
	textbox_rect.y = (unsigned int) ((window_height / 2) - (1.5f * this->textrenderer_ptr->get_glyph_height()));
	textbox_rect.w = window_width;
	textbox_rect.h = 3 * this->textrenderer_ptr->get_glyph_height();
	SDL_SetRenderDrawColor(this->render_ptr, TEXTBOX_BOARDER_R, TEXTBOX_BOARDER_G,
								TEXTBOX_BOARDER_B, TEXTBOX_BOARDER_A);
	SDL_RenderFillRect(this->render_ptr, &textbox_rect);

	/*
	 * Move/shrink the textbox rectangle to draw the center portion of the textbox. 
	 */
	textbox_rect.x += this->textrenderer_ptr->get_glyph_width() / 2;
	textbox_rect.y += this->textrenderer_ptr->get_glyph_width() / 2;
	textbox_rect.w -= this->textrenderer_ptr->get_glyph_width();
	textbox_rect.h -= this->textrenderer_ptr->get_glyph_width();
	SDL_SetRenderDrawColor(this->render_ptr, TEXTBOX_BACKGROUND_R, TEXTBOX_BACKGROUND_G,
								TEXTBOX_BACKGROUND_B, TEXTBOX_BACKGROUND_A);
	SDL_RenderFillRect(this->render_ptr, &textbox_rect);
	SDL_SetRenderDrawColor(this->render_ptr, 0, 0, 0, 255);

	/* Now actaully draw the text. */
	int horizontal_position = this->textrenderer_ptr->get_glyph_width();
	int vertical_position = (window_height / 2) - (this->textrenderer_ptr->get_glyph_height() / 4);
	draw_string(message, horizontal_position, vertical_position, .5);
}

bool SDLFrontEnd::is_ready() {
	return this->ready;
}
