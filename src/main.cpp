// ======================================================================
// File: main.cpp
// Revision: 1.0
// Creation: 05/17/2018 - jsberbel
// Notice: Copyright � 2018 by Jordi Serrano Berbel. All Rights Reserved.
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

        return fv3(0.f);
    }

    const fv3 unit_dir = _ray.direction.get_normalized();
    const f32 t = 0.5f * (unit_dir.y + 1.f);
    return math::lerp(fv3(1.f), fv3(.5f, .7f, 1.f), t);
}

inline fv3 gamma_correction(const fv3& _color)
{
    // Formula: Vout = A * pow( Vin, y ) with A = 1 and y = 1/2
    return fv3(std::sqrt(_color.x), std::sqrt(_color.y), std::sqrt(_color.z));
}

inline Hitable* generate_rnd_world()
{
    const u32 size = 50000;
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
    
    list->add<Sphere>(Transform(fv3(0.f, 1.f, 0.f)), 1.f, Dielectric(1.5f));
    list->add<Sphere>(Transform(fv3(-4.f, 1.f, 0.f)), 1.f, Lambertian(new ConstTexture(fv3(0.4f, 0.2f, 0.1f))));
    list->add<Sphere>(Transform(fv3(4.f, 1.f, 0.f)), 1.f, Metal(fv3(0.7f, 0.6f, 0.5f), 0.f));
    
    //return list;
    return new BVH(list->get_buffer(), list->get_size(), 0.0, 1.0);
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

    /*world.add<Sphere>( fv3( 0.f, 0.f, -1.f ),       0.5f,    Lambertian( fv3( 0.8f, 0.3f, 0.3f ) ) );
    world.add<Sphere>( fv3( 0.f, -100.5f, -1.f ),   100.f,   Lambertian( fv3( 0.8f, 0.8f, 0.f ) ) );
    world.add<Sphere>( fv3( 1.f, 0.f, -1.f ),       0.5f,    Metal( fv3( 0.8f, 0.6f, 0.2f ), 0.3f ) );
    world.add<Sphere>( fv3( -1.f, 0.f, -1.f ),      0.5f,    Dielectric( 1.5f ) );
    world.add<Sphere>( fv3( -1.f, 0.f, -1.f ),     -0.45f,   Dielectric( 1.5f ) );*/

    /*f32 R = std::cos( Math::PI<f32> / 4.f );
    world.add<Sphere>( fv3( -R, 0.f, -1.f ), R, Lambertian( fv3( 0.f, 0.f, 1.f ) ) );
    world.add<Sphere>( fv3( +R, 0.f, -1.f ), R, Lambertian( fv3( 1.f, 0.f, 0.f ) ) );*/

    constexpr fv3 look_from(13.f, 2.f, 3.f);
    constexpr fv3 look_at(0.f);
    constexpr f32 v_FOV = 20.f;
    constexpr f32 aperture = 0.f;
    Camera camera(look_from, look_at, width, height, v_FOV, aperture);

    /*constexpr f32 start_time = 0.f;
    constexpr f32 end_time = 1.f;*/

    //world.sort_by_distance( camera );

    for (s32 j = height - 1; j >= 0; --j)
    {
        for (s32 i = 0; i < s32(width); ++i)
        {
            fv3 color(0.f);
            for (s32 s = 0; s < s32(nb_samples); ++s)
            {
                const f32 u = static_cast<f32>(i + util::frand_01()) / static_cast<f32>(width);
                const f32 v = static_cast<f32>(j + util::frand_01()) / static_cast<f32>(height);
                const Ray ray = camera.trace_ray(u, v);
                const f32 time =  (static_cast<f32>(s) / nb_samples) /** (end_time - start_time)*/; //util::frand_01();
                const s32 depth = 0;
                color += generate_color(ray, world, time, depth);
            }
            color /= f32(nb_samples);
            color = fv3(std::sqrt(color.x), std::sqrt(color.y), std::sqrt(color.z));

            const rgb rgb((255.99f * color).cast<u8>());
            data.push_back(rgb);
        }

        const f32 pct = (static_cast<f32>(height - j) / height) * 100.f;
        util::output_to_console("%.2f%% completed", pct);
    }

    util::output_img_to_incremental_file( width, height, data );
    //util::output_img_to_file("test", width, height, data);

    delete world;

    PROFILER_BATCH_END_AND_LOG("test");

    return 0;
}