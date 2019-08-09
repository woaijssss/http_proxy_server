
#include <iostream>
#include <fstream>

#include "String.h"
#include "WebSourceConfig.h"

using namespace std;

shared_ptr<WebSourceConfig> WebSourceConfig::_webSourceConfig;

WebSourceConfig::WebSourceConfig()
        : ImplConfig()
        , _webConfigPath("./conf/web.conf")
{

}

WebSourceConfig::~WebSourceConfig()
{
        cout << "WebSourceConfig::~WebSourceConfig" << endl;
}

WebSourceConfig& WebSourceConfig::GetInstance()
{
        if (!_webSourceConfig.get())
        {
                _webSourceConfig = std::shared_ptr<WebSourceConfig>(new WebSourceConfig());
        }

        return *_webSourceConfig.get();
}

bool WebSourceConfig::readConfig()
{
        ifstream in(_webConfigPath);

        if (!in) {
                cout << _webConfigPath + ": cannot read." << endl;
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

                string secTmp = this->findSection(line);
                if (!secTmp.empty())  // 找到section
                {
                        currentSection = secTmp;
                        // _whpsSaverWithSection[currentSection] = Map<string, string>();      // 初始化section的key
                        _whpsSaverWithSection[currentSection].swap(map<string, string>());
                }
                else    // 根据当前section进行填写值
                {
                        if (currentSection.size())
                        {
                                String line_(line);
                                line_.strip("\r");
                                SVector sv = line_.split("=");

                                if (sv.size())
                                {
                                        this->set(currentSection, sv[0], sv[1]);
                                } 
                                else    // 没有找到分割符 '=' ，格式错误
                                {
                                        c = false;
                                        cout << _webConfigPath + " [" + to_string(num) + "]:delimiter '=' not found." << endl;
                                        break;
                                }
                        }
                }
        }

        in.close();

        if (_whpsSaverWithSection.empty())
        {
                c = false;
        }

        return c;
}

void WebSourceConfig::set(const string& section, 
                         const string& key, 
                         const string& val)
{
        _whpsSaverWithSection[section][key] = val;
}

const string& WebSourceConfig::get(const string& section, 
                                       const string& key)
{
        return _whpsSaverWithSection[section][key];
}

WebSourceConfig::ConfigTypeWithSection& WebSourceConfig::getAllConfigWithSection()
{
        return _whpsSaverWithSection;
}

void WebSourceConfig::set(const std::string& key, const std::string& val)
{
        _whpsSaver[key] = val;
}

const std::string& WebSourceConfig::get(const std::string& key)
{
        // const string& val = _whpsSaver[key];
        const string& val = _whpsSaver.at(key);
        return val;
}

WebSourceConfig::ConfigType& WebSourceConfig::getAllConfig()
{
        return _whpsSaver;
}

void WebSourceConfig::print()
{
        for (auto& mPari: _whpsSaverWithSection)
        {
                for (auto& obj: mPari.second)
                {
                        cout << mPari.first<< "----" << obj.first << ": " << obj.second << endl;
                }
        }
}