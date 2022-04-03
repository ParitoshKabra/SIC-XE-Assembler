#ifndef PASS2_H
#define PASS2_H
#include <bits/stdc++.h>
#include "structs.h"
#include "utils.h"

using namespace std;
using ll = long long;
void pass2(map<string, SymStruct> &symTab, map<string, OpCodeStruct> &opTab, map<string, LiteralStruct> &litTab, map<string, BlockTable> &blkTab, vector<parsedLine> &v);
#endif