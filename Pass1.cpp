#include "Pass1.h"
// TODO: Handle 'ORG'
void printParsedLineInterMediate(parsedLine &pl)
{

    cout << setfill('0') << setw(4) << right << hex << pl.location << " " << pl.label << " " << pl.opcode << " " << pl.op1 << " " << pl.op2 << " " << pl.err << "\n";
    // printf("%s %s %s %s\n", pl.label, pl.opcode, pl.op1, pl.op2);
}
BlockTable createBlock(string name, int id)
{
    BlockTable b;
    b.name = name;
    b.number = id;
    return b;
}
LiteralStruct createLiteral(string name, BlockTable active, int litSize)
{
    LiteralStruct lt;
    lt.value = name;
    lt.block = active;
    lt.size = litSize;
    return lt;
}

bool comparator(BlockTable b1, BlockTable b2)
{
    return b1.number < b2.number;
}

void manageBlockTable(map<string, BlockTable> &blkTab, map<string, SymStruct> &symTab, long long &programLength, ll &startingAddress, map<string, LiteralStruct> &litTab)
{
    vector<BlockTable> v;

    programLength = 0;
    for (auto it = blkTab.begin(); it != blkTab.end(); ++it)
    {
        programLength += it->second.locCtr;
        v.push_back(it->second);
    }
    sort(v.begin(), v.end(), comparator);
    v[0].blockLength = v[0].locCtr;
    v[0].startingAddress = startingAddress;
    blkTab[v[0].name] = v[0];

    for (auto it = v.begin() + 1; it != v.end(); ++it)
    {
        auto prev = it - 1;
        it->blockLength = it->locCtr;
        it->startingAddress = prev->startingAddress + prev->blockLength;
        blkTab[it->name] = *it;
    }
    for (auto &sym : symTab)
    {
        sym.second.block = blkTab[sym.second.block.name];
    }
    for (auto &lit : litTab)
    {
        lit.second.block = blkTab[lit.second.block.name];
    }
}

void manageLiteralTable(map<string, LiteralStruct> &lt, long long &locCtr, vector<parsedLine> &v, vector<pair<int, parsedLine>> &lits, int index, BlockTable active)
{

    for (auto it = lt.begin(); it != lt.end(); it++)
    {
        if (!it->second.dumped)
        {
            it->second.dumped = true;
            it->second.block = active;
            parsedLine p;
            it->second.address = locCtr;
            p.opcode = "*";
            p.op1 = it->second.value;
            p.location = locCtr;
            lits.push_back({index, p});
            // v.insert(v.begin() + index, p);
            printParsedLineInterMediate(p); // insert into vector
            locCtr += it->second.size;
        }
    }
}

