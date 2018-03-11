/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Util.h"
#include <sstream>
#include <assert.h>
#include <vector>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/des.h>

using namespace std;

Tokenizer::Tokenizer(const std::string &src, const char sep, vn::u32 vectorReserve)
{
    m_str = new char[src.length() + 1];
    memcpy(m_str, src.c_str(), src.length() + 1);

    if (vectorReserve)
        m_storage.reserve(vectorReserve);

    char* posold = m_str;
    char* posnew = m_str;

    for (;;)
    {
        if (*posnew == sep)
        {
            m_storage.push_back(posold);
            posold = posnew + 1;

            *posnew = '\0';
        }
        else if (*posnew == '\0')
        {
            // Hack like, but the old code accepted these kind of broken strings,
            // so changing it would break other things
            if (posold != posnew)
                m_storage.push_back(posold);

            break;
        }

        ++posnew;
    }
}


std::string ByteArrayToHexStr(vn::u8 const* bytes, vn::u32 arrayLen, bool reverse /* = false */)
{
    vn::s32 init = 0;
    vn::s32 end = arrayLen;
    vn::s8 op = 1;

    if (reverse)
    {
        init = arrayLen - 1;
        end = -1;
        op = -1;
    }

    std::ostringstream ss;
    for (vn::s32 i = init; i != end; i += op)
    {
        char buffer[4];
        sprintf(buffer, "%02x", bytes[i]);
        ss << buffer;
    }

    return ss.str();
}
void HexStrToByteArray(std::string const& str, vn::u8* out, bool reverse /*= false*/)
{
    // string must have even number of characters
    if (str.length() & 1)
        return;

    vn::s32 init = 0;
    vn::s32 end = str.length();
    vn::s8 op = 1;

    if (reverse)
    {
        init = str.length() - 2;
        end = -2;
        op = -1;
    }

    vn::u32 j = 0;
    for (vn::s32 i = init; i != end; i += 2 * op)
    {
        char buffer[3] = { str[i], str[i + 1], '\0' };
        out[j++] = strtoul(buffer, NULL, 16);
    }
}

#include <openssl/pem.h>
std::string base64_encode(const char *data, int data_len)
{
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bio = BIO_new(BIO_s_mem());

    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, data, data_len);
    BIO_ctrl(bio, BIO_CTRL_FLUSH, 0, NULL);

    BUF_MEM *bptr = NULL;
    BIO_get_mem_ptr(bio, &bptr);

    std::string ret(bptr->data,bptr->length);

    BIO_free_all(bio);
    return ret;
}

int base64_decode(char *str,int str_len, char* decode){
    int len=0;
    BIO *b64,*bmem;
    b64=BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem=BIO_new_mem_buf(str,str_len);
    bmem=BIO_push(b64,bmem);
    len=BIO_read(bmem,decode,str_len);
    decode[len]=0;
    BIO_free_all(bmem);
    return len;
}

template <class T>
T Random(T min,T max){
	std::default_random_engine generator(time(NULL));
	std::uniform_int_distribution<T> dis(min,max);  //均匀分布
	auto dice= std::bind(dis,generator);
	return dice();
}

/* Return a random number in the range min..max. */
vn::s32 irand(vn::s32 min, vn::s32 max){
	return Random<vn::s32>(min,max);
}

/* Return a random number in the range min..max (inclusive). */
vn::u32 urand(vn::u32 min, vn::u32 max){
	return Random<vn::u32>(min,max);
}


vn::u32 rand32(vn::u32 max){
	return Random<vn::u32>(0,max);
}

#define RAND_BUFF_LEN 67
char Base64Code[RAND_BUFF_LEN]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
'U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7',
'8','9','+','/','='};

vn::str8 randStr(vn::u32 length){
	vn::str8 ret;
	srand((unsigned)time(0));
	int iRand = 0;
	for (vn::u32 i = 0; i < length; ++i) {
		iRand = rand() % RAND_BUFF_LEN;            // iRand = 0 - 61
		ret += Base64Code[iRand];
	}
	return ret;
}

unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}
std::string UrlEncode(std::string str){
	return UrlEncode(str.c_str(),str.length());
}
std::string UrlEncode(const char* pszBuf, const int iBufLen)
{
    std::string strTemp;
    for (int i = 0; i < iBufLen; i++)
    {
        if (isalnum((unsigned char)pszBuf[i]) ||
            (pszBuf[i] == '-') ||
            (pszBuf[i] == '_') ||
            (pszBuf[i] == '.') ||
            (pszBuf[i] == '~'))
            strTemp += pszBuf[i];
        else if (pszBuf[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)pszBuf[i] >> 4);
            strTemp += ToHex((unsigned char)pszBuf[i] % 16);
        }
    }
    return std::move(strTemp);
}
std::string UrlDecode(const char* pszBuf, const int iBufLen)
{
    std::string strTemp = "";
    for (int i = 0; i < iBufLen; i++)
    {
        if (pszBuf[i] == '+') strTemp += ' ';
        else if (pszBuf[i] == '%')
        {
            assert(i + 2 < iBufLen);
            unsigned char high = FromHex((unsigned char)pszBuf[++i]);
            unsigned char low = FromHex((unsigned char)pszBuf[++i]);
            strTemp += high*16 + low;
        }
        else strTemp += pszBuf[i];
    }
    return std::move(strTemp);
}

std::string RSA_Encrypt(const char *plain_text,  size_t size,  char *chPrivateKey)
{
	unsigned char *cipher;
	int len;
	RSA *pRSAPrivateKey;
	BIO *bio = NULL;

	if ((bio = BIO_new_mem_buf(chPrivateKey, -1)) == NULL)       //从字符串读取
	{
		VN_LOG_ERROR("BIO_new_mem_buf failed!");
		return NULL ;
	}
	pRSAPrivateKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
	if (NULL == pRSAPrivateKey)
	{
		ERR_print_errors_fp(stdout);
		BIO_free_all(bio);
		VN_LOG_ERROR("PEM_read_bio_RSA_PUBKEY failed!");
		return NULL ;
	}

	len = RSA_size(pRSAPrivateKey);
	if (NULL == (cipher = (unsigned char *) malloc(len + 1)))
	{
		RSA_free(pRSAPrivateKey);
		return NULL ;
	}
	memset(cipher, 0, len + 1);

	if (0 > RSA_public_encrypt(strlen(plain_text), (unsigned char *) plain_text,
					(unsigned char*) cipher, pRSAPrivateKey, RSA_PKCS1_PADDING))// 这里设置填充方式，注意java和openssl的统一
	{
		RSA_free(pRSAPrivateKey);
		free(cipher);
		return NULL ;
	}

	RSA_free(pRSAPrivateKey);
	std::string result = base64_encode((char *) cipher, strlen((char *) cipher));
	free(cipher);
	return result;
}

int RSA_Verify(const char *text, char *sig, char *chPublicKey)
{
	RSA *pRSAPublicKey;
	BIO *bio = NULL;

	if ((bio = BIO_new_mem_buf(chPublicKey, -1)) == NULL)       //从字符串读取RSA公钥
	{
		VN_LOG_ERROR("BIO_new_mem_buf failed!");
		return -1 ;
	}
	pRSAPublicKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
	if (pRSAPublicKey  == NULL )
	{
		ERR_print_errors_fp(stdout);
		BIO_free_all(bio);
		VN_LOG_ERROR("PEM_read_bio_RSA_PUBKEY failed!");
		return -1;
	}

	char sig_temp[250];
	if(0 == base64_decode(sig, (int)strlen(sig), sig_temp))
	{
		VN_LOG_ERROR( "base64_decodeerror!");
		return -1;
	}

	unsigned char temp[20];
	SHA((const unsigned char *) text, strlen(text), temp);
	int ret = RSA_verify(NID_sha1, temp, 20, (unsigned char *)sig_temp, 128, pRSAPublicKey);
	RSA_free(pRSAPublicKey);
	return (ret == 1) ? 0 : -1;
}


