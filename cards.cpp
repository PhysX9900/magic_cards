/*
 * cards.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: laurentboulet
 */

#include "cards.h"

Card::Card(int idd) : id(idd)
{
	rank = id % 13;
	suitID = id / 13;
}

Card::Card(int rankk, int suitIDD) : rank(rankk), suitID(suitIDD)
{
	id = rank + 13*suitID;
}

void Card::printCard() const
{
	std::string cardFace = this->rank2FaceVal();
	//std::cout << "The card with an ID of " << id << " is the " << cardFace << " of " << suits[suitID] << std::endl;
	std::cout << cardFace << " of " << suits[suitID];
}

int Card::getRank() const
{
	return rank;
}

int Card::getSuitID() const
{
	return suitID;
}

std::string Card::rank2FaceVal() const
{
	if (rank == 0)
		return "Ace";
	else if (rank == 1)
		return "Two";
	else if (rank == 2)
		return "Three";
	else if (rank == 3)
		return "Four";
	else if (rank == 4)
		return "Five";
	else if (rank == 5)
		return "Six";
	else if (rank == 6)
		return "Seven";
	else if (rank == 7)
		return "Eight";
	else if (rank == 8)
		return "Nine";
	else if (rank == 9)
		return "Ten";
	else if (rank == 10)
		return "Jack";
	else if (rank == 11)
		return "Queen";
	else if (rank == 12)
		return "King";
	else
		return "NULL";
}
