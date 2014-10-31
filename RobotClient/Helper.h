#ifndef HELPER_H
#define HELPER_H

#include "../stdafx.h"

using namespace std;

std::vector <string> split(string str, string pattern);

template <typename T> void string2value(string sData, T * fData)
{
    std::vector <string> vString = split(sData, ",");

    for (size_t i=0; i < vString.size(); i++)
    {
        fData[i] = (T)atof(vString[i].c_str());
    }
}

template <typename T> void value2string(const T * fData, string *sData, int count)
{
    ostringstream oss;

    for (int i=0; i < (count - 1); i++)
    {
        oss << setiosflags(ios::fixed) << setprecision(3) << fData[i];
        oss << ',';
    }
    oss << fData[count - 1];

    (*sData) = oss.str();
}

void WriteMessage(string sMsg);

#endif // HELPER_H
