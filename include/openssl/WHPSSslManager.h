/*
 * WHPSSslManager.h
 *
 *  Created on: Dec 26, 2019
 *      Author: wenhan
 */

#ifndef TESTS_OPENSSL_TEST_WHPSSSLMANAGER_H_
#define TESTS_OPENSSL_TEST_WHPSSSLMANAGER_H_

#include <openssl/ssl.h>
#include <openssl/err.h>

/*
 * https服务功能管理类；
 * 主要功能：
 * ① 初始化ssl server各项参数
 * todo 后续需要改成单例模式(与 WHPSTcpServer 一样)
 */
class WHPSSslManager
{
public:
        WHPSSslManager();
        ~WHPSSslManager();

public:
        /* 是否初始化成功 */
        const bool& isInit();

        /* 初始化 ssl server 各项参数 */
        bool init();

        const SSL_CTX* sslCtx();
        SSL* createSsl();
        void freeSsl(SSL* ssl);

private:
        /* 协商最高可用 SSL/TLS 版本 */
        bool setServerMethod();

        /* 設置证书文件 */
        bool setCertificateFile();

        /* 设置私钥文件 */
        bool setPrivateKey();
private:
        SSL_CTX* m_sslCtx;
        const SSL_METHOD* m_sslMethod;
        X509* m_servCert;
        bool m_isInit/* = false*/;      // 是否初始化
};

#endif /* TESTS_OPENSSL_TEST_WHPSSSLMANAGER_H_ */
