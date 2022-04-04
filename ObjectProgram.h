#ifndef OBJECT_PROGRAM_H
#define OBJECT_PROGRAM_H
#include <bits/stdc++.h>
#include "structs.h"
#include "utils.h"
using namespace std;
using ll = long long;

void writeHeaderRecord(string name, int startingAddress);
void writeTextRecord(vector<parsedLine> &vec);
void writeModificationRecord(vector<parsedLine> &vec);
void writeEndRecord(int programLength);

#endif