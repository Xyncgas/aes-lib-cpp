#include "stdafx.h"
#include <iostream>
#include "KeyShedule.h"

KeySchedule::KeySchedule() : KeySchedule(NULL, 10) {}

KeySchedule::KeySchedule(const Key key, int numRounds)
{
	numSubkeys	=	numRounds + 1;
	schedule	=	new BYTE[numSubkeys * 16];
	initKeys(key);
}

KeySchedule::~KeySchedule() 
{
	delete[] schedule;
}

void KeySchedule::initKeys(const Key privateKey)
{
	//Init first 16 bytes (first sub key) as private key
	for (int col = 0; col < 4; col++)
	{
		std::vector<BYTE> colBytes	=	Structure::getColumn(privateKey, col);
		schedule[col * 4]			=	colBytes.at(0);
		schedule[col * 4 + 1]		=	colBytes.at(1);
		schedule[col * 4 + 2]		=	colBytes.at(2);
		schedule[col * 4 + 3]		=	colBytes.at(3);
	}

	int keyLen		=	16;
	int front		=	16;
	int i			=	1;
	int schLength	=	numSubkeys * 16;

	while (front < schLength)
	{
		//The next 4 bytes to be added to the key schedule
		BYTE next[4]; 

		//copy previous 4 bytes into next
		for (int j = 0, k = (front - 4); j < 4; j++, k++)
			next[j] = schedule[k];

		//perform the core operations on next and increment i
		if (front % keyLen == 0)
		{
			core(next, i);
			i++;
		}

		//set next 4 bytes of schedule as: next xor with prev 16 bytes 4 byte block
		//increments front
		for (int j = 0; j < 4; j++, front++)
			schedule[front] = schedule[front - keyLen] ^ next[j];
	}
}

//circular left shift the word by 1 byte
void KeySchedule::rotate(BYTE * word)
{
	BYTE temp = word[0];
	for (int i = 0; i < 3; i++)
		word[i] = word[i + 1];
	
	word[3] = temp;
}

void KeySchedule::core(BYTE * word, int i)
{
	//rotate left 1 byte
	rotate(word);

	//apply s box on word
	for (int j = 0; j < 4; j++)
		word[j]	=	Structure::getSboxEntry(word[j]);

	//xor round coefficient to left most byte
	word[0] ^= getRoundCoeff(i);
}


BYTE KeySchedule::getRoundCoeff(int i)
{
	//get rcon entry from lookup using index i - 1
	return Structure::getRconEntry(i);
}

Key * KeySchedule::getKey(int roundNum)
{
	Key key;

	int sIndex = 0;
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			key[col][row] = schedule[sIndex++];

	return (Key *) key;
}

int KeySchedule::getNumSubkeys()
{
	return numSubkeys;
}


