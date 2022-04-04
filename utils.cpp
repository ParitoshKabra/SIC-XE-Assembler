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