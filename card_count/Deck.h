#pragma once

#include <vector>
#include <string>

/*
 * NOTE: The order here matters since we depend on in when generating a deck.
 * We set the Two value equal to integer 2 so that for the numeric cards we can
 * convert them directly between the enum and a numeric value. Things are more
 * complicated though for the face cards and aces.
 */
enum class CardRank {
	Two = 2,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace,
	
	CardRank_END
};

enum class CardSuit {
	Club,
	Diamond,
	Heart,
	Spade,

	CardSuit_END
};


/*
 * Class for representing playing cards from a standard 52 card deck.
 */
class Card {
	//SDL_Texture* texture_ptr;
	CardRank rank;
	CardSuit suit;
	char count_value;
	char numeric_value;
	bool valid;

public:
	Card();
	Card(CardRank value, CardSuit suit, bool valid);
	char get_numeric_value() const;
	char get_count_value() const;
	CardRank get_card_rank() const;
	CardSuit get_card_suit() const;
	std::string ascii() const;
	bool is_valid() const;
};

/*
 * Class for representing a deck of playing cards.
 */
class Deck
{
private:
	std::vector<Card> unshuffled, shuffled;
public:
	Deck();
	void shuffle();
	Card draw();
	void discard(Card card);
	int shuffeled_card_count();
	int unshuffeled_card_count();
};
