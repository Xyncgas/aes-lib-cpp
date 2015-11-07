#include "stdafx.h"
#include <iostream>
#include "KeyShedule.h"

KeySchedule::KeySchedule() : KeySchedule(NULL, 0) {}

KeySchedule::KeySchedule(const Key key, int numRounds)
{
	initSchedule(key, numRounds);
}

KeySchedule::~KeySchedule() 
{
	delete[] schedule;
}

//initialzes the schedule arr and adds the sub keys
void KeySchedule::initSchedule(const Key key, int numRounds)
{
	if (numRounds > 0)
	{
		//number of sub/round keys in the schedule
		numSubkeys = numRounds + 1; 

		//schedule to hold the subkeys
		schedule = new BYTE[numSubkeys * 16];

		//adds the sub keys to schedule
		initKeys(key);
	}
}

//adds the sub keys generated from private key to the schedule
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

//performs the core key schedule operations on the word
void KeySchedule::core(BYTE * word, int i)
{
	//rotate left 1 byte	
	rotate(word);

	//apply s box on word
	//decrypt sub uses standard sbox
	for (int j = 0; j < 4; j++)
		word[j]	=	Structure::getSboxEntry(word[j], Mode::ENCRYPT);

	

	//xor round coefficient to left most byte
	addRoundCoeff(word[0], i);
}

//gets the round coefficient at index i
//returns the result of xor byte with rc
BYTE KeySchedule::addRoundCoeff(BYTE &byte, int i)
{
	//xor byte with rcon entry at index i
	BYTE result = byte ^ Structure::getRconEntry(i);
	return result;
}

//set the key to the key at round roundNum
void KeySchedule::setRoundKey(int roundNum, Key &key)
{
	int sIndex = (roundNum * 16);
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			key[col][row] = schedule[sIndex++];
}

//makes the passed str into a key
void KeySchedule::makeKey(BYTE *str, Key &key)
{
	Structure::makeState(str, key);
}

//return the number of sub/round keys in schedules
int KeySchedule::getNumSubkeys()
{
	return numSubkeys;
}


