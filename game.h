/* game.h
 * Purpose: Class declaration for the Game class.
 * Written by: Richard Townsend.
 * Edited by: Ryan Beckwith (rbeckw02). */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
#include "player.h"
#include "vector.h"
using namespace std;

class Game {
  public:
    //constructor
    Game(string filename, string play_nobles);
    ~Game();

    //main gameplay
    void playGame();

  private:
    // Constructor helpers
    void readFile(string filename);
    void addToVector(Card *c, string row);
    void initializeGrid();
    // Private member functions associated with the various user inputs
    string indexToColor(int index);
    int colorToIndex(string color);
    int colorToIndexGold(string color);
    bool reserve3Gems(Player *currPlayer);
    bool reserve2Gems(Player *currPlayer);
    void getUserGemRequest(string *gems, int *gemCount, int trueNumGems);
    void reserveGems(Player *currPlayer, string *gems, int gemsLength,
                     int numTimes);
    void reserveGold(Player *currPlayer);
    void returnExcessGems(Player *currPlayer);
    bool hasSameColor(string *gems, int gemsLength);
    bool isValidGemSeq(string *gems, int gemsLength, int trueNumGems);
    void getSeq(string *nextCardRow, string *cardPos, int *numStrings);
    bool reserveCard(Player *currPlayer);
    bool isValidSeq(string nextCardRow, string cardPos, int *cardPosIndex);
    bool addToHand(string nextCardRow, int cardPosIndex, Player *currPlayer);
    Vector *rowToDeck(string nextCardRow);
    bool buyCard(Player *currPlayer);
    bool buySpecifiedCard(string nextCardRow, int cardPosIndex,
                          Player *currPlayer);
    bool canBuyCard(Card *currCard, Player *currPlayer);
    void subtractGems(Card *currCard, Player *currPlayer);
    bool buyReservedCard(Player *currPlayer);
    bool buySpecifiedReservedCard(int cardPosIndex, Player *currPlayer);
    void getReserveSeq(string *cardPos, int *numStrings);
    bool isValidReserveSeq(string cardPos, int *cardPosIndex);
    void deleteVectorElements(Vector *currDeck);
    // Helper functions for running the game itself
    void gameLoop(Player *currPlayer, bool *game_over);
    void endOfGame(bool oneTurnRemaining);
    //drawing functions
    void drawBoard();
    void drawNoble(int vpos, int hpos, Card *c);
    void drawPlayer(int vpos, int hpos, Player *p);
    void drawDeck(int vpos, int hpos, string type);
    void drawCard(int vpos, int hpos, Card *c);
    void drawGem(int vpos, int hpos, string color);
    void clearBoard();

    static const int BOARD_HEIGHT = 41;
    static const int BOARD_WIDTH = 120;
    static const int CARD_WIDTH = 13;
    static const int CARD_HEIGHT = 8;
    static const int CARD_ROWS = 3;
    static const int CARD_COLS = 4;
    static const int NUM_NOBLES = 3;
    static const int GEMS = 6;
    static const int STARTING_GEMS = 4;
    static const int GOLD_STARTING_GEMS = 5;
    static const int PRICE_LENGTH = 5;
    static const int MAX_PLAYER_GEMS = 10;

    //nobles activated true/false
    bool play_nobles;

    //cards in play
    Card *grid[CARD_ROWS][CARD_COLS];

    //display
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    string board_color[BOARD_HEIGHT][BOARD_WIDTH];

    // Both players
    Player *player_1;
    Player *player_2;

    // All available gems to purchase
    int gem_bank[GEMS];

    // Dynamic arrays of each card type
    Vector *vendors;
    Vector *transport;
    Vector *mines;
    Vector *nobles;
};

#endif
