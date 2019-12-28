/*
 * WHPSSslManager.cpp
 *
 *  Created on: Dec 26, 2019
 *      Author: wenhan
 */

#include "WHPSSslManager.h"

#include <iostream>
using namespace std;

const char* certificateKey = "ca.crt";
const char* privateKey = "ca.key";

WHPSSslManager::WHPSSslManager()
        : m_sslCtx(nullptr),
          m_sslMethod(nullptr),
          m_servCert(nullptr)
{
        // 库初始化
        SSL_library_init();
//        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
}

WHPSSslManager::~WHPSSslManager()
{
        if (m_sslCtx)
        {
                SSL_CTX_free(m_sslCtx);
                m_sslCtx = nullptr;
        }
}

bool WHPSSslManager::init()
{
        if (!this->setServerMethod())
        {
                return false;
        }

        m_sslCtx = SSL_CTX_new(m_sslMethod);
        if (nullptr == m_sslCtx)
        {
                cout << "nullptr == m_sslCtx" << endl;
                return false;
        }

        if (!this->setCertificateFile())
        {
                return false;
        }

        if (!this->setPrivateKey())
        {
                return false;
        }

        SSL_CTX_check_private_key(m_sslCtx);

        return true;
}

SSL_CTX* const WHPSSslManager::sslCtx() const
{
        return m_sslCtx;
}

bool WHPSSslManager::setServerMethod()
{
        m_sslMethod = SSLv23_server_method();
        if (nullptr == m_sslMethod)
        {
                cout << "nullptr == m_sslMethod" << endl;
                return false;
        }

        return true;
}

bool WHPSSslManager::setCertificateFile()
{
        int res = SSL_CTX_use_certificate_file(m_sslCtx, certificateKey, SSL_FILETYPE_PEM);
        if (res < 0)
        {
                return false;
        }

        return true;
}

bool WHPSSslManager::setPrivateKey()
{
        int res = SSL_CTX_use_PrivateKey_file(m_sslCtx, privateKey, SSL_FILETYPE_PEM);
        if (res < 0)
        {
                return false;
        }

        return true;
}
