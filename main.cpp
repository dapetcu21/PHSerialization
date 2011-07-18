//
//  main.cpp
//  Porkholt_iOS
//
//  Created by Marius Petcu on 7/18/11.
//  Copyright 2011 Porkholt Labs!. All rights reserved.
//

#include "PHSerialization.h"
#include <cstdio>
#include <typeinfo>

int main()
{
    double d = -1.0f;
    d = ldexp(d,-1022-52);
    uint64_t a = ntohll(PHEncodeF(d));
    double r = PHDecodeF((uint64_t)htonll(a));
    printf("%llx %llx %llx\n",*((uint64_t*)&d),a,*((uint64_t*)&r));
}