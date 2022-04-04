#include <bits/stdc++.h>
#include <regex>
#include "opTab.h"
#include "structs.h"
#include "symTab.h"
#include "Pass1.h"
#include "Pass2.h"
#include "parser.h"

using namespace std;
using ll = long long;

ll mod = 1e9 + 7;
double epsilon = 1e-6;
#define psb push_back
#define rsz resize
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define ff first
#define ss second
#define mp make_pair
#define prDouble(d) cout << fixed << setprecision(10) << d

void setIO(string name = "")
{
    ios_base::sync_with_stdio(0);
    cin.tie(0); // see Fast Input & Output
    if (sz(name))
    {
        freopen((name + ".in").c_str(), "r", stdin); // see Input & Output
        freopen((name + ".out").c_str(), "w", stdout);
    }
}

void print()
{
    cout << endl;
}
// Variadic function Template that takes
// variable number of arguments and prints
// all of them.
template <typename T, typename... Types>
void print(T var1, Types... var2)
{
    cout << var1 << ' ';
    print(var2...);
}

// int baseRegister = -1;
bool base = false;

map<string, int> regs;

void buildRegMap()
{
    regs["A"] = 1;
    regs["X"] = 2;
    regs["L"] = 3;
    regs["B"] = 4;
    regs["S"] = 5;
    regs["T"] = 6;
    regs["F"] = 7;
    regs["PC"] = 8;
    regs["SW"] = 9;
}

void printTable(map<string, SymStruct> &mp)
{
    for (auto it = mp.begin(); it != mp.end(); ++it)
    {
        print(it->first, it->second.location);
    }
}
void printTable(map<string, LiteralStruct> &mp)
{
    for (auto it = mp.begin(); it != mp.end(); ++it)
    {
        print(it->first, it->second.address, it->second.size, it->second.value, it->second.block.name);
    }
}
void printTable(map<string, BlockTable> &mp)
{
    for (auto it = mp.begin(); it != mp.end(); ++it)
    {
        print(it->first, it->second.name, it->second.blockLength, it->second.startingAddress, it->second.number);
    }
}

int main()
{
    setIO("test");
    vector<parsedLine> vec;
    map<string, OpCodeStruct> opTab;
    map<string, SymStruct> symTab;
    map<string, BlockTable> blkTab;
    map<string, LiteralStruct> litTab;
    BlockTable b;
    blkTab["DEFAULT"] = b;
    build(opTab);
    buildRegMap();
    string s;
    ll startingAddress = 0; // custom startingAddress
    ll locCtr = startingAddress;
    ll programLength;

    while (true)
    {
        getline(cin, s);
        if (cin.fail())
        {
            break;
        }
        parsedLine pline = parse(s);
        if (!(pline.label == "" && pline.opcode == "" && pline.op1 == "" && pline.op2 == ""))
        {
            vec.push_back(pline);
        }
    }

    try
    {
        cout << "***Intermediate File***\n\n"
             << endl;
        bool err = Pass1(vec, opTab, symTab, blkTab, litTab, base);
        if (!err)
        {
            print();
            printTable(symTab);
            printTable(blkTab);
            printTable(litTab);
            cout << "\n\n***Listing File***\n"
                 << endl;
            pass2(symTab, opTab, litTab, blkTab, regs, vec);
        }
    }
    catch (string err)
    {
        print(err);
    }

    return 0;
}