#include <vector>

#include "Utils.h"
#include "Limits.h"
#include "World.h"
#include "Sphere.h"
#include "Camera.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

inline FVec3 GenerateColor( const Ray& ray, const World& world )
{
    HitRecord record;
    if ( world.Hit(ray, 0.f, Limits<F32>::Max(), record) )
        return 0.5f * FVec3(record.Normal.X + 1.f, record.Normal.Y + 1.f, record.Normal.Z + 1.f);

    FVec3 unitDirection = ray.Direction.Normalize();
    F32 t = 0.5f * (unitDirection.Y + 1.f);
    return Math::Blend( FVec3(1.f), FVec3(.5f, .7f, 1.f), t );
}

int main()
{
    constexpr U32 width  = 200;
    constexpr U32 height = 100;
    constexpr F32 resolution = width*height;
    constexpr U32 numSamples = 100;

    std::vector<RGB> data;
    data.reserve( width * height );

    World world;
    world.Add<Sphere>( FVec3(0.f, 0.f, -1.f), 0.5f );
    world.Add<Sphere>( FVec3(0.f, -100.5f, -1.f), 100.f );

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
                color += GenerateColor( ray, world );
            }
            color /= F32( numSamples );

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