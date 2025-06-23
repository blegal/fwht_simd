
#include "../src/fwht_x86.hxx"
#include "../src/fwht_neon.hxx"
#include "../src/fwht_avx2.hxx"
#include <cstring>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "../src/const_config_GF64_N64.hpp"

struct symbols_t
{
    value_type value[GF];
    gf_type gf[GF];
    bool is_freq;
};

template <uint32_t gf_size>
void f_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{
    value_type s1 = 0, s2;
    for (size_t i = 0; i < gf_size; i++)
        dst->value[i] = 1e30;
    for (uint16_t i = 0; i < gf_size; i++)
    {
        for (uint16_t j = 0; j < gf_size; j++)
        {
            uint16_t tmp = i ^ j;
            value_type temp_val = src_a->value[i] + src_b->value[j];
            if (temp_val < dst->value[tmp])
            {
                dst->value[tmp] = temp_val;
            }
        }
    }
}

template <uint32_t gf_size>
void g_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b, gf_type decision_left)

{
    gf_type temp_gf;
    value_type s1 = 0;
    for (size_t i = 0; i < gf_size; i++)
        dst->value[i] = 1e30;
    value_type min1 = 1e30;
    for (size_t i = 0; i < gf_size; i++)
    {
        temp_gf = decision_left ^ i;
        dst->value[temp_gf] = src_a->value[i] + src_b->value[temp_gf]; // a.a in PB VN is element by element multiplication
        dst->gf[i] = src_a->gf[i];                                     // a.a
        s1 += dst->value[temp_gf];
        if (min1 > dst->value[temp_gf])
            min1 = dst->value[temp_gf];
    }
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] -= min1;
    }
}

int main(int argc, char *argv[])
{
    symbols_t layers_arr[tot_clust]; // tot_clust=127=1+2+4+8+16+32+64
    symbols_t *layers[logN + 1];
    int offset = 0;
    for (int i = 0; i <= logN; ++i)
    {
        layers[i] = layers_arr + offset;
        offset += (N >> i);
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < GF; ++j)
        {
            layers[0][i].value[j] = chan[i * GF + j]; // remove /100000 i put it because the input data i rounded them to their x1000
            layers[0][i].gf[j] = j;
        }
    }

    bool Roots_arr[tot_clust];
    bool *Roots[logN + 1];
    offset = 0;
    for (int i = 0; i <= logN; ++i)
    {
        Roots[i] = Roots_arr + offset;
        offset += (1 << i);
        for (int j = 0; j < (1 << i); ++j)
        {
            Roots[i][j] = false;
        }
    }

    gf_type decoded_arr[logN * N];
    gf_type *decoded_layers[logN];
    offset = 0;
    for (int i = 0; i < logN; ++i)
    {
        decoded_layers[i] = decoded_arr + offset;
        offset += N;
        for (int j = 0; j < N; ++j)
        {
            decoded_layers[i][j] = max_gf;
        }
    }
    for (int i = K; i < N; i++)
        decoded_layers[logN - 1][reliab_seq[i]] = 0; // frozen

    uint32_t N1 = N * GF;

    int l = 0;
    int s = 0;
    while (true)
    {
        if (!Roots[l + 1][2 * s])
        {
            uint32_t len = N >> (l + 1);

            for (int i = 0; i < len; ++i)
            {
                f_function<GF>(&layers[l + 1][i], &layers[l][i], &layers[l][i + len]);
                bool PAUSE = 0;
            }
            l += 1;
            s *= 2;
            if (l == logN)
            {
                if (!(decoded_layers[logN - 1][s] == 0))
                {
                    value_type s1 = layers[l][0].value[0];
                    decoded_layers[logN - 1][s] = 0;
                    for (size_t i = 1; i < GF; i++)
                    {
                        if (layers[l][0].value[i] < s1)
                        {
                            s1 = layers[l][0].value[i];
                            decoded_layers[logN - 1][s] = i;
                        }
                    }
                    bool PAUSE = 0;
                }
                Roots[l][s] = true;
                l -= 1;
                s /= 2;
            }
        }

        else if (!Roots[l + 1][2 * s + 1])
        {
            uint32_t len = N >> (l + 1);
            uint32_t len1 = N >> l;

            for (int i = 0; i < len; ++i)
            {
                uint32_t id1 = s * len1 + i;
                gf_type decision = decoded_layers[l][id1];
                g_function<GF>(&layers[l + 1][i], &layers[l][i], &layers[l][i + len], decision);
                bool PAUSE = 0;
            }
            l += 1;
            s = (s << 1) + 1;
            if (l == logN)
            {
                if (!(decoded_layers[logN - 1][s] == 0))
                {
                    value_type s1 = layers[l][0].value[0];
                    decoded_layers[logN - 1][s] = 0;
                    for (size_t i = 1; i < GF; i++)
                    {
                        if (layers[l][0].value[i] < s1)
                        {
                            s1 = layers[l][0].value[i];
                            decoded_layers[logN - 1][s] = i;
                        }
                    }
                    bool PAUSE = 0;
                }
                Roots[l][s] = true;
                if (s == N - 1)
                    break;
                l -= 1;
                s >>= 1;
            }
        }
        else
        {
            uint32_t len = N >> (l + 1);
            uint32_t len1 = N >> l;
            for (int i = 0; i < len; ++i)
            {
                uint32_t id1 = s * len1 + i;
                decoded_layers[l - 1][id1] = decoded_layers[l][id1] ^ decoded_layers[l][id1 + len];
                decoded_layers[l - 1][id1 + len] = decoded_layers[l][id1 + len];
            }
            Roots[l][s] = true;
            l -= 1;
            s >>= 1;
        }
    }
    for (int i = 0; i < N; ++i)
    {
        std::cout << decoded_layers[logN - 1][i] << " ";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
