/*
 * mysql_connector.h
 *
 *  Created on: Apr 3, 2019
 *      Author: wenhan
 */

#ifndef __MYSQL_CONNECTOR_H__
#define __MYSQL_CONNECTOR_H__

#include <mysql/mysql.h>
#include <string>

#ifndef __stdcall
#define __stdcall
#endif

typedef struct mysql_conn_info_t {
        std::string ip_addr;    // ip地址
        std::string db_name;    // 数据库名
        std::string username;       // 用户名
        std::string password;        // 密码
}MysqlConnInfo;

class MysqlConnector {
public:
        MysqlConnector();
        ~MysqlConnector();

public:
        /* 连接 */
        bool __stdcall connect(const MysqlConnInfo& conn_info);

        /* 执行sql命令 */
        MYSQL_RES* execSqlCmd(const char* sql_cmd);

        /* 单行执行结果 */
        MYSQL_ROW fetchRow(MYSQL_RES* res);

        /* 获取总行数 */
        unsigned int getRowCount(MYSQL_RES* res);

        /* 获取当前行的列数 */
        unsigned int getColCount(MYSQL_RES* res);

        /* 判断是否是结果尾部 */
        bool isEnd(MYSQL_RES* res);

        /* 设置字符集编码 */
        bool setCharset(const char* charset);

        /* 获取当前状态 */
        const char* getState();

        /* 获取错误信息 */
        const char* errorMsg();

        /* 释放结果集 */
        void freeRes(MYSQL_RES* res);

        /* 断开连接 */
        void disconnect();

protected:

        MYSQL* _conn_ptr;

        /* 目前未实现重连 */
        MysqlConnInfo _conn_info;

};


#endif	// __MYSQL_CONNECTOR_H__
