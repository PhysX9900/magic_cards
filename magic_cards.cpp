/*
 * magic_cards.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: laurentboulet
 */
#include "cards.h"
#include "magic_cards.h"
#include <stdlib.h>
#include <unistd.h>

int pauseTime = 10; // how long in seconds the game pauses for after the five cards are chosen
int numCards = 5;

int main() {
	using std::cout;
	using std::endl;
	using std::cin;
	bool quit = false;
	cout << "Welcome to magic cards. The audience chooses five cards from a standard deck of 52 playing cards "
			"keeping their choices secret from the great magician Laurent.\n";
	cout << "The computer will then reveal four of these cards to the great magician Laurent, who will then read the mind "
			"of the audience and guess the hidden fifth card.\n";
	cout << "Press enter to start the game.";
	cin.get();
	while (!quit)
	{
		std::vector<Card> CardChoices;
		int cardsChosen = 0;
		cout << "\nEnter choices for five cards from a standard deck of 52 playing cards.\n";
		cout << "Valid choices for the face values are {A,2,3,4,5,6,7,8,9,T,J,Q,K}.\n";
		cout << "Valid choices for the suits are {H,C,D,S}.\n";
		cout << "To quit enter q.\n";
		while (cardsChosen < numCards)
		{
			cout << "Enter the face value of card #" << cardsChosen+1 << ":\n";
			int faceVal;
			int suitVal;
			char suitChar;
			char faceChar;
			cin >> faceChar;
			cout << "Enter the suit of card #" << cardsChosen+1 << ":\n";
			cin >> suitChar;
			if ( (faceChar == 'q') | (suitChar == 'q') ) {
				quit = true;
				cout << "Quitting!\n";
				break;
			}
			convertInput2Ints(faceChar, suitChar, faceVal, suitVal);
			bool isGood = checkCardChoice(faceVal, suitVal);
			if (!isGood)
				cout << "Invalid card choice! Please choose again.\n";
			bool isDuplicate = checkForDuplicates(faceVal, suitVal, CardChoices);
			if (isDuplicate)
				cout << "Card has already been chosen! Please choose a different card.\n";
			if (isGood && !isDuplicate) {// indicates a valid card choice
				Card tempCard(faceVal, suitVal);
				CardChoices.push_back(tempCard);
				cardsChosen ++;
			}
		}
		if (quit)
			break;
		cout << "\nHere are the cards that you have chosen:\n";
		for (auto it = CardChoices.begin(); it != CardChoices.end(); ++it) {
			it->printCard();
			cout << "\n";
//			cout << " (rank = " << it->getRank() << ", suitID = " << it->getSuitID() << ")\n";
		}
		sleep(pauseTime);
		for (int i=0; i<1000; i++) // output enough blank lines to hide the chosen cards
			cout << "\n";
		std::vector<Card> OrderedCards = magicallySortCards(CardChoices);
		cout << "Press enter to reveal four cards.\n";
		cin.get();
		cin.get();
		show4Cards(OrderedCards);
		cout << "\nNow the magician must guess the last card.\n";
		cout << "Press enter to reveal the hidden card.\n";
		cin.get();
		cout << "The hidden card is the ";
		OrderedCards[0].printCard();
		cout << "!\n";
		cout << "Press enter to play again.\n";
		cin.get();

	} // end of main while loop
	return 0;
}
