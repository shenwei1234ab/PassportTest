/*
 * RsaVerify.cpp
 *
 *  Created on: 2015年8月16日
 *      Author: leave
 */

#include "RsaVerify.h"
#include "vnLog.h"


#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

template<HashCreateFn HashCreator>
RsaVerify<HashCreator>::RsaVerify() {
	EVP_MD_CTX_init(&_ctx);
	EVP_VerifyInit_ex(&_ctx, HashCreator(), NULL);

}
template<HashCreateFn HashCreator>
RsaVerify<HashCreator>::~RsaVerify() {
	EVP_MD_CTX_cleanup(&_ctx);
}

template<HashCreateFn HashCreator>
void RsaVerify<HashCreator>::UpdateData(std::string const& str) {
	EVP_VerifyUpdate(&_ctx, (uint8 const* )str.c_str(), str.length());
}

template<HashCreateFn HashCreator>
void RsaVerify<HashCreator>::UpdateData(uint8 const* data, size_t len) {
	EVP_VerifyUpdate(&_ctx, data, len);
}

template<HashCreateFn HashCreator>
bool RsaVerify<HashCreator>::Finalize(EVP_PKEY* pkey,
		std::string& sign) {
	if(!pkey){
		return false;
	}
	int ret = EVP_VerifyFinal(&_ctx,  (const unsigned char *)sign.data(), sign.length(), pkey);
	if (ret == 0) {
		return true;
	}
	return false;
}

template class RsaVerify<EVP_sha1>;
