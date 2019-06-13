#include <stdio.h>
#include <stdlib.h>

// https://en.wikipedia.org/wiki/Hilbert_curve

void hilbert_rot(unsigned long long n, unsigned *x, unsigned *y, unsigned long long rx, unsigned long long ry) {
        if (ry == 0) {
                if (rx == 1) {
                        *x = n - 1 - *x;
                        *y = n - 1 - *y;
                }

                unsigned t = *x;
                *x = *y;
                *y = t;
        }
}

unsigned long long hilbert_xy2d(unsigned long long n, unsigned x, unsigned y) {
        unsigned long long d = 0;
        unsigned long long rx, ry;

        for (unsigned long long s = n / 2; s > 0; s /= 2) {
                rx = (x & s) != 0;
                ry = (y & s) != 0;

                d += s * s * ((3 * rx) ^ ry);
                hilbert_rot(s, &x, &y, rx, ry);
        }

        return d;
}

void hilbert_d2xy(unsigned long long n, unsigned long long d, unsigned *x, unsigned *y) {
        unsigned long long rx, ry;
        unsigned long long t = d;

        *x = *y = 0;
        for (unsigned long long s = 1; s < n; s *= 2) {
                rx = 1 & (t / 2);
                ry = 1 & (t ^ rx);
                hilbert_rot(s, x, y, rx, ry);
                *x += s * rx;
                *y += s * ry;
                t /= 4;
        }
}

unsigned long long encode_hilbert(unsigned int wx, unsigned int wy) {
        return hilbert_xy2d(1LL << 32, wx, wy);
}

void decode_hilbert(unsigned long long index, unsigned *wx, unsigned *wy) {
        hilbert_d2xy(1LL << 32, index, wx, wy);
}

int main() {
	unsigned wx, wy;

	for (size_t i = 0; i < (1 << 4) * (1 << 4); i++) {
		unsigned long long index = (i + .5) * ((1LL << 32) / (1 << 4)) * ((1LL << 32) / (1 << 4));
		decode_hilbert(index, &wx, &wy);

		printf("%.6f %.6f %s\n", wx * 612.0 / (1LL << 32), wy * 612.0 / (1LL << 32), i == 0 ? "moveto" : "lineto");
	}

	printf("stroke\n");
}
