#ifndef ENGINE_H
#define ENGINE_H

#include "Structure.h"
#include "KeyShedule.h"

class Engine
{
public:

	//Performs byte substitution layer on state
	static void byteSub(State &state, Mode mode);

	//performs inverse byte substiution layer
	static void inverseByteSub(State &state);

	//perform shift rows layer on state
	static void shiftRows(State &state, Mode mode);

	//perform inverse shift rows on state
	static void inverseShiftRows(State &state);

	//shift a single row numPositions 
	static void shiftRow(BYTE * row, int numPositions);

	//perform mix columns layer
	static void mixColumns(State &state, Mode mode);

	//performs multiplication of stateEntry and mixColEntry in GF(2^8) 
	static BYTE mixMultiply(BYTE stateEntry, BYTE mixColEntry);

	//returns the number of rounds relative to key length
	static const int getNumRounds(KeyLength length);

};


#endif