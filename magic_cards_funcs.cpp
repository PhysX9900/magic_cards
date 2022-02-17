/*
 * magic_cards_funcs.cpp
 *
 *  Created on: Feb 14, 2022
 *      Author: laurentboulet
 */

#include <iostream>
#include <algorithm>
#include "magic_cards.h"
#include "cards.h"
extern int numCards;

bool checkCardChoice (int faceVal, int suitVal) {
	if ( (faceVal < 0) | (faceVal > 12) )
		return false; // not in the range of face values
	if ( (suitVal >= 0) && (suitVal <= 3) )
		return true;
	else
		return false; // not one of the four suits
}

bool checkForDuplicates(int faceVal, int suitVal, std::vector<Card> &CardChoices) {
	for (auto it = CardChoices.begin(); it != CardChoices.end(); ++it) {
	    if ( (it->getRank() == faceVal) && (it->getSuitID() == suitVal) )
	    	return true;
	}
	return false;
}

std::vector<Card> chooseHiddenAndFirstCard(const std::vector<Card> &CardChoices) { // NEED TO DEBUG FOR ALL SAME SUITS!
	/* At least two of the cards in CardChoices must have the same suit (by the Pigeonhole Principle).
	 * The hidden card will be chosen to be one of these cards with the repeated suit.
	 * This function places the hidden card at the 0 position in the vector OrderedCardChoices and the
	 *  other card of the same suit in the 1 position and returns OrderedCardChoices. The last three
	 *  cards that will be shown are in the last three elements of OrderedCardChoices.
	 */
	std::vector<int> NumSuits = {0, 0, 0, 0}; // = {#H,#C,#D,#S} vector that counts the number of each suit in CardChoices
	std::vector<int> SuitOfCard;
	std::vector<int> PosnOfHiddenSuit;
	std::vector<int> PosnOfLast3Cards;
	std::vector<Card> OrderedCardChoices;
	int hiddenSuitID = 0;
	for (auto it = CardChoices.begin(); it != CardChoices.end(); ++it) {
		int suitID = it->getSuitID(); // 0 = H, 1 = C, 2 = D, 3 = S
		SuitOfCard.push_back(it->getSuitID());
		NumSuits[suitID] ++;
		if (NumSuits[suitID] == 2) { // choose the suit of the last pair of repeated suits
			hiddenSuitID = suitID;
		}
	}
	for (auto it = SuitOfCard.begin(); it != SuitOfCard.end(); ++it) { // find positions of cards with hiddenSuit
		if (*it == hiddenSuitID && PosnOfHiddenSuit.size() < 2)
			PosnOfHiddenSuit.push_back(std::distance(SuitOfCard.begin(), it));
		else
			PosnOfLast3Cards.push_back(std::distance(SuitOfCard.begin(), it));
	}
	Card card1 = CardChoices[PosnOfHiddenSuit[0]]; // card1 and card2 are the possible choices for the hidden card
	Card card2 = CardChoices[PosnOfHiddenSuit[1]];
	int rank1 = card1.getRank();
	int rank2 = card2.getRank();
	/* Form a cycle from the ranks: 0->1->2->...->12->0. The hidden card will also
	 * be chosen such that its rank is at most 6 places further along the cycle than a second
	 * card of the same suit.
	 * Take the difference of rank1 - rank2 == diff. This might be positive or negative, but not zero.
	 * If 0 < diff <= 6 or if 0 < (diff + 13) <= 6 then choose card1 to be the hidden card.
	 * Otherwise choose card2 to be the hidden card.
	 */
	int diff = rank1 - rank2;
	if ( (0 < diff && diff <= 6) | (0 < (diff+13) && (diff+13) <= 6) ) {
		OrderedCardChoices.push_back(CardChoices[PosnOfHiddenSuit[0]]);
		OrderedCardChoices.push_back(CardChoices[PosnOfHiddenSuit[1]]);
	}
	else {
		OrderedCardChoices.push_back(CardChoices[PosnOfHiddenSuit[1]]);
		OrderedCardChoices.push_back(CardChoices[PosnOfHiddenSuit[0]]);
	}
	for (int i=0; i<int(PosnOfLast3Cards.size()); i++)
		OrderedCardChoices.push_back(CardChoices[PosnOfLast3Cards[i]]);
    return OrderedCardChoices;
}

