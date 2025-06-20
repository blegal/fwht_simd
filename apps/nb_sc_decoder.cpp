
#include "../src/fwht_x86.hxx"
#include "../src/fwht_neon.hxx"
#include "../src/fwht_avx2.hxx"
#include <cstring>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include"const_config_GF64_N64.hpp"




bool are_equivalent(float *a, float *b, float epsilon, int size)
{
    for (int i = 0; i < size; i++)
    {
        float diff = abs(a[i] - b[i]);
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
    float value[GF];
    char gf[GF];
    bool is_freq;
};

template <int gf_size>
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
        dst->llr[i] = src_a->llr[i] * src_b->llr[i];
        dst->gf[i] = src_a->gf[i]; // gf are from 0 to gf-1, in freq domain we only multiply element-by-element the 2 vectors
    }
    dst->is_freq = true; // a.a we do CN in FD
}

template <int gf_size>
void g_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)

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

    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i]; // a.a in PB VN is element by element multiplication
        dst->gf[i] = src_a->gf[i];                         // a.a
    }
    dst->is_freq = false; // a.a we do VN in PD

}

template <int gf_size>
int32_t final_node(symbols_t *var, int16_t *decoded, const int symbol_id)
{
    printf("-> final_node(%d) : frozen = %d\n", symbol_id, frozen_symbols[symbol_id]);
    if (frozen_symbols[symbol_id] == -1)
    {
        return frozen_symbols[symbol_id];
    }
    if (var->is_freq)
    {
        fwht<gf_size>(var->value);
        var->is_freq = false;
    }

    int max_index = 0;
    float max_value = var->value[0];
    for (int i = 1; i < gf_size; i++)
    {
        if (var->value[i] > max_value)
        {
            max_value = var->value[i];
            max_index = i;
        }
    }
    decoded[symbol_id] = max_index;

    return max_index; // I suppose this is the index of the maximum value: a.a YES IT IS
}

template <int gf_size>

int main(int argc, char *argv[])
{

    symbols_t *channel = new symbols_t[N];
    symbols_t *internal = new symbols_t[N];
    int16_t *decoded = new int16_t[N];
    symbols_t *channel = new symbols_t[N];

    int i, j;
    uint16_t N1 = N * GF;

    for (i = 0; i < K; i++)
        decoded[reliab_seq[i]] = 0;
    for (i = K; i < N; i++)
        decoded[reliab_seq[i]] = 65535;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < GF; j++)
        {
            channel[i].value[j] = chan[i * GF + j];
            channel[i].gf[j] = j;
        }
        channel[i].is_freq = false;
    }

    bool *Roots[logN+1];

    for (int i = 0; i <= logN; i++)
    {
        int len = 1 << i;
        Roots[i] = new bool[len];
        for (int j = 0; j < len; j++)
            Roots[i][j] = false;
    }


    int l=0;
    int s=0;
    int l_leaf=1;
    int l_s_leaf=0;
    int r_s_leaf=0;
    while(1)
    {
        int 
        if(Roots[l_leaf][s_lea])
    }


    delete[] channel;
    delete[] internal;
    delete[] decoded;

    return EXIT_SUCCESS;
}
