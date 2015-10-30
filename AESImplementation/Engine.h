#ifndef ENGINE_H
#define ENGINE_H

#include "Structure.h"
#include "KeyShedule.h"

class Engine
{
public:

	static void byteSub(State &state, Mode mode);

	static void inverseByteSub(State &state);

	static void shiftRows(State &state, Mode mode);

	static void inverseShiftRows(State &state);

	static void shiftRow(BYTE * row, int numPositions);

	static void mixColumns(State &state, Mode mode);

	static void inverseMixColumns(State &state);

	static BYTE mixMultiply(BYTE stateEntry, BYTE mixColEntry);

	static const int getNumRounds(KeyLength length);

};


#endif