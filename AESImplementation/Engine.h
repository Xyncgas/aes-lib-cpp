#ifndef ENGINE_H
#define ENGINE_H

#include "Structure.h"
#include "KeyShedule.h"


class Engine
{
public:

	static void subLayer(State &state);

	static void shiftRows(State &state);

	static void shiftRow(BYTE * row, int numPositions);

	static void mixColumns(State &state);

	static void keyAddition(State &state, const Key subkey);

	static BYTE mixMultiply(BYTE stateEntry, BYTE mixColEntry);

	static const int getNumRounds(KeyLength length);

};


#endif