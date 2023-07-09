This is a game based around practicing black jack card counting. The program will flash a
sequence of cards on the screen and then ask the user what the current count is after a
random number of cards have been displayed. It is written to run on Windows but porting to
other platforms would be trivial.

The counting system used is as follows:
	[2-6]	-> +1
	[7-9]	->  0
	[10-A]	-> -1

The program is divided into front-end and back-end logic. The back-end logic takes care
of managing and updating the game state such as keeping track of the current count,
drawing and discarding cards, and shuffling the deck. The front end takes care of
anything that requires communicating with the user such as displaying information
and receiving user input. There are two front-ends implemented. The first uses the
command-line and displays cards as text. The second front-end is built on SDL2 and
renders cards in a GUI window.

External resources used:
	stb_image -> https://github.com/nothings/stb
	SDL2 -> https://www.libsdl.org/
	Vector-Playing-Cards -> https://github.com/notpeter/Vector-Playing-Cards
	RobotoMono TTF -> https://fonts.google.com/specimen/Roboto+Mono
	ttf2bitmap -> https://github.com/damay27/ttf2bitmap