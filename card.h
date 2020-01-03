/* card.h
 * Purpose: Class declaration for the Card class.
 * Author: Ryan Beckwith (rbeckw02). */

#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Card {
  private:
    // How much prestige a given card is worth
    int prestige;
    // The color of the card's discount
    string gemColor;
    // The length of the price array
    static const int PRICE_LENGTH = 5;
    // The various color-corresponding prices of a card
    int price[PRICE_LENGTH];
  public:
    // Constructors
    Card();
    Card(int prestige, string gemColor, int price[PRICE_LENGTH]);
    // Public member functions
    string getGemColor();
    int getPrestige();
    int getPrice(int index);
};

#endif
