#include "equalize.h"

void equalize_histogram(const uint8_t *input, uint8_t *output, size_t len) {
    uint32_t hist[256] = {0};
    for (size_t i = 0; i < len; i++) {
        hist[input[i]]++;
    }

    uint32_t cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    uint32_t cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) { cdf_min = cdf[i]; break; }
    }

    uint8_t table[256] = {0};
    uint32_t denom = (uint32_t)len - cdf_min;
    if (denom == 0) denom = 1;
    for (int i = 0; i < 256; i++) {
        table[i] = (cdf[i] <= cdf_min) ? 0 : (uint8_t)(((cdf[i] - cdf_min) * 255) / denom);
    }

    for (size_t i = 0; i < len; i++) {
        output[i] = table[input[i]];
    }
}
