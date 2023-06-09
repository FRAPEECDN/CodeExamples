#pragma once

#include <StdAfx.h>
#include <Windows.h>

typedef std::basic_string<TCHAR> tstring;

#define hashsize(n) (1U << (n))
#define hashmask(n) (hashsize(n) - 1)

#define mix(a,b,c) \
{ \
    a -= b; a -= c; a ^= (c >> 13); \
    b -= c; b -= a; b ^= (a << 8); \
    c -= a; c -= b; c ^= (b >> 13); \
    a -= b; a -= c; a ^= (c >> 12); \
    b -= c; b -= a; b ^= (a << 16); \
    c -= a; c -= b; c ^= (b >> 5); \
    a -= b; a -= c; a ^= (c >> 3); \
    b -= c; b -= a; b ^= (a << 10); \
    c -= a; c -= b; c ^= (b >> 15); \
}

struct hash_LONG_PTR {
	size_t operator()(const LONG_PTR &number) const
	{
		size_t x = (size_t)number;
#if defined(_WIN64)
		x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
		x = x ^ (x >> 31);
#else
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
#endif
		return x;
	}
};

size_t static jen_hash(unsigned char *k, size_t length, size_t initval)
{
	size_t a, b;
	size_t c = initval;
	size_t len = length;

	a = b = 0x9e3779b9;

	while (len >= 12)
	{
		a += (k[0] + ((unsigned)k[1] << 8) + ((unsigned)k[2] << 16) + ((unsigned)k[3] << 24));
		b += (k[4] + ((unsigned)k[5] << 8) + ((unsigned)k[6] << 16) + ((unsigned)k[7] << 24));
		c += (k[8] + ((unsigned)k[9] << 8) + ((unsigned)k[10] << 16) + ((unsigned)k[11] << 24));

		mix(a, b, c);

		k += 12;
		len -= 12;
	}

	c += length;

	switch (len)
	{
	case 11: c += ((unsigned)k[10] << 24);
	case 10: c += ((unsigned)k[9] << 16);
	case 9: c += ((unsigned)k[8] << 8);
		/* First byte of c reserved for length */
	case 8: b += ((unsigned)k[7] << 24);
	case 7: b += ((unsigned)k[6] << 16);
	case 6: b += ((unsigned)k[5] << 8);
	case 5: b += k[4];
	case 4: a += ((unsigned)k[3] << 24);
	case 3: a += ((unsigned)k[2] << 16);
	case 2: a += ((unsigned)k[1] << 8);
	case 1: a += k[0];
	}

	mix(a, b, c);

	return c;
}

struct hash_tstring {
	size_t operator()(const tstring &word) const
	{
		_LARGE_INTEGER initcount = { 0 };
		QueryPerformanceCounter(&initcount);
		unsigned char * val = new unsigned char[word.size() + 1];
		_tcscpy((char*)val, word.c_str());
		return jen_hash(val, word.size(), (size_t)initcount.QuadPart);
	}
};
