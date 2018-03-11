////============================================================================
//// Name        : MyProject.cpp
//// Author      : iapppay
//// Version     :
//// Copyright   : Your copyright notice
//// Description : Hello World in C++, Ansi-style
////============================================================================
//
//#include <iostream>
//using namespace std;
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
//#include <string.h>
//#include <sys/types.h>
//
//#include "rsa.h"
//#include "base64.h"
//#include "md5.h"
//
//char *trans_data = "{\"exorderno\":\"10004200000001100042\",\"transid\":\"02113013118562300203\",\"waresid\":1,\"appid\":\"20004600000001200046\",\"feetype\":0,\"money\":3000,\"count\":1,\"result\":0,\"transtype\":0,\"transtime\":\"2013-01-31 18:57:27\",\"cpprivate\":\"123456\"}";
//char * sign = "28adee792782d2f723e17ee1ef877e7 166bc3119507f43b06977786376c0434 633cabdb9ee80044bc8108d2e9b3c86e";
//char * key = "MjhERTEwQkFBRDJBRTRERDhDM0FBNkZBMzNFQ0RFMTFCQTBCQzE3QU1UUTRPRFV6TkRjeU16UTVNRFUyTnpnek9ETXJNVE15T1RRME9EZzROVGsyTVRreU1ETXdNRE0zTnpjd01EazNNekV5T1RJek1qUXlNemN4";
//
//#define MAX_KEY_LEN (64)
//
///*签名验证方法：
// *sKey：应用密钥 需要从爱贝商户自服务获取
// *pTransData:trans_data
// *pSign:sign签名值
// *
// *
// *trans_data和sign为从爱贝同步报文的两个字段，详细信息参考接入文档。
// * */
//int validsign(char *sKey,char *pTransData,int datalen,char *pSign,int signlen)
//{
//	    //获取rsa的密钥，rsa密钥格式:privatekey+modkey
//	    char szCompKey[MAX_KEY_LEN*2+2];
//	    char szPrivateKey[MAX_KEY_LEN+1];
//	    char szModKey[MAX_KEY_LEN+1];
//
//
//	    memset(szCompKey,0,sizeof(szCompKey));
//	    memset(szPrivateKey,0,sizeof(szPrivateKey));
//	    memset(szModKey,0,sizeof(szModKey));
//
//
//
//	    gIAppBase64Decode(sKey,szCompKey);
//
//	    char *pSplit;
//	    pSplit = strchr(szCompKey,'+');
//	    strcpy(szModKey,pSplit+1);   //modkey
//
//	    strncpy(szPrivateKey,szCompKey,pSplit-szCompKey); //privatekey
//
//
//
//	    //开始验证
//	    char *pSignMD5;  //签名值的md5
//	    int nlen;
//	    pSignMD5 = gIAppDecRSA(&nlen,pSign,signlen,szPrivateKey,szModKey);
//
//	    char szOrgMD5[32];
//	    szOrgMD5[0]='\0';
//	    gIAppMD5((char*)pTransData,strlen(pTransData),(char*)szOrgMD5);
//
//	    /*验证比较签名*/
//
//	    return strcmp(szOrgMD5,pSignMD5);
//}
//
//
//int main() {
//	int result = validsign(key,trans_data,strlen(trans_data),sign,strlen(sign));
//
//	printf("ret:%d\n", result);
//
//}
//
