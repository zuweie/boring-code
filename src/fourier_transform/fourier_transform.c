/*
 * @Author: your name
 * @Date: 2020-12-29 16:12:36
 * @LastEditTime: 2021-04-09 09:32:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/fourier_transform/fourier_transform.c
 */
#include <math.h>
#include "complex/complex.h"
#include "fourier_transform.h"
#define PI 3.1415926

/**
 * 离散傅立叶变换, O(n^2) 版本
 * squence 采样数据，大小一般为 2^x 大小
 * out 傅立叶变换输出的复数
*/
int Discrete_fourier_transform (float sequence[], int sequence_size, complex_t out[]) 
{
    int N = sequence_size;
    complex_t k_point;
    for (int i = 0; i<N; ++i) {
        k_point = _complex(0.f, 0.f);
        for (int n=0; n<N; ++n) {
            k_point.real  += cos(2*PI/N * i * n) * sequence[n];
            k_point.image += sin(2*PI/N * i * n) * sequence[n];
        }
        out[i] = k_point;
    }
    return 0;
}

int Recursive_fast_fourier_transform(complex_t sequence[], int n, complex_t y[], int reverse) 
{
    if (n == 1) {
        y[0] = sequence[0];
        return 0;
    }
    
    int half_n = n>>1;
    complex_t sub_seq[n];
    complex_t wn = reverse?complex_rw(n, 1):complex_w(n, 1);
    complex_t w1 = _complex(1,0);

    for (int i=0, j=0; i < n; i = i+2, ++j) {
        sub_seq[j]        = sequence[i];
        sub_seq[j+half_n] = sequence[i+1];
    }
    
    complex_t yk[n];
    Recursive_fast_fourier_transform(sub_seq, half_n, yk, reverse);
    Recursive_fast_fourier_transform(&sub_seq[half_n], half_n, &yk[half_n], reverse);

    for (int k=0; k < half_n; k++) {
        complex_t wy1 = complex_multiply(w1, yk[k+half_n]);
        y[k] = complex_sum(yk[k], wy1);
        y[k+half_n] = complex_sub(yk[k], wy1);
        w1 = complex_multiply(w1, wn);
    }
    return 0;
}

/* __Bit_reverse_copy 与 __Bit_reverse_copy2 功效一样 
 * __Bit_reverse_copy 浪费一个 complex_t a[] 的空间，时间是 O(n)
 * __Bit_reverse_copy2 省下一个 complex_t a[] 的空间，但是时间是 O(nlgn)
*/
static void __Bit_reverse_copy(complex_t a[], int size_a, complex_t A[]) 
{
    int r = log2(size_a);

    for (int k=0; k<size_a; ++k) {
        int k_index = Bit_reverse_32(k, r);
        A[k_index] = a[k];
    }
    
    return;
}

