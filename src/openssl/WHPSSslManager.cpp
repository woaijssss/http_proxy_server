/*
 * WHPSSslManager.cpp
 *
 *  Created on: Dec 26, 2019
 *      Author: wenhan
 */

#include "WHPSSslManager.h"

#include <iostream>
using namespace std;

const char* certificateKey = "conf/cacert/ca.crt";
const char* privateKey = "conf/cacert/ca.key";

WHPSSslManager::WHPSSslManager()
        : m_sslCtx(nullptr),
          m_sslMethod(nullptr),
          m_servCert(nullptr),
          m_isInit(false)
{
        // 库初始化
        SSL_library_init();
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

const bool& WHPSSslManager::isInit()
{
        return m_isInit;
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
        m_isInit = true;

        return true;
}

const SSL_CTX* WHPSSslManager::sslCtx()
{
        return m_sslCtx;
}

SSL* WHPSSslManager::createSsl()
{
        if (m_sslCtx)
        {
                return SSL_new(m_sslCtx);
        }

        return nullptr;
}

void WHPSSslManager::freeSsl(SSL* ssl)
{
//        SSL_shutdown(ssl);
        SSL_free(ssl);
}

bool WHPSSslManager::setServerMethod()
{
        m_sslMethod = SSLv23_server_method();
        if (nullptr == m_sslMethod)
        {
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
