/* game.cpp
 * Purpose: Class definition for the Game class.
 * Written by: Richard Townsend.
 * Edited by: Ryan Beckwith (rbeckw02). */

#include "game.h"

// Constructor that takes a filename and either "true" or "false" as input
Game::Game(string filename, string play_nobles){
   //include nobles or not
   if(play_nobles == "true"){
        this->play_nobles = true;
   }else{
        this->play_nobles = false;
   }
   //initialize the board
   for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
   }
   // Initialize grid
   for (int i = 0; i < CARD_ROWS; i++) {
     for (int j = 0; j < CARD_COLS; j++) {
       Card c;
       grid[i][j] = &c;
     }
   }
   // Initialize the gem bank
   for (int i = 0; i < GEMS - 1; i++) {
     gem_bank[i] = STARTING_GEMS;
   }
   gem_bank[GEMS - 1] = GOLD_STARTING_GEMS;
   // Read data from the input file, instantiating the other fields
   readFile(filename);
}

// Destructor for the Game class
Game::~Game() {
  // Deletes the elements of each vector, then deletes the vector itself
  if (vendors != nullptr) {
    deleteVectorElements(vendors);
    delete vendors;
  }
  if (transport != nullptr) {
    deleteVectorElements(transport);
    delete transport;
  }
  if (mines != nullptr) {
    deleteVectorElements(mines);
    delete mines;
  }
  if (nobles != nullptr) {
    deleteVectorElements(nobles);
    delete nobles;
  }
  // Deletes both of the player objects
  if (player_1 != nullptr) {
    delete player_1;
  }
  if (player_2 != nullptr) {
    delete player_2;
  }
}

// Deletes the dynamically allocated elements in the vector currDeck
void Game::deleteVectorElements(Vector *currDeck) {
  for (int i = 0; i < currDeck->size(); i++) {
    delete currDeck->at(i);
  }
}

// Adds a given Card object to its respective Vector determined by the value
// of row, a string that can either be m, t, v, or n
void Game::addToVector(Card *c, string row) {
  // Check only for the letters m, t, v, and n
  if (row == "m") {
    mines->push_back(c);
  } else if (row == "t") {
    transport->push_back(c);
  } else if (row == "v") {
    vendors->push_back(c);
  } else if (row == "n") {
    nobles->push_back(c);
  }
}

// Initializes the values of the grid double array by taking the last four
// cards from each Vector field (vendors, transport, and mines) and adding them
// to grid, placing the last card of each Vector in the first column, and so on
void Game::initializeGrid() {
  // Loop through the columns of grid
  for (int j = 0; j < CARD_COLS; j++) {
    // Set the first row to be the vendor row
    grid[0][j] = vendors->at(vendors->size() - 1 - j);
    // Set the second row to be the transport row
    grid[1][j] = transport->at(transport->size() - 1 - j);
    // Set the third row to be the mines row
    grid[2][j] = mines->at(mines->size() - 1 - j);
  }
}

// Instantiates the Player and Vector fields of the Game class, then reads
// input from a file specified in the string parameter and stores it in the
// various vectors, then initializes the grid field
void Game::readFile(string filename) {
  // Instantiate players and vectors
  player_1 = new Player("PLAYER 1");
  player_2 = new Player("PLAYER 2");
  vendors = new Vector();
  transport = new Vector();
  mines = new Vector();
  nobles = new Vector();
  // Create an ifstream object
  ifstream infile(filename);
  string firstLine, nextLine, row, discount;
  int prestige, nextPrice;
  int price[PRICE_LENGTH];
  getline(infile, firstLine);
  istringstream lineStream;
  // Loop until the call to getline() fails
  while (getline(infile, nextLine)) {
    lineStream.str(nextLine);
    lineStream >> row >> prestige >> discount;
    for (int i = 0; i < PRICE_LENGTH; i++) {
      lineStream >> nextPrice;
      price[i] = nextPrice;
    }
    // Add a new card to its respective vector based on the value of row
    addToVector(new Card(prestige, discount, price), row);
    lineStream.clear();
  }
  // Initialize the starting values in grid
  initializeGrid();
}

// Converts an index from 0-5 into a string representing a color
string Game::indexToColor(int index) {
  string COLORS[6] = {"white", "red", "pink", "blue", "green", "yellow"};
  return COLORS[index];
}

