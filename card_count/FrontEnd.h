#pragma once

#include "Deck.h"

#include <string>

/*
 * This class specifies the required functions on the frontend portion
 * of the card count program. This is a pure virtual class that acts
 * as an interface definition.
 */
class FrontEnd
{
public:
	virtual void draw_card(Card& card) = 0;
	virtual int get_count_input() = 0;
	virtual void print_message(std::string message) = 0;
	virtual bool is_ready() = 0;
	virtual void handle_events() = 0;
};

