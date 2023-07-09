#include <iostream>
#include <string>
#include <stdlib.h>
#include <Windows.h>

#include "SDL.h"
#undef main

#include "Deck.h"
#include "FrontEnd.h"
#include "AsciiFrontEnd.h"
#include "SDLFrontEnd.h"

/* This macro determines if we use the SDL or ascii frontend. */
#define FRONTEND_SDL

void event_handler_sleep(unsigned int ms, FrontEnd &frontend) {
	unsigned int ms_count = 0;

	while (ms_count < ms) {
		frontend.handle_events();
		Sleep(10);
		ms_count += 10;
	}
}

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
	{
	srand(time(NULL));

	Deck deck;

#ifdef FRONTEND_SDL
	SDLFrontEnd frontend(deck);
#else
	AsciiFrontEnd frontend;
#endif

	deck.shuffle();
	Card card;

	int ask_card_count = rand() % deck.shuffeled_card_count();
	int count = 0;
	do {
		card = deck.draw();
		event_handler_sleep(2000, frontend);
		if (card.is_valid()) {
			count += card.get_count_value();
			frontend.draw_card(card);
			deck.discard(card);
			if (ask_card_count == deck.shuffeled_card_count()) {
				frontend.print_message("What is the count? ");
				int user_count = frontend.get_count_input();
				if (user_count != count) {
					frontend.print_message("Incorrect count!\nThe correct count is " + std::to_string(count));

				}
				else {
					frontend.print_message("Correct!");
				}
				event_handler_sleep(1000, frontend);
				frontend.print_message("Starting new deck...");
				event_handler_sleep(1000, frontend);
				count = 0;
				deck.shuffle();
			}
		}

		frontend.handle_events();
	} while (1);

	return 0;
}
