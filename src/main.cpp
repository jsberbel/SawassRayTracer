// ======================================================================
// File: main.cpp
// Revision: 1.0
// Creation: 05/17/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#include "core/utils.h"
#include "core/profiler.h"

#include "engine/world.h"
#include "engine/geometry/sphere.h"
#include "engine/material/lambertian.h"
#include "engine/material/metal.h"
#include "engine/material/dielectric.h"
#include "engine/camera.h"

#include <vector>

inline fv3 generate_color(const Ray& _ray, const World& _world, s32 _depth)
{
    if (Hit hit; _world.hit(_ray, 0.001f, std::numeric_limits<f32>::max(), &hit))
    {
        Ray scattered;
        fv3 attenuation;
        if (_depth < 50 && hit.material->scatter(_ray, hit, &attenuation, &scattered))
            return attenuation * generate_color(scattered, _world, _depth + 1);

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

inline World generate_rnd_world()
{
    World world;
    world.add<StaticSphere>(fv3(0.f, -1000.f, 0.f), 1000.f, Lambertian(fv3(0.5f)));

    for (s32 a = -10; a < 10; ++a)
    {
        for (s32 b = -10; b < 10; ++b)
        {
            f32 choose_mat = utils::rnd_01();
            fv3 center(a + 0.9f * utils::rnd_01(), 0.2f, b + 0.9f * utils::rnd_01());

            if ((center - fv3(4.f, 0.2f, 0.f)).get_length() > 0.9f)
            {
                if (choose_mat < 0.8f) // Diffuse
                {
                    auto rnd_sqr_unit = []() constexpr { return utils::rnd_01() * utils::rnd_01(); };
                    world.add<DynamicSphere>(center, center + fv3(0.f, 0.5f * utils::rnd_01(), 0.f), 0.f, 1.f, 0.2f,
                                             Lambertian(fv3(rnd_sqr_unit(), rnd_sqr_unit(), rnd_sqr_unit())));
                }
                else if (choose_mat < 0.95f) // Metal
                {
                    auto rnd_half_unit = []() constexpr { return 0.5f * (1.f + utils::rnd_01()); };
                    world.add<StaticSphere>(center, 0.2f, Metal(fv3(rnd_half_unit(), rnd_half_unit(), rnd_half_unit()), 0.5f * utils::rnd_01()));
                }
                else // Glass
                {
                    world.add<StaticSphere>(center, 0.2f, Dielectric(1.5f));
                }
            }
        }
    }

    world.add<StaticSphere>(fv3(0.f, 1.f, 0.f), 1.f, Dielectric(1.5f));
    world.add<StaticSphere>(fv3(-4.f, 1.f, 0.f), 1.f, Lambertian(fv3(0.4f, 0.2f, 0.1f)));
    world.add<StaticSphere>(fv3(4.f, 1.f, 0.f), 1.f, Metal(fv3(0.7f, 0.6f, 0.5f), 0.f));

    return world;
}

int main()
{
    PROFILER_BATCH_START(1);

    constexpr u32 width = 800;
    constexpr u32 height = 480;
    constexpr f32 resolution = width * height;
    constexpr u32 nb_samples = 30;

    std::vector<rgb> data;
    data.reserve(width * height);

    const World world = generate_rnd_world();

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
    constexpr f32 vertical_FOV = 20.f;
    constexpr f32 aperture = 0.f;
    constexpr f32 start_time = 0.f;
    constexpr f32 end_time = 1.f;
    Camera camera(look_from, look_at, width, height, vertical_FOV, aperture, start_time, end_time);

    //world.sort_by_distance( camera );

    for (s32 j = height - 1; j >= 0; --j)
    {
        for (s32 i = 0; i < width; ++i)
        {
            fv3 color(0.f);
            for (s32 s = 0; s < nb_samples; ++s)
            {
                const f32 u = static_cast<f32>(i + utils::rnd_01()) / static_cast<f32>(width);
                const f32 v = static_cast<f32>(j + utils::rnd_01()) / static_cast<f32>(height);
                const Ray ray = camera.trace_ray(u, v);
                const fv3 point = ray.point_at(2.f);
                color += generate_color(ray, world, 0);
            }
            color /= f32(nb_samples);
            color = fv3(std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]));

            const rgb rgb(u8(255.99 * color.x), u8(255.99 * color.y), u8(255.99 * color.z));
            data.push_back(rgb);
        }

        const f32 pct = (static_cast<f32>(height - j) / height) * 100.f;
        utils::output_to_console("%.2f%% completed", pct);
    }

    //utils::OutputImageToIncrementalFile( width, height, data );
    utils::output_img_to_file("test", width, height, data);

    PROFILER_BATCH_END_AND_LOG("test");

    return 0;
}
