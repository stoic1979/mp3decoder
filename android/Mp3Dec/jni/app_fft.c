//
//  fft.cpp
//  Fast Fourier Transform
//

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "app_fft.h"


//
// 	fft -> input vector size n+2!!
//	Fast Fourier Transform
// 	y = input vectorde long=n+2 
//       [Re(0),Re(1), ..., Re(N-1)] (direct transform)
//       [Re(0),Im(0),Re(1),Im(1),...,Re(N/2),Im(N/2)] (inverse)
//			
//	n = n values
//      m = log(n)/log(2) ; 2^m=n


// max fft items
#define L2MAX_LEN    	     19
#define MAX_LEN   		    (1<<L2MAX_LEN)
#define PI        M_PI
#define TWOPI     (M_PI*2)
#define SQRT2     M_SQRT2

// m= 1 2 3  4  5  6   7   8   9   10   11...
// use powers of 2:  2,4,8,16,32,64,128,256,512,1024,2048...
// y[n+2] len important !!
//
void fft(double *y, int m) {
    int j=1,is,id,k,n1,i,i0,n2,n4,n8,i1,i2,i3,i4,i5,i6,i7,i8;
    int n;
    double  xt,r1,t1,t2,t3,t4,t5,t6,cc1,ss1,cc3,ss3;
    double e,a,a3;
    double *x;
    // static double x1 [MAX_LEN+2];
    double *x1;
    static double *ccc1 [L2MAX_LEN+1], *sss1 [L2MAX_LEN+1],
                  *ccc3 [L2MAX_LEN+1], *sss3 [L2MAX_LEN+1];
    static int last_n = 0, last_m = 0;

    // calc n=2^m
    n=1<<m;

    /******* Check transform length *********************************************/
    if (n > MAX_LEN || m > L2MAX_LEN) {
        printf("ERROR: (fft.cpp) :: 'n' or 'm' is invlid\n");
        return;
    }

    /******* Compute table of sines/cosines *************************************/
    if (n != last_n)    /* Transform size has changed */
    {
        if (last_n != 0)     /* Correct , it does not work !!! */
            for (k = 3; k <= last_m; k++) {
                free(sss1[k]);
                free(sss3[k]);
                free(ccc1[k]);
                free(ccc3[k]);
            }
        n2 = 4;
        for (k = 3; k <= m; k++) /* Get space for new ones */
        {
            n2 <<= 1;
            n8 = n2 >> 3;
            e = a = TWOPI / n2;
            sss1[k] = (double*)malloc(sizeof(double) * (n8+1));
            sss3[k] = (double*)malloc(sizeof(double) * (n8+1));
            ccc1[k] = (double*)malloc(sizeof(double) * (n8+1));
            ccc3[k] = (double*)malloc(sizeof(double) * (n8+1));

            if (sss1[k] == NULL || sss3[k] == NULL || ccc1 [k] == NULL || ccc3[k] == NULL)
                return;
            for (j = 2; j <= n8; j++)
            {
                a3 = a+a+a;
                ccc1[k][j] = cos (a);
                sss1[k][j] = sin (a);
                ccc3[k][j] = cos (a3);
                sss3[k][j] = sin (a3);
                a += e;
            }
        }
        last_n = n;     /* Update last size */
        last_m = m;
    }

    /********************* Copy items to temp. array ****************************/

    x1 = (double*) malloc(sizeof(double) + (MAX_LEN+2));
    memcpy (x1, y, (n+2) * sizeof (double));

    x = x1 -1;  /*** Dirty trick to simulate that indices begin at 1 ***/


    /* Direct transform */
    /*******************  Digit reverse  counter  *******************************/
    {
        j  = 1;
        n1 = n-1;
        for (i = 1; i <= n1; i++)
        {
            if (i < j)
            {
                xt   = x[j];
                x[j] = x[i];
                x[i] = xt;
            }
            k = n >> 1;      /* n/2; */
            while (k < j)
            {
                j -= k;
                k >>= 1;      /* k/=2 */
            }
            j += k;
        }

        /******************  Length two butterflies  *********************************/

        is = 1;
        id = 4;
        do
        {
            for (i0 = is; i0 <= n; i0 += id)
            {
                i1    = i0 + 1;
                r1    = x[i0];
                x[i0] = r1 + x[i1];
                x[i1] = r1 - x[i1];
            }
            is = (id << 1) - 1;
            id <<= 2;           /* id *= 4; */
        }
        while (is < n);

        /*************** L-shaped butterflies  **************************************/

        n2 = 2;
        for (k = 2; k <= m; k++) {
            n2 <<= 1;       /* n2 *= 2; */
            n4 = n2 >> 2;   /* n2/4;    */
            n8 = n2 >> 3;   /* n2/8;    */
            is = 0;
            id = n2 << 1;
            do {
                for (i = is; i < n ; i += id) {
                    i1     = i + 1;
                    i2     = i1 + n4;
                    i3     = i2 + n4;
                    i4     = i3 + n4;
                    t1     = x[i4] + x[i3];
                    x[i4] -= x[i3];
                    x[i3]  = x[i1] - t1;
                    x[i1] += t1;
                    if (n4 != 1) {
                        i1    += n8;
                        i2    += n8;
                        i3    += n8;
                        i4    += n8;
                        t1     = (x[i3] + x[i4]) / SQRT2;
                        t2     = (x[i3] - x[i4]) / SQRT2;
                        x[i4]  = x[i2] - t1;
                        x[i3]  = -x[i2] - t1;
                        x[i2]  = x[i1] - t2;
                        x[i1] += t2;
                    }
                }
                is = (id << 1) - n2;
                id <<= 2;            /* id *= 4; */
            }
            while (is < n);
            for (j = 2; j <= n8; j++) {
                cc1 = ccc1[k][j];
                ss1 = sss1[k][j];
                cc3 = ccc3[k][j];
                ss3 = sss3[k][j];
                is  = 0;
                id  = n2 << 1;
                do {
                    for (i = is; i < n; i += id) {
                        i1 = i + j;
                        i2 = i1 + n4;
                        i3 = i2 + n4;
                        i4 = i3 + n4;
                        i5 = i + n4 - j + 2;
                        i6 = i5 + n4;
                        i7 = i6 + n4;
                        i8 = i7 + n4;
                        t1 = x[i3] * cc1 + x[i7] * ss1;
                        t2 = x[i7] * cc1 - x[i3] * ss1;
                        t3 = x[i4] * cc3 + x[i8] * ss3;
                        t4 = x[i8] * cc3 - x[i4] * ss3;
                        t5 = t1 + t3;
                        t6 = t2 + t4;
                        t3 = t1 - t3;
                        t4 = t2 - t4;
                        t2 = x[i6] + t6;
                        x[i3] = t6 - x[i6];
                        x[i8] = t2;
                        t2    = x[i2] - t3;
                        x[i7] = -x[i2] - t3;
                        x[i4] = t2;
                        t1    = x[i1] + t5;
                        x[i6] = x[i1] - t5;
                        x[i1] = t1;
                        t1    = x[i5] + t4;
                        x[i5] -= t4;
                        x[i2] = t1;
                    }
                    is = (id << 1) - n2;
                    id <<= 2;           /* id *=4;*/
                }
                while (is < n);
            }
        }
        /******* Rerrange data as in program header, copy to y **********************/
        j      = n >> 1;   /* n/2 */
        y[0]   = x1[0];
        y[1]   = 0.0;
        y[n]   = x1[j];
        y[n+1] = 0.0;
        for (i = 1, k = 2; i < j; i++, k += 2)
        {
            y[k]   = x1[i];
            y[k+1] = x1[n-i];
        }
    }
    free(x1);
}


