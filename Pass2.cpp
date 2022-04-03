#include "Pass2.h"

void print(ObjCode obj)
{
    int muchAd = stoi(obj.flags.to_string().substr(0, 2), 0, 2);
    int dispAd = stoi(obj.flags.to_string().substr(2), 0, 2);
    // cout << obj.flags[0] + obj.flags[1] << endl;
    print_hex_from_bin(muchAd + obj.opcode);
    print_hex_from_bin(dispAd + obj.displacement);
    cout << " ";
}

void printParsedLineListing(parsedLine &pl)
{

    cout << setfill('0') << setw(4) << right << hex << pl.location << " " << pl.label << " " << pl.opcode << " " << pl.op1 << " " << pl.op2 << " ";
    // print(pl.objCode);
    cout << pl.err << "\n";
}

void createObjectCodeForData(parsedLine &pl)
{
    ObjCode obj;
    obj.isData = true;
    if (pl.op1[0] == 'X')
    {
        obj.data = pl.op1.substr(2, pl.op1.length() - 1);
    }
    else if (pl.op1[0] == 'C')
    {
        obj.data = string_to_hex(pl.op1.substr(1, pl.op1.length() - 1));
    }
    pl.objCode = obj;
}

SymStruct getSymbol(map<string, SymStruct> &symTab, parsedLine &pl, int substrIndex)
{
    string str = pl.op1.substr(1);
    auto symbol = symTab.find(str);
    if (symbol == symTab.end())
    {
        pl.err = "Undefined Label";
        throw pl.err;
    }
    return symbol->second;
}

