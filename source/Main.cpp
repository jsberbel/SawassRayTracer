#include <vector>

#include "Limits.h"
#include "World.h"
#include "Sphere.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

inline FVec3 TraceColor( const Ray& ray, const World& world )
{
    HitRecord record;
    if ( world.Hit(ray, 0.f, Limits<F32>::Max(), record) )
        return 0.5f * FVec3(record.Normal.X + 1.f, record.Normal.Y + 1.f, record.Normal.Z + 1.f);

    FVec3 unitDirection = ray.Direction.Unit();
    F32 t = 0.5f * (unitDirection.Y + 1.f);
    return Math::Blend( FVec3(1.f), FVec3(.5f, .7f, 1.f), t );
}

int main()
{
    constexpr U32 width  = 400;
    constexpr U32 height = 200;

    std::vector<RGB> data;
    data.reserve( width * height );

    constexpr FVec3 lowerLeftCorner( -2.f, -1.f, -1.f );
    constexpr FVec3 horizontal( 4.f, 0.f, 0.f );
    constexpr FVec3 vertical( 0.f, 2.f, 0.f );
    constexpr FVec3 origin( 0.f );

    World world;
    world.Add<Sphere>( FVec3(0.f, 0.f, -1.f), 0.5f );
    world.Add<Sphere>( FVec3(0.f, -100.5f, -1.f), 100.f );

    for ( S32 j = height - 1; j >= 0; --j )
    {
        for ( S32 i = 0; i < width; ++i )
        {
            F32 u = F32(i) / F32(width);
            F32 v = F32(j) / F32(height);
            Ray ray( origin, lowerLeftCorner + u * horizontal + v * vertical );

            FVec3 point = ray.PointAt(2.f);
            FVec3 color = TraceColor(ray, world);

            RGB rgb( Byte( 255.99 * color.R ), Byte( 255.99 * color.G ), Byte( 255.99 * color.B ) );
            data.push_back(rgb);
        }
    }

    if ( stbi_write_png( "output.png", width, height, 3, &data[0], 0 ) )
    {
        std::cout << "Image saved successfully!" << std::endl;
    }

    return 0;
}