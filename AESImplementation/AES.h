#ifndef AES_ENC_H
#define AES_ENC_H

#include "Structure.h"
#include "KeyShedule.h"

class AES
{
public:
	//encrypts state block, where schedule is not init 
	static void encrypt(State &state, KeySchedule &schedule, Key &privateKey, KeyLength length);

	//encrypts the state block with an init schedule
	//is the core encrypt function
	static void encrypt(State &state, KeySchedule &schedule, KeyLength length);

	//encrypts the passed plain text bytes and key bytes
	static void encrypt(State &state, KeySchedule &schedule, Key &privateKey, BYTE bytes[16], BYTE privateKeyBytes[16], KeyLength length);

	//encrypt the states in CBC mode
	static void encrypt_cbc(std::vector<State*> &plainText, Key &privateKey, State &initialVector, KeyLength length);

	//decrypts the state block where schedule is init
	//is the core decrypt function
	static void decrypt(State &state, KeySchedule &schedule, KeyLength length);

	//decrypts the plain text bytes and private key bytes
	static void decrypt(State &state, KeySchedule &schedule, Key &privateKey, BYTE bytes[16], BYTE privateKeyBytes[16], KeyLength length);

	//decrypts state block, where schedule is not init 
	static void decrypt(State &state, KeySchedule &schedule, Key &privateKey, KeyLength length);

	//decrypts the states in CBC mode
	static void decrypt_cbc(std::vector<State*> &plainText, Key &privateKey, State &initialVector, KeyLength length);

	//returns the number of rounds relative to the key length
	static int getNumRounds(KeyLength length);
};

#endif