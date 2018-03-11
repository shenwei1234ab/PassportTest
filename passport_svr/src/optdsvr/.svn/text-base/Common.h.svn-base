/*
 * common.h
 *
 *  Created on: 2015年6月24日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_COMMON_H_
#define SRC_PASSPORTSVR_COMMON_H_

/** 第三方平台id
 */
enum PLATFORM_ID {
	PLATFORM_ID_ERROR = 0, //不存在，用来检测错误
	PLATFORM_ID_TEST = 1, //官服*
	PLATFORM_ID_HUAWEI = 2, //华为*
	PLATFORM_ID_XIAOMI = 3, //小米*
	PLATFORM_ID_LENOVO = 4, //联想*
	PLATFORM_ID_KUAIYONG = 5, //快用苹果助手*
	PLATFORM_ID_360 = 6, //360*
	PLATFORM_ID_ANZHI = 7, //安智*
	PLATFORM_ID_UC = 8, //UC*
	PLATFORM_ID_XMW = 9, //熊猫玩*
	PLATFORM_ID_BAIDU = 10, //百度*
	PLATFORM_ID_4399 = 11, //4399*
	PLATFORM_ID_OPPO = 12, //OPPO*
	PLATFORM_ID_PPTV = 13, //PPTV*
	PLATFORM_ID_GIONEE = 14, //金立*
	PLATFORM_ID_FLYME = 15, //魅族*
	PLATFORM_ID_APPCHINA = 16, //应用汇*
	PLATFORM_ID_I4 = 17, //爱思*
	PLATFORM_ID_XY = 18, //XY*
	PLATFORM_ID_19196 = 19, //19196*
	PLATFORM_ID_WANDOUJIA = 20, //豌豆夹*
	PLATFORM_ID_MUZHIWAN = 21, //拇指玩*
	PLATFORM_ID_DOWNJOY = 22, //当乐*
	PLATFORM_ID_COOLYUN = 23, //酷派*
	PLATFORM_ID_CCPLAY = 24, //虫虫助手*
	PLATFORM_ID_QQ = 25, //QQ*
	PLATFORM_ID_WX = 26, //微信*
	PLATFORM_ID_TBT = 27, //同步推*
	PLATFORM_ID_VIVO = 28,	//VIVO*
	PLATFORM_ID_HAIMA_AND = 29, //海马玩(安卓)*
	PLATFORM_ID_HAIMA_IOS = 30, //海马玩（ios）*
	PLATFORM_ID_PP_IOS = 31, //PP助手（IOS）
	PLATFORM_ID_ITOOLS = 32, //itools
	PLATFORM_ID_QIYPPS = 33, //爱其艺PPS*
	PLATFORM_ID_YOUKU = 34, //优酷土豆*
	PLATFORM_ID_SOGOU = 35, //搜狗*
	PLATFORM_ID_SINA = 36, //新浪*
	PLATFORM_ID_IAPPLE = 37, //I苹果*
	PLATFORM_ID_GFAN = 38, //鸡疯*
	//PLATFORM_ID_DIDI = 39, //滴滴打车（占用）
	PLATFORM_ID_GUOPAN_AND = 40, //果盘（Android）
	PLATFORM_ID_GUOPAN_IOS = 41, //果盘（IOS）


	PLATFORM_ID_COUNT,
	PLATFORM_ID_VISITOR = 9999 //visitor
};

/** 操作系统id */
enum OS_ID {
	OS_ID_IOS = 0, OS_ID_ANDROID = 1, OS_ID_WIN = 2,
};

enum GAME_ID {
	GAME_ID_ICE_FIRE = 1, //冰与火传说

	GAME_ID_NUM,
};

