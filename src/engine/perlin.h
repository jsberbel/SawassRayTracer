#pragma once

#include "core/math/v3.h"
#include "core/utils.h"

#include <array>

class Perlin
{
public:
    static constexpr f32 noise(const fv3& p);

private:
    static constexpr usz k_size = 256u;

    static constexpr void permute(std::array<sv3, k_size>& arr, s32 n);
    static constexpr std::array<f32, k_size> generate_frand();
    static constexpr std::array<sv3, k_size> generate_perm();

private:
    static inline std::array<f32, k_size> s_frand = generate_frand();
    static inline std::array<sv3, k_size> s_perm  = generate_perm();
};

constexpr f32 Perlin::noise(const fv3& p)
{
    const f32 u = p.x - math::floor(p.x);
    const f32 v = p.y - math::floor(p.y);
    const f32 w = p.z - math::floor(p.z);
    const s32 i = s32(4 * p.x) & 255;
    const s32 j = s32(4 * p.y) & 255;
    const s32 k = s32(4 * p.z) & 255;
    return s_frand[s_perm[i].x ^ s_perm[j].y ^ s_perm[k].z];
}

constexpr void Perlin::permute(std::array<sv3, k_size>& arr, s32 n)
{
    for (usz i = n-1; i > 0; --i)
    {
        const s32 target_x = s32(util::drand_01()*(i+1));
        const s32 target_y = s32(util::drand_01()*(i+1));
        const s32 target_z = s32(util::drand_01()*(i+1));
        std::swap(arr[i].x, arr[target_x].x);
        std::swap(arr[i].y, arr[target_y].y);
        std::swap(arr[i].z, arr[target_z].z);
    }
}

constexpr std::array<f32, Perlin::k_size> Perlin::generate_frand()
{
    std::array<f32, k_size> arr;
    for (usz i = 0; i < k_size; ++i)
        arr[i] = util::drand_01();
    return std::move(arr);
}

constexpr std::array<sv3, Perlin::k_size> Perlin::generate_perm()
{
    std::array<sv3, k_size> arr;
    for (usz i = 0; i < 256; ++i)
        arr[i].set(i);
    permute(arr, 256);
    return std::move(arr);
}
