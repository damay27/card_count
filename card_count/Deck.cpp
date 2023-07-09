#include <string>

#include "Deck.h"

/*
 * Default constructor for the Card class.
 */
Card::Card() :
	rank(CardRank::CardRank_END), suit(CardSuit::CardSuit_END),
	count_value(0), numeric_value(0), valid(false)
{}

/*
 * Constructor for for the card class.
 *
 * Parameters:
 *	rank: Rank of the new card being created.
 *	suit: Suit of the new card being created.
 *	valid: Flag indicating if the card is currently valid and
 *	in a good state.
 */
Card::Card(CardRank rank, CardSuit suit, bool valid = true) :
	rank(rank), suit(suit), valid(valid)
{
	/* Set the numeric value based on the rank. */
	if (this->rank == CardRank::Ace) {
		this->numeric_value = 11;
	}
	else if (this->rank == CardRank::Jack ||
		this->rank == CardRank::Queen ||
		this->rank == CardRank::King) {
		this->numeric_value = 10;
	}
	else {
		/* This is a similification we can make for number cards only. */
		this->numeric_value = (char)this->rank;
	}

	/* Set the count value. */
	if (this->rank == CardRank::Ten ||
		this->rank == CardRank::Jack ||
		this->rank == CardRank::Queen ||
		this->rank == CardRank::King ||
		this->rank == CardRank::Ace) {
		this->count_value = -1;
	}
	else if (this->rank == CardRank::Two ||
		this->rank == CardRank::Three ||
		this->rank == CardRank::Four ||
		this->rank == CardRank::Five ||
		this->rank == CardRank::Six) {
		this->count_value = 1;
	}
	/* This condition handles 7, 8, 9.*/
	else {
		this->count_value = 0;
	}
}

/*
 * Get the numeric value of the card.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	The numeric value of the card as an 8 bit value.
 */
char Card::get_numeric_value() const {
	return this->numeric_value;
}

/*
 * Get the count value of the card.
 *
 * Parameters:
 *	None
 * 
 * Return:
 *	Count value of the card as an 8 bit value.
 */
char Card::get_count_value() const {
	return this->count_value;
}

/*
 * Get the rank of the card.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	The rank of the card as a CardRank enum.
 */
CardRank Card::get_card_rank() const {
	return this->rank;
}

/*
 * Get the suit of the card.
 *
 * Parameters:
 *	None
 *
 * Return:
 *	The suit of the card as a CardSuit enum.
 */
CardSuit Card::get_card_suit() const {
	return this->suit;
}

/*
 * Return the rank and suit of the card represented
 * as an ascii string.
 * 
 * Patameters:
 *	None
 * 
 * Return:
 *	String representation of the rank/suit of the card.
 *	For exmaple the 5 of clubs would be represented as
 *	"5C".
 */
std::string Card::ascii() const {
	std::string str;

	switch (this->rank) {
	case CardRank::Two:
		str += "2";
		break;
	case CardRank::Three:
		str += "3";
		break;
	case CardRank::Four:
		str += "4";
		break;
	case CardRank::Five:
		str += "5";
		break;
	case CardRank::Six:
		str += "6";
		break;
	case CardRank::Seven:
		str += "7";
		break;
	case CardRank::Eight:
		str += "8";
		break;
	case CardRank::Nine:
		str += "9";
		break;
	case CardRank::Ten:
		str += "10";
		break;
	case CardRank::Jack:
		str += "J";
		break;
	case CardRank::Queen:
		str += "Q";
		break;
	case CardRank::King:
		str += "K";
		break;
	case CardRank::Ace:
		str += "A";
		break;

	default:
		str += "ERROR";
	}

	switch (this->suit) {
	case CardSuit::Club:
		str += "C";
		break;
	case CardSuit::Diamond:
		str += "D";
		break;
	case CardSuit::Heart:
		str += "H";
		break;
	case CardSuit::Spade:
		str += "S";
		break;

	default:
		str += "ERROR";
	}

	return str;

}

/*
 * Check if the card object is in a valid state.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Boolean value. True if the card is valid and
 *	false otherwise.
 */
bool Card::is_valid() const {
	return this->valid;
}

/*
 * Constructor for the Deck class.
 * 
 * Parameters:
 *	None
 */
Deck::Deck() {
	/* Create one instance of each card. */
	for (int suit = (int)CardSuit::Club; suit < (int)CardSuit::CardSuit_END; suit++) {
		for (int rank = (int)CardRank::Two; rank < (int)CardRank::CardRank_END; rank++) {
			this->unshuffled.push_back(Card((CardRank)rank, (CardSuit)suit));
		}
	}

	/* Shuffle the deck. */
	this->shuffle();
}

/*
 * Helper function to huffle the deck.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Nothing
 */
void Deck::shuffle() {
	/* Empty the shuffled deck before starting. */
	while (!this->shuffled.empty()) {
		this->unshuffled.push_back(this->shuffled.back());
		this->shuffled.pop_back();
	}

	/* Randomly move cards from the unshffled deck to the shuffled deck. */
	while (!this->unshuffled.empty()) {
		int index = rand() % unshuffled.size();
		this->shuffled.push_back(this->unshuffled.at(index));
		this->unshuffled.erase(this->unshuffled.begin() + index);
	}

	return;
}

/*
 * Remove a card from the shuffled deck. If the shuffled deck
 * is empty a invalid card will be returned.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Always returns a Card object. If the shuffled deck is empty
 *	an invalid card will be returned. The returned card should always
 *	be checked using the is_valid method.
 */
Card Deck::draw() {

	if (this->shuffled.empty()) {
		return Card();
	}
	Card card = this->shuffled.back();
	this->shuffled.pop_back();

	return card;
}

/*
 * Places a card into the unshuffled deck.
 * 
 * Parameters:
 *	card: Card object to place in the unshuffled deck.
 * 
 * Return:
 *	Nothing
 */
void Deck::discard(Card card) {
	this->unshuffled.push_back(card);
}

/*
 * Returns the number of cards left in the
 * shuffled deck.
 * 
 * Parameters:
 *	None
 * 
 * Return:
 *	Number of cards in the shuffled deck as an integer.
 */
int Deck::shuffeled_card_count() {
	return this->shuffled.size();
}

/*
 * Returns the number of cards left in the
 * unshuffled deck.
 *
 * Parameters:
 *	None
 *
 * Return:
 *	Number of cards in the unshuffled deck as an integer.
 */
int Deck::unshuffeled_card_count() {
	return this->unshuffled.size();
}