//************************
/*** Inverse transform **/
//************************
void fft_inv(double *y, int m) {
    int j=1,is,id,k,n1,i,i0,n2,n4,n8,i1,i2,i3,i4,i5,i6,i7,i8;
    int n;
    double  xt,r1,t1,t2,t3,t4,t5,cc1,ss1,cc3,ss3,fn;
    double e,a,a3;
    double *x;
    // static double x1 [MAX_LEN+2];
    double *x1;
    static double *ccc1 [L2MAX_LEN+1], *sss1 [L2MAX_LEN+1],
                  *ccc3 [L2MAX_LEN+1], *sss3 [L2MAX_LEN+1];
    static int last_n = 0, last_m = 0;

    // calc n=2^m
    n=1<<m;

    /******* Check transform length *********************************************/
    if (n > MAX_LEN || m > L2MAX_LEN) return;

    /******* Compute table of sines/cosines *************************************/
    if (n != last_n)    /* Transform size has changed */
    {
        if (last_n != 0)     /* Correct , it does not work !!! */
            for (k = 3; k <= last_m; k++) {
                free(sss1[k]); free(sss3[k]);
                free(ccc1[k]); free(ccc3[k]);
            }
        n2 = 4;
        for (k = 3; k <= m; k++) /* Get space for new ones */ {
            n2 <<= 1;
            n8 = n2 >> 3;
            e = a = TWOPI / n2;
            sss1[k] = (double*)malloc(sizeof(double) * (n8+1));
            sss3[k] = (double*)malloc(sizeof(double) * (n8+1));
            ccc1[k] = (double*)malloc(sizeof(double) * (n8+1));
            ccc3[k] = (double*)malloc(sizeof(double) * (n8+1));
            if (sss1[k] == NULL || sss3[k] == NULL || ccc1 [k] == NULL || ccc3[k] == NULL)
                return;
            for (j = 2; j <= n8; j++) {
                a3 = a+a+a;
                ccc1[k][j] = cos (a);
                sss1[k][j] = sin (a);
                ccc3[k][j] = cos (a3);
                sss3[k][j] = sin (a3);
                a += e;
            }
        }
        last_n = n;     /* Update last size */
        last_m = m;
    }

    /********************* Copy items to temp. array ****************************/

    x1 = (double*) malloc(sizeof(double) + (MAX_LEN+2));
    memcpy (x1, y, (n+2) * sizeof (double));

    {
        j = n >> 1;   /* n/2 */
        y[0] = x1[0];
        y[j] = x1[n];
        for (i = 1, k = 2; i < j; i++, k += 2)
        {
            y[i]   = x1[k];
            y[n-i] = x1[k+1];
        }
        x = y - 1;    /*** Operate with reversed data ***/
    }

    {
        n2 = n << 1;
        for (k = 1; k < m; k++)
        {
            is = 0;
            id = n2;
            n2 >>= 1;
            n4 = n2 >> 2;
            n8 = n4 >> 1;
            do
            {
                for (i = is; i < n; i += id)
                {
                    i1 = i + 1;
                    i2 = i1 + n4;
                    i3 = i2 + n4;
                    i4 = i3 + n4;
                    t1 = x[i1] - x[i3];
                    x[i1] += x[i3];
                    x[i2] *= 2.0;
                    x[i3]  = t1 - 2.0 * x[i4];
                    x[i4]  = t1 + 2.0 * x[i4];
                    if (n4 != 1)
                    {
                        i1 += n8;
                        i2 += n8;
                        i3 += n8;
                        i4 += n8;
                        t1  = (x[i2] - x[i1]) / SQRT2;
                        t2  = (x[i4] + x[i3]) / SQRT2;
                        x[i1] += x[i2];
                        x[i2]  = x[i4] - x[i3];
                        x[i3]  = 2.0 * (-t2 - t1);
                        x[i4]  = 2.0 * (-t2 + t1);
                    }
                }
                is = (id << 1) - n2;
                id <<= 2;
            }
            while (is < n-1);
            for (j = 2; j <= n8; j++)
            {
                cc1 = ccc1[m-k+1][j];  /*** Note that cosine table is inverted]] ***/
                ss1 = sss1[m-k+1][j];
                cc3 = ccc3[m-k+1][j];
                ss3 = sss3[m-k+1][j];
                is  = 0;
                id  = n2 << 1;
                do
                {
                    for (i = is; i < n; i += id)
                    {
                        i1 = i + j;
                        i2 = i1 + n4;
                        i3 = i2 + n4;
                        i4 = i3 + n4;
                        i5 = i + n4 - j + 2;
                        i6 = i5 + n4;
                        i7 = i6 + n4;
                        i8 = i7 + n4;
                        t1 = x[i1] - x[i6];
                        x[i1] += x[i6];
                        t2     = x[i5] - x[i2];
                        x[i5] += x[i2];
                        t3     = x[i8] + x[i3];
                        x[i6]  = x[i8] - x[i3];
                        t4     = x[i4] + x[i7];
                        x[i2]  = x[i4] - x[i7];
                        t5     = t1 - t4;
                        t1    += t4;
                        t4     = t2 - t3;
                        t2    += t3;
                        x[i3]  = t5 * cc1 + t4 * ss1;
                        x[i7]  = -t4 * cc1 + t5 * ss1;
                        x[i4]  = t1 * cc3 - t2 * ss3;
                        x[i8]  = t2 * cc3 + t1 * ss3;
                    }
                    is = (id << 1) - n2;
                    id <<= 2;
                }
                while (is < n-1);
            }
        }

        /*******************  Length two butterflies  *******************************/

        is = 1;
        id = 4;
        do
        {
            for (i0 = is; i0 <= n; i0 += id) {
                i1 = i0 + 1;
                r1 = x[i0];
                x[i0] = r1 + x[i1];
                x[i1] = r1 - x[i1];
            }
            is = (id << 1) - 1;
            id <<= 2;
        }
        while (is < n);

        /*******************  Digit reverse counter  ********************************/

        j = 1;
        n1 = n - 1;
        for (i = 1; i <= n1; i++)
        {
            if (i < j)
            {
                xt   = x[j];
                x[j] = x[i];
                x[i] = xt;
            }
            k = n >> 1;
            while (k < j)
            {
                j  -= k;
                k >>= 1;
            }
            j += k;
        }

        /*********** Re-scale output ************************************************/
        fn = n;
        for (i = 1; i <= n; i++)
            x[i] /= fn;
    }
    free(x1);
}