// Converts a string representing a color (not gold) to an associated index
int Game::colorToIndex(string color) {
  string COLORS[5] = {"white", "red", "pink", "blue", "green"};
  for (int i = 0; i < 5; i++) {
    if (color == COLORS[i]) {
      return i;
    }
  }
  return -1;
}

// Converts a string representing a color (including gold) to an associated
// index
int Game::colorToIndexGold(string color) {
  string COLORS[6] = {"white", "red", "pink", "blue", "green", "gold"};
  for (int i = 0; i < 6; i++) {
    if (color == COLORS[i]) {
      return i;
    }
  }
  return -1;
}

// Master function that is called to reserve three gems for the current player,
// which is represented by the Player object pointed to by currPlayer
// Returns true if a fully valid sequence was specified by the user,
// false otherwise
bool Game::reserve3Gems(Player *currPlayer) {
  string gems[3];
  int gemCount = 0;
  // Get the user's gem request
  getUserGemRequest(gems, &gemCount, 3);
  // Check to see if the sequence is syntactically valid
  if (isValidGemSeq(gems, gemCount, 3)) {
    // Reserve the specified gems
    reserveGems(currPlayer, gems, gemCount, 1);
    // If the player has more than 10 gems, return any excess gems
    if (currPlayer->getTotalGems() > MAX_PLAYER_GEMS) {
      returnExcessGems(currPlayer);
    }
  } else {
    return false;
  }
  return true;
}

// Master function that is called to reserve two gems for the current player,
// which is represented by the Player object pointed to by currPlayer
// Returns true if a fully valid sequence was specified by the user,
// false otherwise
bool Game::reserve2Gems(Player *currPlayer) {
  string gems[1];
  int gemCount = 0;
  // Get the user's gem request
  getUserGemRequest(gems, &gemCount, 1);
  // Check to see if the sequence is syntactically valid
  if (isValidGemSeq(gems, gemCount, 1)) {
    // Check to see if the number of gems in the bank is not 4
    if (gem_bank[colorToIndex(gems[0])] != 4) {
      cout << " Invalid move (unavailable gems) - enter a valid move: ";
      return false;
    }
    // Reserve the specified gems
    reserveGems(currPlayer, gems, gemCount, 2);
  } else {
    return false;
  }
  // If the player has more than 10 gems, return any excess gems
  if (currPlayer->getTotalGems() > MAX_PLAYER_GEMS) {
    returnExcessGems(currPlayer);
  }
  return true;
}

// Using the stringstream functionality, gets the user's typed request and
// saves the values into gems, a pointer to the first element in an array of
// strings, and gemCount, a pointer to an integer representing the number of
// gems in the array
void Game::getUserGemRequest(string *gems, int *gemCount, int trueNumGems) {
  string line, nextGem;
  *gemCount = 0;
  // Get the next line of input (after the move)
  getline(cin, line);
  // Instantiate and initialize an object of type istringstream
  istringstream lineStream;
  lineStream.str(line);
  // Until the end of the line, read in data into nextGem
  while (lineStream >> nextGem) {
    if (*gemCount > trueNumGems - 1) {
      break;
    }
    // Adjust the values of gems and gemCount
    gems[*gemCount] = nextGem;
    (*gemCount)++;
    lineStream.clear();
  }
}

// Reserves a given sequence of gems specified in the parameter gems for the
// player specified by currPlayer
// gemsLength represents the length of the array of strings pointed to by gems,
// and numTimes indicates the number of times the gems array should be reserved
void Game::reserveGems(Player *currPlayer, string *gems, int gemsLength,
                       int numTimes) {
  int index;
  for (int i = 0; i < numTimes; i++) {
    for (int j = 0; j < gemsLength; j++) {
      index = colorToIndex(gems[j]);
      // Increment the number of gems at the proper index
      currPlayer->changeGem(index, 1);
      gem_bank[index]--;
    }
  }
}

