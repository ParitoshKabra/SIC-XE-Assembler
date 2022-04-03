#include "parser.h"

// std::vector<std::string> parse(const std::string &s, std::string rgx_str = "\\s+")
// {
//     std::vector<std::string> elems;

//     std::regex rgx(rgx_str);

//     std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
//     std::sregex_token_iterator end;

//     while (iter != end)
//     {
//         // std::cout << "S43:" << *iter << std::endl;
//         elems.push_back(*iter);
//         ++iter;
//     }

//     if (elems.size() < 3)
//     {
//         elems.push_back(" ");
//     }
//     return elems;
// }
#include "Parser.h"
#include "Parser.h"
#include "utils.h"

bool needNoOperands(string s)
{
    // This function check the operand for Format 1 type.
    // Because in that case no operand will be required to be supplies with them.
    s = toUpperCase(s);
    // Whole string is converted to Uppercase to bring uniformity for comparision
    return s.compare("RSUB") == 0 || s.compare("NOBASE") == 0 || s.compare("HIO") == 0 || s.compare("FIX") == 0 || s.compare("NORM") == 0 || s.compare("TIO") == 0 || s.compare("START") == 0 || s.compare("SIO") == 0 || s.compare("FLOAT") == 0 || s.compare("CSECT") == 0;
}

parsedLine parse(string line)
{
    // This function read the line supplied as argument and split it into
    //  fundamental elements of an instruction like label, opcode, op1 and
    // op2 if they exists.
    unsigned int i;
    parsedLine pl;
    pl.label = "";
    pl.opcode = "";
    pl.op1 = "";
    pl.op2 = "";

    string s[5] = {"", "", "", "", ""};
    // Initialising the array with empty string which will be replaced as string
    // is parsed
    i = 0;
    for (unsigned int j = 0; j < 5 && i < line.length(); j++)
    {
        for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++)
            ;
        // Skiping all blank spaces and tabs until it encounter characters
        // So that freely formated SIC/XE instruction can also be converted by
        // assembler
        for (; i < line.length() && (line[i] != ' ' && line[i] != '\t'); i++)
            s[j] += line[i];
        // When some character is encountered words are being added in the
        //  respective location in s[]
    }
    for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++)
        ;

    if (s[0] != "" && s[0][0] == '.')
    { // comment line
        for (i = 0; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++)
            ;
        for (; i < line.length(); i++)
            pl.opcode += line[i];
        return pl;
    }

    if (i != line.length())
        throw new string("Too Many Arguments");
    // line containing too many arguments

    if (s[0] == "")
        return pl;
    else if (s[1] == "")
        pl.opcode = s[0];
    else if (s[2] == "")
    {
        if (needNoOperands(s[1]))
        {
            pl.label = s[0];
            pl.opcode = s[1];
        }
        else
        {
            pl.opcode = s[0];
            pl.op1 = s[1];
        }
    }
    else if (s[3] == "")
    {
        if (s[1][s[1].length() - 1] == ',' || s[2][0] == ',')
        {
            pl.opcode = s[0];
            pl.op1 = s[1] + s[2];
        }
        else
        {
            pl.label = s[0];
            pl.opcode = s[1];
            pl.op1 = s[2];
        }
    }
    else if (s[4] == "")
    {
        if (s[2].compare(",") == 0)
        {
            pl.opcode = s[0];
            pl.op1 = s[1] + s[2] + s[3];
        }
        else
        {
            pl.label = s[0];
            pl.opcode = s[1];
            pl.op1 = s[2] + s[3];
        }
    }
    else
    {
        if (s[3].compare(",") != 0)
            throw new string("Too Many Arguments");
        // line containging too many arguments

        pl.label = s[0];
        pl.opcode = s[1];
        pl.op1 = s[2] + s[3] + s[4];
    }

    s[0] = "";
    s[1] = "";
    for (i = 0; i < pl.op1.length() && pl.op1[i] != ','; i++)
        s[0] += pl.op1[i];
    if (i == pl.op1.length() - 1 && pl.op1[i] == ',')
        s[1] = ",";
    else
        for (i++; i < pl.op1.length(); i++)
            s[1] += pl.op1[i];
    pl.op1 = s[0];
    pl.op2 = s[1];
    // cout << pl.label << " " << pl.opcode << " " << pl.op1 << " " << pl.op2 << " " << pl.err << endl;
    return pl;
}
