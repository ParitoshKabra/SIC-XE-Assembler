#ifndef SYM_TAB_H_
#define SYM_TAB_H
#include <string>
#include "structs.h"

using namespace std;
using ll = long long int;

SymStruct *createSymbol(string label, ll locCtr, BlockTable b);

#endif