std::string ChangeTimeToStr(long long sec,const char* format){
	time_t t = sec+28800;//东八区
	struct tm *p;
	p = gmtime(&t);
	char s[80];
	strftime(s, 80, format, p);
	//printf("%d: %s\n", (int) t, s);
	return s;
}

static unsigned char cbc_iv[8] = {'0', '1', 'A', 'B', 'a', 'b', '9', '8'};
std::string DES_Encrypt(const std::string cleartext, const std::string key, CRYPTO_MODE mode){
	string strCipherText;

	    switch (mode) {
	    case GENERAL:
	    case ECB:
	        {
	            DES_cblock keyEncrypt;
	            memset(keyEncrypt, 0, 8);

	            if (key.length() <= 8)
	                memcpy(keyEncrypt, key.c_str(), key.length());
	            else
	                memcpy(keyEncrypt, key.c_str(), 8);

	            DES_key_schedule keySchedule;
	            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	            const_DES_cblock inputText;
	            DES_cblock outputText;
	            vector<unsigned char> vecCiphertext;
	            unsigned char tmp[8];

	            for (int i = 0; i < (int)cleartext.length() / 8; i ++) {
	                memcpy(inputText, cleartext.c_str() + i * 8, 8);
	                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCiphertext.push_back(tmp[j]);
	            }

	            if ((int)cleartext.length() % 8 != 0) {
	                int tmp1 = (int)cleartext.length() / 8 * 8;
	                int tmp2 = (int)cleartext.length() - tmp1;
	                memset(inputText, 0, 8);
	                memcpy(inputText, cleartext.c_str() + tmp1, tmp2);

	                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCiphertext.push_back(tmp[j]);
	            }

	            strCipherText.clear();
	            strCipherText.assign(vecCiphertext.begin(), vecCiphertext.end());
	        }
	        break;
	    case CBC:
	        {
	            DES_cblock keyEncrypt, ivec;
	            memset(keyEncrypt, 0, 8);

	            if (key.length() <= 8)
	                memcpy(keyEncrypt, key.c_str(), key.length());
	            else
	                memcpy(keyEncrypt, key.c_str(), 8);

	            DES_key_schedule keySchedule;
	            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	            memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	            int iLength = (int)cleartext.length() % 8 ? ((int)cleartext.length() / 8 + 1) * 8 : (int)cleartext.length();
	            unsigned char* tmp = new unsigned char[iLength + 16];
	            memset(tmp, 0, iLength);

	            DES_ncbc_encrypt((const unsigned char*)cleartext.c_str(), tmp, cleartext.length()+1, &keySchedule, &ivec, DES_ENCRYPT);

	            strCipherText = (char*)tmp;

	            delete [] tmp;
	        }
	        break;
	    case CFB:
	        {
	            DES_cblock keyEncrypt, ivec;
	            memset(keyEncrypt, 0, 8);

	            if (key.length() <= 8)
	                memcpy(keyEncrypt, key.c_str(), key.length());
	            else
	                memcpy(keyEncrypt, key.c_str(), 8);

	            DES_key_schedule keySchedule;
	            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	            memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	            unsigned char* outputText = new unsigned char[cleartext.length()];
	            memset(outputText, 0, cleartext.length());

	            const unsigned char* tmp = (const unsigned char*)cleartext.c_str();

	            DES_cfb_encrypt(tmp, outputText, 8, cleartext.length(), &keySchedule, &ivec, DES_ENCRYPT);

	            strCipherText = (char*)outputText;

	            delete [] outputText;
	        }
	        break;
	    case TRIPLE_ECB:
	        {
	            DES_cblock ke1, ke2, ke3;
	            memset(ke1, 0, 8);
	            memset(ke2, 0, 8);
	            memset(ke2, 0, 8);

	            if (key.length() >= 24) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, 8);
	            } else if (key.length() >= 16) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, key.length() - 16);
	            } else if (key.length() >= 8) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, key.length() - 8);
	                memcpy(ke3, key.c_str(), 8);
	            } else {
	                memcpy(ke1, key.c_str(), key.length());
	                memcpy(ke2, key.c_str(), key.length());
	                memcpy(ke3, key.c_str(), key.length());
	            }

	            DES_key_schedule ks1, ks2, ks3;
	            DES_set_key_unchecked(&ke1, &ks1);
	            DES_set_key_unchecked(&ke2, &ks2);
	            DES_set_key_unchecked(&ke3, &ks3);

	            const_DES_cblock inputText;
	            DES_cblock outputText;
	            vector<unsigned char> vecCiphertext;
	            unsigned char tmp[8];

	            for (int i = 0; i < (int)cleartext.length() / 8; i ++) {
	                memcpy(inputText, cleartext.c_str() + i * 8, 8);
	                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_ENCRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCiphertext.push_back(tmp[j]);
	            }

	            if ((int)cleartext.length() % 8 != 0) {
	                int tmp1 = (int)cleartext.length() / 8 * 8;
	                int tmp2 = (int)cleartext.length() - tmp1;
	                memset(inputText, 0, 8);
	                memcpy(inputText, cleartext.c_str() + tmp1, tmp2);

	                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_ENCRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCiphertext.push_back(tmp[j]);
	            }

	            strCipherText.clear();
	            strCipherText.assign(vecCiphertext.begin(), vecCiphertext.end());
	        }
	        break;
	    case TRIPLE_CBC:
	        {
	            DES_cblock ke1, ke2, ke3, ivec;
	            memset(ke1, 0, 8);
	            memset(ke2, 0, 8);
	            memset(ke2, 0, 8);

	            if (key.length() >= 24) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, 8);
	            } else if (key.length() >= 16) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, key.length() - 16);
	            } else if (key.length() >= 8) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, key.length() - 8);
	                memcpy(ke3, key.c_str(), 8);
	            } else {
	                memcpy(ke1, key.c_str(), key.length());
	                memcpy(ke2, key.c_str(), key.length());
	                memcpy(ke3, key.c_str(), key.length());
	            }

	            DES_key_schedule ks1, ks2, ks3;
	            DES_set_key_unchecked(&ke1, &ks1);
	            DES_set_key_unchecked(&ke2, &ks2);
	            DES_set_key_unchecked(&ke3, &ks3);

	            memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	            int iLength = (int)cleartext.length() % 8 ? ((int)cleartext.length() / 8 + 1) * 8 : (int)cleartext.length();
	            unsigned char* tmp = new unsigned char[iLength + 16];
	            memset(tmp, 0, iLength);

	            DES_ede3_cbc_encrypt((const unsigned char*)cleartext.c_str(), tmp, cleartext.length()+1, &ks1, &ks2, &ks3, &ivec, DES_ENCRYPT);

	            strCipherText = (char*)tmp;

	            delete [] tmp;
	        }
	        break;
	    }

	    return strCipherText;
}
std::string DES_Decrypt(const std::string ciphertext, const std::string key, CRYPTO_MODE mode){
	std::string strClearText;

	    switch (mode) {
	    case GENERAL:
	    case ECB:
	        {
	            DES_cblock keyEncrypt;
	            memset(keyEncrypt, 0, 8);

	            if (key.length() <= 8)
	                memcpy(keyEncrypt, key.c_str(), key.length());
	            else
	                memcpy(keyEncrypt, key.c_str(), 8);

	            DES_key_schedule keySchedule;
	            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	            const_DES_cblock inputText;
	            DES_cblock outputText;
	            vector<unsigned char> vecCleartext;
	            unsigned char tmp[8];

	            for (int i = 0; i < (int)ciphertext.length() / 8; i ++) {
	                memcpy(inputText, ciphertext.c_str() + i * 8, 8);
	                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCleartext.push_back(tmp[j]);
	            }

	            if ((int)ciphertext.length() % 8 != 0) {
	                int tmp1 = (int)ciphertext.length() / 8 * 8;
	                int tmp2 = (int)ciphertext.length() - tmp1;
	                memset(inputText, 0, 8);
	                memcpy(inputText, ciphertext.c_str() + tmp1, tmp2);

	                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCleartext.push_back(tmp[j]);
	            }

	            strClearText.clear();
	            strClearText.assign(vecCleartext.begin(), vecCleartext.end());
	        }
	        break;
	    case CBC:
	        {
	            DES_cblock keyEncrypt, ivec;
	            memset(keyEncrypt, 0, 8);

	            if (key.length() <= 8)
	                memcpy(keyEncrypt, key.c_str(), key.length());
	            else
	                memcpy(keyEncrypt, key.c_str(), 8);

	            DES_key_schedule keySchedule;
	            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	            memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	            int iLength = (int)ciphertext.length() % 8 ? ((int)ciphertext.length() / 8 + 1) * 8 : (int)ciphertext.length();
	            unsigned char* tmp = new unsigned char[iLength];
	            memset(tmp, 0, iLength);

	            DES_ncbc_encrypt((const unsigned char*)ciphertext.c_str(), tmp, ciphertext.length()+1, &keySchedule, &ivec, DES_DECRYPT);

	            strClearText = (char*)tmp;

	            delete [] tmp;
	        }
	        break;
	    case CFB:
	        {
	            DES_cblock keyEncrypt, ivec;
	            memset(keyEncrypt, 0, 8);

	            if (key.length() <= 8)
	                memcpy(keyEncrypt, key.c_str(), key.length());
	            else
	                memcpy(keyEncrypt, key.c_str(), 8);

	            DES_key_schedule keySchedule;
	            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	            memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	            unsigned char* outputText = new unsigned char[ciphertext.length()];
	            memset(outputText, 0, ciphertext.length());

	            const unsigned char* tmp = (const unsigned char*)ciphertext.c_str();

	            DES_cfb_encrypt(tmp, outputText, 8, 32/*ciphertext.length() - 16*/, &keySchedule, &ivec, DES_DECRYPT);

	            strClearText = (char*)outputText;

	            delete [] outputText;
	        }
	        break;
	    case TRIPLE_ECB:
	        {
	            DES_cblock ke1, ke2, ke3;
	            memset(ke1, 0, 8);
	            memset(ke2, 0, 8);
	            memset(ke2, 0, 8);

	            if (key.length() >= 24) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, 8);
	            } else if (key.length() >= 16) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, key.length() - 16);
	            } else if (key.length() >= 8) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, key.length() - 8);
	                memcpy(ke3, key.c_str(), 8);
	            } else {
	                memcpy(ke1, key.c_str(), key.length());
	                memcpy(ke2, key.c_str(), key.length());
	                memcpy(ke3, key.c_str(), key.length());
	            }

	            DES_key_schedule ks1, ks2, ks3;
	            DES_set_key_unchecked(&ke1, &ks1);
	            DES_set_key_unchecked(&ke2, &ks2);
	            DES_set_key_unchecked(&ke3, &ks3);

	            const_DES_cblock inputText;
	            DES_cblock outputText;
	            vector<unsigned char> vecCleartext;
	            unsigned char tmp[8];

	            for (int i = 0; i < (int)ciphertext.length() / 8; i ++) {
	                memcpy(inputText, ciphertext.c_str() + i * 8, 8);
	                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_DECRYPT);
	                memcpy(tmp, outputText, 8);

	                for (int j = 0; j < 8; j++)
	                    vecCleartext.push_back(tmp[j]);
	            }

	            if ((int)ciphertext.length() % 8 != 0) {
	                int tmp1 = (int)ciphertext.length() / 8 * 8;
	                int tmp2 = (int)ciphertext.length() - tmp1;
	                memset(inputText, 0, 8);
	                memcpy(inputText, ciphertext.c_str() + tmp1, tmp2);

	                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_DECRYPT);
	                memcpy(tmp, outputText, tmp2);

	                for (int j = 0; j < tmp2; j++)
	                    vecCleartext.push_back(tmp[j]);
	            }

	            strClearText.clear();
	            strClearText.assign(vecCleartext.begin(), vecCleartext.end());
	        }
	        break;
	    case TRIPLE_CBC:
	        {
	            DES_cblock ke1, ke2, ke3, ivec;
	            memset(ke1, 0, 8);
	            memset(ke2, 0, 8);
	            memset(ke2, 0, 8);

	            if (key.length() >= 24) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, 8);
	            } else if (key.length() >= 16) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, 8);
	                memcpy(ke3, key.c_str() + 16, key.length() - 16);
	            } else if (key.length() >= 8) {
	                memcpy(ke1, key.c_str(), 8);
	                memcpy(ke2, key.c_str() + 8, key.length() - 8);
	                memcpy(ke3, key.c_str(), 8);
	            } else {
	                memcpy(ke1, key.c_str(), key.length());
	                memcpy(ke2, key.c_str(), key.length());
	                memcpy(ke3, key.c_str(), key.length());
	            }

	            DES_key_schedule ks1, ks2, ks3;
	            DES_set_key_unchecked(&ke1, &ks1);
	            DES_set_key_unchecked(&ke2, &ks2);
	            DES_set_key_unchecked(&ke3, &ks3);

	            memcpy(ivec, cbc_iv, sizeof(cbc_iv));

	            int iLength = (int)ciphertext.length() % 8 ? ((int)ciphertext.length() / 8 + 1) * 8 : (int)ciphertext.length();
	            unsigned char* tmp = new unsigned char[iLength];
	            memset(tmp, 0, iLength);

	            DES_ede3_cbc_encrypt((const unsigned char*)ciphertext.c_str(), tmp, ciphertext.length()+1, &ks1, &ks2, &ks3, &ivec, DES_DECRYPT);

	            strClearText = (char*)tmp;

	            delete [] tmp;
	        }
	        break;
	    }

	    return strClearText;
}

