#include <cstdint>

extern const int32_t Hadamard_8x8_i32[8][8];
extern const int32_t Hadamard_16x16_i32[16][16];
extern const int32_t Hadamard_32x32_i32[32][32];
extern const int32_t Hadamard_64x64_i32[64][64];
extern const int32_t Hadamard_128x128_i32[128][128];
extern const int32_t Hadamard_256x256_i32[256][256];
extern const int32_t Hadamard_512x512_i32[512][512];
extern const int32_t Hadamard_1024x1024_i32[1024][1024];
extern const int32_t Hadamard_2048x2048_i32[2048][2048];
extern const int32_t Hadamard_4096x4096_i32[4096][4096];

template <int gf_size>
inline constexpr auto get_Hadamard_matrix()
{
	if constexpr (gf_size == 8)
	{
		return Hadamard_8x8_i32;
	}
	else if constexpr (gf_size == 16)
	{
		return Hadamard_16x16_i32;
	}
	else if constexpr (gf_size == 32)
	{
		return Hadamard_32x32_i32;
	}
	else if constexpr (gf_size == 64)
	{
		return Hadamard_64x64_i32;
	}
	else if constexpr (gf_size == 128)
	{
		return Hadamard_128x128_i32;
	}
	else if constexpr (gf_size == 256)
	{
		return Hadamard_256x256_i32;
	}
	else if constexpr (gf_size == 512)
	{
		return Hadamard_512x512_i32;
	}
	else if constexpr (gf_size == 1024)
	{
		return Hadamard_1024x1024_i32;
	}
	else if constexpr (gf_size == 2048)
	{
		return Hadamard_2048x2048_i32;
	}
	else if constexpr (gf_size == 4096)
	{
		return Hadamard_4096x4096_i32;
	}
	else
	{
		static_assert(gf_size == -1, "We never should be here !");
	}
}

template <int gf_size>
inline const int32_t *get_Hadamard_line(const int line)
{
	if constexpr (gf_size == 8)
	{
		return &Hadamard_8x8_i32[line][0];
	}
	else if constexpr (gf_size == 16)
	{
		return &Hadamard_16x16_i32[line][0];
	}
	else if constexpr (gf_size == 32)
	{
		return &Hadamard_32x32_i32[line][0];
	}
	else if constexpr (gf_size == 64)
	{
		return &Hadamard_64x64_i32[line][0];
	}
	else if constexpr (gf_size == 128)
	{
		return &Hadamard_128x128_i32[line][0];
	}
	else if constexpr (gf_size == 256)
	{
		return &Hadamard_256x256_i32[line][0];
	}
	else if constexpr (gf_size == 512)
	{
		return &Hadamard_512x512_i32[line][0];
	}
	else if constexpr (gf_size == 1024)
	{
		return &Hadamard_1024x1024_i32[line][0];
	}
	else if constexpr (gf_size == 2048)
	{
		return &Hadamard_2048x2048_i32[line][0];
	}
	else if constexpr (gf_size == 4096)
	{
		return &Hadamard_4096x4096_i32[line][0];
	}
	else
	{
		static_assert(gf_size == -1, "We never should be here !");
	}
}