/** 错误码 */
enum ERROR_CODE
	: int {
		ERROR_CODE_OK = 0,
				ERROR_CODE_LOGIN_VALIDATE = 1, //第三方校验失败
	ERROR_CODE_LOGIN_TOKEN = 2,		//token过期

	ERROR_CODE_ORDER_NO_GAME = 3,	//找不到该游戏
	ERROR_CODE_ORDER_NO_PRODUCT = 4,	//找不到该商品
	ERROR_CODE_ORDER_NO_SERVERID = 5, //游戏服非法
	ERROR_CODE_ORDER_NO_CONNECT = 6,  //无法连接游戏服
	ERROR_CODE_ORDER_CREART = 7,  //订单创建失败

	ERROR_CODE_ORDER_NO_ORDERID = 8,  //找不到该订单
	ERROR_CODE_ORDER_REPEAT_RECEIPT = 9,  //重复的订单凭据
	ERROR_CODE_ORDER_REPEAT_ORDER = 10,  //已经成功发货的订单，不能重复发货
	ERROR_CODE_ORDER_CLANCEL = 11, //订单取消

	ERROR_CODE_Player_NOPlayer = 12, //找不到该玩家

	ERROR_CODE_STATE_UNLOGIN = 13,   //未登录

	//官服
	ERROR_CODE_REGISTER_FAIL_ACCOUNT_ILLEGAL_SIZE = 14,		//注册失败-帐号长度失败
	ERROR_CODE_REGISTER_FAIL_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL = 15, //注册失败-帐号未知的符号
	ERROR_CODE_REGISTER_FAIL_ACCOUNT_USED = 16, 				//注册失败-帐号已存在
	ERROR_CODE_REGISTER_FAIL_PASSWORD_SIZE = 17,				//注册失败-密码长度失败
	ERROR_CODE_REGISTER_FAIL_PALTFORM_ID_ERROR = 18,       //注册失败-平台ID错误
	ERROR_CODE_REGISTER_FAIL_INSERT_ACOUNT_LONGSGOO_FAIL = 20,  //注册失败-帐号写入数据库失败
	ERROR_CODE_REGISTER_FAIL_INSERT_ACOUNT_FAIL = 21,			//注册失败-帐号写入数据库失败

	ERROR_CODE_LOGIN_FAIL_ACCOUNT_UNFINED = 19,   //登录失败-帐号不存在
	ERROR_CODE_LOGIN_FAIL_ACCOUNT_ILLEGAL_SIZE = 22,   //登录失败-帐号长度失败
	ERROR_CODE_LOGIN_FAIL_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL = 23, //登录失败-帐号未知的符号
	ERROR_CODE_LOGIN_FAIL_PASSWORD_SIZE = 24,        //登录失败-密码长度不对
	ERROR_CODE_LOGIN_FAIL_ACCOUNT_NOT_FOUND = 25,    //登录失败-账户不存在
	ERROR_CODE_LOGIN_FAIL_ERROR_PASSWORD = 26,       //登录失败-密码错误

	ERROR_CODE_ACCOUNT_ILLEGAL_SIZE = 27,            //帐号验证-长度不合法
	ERROR_CODE_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL = 28,  //帐号验证-帐号未知的符号
	ERROR_CODE_ACCOUNT_ACCOUNT_USED = 29,            //帐号验证-帐号已经存在
	ACCOUNT_OK = 30,

	ERROR_CODE_NICKNAME_ILLEGAL_SIZE = 31,           //昵称验证-长度不合法
	ERROR_CODE_NICKNAME_ILLEGAL_SYMBOL = 32,         //昵称验证-不非法的字符
	ERROR_CODE_NICKNAME_USED = 33,                   //昵称验证-已存在

	ERROR_CODE_REGISTER_FAIL_PHONENUMBER_ILLEGAL = 34, //注册失败 手机无效

	ERROR_CODE_PAY_CLOSE = 35 //支付关闭
};

//密码长度错误
//const static int ERROR_CODE_LOGIN_FAIL_ERROR_ACCOUNT_NOT_FIND = 21;//帐号不存在

/** 订单操作 */
enum ORDER_OPT {
	ORDER_OPT_CLEAN = 1, //取消
	ORDER_OPT_DELAY = 2, //延时
};

/** 订单状态 */
enum ORDER_STATUS {
	ORDER_STATUS_NO_PAY, //未支付
	ORDER_STATUS_PAY, //已支付，并验证完成
//	ORDER_STATUS_NOTICE ,//已成功通知游戏服发货
	ORDER_STATUS_CONFIRM, //游戏服发货成功
};

const static std::string TEST_PbuKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRA\r\n"
		"FljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQE\r\n"
		"B/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5Ksi\r\n"
		"NG9zpgmLCUYuLkxpLQIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
