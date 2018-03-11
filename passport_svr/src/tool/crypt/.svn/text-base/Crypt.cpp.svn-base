//
//  Crypt.cpp
//  encrypt
//
//  Created by 唐少 on 15/8/14.
//  Copyright (c) 2015年 encrypt. All rights reserved.
//

#include "Crypt.h"
const int C1 = 52845;
const int C2 = 22719;
const int KEY = 72957;


bool Crypt::Encrypt(const std::string &source, std::string &destination) {
    int Key = KEY;
    
    if( source.empty())
        return false;
    for(unsigned int i = 0; i < source.length(); ++i) {
        destination.push_back(source[i]^Key >> 8);
        Key = (destination[i] + Key) * C1 + C2;
    }
    return true;
}

bool Crypt::Decrypt(const std::string &source, std::string &destination){
    int Key = KEY;
    char PreviousBlock;
    if( source.empty())
        return false;
    for(unsigned int i = 0; i < source.length(); ++i) {
        PreviousBlock = source[i];
        destination.push_back(source[i]^Key >> 8);
        Key = (PreviousBlock + Key) * C1 + C2;
    }
    return true;
}
