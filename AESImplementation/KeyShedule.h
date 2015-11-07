#ifndef KEY_SCH_H
#define KEY_SCH_H

#include "Structure.h"
#include <bitset>

typedef BYTE Key[4][4];

typedef BYTE KeyEntry[4];

//the key size
//sizes: 128/192/256
enum KeyLength
{
	SMALL,
	MEDIUM,
	LARGE
};


class KeySchedule
{
public:
	KeySchedule(const Key key, int numRounds);
	KeySchedule();
	virtual ~KeySchedule();

	//set the key to the key at round roundNum
	void setRoundKey(int roundNum, Key &key);

	//gets the round coefficient at index i
	//returns the result of xor byte with rc
	BYTE addRoundCoeff(BYTE &byte, int i);

	//performs the core key schedule operations on the word
	void core(BYTE *word, int i);

	//left shift the word by 1 byte
	void rotate(BYTE *word);

	//return the number of sub/round keys in schedule
	int getNumSubkeys();

	//initialzes the schedule arr and adds the sub keys
	void initSchedule(const Key key, int numRounds);

	//makes the passed str into a key
	static void makeKey(BYTE *str, Key &key);

private:
	//schedule to hold the subkeys
	BYTE * schedule;

	//number of sub/round keys in the schedule
	int numSubkeys;

	//adds the sub keys generated from private key to the schedule
	void initKeys(const Key privateKey);

};

#endif