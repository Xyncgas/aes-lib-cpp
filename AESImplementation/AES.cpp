#include "stdafx.h"
#include "AES.h"
#include "Structure.h"
#include "Engine.h"
#include "KeyShedule.h"

using namespace System;
using namespace std;

void AES::encrypt(State &state, Key &privateKey, KeyLength length)
{
	int numRounds	=	getNumRounds(length);
	KeySchedule keySchedule(privateKey, numRounds);

	//Add round 0 key
	//-----------------------------------
	Key roundKey;
	keySchedule.setRoundKey(0, roundKey);
	Engine::keyAddition(state, roundKey);
	//-----------------------------------

	//Perform internals on numRounds
	for (int round = 1; round <= numRounds; round++)
	{
		//Byte substitution layer
		//Confusion component
		Engine::byteSub(state);

		//Difussion components
		//-----------------------------------
		//Shift rows layer
		Engine::shiftRows(state);

		//Mix columns layer
		//Only performed on rounds 1..numRounds-1
		if (round != numRounds)
			Engine::mixColumns(state);
		//-----------------------------------

		//Add round key to state
		keySchedule.setRoundKey(round, roundKey);
		Engine::keyAddition(state, roundKey);
	}
}

void AES::encrypt(BYTE state[16], BYTE privateKey[16], KeyLength length)
{
	State st;
	Key key;
	Structure::makeState(state, st);
	KeySchedule::makeKey(privateKey, key);

	AES::encrypt(st, key, length);
}

void AES::decrypt(BYTE state[16], BYTE privateKey[16], KeyLength length)
{
	State st;
	Key key;
	Structure::makeState(state, st);
	KeySchedule::makeKey(privateKey, key);

	AES::decrypt(st, key, length);
}

void AES::decrypt(State &state, Key &privateKey, KeyLength length)
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