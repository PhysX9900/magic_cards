/*
 * cards.h
 *
 *  Created on: Feb 13, 2022
 *      Author: laurentboulet
 */

#ifndef CARDS_H_
#define CARDS_H_

#include <iostream>
#include <string.h>

/* class Card creates a card object given an id. The card IDs are 0-12 for hearts,
 * 13-25 for clubs, 26-38 for diamonds, and 39-51 for spades.
 * A card object can also be created given an integer rank and an integer suit ID.
 * The rank ranges from 0 to 12, where 0 stands for an ace and 12 stands for a king.
 * The suit ID ranges from 0 to 3 where 0 stands for Hearts, 1 stands for Clubs,
 * 2 stands for Diamonds, and 3 stands for Spades.
*/
class Card {
private:
	int id;
	int rank;
	int suitID;
	std::string suits[4] {"Hearts", "Clubs", "Diamonds", "Spades"};
public:
	Card(int id);
	Card(int rank, int suitID);
	void printCard() const;
	std::string rank2FaceVal() const;
	int getRank() const;
	int getSuitID() const;
};

#endif /* CARDS_H_ */