// Returns excess gems to the gem bank, based on the specified player
// This function should not be called unless the current player has a total
// sum of gems greater than or equal to 10
void Game::returnExcessGems(Player *currPlayer) {
  string color;
  for (int i = 0; i <= currPlayer->getTotalGems() - MAX_PLAYER_GEMS; i++) {
    // Clear the board
    clearBoard();
    screen_clear();
    screen_home();
    // Draw the board
    drawBoard();
    // Get the user's input
    cout << " " << currPlayer->getName() << ", you have more than 10 gems! ";
    cout << "Name one gem you would like to return: ";
    getline(cin, color);
    int index = colorToIndexGold(color);
    // Ask again if an improper color was specified
    while (index == -1 || currPlayer->getGemCount(index) <= 0) {
      cout << " Invalid color. Name one gem you would like to return: ";
      getline(cin, color);
      // Since the color to return could be gold, use colorToIndexGold
      index = colorToIndexGold(color);
    }
    // Decrement the number of gems at the current index
    currPlayer->changeGem(index, -1);
    // Increment the number of gems in the bank
    gem_bank[index]++;
  }
}

// Returns true if any two gems in the array pointed to by the parameter gems
// are the same color, false otherwise
bool Game::hasSameColor(string *gems, int gemsLength) {
  for (int i = 0; i < gemsLength - 1; i++) {
    for (int j = i + 1; j < gemsLength; j++) {
      if (gems[i] == gems[j]) {
        return true;
      }
    }
  }
  return false;
}

// Returns true if a given gem sequence is syntactically valid - that is,
// if the user specifies proper colors that do not repeat and can be taken
// from the bank, and returns false otherwise
bool Game::isValidGemSeq(string *gems, int gemsLength, int trueNumGems) {
  // Check for enough colors
  if (gemsLength < trueNumGems) {
    cout << " Invalid move (too few colors) - enter a valid move: ";
    return false;
  }
  // Check to see if there are two of the same colors
  if (hasSameColor(gems, gemsLength)) {
    cout << " Invalid move (duplicate color) - enter a valid move: ";
    return false;
  }
  int index;
  for (int i = 0; i < gemsLength; i++) {
    index = colorToIndex(gems[i]);
    // Check to see if the colors themselves were valid colors
    if (index == -1) {
      cout << " Invalid move (invalid color) - enter a valid move: ";
      return false;
    } else {
      // Check to see if the bank has enough gems for one to be taken
      if (gem_bank[index] <= 0) {
        cout << " Invalid move (unavailable gems) - enter a valid move: ";
        return false;
      }
    }
  }
  return true;
}

// Gets a sequence of card row and position from the user, changing the values
// of nextCardRow, cardPos, and numStrings accordingly
void Game::getSeq(string *nextCardRow, string *cardPos, int *numStrings) {
  string line, nextString;
  *numStrings = 0;
  // Get the next line from the user (after the move)
  getline(cin, line);
  // Instantiate and initialize an object of type istringstream
  istringstream lineStream;
  lineStream.str(line);
  // Loop until the end of the line
  while (lineStream >> nextString) {
    // If more than two arguments are specified, break
    if (*numStrings > 1) {
      break;
    }
    // Set the first argument to be the row
    if (*numStrings == 0) {
      *nextCardRow = nextString;
    }
    // Set the second argument to be the position
    if (*numStrings == 1) {
      *cardPos = nextString;
    }
    (*numStrings)++;
    lineStream.clear();
  }
}

// Master function that is called to reserve a card to the user's hand
// Returns true if a valid reserve sequence was specified, false otherwise
bool Game::reserveCard(Player *currPlayer) {
  string nextCardRow, cardPos;
  int numStrings = 0, cardPosIndex;
  // Gets the reserve sequence from the user
  getSeq(&nextCardRow, &cardPos, &numStrings);
  // Checks to see if the sequence was syntactically valid
  if (isValidSeq(nextCardRow, cardPos, &cardPosIndex)) {
    // If the addToHand function fails, return false
    if (!addToHand(nextCardRow, cardPosIndex, currPlayer)) {
      return false;
    }
    // Reserve a gold to the player's bank
    reserveGold(currPlayer);
  } else {
    return false;
  }
  return true;
}

