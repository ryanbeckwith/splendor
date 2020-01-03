/* card.cpp
 * Class definition for the Card class.
 * Author: Ryan Beckwith (rbeckw02). */

#include "card.h"

// Default constructor for Card objects
Card::Card() {
  gemColor = "";
  prestige = 0;
  for (int i = 0; i < PRICE_LENGTH; i++) {
    price[i] = 0;
  }
}

// Full constructor for Card objects
Card::Card(int prestige, string gemColor, int price[PRICE_LENGTH]) {
  this->prestige = prestige;
  this->gemColor = gemColor;
  for (int i = 0; i < PRICE_LENGTH; i++) {
    this->price[i] = price[i];
  }
}

// Getter for the gemColor field
string Card::getGemColor() {
  return gemColor;
}

// Getter for the prestige field
int Card::getPrestige() {
  return prestige;
}

// Getter for the price of a card at a given index specified by the parameter
int Card::getPrice(int index) {
  if (index >= PRICE_LENGTH) {
    cerr << "ERROR: Specified index was greater than " << PRICE_LENGTH << endl;
    exit(EXIT_FAILURE);
  }
  return price[index];
}
