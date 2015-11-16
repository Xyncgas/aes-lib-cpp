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
	//returns the s-box entry a[row][column]
	static BYTE getSboxEntry(int row, int column, Mode mode);

	//return the s-box entry for bin where rows, cols are calculated
	static BYTE getSboxEntry(BYTE bin, Mode mode);

	//returns the round coefficient at index
	static BYTE getRconEntry(int index);

	//returns the mix column entry mixColMatrix[row][column]
	//or for inverse inverseMixColMatrix[row][column
	static BYTE getMixColEntry(int row, int column, Mode mode);

	//returns the column at column from the state 
	static std::vector<BYTE> getColumn(const State state, int column);

	//prints out the rows and columns of the state
	static void printState(const State state);

	//prints out all the states strings 
	static void printStatesString(State *states, int numStates);

	//puts the bytes in str into the state
	static void makeState(BYTE str[16], State &state);

	//returns sum of states a, b where addition is XOR
	static void addState(State &a, const State b);

	//copies the state b into a
	static void copyState(State &a, State &b);

	//puts the text into states
	static State * makeStates(std::string text);

	//returns the number of states needed for text
	static int getNumStates(const std::string text);

	//returns the states string 
	static std::string stateToString(State state);

	//prints out the values of the passed row
	static void printRow(BYTE * row);

private:
	//AES substition box
	static unsigned char sbox[16][16];

	//AES inverse substitution box 
	static unsigned char inverseSbox[16][16];

	//round coefficients
	static unsigned char rcon[256];

	//mix columns matrix
	static BYTE mixColMatrix[4][4];

	//inverse mix column matrix 
	static BYTE inverseMixColMatrix[4][4];
};

#endif