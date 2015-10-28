#include "stdafx.h"
#include "AES.h"
#include "Structure.h"
#include "Engine.h"
#include "KeyShedule.h"

using namespace System;
using namespace std;

void AES::encrypt(State &state, Key &privateKey, KeyLength length)
{

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

