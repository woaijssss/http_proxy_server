
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

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

}

WebSourceConfig& WebSourceConfig::GetInstance()
{
        // if (!_webSourceConfig.get())
        if (!_webSourceConfig)
        {
                _webSourceConfig = std::shared_ptr<WebSourceConfig>(new WebSourceConfig());
        }

        return *_webSourceConfig.get();
}

void WebSourceConfig::init()
{
        _whpsSaverWithSection["whps"]["whps-name"] = "WhpsDefault";             // 默认whps对象
        _whpsSaverWithSection["StaticResource"]["rootDir"] = "./webResource";   // 静态资源根目录
        _whpsSaverWithSection["Server"]["tcpPort"] = "1024";                    // 默认监听tcp端口
        _whpsSaverWithSection["Server"]["ioThreads"] = "4";                     // 默认io线程数
        _whpsSaverWithSection["Server"]["workThreads"] = "4";                   // 默认worker线程数
        _whpsSaverWithSection["Server"]["httpTimeOut"] = "100";                 // 默认http超时时间(ms)
        _whpsSaverWithSection["Server"]["logPath"] = "./log/server.log";        // 默认日志输出目录
        _whpsSaverWithSection["Mysql"]["ip"] = "127.0.0.1";
        _whpsSaverWithSection["Mysql"]["DBName"] = "test";
        _whpsSaverWithSection["Mysql"]["username"] = "root";
        _whpsSaverWithSection["Mysql"]["password"] = "123456";
}

bool WebSourceConfig::readConfig()
{
        ifstream in(_webConfigPath);

        if (!in) {
                perror("Cannot read config path, exit program");
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
                                        WHPSLogWarn(_webConfigPath + " [" + to_string(num) + "]:delimiter '=' not found.");
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

static string getStartPrint(WebSourceConfig* config)
{
        std::stringstream out;
        out.setf(ios::left);
        out << setw(15) << "<section>" << "\t\t" << setw(15) << "<key>" << "\t\t" << setw(15) << "<value>" << endl;
        for (auto& mPari: config->get())
        {
                for (auto& obj: mPari.second)
                {
                        out << setw(15) << mPari.first << "\t\t" << setw(15) << obj.first << "\t\t" << setw(15) << obj.second << endl;
                }
        }

        return out.str();
}

void WebSourceConfig::print()
{
        string s_out = "\n*********************configuration*********************\n"
                                        + getStartPrint(this)
                                        + "\n*******************************************************";
        WHPSLogInfo(s_out);
}
