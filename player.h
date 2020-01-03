/* player.h
 * Class declaration for the Player class.
 * Author: Ryan Beckwith (rbeckw02) */

#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "card.h"
#include "vector.h"

using namespace std;

class Player {
  private:
    static const int DISCOUNTS_LENGTH = 5;
    static const int GEMS_LENGTH = 6;
    static const int RESERVED_LENGTH = 3;
    string name;
    // Array of total discounts a player has obtained from purchasing cards
    int discounts[DISCOUNTS_LENGTH];
    // Array of total gems a player has obtained
    int gems[GEMS_LENGTH];
    // Array of Card pointers representing the cards a user has reserved
    Card *reservedCards[RESERVED_LENGTH];
    // Dynamic array representing the cards the user has purchased
    Vector *boughtCards;
  public:
    // Constructors
    Player();
    Player(string name);
    Player(int discounts[DISCOUNTS_LENGTH], int gems[GEMS_LENGTH],
           Card *reservedCards[RESERVED_LENGTH], string name);
    // Destructor
    ~Player();
    // Public member functions
    int totalPrestige();
    int getDiscount(int index);
    int getGemCount(int index);
    int getTotalGems();
    Card *getReserve(int index);
    bool isHandFull();
    void addToHand(Card *currCard);
    void changeGem(int index, int amount);
    void changeDiscount(int index, int amount);
    void addToBoughtCards(Card *currCard);
    void setReserve(int index, Card *currCard);
    string getName();
};

#endif
