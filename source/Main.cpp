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

inline World GenerateRandomSpheresWorld()
{
    U32 nbSpheres = 50u;

    World world;
    world.Add<Sphere>( FVec3( 0.f, -1000.f, 0.f ), 1000.f, Lambertian( FVec3( 0.5f ) ) );

    for( S32 a = -11; a < 11; ++a )
    {
        for( S32 b = -11; b < 11; ++b )
        {
            F32 chooseMat = Utils::Random01();
            FVec3 center( a + 0.9f*Utils::Random01(), 0.2f, b + 0.9f*Utils::Random01() );
            if( ( center - FVec3( 4.f, 0.2f, 0.f ) ).Length() > 0.9f )
            {
                if( chooseMat < 0.8f ) // Diffuse
                {
                    auto RandomQuadraticUnit = []() constexpr { return Utils::Random01() * Utils::Random01(); };
                    world.Add<Sphere>( center, 0.2f, Lambertian( FVec3( RandomQuadraticUnit(), RandomQuadraticUnit(), RandomQuadraticUnit() ) ) );
                }
                else if( chooseMat < 0.95f ) // Metal
                {
                    auto RandomSemiUnit = []() constexpr { return 0.5f*( 1.f + Utils::Random01() ); };
                    world.Add<Sphere>( center, 0.2f, Metal( FVec3( RandomSemiUnit(), RandomSemiUnit(), RandomSemiUnit() ), 0.5f*Utils::Random01() ) );
                }
                else // Glass
                {
                    world.Add<Sphere>( center, 0.2f, Dielectric( 1.5f ) );
                }
            }
        }
    }

    world.Add<Sphere>( FVec3(0.f, 1.f, 0.f),  1.f, Dielectric( 1.5f ) );
    world.Add<Sphere>( FVec3(-4.f, 1.f, 0.f), 1.f, Lambertian( FVec3(0.4f, 0.2f, 0.1f) ) );
    world.Add<Sphere>( FVec3(4.f, 1.f, 0.f),  1.f, Metal( FVec3(0.7f, 0.6f, 0.5f), 0.f ) );

    return world;
}

int main()
{
    constexpr U32 width  = 200;
    constexpr U32 height = 100;
    constexpr F32 resolution = width*height;
    constexpr U32 numSamples = 20;

    std::vector<RGB> data;
    data.reserve( width * height );

    World world = GenerateRandomSpheresWorld();

    /*world.Add<Sphere>( FVec3( 0.f, 0.f, -1.f ),       0.5f,    Lambertian( FVec3( 0.8f, 0.3f, 0.3f ) ) );
    world.Add<Sphere>( FVec3( 0.f, -100.5f, -1.f ),   100.f,   Lambertian( FVec3( 0.8f, 0.8f, 0.f ) ) );
    world.Add<Sphere>( FVec3( 1.f, 0.f, -1.f ),       0.5f,    Metal( FVec3( 0.8f, 0.6f, 0.2f ), 0.3f ) );
    world.Add<Sphere>( FVec3( -1.f, 0.f, -1.f ),      0.5f,    Dielectric( 1.5f ) );
    world.Add<Sphere>( FVec3( -1.f, 0.f, -1.f ),     -0.45f,   Dielectric( 1.5f ) );*/

    /*F32 R = std::cos( Math::PI<F32> / 4.f );
    world.Add<Sphere>( FVec3( -R, 0.f, -1.f ), R, Lambertian( FVec3( 0.f, 0.f, 1.f ) ) );
    world.Add<Sphere>( FVec3( +R, 0.f, -1.f ), R, Lambertian( FVec3( 1.f, 0.f, 0.f ) ) );*/

    const FVec3 lookFrom( 13.f, 2.f, 3.f );
    const FVec3 lookAt( 0.f );
    const F32 aperture = 0.1f;
    const F32 distToFocus = 10.f; //( lookFrom - lookAt ).Length();
    Camera camera( lookFrom, lookAt, F32(width), F32(height), 20, aperture, distToFocus );

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
            Utils::ConsoleOutput( "Progress: %.2f%% - Computing pixel [%d, %d] = { %d, %d, %d }", percentage, j, i, rgb.R, rgb.G, rgb.G );
        }
    }

    if ( stbi_write_png( "output.png", width, height, 3, &data[0], 0 ) )
    {
        Utils::ConsoleOutput( "Image saved successfully!" );
    }

    return 0;
}