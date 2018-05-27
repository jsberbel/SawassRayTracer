#include <vector>

#include <Core/Utils.h>
#include <Core/Limits.h>

#include <Scene/World.h>
#include <Scene/Geometry/Sphere.h>
#include <Scene/Material/Lambertian.h>
#include <Scene/Material/Metal.h>
#include <Scene/Material/Dielectric.h>

#include <Engine/Camera.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

inline FVec3 GenerateColor( const Ray& ray, const World& world, S32 depth )
{
    if( HitRecord record; world.Hit( ray, 0.001f, Limits<F32>::Max(), record ) )
    {
        Ray scattered;
        FVec3 attenuation;
        if( depth < 50 && record.Material->Scatter( ray, record, attenuation, scattered ) )
            return attenuation * GenerateColor( scattered, world, depth + 1 );

        return FVec3( 0.f );
    }

    FVec3 unitDirection = ray.Direction.Normalize();
    F32 t = 0.5f * ( unitDirection.Y + 1.f );
    return Math::Blend( FVec3(1.f), FVec3(.5f, .7f, 1.f), t );
}

inline FVec3 GammaCorrection( const FVec3& color )
{
    // Formula: Vout = A * pow( Vin, y ) with A = 1 and y = 1/2
    return FVec3( std::sqrt(color.R), std::sqrt(color.G), std::sqrt(color.B) );
}

int main()
{
    constexpr U32 width  = 200;
    constexpr U32 height = 100;
    constexpr F32 resolution = width*height;
    constexpr U32 numSamples = 600;

    std::vector<RGB> data;
    data.reserve( width * height );

    World world;
    world.Add<Sphere>( FVec3( 0.f, 0.f, -1.f ),       0.5f,    Lambertian( FVec3( 0.8f, 0.3f, 0.3f ) ) );
    world.Add<Sphere>( FVec3( 0.f, -100.5f, -1.f ),   100.f,   Lambertian( FVec3( 0.8f, 0.8f, 0.f ) ) );
    world.Add<Sphere>( FVec3( 1.f, 0.f, -1.f ),       0.5f,    Metal( FVec3( 0.8f, 0.6f, 0.2f ), 0.3f ) );
    world.Add<Sphere>( FVec3( -1.f, 0.f, -1.f ),      0.5f,    Dielectric( 1.5f ) );
    world.Add<Sphere>( FVec3( -1.f, 0.f, -1.f ),     -0.45f,   Dielectric( 1.5f ) );

    Camera camera( FVec3(0.f), F32(width), F32(height) );

    F32 progress = 0;

    for ( S32 j = height - 1; j >= 0; --j )
    {
        for ( S32 i = 0; i < width; ++i )
        {
            FVec3 color( 0.f );
            for ( S32 s = 0; s < numSamples; ++s )
            {
                F32 u = F32( i + Utils::Random01() ) / F32( width );
                F32 v = F32( j + Utils::Random01() ) / F32( height );
                Ray ray = camera.TraceRay( u, v );
                FVec3 point = ray.PointAt( 2.f );
                color += GenerateColor( ray, world, 0 );
            }
            color /= F32( numSamples );
            color = FVec3( std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]) );

            RGB rgb( Byte( 255.99 * color.R ), Byte( 255.99 * color.G ), Byte( 255.99 * color.B ) );
            data.push_back(rgb);

            const F32 percentage = (++progress / resolution) * 100.f;
            Utils::DebugLog( "Progress: %.2f%% - Computing pixel [%d, %d] = { %d, %d, %d }", percentage, j, i, rgb.R, rgb.G, rgb.G );
        }
    }

    if ( stbi_write_png( "output.png", width, height, 3, &data[0], 0 ) )
    {
        Utils::DebugLog( "Image saved successfully!" );
    }

    return 0;
}