
#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include <stdio.h>

using namespace std;

map<string, string> _saver;
map<string, map<string, string>> _saverWithSection;

// 检查一行是不是section
string findSection(const string& line)
{
        string section;

        if ((line[0] == '[') && (line[line.size()-2]==']'))             // 当前行是section
        {
                section = line.substr(1, line.size()-3);
        }

        return section;
}

void load(string filename)
{
        ifstream in(filename);

        if (!in) {
                cout << filename + ": cannot read." << endl;
                exit(-1);       // 没有找到配置文件即退出进程
        }
        
        bool c = true;
        size_t num = 1;
        string currentSection;

        for (string line; getline(in, line); ++num) 
        {
                // 支持使用"#"进行注释，以及使用空行分隔
                if (line[0] == '#' || !line.size() || line[0] == '\r' || line[0] == '\n') 
                {
                        continue;
                }

                string secTmp = findSection(line);
                if (!secTmp.empty())  // 找到section
                {
                        cout << "1" << endl;
                        currentSection = secTmp;
                        map<string, string> tmp;
                        _saverWithSection[currentSection] = tmp;      // 初始化section的key
                }
                else    // 根据当前section进行填写值
                {
                        cout << "currentSection: " << currentSection << endl;
                        const size_t pos = line.find('=');

                        if (pos != string::npos) 
                        {
                                _saverWithSection[currentSection][line.substr(0, pos)] = line.substr(pos+1, line.size()-2-pos);
                        } 
                        else    // 没有找到分割符 '=' ，格式错误
                        {
                                c = false;
                                cout << filename + " [" + to_string(num) + "]:delimiter '=' not found." << endl;
                                break;
                        }
                }
        }

        in.close();
}

void print()
{
        for (auto& mPari: _saverWithSection)
        {
                for (auto& obj: mPari.second)
                {
                        cout << mPari.first<< "----" << obj.first << ": " << obj.second << endl;
                }
        }
}

int main(int argc, char const *argv[])
{
        string filename = "../conf/web.conf";
        load(filename);
        print();

        cout << _saverWithSection["whps"]["rootDir"]+"123123213" << endl;

        return 0;
}