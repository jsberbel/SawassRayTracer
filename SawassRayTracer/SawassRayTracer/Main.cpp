#include <vector>

#include "Ray.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

F32 HitSphere( const FVec3& center, F32 radius, const Ray& ray )
{
    const FVec3 oc = ray.GetOrigin() - center;
    const F32 a = FVec3::Dot( ray.GetDirection(), ray.GetDirection() );
    const F32 b = 2.f * FVec3::Dot( oc, ray.GetDirection() );
    const F32 c = FVec3::Dot( oc, oc ) - radius * radius;
    const F32 discriminant = b*b - 4*a*c;

    return (discriminant < 0) ? (-1.f) : ((-b - std::sqrt(discriminant)) / (2.f * a));
}

FVec3 TraceColor( const Ray& ray )
{
    F32 t = HitSphere( FVec3(0.f, 0.f, -1.f), 0.5f, ray );
    if (t > 0.f)
    {
        FVec3 normal = ( ray.GetPointAt(t) - FVec3(0.f, 0.f, -1.f) ).GetUnit();
        return 0.5f * FVec3(normal.X + 1.f, normal.Y + 1.f, normal.Z + 1.f);
    }

    FVec3 unitDirection = ray.GetDirection().GetUnit();
    t = 0.5f * (unitDirection.Y + 1.f);
    return (1.f - t) * FVec3(1.f) + t * FVec3(.5f, .7f, 1.f);
}

int main()
{
    constexpr U32 width  = 400;
    constexpr U32 height = 200;

    std::vector<BVec3> data;
    data.reserve( width * height );

    constexpr FVec3 lowerLeftCorner( -2.f, -1.f, -1.f );
    constexpr FVec3 horizontal( 4.f, 0.f, 0.f );
    constexpr FVec3 vertical( 0.f, 2.f, 0.f );
    constexpr FVec3 origin( 0.f );

    for ( S32 j = height - 1; j >= 0; --j )
    {
        for ( S32 i = 0; i < width; ++i )
        {
            F32 u = F32(i) / F32(width);
            F32 v = F32(j) / F32(height);
            Ray ray( origin, lowerLeftCorner + u * horizontal + v * vertical );
            FVec3 fCol = TraceColor(ray);
            BVec3 iCol( Byte( 255.99 * fCol.R ), Byte( 255.99 * fCol.G ), Byte( 255.99 * fCol.B ) );
            data.push_back(iCol);
        }
    }

    if ( stbi_write_png( "output.png", width, height, 3, &data[0], 0 ) )
    {
        std::cout << "Image saved successfully!" << std::endl;
    }

    return 0;
}