bool createObjectCodeForInstruction(parsedLine &pl, map<string, OpCodeStruct> &opTab, map<string, SymStruct> &symTab, map<string, LiteralStruct> &litTab, long long &locCtr, BlockTable bt)
{
    try
    {
        ObjCode obj;
        if (pl.op1[0] == '@')
        {
            auto symbol = getSymbol(symTab, pl, 1);

            long long effectiveLoc = symbol.block.startingAddress + symbol.location;

            if (pl.isFormat4)
            {
                if (validf4(effectiveLoc))
                {
                    long long effectiveLoc = symbol.block.startingAddress + symbol.location;
                    obj.displacement = effectiveLoc;
                    obj.flags = bitset<6>("100001"); // extended + immediate
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    pl.err = "Symbol " + pl.op1 + "displacement overflows";
                }
            }
            else
            {
                long long effectiveLoc = symbol.block.startingAddress + symbol.location - locCtr;
                if (validf3(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.flags = bitset<6>("100010"); // extended + immediate
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    // handle base relative
                }
            }
            // handle base relative
            // address pc relative?
        }
        else if (pl.op1[0] == '#')
        {
            auto symbol = getSymbol(symTab, pl, 1);
            if (pl.op2 != "")
            {
                pl.err = "Too many arguments";
                return true;
            }
            // generic function for pc and base relative -> 7:30 am
            // handle errors exhaustively -> 5 pm
            // generic function to check for format3 vs format4 -> 8:30 am
            // generic function to limit arguments in a particular opcode -> 9:00 am
            // map for registers char:int

            string str = pl.op1.substr(1);
            if (isNumeric(str))
            {
                int disp = stoi(str);
                if (pl.isFormat4)
                {
                    if (validf4(disp))
                    {
                        obj.displacement = disp;
                        obj.flags = bitset<6>("010001"); // extended + immediate
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else
                    {
                        pl.err = "Immediate " + pl.op1 + "displacement overflows";
                    }
                }
                else
                {
                    if (validf3(disp - locCtr))
                    {
                        obj.displacement = disp - locCtr;
                        obj.flags = bitset<6>("010010"); // nixbpe
                    }
                    else
                    {
                        // Base Relative
                    }
                    // otherwise raise error
                }
            }
            else
            {
                auto symbol = getSymbol(symTab, pl, 0);
                if (pl.isFormat4)
                {
                    long long effectiveLoc = symbol.block.startingAddress + symbol.location;
                    if (validf4(effectiveLoc))
                    {
                        obj.displacement = effectiveLoc;
                        obj.flags = bitset<6>("010001"); // extended + immediate
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else
                    {
                        pl.err = "Symbol " + pl.op1 + "displacement overflows";
                    }
                }
                else
                {
                    long long effectiveLoc = symbol.block.startingAddress + symbol.location - locCtr;
                    if (validf3(effectiveLoc))
                    {
                        obj.displacement = effectiveLoc;
                        obj.flags = bitset<6>("010010"); // extended + immediate
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else
                    {
                        // handle base relative
                    }
                    // otherwise raise error
                }
            }
        }
        else if (pl.op1[0] == '=')
        {
            string str = pl.op1.substr(1);
            auto symbol = symTab.find(str);
            if (symbol == symTab.end())
            {
                pl.err = "Undefined Label";
                return true;
            }

            auto literal = litTab.find(pl.op1);

            if (pl.isFormat4)
            {
                long long effectiveLoc = literal->second.block.startingAddress + literal->second.address;
                if (validf4(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.flags = bitset<6>("010001"); // extended + immediate
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    pl.err = "Literal " + pl.op1 + "displacement overflows";
                }
            }
            else
            {

                long long effectiveLoc = literal->second.block.startingAddress + literal->second.address - locCtr;
                if (validf3(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.flags = bitset<6>("010010"); // extended + immediate
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    // handle base relative
                }
                // otherwise raise error
            }
        }
        else
        {
            auto symbol = getSymbol(symTab, pl, 0);
            if (pl.isFormat4)
            {
                long long effectiveLoc = symbol.block.startingAddress + symbol.location;
                if (validf4(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.flags = bitset<6>("110001"); // extended + immediate
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    pl.err = "Symbol " + pl.op1 + "displacement overflows";
                }
            }
            else
            {
                long long effectiveLoc = symbol.block.startingAddress + symbol.location - locCtr;
                if (validf3(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.flags = bitset<6>("110010"); // extended + immediate
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    // handle base relative
                }
                // otherwise raise error
            }
        }
    }
    catch (string err)
    {
        return true;
    }
    return false;
}

void setProgramLength(map<string, BlockTable> &blkTab, ll &programLength)
{
    programLength = 0;
    for (auto it = blkTab.begin(); it != blkTab.end(); ++it)
    {
        programLength += it->second.locCtr;
    }
}

void pass2(map<string, SymStruct> &symTab, map<string, OpCodeStruct> &opTab, map<string, LiteralStruct> &litTab, map<string, BlockTable> &blkTab, vector<parsedLine> &v)
{
    ll startingAddress = 0;
    ll locCtr = startingAddress;
    ll programLength = 0;
    bool err = false;
    BlockTable active = blkTab["DEFAULT"];
    locCtr = active.locCtr;

    for (int i = 0; i < v.size(); i++)
    {
        auto pl = v[i];
        if (pl.opcode == "START")
        {
            startingAddress = stoi(pl.op1);
            locCtr = startingAddress;
            cout << pl.label << " " << pl.opcode << " " << startingAddress << endl;
        }
        else if (pl.opcode[0] == '.')
        {
            cout << "\t\t" + pl.opcode + "\t\t" << endl;
        }
        else if (pl.opcode == "END")
        {
            // Manage Block Table
            setProgramLength(blkTab, programLength);
            cout
                << "END"
                << " " << programLength;
        }
        else if (pl.opcode == "RESB")
        {
            locCtr += stoi(pl.op1); // raise error if empty or not a valid integer
        }
        else if (pl.opcode == "WORD")
        {
            locCtr += 3;
            createObjectCodeForData(pl);
        }
        else if (pl.opcode == "RESW")
        {
            locCtr += 3 * stoi(pl.op1);
        }
        else if (pl.opcode == "BYTE" || pl.opcode == "*")
        {
            if (pl.op1[0] == 'C')
            {
                locCtr += pl.op1.length() - 3;
                createObjectCodeForData(pl);
            }
            else if (pl.op1[0] == 'X')
            {
                locCtr += (pl.op1.length() - 3) / 2; // raise error if not a valid prepended hex
                createObjectCodeForData(pl);
            }
        }
        else if (pl.opcode == "USE")
        {
            cout << "\t\tUSE " << pl.op1 << "\t\t" << endl;
            if (pl.op1 == "")
            {
                pl.op1 = "DEFAULT";
            }
            active = blkTab[pl.op1];
            locCtr = active.locCtr;
        }
        else
        {
            if (pl.isFormat4)
            {
                locCtr += 4;
            }
            else
            {
                OpCodeStruct op = opTab.find(pl.opcode)->second;
                locCtr += op.possibleFormat;
            }
            ll pcRel = active.startingAddress + locCtr;
            createObjectCodeForInstruction(pl, opTab, symTab, litTab, pcRel, active);
        }
        v[i] = pl;
        if (pl.opcode != "START" && pl.opcode != "END" && pl.opcode != "LTORG" && pl.opcode != "USE" && pl.opcode[0] != '.')
        {
            printParsedLineListing(pl);
        }
    }
}