void reorderLast3Cards(std::vector<Card> &OrderedCards) {
	/* This function modifies the ordering of the last three elements of OrderedCards.
	 * The order in which the last three cards are revealed communicates the difference between the ranks of the
	 * hidden and the first card according to the following scheme:
	 * {small; medium; large} = 1
	 * {small; large; medium} = 2
	 * {medium; small; large} = 3
	 * {medium; large; small} = 4
	 * {large; small; medium} = 5
	 * {large; medium; small} = 6
	 * where small, medium, and large are determined from the card ordering given by rank and suitID. Define the ordering
	 * by letting a card with a higher rank then a second card have a greater value. For two cards with equal rank,
	 * use their suit IDs to determine which card has the higher value in the ordering. For example, consider the
	 * ten of hearts and the ten of spades, because the suit ID of spades = 3 > the suit ID of hearts = 0, the ten of
	 * spades is greater than the ten of hearts.
	 *
	 * The number 1-6 is the amount of steps along the cycle of ranks: 0->1->2->...->12->0 from the first card
	 * shown to the rank of the hidden card (e.g. if rank(card1) = 11 and rank(hidden card) = 1, the rank of the
	 * hidden card is 3 steps along the rank cycle, and the last three cards will be shown in the order {medium; small; large}).
	 */
	// get the number of steps along the cycle (called diff) needed to get the hidden card from the first card
	int rankHidden = OrderedCards[0].getRank();
	int rankFirst = OrderedCards[1].getRank();
	int diff = rankHidden - rankFirst;
	if (diff < 0)
		diff += 13;

	std::array<int, 3> OrderingOfLast3;
	findCurrentOrderingOfLast3(OrderedCards, OrderingOfLast3);
	// get the indices of the small, medium, and large cards in the last three of OrderedCards
	std::array<int, 3> IndexSML; // array that stores the indices of small, medium, and Large in the order {S, M, L}
	for (int i=2; i<numCards; i++) {
		if(OrderingOfLast3[i-2] == 1)
			IndexSML[0] = i;
		else if (OrderingOfLast3[i-2] == 2)
			IndexSML[1] = i;
		else
			IndexSML[2] = i;
	}
	swapToScheme(OrderedCards, diff, IndexSML); // reorders last three elements according to the scheme
	return;
}

void swapToScheme(std::vector<Card> &OrderedCards, const int schemeID, std::array<int, 3> &IndexSML) {
	/* This function swaps the last three elements of OrderedCards according to the scheme:
	 * {small; medium; large} = 1
	 * {small; large; medium} = 2
	 * {medium; small; large} = 3
	 * {medium; large; small} = 4
	 * {large; small; medium} = 5
	 * {large; medium; small} = 6
	 * where schemeID = 1-6.
	 */
	switch(schemeID) {
	case 1: {//{small; medium; large}
		std::array<int, 3> Desired = {2,3,4};
		if (std::equal(IndexSML.begin(), IndexSML.end(), Desired.begin(), Desired.end()) ) // already in correct order
			break;
		else // need to reorder
			swapToSchemeAux(OrderedCards, IndexSML, Desired);
		break;
	}
	case 2: {//{small; large; medium}
		std::array<int, 3> Desired = {2,4,3};
		if (std::equal(IndexSML.begin(), IndexSML.end(), Desired.begin(), Desired.end()) ) // already in correct order
			break;
		else // need to reorder
			swapToSchemeAux(OrderedCards, IndexSML, Desired);
		break;
	}
	case 3: {//{medium; small; large}
		std::array<int, 3> Desired = {3,2,4};
		if (std::equal(IndexSML.begin(), IndexSML.end(), Desired.begin(), Desired.end()) ) // already in correct order
			break;
		else // need to reorder
			swapToSchemeAux(OrderedCards, IndexSML, Desired);
		break;
	}
	case 4: {//{medium; large; small}
		std::array<int, 3> Desired = {4,2,3};
		if (std::equal(IndexSML.begin(), IndexSML.end(), Desired.begin(), Desired.end()) ) // already in correct order
			break;
		else // need to reorder
			swapToSchemeAux(OrderedCards, IndexSML, Desired);
		break;
	}
	case 5: {//{large; small; medium}
		std::array<int, 3> Desired = {3,4,2};
		if (std::equal(IndexSML.begin(), IndexSML.end(), Desired.begin(), Desired.end()) ) // already in correct order
			break;
		else // need to reorder
			swapToSchemeAux(OrderedCards, IndexSML, Desired);
		break;
	}
	case 6: {//{large; medium; small}
		std::array<int, 3> Desired = {4,3,2};
		if (std::equal(IndexSML.begin(), IndexSML.end(), Desired.begin(), Desired.end()) ) // already in correct order
			break;
		else // need to reorder
			swapToSchemeAux(OrderedCards, IndexSML, Desired);
		break;
	}
	}
	return;
}

