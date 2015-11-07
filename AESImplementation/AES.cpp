#include "stdafx.h"
#include "AES.h"
#include "Structure.h"
#include "Engine.h"
#include "KeyShedule.h"
#include <iostream>

using namespace System;
using namespace std;

void AES::encrypt(State &plainState, KeySchedule &schedule, Key &privateKey, KeyLength length)
{
	int numRounds	=	getNumRounds(length);
	schedule.initSchedule(privateKey, numRounds);
	encrypt(plainState, schedule, length);
}

void AES::encrypt(State &state, KeySchedule &schedule, KeyLength length)
{

	//Add round 0 key
	//-----------------------------------
	Key roundKey;
	schedule.setRoundKey(0, roundKey);
	Structure::addState(state, roundKey);
	//-----------------------------------


	//Perform internals for numRounds
	int numRounds = getNumRounds(length);
	for (int round = 1; round <= numRounds; round++)
	{
		//Byte substitution layer
		//Confusion component
		Engine::byteSub(state, Mode::ENCRYPT);

		//Difussion components
		//-----------------------------------
		//Shift rows layer
		Engine::shiftRows(state, Mode::ENCRYPT);

		//Mix columns layer
		//Only performed on rounds 1..numRounds-1
		if (round != numRounds)
			Engine::mixColumns(state, Mode::ENCRYPT);
		//-----------------------------------

		//Add round key to state
		schedule.setRoundKey(round, roundKey);
		Structure::addState(state, roundKey);
	}
}

void AES::encrypt(State &state, KeySchedule &schedule, Key &key, BYTE bytes[16], BYTE privateKey[16], KeyLength length)
{
	Structure::makeState(bytes, state);
	KeySchedule::makeKey(privateKey, key);

	AES::encrypt(state, schedule, key, length);
}

void AES::decrypt(State &state, KeySchedule &schedule, Key &key, BYTE bytes[16], BYTE privateKey[16], KeyLength length)
{
	Structure::makeState(bytes, state);
	KeySchedule::makeKey(privateKey, key);

	AES::decrypt(state, schedule, key, length);
}

void AES::decrypt(State &state, KeySchedule &schedule, KeyLength length)
{
	int numRounds = getNumRounds(length);
	Key roundKey;


	//Perform inverse internals for numRounds
	for (int round = numRounds; round > 0; round--)
	{

		//Add round key to state
		schedule.setRoundKey(round, roundKey);
		Structure::addState(state, roundKey);

		//Difussion components
		//---------------------------------------------------
		//Inverse mix columns layer
		//Only performed on rounds 1..numRounds-1
		if (round != numRounds)
			Engine::mixColumns(state, Mode::DECRYPT);

		if (round == numRounds - 1)
		{
			std::cout << "\noutput block:\n";
			Structure::printState(state);
			std::cout << "\n";
		}

		//Inverse shift rows layer
		Engine::shiftRows(state, Mode::DECRYPT);
		//--------------------------------------------------- 


		//Confusion component
		//Inverse byte substitution layer
		Engine::byteSub(state, Mode::DECRYPT);
	}


	//Add round 0 key
	//--------------------------------------------
	schedule.setRoundKey(0, roundKey);
	Structure::addState(state, roundKey);
	//-------------------------------------------- 
}

void AES::decrypt(State &cipherState, KeySchedule &schedule, Key &privateKey, KeyLength length)
{
	int numRounds = getNumRounds(length);
	schedule.initSchedule(privateKey, numRounds);
	decrypt(cipherState, schedule, length);
}

void AES::encrypt_cbc(std::vector<State*> &plainText, Key &privateKey, State &initialVector, KeyLength length)
{
	int numRounds		=	getNumRounds(length);
	std::vector<State*>::size_type numBlocks	=	plainText.size();
	KeySchedule schedule(privateKey, numRounds); 
	
	//Encrypt numBlocks cipher text blocks in cbc mode
	for (std::vector<State*>::size_type i = 0; i < numBlocks; i++)
	{
		//create input block
		//-----------------------------------------------------------
		//xor initial vector on first block
		if (i == 0)
			Structure::addState(*plainText[0], initialVector);

		//xor previous cipher text block
		else
			Structure::addState(*plainText[i], *plainText[i - 1]);
		//-----------------------------------------------------------

		//encrypt input block to make cipher block
		encrypt(*plainText[i], schedule, length);
	}
}

void AES::decrypt_cbc(std::vector<State*> &plainText, Key &privateKey, State &initialVector, KeyLength length)
{
	int numRounds = getNumRounds(length);
	typedef std::vector<State*>::size_type v_size;
	const v_size numBlocks = plainText.size();
	KeySchedule schedule(privateKey, numRounds);

	
	//Store the cipher text before mutation
	//-------------------------------------------------------
	State *prevCiphers = new State[numBlocks];
	for (v_size i = 0; i < numBlocks; i++)
		Structure::copyState(prevCiphers[i], *plainText[i]);
	//-------------------------------------------------------


	//Decrypt numBlocks cipher text blocks in cbc mode
	for (v_size i = 0; i < numBlocks; i++)
	{
		//decrypt input cipher block
		//creates output block i
		decrypt(*plainText[i], schedule, length);

		//create plain text block i
		//-----------------------------------------------------------
		//xor initial vector on first block
		if (i == 0)
			Structure::addState(*plainText[0], initialVector);

		//xor previous cipher text block
		else
			Structure::addState(*plainText[i], prevCiphers[i - 1]);
		//-----------------------------------------------------------
	}

	delete[] prevCiphers;
}

//Return the number of rounds needed for key length
int AES::getNumRounds(KeyLength length)
{
	switch (length)
	{
		//Key: 128bit
		case SMALL:
			return 10;

		//Key: 192bit
		case MEDIUM:
			return 12;

		//Key: 256bit
		case LARGE:
			return 14;

		//Key: undefined
		default:
			return 0;
	}
}