int getRelativity(SymStruct st)
{
    if (st.flags == "R")
        return 1;
    return 0;
}
bool fillSymTab(parsedLine &pl, map<string, SymStruct> &symTab, ll &locCtr, BlockTable active)
{
    if (pl.label == "")
    {
        pl.err = "Label can't be empty";
        return true; // returning that an error occured
    }
    else if (symTab.find(pl.label) == symTab.end())
    {
        if (pl.op1 == "*")
        {
            symTab[pl.label] = *createSymbol(pl.label, locCtr, active);
            return false;
        }
        else
        {
            try
            {
                string inValidArithmeticOperations = "*/&|!^";
                bool inValidOperation = false;
                for (auto op : inValidArithmeticOperations)
                {
                    if (pl.op1.find(op) != string::npos)
                    {
                        inValidOperation = true; // use throw and catch into the error
                        throw op;
                    }
                }
                // assembler will crash for any other unsupported operands, add them to above list etc.
                if (!inValidOperation)
                {
                    // check valid relative or absolute expression

                    // TODO:  Allow spaces between expressions
                    // Note: Valid Expression SYNTAX is BUFEND-BUFFER+MAXLEN (No spaces between operands and operation)

                    string validArithmeticOperations = "+-";
                    string symLabel = "";
                    deque<pair<SymStruct, int>> labels;
                    deque<string> operations;
                    int R = 0;
                    for (int i = 0; i < pl.op1.length(); i++)
                    {
                        if (validArithmeticOperations.find(pl.op1[i]) == string::npos)
                        {
                            symLabel.push_back(pl.op1[i]);
                        }
                        else
                        {
                            if (isNumeric(symLabel))
                            {
                                labels.push_back({*createSymbol("numeric", stoi(symLabel), createBlock("numeric", 0)), 0});
                                operations.push_back(getString(pl.op1[i]));
                                symLabel = "";
                                continue;
                            }
                            if (symTab.find(symLabel) == symTab.end())
                            {
                                pl.err = "Undefined " + symLabel;
                                return true;
                            }
                            if (symLabel == "")
                            {
                                pl.err = "Invalid Expression";
                                return true;
                            }
                            auto symbol = symTab.find(symLabel)->second;
                            labels.push_back({symbol, getRelativity(symbol)});
                            operations.push_back(getString(pl.op1[i]));
                            symLabel = "";
                        }
                    }
                    bool num = false;
                    if (isNumeric(symLabel))
                    {
                        num = true;
                        labels.push_back({*createSymbol(pl.label, stoi(symLabel), active), 0});
                        symLabel = "";
                    }
                    if (!num)
                    {
                        if (symTab.find(symLabel) == symTab.end())
                        {
                            pl.err = "Undefined " + symLabel;
                            return true;
                        }
                        auto symbol = symTab.find(symLabel)->second;

                        labels.push_back({symbol, getRelativity(symTab.find(symLabel)->second)});
                    }
                    while (!labels.empty() && !operations.empty())
                    {
                        auto op1 = labels.front();
                        labels.pop_front();
                        auto op2 = labels.front();
                        labels.pop_front();
                        string arith = operations.front();
                        operations.pop_front();
                        ll location = 0;
                        int relativity = 0;
                        if (arith == "+")
                        {
                            location = op1.first.location + op2.first.location; // two symbols must be in same block
                            relativity = op1.second + op2.second;
                        }
                        else
                        {
                            location = op1.first.location - op2.first.location;
                            relativity = op1.second - op2.second;
                        }
                        labels.push_front({*createSymbol(pl.label, location, active), relativity});
                    }
                    if (labels.front().second == 0 or labels.front().second == 1)
                    {
                        SymStruct ssy = labels.front().first;
                        ssy.flags = "A";
                        symTab[pl.label] = ssy;
                    }
                    else
                    {
                        pl.err = "Inappropriate Relative or Absolute Expression";
                        return true;
                    }
                }
            }
            catch (char err)
            {
                pl.err = "Invalid operation for this constant: " + getString(err);
                return true;
            }
            return false;
        }
    }
    else
    {
        // raise error label already exists
        pl.err = "Label already defined";
        return true;
    }
}

