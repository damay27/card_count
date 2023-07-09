#pragma once

#include "FrontEnd.h"
#include "Deck.h"


/*
 * Frontend class that handles all user facing I/O through the terminal.
 * This class implements the interface defined by the FrontEnd class.
 */
class AsciiFrontEnd : public FrontEnd
{
public:
	void draw_card(Card& card);
	int get_count_input();
	void print_message(std::string message);
	bool is_ready();
	void handle_events();
};
