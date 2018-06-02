#include <vector>

#include <Core/Utils.h>
#include <Core/Limits.h>
#include <Core/Benchmark.h>

#include <Scene/World.h>
#include <Scene/Geometry/Sphere.h>
#include <Scene/Material/Lambertian.h>
#include <Scene/Material/Metal.h>
#include <Scene/Material/Dielectric.h>

#include <Engine/Camera.h>

inline FVec3 GenerateColor( const Ray& ray, const World& world, S32 depth )
{
    if( HitRecord hitRecord; world.Hit( ray, 0.001f, Limits<F32>::Max(), &hitRecord ) )
    {
        Ray scattered; FVec3 attenuation;
        if( depth < 50 && hitRecord.Material->Scatter( ray, hitRecord, &attenuation, &scattered ) )
            return attenuation * GenerateColor( scattered, world, depth + 1 );

        return FVec3( 0.f );
    }

    const FVec3 unitDirection = ray.Direction.Normalize();
    const F32 t = 0.5f * ( unitDirection.Y + 1.f );
    return Math::Blend( FVec3(1.f), FVec3(.5f, .7f, 1.f), t );
}

inline FVec3 GammaCorrection( const FVec3& color )
{
    // Formula: Vout = A * pow( Vin, y ) with A = 1 and y = 1/2
    return FVec3( std::sqrt(color.X), std::sqrt(color.Y), std::sqrt(color.Z) );
}

inline World GenerateRandomSpheresWorld()
{
    World world;
    world.Add<StaticSphere>( FVec3( 0.f, -1000.f, 0.f ), 1000.f, Lambertian( FVec3( 0.5f ) ) );

    for( S32 a = -10; a < 10; ++a )
    {
        for( S32 b = -10; b < 10; ++b )
        {
            F32 chooseMat = Utils::Random01();
            FVec3 center( a + 0.9f*Utils::Random01(), 0.2f, b + 0.9f*Utils::Random01() );

            if( ( center - FVec3(4.f, 0.2f, 0.f) ).Length() > 0.9f )
            {
                if( chooseMat < 0.8f ) // Diffuse
                {
                    auto RandomQuadraticUnit = []() constexpr { return Utils::Random01() * Utils::Random01(); };
                    world.Add<DynamicSphere>( center, center + FVec3( 0.f, 0.5f*Utils::Random01(), 0.f ), 0.f, 1.f, 0.2f,
                                              Lambertian( FVec3( RandomQuadraticUnit(), RandomQuadraticUnit(), RandomQuadraticUnit() ) ) );
                }
                else if( chooseMat < 0.95f ) // Metal
                {
                    auto RandomSemiUnit = []() constexpr { return 0.5f*( 1.f + Utils::Random01() ); };
                    world.Add<StaticSphere>( center, 0.2f, Metal( FVec3( RandomSemiUnit(), RandomSemiUnit(), RandomSemiUnit() ), 0.5f*Utils::Random01() ) );
                }
                else // Glass
                {
                    world.Add<StaticSphere>( center, 0.2f, Dielectric( 1.5f ) );
                }
            }
        }
    }

    world.Add<StaticSphere>( FVec3(0.f, 1.f, 0.f),  1.f, Dielectric( 1.5f ) );
    world.Add<StaticSphere>( FVec3(-4.f, 1.f, 0.f), 1.f, Lambertian( FVec3(0.4f, 0.2f, 0.1f) ) );
    world.Add<StaticSphere>( FVec3(4.f, 1.f, 0.f),  1.f, Metal( FVec3(0.7f, 0.6f, 0.5f), 0.f ) );

    return world;
}

int main()
{
    BENCHMARK_BATCH_START( 1 );

    constexpr U32 width  = 800;
    constexpr U32 height = 600;
    constexpr F32 resolution = width*height;
    constexpr U32 numSamples = 30;

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

    constexpr FVec3 lookFrom( 13.f, 2.f, 3.f );
    constexpr FVec3 lookAt( 0.f );
    constexpr F32 verticalFOV = 20.f;
    constexpr F32 aperture    = 0.f;
    constexpr F32 startTime   = 0.f;
    constexpr F32 endTime     = 1.f;
    Camera camera( lookFrom, lookAt, width, height, verticalFOV, aperture, startTime, endTime );

    //world.SortByDistance( camera );

    for( S32 j = height - 1; j >= 0; --j )
    {
        for( S32 i = 0; i < width; ++i )
        {
            FVec3 color( 0.f );
            for( S32 s = 0; s < numSamples; ++s )
            {
                const F32 u       = static_cast<F32>( i + Utils::Random01() ) / static_cast<F32>( width );
                const F32 v       = static_cast<F32>( j + Utils::Random01() ) / static_cast<F32>( height );
                const Ray ray     = camera.TraceRay( u, v );
                const FVec3 point = ray.PointAt( 2.f );
                color += GenerateColor( ray, world, 0 );
            }
            color /= F32( numSamples );
            color = FVec3( std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]) );

            RGB rgb( Byte( 255.99 * color.X ), Byte( 255.99 * color.Y ), Byte( 255.99 * color.Z ) );
            data.push_back(rgb);
        }

        const F32 percentage = ( static_cast<F32>( height - j ) / height ) * 100.f;
        Utils::ConsoleOutput( "%.2f%% completed", percentage );
    }

    //Utils::OutputImageToIncrementalFile( width, height, data );
    Utils::OutputImageToFile( "test", width, height, data );

    BENCHMARK_BATCH_END_AND_LOG( "test" );

    return 0;
}