bool Pass1(vector<parsedLine> &vec, map<string, OpCodeStruct> &opTab, map<string, SymStruct> &symTab, map<string, BlockTable> &blkTab, map<string, LiteralStruct> &litTab, bool &base)
{
    ll startingAddress = 0;
    ll locCtr = startingAddress;
    ll programLength = 0;
    bool err = false;
    BlockTable active = blkTab["DEFAULT"];
    locCtr = active.locCtr;
    int id = active.number;
    vector<pair<int, parsedLine>> lits;
    for (int i = 0; i < vec.size(); i++)
    {
        auto pl = vec[i];
        pl.location = locCtr;
        if (pl.opcode == "START")
        {
            startingAddress = stoi(pl.op1);
            locCtr = startingAddress;
        }
        else if (pl.opcode == "*")
        {
            continue;
        }
        else if (pl.opcode == "BASE")
        {
            base = true;
            cout << "\t\t" + pl.opcode << " " << pl.op1 << " " << pl.op2 << " " << pl.err << "\n";
        }
        else if (pl.opcode == "LTORG")
        {
            // Manage Literal Table
            cout << "\t\tLTORG\t\t" << endl;
            manageLiteralTable(litTab, locCtr, vec, lits, i + 1, active);
        }
        else if (pl.opcode == "END")
        {
            // Manage Block Table
            manageBlockTable(blkTab, symTab, programLength, startingAddress, litTab);
            manageLiteralTable(litTab, locCtr, vec, lits, i + 1, blkTab["DEFAULT"]);
            cout
                << "END"
                << " " << programLength;
        }
        else if (pl.opcode == "USE")
        {
            cout << "\t\tUSE " << pl.op1 << "\t\t" << endl;
            if (pl.op1 == "")
            {
                pl.op1 = "DEFAULT";
            }
            blkTab[active.name].locCtr = locCtr;
            auto block = blkTab.find(pl.op1);
            if (block == blkTab.end())
            {

                blkTab[pl.op1] = createBlock(pl.op1, ++id);
            }

            active = blkTab[pl.op1];
            locCtr = active.locCtr;
        }
        else if (pl.opcode == "EQU")
        {
            err = fillSymTab(pl, symTab, locCtr, active);
        }
        else
        {

            if (pl.label != "")
            {
                symTab[pl.label] = *createSymbol(pl.label, locCtr, active); // raise error here if symbol exists
            }

            if (pl.opcode == "RESB")
            {
                locCtr += stoi(pl.op1); // raise error if empty or not a valid integer
            }
            else if (pl.opcode == "WORD")
            {
                locCtr += 3;
            }
            else if (pl.opcode == "RESW")
            {
                locCtr += 3 * stoi(pl.op1);
            }
            else if (pl.opcode == "BYTE")
            {
                if (pl.op1[0] == 'C')
                {
                    locCtr += pl.op1.length() - 3;
                }
                else if (pl.op1[0] == 'X')
                {
                    locCtr += (pl.op1.length() - 3) / 2; // raise error if not a valid prepended hex
                }
            }
            else
            {
                string mnemonic = pl.opcode;
                bool isFormat4 = false;
                if (pl.opcode[0] == '+')
                {
                    mnemonic = pl.opcode.substr(1);
                    isFormat4 = true;
                }
                if (pl.opcode[0] == '.')
                {
                    cout << "\t\t" << pl.opcode << "\t\t" << endl;
                    continue;
                }
                auto op = opTab.find(mnemonic);

                if (op == opTab.end())
                {
                    pl.err = "Invalid Opcode";
                    err = true;
                }
                else
                {
                    if (pl.op1[0] == '=')
                    {
                        auto literal = litTab.find(pl.op1);
                        if (literal == litTab.end())
                        {
                            int litSize = 0;
                            switch (pl.op1[1])
                            {
                            case '*':
                                if (pl.op1.length() != 2)
                                {
                                    pl.err = "Invalid literal format";
                                    err = true;
                                }
                                litSize = 3;

                                break;
                            case 'C':
                                litSize = pl.op1.length() - 4;
                                break;
                            case 'X':
                                litSize = (pl.op1.length() - 4) / 2; // raise error if not a vaild prepended hex
                                break;

                            default:
                                break;
                            }
                            litTab[pl.op1] = createLiteral(pl.op1, active, litSize);
                        }
                    }
                    pl.isFormat4 = isFormat4;
                    if (isFormat4)
                    {
                        locCtr += 4;
                    }
                    else
                    {
                        locCtr += op->second.possibleFormat;
                    }
                }
            }
        }
        vec[i] = pl;
        if (pl.opcode != "END" && pl.opcode != "LTORG" && pl.opcode != "USE" && pl.opcode[0] != '.' && pl.opcode != "BASE")
        {
            printParsedLineInterMediate(pl);
        }
    }
    for (int i = 0; i < lits.size(); i++)
    {
        vec.insert(vec.begin() + lits[i].first, lits[i].second);
    }

    return err;
}