std::map<std::string, std::string> GetURLParameter(std::string content,bool isDecode ) {
	std::map<std::string, std::string> valueMap;
	const char * split1 = "?&";
	char * valueKey = strtok((char*)content.c_str(), split1);
	while (valueKey != NULL) {
		std::string tmpStr = valueKey;
		size_t pos = tmpStr.find("=");
		if (pos != std::string::npos) {
			std::string value =tmpStr.substr(pos + 1);
			if (isDecode) {
				valueMap[tmpStr.substr(0, pos)] = UrlDecode(value.c_str(), value.length());
			} else {
				valueMap[tmpStr.substr(0, pos)] = value;
			}
		}
		valueKey = strtok(NULL, split1);
	}
	return std::move(valueMap);
}
std::string SetURLParameter(std::map<std::string, std::string>& valueMap,bool isNull ) {
	std::string retStr = "";
	bool isNotStart = false;
	//std::map 默认是升序排列的
	std::map<std::string, std::string>::iterator itrValueMap = valueMap.begin();
	for (; itrValueMap != valueMap.end(); itrValueMap++) {
		if (itrValueMap->second.empty() && !isNull) {
			continue;
		}
		if (isNotStart) {
			retStr += '&';
		}
		retStr += itrValueMap->first;
		retStr += '=';
		retStr += itrValueMap->second;
		isNotStart = true;
	}
	VN_LOG_DEBUG("GetURLParameter: " << retStr);
	return std::move(retStr);
}
