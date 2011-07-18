//
//  PHSerialization.h
//  Porkholt_iOS
//
//  Created by Marius Petcu on 7/18/11.
//  Copyright 2011 Porkholt Labs!. All rights reserved.
//

#ifndef PHSERIALIZATION_H
#define PHSERIALIZATION_H

#include <arpa/inet.h>
#include <stdint.h>
#include <cmath>

#ifndef htonll
#ifdef __BIG_ENDIAN__
#define htonll(x) (x)
#else
#ifdef __GNUC__
#define htonll __builtin_bswap64
#elif _MSC_VER
#define htonll _byteswap_uint64
#else
inline uint64_t htonll(uint64_t x)
{
    return htonl(x>>32) | ((uint64_t)htonl(x&0xFFFFFFFF)<<32);
}
#endif
#endif

#define ntohll htonll
#endif

inline uint16_t PHEncode(uint16_t n) 
{
    return htons(n);
}

inline uint32_t PHEncode(uint32_t n)
{
    return htonl(n);
}

inline uint64_t PHEncode(uint64_t n)
{
    return htonll(n);
}

inline uint16_t PHDecode(uint16_t n) 
{
    return ntohs(n);
}

inline uint32_t PHDecode(uint32_t n)
{
    return ntohl(n);
}

inline uint64_t PHDecode(uint64_t n)
{
    return ntohll(n);
}

template <class ftype>
inline ftype PHldexp(ftype f, int n)
{
    return ldexp(f,n);
}

template <>
inline float PHldexp<float>(float f, int n)
{
    return ldexpf(f,n);
}

template <class ftype>
inline ftype PHfrexp(ftype f, int * n)
{
    return frexp(f,n);
}

template <>
inline float PHfrexp<float>(float f, int * n)
{
    return frexpf(f,n);
}

template <class ftype,class itype,int exp_size,int mantissa_size,int exp_bias>
itype PHFloatToIEEE754(ftype f)
{
    itype sign = f<0;
    if (f==0)
        return 0;
    if (f != f)
        return ((((itype)1<<(exp_size+1))-1)<<(mantissa_size-1)); //qNaN
    if (f - f != 0)
        return (sign<<(exp_size+mantissa_size))|((((itype)1<<exp_size)-1)<<mantissa_size); //infinity
        
    int exp;
    if (sign)
        f=-f;
    f = frexp(f,&exp);
    exp--;
    f = PHldexp<ftype>(f,1);
    exp+=exp_bias;
    if (exp<=0)
    {
        f = PHldexp<ftype>(f,exp-1);
        exp = 0;
    }
    itype mantissa = (itype)(PHldexp<ftype>(f,mantissa_size)) & (((itype)1<<mantissa_size)-1);
    return (sign<<(exp_size+mantissa_size))| ((itype)exp<<mantissa_size) | mantissa;
}

template <class ftype,class itype,int exp_size,int mantissa_size,int exp_bias>
ftype PHFloatFromIEEE754(itype i)
{
    itype mantissa = i & (((itype)1<<mantissa_size)-1);
    int exp = (i>>mantissa_size) & (((itype)1<<exp_size)-1);
    if (!exp && !mantissa)
        return 0;
    if (exp == (((itype)1<<exp_size)-1))
    {
        if (mantissa)
            return NAN;
        if (i & (((itype)1)<<(exp_size+mantissa_size)))
            return -INFINITY;
        return INFINITY;
    }
    ftype f;
    if (exp == 0)
        f = PHldexp<ftype>((ftype)mantissa, -mantissa_size-exp_bias+1);
    else
        f = PHldexp<ftype>((ftype)(mantissa | ((itype)1<<mantissa_size)), -mantissa_size+exp-exp_bias);
    if (i & (((itype)1)<<(exp_size+mantissa_size)))
        return -f;
    else
        return f;
}

inline uint32_t PHEncodeF(float n)
{
    uint32_t e = PHFloatToIEEE754<float,uint32_t,8,23,127>(n);
    return htonl(e);
}

inline float PHDecodeF(uint32_t n)
{
    return PHFloatFromIEEE754<float,uint32_t,8,23,127>(ntohl(n));
}

inline uint64_t PHEncodeF(double n)
{
    uint64_t e = PHFloatToIEEE754<double,uint64_t,11,52,1023>(n);
    return htonll(e);
}

inline double PHDecodeF(uint64_t n)
{
    return PHFloatFromIEEE754<double,uint64_t,11,52,1023>(ntohll(n));
}

#endif