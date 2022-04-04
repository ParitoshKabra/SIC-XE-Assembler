#include "Pass2.h"
int baseRegister = 0;
bool base = false;

pair<int, int> genObjcode(ObjCode obj, parsedLine &pl)
{
    if (obj.isWord != -13371337)
    {
        return {obj.isWord, 3};
    }
    int op1 = obj.ni + obj.opcode;
    if (obj.isOnlyOpcode)
    {
        return {obj.opcode, 1};
    }
    if (obj.isData)
    {
        return {stoi(obj.data, 0, 16), obj.data.length() / 2};
    }
    if (obj.hasReg)
    {
        int ans = (op1 << 8) + (obj.reg1 << 4);
        if (obj.reg2 == -1)
        {
            return {ans, 2};
        }
        else
        {
            ans += obj.reg2;
            return {ans, 2};
        }
    }
    int op2 = obj.xbpe;
    int op3 = obj.displacement;
    if (pl.isFormat4)
    {
        return {(op1 << 24) + (op2 << 20) + (op3 & 0xffff), 4};
    }
    return {(op1 << 16) + (op2 << 12) + (op3 & 0xfff), 3};
}

string printFormat4(parsedLine pl)
{
    if (pl.isFormat4)
        return "+";
    else
        return "";
}

void printParsedLineListing(parsedLine pl)
{
    auto p = genObjcode(pl.objCode, pl);
    cout << setfill('0') << setw(4) << right << hex << pl.location << " " << pl.label << " " << printFormat4(pl) << pl.opcode << " " << pl.op1 << " " << pl.op2 << " ";
    cout << pl.err << " " << setfill('0') << setw(p.second * 2) << right << hex << p.first << "\n";
}

void printParsedLineWithoutObjCode(parsedLine pl)
{
    cout << setfill('0') << setw(4) << right << hex << pl.location << " " << pl.label << " " << pl.opcode << " " << pl.op1 << " " << pl.op2 << " " << pl.err << "\n";
}

