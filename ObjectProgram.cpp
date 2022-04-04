#include "ObjectProgram.h"
TextRecord curr;
void writeHeaderRecord(string name, int startingAddress)
{
    cout << 'H' << name << getProgramName(name) << setfill('0') << setw(6) << right << hex << startingAddress << '\n';
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
    cout << hex << t.label << setfill('0') << setw(6) << t.startingAddress << uppercase << hex << t.size << t.text << "\n";
}
void breakRecord()
{
    if (curr.size != 0)
        printRecord(curr);
    curr.size = 0;
}
// for(auto &pl){

// }
// std::ostringstream ss;
// ss << uppercase << setfill('0') << setw(2 * siz) << std::hex << t.objectcode;
// std::string result = ss.str();

void writeTextRecord(map<string, SymStruct> &symTab, map<string, OpCodeStruct> &opTab, map<string, LiteralStruct> &litTab, map<string, BlockTable> &blkTab, map<string, int> &regs, vector<parsedLine> &vec)
{
    ll startingAddress = 0;
    ll locCtr = startingAddress;
    BlockTable active = blkTab["DEFAULT"];

    for (auto &pl : vec)
    {

        if (pl.opcode == "END" || pl.opcode == "LTORG" || pl.opcode[0] == '.' || pl.opcode == "BASE" || pl.opcode == "BASE" || pl.opcode == "START")
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
        }
        else if (pl.opcode == "WORD")
        {
            locCtr += 3;
        }
        else if (pl.opcode == "RESB")
        {
            locCtr += 3;
        }
    }
}
void writeEndRecord(int programLength)
{
    cout << 'E' << setfill('0') << setw(6) << hex << programLength << "\n";
}