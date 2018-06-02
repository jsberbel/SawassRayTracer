#pragma once

// SFINAE Template Conditions
#define ENABLE_IF(...)              class = std::enable_if<__VA_ARGS__>
#define IS_REAL(NUM)                std::is_floating_point<NUM>::value
#define IS_INT(NUM)                 std::is_integral<NUM>::value
#define IS_CLASS(C)                 std::is_class<C>::value
#define IS_ENUM(E)                  std::is_enum<E>::value
#define IS_FUNCTION(F)              std::is_function<F>::value
#define IS_REFERENCE(R)             std::is_reference<R>::value
#define IS_POINTER(P)               std::is_pointer<P>::value
#define IS_ARRAY(A)                 std::is_array<A>::value
#define IS_BASE_OF(BASE, DERIVED)   std::is_base_of<BASE, DERIVED>::value
#define IS_SAME(A, B)               std::is_same<A, B>::value
#define ARE_SAME(T, Ts)             std::conjunction_v<std::is_same<T, Ts>...>
#define IS_ANY_OF(T, ...)           Framework::Utils::IsAnyOfTrait<T, __VA_ARGS__>::value
#define CAN_CAST(FROM, TO)          std::is_convertible<FROM, TO>::value
#define IF(CONDITION, THEN, ELSE)   std::conditional<CONDITION, THEN, ELSE>::type
#define ADD_ALL(Ts)                 Framework::Utils::AddAllTrait< sizeof(Ts)... >::value

#include <random>
#include <iostream>
#include <tuple>
#include <sstream>
#include <chrono>
#include <fstream>
#include <unordered_map>
#include <numeric>
#include <iomanip>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
namespace fs = std::filesystem;

#include <Core/Vec3.h>
#include <Core/Math.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Utils
{
    template <class Type, ENABLE_IF( IS_POINTER( Type ) ) >
    constexpr void SafeDelete( Type& ptr )
    {
        if( ptr != nullptr )
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    template <class Type, ENABLE_IF( IS_REAL( Type ) || IS_INT( Type ) )>
    constexpr Type Random( Type begin, Type end )
    {
        static std::random_device g_RandomDevice;
        static std::mt19937_64 g_MTEngine( g_RandomDevice() );

        if constexpr( IS_REAL( Type ) )
        {
            std::uniform_real_distribution<Type> distribution( begin, end );
            return distribution( g_MTEngine );
        }
        else if constexpr( IS_INT( Type ) )
        {
            std::uniform_int_distribution<Type> distribution( begin, end );
            return distribution( g_MTEngine );
        }
    }

    constexpr F32 Random01()
    {
        return Random( 0.f, 1.f );
    }

    constexpr FVec3 RandomUnitFVec3()
    {
        return FVec3( Random01(), Random01(), Random01() );
    }

    constexpr FVec3 RandomPointInUnitSphere()
    {
        FVec3 point;
        do point = 2.f * FVec3( Utils::Random01(), Utils::Random01(), 0.f) - FVec3( 1.f, 1.f, 0.f );
        while( Math::Dot( point, point ) >= 1.f );
        return point;
    }

    constexpr FVec3 RandomPointInUnitDisk()
    {
        FVec3 point;
        do point = 2.f * Utils::RandomUnitFVec3() - FVec3( 1.f );
        while( point.SquaredLength() >= 1.f );
        return point;
    }

    template <class... Args>
    constexpr void ConsoleOutput( const std::string& format, Args&&... args )
    {
        printf( ( format + "\n" ).c_str(), std::forward<Args>( args )... );
    }

    inline std::string StringFormat( const char* const format, ...)
    {
        Assert( format );

        static constexpr char endOfLineChars[]{ '\r', '\n', '\0' };
        static constexpr U32 outBufferSize = 2048u;
        char outBuffer[outBufferSize];

        char* p = outBuffer;
        {
            va_list args;
            va_start( args, format );
            const int n = _vsnprintf_s( outBuffer, outBufferSize - sizeof( endOfLineChars ) + 1,
                                        outBufferSize - sizeof( endOfLineChars ), format, args );
            va_end( args );
            Assert( n >= 0 );

            p += (size_t)n;
        }

        for( const auto& character : endOfLineChars )
            *p++ = character;

        return outBuffer;
    }

    inline std::string GetTimeOfDayIdentifier()
    {
        std::time_t now = std::time( nullptr );
        std::tm localBuffer;
        errno_t result = gmtime_s( &localBuffer, &now );
        Assert( result == 0 );
        std::stringstream ss;
        ss << std::put_time( &localBuffer, "%y%m%d_%H%M%S" );
        return ss.str();
    }

    inline void OutputImageToFile( const std::string& fileName, U32 width, U32 height, const std::vector<RGB>& data )
    {
        if( !fs::exists( "output" ) )
            fs::create_directory( "output" );

        std::string filePath = "output/" + fileName + ".png";
        bool result = stbi_write_png( filePath.c_str(), width, height, 3, &data[0], 0 );
        Assert( result );
    }

    inline void OutputImageToIncrementalFile( U32 width, U32 height, const std::vector<RGB>& data )
    {
        Utils::OutputImageToFile( Utils::GetTimeOfDayIdentifier(), width, height, data );
    }
}