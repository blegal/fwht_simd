//
// Created by legal on 03/07/2025.
//
#pragma once

template <int gf_size>
void normalize(float * tab) {
    float sum = 1e-32f;
    for (int i = 0; i < gf_size; i += 1) {
        sum += tab[i];
    }
    const float factor = 1.f / sum;
    for (int i = 0; i < gf_size; i++) {
        tab[i] *= factor;
    }
}


#if 0
#include <immintrin.h>
template <int gf_size>
void multiply_and_normalize_avx2(float *dst, const float *src1, const float *src2)
{
    // Initialiser la somme des carrés à une petite valeur pour éviter norm == 0
    __m256 sum_sq = _mm256_set1_ps(1e-32f);

    // Étape 1 : multiplication élément par élément + accumulation des carrés
    for (size_t i = 0; i < gf_size; i += 8) {
        __m256 a = _mm256_loadu_ps(src1 + i);
        __m256 b = _mm256_loadu_ps(src2 + i);
        __m256 prod = _mm256_mul_ps(a, b);

        _mm256_storeu_ps(dst + i, prod);

        __m256 prod_sq = _mm256_mul_ps(prod, prod);
        sum_sq = _mm256_add_ps(sum_sq, prod_sq);
    }

    // Étape 2 : réduction horizontale
    __m128 low = _mm256_castps256_ps128(sum_sq);
    __m128 high = _mm256_extractf128_ps(sum_sq, 1);
    __m128 sum128 = _mm_add_ps(low, high);
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);

    float sumsq = _mm_cvtss_f32(sum128);
    float norm = sqrtf(sumsq);

    float inv_norm = 1.0f / norm;
    __m256 inv_norm_vec = _mm256_set1_ps(inv_norm);

    // Étape 3 : normalisation par multiplication avec l'inverse
    for (size_t i = 0; i < gf_size; i += 8) {
        __m256 v = _mm256_loadu_ps(dst + i);
        __m256 normalized = _mm256_mul_ps(v, inv_norm_vec);
        _mm256_storeu_ps(dst + i, normalized);
    }
}

void multiply_and_normalize_avx2_unrolled8(const float *src1, const float *src2, float *dst)
{
    __m256 sum_sq = _mm256_set1_ps(1e-32f);

    __m256 a = _mm256_loadu_ps(src1);
    __m256 b = _mm256_loadu_ps(src2);
    __m256 prod = _mm256_mul_ps(a, b);
    sum_sq = _mm256_fmadd_ps(prod, prod, sum_sq);

    __m128 low = _mm256_castps256_ps128(sum_sq);
    __m128 high = _mm256_extractf128_ps(sum_sq, 1);
    __m128 sum128 = _mm_add_ps(low, high);
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);

    float sumsq = _mm_cvtss_f32(sum128);
    float norm = sqrtf(sumsq);
    float inv_norm = 1.0f / norm;

    __m256 inv_norm_vec = _mm256_set1_ps(inv_norm);
    _mm256_storeu_ps(dst, _mm256_mul_ps(prod, inv_norm_vec));
}

void multiply_and_normalize_avx2_unrolled32(const float *src1, const float *src2, float *dst)
{
    // Initialiser somme des carrés à une petite valeur pour éviter la division par zéro
    __m256 sum_sq = _mm256_set1_ps(1e-32f);

    // Déroulé pour 32 floats = 4 blocs de 8
    __m256 prod0, prod1, prod2, prod3;

    __m256 a0 = _mm256_loadu_ps(src1 + 0);
    __m256 b0 = _mm256_loadu_ps(src2 + 0);
    prod0 = _mm256_mul_ps(a0, b0);
    sum_sq = _mm256_fmadd_ps(prod0, prod0, sum_sq);

    __m256 a1 = _mm256_loadu_ps(src1 + 8);
    __m256 b1 = _mm256_loadu_ps(src2 + 8);
    prod1 = _mm256_mul_ps(a1, b1);
    sum_sq = _mm256_fmadd_ps(prod1, prod1, sum_sq);

    __m256 a2 = _mm256_loadu_ps(src1 + 16);
    __m256 b2 = _mm256_loadu_ps(src2 + 16);
    prod2 = _mm256_mul_ps(a2, b2);
    sum_sq = _mm256_fmadd_ps(prod2, prod2, sum_sq);

    __m256 a3 = _mm256_loadu_ps(src1 + 24);
    __m256 b3 = _mm256_loadu_ps(src2 + 24);
    prod3 = _mm256_mul_ps(a3, b3);
    sum_sq = _mm256_fmadd_ps(prod3, prod3, sum_sq);

    // Réduction horizontale de sum_sq
    __m128 low = _mm256_castps256_ps128(sum_sq);
    __m128 high = _mm256_extractf128_ps(sum_sq, 1);
    __m128 sum128 = _mm_add_ps(low, high);
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);
    float sumsq = _mm_cvtss_f32(sum128);
    float norm = sqrtf(sumsq);
    float inv_norm = 1.0f / norm;

    __m256 inv_norm_vec = _mm256_set1_ps(inv_norm);

    // Normalisation et stockage
    _mm256_storeu_ps(dst + 0, _mm256_mul_ps(prod0, inv_norm_vec));
    _mm256_storeu_ps(dst + 8, _mm256_mul_ps(prod1, inv_norm_vec));
    _mm256_storeu_ps(dst + 16, _mm256_mul_ps(prod2, inv_norm_vec));
    _mm256_storeu_ps(dst + 24, _mm256_mul_ps(prod3, inv_norm_vec));
}
#endif