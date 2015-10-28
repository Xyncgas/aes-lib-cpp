#include "stdafx.h"
#include "AES.h"
#include "Structure.h"
#include "Engine.h"
#include "KeyShedule.h"

using namespace System;
using namespace std;

void AES::encrypt(State &plainState, Key &privateKey, KeyLength length)
{
	int numRounds	=	getNumRounds(length);
	KeySchedule keySchedule(privateKey, numRounds);

	//Add round 0 key
	//-----------------------------------
	Key roundKey;
	keySchedule.setRoundKey(0, roundKey);
	Engine::keyAddition(plainState, roundKey);
	//-----------------------------------

	//Perform internals on numRounds
	for (int round = 1; round <= numRounds; round++)
	{
		//Byte substitution layer
		//Confusion component
		Engine::byteSub(plainState, Mode::ENCRYPT);

		//Difussion components
		//-----------------------------------
		//Shift rows layer
		Engine::shiftRows(plainState, Mode::ENCRYPT);

		//Mix columns layer
		//Only performed on rounds 1..numRounds-1
		if (round != numRounds)
			Engine::mixColumns(plainState, Mode::ENCRYPT);
		//-----------------------------------

		//Add round key to state
		keySchedule.setRoundKey(round, roundKey);
		Engine::keyAddition(plainState, roundKey);
	}
}

void AES::encrypt(State &state, BYTE bytes[16], BYTE privateKey[16], KeyLength length)
{
	Key key;
	Structure::makeState(bytes, state);
	KeySchedule::makeKey(privateKey, key);

	AES::encrypt(state, key, length);
}

void AES::decrypt(State &state, BYTE bytes[16], BYTE privateKey[16], KeyLength length)
{
	Key key;
	Structure::makeState(bytes, state);
	KeySchedule::makeKey(privateKey, key);

	AES::decrypt(state, key, length);
}

void AES::decrypt(State &cipherState, Key &privateKey, KeyLength length)
{

}

int AES::getNumRounds(KeyLength length)
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