// Returns true if a given gem sequence is syntactically valid - that is,
// if the user specifies a proper row and position, or false otherwise
// The integer version of cardPos is saved into cardPosIndex, which uses the
// stoi() function to convert from a string to an integer
bool Game::isValidSeq(string nextCardRow, string cardPos, int *cardPosIndex) {
  string validRows[3] = {"m", "t", "v"};
  bool validNextCardRow = false, validCardPos = false;
  string digits = "";
  for (int i = 0; i < 3; i++) {
    if (validRows[i] == nextCardRow) {
      validNextCardRow = true;
      break;
    }
  }
  if (!validNextCardRow) {
    cout << " Invalid move (invalid card type) - enter a valid move: ";
    return false;
  }
  for (int i = 0; i < int(cardPos.length()); i++) {
    if (isdigit(cardPos[i])) {
      digits += cardPos[i];
      validCardPos = true;
    }
  }
  if (!validCardPos) {
    cout << " Invalid move (invalid card column) - enter a valid move: ";
    return false;
  }
  int tempCardPosIndex = stoi(digits) - 1;
  if (tempCardPosIndex < 0 || tempCardPosIndex > 3) {
    cout << " Invalid move (invalid card column) - enter a valid move: ";
    return false;
  }
  *cardPosIndex = tempCardPosIndex;
  return true;
}

// Adds a given card specefied by a row and position to the current player's
// hand of reserved cards, so long as their hand is not already full
// Returns true if the card was successfully added, false if the hand was full
bool Game::addToHand(string nextCardRow, int cardPosIndex,
                     Player *currPlayer) {
  // Check to see if the hand is full
  if (currPlayer->isHandFull()) {
    cout << " Invalid move (reserve is already full) - enter a valid move: ";
    return false;
  }
  // Get the current deck based on the value of nextCardRow
  Vector *currDeck = rowToDeck(nextCardRow);
  if (currDeck != nullptr) {
    int currIndex = currDeck->size() - cardPosIndex - 1;
    Card *currCard = currDeck->at(currIndex);
    if (currCard != nullptr) {
      // Find the index of the next card in the deck to replace the card that
      // was just reserved
      int indexOfReplacement = currDeck->size() - 5;
      if (currDeck->at(indexOfReplacement) != nullptr) {
        // Add the card to the player's hand
        currPlayer->addToHand(currCard);
        // Remove and replace the card from the current deck
        currDeck->removeAndReplace(currIndex, indexOfReplacement);
        // Re-initialize the values of grid
        initializeGrid();
      }
    }
  }
  return true;
}

// Converts a given string into a pointer to a vector that represents one of
// the rows of the game (mines, transport, or vendors)
// If an invalid string is given, a null pointer is returned
Vector *Game::rowToDeck(string nextCardRow) {
  if (nextCardRow == "m") {
    return mines;
  } else if (nextCardRow == "t") {
    return transport;
  } else if (nextCardRow == "v") {
    return vendors;
  }
  return nullptr;
}

// Reserves a gold gem to the player's bank, if any are available
void Game::reserveGold(Player *currPlayer) {
  // Check to see if there are gold gems to reserve
  if (gem_bank[5] > 0) {
    currPlayer->changeGem(5, 1);
    gem_bank[5]--;
  }
  // If receiving a gold gem causes the player to go above 10 gems, return
  // any excess gems
  if (currPlayer->getTotalGems() > MAX_PLAYER_GEMS) {
    returnExcessGems(currPlayer);
  }
}