static void __Bit_reverse_local(complex_t a[], int size_a) 
{
    int log_length = log2(size_a);
    for (int i=0, j=0; i<size_a; ++i, j=0) {
        for (int k=0; k<log_length; ++k) {
            j = (j << 1)|(1 & (i>>k));
        }

        if (j<i) {
            // 交换
            complex_t t;
            t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }
}

static void __Real_Bit_reverse_local(vfloat_t a[], int size_a) 
{
    int log_length = log2(size_a);
    for (int i=0, j=0; i<size_a; ++i, j=0) {
        for (int k=0; k<log_length; ++k) {
            j = (j << 1)|(1 & (i>>k));
        }

        if (j<i) {
            // 交换
            vfloat_t t;
            t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }   
}

int Iterative_fast_fourier_transform(complex_t sequence[], int N, complex_t A[], int reverse) 
{
    // bit reverse_copy
    __Bit_reverse_copy(sequence, N, A);
    
    int n = log2(N);

    for (int s=1; s<=n; ++s) {
        int m = pow(2, s);
        int half_m = m>>1;
        complex_t wm = reverse? complex_rw(m, 1):complex_w(m, 1);
        for (int k=0; k<N; k=k+m) {
            complex_t w = _complex(1, 0);
            for (int j=0; j<(half_m); ++j) {
                complex_t t = complex_multiply(w, A[k+j+half_m]);
                complex_t u = A[k+j];
                A[k+j] = complex_sum(u, t);
                A[k+j+half_m] = complex_sub(u, t);
                w = complex_multiply(w, wm);
            }
        }
    }
    return 0;
}

int Iterative_fast_fourier_transform2(complex_t sequence[], int N, int reverse)
{
    // bit reverse_copy
    __Bit_reverse_local(sequence, N);
    int n = log2(N);

    for (int s=1; s<=n; ++s) {
        int m = pow(2, s);
        int half_m = m>>1;
        complex_t wm = reverse? complex_rw(m, 1):complex_w(m, 1);
        for (int k=0; k<N; k=k+m) {
            complex_t w = _complex(1, 0);
            for (int j=0; j<(half_m); ++j) {
                complex_t t = complex_multiply(w, sequence[k+j+half_m]);
                complex_t u = sequence[k+j];
                sequence[k+j] = complex_sum(u, t);
                sequence[k+j+half_m] = complex_sub(u, t);
                w = complex_multiply(w, wm);
            }
        }
    }
    return 0;
}

int Real_fast_fourier_transform(vfloat_t x[], int n)
{
    // 一下这段神奇的代码看不懂：反正最后的结果是：
    // [R(0), R(1), R(2), R(3), R(4), R(5)... R(N/2), i(n/2-1), i(n/2-2), i(n/2-3)....i(1)];
    /** 使用 DTF 算出来的结果：
    <276.000000 0.000000i, amplitude: 276.000000>

    <66.910201 -124.659086i, amplitude: 141.480963>
    <37.455819 -129.923888i, amplitude: 135.215217>
    <-104.564860 -100.443498i, amplitude: 144.992090>
    <-27.999990 54.000011i, amplitude: 60.827631>
    <38.138405 -50.502409i, amplitude: 63.285317>
    <-13.455880 -53.923874i, amplitude: 55.577377>
    <-48.483870 -106.717911i, amplitude: 117.215179>

    <-172.000000 0.000118i, amplitude: 172.000000>
    
    <-48.483725 106.718005i, amplitude: 117.215204>
    <-13.455784 53.923894i, amplitude: 55.577374>
    <38.138472 50.502368i, amplitude: 63.285324>
    <-28.000031 -53.999966i, amplitude: 60.827609>
    <-104.564717 100.443626i, amplitude: 144.992076>
    <37.456023 129.923839i, amplitude: 135.215227>
    <66.910395 124.658947i, amplitude: 141.480931>
    */

   /**
    * 使用本函数算出来的结果(一维数组):
    *  0 ~ 8 : 为实部：
    *  276.000000  66.910213  37.455844  -104.564833  -28.000000  38.138426  -13.455844  -48.483806  -172.000000
    *   9 ～ 15 为虚部：
    *   106.717952  53.923882  50.502396  -54.000000  100.443522  129.923882  124.659078
    * 
    *   参考以上两组数据，可以补全完整的 fft 变换。
   */

    int i, j, k, m, i1, i2, i3, i4, n1, n2, n4;
	vfloat_t a, e, cc, ss, xt, t1, t2;
    int N = n;
	for(j = 1, i = 1; i < 16; i++) {
		m = i;
		j = 2 * j;
		if(j == n) break;
	}
    
	n1 = n - 1;
	for(j = 0, i = 0; i < n1; i++) {
		if(i < j) {
			xt = x[j];
			x[j] = x[i];
			x[i] = xt;
		}
		k = n / 2;
		while(k < (j + 1)) {
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}
	for(i = 0; i < n; i += 2) {
		xt = x[i];
		x[i] = xt + x[i + 1];
		x[i + 1] = xt - x[i + 1];
	}
	n2 = 1;
	for(k = 2; k <= m; k++) {
		n4 = n2;
		n2 = 2 * n4;
		n1 = 2 * n2;
		e = 6.28318530718 / n1;
        // 此处开始收割计算结果
		for(i = 0; i < n; i += n1) {
			xt = x[i];
			x[i] = xt + x[i + n2];
			x[i + n2] = xt - x[i + n2];
			x[i + n2 + n4] = -x[i + n2 + n4];
			a = e;
			for(j = 1; j <= (n4-1); j++) {
				i1 = i + j;
				i2 = i - j + n2;
				i3 = i + j + n2;
				i4 = i - j + n1;
				cc = cos(a);
				ss = sin(a);
				a = a + e;
				t1 = cc * x[i3] + ss * x[i4];
				t2 = ss * x[i3] - cc * x[i4];
				x[i4] = x[i2] - t2;
				x[i3] = -x[i2] - t2;
				x[i2] = x[i1] - t1;
				x[i1] = x[i1] + t1;
			}
		}
	}
    return 0;
}

int Real_fast_fourier_transform_complex(vfloat_t sequence[], int N, complex_t out[]) 
{
    Real_fast_fourier_transform(sequence, N);

    for (int i=1, j=N-1; i<N/2; ++i, --j) {
        out[i].real = sequence[i];
        out[i].image = (-1)*sequence[j];
    }
    out[0].real = sequence[0]; out[0].image = 0.f;
    out[N/2].real = sequence[N/2]; out[N/2].image = 0.f;
    return 0;
}
int Reverse_recursive_fast_fourier_transorm(complex_t sequence[], int n, complex_t out[]) 
{
    Recursive_fast_fourier_transform(sequence, n, out, 1);
    for (int i=0; i<n; ++i) {
        out[i].real  /= (vfloat_t) n;
        out[i].image /= (vfloat_t) n;
    }
    return 0;
}

int Reverse_iterative_fast_fourier_transorm(complex_t sequence[], int N, complex_t A[])
{
    Iterative_fast_fourier_transform(sequence, N, A, 1);

    for (int i=0; i<N; ++i) {
        A[i].real  /= (vfloat_t) N;
        A[i].image /= (vfloat_t) N;
    }
    return 0;
}

int Reverse_iterative_fast_fourier_transorm2(complex_t sequence[], int N)
{
    Iterative_fast_fourier_transform2(sequence, N, 1);

    for (int i=0; i<N; ++i) {
        sequence[i].real  /= (vfloat_t) N;
        sequence[i].image /= (vfloat_t) N;
    }
    return 0;
}
