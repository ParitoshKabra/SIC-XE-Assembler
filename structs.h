#ifndef STRUCT_H
#define STRUCT_H
#include <bits/stdc++.h>
using namespace std;

const unsigned int FORMAT_1 = 1;
const unsigned int FORMAT_2 = 2;
const unsigned int FORMAT_3_4 = 3;

struct OpCodeStruct
{
    std::string mnemonic;
    int opcode;
    int possibleFormat;
};

struct ObjCode
{
    bool isData = false;
    int opcode;
    int displacement;
    char reg1;
    char reg2;
    bitset<6> flags;
    unsigned int format;
    string data = "";
};

struct parsedLine
{
    long long int location;
    string label;
    string opcode;
    string op1;
    string op2;
    string err;
    bool isFormat4 = false;
    ObjCode objCode;
};

struct BlockTable
{
    string name = "DEFAULT";
    int number = 0;
    long long int locCtr = 0;
    long long int startingAddress = 0;
    long long int blockLength;
};
struct SymStruct
{
    string label;
    int location = -1;
    BlockTable block;
    string flags = "R";
};

struct LiteralStruct
{
    string value;
    long long int address;
    BlockTable block;
    int size;
    bool dumped = false;
};

#endif