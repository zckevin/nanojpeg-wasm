/* sprintf.c */

/*
 * Copyright (c) 1997-2010, 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdarg.h>
#include <stdio.h>
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg
typedef __SIZE_TYPE__ size_t;
#define stderr 0
#define _MLIBC_RESTRICT

extern int _prf(int (*func)(), void *dest,
				const char *format, va_list vargs);

struct emitter {
	char *ptr;
	int len;
};

static int sprintf_out(int c, struct emitter *p)
{
	if (p->len > 1) { /* need to reserve a byte for EOS */
		*(p->ptr) = c;
		p->ptr += 1;
		p->len -= 1;
	}
	return 0; /* indicate keep going so we get the total count */
}

int snprintf(char *_MLIBC_RESTRICT s, size_t len,
	     const char *_MLIBC_RESTRICT format, ...)
{
	va_list vargs;

	struct emitter p;
	int     r;
	char    dummy;

	if (len == 0) {
		s = &dummy; /* write final NUL to dummy, can't change *s */
	}

	p.ptr = s;
	p.len = (int) len;

	va_start(vargs, format);
	r = _prf(sprintf_out, (void *) (&p), format, vargs);
	va_end(vargs);

	*(p.ptr) = 0;
	return r;
}

int sprintf(char *_MLIBC_RESTRICT s, const char *_MLIBC_RESTRICT format, ...)
{
	va_list vargs;

	struct emitter p;
	int     r;

	p.ptr = s;
	p.len = (int) 0x7fffffff; /* allow up to "maxint" characters */

	va_start(vargs, format);
	r = _prf(sprintf_out, (void *) (&p), format, vargs);
	va_end(vargs);

	*(p.ptr) = 0;
	return r;
}

int vsnprintf(char *_MLIBC_RESTRICT s, size_t len,
	      const char *_MLIBC_RESTRICT format, va_list vargs)
{
	struct emitter p;
	int     r;
	char    dummy;

	if (len == 0) {
		s = &dummy; /* write final NUL to dummy, can't change * *s */
	}

	p.ptr = s;
	p.len = (int) len;

	r = _prf(sprintf_out, (void *) (&p), format, vargs);

	*(p.ptr) = 0;
	return r;
}

int vsprintf(char *_MLIBC_RESTRICT s, const char *_MLIBC_RESTRICT format,
	     va_list vargs)
{
	struct emitter p;
	int     r;

	p.ptr = s;
	p.len = (int) 0x7fffffff; /* allow up to "maxint" characters */

	r = _prf(sprintf_out, (void *) (&p), format, vargs);

	*(p.ptr) = 0;
	return r;
}