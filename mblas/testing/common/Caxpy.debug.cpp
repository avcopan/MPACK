/*
 * Copyright (c) 2008-2010
 *	Nakata, Maho
 * 	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <mblas.h>
#include <blas.h>
#include <mpack_debug.h>

#if defined VERBOSE_TEST
#include <iostream>
#endif

#define MAX_INCX  5
#define MIN_INCX -5
#define MAX_INCY  5
#define MIN_INCY -5
#define MAX_N     10
#define MAX_ITER  10

REAL_REF maxdiff = 0.0;

void Caxpy_test2(COMPLEX_REF alpha_ref, COMPLEX alpha)
{
    int errorflag = FALSE;
    COMPLEX ctemp;

    for (int incx = MAX_INCX; incx >= MIN_INCX; incx--) {
	for (int incy = MAX_INCY; incy >= MIN_INCY; incy--) {
	    for (int n = 0; n < MAX_N; n++) {
		printf("# n:%d incx:%d, incy:%d\n", n, incx, incy);
		COMPLEX_REF *x_ref = new COMPLEX_REF[veclen(n, incx)];
		COMPLEX_REF *y_ref = new COMPLEX_REF[veclen(n, incy)];
		COMPLEX *x = new COMPLEX[veclen(n, incx)];
		COMPLEX *y = new COMPLEX[veclen(n, incy)];
		int j = 0;
		while (j < MAX_ITER) {
		    set_random_vector(x_ref, x, veclen(n, incx));
		    set_random_vector(y_ref, y, veclen(n, incy));
#if defined ___MPACK_BUILD_WITH_MPFR___
		    zaxpy_f77(&n, &alpha_ref, x_ref, &incx, y_ref, &incy);
#else
		    Caxpy(n, alpha_ref, x_ref, incx, y_ref, incy);
#endif
		    Caxpy(n, alpha, x, incx, y, incy);
		    REAL_REF diff = infnorm(y_ref, y, veclen(n, incy), 1);
		    if (abs(diff) > EPSILON) {
			printf("error: "); printnum(diff); printf("\n");
			errorflag = TRUE;
		    }
		    if (maxdiff < diff)
			maxdiff = diff;
		    printf("max error: "); printnum(diff); printf("\n");
		    j++;
		}
		delete[]x;
		delete[]x_ref;
		delete[]y;
		delete[]y_ref;
	    }
	}
    }
    if (errorflag == TRUE) {
	printf("Caxpy test failed...\n");
	exit(1);
    }
    printf("max error "); printnum(maxdiff); printf("\n");
}

void Caxpy_test()
{
    COMPLEX_REF alpha_ref;
    COMPLEX alpha;

    alpha_ref = 0.0, alpha = 0.0;
    printf("Caxpy with zero test...\n");
    Caxpy_test2(alpha_ref, alpha);

    alpha_ref = 1.0, alpha = 1.0;
    printf("Caxpy with 1.0 test...\n");
    Caxpy_test2(alpha_ref, alpha);

    alpha_ref = COMPLEX_REF(0.0, 1.0);
    alpha = COMPLEX(0.0, 1.0);
    printf("Caxpy with 1.0i test...\n");
    Caxpy_test2(alpha_ref, alpha);

    printf("Caxpy with random number test...\n");
    set_random_number(alpha_ref, alpha);
    Caxpy_test2(alpha_ref, alpha);
}

int main(int argc, char *argv[])
{
    Caxpy_test();
    printf("Caxpy test passed...\n");
    return (0);
}
