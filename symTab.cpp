#include "symTab.h"

SymStruct *createSymbol(string label, ll locCtr, BlockTable active)
{
    SymStruct *st = new SymStruct;
    st->label = label;
    st->location = locCtr;
    st->block = active;
    return st;
}