void swapToSchemeAux(std::vector<Card> &OrderedCards, std::array<int, 3> &IndexSML, std::array<int, 3> DesiredIndices) {
	/* IndexSML gives the indices of the small, medium, and large of the last three elements of OrderedCards.
	 * This function wants to swap these elements such that their indices after swapping match those
	 * given in DesiredIndices.
	 */
	for (int i=0; i<2; i++) {
		int j = i + 1;
		while (IndexSML[i] != DesiredIndices[i]) {
			swap(IndexSML[i], IndexSML[j]); // swap small with medium (j=1) or large (j=2)
			swap(OrderedCards[IndexSML[i]], OrderedCards[IndexSML[j]]);
			j++;
		}
	}
	return;
}

template<typename T>
void swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}

void findCurrentOrderingOfLast3(const std::vector<Card> &OrderedCards, std::array<int, 3> &Ordering) {
	/* This function finds the current card ordering of the last three elements of OrderedCards
	 * It returns an array that shows the relative ordering with the integers 1,2,3.
	 * The first element in the array contains the order value for OrderedCards[2], the second for
	 * OrderedCards[3], and the third for OrderedCards[4].
	 * For example, say OrderedCards[3] > OrderedCards[4] > OrderedCards[2], then this function
	 * returns {1,3,2}.
	 */
	int rank1 = OrderedCards[2].getRank();
	int rank2 = OrderedCards[3].getRank();
	int rank3 = OrderedCards[4].getRank();
	int suitID1 = OrderedCards[2].getSuitID();
	int suitID2 = OrderedCards[3].getSuitID();
	int suitID3 = OrderedCards[4].getSuitID();
	int order1 = 1;
	int order2 = 1;
	int order3 = 1;
	currentOrderingAux(rank1, rank2, suitID1, suitID2, order1, order2); // compare 1 and 2
	currentOrderingAux(rank1, rank3, suitID1, suitID3, order1, order3); // compare 1 and 3
	currentOrderingAux(rank3, rank2, suitID3, suitID2, order3, order2); // compare 3 and 2
	Ordering[0] = order1;
	Ordering[1] = order2;
	Ordering[2] = order3;
	return;
}

void currentOrderingAux(const int rank1, const int rank2, const int suitID1, const int suitID2,
		int &order1, int &order2) {
	if (rank1 > rank2)
		order1++;
	else if (rank2 > rank1)
		order2++;
	else {// rank1 == rank2
		if (suitID1 > suitID2)
			order1++;
		else
			order2++;
	}
	return;
}

std::vector<Card> magicallySortCards(const std::vector<Card> &CardChoices) {
	std::vector<Card> OrderedCards = chooseHiddenAndFirstCard(CardChoices);
	reorderLast3Cards(OrderedCards);
	return OrderedCards;
}

void show4Cards(const std::vector<Card> &OrderedCards) {
	for (int i=1; i<numCards; i++) {
		std::cout << "Card #" << i << ": ";
		OrderedCards[i].printCard();
		std::cout << "\n";
	}
	return;
}

void convertInput2Ints(const char faceChar, const char suitChar, int &faceVal, int &suitVal) {
	if (faceChar == 'A')
		faceVal = 0;
	else if (faceChar == '2')
		faceVal = 1;
	else if (faceChar == '3')
		faceVal = 2;
	else if (faceChar == '4')
		faceVal = 3;
	else if (faceChar == '5')
		faceVal = 4;
	else if (faceChar == '6')
		faceVal = 5;
	else if (faceChar == '7')
		faceVal = 6;
	else if (faceChar == '8')
		faceVal = 7;
	else if (faceChar == '9')
		faceVal = 8;
	else if (faceChar == 'T')
		faceVal = 9;
	else if (faceChar == 'J')
		faceVal = 10;
	else if (faceChar == 'Q')
		faceVal = 11;
	else if (faceChar == 'K')
		faceVal = 12;
	else
		faceVal = -1; // bad input

	if (suitChar == 'H')
		suitVal = 0;
	else if (suitChar == 'C')
		suitVal = 1;
	else if (suitChar == 'D')
		suitVal = 2;
	else if (suitChar == 'S')
		suitVal = 3;
	else
		suitVal = -1; // bad input
}
