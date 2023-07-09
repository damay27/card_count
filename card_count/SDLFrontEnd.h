#pragma once

#include "FrontEnd.h"
#include "TextRenderer.h"

#include "SDL.h"

#include <vector>
#include <string>
#include <mutex>
#include <thread>

/*
 * Frontend class that handles all user facing I/O through
 * a GUI using SDL. This class implements the interface
 * defined by the FrontEnd class.
 */
class SDLFrontEnd : public FrontEnd
{
private:
	SDL_Window *window_ptr;
	SDL_Renderer *render_ptr;
	SDL_Texture *card_textures[4][13];
	TextRenderer *textrenderer_ptr;
	std::vector<SDL_Keycode> key_press_queue;
	std::string input_string;
	bool ready;

	void cleanup();
	void draw_string(std::string message, int horizontal_position, int vertical_position, float scale_factor);

public:
	SDLFrontEnd(Deck& deck);
	~SDLFrontEnd();

	void draw_card(Card& card);
	int get_count_input();
	void print_message(std::string message);
	bool is_ready();
	void handle_events();
};

