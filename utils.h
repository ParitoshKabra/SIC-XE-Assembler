#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <bits/stdc++.h>
#include "structs.h"

using namespace std;

string toUpperCase(string s);
string getString(char x);
std::string string_to_hex(const std::string &input);
bool isNumeric(string s);
bool validf3(int disp);
bool validf4(int disp);
string print_hex_from_bin(int str);
pair<int, int> genObjcode(ObjCode obj, parsedLine &pl);
string getProgramName(string label);

#endif