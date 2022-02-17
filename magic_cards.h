/*
 * magic_cards.h
 *
 *  Created on: Feb 14, 2022
 *      Author: laurentboulet
 */

#ifndef MAGIC_CARDS_H_
#define MAGIC_CARDS_H_

#include <vector>
#include <array>
#include "cards.h"

bool checkCardChoice (int faceVal, int suitVal);
bool checkForDuplicates(int faceVal, int suitVal, std::vector<Card> &CardChoices);
std::vector<Card> chooseHiddenAndFirstCard(const std::vector<Card> &CardChoices);
void findCurrentOrderingOfLast3(const std::vector<Card> &OrderedCards, std::array<int, 3> &Ordering);
void currentOrderingAux(const int rank1, const int rank2, const int suitID1, const int suitID2,
		int &order1, int &order2);
void reorderLast3Cards(std::vector<Card> &OrderedCards);
void swapToScheme(std::vector<Card> &OrderedCards, const int schemeID, std::array<int, 3> &IndexSML);
void swapToSchemeAux(std::vector<Card> &OrderedCards, std::array<int, 3> &IndexSML, std::array<int, 3> DesiredIndices);
template<typename T>
void swap(T &a, T &b);
std::vector<Card> magicallySortCards(const std::vector<Card> &CardChoices);
void show4Cards(const std::vector<Card> &OrderedCards);
void convertInput2Ints(const char faceChar, const char suitChar, int &faceVal, int &suitVal);

#endif /* MAGIC_CARDS_H_ */