const static std::string XIAOMI_AppID = "2882303761517392454";
const static std::string XIAOMI_AppKey = "5671739262454";
const static std::string XIAOMI_AppSecret = "LK5Cj1+k18FYa9mYiYTyjw==";

const static std::string HUAWEI_AppID = "10327193";
const static std::string HUAWEI_AppKey = "";
const static std::string HUAWEI_AppSecret = "oxhzzpl8m7pfi5oeuxz6iwxi2ovanea9";
const static std::string HUAWEI_PubKey_TEST = "-----BEGIN PUBLIC KEY-----\r\n"
		"MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAIW1g+KAqqOeC1ypte8L3qT\r\n"
		"Dk2nz6jUbM6o6Jg9obvivPnCAm/wZvV3jWbYWfOuO/wrFJygn/jZqf8\r\n"
		"cR1T1CQa8CAwEAAQ==\r\n"
		"-----END PUBLIC KEY-----\r\n";
const static std::string HUAWEI_PubKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAPBFRNgPeWTyJUP5beKdIV0aSfAHyO\r\n"
		"qYWMYYwaXQY8p1px+sibPg5QtIWfjT2cd5ugOONoKNV++O2mRYGG7SJjsCAwEA\r\n"
		"AQ==\r\n"
		"-----END PUBLIC KEY-----\r\n";

const static std::string LENOVO_AppID = "1508140542842.app.ln";
const static std::string LENOVO_AppKey =
		"-----BEGIN PRIVATE KEY-----\r\n"
				"MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAMG9BhP+4j4IdMDEPwxq6ZPdF/Ev\r\n"
				"byeDXkWHj2TWxPbQEKpvHCvGPTyfdio5N4J0dcVWKBz85PV/YFb4v3ExJq0WHAxZzeEHqp9msB+U\r\n"
				"OspFiqA/+zhlGjcWhqmPBN85APUZDGZ5XOi5abatDEhcV+6rzBFW7ol+oYTNl+H6Qu7nAgMBAAEC\r\n"
				"gYEAvWwgXjqOYBKKfNMjxfNeT0kTmKhFKMAD0z14kSJ/z8WdFdavF4mq4/QRZ7UrtexCHeBgNf95\r\n"
				"qSKSQlbRb02cPlajYqah22jx6bQyDvQj2vkkTEmYgdX6ox488fsPxgwBcJOdjn3RC2blRJb2LCtO\r\n"
				"6tyVqUJV3VQTayT3BsCCVNECQQD7wrt4/90G8QKB8YeNiRGrKWw4f1wIIiL4J25QmHhzrGMDo4Nq\r\n"
				"GQ7UUlqyyoXw+AkJPSMZuXFHEYTgtiOPGn55AkEAxQAsjL5tHPhDTQcqeyzCuAjxlu+hvFo3VCrB\r\n"
				"9zngt4yKfRGHEqOLC8YmqzV8Vnu/L+8jtfUnIqUJmqj5J14AXwJATZFR8ZXjo5/eBetEuHbi2rmI\r\n"
				"fl+/G0JpQ8viUTKo6frTBuv6MVZHG2PEThHX8sZ5Aj+z8NkR7bOYHSQBELY5AQJAT2rGiuaprigX\r\n"
				"WZIt+gINbYuNeT+ZgAkOjQu/rh7TDKvHmKvcPBNaFqEHLbYeLe8tH/HT/6KY6EqIGKVN7CTlGQJA\r\n"
				"a8c6OXJv+4hakf0UyCMFQxcsH5awTSBXdYNj/LShaDP6L/gSh04foIwe1We/TL/XqH8Yh0HaJoVt\r\n"
				"YmCbZyLG5w==\r\n"
				"-----END PRIVATE KEY-----\r\n";

const static std::string AppSecret_360 = "ff87d00b6c7f685501de702a73e7b845";

//****** 快用 **********************
const static std::string KUAIYONG_AppKey = "69644641fa9b8f309b7d55a61befac5a";
const static std::string KUAIYONG_PublicKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCxclyzBStpxfGx3iXsqm3V5vHt\r\n"
		"U14zfGTVhSYVNeOOfnC6GRmN+piRuF7Lz5i+MQi9+svOoM96LLE3rhvTy1i2srso\r\n"
		"entqJFboNZ8u/HbTEIis1gh1yu874lD+zUB/ncF8rofLjpmIGxUp1KonxLHYSmkm\r\n"
		"ew27Cc+Cq9yW7Gf4cwIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
