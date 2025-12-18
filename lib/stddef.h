#ifndef _STDDEF_H
#define _STDDEF_H

#define NULL ((void *)0)

//size type (for amd64 alteast)
typedef unsigned long long size_t;

//signed size type
typedef long long ssize_t;

//pointer difference type
typedef long long ptrdiff_t;

//wide character type
typedef int wchar_t;

//max alignment type
typedef long double max_align_t;

#define offsetof(type, member) ((size_t)&((type *)0)->member)

#endif
