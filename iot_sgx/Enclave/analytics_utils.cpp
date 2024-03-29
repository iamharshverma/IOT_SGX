//
// Created by shihab on 3/29/19.
//

#include "analytics_utils.h"
#include <stdio.h>
//#include <cstring>
#include <string.h>
#include <algorithm>

#include "Enclave.h"
#include "Enclave_t.h"


static sgx_aes_gcm_128bit_key_t key2 = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };

static const unsigned char gcm_key[] = {
        0xee, 0xbc, 0x1f, 0x57, 0x48, 0x7f, 0x51, 0x92, 0x1c, 0x04, 0x65, 0x66, 0x5f, 0x8a, 0xe6, 0xd1
};
static const unsigned char gcm_iv[] = {
        0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
};
static const unsigned char gcm_aad[] = {
        0x4d, 0x23, 0xc3, 0xce, 0xc3, 0x34, 0xb4, 0x9b, 0xdb, 0x37, 0x0c, 0x43,
        0x7f, 0xec, 0x78, 0xde
};


void check_error_code(sgx_status_t stat){
    printf("\nSTATUS: %d \n",stat);
//    switch (stat){
//        case SGX_SUCCESS:
//            printf("SGX_SUCCESS\n");
//            break;
//        case SGX_ERROR_INVALID_PARAMETER:
//            printf("SGX_ERROR_INVALID_PARAMETER\n");
//            break;
//        case SGX_ERROR_MAC_MISMATCH:
//            printf("SGX_ERROR_MAC_MISMATCH\n");
//            break;
//        case SGX_ERROR_OUT_OF_MEMORY:
//            printf("SGX_ERROR_OUT_OF_MEMORY\n");
//            break;
//        case SGX_ERROR_UNEXPECTED:
//            printf("SGX_ERROR_UNEXPECTED\n");
//            break;
//        default:
//            printf("Unknown error\n");
//    }
    return;
}



void decryptMessageAES(char *encMessageIn, size_t len, char *decMessageOut, size_t lenOut, char *tag){
    printf("Started Test Decryption.....");
    sgx_aes_gcm_128bit_key_t *key = (sgx_aes_gcm_128bit_key_t*)gcm_key;
    uint8_t *iv = (uint8_t *)gcm_iv;
    uint8_t *aad = (uint8_t *)gcm_aad;
    uint8_t  *encMessage = (uint8_t *)encMessageIn;
    uint8_t p_dst[BUFLEN] = {0};
    sgx_status_t stat = sgx_rijndael128GCM_decrypt(key, encMessage, len, p_dst, iv, SGX_AESGCM_IV_SIZE, aad, 16, (sgx_aes_gcm_128bit_tag_t *) tag );
    check_error_code(stat);
    memcpy(decMessageOut, p_dst, lenOut);
}


void encryptMessageAES(char *decMessageIn, size_t len, char *encMessageOut, size_t lenOut, char *tagMessageIn){
    sgx_aes_gcm_128bit_key_t *key = (sgx_aes_gcm_128bit_key_t*)gcm_key;
    uint8_t *iv = (uint8_t *)gcm_iv;
    uint8_t *aad = (uint8_t *)gcm_aad;
    uint8_t  *decMessage = (uint8_t *)decMessageIn;
    uint8_t p_dst[BUFLEN] = {0};

    uint8_t p_dst2[BUFLEN] = {0};
//    (sgx_aes_gcm_128bit_tag_t *) tag;

    sgx_status_t stat = sgx_rijndael128GCM_encrypt(key, decMessage, len, p_dst, iv, SGX_AESGCM_IV_SIZE, aad, 16, (sgx_aes_gcm_128bit_tag_t *) p_dst2);
    check_error_code(stat);
    memcpy(encMessageOut, p_dst, lenOut);
    memcpy(tagMessageIn, p_dst2, 16);
    printf("ENCRYPTED MESSAGE: %s\n",(char*)encMessageOut);
    printf("Tag: %s\n", (char*)tagMessageIn);
}