//****** 快用 END**********************

const static std::string ANZHI_AppID = "1444358687pcvkJI4bL2f7xjuLu5xx";
const static std::string ANZHI_AppSecret = "LS3io6mRL64QPS231Yx72r52";

const static std::string UC_GameID = "576587";
const static std::string UC_CPID = "57362";
const static std::string UC_ApiKey = "c52ab699f6736197fd343693b4240817";

//******XMWAN*******************************************************************
const static std::string XMWAN_ClientSecret = "5523304a560212e06bdd82706b69f5ee";
//******XMWAN END***************************************************************

//******BAIDU*******************************************************************
const static std::string BAIDU_AppID = "6685863";
const static std::string BAIDU_AppKey = "zLGKVnyRGlcbW1chPd2szcTF";
const static std::string BAIDU_SecretKey = "5YjTT5MAz2IOcpW7aA9TCypA0Sk8BeeF";
//******BADDU END***************************************************************

//******OPPO*****************************************************************
const static std::string OPPO_AppId = "4102";
const static std::string OPPO_AppKey = "25ht032cI934SCWo000Sk0k0S";
const static std::string OPPO_AppSecret = "bA95b2207f4805408D794e93DF986122";
const static std::string OPPO_PubKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmreYIkPwVovKR8rLHWlFVw7YD\r\n"
		"fm9uQOJKL89Smt6ypXGVdrAKKl0wNYc3/jecAoPi2ylChfa2iRu5gunJyNmpWZzl\r\n"
		"CNRIau55fxGW0XEu553IiprOZcaw5OuYGlf60ga8QT6qToP0/dpiL/ZbmNUO9kUh\r\n"
		"osIjEu22uFgR+5cYyQIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
//******OPPO END*************************************************************

//******PPTV*****************************************************************
const static std::string PPTV_Key = "9a077b2c072fae2764a22bbace6dfe11";
//******PPTV END*************************************************************

//*****GIONEE****************************************************************
const static std::string GIONEE_APIKey = "2BCB4933F92D4954ABB0B86F87736E79";
const static std::string GIONEE_SecretKey = "B69DC911679A471D8AAFEBEFFAC7DC95";
const static std::string GIONEE_PublicKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCUJILNX7K0K+7E/As0PmQWyXbV\r\n"
		"eVZu2vn5gWd8Z7ETH42W+sNiPCU9m76fQjz2D688Bn6VCQ+ZSKBOX2eC2ZCoJgX5\r\n"
		"ikh7gAijQRmZpv8NSQ+nWzGA+nC29ec82UfdDJu3566xLjGeHeE1uvFw5aQwfqkE\r\n"
		"0W+oQG89QFKk8JSoBQIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
//*****GIONEE END************************************************************

//*****FLYME*****************************************************************
const static std::string FLYME_AppID = "2681137";
const static std::string FLYME_AppKey = "29b81d3721654c01a1c2e56d67589af0";
const static std::string FLYME_AppSecret = "dVK5wtDskGK6GwhnF8ZfdawWGdsOkdbe";
const static std::string FLYME_PubKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCu9g45AmEvF7VxtfeEZK3OyHhT\r\n"
		"5vgO4B5n2owtVpRcsAb04ryAr7nMMqVJB3LHj+q9DSp0y8RmRF7QbBUgHbomb1cU\r\n"
		"tBsZBMG4U3LarJANz+tEygaE+C4gNRxGF8evH9Frlq2CPoXFzy/G9/isRNbcnmYf\r\n"
		"sDeUglpxyeYLvH/hTwIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
//*****FLYME END*************************************************************

//*****APP CHINA*************************************************************

const static std::string APPCHINA_APPID = "5000505381";
const static std::string APPCHINA_AppKey =
		"QzM2QkM2RTc4MTAwOTdFOTc5NTQzRERFMUM5OUI1NkFCMDM1NkVBRE1USXdNakE1T1RFd09UUTJOVGd6T0RBME5Ua3JNVFkyTmpneE56RXhOek0wTURFNE1ETTFOelV3TURJeU5ESTBOalkwTURZM01qRTRPVFEz";
