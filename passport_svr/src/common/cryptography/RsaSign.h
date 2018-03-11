/*
 * RsaSign.h
 *
 *  Created on: 2015年8月16日
 *      Author: leave
 */

#ifndef SRC_COMMON_CRYPTOGRAPHY_RSASIGN_H_
#define SRC_COMMON_CRYPTOGRAPHY_RSASIGN_H_
#include "Define.h"
#include <openssl/evp.h>
#include <string>

typedef EVP_MD const* (*HashCreateFn)();

template<HashCreateFn HashCreator>
class RsaSign {
public:
	RsaSign();
	virtual ~RsaSign();
	void UpdateData(std::string const& str);
	void UpdateData(uint8 const* data, size_t len);
	std::string& Finalize(EVP_PKEY* pkey);

private:
	EVP_MD_CTX _ctx;
	std::string sign;
};
typedef RsaSign<EVP_sha1> RsaSignSha1;
#endif /* SRC_COMMON_CRYPTOGRAPHY_RSASIGN_H_ */