// Master function called for when a player attempts to buy a card
// Returns true if a valid buy sequence is specified, false otherwise
bool Game::buyCard(Player *currPlayer) {
  string nextCardRow, cardPos;
  int numStrings = 0, cardPosIndex;
  // Get the buy sequence from the user
  getSeq(&nextCardRow, &cardPos, &numStrings);
  // Check to see if the sequence is a valid buy sequence
  if (isValidSeq(nextCardRow, cardPos, &cardPosIndex)) {
    // If the attempted buy fails, return false - otherwise, the buy will
    // execute without causing this function to return false
    if (!buySpecifiedCard(nextCardRow, cardPosIndex, currPlayer)) {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

// Buy a specified card based on the row and position specified by the first
// two parameters for the current player, returning true if the card can be
// bought successfully and false otherwise
bool Game::buySpecifiedCard(string nextCardRow, int cardPosIndex,
                            Player *currPlayer) {
  // Get the current deck from nextCardRow
  Vector *currDeck = rowToDeck(nextCardRow);
  if (currDeck != nullptr) {
    // Find the card at the proper index that corresponds to the card that the
    // user wants to buy
    int currIndex = currDeck->size() - cardPosIndex - 1;
    Card *currCard = currDeck->at(currIndex);
    if (currCard != nullptr) {
      // If the card cannot be bought, terminate immediately by returning false
      if (!canBuyCard(currCard, currPlayer)) {
        cout << " Invalid move (cannot afford card) - enter a valid move: ";
        return false;
      }
      // Find the index of the card to replace the bought card
      int indexOfReplacement = currDeck->size() - 5;
      if (currDeck->at(indexOfReplacement) != nullptr) {
        // Buy the card using discounts, then colored gems, then gold gems
        subtractGems(currCard, currPlayer);
        // Replace the card with the new card from the deck
        currDeck->removeAndReplace(currIndex, indexOfReplacement);
        // Re-initialize the grid
        initializeGrid();
      }
    }
  }
  return true;
}

// Checks to see if the current player can purchase the card specified by the
// parameter currCard, return true if the card can be purchased and false
// otherwise
bool Game::canBuyCard(Card *currCard, Player *currPlayer) {
  int currGoldGems = currPlayer->getGemCount(5);
  int goldGemsToSubtract = 0;
  for (int i = 0; i < 5; i++) {
    int currPrice = currCard->getPrice(i);
    // If, at any point, the current price at a color exceeds the sum of the
    // player's discounts, gems for that color, and gold gems, return false
    if (currPrice > currPlayer->getDiscount(i) + currPlayer->getGemCount(i) +
        currGoldGems) {
      return false;
    } else {
      // Check first to see if a discount will suffice in buying the card
      if (currPrice - currPlayer->getDiscount(i) <= 0) {
        continue;
      // Check next to see if a discount and current gems will suffice
      } else if (currPrice - (currPlayer->getDiscount(i) +
                 currPlayer->getGemCount(i)) <= 0) {
        continue;
      // Check last to see if all sources can be used to buy the card, and if
      // so, detract the number of gold gems required to do so
      } else if (currPrice - (currPlayer->getDiscount(i) +
                 currPlayer->getGemCount(i) + currGoldGems) <= 0) {
        goldGemsToSubtract = currPrice - (currPlayer->getDiscount(i) +
                             currPlayer->getGemCount(i));
        currGoldGems -= goldGemsToSubtract;
      }
    }
  }
  return true;
}

// Subtracts the number of necessary gems from the current player in order to
// buy the card specified by currCard, first with discounts, then colored gems,
// and lastly with gold gems
void Game::subtractGems(Card *currCard, Player *currPlayer) {
  int currGemsToSubtract = 0, goldGemsToSubtract = 0;
  for (int i = 0; i < 5; i++) {
    int currPrice = currCard->getPrice(i);
    // If the current color price can be satisfied by the discount, continue
    if (currPrice - currPlayer->getDiscount(i) <= 0) {
      continue;
    // Otherwise, if the current color price can be satisfied by a discount and
    // the current colored gem, subtract the amount of gems required to do so
    } else if (currPrice - (currPlayer->getDiscount(i) +
               currPlayer->getGemCount(i)) <= 0) {
      currGemsToSubtract = currPrice - currPlayer->getDiscount(i);
      currPlayer->changeGem(i, -(currGemsToSubtract));
      gem_bank[i] += currGemsToSubtract;
      // Otherwise, if the current color price can be satisfied with all
      // sources of gems, subtract all colored gems, then the required amount
      // of gold gems
    } else if (currPrice - (currPlayer->getDiscount(i) +
               currPlayer->getGemCount(i) + currPlayer->getGemCount(5)) <= 0) {
      currGemsToSubtract = currPlayer->getGemCount(i);
      currPlayer->changeGem(i, -(currGemsToSubtract));
      gem_bank[i] += currGemsToSubtract;
      goldGemsToSubtract = currPrice - (currPlayer->getDiscount(i) +
                                        currGemsToSubtract);
      currPlayer->changeGem(5, -(goldGemsToSubtract));
      gem_bank[5] += goldGemsToSubtract;
    }
  }
  currPlayer->addToBoughtCards(currCard);
  int index = colorToIndex(currCard->getGemColor());
  currPlayer->changeDiscount(index, 1);
}

// Master function that takes care of buying a card from the player's reserve
bool Game::buyReservedCard(Player *currPlayer) {
  string cardPos;
  int numStrings = 0, cardPosIndex;
  // Get the reserve sequence from the user
  getReserveSeq(&cardPos, &numStrings);
  // Check to see if the reserve sequence is a valid sequence
  if (isValidReserveSeq(cardPos, &cardPosIndex)) {
    // If the card cannot be bought successfully, return false
    if (!buySpecifiedReservedCard(cardPosIndex, currPlayer)) {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

// Returns true if the card specified by cardPosIndex can be bought
// successfully, and buys the card. If it cannot be bought successfully, this
// function returns false before attempting to buy the card
bool Game::buySpecifiedReservedCard(int cardPosIndex, Player *currPlayer) {
  Card *currCard = currPlayer->getReserve(cardPosIndex);
  if (currCard != nullptr) {
    // If the card cannot be bought, return false
    if (!canBuyCard(currCard, currPlayer)) {
      cout << " Invalid move (cannot afford card) - enter a valid move: ";
      return false;
    }
    // Buy the card using discounts first, then colored gems, then gold gems
    subtractGems(currCard, currPlayer);
    // Set the card in the reserve to a null pointer
    currPlayer->setReserve(cardPosIndex, nullptr);
    // Rearrange all other cards to remove the empty space from the beginning
    // or middle
    for (int i = cardPosIndex; i < 2; i++) {
      Card *temp = currPlayer->getReserve(i);
      currPlayer->setReserve(i, currPlayer->getReserve(i + 1));
      currPlayer->setReserve(i + 1, temp);
    }
  } else {
    cout << " Invalid move (no card at that index) - enter a valid move: ";
    return false;
  }
  return true;
}

// Gets the specified reserve sequence from the user using the stringstream
// framework
void Game::getReserveSeq(string *cardPos, int *numStrings) {
  string line, nextString;
  *numStrings = 0;
  // Gets the next line from the user (excluding the move)
  getline(cin, line);
  // Instantiate and initialize an object of type istringstream
  istringstream lineStream;
  lineStream.str(line);
  // Loop until the end of the line, storing values in nextString
  while (lineStream >> nextString) {
    // If more than one argument is specified, skip them
    if (*numStrings > 0) {
      break;
    }
    // Set the first argument to be the postion
    if (*numStrings == 0) {
      *cardPos = nextString;
    }
    (*numStrings)++;
    lineStream.clear();
  }
}

// Returns true if the given position is valid for buying a reserved card,
// checking to see if a card exists at the specified position and if the
// specified position contains a valid number, false otherwise
// The value of cardPosIndex is adjusted if a valid number is specified in
// cardPos
bool Game::isValidReserveSeq(string cardPos, int *cardPosIndex) {
  bool validCardPos = false;
  string digits = "";
  // Check to see if cardPos contained any integers
  for (int i = 0; i < int(cardPos.length()); i++) {
    if (isdigit(cardPos[i])) {
      digits += cardPos[i];
      validCardPos = true;
    }
  }
  // If there were no integers, return false
  if (!validCardPos) {
    cout << " Invalid move (no card at that index) - enter a valid move: ";
    return false;
  }
  // Convert the digits from cardPos into an integer using the stoi() function
  int tempCardPosIndex = stoi(digits) - 1;
  // Check to see if the index is not within the correct range
  if (tempCardPosIndex < 0 || tempCardPosIndex > 2) {
    cout << " Invalid move (no card at that index) - enter a valid move: ";
    return false;
  }
  // Adjust the value at the address of cardPosIndex
  *cardPosIndex = tempCardPosIndex;
  return true;
}

// Represents the looping structure that prompts the user for a valid move
// until one is given
// The parameters are the current player and a pointer to a boolean that
// represents whether or not the game should be over
void Game::gameLoop(Player *currPlayer, bool *game_over) {
  bool valid_move = false;
  string move;
  while (!valid_move) {
    cin >> move;
    // Quit the function if either user types "q"
    if (move == "q") {
      *game_over = true;
      break;
    } else if (move == "p3") {
      valid_move = reserve3Gems(currPlayer);
    } else if (move == "p2") {
      valid_move = reserve2Gems(currPlayer);
    } else if (move == "r") {
      valid_move = reserveCard(currPlayer);
    } else if (move == "b") {
      valid_move = buyCard(currPlayer);
    } else if (move == "br") {
      valid_move = buyReservedCard(currPlayer);
    } else {
      cout << " Invalid move - enter a valid move: ";
      valid_move = false;
    }
  }
  // At the end of a turn, reset valid_move to false
  valid_move = false;
}

// Checks to see which player won at the end of the game
void Game::endOfGame(bool oneTurnRemaining) {
  if (oneTurnRemaining) {
    if (player_1->getTotalGems() > player_2->getTotalGems()) {
      cout << " Congratulations Player 1: YOU WIN!!!!" << endl;
    } else {
      cout << " Congratulations Player 2: YOU WIN!!!!" << endl;
    }
  }
}

//main game loop
void Game::playGame(){
    screen_bg("black");
    screen_fg("white");
    bool player1_turn = true, game_over = false, oneTurnRemaining = false;
    Player *currPlayer = player_1;
    while(!game_over){
        //reset the screen
        clearBoard();
        screen_clear();
        screen_home();
        //draw the board
        drawBoard();
        if (player1_turn) {
          currPlayer = player_1;
          cout << " PLAYER 1, enter your next move: ";
        } else {
          currPlayer = player_2;
          cout << " PLAYER 2, enter your next move: ";
        }
        gameLoop(currPlayer, &game_over);
        if (oneTurnRemaining) {
          break;
        }
        if (currPlayer->totalPrestige() >= 15) {
          oneTurnRemaining = true;
        }
        player1_turn = !player1_turn;
    }
    endOfGame(oneTurnRemaining);
    screen_bg("white");
    screen_fg("black");
}

//draw all of the elements onto the game board
void Game::drawBoard(){
    int vpos = 0;
    int hpos = 1;

    //draw the card decks
    if(!vendors->empty()){
        drawDeck(vpos, hpos, "Vendors");
    }
    vpos += CARD_HEIGHT + 1;
    if(!transport->empty()){
        drawDeck(vpos, hpos, "Transport");
    }
    vpos += CARD_HEIGHT + 1;
    if(!mines->empty()){
        drawDeck(vpos, hpos, "Mines");
    }

    //draw the in play cards
    vpos = 0;
    hpos = CARD_WIDTH + 2;
    for(int i = 0; i < CARD_ROWS; i++){
        for(int j = 0; j < CARD_COLS; j++){
            if(grid[i][j] != nullptr){
                drawCard(vpos, hpos, grid[i][j]);
            }
            hpos += CARD_WIDTH + 1;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = CARD_WIDTH + 2;
    }

    //draw the gems
    int gemOffset = ((CARD_WIDTH + 1) * 5) + 3;
    vpos = 1;
    hpos = gemOffset;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            int index = (i * 2) + j;

            if(gem_bank[index] > 0){
                board[vpos][hpos + 5] = gem_bank[index] + 48;
                board_color[vpos][hpos + 5] = indexToColor(index);
                drawGem(vpos + 1, hpos, indexToColor(index));
            }
            hpos += CARD_WIDTH;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = gemOffset;
    }

    //draw players
    drawPlayer(vpos + 1, 1, player_1);
    drawPlayer(vpos + 1, ((CARD_WIDTH + 1) * 4), player_2);

    //draw nobles
    if(play_nobles){
        hpos = ((CARD_WIDTH + 1) * 7) + 2;
        vpos = 2;
        for(int i = 0; i < NUM_NOBLES; i++){
            if(nobles->at(i) != nullptr){
                drawNoble(vpos, hpos, nobles->at(i));
            }
            vpos += CARD_HEIGHT + 1;
        }
    }

    //print out the finished board
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board_color[i][j] != "white"){
                screen_fg(board_color[i][j]);
            }
            cout << board[i][j];
            screen_fg("white");
        }
        cout << endl;
   }
   cout << endl;
}

//draws a noble at the specified position of the board
void Game::drawNoble(int vpos, int hpos, Card *c){
    string top    = "|\\  /*\\  /|";
    string second = "| \\/   \\/ |";
    string blank  = "N         N";
    string bottom = "~~~~~~~~~~~";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < 5; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + 5][hpos + i] = bottom[i];
    }

    //draw the prices
    vpos += 2;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 4] = indexToColor(i);
            board_color[vpos][hpos + 5] = indexToColor(i);
            board_color[vpos][hpos + 6] = indexToColor(i);
            board[vpos][hpos + 4] = '|';
            board[vpos][hpos + 5] = price + 48;
            board[vpos][hpos + 6] = '|';
            vpos++;
        }
    }
}

