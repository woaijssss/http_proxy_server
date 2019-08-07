
#include <iostream>

#include "Codec.h"

using namespace std;

char toHex(char x) 
{ 
        return  x > 9 ? x + 55 : x + 48; 
}
 
char fromHex(char x) 
{ 
        unsigned char y = 0;
        if (x >= 'A' && x <= 'Z') 
        {
                y = x - 'A' + 10;
        }
        else if (x >= 'a' && x <= 'z') 
        {
                y = x - 'a' + 10;
        }
        else if (x >= '0' && x <= '9') 
        {
                y = x - '0';
        }
        else 
        {
                // pass
        }

        return y;
}

string Codec::encode(const string& str, const string& type)
{
        string str_ = str;

        if (type == "utf-8")
        {

        }
        else if (type == "UrlCode")
        {
                str_ = urlEncode(str);
        }
        else
        {
                cout << "String not support encode this type: [" + type + "]" << endl;
        }

        return str_;
}
        
string Codec::decode(const string& str, const string& type)
{
        string str_ = str;

        if (type == "utf-8")
        {

        }
        else if (type == "UrlCode")
        {
                str_ = urlDecode(str);
        }
        else
        {
                cout << "String not support decode this type: [" + type + "]" << endl;
        }

        return str_;
}

string Codec::urlEncode(const string& str)
{
        std::string stmp = "";
        size_t length = str.length();

        for (size_t i = 0; i < length; i++)
        {
                if (isalnum((unsigned char)str[i]) 
                    || (str[i] == '-') 
                    ||(str[i] == '_') 
                    || (str[i] == '.') 
                    || (str[i] == '~'))
                {
                        stmp += str[i];
                }
                else if (str[i] == ' ')
                {
                        stmp += "+";
                }
                else
                {
                        stmp += '%';
                        stmp += toHex((unsigned char)str[i] >> 4);
                        stmp += toHex((unsigned char)str[i] % 16);
                }
        }

        return stmp;
}

string Codec::urlDecode(const string& str)
{
        std::string stmp = "";
        size_t length = str.length();

        for (size_t i = 0; i < length; i++)
        {
                if (str[i] == '+') 
                {
                        stmp += ' ';
                }
                else if (str[i] == '%')
                {
                        // assert(i + 2 < length);
                        if ((i+2) >= length)
                        {
                                break;
                        }

                        unsigned char high = fromHex((unsigned char)str[++i]);
                        unsigned char low = fromHex((unsigned char)str[++i]);
                        stmp += high*16 + low;
                }
                else 
                {
                        stmp += str[i];
                }
        }

        return stmp;
}
