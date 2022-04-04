#ifndef PASS2_H
#define PASS2_H
#include <bits/stdc++.h>
#include "structs.h"
#include "utils.h"

using namespace std;
using ll = long long;
bool pass2(map<string, SymStruct> &symTab, map<string, OpCodeStruct> &opTab, map<string, LiteralStruct> &litTab, map<string, BlockTable> &blkTab, map<string, int> &regs, vector<parsedLine> &v, ll &programLength);
#endif