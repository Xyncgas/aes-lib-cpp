#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <bitset>
#include <vector>

typedef std::bitset<8> BYTE;
typedef BYTE State[4][4];

enum Mode
{
	ENCRYPT,
	DECRYPT
};

class Structure
{
public:
	static BYTE getSboxEntry(int row, int column, Mode mode);

	static BYTE getSboxEntry(BYTE bin, Mode mode);

	static BYTE getRconEntry(int index);

	static BYTE getMixColEntry(int row, int column, Mode mode);

	static std::vector<BYTE> getColumn(const State state, int column);

	static void printState(const State state);

	static void makeState(BYTE str[16], State &state);

	static void printRow(BYTE * row);

private:
	static unsigned char sbox[16][16];

	static unsigned char inverseSbox[16][16];

	static unsigned char rcon[256];

	static BYTE mixColMatrix[4][4];

	static BYTE inverseMixColMatrix[4][4];
};

#endif