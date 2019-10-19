/*
 * mysql_connector.cpp
 *
 *  Created on: Apr 3, 2019
 *      Author: wenhan
 */

#include <cstdio>
#include <string>

#include "MysqlConnector.h"

#include <iostream>
using namespace std;

MysqlConnector::MysqlConnector()
        : _conn_ptr(NULL)
{

}

MysqlConnector::~MysqlConnector()
{
        disconnect();
}

bool MysqlConnector::connect(const MysqlConnInfo &conn_info)
{
        if (_conn_ptr) 
		{
                return true;
        }

        // 保存连接信息，可做重连使用
        _conn_info = conn_info;
        _conn_ptr = new MYSQL();

        if (!mysql_init(_conn_ptr)) 
		{
                printf("\nmysql_init error\n");
                delete _conn_ptr;
                _conn_ptr = NULL;

                return false;
        }
		
		std::string s;
		for (size_t i = 0 ; i < conn_info.password.size(); i++) 
		{
			s += "*";
		}

        printf("%s(%d): mysql host = %s, db = %s, user = %s, pass = %s\n", __FILE__, __LINE__,
                conn_info.ip_addr.c_str(), conn_info.db_name.c_str(), conn_info.username.c_str(),
				s.c_str());
//                conn_info.password.c_str());

        if (!mysql_real_connect(_conn_ptr, conn_info.ip_addr.c_str(),
                        conn_info.username.c_str(), conn_info.password.c_str(),
                        conn_info.db_name.c_str(), 3306,
                        NULL, 0)) 
		{
                printf("%s(%d): connection mysql fails\n", __FILE__, __LINE__);
                printf("failed to connect to database: error: [%s]\n", this->errorMsg());
                delete _conn_ptr;
                _conn_ptr = NULL;

                return false;
        }

        printf("%s(%d): connection mysql success\n", __FILE__, __LINE__);

        return true;

}

MYSQL_RES* MysqlConnector::execSqlCmd(const char* sql_cmd)
{
        MYSQL_RES * res = NULL;

        if (mysql_query(_conn_ptr, sql_cmd))	// 执行语句不成功
		{
			printf("\texcute error: %s\n\t%s\n", sql_cmd, this->errorMsg());
			
            return NULL;
        }

        res = mysql_store_result(_conn_ptr);

        return res;
}

void MysqlConnector::freeRes(MYSQL_RES *res)
{
        mysql_free_result(res);
}

MYSQL_ROW MysqlConnector::fetchRow(MYSQL_RES *res)
{
        return mysql_fetch_row(res);
}

unsigned int MysqlConnector::getRowCount(MYSQL_RES *res)
{
        return (unsigned int)mysql_num_rows(res);
}

unsigned int MysqlConnector::getColCount(MYSQL_RES *res)
{
        return mysql_num_fields(res);
}

bool MysqlConnector::isEnd(MYSQL_RES *res)
{
        /* 如果到达结果集尾部，mysql_eof()返回非0值；
         * 如果出现错误，则返回0
         */
        if (!mysql_eof(res)) 
		{
                return false;
        }

        return true;
}

const char* MysqlConnector::getState()
{
        return mysql_stat(_conn_ptr);
}

bool MysqlConnector::setCharset(const char* charset)
{
        char sql_cmd[128] = {0};
        snprintf(sql_cmd, sizeof(sql_cmd) - 1, "set names %s", charset);

        if (!_conn_ptr) 
		{
                return false;
        }

        if (mysql_query(_conn_ptr, sql_cmd)) 
		{
                return true;
        }

        return false;
}
const char* MysqlConnector::errorMsg()
{
        return mysql_error(_conn_ptr);
}

void MysqlConnector::disconnect()
{
        if (_conn_ptr) 
		{
                mysql_close(_conn_ptr);
                delete _conn_ptr;
                _conn_ptr = NULL;
        }
}

