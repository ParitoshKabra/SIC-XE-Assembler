#include "Pass2.h"

int genObjcode(ObjCode obj, parsedLine &pl)
{
    int op1 = obj.ni + obj.opcode;
    int op2 = obj.xbpe;
    int op3 = obj.displacement;
    // cout << op1 << endl;
    // cout << op1 << " " << op2 << " " << op3 << endl;
    // return 0;
    return op1 * (0x10000) + op2 * (0x1000) + op3;
    // return ((op1 << 4 + op2) << 12) + op3;
}

void printParsedLineListing(parsedLine pl)
{

    cout << setfill('0') << setw(4) << right << hex << pl.location << " " << pl.label << " " << pl.opcode << " " << pl.op1 << " " << pl.op2 << " ";
    cout << pl.err << " " << setfill('0') << setw(6) << left << hex << genObjcode(pl.objCode, pl) << "\n";
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
    string str = pl.op1.substr(substrIndex);
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
                    obj.ni = 2;
                    obj.xbpe = 1;
                    // obj.flags = bitset<6>("100001"); // extended + immediate
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
                    obj.ni = 2;
                    obj.xbpe = 2;
                    // obj.flags = bitset<6>("100010"); // extended + immediate
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
                        obj.ni = 1;
                        obj.xbpe = 1;
                        // obj.flags = bitset<6>("010001"); // extended + immediate
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
                        obj.ni = 1;
                        obj.xbpe = 2;
                        // obj.flags = bitset<6>("010010"); // nixbpe
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
                        obj.ni = 2;
                        obj.xbpe = 1;
                        // obj.flags = bitset<6>("010001"); // extended + immediate
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
                        obj.ni = 1;
                        obj.xbpe = 2;
                        // obj.flags = bitset<6>("010010"); //
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
            auto literal = litTab.find(pl.op1);
            if (pl.isFormat4)
            {
                long long effectiveLoc = literal->second.block.startingAddress + literal->second.address;
                if (validf4(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.ni = 1;
                    obj.xbpe = 1;
                    // obj.flags = bitset<6>("010001"); // extended + immediate
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
                    obj.ni = 2;
                    obj.xbpe = 2;
                    // obj.flags = bitset<6>("010010"); //
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
            // cout << "came here in else " << opTab[pl.opcode].opcode << endl;
            auto symbol = getSymbol(symTab, pl, 0);
            if (pl.isFormat4)
            {
                long long effectiveLoc = symbol.block.startingAddress + symbol.location;
                if (validf4(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.ni = 3;
                    obj.xbpe = 1;
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    pl.err = "Symbol " + pl.op1 + "displacement overflows";
                }
            }
            else
            {
                // cout << symbol.block.name << " " << symbol.block.startingAddress << " " << symbol.location << endl;
                long long effectiveLoc = symbol.block.startingAddress + symbol.location - locCtr;
                if (validf3(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.ni = 3;
                    obj.xbpe = 2;
                    obj.opcode = opTab[pl.opcode].opcode;
                    // cout << "obj: " << obj.opcode + obj.ni << endl;
                    // cout << "obj: " << obj.xbpe << endl;
                    // cout << "obj: " << obj.displacement << endl;
                }
                else
                {
                    // handle base relative
                }
                // otherwise raise error
            }
        }
        pl.objCode = obj;
    }
    catch (string err)
    {
        return true;
    }
    return false;
}

bool createObjectCodeWithRegisters(
    parsedLine &pl, map<string, OpCodeStruct> &opTab, map<string, SymStruct> &symTab, map<string, LiteralStruct> &litTab, long long &locCtr, BlockTable bt)
{
    if (pl.opcode)
    {
    }
}
bool createObjectCodeWithOnlyOpcode(
    parsedLine &pl, map<string, OpCodeStruct> &opTab, map<string, SymStruct> &symTab, map<string, LiteralStruct> &litTab, long long &locCtr, BlockTable bt)
{
    ObjCode obj;
    int op = opTab.find(pl.opcode)->second.opcode;
    obj.displacement = 0;
    obj.ni = 3;
    obj.xbpe = 0;
    obj.opcode = op;
    pl.objCode = obj; // handle operands should not come with zero operand instructions
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
        else if (pl.opcode == "EQU")
        {
            cout << setfill('0') << setw(4) << right << hex << pl.location << " " << pl.label << " " << pl.opcode << " " << pl.op1 << " " << pl.op2 << " " << pl.err << "\n";
            continue;
        }

        else if (pl.opcode == "LTORG")
        {
            // Manage Literal Table
            cout << "\t\tLTORG\t\t" << endl;
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
            blkTab[active.name].locCtr = locCtr;
            active = blkTab[pl.op1];
            locCtr = active.locCtr;
        }
        else
        {
            OpCodeStruct op = opTab.find(pl.opcode)->second;
            if (pl.isFormat4)
            {
                locCtr += 4;
            }
            else
            {
                locCtr += op.possibleFormat;
            }
            ll pcRel = active.startingAddress + locCtr;
            if (op.possibleFormat == FORMAT_2)
            {
                createObjectCodeWithRegisters(pl, opTab, symTab, litTab, pcRel, active);
            }
            else if (op.possibleFormat == FORMAT_1)
            {
                createObjectCodeWithOnlyOpcode(pl, opTab, symTab, litTab, pcRel, active);
            }
            else
            {
                createObjectCodeForInstruction(pl, opTab, symTab, litTab, pcRel, active);
            }
        }
        v[i] = pl;
        if (pl.opcode != "START" && pl.opcode != "END" && pl.opcode != "LTORG" && pl.opcode != "USE" && pl.opcode[0] != '.' && pl.opcode != "RESB" && pl.opcode != "RESW")
        {
            printParsedLineListing(pl);
        }
    }
}