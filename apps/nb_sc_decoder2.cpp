
#include "../src/fwht_x86.hxx"
#include "../src/fwht_neon.hxx"
#include "../src/fwht_avx2.hxx"
#include <cstring>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "../src/const_config_GF64_N64.hpp"

bool are_equivalent(value_type *a, value_type *b, value_type epsilon, uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        value_type diff = abs(a[i] - b[i]);
        if (diff > epsilon)
        {
            printf("- maximum absolute error is : %f\n", diff);
            printf("- a[%d] = %f and b[%d] = %f\n", i, a[i], i, b[i]);
            return false;
        }
    }
    return true;
}
struct symbols_t
{
    value_type value[GF];
    gf_type gf[GF];
    bool is_freq;
};

template <uint32_t gf_size>
void f_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{

    if (src_a->is_freq == false)
    {
        fwht<gf_size>(src_a->value);
        src_a->is_freq = true;
    }

    if (src_b->is_freq == false)
    {
        fwht<gf_size>(src_b->value);
        src_b->is_freq = true;
    }

    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
        dst->gf[i] = src_a->gf[i]; // gf are from 0 to gf-1, in freq domain we only multiply element-by-element the 2 vectors
    }
    dst->is_freq = true; // a.a we do CN in FD

    fwht<gf_size>(dst->value);
    dst->is_freq = false;

    value_type s1 = 0;
    for (size_t i = 0; i < gf_size; i++)
    {
        s1 += dst->value[i];
    }
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] /= s1;
    }
}

template <uint32_t gf_size>
void g_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b, gf_type decision_left)

{
    symbols_t result;
    if (src_a->is_freq == true)
    {
        fwht<gf_size>(src_a->value);
        src_a->is_freq = false;
    }

    if (src_b->is_freq == true)
    {
        fwht<gf_size>(src_b->value);
        src_b->is_freq = false;
    }
    gf_type temp_gf;
    value_type s1 = 0;
    // value_type temp_val[GF];
    // for (size_t i = 0; i < gf_size; i++)
    // {
    //     temp_gf = decision_left ^ i;
    //     temp_val[temp_gf] = src_a->value[i];
    // }

    for (size_t i = 0; i < gf_size; i++)
    {
        temp_gf = decision_left ^ i;
        dst->value[temp_gf] = src_a->value[i] * src_b->value[temp_gf]; // a.a in PB VN is element by element multiplication
        dst->gf[i] = src_a->gf[i];                                     // a.a
        s1 += dst->value[temp_gf];
    }
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] /= s1;
    }
    dst->is_freq = false; // a.a we do VN in PD
}

template <uint32_t gf_size>
void final_node(symbols_t *var, gf_type *decoded, const uint32_t symbol_id)
{
    if (var->is_freq)
    {
        fwht<gf_size>(var->value);
        var->is_freq = false;
    }

     value_type s1 = 0;
    for (size_t i = 0; i < gf_size; i++)
    {
        s1 += var->value[i];
    }
    for (size_t i = 0; i < gf_size; i++)
    {
        var->value[i] /= s1;
    }

    int max_index = 0;
    value_type max_value = var->value[0];
    for (int i = 1; i < gf_size; i++)
    {
        if (var->value[i] > max_value)
        {
            max_value = var->value[i];
            max_index = i;
        }
    }
    decoded[symbol_id] = max_index;
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
            layers[0][i].value[j] = chan[i * GF + j]/100000 ; // remove /100000 i put it because the input data i rounded them to their x1000
            layers[0][i].gf[j] = j;
        }
        layers[0][i].is_freq = false;
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
                    final_node<GF>(&layers[l][0], decoded_layers[logN - 1], s);
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
                    final_node<GF>(&layers[l][0], decoded_layers[logN - 1], s);
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
    for (int i = 0; i < K; ++i)
    {
        std::cout << decoded_layers[logN - 1][reliab_seq[i]] << " ";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
