/*
 * RsaVerify.h
 *
 *  Created on: 2015年8月16日
 *      Author: leave
 */

#ifndef SRC_COMMON_CRYPTOGRAPHY_RSAVERIFY_H_
#define SRC_COMMON_CRYPTOGRAPHY_RSAVERIFY_H_

#include "Define.h"
#include <openssl/evp.h>
#include <string>

typedef EVP_MD const* (*HashCreateFn)();

template<HashCreateFn HashCreator>
class RsaVerify {
public:
	RsaVerify();
	virtual ~RsaVerify();

	void UpdateData(std::string const& str);
	void UpdateData(uint8 const* data, size_t len);
	bool Finalize(EVP_PKEY* pkey,std::string& sign);

private:
	EVP_MD_CTX _ctx;
};

typedef RsaVerify<EVP_sha1> RsaVerifySha1;

#endif /* SRC_COMMON_CRYPTOGRAPHY_RSAVERIFY_H_ */