//*****APP CHINA END*********************************************************

//****** XY ***********************************************************
const static std::string XY_AppId = "100023841";
const static std::string XY_AppKey = "Rgkz1FUmbkzInUAkG9l4JDWTsPZG1aCX";
const static std::string XY_PayKey = "1qoOHgzjpdCD2jbQFvVYsmxFSmElXEAs";
//****** XY END***********************************************************

//****** 19196 ***********************************************************
const static std::string P_19196_Pid = "104158";
const static std::string P_19196_PKEY = "77661613dcf13531716d98c8cf1ae8f1";
//****** 19196 END***********************************************************

//*******4399*************************************************************
const static std::string APP_Secrect4399 = "18fab67843d213d5f3cad0c195453752";
//************************************************************************

//*******拇指玩*****************************doTBTReq********************************
const static std::string MUZHIWAN_KEY = "85e3dceed9502985bbe2f277bbfc6973";
const static std::string MUZHIWAN_PAYMENT_KEY = "55dc020d1ae8a";
//*******拇指玩 END************************************************************

//*******豌豆夹*************************************************************
//const static std::string WANDOUJIA_AppKeyID = "100032865";
//const static std::string WANDOUJIA_SecretKey =	"c6859043650c557ce9730bb763fa3699";

const static std::string WANDOUJIA_AppKeyID = "100035224";
const static std::string WANDOUJIA_SecretKey =	"60ada924610086651e9e894e42450028";

const static std::string WANDOUJIA_PublicKey =
		"-----BEGIN PUBLIC KEY-----\r\n"
				"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCd95FnJFhPinpNiE/h4VA6bU1rzRa5+a2\r\n"
				"5BxsnFX8TzquWxqDCoe4xG6QKXMXuKvV57tTRpzRo2jeto40eHKClzEgjx9lTYVb2RFHHFW\r\n"
				"io/YGTfnqIPTVpi7d7uHY+0FZ0lYL5LlW4E2+CQMxFOPRwfqGzMjs1SDlH7lVrLEVy6QIDA\r\n"
				"QAB\r\n"
				"-----END PUBLIC KEY-----\r\n";
//*******豌豆夹 END************************************************************

//*******当乐*************************************************************
const static std::string DOWNJON_APP_ID = "4043";
const static std::string DOWNJON_MERCHANT_ID = "1497";
const static std::string DOWNJON_APP_KEY = "Phwa2Iia";
const static std::string DOWNJON_PAYMENT_Key = "1a8ICu0S4A8d";
//*******当乐 END************************************************************

//**********I4*****************************************
const static std::string I4_PubKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCkI2fm54aUuaxA1qQDV/M300vs\r\n"
		"fo12K1xg4xkr9WIJG9l026+1I5k0hJT5+2Ayf0d0Pc1nBq9LEG2f0PaWjl0WXa0M\r\n"
		"DUMUdWz3tLGPHzFr8Pd/Ng1hnvkfx6Kn8q2BOB5XN5dP0t2KvIL1ZkkYlIt4Cl+C\r\n"
		"of9A4vRyrGguGzzZMwIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
//**********I4 END*************************************

//*************CCPLAY**********************************
const static std::string CCPLAY_Secret = "14c1e170db824876a5a9248504f9ce67";
//*************CCPLAY END******************************

