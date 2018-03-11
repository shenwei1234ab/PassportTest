/*
 * RsaSign.cpp
 *
 *  Created on: 2015年8月16日
 *      Author: leave
 */

#include "RsaSign.h"
#include <assert.h>
#include "vnLog.h"


#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

template<HashCreateFn HashCreator>
RsaSign<HashCreator>::RsaSign() {
	EVP_MD_CTX_init(&_ctx);
	EVP_SignInit_ex(&_ctx,  HashCreator(), NULL);
}

template<HashCreateFn HashCreator>
RsaSign<HashCreator>::~RsaSign() {
	EVP_MD_CTX_cleanup(&_ctx);
}
template<HashCreateFn HashCreator>
void RsaSign<HashCreator>::UpdateData(std::string const& str) {
	EVP_SignUpdate(&_ctx, (uint8 const*)str.c_str(), str.length());
}

template<HashCreateFn HashCreator>
void RsaSign<HashCreator>::UpdateData(uint8 const* data,
		size_t len) {
	EVP_SignUpdate(&_ctx, data, len);
}

template<HashCreateFn HashCreator>
std::string& RsaSign<HashCreator>::Finalize(EVP_PKEY* pkey) {
	 uint32 length = 0;
	 sign.resize(EVP_PKEY_size(pkey));
	 EVP_SignFinal(&_ctx, (unsigned char*)sign.data(), &length,pkey);
	 return sign;
}

template class RsaSign<EVP_sha1>;
