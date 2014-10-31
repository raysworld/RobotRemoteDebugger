#include "../stdafx.h"

using namespace std;

// string split function
vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> vString;
    str += pattern;
    auto size = str.size();

    for (string::size_type i = 0; i<size; i++)
    {
        pos = str.find(pattern, i);
        if (pos<size)
        {
            string s = str.substr(i, pos - i);
            vString.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return vString;
}


void WriteMessage(string sMsg)
{
    std::cout<<sMsg<<std::endl;
}