//******************Coolyun**********************
const static std::string COOLYUN_APPID = "5000001794";
const static std::string COOLYUN_APPKEY = "2802db5f589746e2b1df022df1050cef";
const static std::string COOLYUN_PlatKey =
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCPgJMlKs1dC7Dy3d5THKN/xsDTtZQmX0KrXDodBpbbcXZT7eZXv4yKqZ4+TU31Wmt2RTk58dTEKt9V0ygcSuW7Kw/KvBtp15SZUlTDEnes8V19hDiY/We/o2+monn2Pm7yKDDVJp9M/D5aPacZ9gIEORdojr1z6C819SApSKyPCQIDAQAB";
//const static std::string COOLYUN_PrivateKey = "MIICXAIBAAKBgQCdsaDmfNWZxSMi5vSbYkN+p7S97P+F9mGlqj/UooCmgXsFrWdhLI8uQvpNf/D78nkn/SaZEbSyuUK4g7DUK4dcuinfeFIJVvSVNnHJ0yVu8nSb00sRUizYg+Hla0qYLtdBIvF1Dwf9KzJzUAcWAVABqB7hvfyC8G4/N7LrP9Nb7QIDAQABAoGAUVTQ4K+X1zQ22AR4Oj/nAcbrUF2QuH4HhNMXEssxQS9czOPPgu3hdwGyaRBIsRkw5bAKHT5ljRxBzjqVlAhTSm+YwxbQxRb4SXFzJPm1R9/jk7Y9HMuXH5NBKtzd2NtWXsPQVYoT4oD8zs5Csb1mUhBICJdN91ON/G+hYRg6AP0CQQDPfFkLYPNpMSJHpDxn45b4thMSQxWdStS1vcLvQFSTW71YxkaVjPLd0ggkwjDVOs02En/QXr1GGHIr2sg+pMRjAkEAwpDYgWBzXjNLecLwOiO53mB+fEVBKPdZTWWnfJpNt4a9AggmeyqWatvdVcbYbrVkvwrSxEwdZzjNGij5LYaHbwJAQGXy6UI1KoASiLPj/8a21AkHEFL8AW75aFM2jrWjynzd3xDjVYWC1/BZkLcpGvV+Pfst6omNK5Vo0k28exz+ywJBAIQtNogd6AI0wq+DWQOGRbzmvMbbnXqhHs/PoUqq4OUu6bQ5qTq/IdSSbPjazynWnQyT479rHgaDtgJxl2XwII0CQGmkij7vKq1H4GEyd67uUJSJSGbdJSDBK+1c1NETDrUZopzllEOflwmCvBgt69pnpUJsMvql5pugrWs5oOSMwtU=";
//"-----BEGIN PUBLIC KEY-----\r\n"
//"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCPgJMlKs1dC7Dy3d5THKN/xsDTtZQmX0K\r\n"
//"rXDodBpbbcXZT7eZXv4yKqZ4+TU31Wmt2RTk58dTEKt9V0ygcSuW7Kw/KvBtp15SZUlTDEn\r\n"
//"es8V19hDiY/We/o2+monn2Pm7yKDDVJp9M/D5aPacZ9gIEORdojr1z6C819SApSKyPCQIDA\r\n"
//"QAB\r\n";
//"-----END PUBLIC KEY-----\r\n";
//******************Coolyun end*****************

//*******腾讯*************************************************************
const static std::string QQ_APPID = "1104792347";
const static std::string QQ_APPKEY = "mf0h1q7xGBroD6US";
const static std::string QQ_APPKEY_PAY ="BFdn8b3GzKWmfOh05fCc5eN94Mx0rdoj";//"mf0h1q7xGBroD6US" ;
const static std::string WX_APPID = "wxe5d55c7ddfd4a758";
const static std::string WX_APPKEY = "8b91661c3894ce8ee52045a0c92b773b";
const static std::string MSDKKEY = "e001a244da1e2662da99245e93a858ca";
//*******腾讯 END*************************************************************

//**********同步推********************************************************
const static std::string TBT_APPID = "150973";
const static std::string TBT_APPKEY = "9SH3rC5mby#VLivF9Hf3r5Ombo#ViXvF";
//**********同步推end**************************************************

//*************VIVO ***********************************************
const static std::string VIVO_CPKEY = "be41d5ba76c78e935990b5ed67f3e66e";
const static std::string VIVO_APPID = "737c91c6dd23adf886aa81fbb9036da1";
const static std::string VIVO_CPID = "20150825180134399960";
//*************VIVO END*******************************************

//*****************海马玩***************************************************************
const static std::string HMW_AND_APPID = "4256bca9de25e96d43a277f2a91a41b0";
const static std::string HMW_AND_APPKEY = "60d30b1e38da8d52fe74f37ca1078adf";
const static std::string HMW_IOS_APPID = "7d77ab105daa333aa4a63ac681cdbd67";
const static std::string HMW_IOS_APPKEY = "bc9f08b475dcdf9803c01962abc423a3";
//*****************海马玩***************************************************************

