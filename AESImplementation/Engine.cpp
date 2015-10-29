#include "stdafx.h"
#include "Engine.h"
#include "KeyShedule.h"
#include "Structure.h"
#include <iostream>


void Engine::byteSub(State &state, Mode mode)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			state[row][col] = Structure::getSboxEntry(state[row][col], mode);
		}
	}
}

void Engine::shiftRows(State &state, Mode mode)
{
	if (mode == Mode::ENCRYPT)
	{
		//shifts rows 1..4 in state 3..1 positions right
		for (int numPositions = 3, row = 1; numPositions > 0; numPositions--, row++)
			shiftRow(state[row], numPositions);
	}

	else if (mode == Mode::DECRYPT)
	{
		//shifts rows 1..4 in state 1..3 positions right
		for (int numPositions = 1, row = 1; numPositions <= 3; numPositions++, row++)
			shiftRow(state[row], numPositions);
	}
}

void Engine::shiftRow(BYTE * row, int numPositions)
{
	BYTE temp[4];
	for (int i = 0; i < 4; i++)
	{
		int index = (i + numPositions) % 4;
		temp[index] = row[i];
	}

	memcpy(row, temp, sizeof(temp));
}

void Engine::mixColumns(State &state, Mode mode)
{
	State temp;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{ 
			temp[col][row]	= mixMultiply(state[0][row], Structure::getMixColEntry(col, 0, mode))
							^ mixMultiply(state[1][row], Structure::getMixColEntry(col, 1, mode))
							^ mixMultiply(state[2][row], Structure::getMixColEntry(col, 2, mode))
							^ mixMultiply(state[3][row], Structure::getMixColEntry(col, 3, mode)); 

		}
	} 

	std::swap(state, temp);
}

BYTE Engine::mixMultiply(BYTE stateEntry, BYTE mixEntry) 
{
	//mix col entry is <= 3
	//don't need to perform peasents algorithm below
	if (mixEntry.to_ulong() <= 3)
	{
		if (mixEntry == 0x01)
			return stateEntry;

		BYTE product = stateEntry << 1;

		if (mixEntry == 0x03)
			product ^= stateEntry;

		if (stateEntry.at(7) == 1)
			product ^= 0x1B;

		return product;
	}

	//For mix col entries > 3 perform peasants algorithm
	else
	{
		if (stateEntry == 0x00 || mixEntry == 0x00)
			return 0x00;

		BYTE a, b, product;
		bool carry;
		a = stateEntry;
		b = mixEntry;

		for (int i = 0; i < 8; i++)
		{
			//if rightmost bit of b is 1, xor product and a
			if (b[0] == 1)
				product ^= a;

			//shift b 1 bit right
			b >>= 1;

			//set carry before shifting a
			//carry is true if leftmost bit of a is set
			carry = (a[7] == 1);

			//shift a 1 bit left
			a <<= 1;

			//carrying bit, xor a with 1b
			if (carry)
				a ^= 0x1b;
		}

		return product;
	}
}


void Engine::keyAddition(State &state, const Key subKey)
{
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			state[col][row] ^= subKey[col][row];
}



