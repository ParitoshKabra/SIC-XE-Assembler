#include "ObjectProgram.h"
TextRecord curr;
void writeHeaderRecord(string name, int startingAddress)
{
    cout << 'H' << getProgramName(name) << setfill('0') << setw(6) << right << hex << startingAddress << '\n';
}

void initTextRecord(int start)
{
    curr.text = "";
    curr.startingAddress = start;
    curr.label = 'T';
    curr.size = 0;
}

void printRecord(TextRecord t)
{
    cout << uppercase << t.label << setfill('0') << setw(6) << hex << t.startingAddress << hex << (t.size / 2) << t.text << "\n";
}
void breakRecord()
{
    if (curr.size != 0)
        printRecord(curr);
    curr.size = 0;
}

void writeTextEntity(pair<int, int> p, ll &locCtr)
{
    std::ostringstream ss;
    ss << uppercase << setfill('0') << setw(2 * p.second) << std::hex << p.first;
    std::string result = ss.str();
    // cout << locCtr << " " << result << endl;
    if ((curr.size + result.length()) <= 60)
    {
        curr.text += result;
        curr.size += result.length();
    }
    else
    {
        breakRecord();
        initTextRecord(locCtr);
    }
}

void writeTextRecord(map<string, BlockTable> &blkTab, vector<parsedLine> &vec)
{
    ll startingAddress = 0;
    ll locCtr = startingAddress;
    BlockTable active = blkTab["DEFAULT"];
    initTextRecord(startingAddress);
    ll newRel = active.startingAddress + locCtr;

    for (auto &pl : vec)
    {
        newRel = active.startingAddress + locCtr;
        pair<int, int> p;
        if (pl.opcode == "END")
        {
            breakRecord();
            continue;
        }
        if (pl.opcode == "LTORG" || pl.opcode[0] == '.' || pl.opcode == "BASE" || pl.opcode == "NOBASE" || pl.opcode == "START" || pl.opcode == "EQU")
        {
            continue;
        }
        if (pl.opcode == "USE")
        {
            if (pl.op1 == "")
            {
                pl.op1 = "DEFAULT";
            }
            blkTab[active.name].locCtr = locCtr;
            active = blkTab[pl.op1];
            locCtr = active.locCtr;
            newRel = active.startingAddress + locCtr;
            breakRecord();
            initTextRecord(newRel);
        }
        else if (pl.opcode == "WORD")
        {

            p = genObjcode(pl.objCode, pl);
            writeTextEntity(p, newRel);
            locCtr += 3;
        }
        else if (pl.opcode == "BYTE")
        {
            p = genObjcode(pl.objCode, pl);
            writeTextEntity(p, newRel);
            if (pl.op1[0] == 'X')
            {
                locCtr += (pl.op1.length() - 3) / 2;
            }
            else
            {
                locCtr += pl.op1.length() - 3;
            }
        }
        else if (pl.opcode == "RESB")
        {
            breakRecord();
            initTextRecord(newRel);
            locCtr += stoi(pl.op1);
        }
        else if (pl.opcode == "RESW")
        {
            breakRecord();
            initTextRecord(newRel);
            locCtr += 3 * stoi(pl.op1);
        }
        else if (pl.opcode == "*")
        {
            p = genObjcode(pl.objCode, pl);
            writeTextEntity(p, newRel);
            if (pl.op1[1] == 'X')
            {
                locCtr += (pl.op1.length() - 4) / 2;
            }
            else
            {
                locCtr += (pl.op1.length() - 4);
            }
        }
        else
        {
            p = genObjcode(pl.objCode, pl);

            writeTextEntity(p, newRel);

            if (pl.isFormat4)
            {
                locCtr += 4;
            }
            else
            {
                locCtr += pl.isFormat4;
            }
        }
    }
}

void writeEndRecord(int programLength)
{
    cout << 'E' << setfill('0') << setw(6) << hex << programLength << "\n";
}
void writeModificationRecord(vector<parsedLine> &vec)
{
}