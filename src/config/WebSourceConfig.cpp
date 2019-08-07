
#include <iostream>
#include <fstream>

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

        for (string line; getline(in, line); ++num) 
        {
                // 支持使用"#"进行注释，以及使用空行分隔
                if (line[0] == '#' || !line.size()) 
                {
                        continue;
                }

                const size_t pos = line.find('=');

                if (pos != string::npos) 
                {
                        // _whpsSaver[line.substr(0, pos)] = line.substr(pos + 1);
                        this->set(line.substr(0, pos), line.substr(pos + 1));
                } 
                else    // 没有找到分割符 '=' ，格式错误
                {
                        c = false;
                        cout << _webConfigPath + " [" + to_string(num) + "]:delimiter '=' not found." << endl;
                        break;
                }
        }

        in.close();

        return c;
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

ImplConfig::ConfigType& WebSourceConfig::getAllConfig()
{
        return _whpsSaver;
}