//***************SOGOU********************************************
const static std::string SOGOU_GID = "1170";
const static std::string SOGOU_APPKEY =
		"b7fb3e8acd27478ee80853d0c57b61ec731f5b4ff2e3d26ef59c4cd2eed34cfc";
const static std::string SOGOU_APPSECRET =
		"17300317e11674bd1b50794a90f2857ce0ed8e89686ab9860e6b65529ffbab42";
const static std::string SOGOU_PAYKEY = "{0006C303-1EB4-45F7-A489-F0A3CF0C1EBB}";
//**************SOGOU*********************************************

//*************SINA*********************************************
const static std::string SINA_APPID = "4236082600";
const static std::string SINA_SIGNKEY = "ba3c2b2f0c290af830ad3b2a1ca19a5d";
const static std::string SINA_APPSECRET = "53f33fba8cffbb14957bbbdc14a23cf2";
//*************SINA***********************************************

//**************PPS**********************************************
const static std::string PPS_KEY = "BYHCS8975af9b3947ae3a802a4d73885e54943937";
//*******************************************************

//*********************GFUN*******************************
const static std::string GFUN_UID = "28928685";
//*********************************GFUN***************************

//************************YOUKU**************************************
const static std::string YOUKU_APPID = "2148";
const static std::string YOUKU_APPKEY = "77db85b21fe02d0b";
const static std::string YOUKU_APPSECRET = "26931cfaed0575551386469249bffcab";
const static std::string YOUKU_PAYKEY = "94b6aa5c00ebd6eeb05076f6a97830fb";
//************************YOUKU**************************************

//*******************IAPPLE********************************************
const static std::string IAPPLE_GAMEKEY = "85e654e73178e05003c09eeae003cb92";
const static std::string IAPPLE_SECRETKEY = "07c779ad9bf80e4132afd78bdf3f51f9";
//*******************IAPPLE*******************************************

//*******************果盘（Android）********************************************
const static std::string GUOPAN_APP_ID_AND = "101966";
const static std::string GUOPAN_APP_KEY_AND = "WN4TNCWS0VJBXUEODUJ9449UD8T9I3W9K0XGB6NXLH97GS7KBPUP5D61X594Z74Z";
//*******************果盘（Android）********************************************


//*******************果盘（IOS））********************************************
const static std::string GUOPAN_APP_ID_IOS = "102191";
const static std::string GUOPAN_APP_KEY_IOS = "9FJMK8F3J3DCWI89ENVLHNN0S2GHG028788R6F93NK43XJPLGK0CH7VRNP8FR76X";
//*******************果盘（Android）********************************************

//*******************PP助手（IOS））********************************************
const static std::string PP_APP_ID_IOS = "7049";
const static std::string PP_APP_KEY_IOS = "8e122a50baf625a6a5396a0f7315f959";
const static std::string PP_PubKey = "-----BEGIN PUBLIC KEY-----\r\n"
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAx1ewEY7HdE9H8ZDmlfZD\r\n"
		"AwTwxRyrxOtEw2CUYG0ggDBuO8SaZeGzlfysrwsge7b10pMGhNo+F3wBm2WkkVQL\r\n"
		"1ltcAfgDUb9HhG+3kT7u3Qn0jdNAQoHaXFJSpGxW/m8cYRZHzj2iyzc1W3HtMgGP\r\n"
		"ze+4BD3Yid+1T4HpfgoqlCXykOcZhfzFxx8pLn9cSy9FwGGzeXTprUhRlD19ByeF\r\n"
		"8MGHyOpUO+xg79ZxxzTvfLE9XBRIO8kWYOXrTQ0HqDr+bB9b5zPjpapSeoPs5wno\r\n"
		"imOdPdZHj1faPx+ykloi+oBsf2gnD81Wedl6TiSHfVLxuMmqEy69zpW0XGRRd2bP\r\n"
		"JwIDAQAB\r\n"
		"-----END PUBLIC KEY-----\r\n";
//*******************PP助手（IOS））********************************************
#endif /* SRC_PASSPORTSVR_COMMON_H_ */
