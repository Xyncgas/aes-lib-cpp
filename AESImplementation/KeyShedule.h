#ifndef KEY_SCH_H
#define KEY_SCH_H

#include "Structure.h"
#include <bitset>

typedef BYTE Key[4][4];

typedef BYTE KeyEntry[4];

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

	void setKey(int roundNum, Key &key);
	BYTE addRoundCoeff(BYTE &byte, int i);
	void core(BYTE *word, int i);
	void rotate(BYTE *word);
	int getNumSubkeys();
	static void makeKey(BYTE *str, Key &key);

private:
	BYTE * schedule;
	int numSubkeys;
	void initKeys(const Key privateKey);

};

#endif