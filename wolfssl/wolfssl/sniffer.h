/* sniffer.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



#ifndef WOLFSSL_SNIFFER_H
#define WOLFSSL_SNIFFER_H

#include <wolfssl/wolfcrypt/settings.h>

#ifdef _WIN32
    #ifdef SSL_SNIFFER_EXPORTS
        #define SSL_SNIFFER_API __declspec(dllexport)
    #else
        #define SSL_SNIFFER_API __declspec(dllimport)
    #endif
#else
    #define SSL_SNIFFER_API
#endif /* _WIN32 */


#ifdef __cplusplus
    extern "C" {
#endif

/* @param typeK: (formerly keyType) was shadowing a global declaration in
 *                wolfssl/wolfcrypt/asn.h line 175
 */
WOLFSSL_API
SSL_SNIFFER_API int ssl_SetPrivateKey(const char* address, int port,
                                      const char* keyFile, int typeK,
                                      const char* password, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetNamedPrivateKey(const char* name,
                                           const char* address, int port,
                                           const char* keyFile, int typeK,
                                           const char* password, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_DecodePacket(const unsigned char* packet, int length,
                                     unsigned char** data, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_FreeDecodeBuffer(unsigned char** data, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_FreeZeroDecodeBuffer(unsigned char** data, int sz,
                                             char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_Trace(const char* traceFile, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_EnableRecovery(int onOff, int maxMemory, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_GetSessionStats(unsigned int* active,
                                        unsigned int* total,
                                        unsigned int* peak,
                                        unsigned int* maxSessions,
                                        unsigned int* missedData,
                                        unsigned int* reassemblyMemory,
                                        char* error);

WOLFSSL_API void ssl_InitSniffer(void);

WOLFSSL_API void ssl_FreeSniffer(void);


/* ssl_SetPrivateKey typeKs */
enum {
    FILETYPE_PEM = 1,
    FILETYPE_DER = 2,
};


/*
 * New Sniffer API that provides read-only access to the TLS and cipher
 * information associated with the SSL session.
 */

#if defined(__GNUC__)
    #define WOLFSSL_PACK __attribute__ ((packed))
#else
    #define WOLFSSL_PACK
#endif


typedef struct SSLInfo
{
    unsigned char  isValid;
            /* indicates if the info in this struct is valid: 0 = no, 1 = yes */
    unsigned char  protocolVersionMajor;    /* SSL Version: major */
    unsigned char  protocolVersionMinor;    /* SSL Version: minor */
    unsigned char  serverCipherSuite0;      /* first byte, normally 0 */
    unsigned char  serverCipherSuite;       /* second byte, actual suite */
    unsigned char  serverCipherSuiteName[256];
            /* cipher name, e.g., "TLS_RSA_..." */
    unsigned char  serverNameIndication[128];
    unsigned int   keySize;
} WOLFSSL_PACK SSLInfo;


WOLFSSL_API
SSL_SNIFFER_API int ssl_DecodePacketWithSessionInfo(
                        const unsigned char* packet, int length,
                        unsigned char** data, SSLInfo* sslInfo, char* error);

typedef void (*SSLConnCb)(const void* session, SSLInfo* info, void* ctx);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetConnectionCb(SSLConnCb cb);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetConnectionCtx(void* ctx);


typedef struct SSLStats
{
    unsigned long int sslStandardConns;
    unsigned long int sslClientAuthConns;
    unsigned long int sslResumedConns;
    unsigned long int sslEphemeralMisses;
    unsigned long int sslResumeMisses;
    unsigned long int sslCiphersUnsupported;
    unsigned long int sslKeysUnmatched;
    unsigned long int sslKeyFails;
    unsigned long int sslDecodeFails;
    unsigned long int sslAlerts;
    unsigned long int sslDecryptedBytes;
    unsigned long int sslEncryptedBytes;
    unsigned long int sslEncryptedPackets;
    unsigned long int sslDecryptedPackets;
    unsigned long int sslKeyMatches;
    unsigned long int sslEncryptedConns;
} SSLStats;


WOLFSSL_API
SSL_SNIFFER_API int ssl_ResetStatistics(void);


WOLFSSL_API
SSL_SNIFFER_API int ssl_ReadStatistics(SSLStats* stats);


WOLFSSL_API
SSL_SNIFFER_API int ssl_ReadResetStatistics(SSLStats* stats);


typedef int (*SSLWatchCb)(void* vSniffer,
                        const unsigned char* certHash,
                        unsigned int certHashSz,
                        const unsigned char* certChain,
                        unsigned int certChainSz,
                        void* ctx, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetWatchKeyCallback(SSLWatchCb cb, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetWatchKeyCtx(void* ctx, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetWatchKey_buffer(void* vSniffer,
                        const unsigned char* key, unsigned int keySz,
                        int keyType, char* error);

WOLFSSL_API
SSL_SNIFFER_API int ssl_SetWatchKey_file(void* vSniffer,
                        const char* keyFile, int keyType,
                        const char* password, char* error);


#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* wolfSSL_SNIFFER_H */
