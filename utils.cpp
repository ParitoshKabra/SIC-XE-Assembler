#include "utils.h"

string toUpperCase(string su)
{
    transform(su.begin(), su.end(), su.begin(), ::toupper);
    return su;
}
string getString(char x)
{
    // string class has a constructor
    // that allows us to specify size of
    // string as first parameter and character
    // to be filled in given size as second
    // parameter.
    string s(1, x);

    return s;
}
std::string string_to_hex(const std::string &input)
{
    static const char hex_digits[] = "0123456789ABCDEF";
    // trouble with quotes in c++
    std::string output;
    output.reserve((input.length() - 1) * 2);
    for (int i = 0; i < input.length() - 1; i++)
    {
        unsigned char c = input[i];
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}
bool isNumeric(string s)
{
    if (s.length() == 0)
        return false;
    for (unsigned int i = 0; i < s.length(); i++)
    {
        if (s.at(i) - '0' < 0 || s.at(i) - '0' > 9)
            return false;
    }
    return true;
}
bool validf3(int disp)
{
    if (-2048 <= disp && disp <= 2047)
    {
        return true;
    }
    return false;
}
bool validf4(int disp)
{
    if (-524288 <= disp && disp <= 524287)
    {
        return true;
    }
    return false;
}

string print_hex_from_bin(int str)
{
    char buf[2];
    sprintf(buf, "%0x", str);
    // cout << "in print hex: " + string(buf) << endl;
    return string(buf);
}
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
string getProgramName(string label)
{
    if (label.length() > 6)
    {
        return label.substr(0, 6);
    }
    else
    {
        string ans = label;
        while (ans.length() < 6)
        {
            ans += '_';
        }
        return ans;
    }
}
