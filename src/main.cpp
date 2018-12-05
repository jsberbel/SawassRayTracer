// ======================================================================
// File: main.cpp
// Revision: 1.0
// Creation: 05/17/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#include "core/utils.h"
#include "core/profiler.h"

#include "engine/hitablelist.h"
#include "engine/sphere.h"
#include "engine/bvh.h"
#include "engine/lambertian.h"
#include "engine/metal.h"
#include "engine/dielectric.h"
#include "engine/camera.h"

#include <vector>

inline fv3 generate_color(const Ray& _ray, Hitable* _world, f32 _time, s32 _depth)
{
    if (Hit hit; _world->hit(_ray, _time, 0.001f, std::numeric_limits<f32>::max(), &hit))
    {
        Ray scattered;
        fv3 attenuation;
        if (_depth < 50 && hit.material->scatter(_ray, hit, &attenuation, &scattered))
            return attenuation * generate_color(scattered, _world, _time, _depth + 1);

        return fv3(1.f, 0.f, 1.f);
    }

    const fv3 unit_dir = _ray.direction.get_normalized();
    const f32 t = 0.5f * (unit_dir.y + 1.f);
    return math::lerp(fv3(1.f), fv3(.5f, .7f, 1.f), t);
}

inline fv3 correct_gamma(const fv3& _color)
{
    // Formula: Vout = A * pow( Vin, y ) with A = 1 and y = 1/2
    return fv3(math::sqrt(_color.x), math::sqrt(_color.y), math::sqrt(_color.z));
}

inline Hitable* generate_rnd_world()
{
    constexpr u32 size = 50000;
    HitableList* list = new HitableList(size);

    Texture* checker = new CheckerTexture(new ConstTexture(fv3(0.2f, 0.3f, 0.1f)), new ConstTexture(fv3(0.9f, 0.9f, 0.9f)));
    list->add<Sphere>(Transform(fv3(0.f, -1000.f, 0.f)), 1000.f, Lambertian(checker));

    for (s32 a = -10; a < 10; ++a)
    {
        for (s32 b = -10; b < 10; ++b)
        {
            const f32 mat_to_choose = util::frand_01();
            const fv3 center(a + 0.9f * util::frand_01(), 0.2f, b + 0.9f * util::frand_01());

            if ((center - fv3(4.f, 0.2f, 0.f)).get_length() > 0.9f)
            {
                if (mat_to_choose < 0.8f) // Diffuse
                {
                    auto rnd_sqr_unit = []() constexpr { return util::frand_01() * util::frand_01(); };
                    Transform tf(center, center + fv3(0.f, 0.5f * util::frand_01(), 0.f));
                    Lambertian mt(new ConstTexture(fv3(rnd_sqr_unit(), rnd_sqr_unit(), rnd_sqr_unit())));
                    list->add<Sphere>(std::move(tf), 0.2f, std::move(mt));
                }
                else if (mat_to_choose < 0.95f) // Metal
                {
                    auto rnd_half_unit = []() constexpr { return 0.5f * (1.f + util::frand_01()); };
                    Transform tf(center);
                    Metal mt(fv3(rnd_half_unit(), rnd_half_unit(), rnd_half_unit()), 0.5f * util::frand_01());
                    list->add<Sphere>(std::move(tf), 0.2f, std::move(mt));
                }
                else // Glass
                {
                    Transform tf(center);
                    Dielectric mt(1.5f);
                    list->add<Sphere>(std::move(tf), 0.2f, std::move(mt));
                }
            }
        }
    }
    
    list->add<Sphere>(Transform(fv3(0.f, 1.f, 0.f)),  1.f, Dielectric(1.5f));
    list->add<Sphere>(Transform(fv3(-4.f, 1.f, 0.f)), 1.f, Lambertian(new ConstTexture(fv3(0.4f, 0.2f, 0.1f))));
    list->add<Sphere>(Transform(fv3(4.f, 1.f, 0.f)),  1.f, Metal(fv3(0.7f, 0.6f, 0.5f), 0.f));
    
    //return list;
    return new BVH(list->get_buffer(), list->get_size(), 0.0, 1.0);
}

inline Hitable* generate_perlin_spheres()
{
    Texture* pertext = new NoiseTexture(1);
    Texture* pertext2 = new NoiseTexture(2);
    HitableList* list = new HitableList(2);
    list->add<Sphere>(Transform(fv3(0, -1000, 0)), 1000.f, Lambertian(pertext));
    list->add<Sphere>(Transform(fv3(0, 2, 0)), 2.f, Lambertian(pertext2));
    return list;
}

int main()
{
    PROFILER_BATCH_START(1);

    constexpr u32 width = 600;
    constexpr u32 height = 480;
    constexpr u32 nb_samples = 30;

    std::vector<rgb> data;
    data.reserve(width * height);

    Hitable* world = generate_rnd_world();
    //Hitable* world = generate_perlin_spheres();

    constexpr fv3 look_from(-13.f, 2.f, 3.f);
    constexpr fv3 look_at(0.f, 0.f, 0.f);
    constexpr f32 v_FOV = 25.f;
    constexpr f32 aperture = 0.f;
    Camera camera(look_from, look_at, width, height, v_FOV, aperture);

    /*constexpr f32 start_time = 0.f;
    constexpr f32 end_time = 1.f;*/

    //world.sort_by_distance( camera );
    
    constexpr f32 inv_width       = math::inv(f32(width));
    constexpr f32 inv_height      = math::inv(f32(height));
    constexpr f32 inv_nb_samples  = math::inv(f32(nb_samples));
    const f32 sqrt_nb_samples     = math::sqrt(f32(nb_samples));
    const f32 inv_sqrt_nb_samples = math::inv(sqrt_nb_samples);

    for (s32 y = height - 1; y >= 0; --y)
    {
        for (s32 x = 0; x < s32(width); ++x)
        {
            fv3 color = fv3::zero();
            for (s32 s = 0; s < s32(nb_samples); ++s)
            {
                const f32 sx    = f32(std::fmod(s, sqrt_nb_samples)) * inv_sqrt_nb_samples; // util::frand_01()
                const f32 sy    = u32(s / sqrt_nb_samples) * inv_sqrt_nb_samples; // util::frand_01()
                const f32 u     = (x + sx) * inv_width;
                const f32 v     = (y + sy) * inv_height;
                const Ray ray   = camera.trace_ray(u, v);
                const f32 time  = f32(s) * inv_nb_samples; //util::frand_01();
                const s32 depth = 0;
                color += generate_color(ray, world, time, depth);
            }
            color *= inv_nb_samples;
            color = correct_gamma(color);

            const rgb rgb((255.99f * color).cast<u8>());
            data.push_back(rgb);
        }

        const f32 pct = (f32(height - y) / height) * 100.f;
        util::output_to_console("%.2f%% completed", pct);
    }

    util::output_img_to_incremental_file( width, height, data );
    //util::output_img_to_file("test", width, height, data);

    util::safe_del(world);

    PROFILER_BATCH_END_AND_LOG("test");

    return 0;
}