//draws a player at the specified position of the board
void Game::drawPlayer(int vpos, int hpos, Player *p){
    //player string
    string prestige = to_string(p->totalPrestige());
    string player_s;
    if(hpos < 10){
        player_s = " PLAYER 1: " + prestige + " prestige point(s)";
    }else{
        player_s = " PLAYER 2: " + prestige + " prestige point(s)";
    }
    int length = player_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = player_s[i];
    }

    //bonus string
    vpos++;
    string bonus_s = "Discounts: ";
    length = bonus_s.length();
    for(int i = 0; i < GEMS-1; i++){
        int bonus = p->getDiscount(i);
        bonus_s += "|" + to_string(bonus) + "| ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
        if(bonus >= 10){
            length += 1;
        }
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = bonus_s[i];
    }

    //gem string
    vpos++;
    string gems_s = "     Gems: ";
    length = gems_s.length();
    for(int i = 0; i < GEMS; i++){
        int gems = p->getGemCount(i);
        gems_s += "(" + to_string(gems) + ") ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = gems_s[i];
    }

    //reserve string
    vpos++;
    string reserve_s = " Reserved:";
    length = reserve_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = reserve_s[i];
    }

    hpos += reserve_s.length();
    for(int i = 0; i < 3; i++){
        Card *c = p->getReserve(i);
        if(c == nullptr){
            drawDeck(vpos, hpos, "");
        }else{
            drawCard(vpos, hpos, c);
        }
        hpos += CARD_WIDTH + 1;
    }
}

