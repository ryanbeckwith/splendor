/* player.cpp
 * Class definition for the Player class.
 * Author: Ryan Beckwith (rbeckw02) */

#include "player.h"

// Default constructor for Player objects
// Note that this leaves all values of reservedCards as null pointers
Player::Player() {
  for (int i = 0; i < DISCOUNTS_LENGTH; i++) {
    discounts[i] = 0;
  }
  for (int i = 0; i < GEMS_LENGTH; i++) {
    gems[i] = 0;
  }
  for (int i = 0; i < RESERVED_LENGTH; i++) {
    reservedCards[i] = nullptr;
  }
  boughtCards = new Vector();
  name = "";
}

// Destructor for Player objects
Player::~Player() {
  // Delete all elements of boughtCards, then boughtCards itself
  if (boughtCards != nullptr) {
    for (int i = 0; i < boughtCards->size(); i++) {
      if (boughtCards->at(i) != nullptr) {
        delete boughtCards->at(i);
      }
    }
    delete boughtCards;
  }
  // Delete all remaining cards in reservedCards
  for (int i = 0; i < RESERVED_LENGTH; i++) {
    if (reservedCards[i] != nullptr) {
      delete reservedCards[i];
    }
  }
}

// Default constructor for Player objects
// Note that this leaves all values of reservedCards as null pointers
Player::Player(string name) {
  for (int i = 0; i < DISCOUNTS_LENGTH; i++) {
    discounts[i] = 0;
  }
  for (int i = 0; i < GEMS_LENGTH; i++) {
    gems[i] = 0;
  }
  for (int i = 0; i < RESERVED_LENGTH; i++) {
    reservedCards[i] = nullptr;
  }
  boughtCards = new Vector();
  this->name = name;
}

// Full constructor for Player objects
Player::Player(int discounts[DISCOUNTS_LENGTH], int gems[GEMS_LENGTH],
       Card *reservedCards[RESERVED_LENGTH], string name) {
  for (int i = 0; i < DISCOUNTS_LENGTH; i++) {
    this->discounts[i] = discounts[i];
  }
  for (int i = 0; i < GEMS_LENGTH; i++) {
    this->gems[i] = gems[i];
  }
  for (int i = 0; i < RESERVED_LENGTH; i++) {
    this->reservedCards[i] = reservedCards[i];
  }
  boughtCards = new Vector();
  this->name = name;
}

// Sums up the total prestige that a user has obtained from purchasing cards
// by looping through boughtCards
int Player::totalPrestige() {
  int total = 0;
  // Loop through boughtCards
  for (int i = 0; i < boughtCards->size(); i++) {
    total += (boughtCards->at(i))->getPrestige();
  }
  return total;
}

// Returns the quantity of discounts for a certain index (color) associated
// with a specific player
int Player::getDiscount(int index) {
  if (index >= DISCOUNTS_LENGTH) {
    cerr << "ERROR: Specified index was greater than or equal to ";
    cerr << DISCOUNTS_LENGTH << "." << endl;
    exit(EXIT_FAILURE);
  }
  return discounts[index];
}

// Returns the quantity of gems for a certain index (color) associated with a
// specific player
int Player::getGemCount(int index) {
  if (index >= GEMS_LENGTH) {
    cerr << "ERROR: Specified index was greater than or equal to ";
    cerr << GEMS_LENGTH << "." << endl;
    exit(EXIT_FAILURE);
  }
  return gems[index];
}

int Player::getTotalGems() {
  int sum = 0;
  for (int i = 0; i < GEMS_LENGTH; i++) {
    sum += getGemCount(i);
  }
  return sum;
}

// Returns a pointer to a Card object that represents a reserved card at a
// given index in the reservedCards array
Card *Player::getReserve(int index) {
  if (index >= RESERVED_LENGTH) {
    cerr << "ERROR: Specified index was greater than or equal to ";
    cerr << RESERVED_LENGTH << "." << endl;
    exit(EXIT_FAILURE);
  }
  return reservedCards[index];
}

// Returns true if a player's hand of reserved cards is full
bool Player::isHandFull() {
  bool isFull = true;
  for (int i = 0; i < RESERVED_LENGTH; i++) {
    if (getReserve(i) == nullptr) {
      isFull = false;
      break;
    }
  }
  return isFull;
}

// Adds a card to the player's hand of reserved cards
void Player::addToHand(Card *currCard) {
  for (int i = 0; i < RESERVED_LENGTH; i++) {
    if (getReserve(i) == nullptr) {
      reservedCards[i] = currCard;
      return;
    }
  }
}

// Changes the number of gems at the specified index by the specified amount
void Player::changeGem(int index, int amount) {
  gems[index] += amount;
}

// Changes the discount at the specified index by the specified amount
void Player::changeDiscount(int index, int amount) {
  discounts[index] += amount;
}

// Adds a card to the end of the boughtCards vector
void Player::addToBoughtCards(Card *currCard) {
  boughtCards->push_back(currCard);
}

// Reserves a card by changing the value of a specified index in reservedCards
// to be the value of currCard
void Player::setReserve(int index, Card *currCard) {
  reservedCards[index] = currCard;
}

// Returns the name of the player
string Player::getName() {
  return name;
}
