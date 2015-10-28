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
			if (mode == Mode::ENCRYPT)
				state[row][col] = Structure::getSboxEntry(state[row][col], mode);

			else if (mode == Mode::DECRYPT)
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
		for (int numPositions = 1, row = 1; numPositions > 0; numPositions++, row++)
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

BYTE Engine::mixMultiply(BYTE stateEntry, BYTE mixColEntry)
{
	if (mixColEntry == 0x01)
		return stateEntry;

	BYTE product = stateEntry << 1;
	
	if (mixColEntry == 0x03)
		product ^= stateEntry;

	if (stateEntry.at(7) == 1)
		product ^= 0x1B;

	return product;
}


void Engine::keyAddition(State &state, const Key subKey)
{
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			state[col][row] ^= subKey[col][row];
}