//draws a card deck at the specified position of the board
void Game::drawDeck(int vpos, int hpos, string type){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];
        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    int start_pos = hpos + (blank.length() / 2) - (type.length() / 2);
    length = type.length();
    for(int i = 0; i < length; i++){
        board[vpos + (CARD_HEIGHT/2)][start_pos + i] = type[i];
    }
}

//draws a gem at the specified position of the board
void Game::drawGem(int vpos, int hpos, string color){
    string gem_image[6];
    gem_image[0] = "  _______  ";
    gem_image[1] = ".'_/_|_\\_'.";
    gem_image[2] = "\\`\\  |  /`/";
    gem_image[3] = " `\\  | //' ";
    gem_image[4] = "   `\\|/`   ";
    gem_image[5] = "     `     ";

    for(int i = 0; i < GEMS; i++){
        int length = gem_image[i].length();
        for(int j = 0; j < length; j++){
            board[vpos + i][hpos + j] = gem_image[i][j];
            board_color[vpos + i][hpos + j] = color;
        }
    }
}

//draws a card at the specified position of the board
void Game::drawCard(int vpos, int hpos, Card *c){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];

        if(i == 8 || i == 9){
            board_color[vpos + 1][hpos + i] = c->getGemColor();
        }
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    //draw the prestige number and gem
    board[vpos + 2][hpos + 3] = c->getPrestige() + 48;
    board_color[vpos + 2][hpos + 8] = c->getGemColor();
    board_color[vpos + 2][hpos + 9] = c->getGemColor();
    board[vpos + 2][hpos + 8] = '\\';
    board[vpos + 2][hpos + 9] = '/';
    board_color[vpos + 1][hpos + 8] = c->getGemColor();
    board_color[vpos + 1][hpos + 9] = c->getGemColor();
    board[vpos + 1][hpos + 8] = '_';
    board[vpos + 1][hpos + 9] = '_';

    //draw the prices
    vpos += 4;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 2] = indexToColor(i);
            board_color[vpos][hpos + 3] = indexToColor(i);
            board_color[vpos][hpos + 4] = indexToColor(i);
            board[vpos][hpos + 2] = '(';
            board[vpos][hpos + 3] = price + 48;
            board[vpos][hpos + 4] = ')';
            vpos++;
        }
    }

}

//clear the board display
void Game::clearBoard(){
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
   }
}
