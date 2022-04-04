#include "OpTab.h"

void build(map<string, OpCodeStruct> &opTab)
{
    string mnemonic = "";
    OpCodeStruct *op = new OpCodeStruct;
    mnemonic = "ADD";
    op->mnemonic = "ADD";
    op->opcode = 0x18;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "ADDF";
    op->mnemonic = mnemonic;
    op->opcode = 0x58;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "ADDR";
    op->mnemonic = mnemonic;
    op->opcode = 0x90;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "AND";
    op->mnemonic = mnemonic;
    op->opcode = 0x40;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "CLEAR";
    op->mnemonic = mnemonic;
    op->opcode = 0xB4;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "COMP";
    op->mnemonic = mnemonic;
    op->opcode = 0x28;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "COMPF";
    op->mnemonic = mnemonic;
    op->opcode = 0x88;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "COMPR";
    op->mnemonic = mnemonic;
    op->opcode = 0xA0;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "DIV";
    op->mnemonic = mnemonic;
    op->opcode = 0x24;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "DIVF";
    op->mnemonic = mnemonic;
    op->opcode = 0x64;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "DIVR";
    op->mnemonic = mnemonic;
    op->opcode = 0x9C;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "FIX";
    op->mnemonic = mnemonic;
    op->opcode = 0xC4;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "FLOAT";
    op->mnemonic = mnemonic;
    op->opcode = 0xC0;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "HIO";
    op->mnemonic = mnemonic;
    op->opcode = 0xF4;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "J";
    op->mnemonic = mnemonic;
    op->opcode = 0x3C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "JEQ";
    op->mnemonic = mnemonic;
    op->opcode = 0x30;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "JGT";
    op->mnemonic = mnemonic;
    op->opcode = 0x34;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "JLT";
    op->mnemonic = mnemonic;
    op->opcode = 0x38;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "JSUB";
    op->mnemonic = mnemonic;
    op->opcode = 0x48;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDA";
    op->mnemonic = mnemonic;
    op->opcode = 0x00;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDB";
    op->mnemonic = mnemonic;
    op->opcode = 0x68;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDCH";
    op->mnemonic = mnemonic;
    op->opcode = 0x50;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDF";
    op->mnemonic = mnemonic;
    op->opcode = 0x70;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDL";
    op->mnemonic = mnemonic;
    op->opcode = 0x08;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDS";
    op->mnemonic = mnemonic;
    op->opcode = 0x6C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDT";
    op->mnemonic = mnemonic;
    op->opcode = 0x74;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LDX";
    op->mnemonic = mnemonic;
    op->opcode = 0x04;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "LPS";
    op->mnemonic = mnemonic;
    op->opcode = 0xD0;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "MUL";
    op->mnemonic = mnemonic;
    op->opcode = 0x20;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "MULF";
    op->mnemonic = mnemonic;
    op->opcode = 0x60;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "MULR";
    op->mnemonic = mnemonic;
    op->opcode = 0x98;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "NORM";
    op->mnemonic = mnemonic;
    op->opcode = 0xC8;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "OR";
    op->mnemonic = mnemonic;
    op->opcode = 0x44;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "RD";
    op->mnemonic = mnemonic;
    op->opcode = 0xD8;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "RMO";
    op->mnemonic = mnemonic;
    op->opcode = 0xAC;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "RSUB";
    op->mnemonic = mnemonic;
    op->opcode = 0x4C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "SHIFTL";
    op->mnemonic = mnemonic;
    op->opcode = 0xA4;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "SHIFTR";
    op->mnemonic = mnemonic;
    op->opcode = 0xA8;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "SIO";
    op->mnemonic = mnemonic;
    op->opcode = 0xF0;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "SSK";
    op->mnemonic = mnemonic;
    op->opcode = 0xEC;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STA";
    op->mnemonic = mnemonic;
    op->opcode = 0x0C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STB";
    op->mnemonic = mnemonic;
    op->opcode = 0x78;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STDCH";
    op->mnemonic = mnemonic;
    op->opcode = 0x54;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STF";
    op->mnemonic = mnemonic;
    op->opcode = 0x80;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STI";
    op->mnemonic = mnemonic;
    op->opcode = 0xD4;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STL";
    op->mnemonic = mnemonic;
    op->opcode = 0x14;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STS";
    op->mnemonic = mnemonic;
    op->opcode = 0x7C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STSW";
    op->mnemonic = mnemonic;
    op->opcode = 0xE8;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STT";
    op->mnemonic = mnemonic;
    op->opcode = 0x84;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "STX";
    op->mnemonic = mnemonic;
    op->opcode = 0x10;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "SUB";
    op->mnemonic = mnemonic;
    op->opcode = 0x1C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "SUBF";
    op->mnemonic = mnemonic;
    op->opcode = 0x5C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "SUBR";
    op->mnemonic = mnemonic;
    op->opcode = 0x94;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "SVC";
    op->mnemonic = mnemonic;
    op->opcode = 0xB0;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "TD";
    op->mnemonic = mnemonic;
    op->opcode = 0xE0;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "TIO";
    op->mnemonic = mnemonic;
    op->opcode = 0xF8;
    op->possibleFormat = FORMAT_1;
    opTab[mnemonic] = *op;

    mnemonic = "TIX";
    op->mnemonic = mnemonic;
    op->opcode = 0x2C;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;

    mnemonic = "TIXR";
    op->mnemonic = mnemonic;
    op->opcode = 0xB8;
    op->possibleFormat = FORMAT_2;
    opTab[mnemonic] = *op;

    mnemonic = "WD";
    op->mnemonic = mnemonic;
    op->opcode = 0xDC;
    op->possibleFormat = FORMAT_3_4;
    opTab[mnemonic] = *op;
}
