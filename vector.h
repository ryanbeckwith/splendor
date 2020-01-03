/* vector.h
 * Written by: Richard Townsend.
 * Edited by: Ryan Beckwith (rbeckw02).
 * Define the Vector class type and the type name that will be used to refer to
 * the Data in a Vector (using a typedef).
 * In this case, the chosen type is the "Card" class. */

#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <string>
#include "card.h"

using namespace std;

// Typedef setting Data to be a pointer to a Card
typedef Card* Data;

class Vector {
  public:
    // Constructor and destructor
    Vector();
    ~Vector();
    // Public member functions
    int size();
    bool empty();
    void push_back(Data value);
    void pop_back();
    Data front();
    Data back();
    Data at(int index);
    void insert(int index, Data value);
    void erase(int index);
    void removeAndReplace(int indexToRemove, int indexOfReplacement);
  private:
    int vCapacity; //Number of locations in the vector
    int vSize;     //Number of elements stored in the vector
    Data *vBuffer;  //Pointer to the vector's dynamically allocated array
    void reserve(int n);
};

#endif
