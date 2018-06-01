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

    class BenchmarkGuard
    {
        friend inline BenchmarkGuard CreateBenchmarkGuard( const std::string& tag );

    public:
        inline BenchmarkGuard( const std::string& tag );
        inline ~BenchmarkGuard();

        BenchmarkGuard( const BenchmarkGuard& ) = delete;
        BenchmarkGuard( BenchmarkGuard&& ) = default;

    private:
        const std::string m_Tag;
    };

    class Benchmark
    {
        friend inline void PushBenchmark( const std::string& tag );
        friend inline void PopBenchmark( const std::string& tag );
        friend inline void OutputBenchmarksToFile();

    private:
        inline void Start();
        inline void End();

        static inline std::unordered_map<std::string, Benchmark> g_Benchmarks;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastStart;
        std::vector<U64> m_Milliseconds;
    };

    inline void Benchmark::Start()
    {
        m_LastStart = std::chrono::high_resolution_clock::now();
    }

    inline void Benchmark::End()
    {
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now() - m_LastStart );
        m_Milliseconds.emplace_back( duration.count() );
    }

    inline BenchmarkGuard CreateBenchmarkGuard( const std::string& tag )
    {
        return BenchmarkGuard( tag );
    }

    inline void PushBenchmark( const std::string& tag )
    {
        Benchmark::g_Benchmarks[tag].Start();
    }

    inline void PopBenchmark( const std::string& tag )
    {
        Benchmark::g_Benchmarks[tag].End();
    }

    inline BenchmarkGuard::BenchmarkGuard( const std::string& tag ) 
        : m_Tag( tag )
    { 
        PushBenchmark( m_Tag );
    }

    inline BenchmarkGuard::~BenchmarkGuard()
    {
        PopBenchmark( m_Tag );
    }

    inline void OutputBenchmarksToFile()
    {
        struct BenchmarkResult
        {
            std::string Tag;
            F64 TotalSeconds;
            F64 MSAverage;
        };
        std::vector<BenchmarkResult> results;

        std::transform( Benchmark::g_Benchmarks.begin(), Benchmark::g_Benchmarks.end(), std::back_inserter(results), []( const auto& x )
        {
            const std::vector<U64>& milliseconds = x.second.m_Milliseconds;
            const F64 totalMS = static_cast<F64>(std::accumulate( milliseconds.begin(), milliseconds.end(), 0ull ));
            const F64 averageMS = totalMS / milliseconds.size();
            return BenchmarkResult{ x.first, totalMS / 1000., averageMS };
        } );

        std::sort( results.begin(), results.end(), []( const BenchmarkResult& a,  const BenchmarkResult& b )
        {
            return a.TotalSeconds > b.TotalSeconds;
        } );

        if( !fs::exists( "benchmark" ) )
            fs::create_directory( "benchmark" );

        std::ofstream fileStream( "benchmark/benchmark_" + GetTimeOfDayIdentifier() + ".txt" );
        Assert( fileStream.good() && fileStream.is_open() );

        for( const BenchmarkResult& benchmark : results )
            fileStream << benchmark.Tag << " => " << benchmark.TotalSeconds << "s (avg: " << benchmark.MSAverage << "ms)" << std::endl;

        fileStream.close();
    }

    inline void OutputImageToFile( U32 width, U32 height, const std::vector<RGB>& data )
    {
        if( !fs::exists( "output" ) )
            fs::create_directory( "output" );

        std::string filePath = "output/output_" + GetTimeOfDayIdentifier() + ".png";
        bool result = stbi_write_png( filePath.c_str(), width, height, 3, &data[0], 0 );
        Assert( result );
    }
}