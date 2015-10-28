#ifndef AES_ENC_H
#define AES_ENC_H

#include "Structure.h"
#include "KeyShedule.h"

class AES
{
public:
	static void encrypt(State &state, Key &privateKey, KeyLength length);

	static void encrypt(BYTE state[16], BYTE privateKey[16], KeyLength length);

	static void decrypt(BYTE state[16], BYTE privateKey[16], KeyLength length);

	static void decrypt(State &state, Key &privateKey, KeyLength length);
};

#endif