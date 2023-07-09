#include "AsciiFrontEnd.h"
#include "Deck.h"

#include <iostream>
#include <string>

/*
 * Draw a card on screen as ascii text.
 *
 * Parameters:
 *	Reference to the card to be drawn in the console.
 * 
 * Return:
 *	Nothing
 */
void AsciiFrontEnd::draw_card(Card& card) {
	std::cout << "---------------\n\n" << std::endl;
	std::cout << card.ascii() << "\n\n" << std::endl;
	std::cout << "---------------\n\n" << std::endl;

}

/*
 * Get the input from the user regarding the current count.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Integer value representing the count.
 */
int AsciiFrontEnd::get_count_input() {
	std::string input_str;
	std::cin >> input_str;
	return std::stoi(input_str);
}

/*
 * Print a message in the console.
 * 
 * Parametsrs:
 *	message: String object contaning the message to print.
 * 
 * Return:
 *	Nothing.
 */
void AsciiFrontEnd::print_message(std::string message) {
	std::cout << message << std::endl;
}

/*
 * Check if the object is in a ready state.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Always returns true.
 */
bool AsciiFrontEnd::is_ready() {
	return true;
}

/*
 * The AsciiFrontEnd doesn't do event handling so this
 * is a no-op.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Nothing
 */
void AsciiFrontEnd::handle_events() {
	return;
}

