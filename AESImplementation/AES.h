#ifndef AES_ENC_H
#define AES_ENC_H

#include "Structure.h"
#include "KeyShedule.h"

class AES
{
public:
	static void encrypt(State &state, KeySchedule &schedule, Key &privateKey, KeyLength length);

	static void encrypt(State &state, KeySchedule &schedule, KeyLength length);

	static void encrypt(State &state, KeySchedule &schedule, Key &privateKey, BYTE bytes[16], BYTE privateKeyBytes[16], KeyLength length);

	static void encrypt_cbc(std::vector<State*> &plainText, Key &privateKey, State &initialVector, KeyLength length);

	static void decrypt(State &state, KeySchedule &schedule, KeyLength length);

	static void decrypt(State &state, KeySchedule &schedule, Key &privateKey, BYTE bytes[16], BYTE privateKeyBytes[16], KeyLength length);

	static void decrypt(State &state, KeySchedule &schedule, Key &privateKey, KeyLength length);

	static void decrypt_cbc(std::vector<State*> &plainText, Key &privateKey, State &initialVector, KeyLength length);

	static int getNumRounds(KeyLength length);
};

#endif