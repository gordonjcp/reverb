/*

reverb.c
Effect engine for a simple reverb plugin

Copyright (c) 2015 Gordon JC Pearce <gordonjcp@gjcp.net>

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright notice
and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

*/

#include "reverb.h"

/* these values are taken from the CCRMA article on Schroeder's original reverb */
unsigned int tap[NUM_COMBS] = {2975, 2824, 3621, 3970};
float tap_gain[NUM_COMBS] = {0.773, 0.802, 0.753, 0.733};

void reverb(float *in, float *out, unsigned long sample_count, reverb_t *params) {
    // handle the actual processing
    unsigned long pos;
    unsigned long comb_pos = params->comb_pos;
    int c;

    float * const input = in;
    float * const output = out;

    float decay=params->decay;

    float temp;

    /* loop around the buffer */
    for (pos = 0; pos < sample_count; pos++) {
        /* loop around the comb filters */
        temp = 0;
        for (c = 0; c<3; c++) {
            params->comb[c][(comb_pos + tap[c]) & COMB_MASK] =
                input[pos] + (decay * tap_gain[c]) * params->comb[c][comb_pos];
            temp += params->comb[c][comb_pos];
        }
        output[pos] = temp * 0.35;
        comb_pos++;
        comb_pos &= COMB_MASK;  /* increment and wrap buffer */
        params->comb_pos = comb_pos;
    }

}
