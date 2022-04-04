#ifndef PASS_1_
#define PASS_1_
#include "parser.h"
#include "opTab.h"
#include "symTab.h"
#include "structs.h"
#include "utils.h"

#include <bits/stdc++.h>
using namespace std;

bool Pass1(vector<parsedLine> &v, map<string, OpCodeStruct> &opTab, map<string, SymStruct> &symTab, map<string, BlockTable> &blkTab, map<string, LiteralStruct> &litTab, bool &base);
#endif