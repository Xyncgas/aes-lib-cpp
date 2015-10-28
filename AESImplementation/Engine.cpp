#include "stdafx.h"
#include "Engine.h"
#include "KeyShedule.h"
#include "Structure.h"
#include <iostream>

const int Engine::getNumRounds(KeyLength length)
{
	switch (length)
	{
		case SMALL:
			return 10;
			
		case MEDIUM:
			return 12;

		case LARGE:
			return 14;

		default:
			return 0;
	}
}

void Engine::subLayer(State &state)
{
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			state[row][col] = Structure::getSboxEntry(state[row][col]);
}

void Engine::shiftRows(State &state)
{
	//second row: shift right 3 bytes
	shiftRow(state[1], 3);

	//third row: shift right 2 bytes
	shiftRow(state[2], 2);

	//fourth row: shift right 1 bytes
	shiftRow(state[3], 1);
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

void Engine::mixColumns(State &state)
{
	State temp;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{ 
			temp[col][row]	= mixMultiply(state[0][row], Structure::getMixColEntry(col, 0))
							^ mixMultiply(state[1][row], Structure::getMixColEntry(col, 1))
							^ mixMultiply(state[2][row], Structure::getMixColEntry(col, 2))
							^ mixMultiply(state[3][row], Structure::getMixColEntry(col, 3)); 
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