void createObjectCodeForData(parsedLine &pl, int subStrIndex)
{
    ObjCode obj;
    obj.isData = true;
    string str = pl.op1.substr(2 + subStrIndex, pl.op1.length() - 1);
    if (pl.op1[subStrIndex] == 'X')
    {
        obj.data = str;
    }
    else if (pl.op1[subStrIndex] == 'C')
    {

        obj.data = string_to_hex(str);
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

ObjCode pcOrBaseRelativeAddressing(map<string, SymStruct> &symTab, map<string, OpCodeStruct> &opTab, ll &locCtr, parsedLine &pl, int ni, int subStrIndex)
{
    ObjCode obj;
    auto symbol = getSymbol(symTab, pl, subStrIndex);
    bool isAbsolute = symbol.flags == "A";
    long long effectiveLoc = symbol.block.startingAddress + symbol.location;

    bool simple = false;
    int isIndexed = 0;
    if (pl.op2 != "")
    {
        if (pl.op2 == "X")
        {
            isIndexed = 1;
        }
        else
        {
            throw "Index Register should be X";
        }
    }
    if (isAbsolute)
    {
        effectiveLoc = symbol.location;
    }
    if (pl.isFormat4)
    {
        if (validf4(effectiveLoc))
        {
            obj.displacement = effectiveLoc;
            obj.ni = ni;
            obj.xbpe = 1 | (isIndexed << 3);
            obj.opcode = opTab[pl.opcode].opcode;
            return obj;
        }
        else
        {
            pl.err = "Symbol " + pl.op1 + "displacement overflows";
        }
    }
    else
    {
        effectiveLoc -= locCtr;

        if (validf3(effectiveLoc))
        {
            obj.displacement = effectiveLoc;
            obj.ni = ni;
            obj.xbpe = 2 | (isIndexed << 3);
            obj.opcode = opTab[pl.opcode].opcode;
            return obj;
        }
        else if (base)
        {
            effectiveLoc += locCtr - baseRegister;
            if (validf3(effectiveLoc))
            {
                obj.displacement = effectiveLoc;
                obj.ni = ni;
                obj.xbpe = 4 | (isIndexed << 3);
                obj.opcode = opTab[pl.opcode].opcode;
                return obj;
            }
            simple = true;
        }
        else if (simple)
        {
            long long simpleLoc = symbol.location;
            if (validf3(simpleLoc))
            {
                obj.displacement = simpleLoc;
                obj.ni = ni;
                obj.xbpe = 0 | (isIndexed << 3);
                obj.opcode = opTab[pl.opcode].opcode;
                return obj;
            }
            else
            {
                throw "Invalid Displacement";
            }
        }
        throw "Invalid Displacement";
    }
}

bool createObjectCodeForInstruction(parsedLine &pl, map<string, OpCodeStruct> &opTab, map<string, SymStruct> &symTab, map<string, LiteralStruct> &litTab, long long &locCtr)
{

    try
    {
        ObjCode obj;
        if (pl.opcode == "RSUB")
        {
            int op = opTab.find(pl.opcode)->second.opcode;
            obj.displacement = 0;
            obj.ni = 3;
            obj.xbpe = 0;
            obj.opcode = op;
            pl.objCode = obj;
            return false;
        }

        if (pl.op1[0] == '@')
        {
            obj = pcOrBaseRelativeAddressing(symTab, opTab, locCtr, pl, 2, 1);
        }
        else if (pl.op1[0] == '#')
        {
            if (pl.op2 != "")
            {
                pl.err = "Too many arguments";
                return true;
            }

            string str = pl.op1.substr(1);

            if (isNumeric(str))
            {
                int disp = stoi(str);
                bool simple = false;

                if (pl.isFormat4)
                {
                    if (validf4(disp))
                    {
                        obj.displacement = disp;
                        obj.ni = 1;
                        obj.xbpe = 1;
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
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else if (base)
                    {
                        disp += locCtr - baseRegister;
                        if (validf3(disp))
                        {
                            obj.displacement = disp;
                            obj.ni = 1;
                            obj.xbpe = 4;
                            obj.opcode = opTab[pl.opcode].opcode;
                        }
                        else
                        {
                            simple = true;
                        }
                    }
                    else if (simple)
                    {
                        int simpleDisp = stoi(str);
                        if (validf3(simpleDisp))
                        {
                            obj.displacement = simpleDisp;
                            obj.ni = 1;
                            obj.xbpe = 0;
                            obj.opcode = opTab[pl.opcode].opcode;
                        }
                        else
                        {
                            throw "Invalid Displacement";
                        }
                    }
                    else
                    {
                        throw "Invalid Displacement";
                    }
                }
            }
            else
            {
                auto symbol = getSymbol(symTab, pl, 1);
                bool isAbsolute = symbol.flags == "A";
                long long effectiveLoc = symbol.block.startingAddress + symbol.location;
                bool simple = false;

                if (isAbsolute)
                {
                    effectiveLoc = symbol.location;
                }
                if (pl.isFormat4)
                {

                    if (validf4(effectiveLoc))
                    {
                        obj.displacement = effectiveLoc;
                        obj.ni = 2;
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
                    effectiveLoc -= locCtr;
                    if (validf3(effectiveLoc))
                    {
                        obj.displacement = effectiveLoc;
                        obj.ni = 1;
                        obj.xbpe = 2;
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else if (base)
                    {
                        effectiveLoc += locCtr - baseRegister;
                        if (validf3(effectiveLoc))
                        {
                            obj.displacement = effectiveLoc;
                            obj.ni = 1;
                            obj.xbpe = 4;
                            obj.opcode = opTab[pl.opcode].opcode;
                        }
                        else
                        {
                            simple = true;
                        }
                    }
                    else if (simple)
                    {
                        long long simpleLoc = symbol.location;
                        if (validf3(simpleLoc))
                        {
                            obj.displacement = simpleLoc;
                            obj.ni = 1;
                            obj.xbpe = 0;
                            obj.opcode = opTab[pl.opcode].opcode;
                        }
                        else
                        {
                            throw "Invalid Displacement";
                        }
                    }
                    else
                    {
                        throw "Invalid Displacement";
                    }
                }
            }
        }
        else if (pl.op1[0] == '=')
        {
            auto literal = litTab.find(pl.op1)->second;
            bool simple = false;

            if (pl.isFormat4)
            {
                long long effectiveLoc = literal.block.startingAddress + literal.address;
                if (validf4(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.ni = 3;
                    obj.xbpe = 1;
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else
                {
                    pl.err = "Literal " + pl.op1 + "displacement overflows";
                }
            }
            else
            {

                long long effectiveLoc = literal.block.startingAddress + literal.address - locCtr;
                if (validf3(effectiveLoc))
                {
                    obj.displacement = effectiveLoc;
                    obj.ni = 3;
                    obj.xbpe = 2;
                    obj.opcode = opTab[pl.opcode].opcode;
                }
                else if (base)
                {
                    effectiveLoc += locCtr - baseRegister;
                    if (validf3(effectiveLoc))
                    {
                        obj.displacement = effectiveLoc;
                        obj.ni = 3;
                        obj.xbpe = 4;
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else
                    {
                        simple = true;
                    }
                }
                else if (simple)
                {
                    long long simpleLoc = literal.address;
                    if (validf3(simpleLoc))
                    {
                        obj.displacement = simpleLoc;
                        obj.ni = 3;
                        obj.xbpe = 0;
                        obj.opcode = opTab[pl.opcode].opcode;
                    }
                    else
                    {
                        throw "Invalid Displacement";
                    }
                }
                else
                {
                    throw "Invalid Displacement";
                }
            }
        }
        else
        {
            obj = pcOrBaseRelativeAddressing(symTab, opTab, locCtr, pl, 3, 0);
        }
        pl.objCode = obj;
        if (pl.opcode == "LDB")
        {
            baseRegister = obj.displacement;
        }
    }
    catch (string err)
    {
        return true;
    }
    return false;
}

bool createObjectCodeWithRegisters(
    parsedLine &pl, map<string, OpCodeStruct> &opTab, map<string, int> &regs)
{
    ObjCode obj;
    obj.ni = 3;
    int op = opTab.find(pl.opcode)->second.opcode;
    if (pl.opcode == "CLEAR" || pl.opcode == "TIXR")
    {
        if (pl.op2 != "")
        {
            pl.err = "Too many arguments for this opcode";
            return true;
        }
        if (regs.find(pl.op1) == regs.end())
        {
            pl.err = "Not a valid register " + pl.op1;
            return true;
        }
        obj.hasReg = true;
        obj.reg1 = regs.find(pl.op1)->second;
        obj.reg2 = -1;
        obj.opcode = op;
        pl.objCode = obj;
        return false;
    }
    if (regs.find(pl.op1) == regs.end() || regs.find(pl.op2) == regs.end())
    {
        pl.err = "Not a valid register " + pl.op1 + " or " + pl.op2;
        return true;
    }
    obj.hasReg = true;
    obj.reg1 = regs.find(pl.op1)->second;
    obj.reg2 = regs.find(pl.op2)->second;
    obj.opcode = op;
    pl.objCode = obj;
    return false;
}
bool createObjectCodeWithOnlyOpcode(
    parsedLine &pl, map<string, OpCodeStruct> &opTab)
{
    ObjCode obj;
    int op = opTab.find(pl.opcode)->second.opcode;
    obj.opcode = op;
    pl.objCode = obj; // handle operands should not come with zero operand instructions
    return false;
}
bool createObjectCodeForWord(parsedLine &pl)
{
    ObjCode obj;
    if (pl.op1 == "")
    {
        pl.err = "WORD cannot be empty";
        return true;
    }
    if (isNumeric(pl.op1))
    {
        obj.isWord = stoi(pl.op1) & 0xffffff;
        return false;
    }
    pl.err = "Use BYTE for strings, integers for WORD";
    return true;
}
void setProgramLength(map<string, BlockTable> &blkTab, ll &programLength)
{
    programLength = 0;
    for (auto it = blkTab.begin(); it != blkTab.end(); ++it)
    {
        programLength += it->second.locCtr;
    }
}

void pass2(map<string, SymStruct> &symTab, map<string, OpCodeStruct> &opTab, map<string, LiteralStruct> &litTab, map<string, BlockTable> &blkTab, map<string, int> &regs, vector<parsedLine> &v)
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
        else if (pl.opcode == "BASE")
        {
            auto sym = symTab.find(pl.op1);
            if (sym == symTab.end())
            {
                pl.err = "Symbol Undefined";
                err = true;
                continue;
            }
            base = true;
            printParsedLineWithoutObjCode(pl);
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
            setProgramLength(blkTab, programLength); // check
            cout
                << "END"
                << " " << programLength;
        }
        else if (pl.opcode == "RESB")
        {
            locCtr += stoi(pl.op1); // raise error if empty or not a valid integer
            printParsedLineWithoutObjCode(pl);
        }
        else if (pl.opcode == "WORD")
        {
            locCtr += 3;
            createObjectCodeForWord(pl);
        }
        else if (pl.opcode == "RESW")
        {
            locCtr += 3 * stoi(pl.op1);
            printParsedLineWithoutObjCode(pl);
        }
        else if (pl.opcode == "*")
        {
            if (pl.op1[1] == 'C')
            {
                locCtr += pl.op1.length() - 4;
                createObjectCodeForData(pl, 1);
            }
            else if (pl.op1[1] == 'X')
            {
                locCtr += (pl.op1.length() - 4) / 2; // raise error if not a valid prepended hex
                createObjectCodeForData(pl, 1);
            }
        }
        else if (pl.opcode == "BYTE" || pl.opcode == "*")
        {
            // cout << "came here " << pl.opcode << endl;

            if (pl.op1[0] == 'C')
            {
                locCtr += pl.op1.length() - 3;
                createObjectCodeForData(pl, 0);
            }
            else if (pl.op1[0] == 'X')
            {
                locCtr += (pl.op1.length() - 3) / 2; // raise error if not a valid prepended hex
                createObjectCodeForData(pl, 0);
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
                createObjectCodeWithRegisters(pl, opTab, regs);
            }
            else if (op.possibleFormat == FORMAT_1)
            {
                createObjectCodeWithOnlyOpcode(pl, opTab);
            }
            else
            {
                createObjectCodeForInstruction(pl, opTab, symTab, litTab, pcRel);
            }
        }
        v[i] = pl;
        if (pl.opcode != "START" && pl.opcode != "END" && pl.opcode != "LTORG" && pl.opcode != "USE" && pl.opcode[0] != '.' && pl.opcode != "RESB" && pl.opcode != "RESW" && pl.opcode != "BASE")
        {
            printParsedLineListing(pl);
        }
    }
}