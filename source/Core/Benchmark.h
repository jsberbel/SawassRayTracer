#pragma once

#ifdef BENCHMARKING

#include <unordered_map>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#include <Core/Utils.h>

class Benchmark
{
    class ProfileGuard
    {
    public:
        inline ProfileGuard( const std::string& tag );
        ~ProfileGuard();

        constexpr ProfileGuard( const ProfileGuard& ) = delete;
        constexpr ProfileGuard& operator=( const ProfileGuard& ) = delete;
        constexpr ProfileGuard( ProfileGuard&& ) = default;

    private:
        const std::string m_Tag;
    };

    struct ProfileResult
    {
        std::string Tag;
        F64 TotalSeconds;
        F64 MSAverage;
    };

public:
    static inline void PushMark( const std::string& tag );
    static inline void PopMark( const std::string& tag );
    static inline ProfileGuard CreateGuard( const std::string& tag );

    static inline void OutputToIncrementalFile();
    static inline void OutputToFile( const std::string& fileName );

private:
    static inline std::vector<ProfileResult> ComputeResults();
    inline void Start();
    inline void End();

private:
    static inline std::unordered_map<std::string, Benchmark> g_Marks;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_LastStart;
    std::vector<U64> m_Durations;
};

inline void Benchmark::Start()
{
    m_LastStart = std::chrono::high_resolution_clock::now();
}

inline void Benchmark::End()
{
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - m_LastStart );
    m_Durations.emplace_back( duration.count() );
}

inline void Benchmark::PushMark( const std::string& tag )
{
    Benchmark::g_Marks[tag].Start();
}

inline void Benchmark::PopMark( const std::string& tag )
{
    Benchmark::g_Marks[tag].End();
}

inline Benchmark::ProfileGuard Benchmark::CreateGuard( const std::string& tag )
{
    return ProfileGuard( tag );
}

inline Benchmark::ProfileGuard::ProfileGuard( const std::string& tag ) 
    : m_Tag( tag )
{ 
    PushMark( m_Tag );
}

inline Benchmark::ProfileGuard::~ProfileGuard()
{
    PopMark( m_Tag );
}

inline std::vector<Benchmark::ProfileResult> Benchmark::ComputeResults()
{
    std::vector<ProfileResult> results;

    std::transform( Benchmark::g_Marks.begin(), Benchmark::g_Marks.end(), std::back_inserter(results), []( const auto& x )
    {
        const std::vector<U64>& durations = x.second.m_Durations;
        const F64 totalMS = static_cast<F64>(std::accumulate( durations.begin(), durations.end(), 0ull )) / 1000'000.;
        const F64 averageMS = totalMS / durations.size();
        return ProfileResult{ x.first, totalMS / 1000., averageMS };
    } );

    Benchmark::g_Marks.clear();

    std::sort( results.begin(), results.end(), []( const ProfileResult& a,  const ProfileResult& b )
    {
        return a.TotalSeconds > b.TotalSeconds;
    } );

    return results;
}

inline void Benchmark::OutputToIncrementalFile()
{
    OutputToFile( "benchmark_" + Utils::GetTimeOfDayIdentifier() );
}

inline void Benchmark::OutputToFile( const std::string& fileName )
{
    std::vector<ProfileResult> results = ComputeResults();

    if( !fs::exists( "benchmark" ) )
        fs::create_directory( "benchmark" );

    std::ofstream fileStream( "benchmark/" + fileName + ".txt", std::fstream::app );
    Assert( fileStream.good() && fileStream.is_open() );

    for( const ProfileResult& benchmark : results )
        fileStream << benchmark.Tag << " => " << benchmark.TotalSeconds << "s (avg: " << benchmark.MSAverage << "ms)" << std::endl;

    fileStream.close();
}

#define BENCHMARK_GUARD( tag ) const auto guard##tag = Benchmark::CreateGuard( #tag )
#define BENCHMARK_INCREMENTAL_LOG() Benchmark::OutputToIncrementalFile()
#define BENCHMARK_LOG( fileName ) Benchmark::OutputToFile( fileName )
#define BENCHMARK_BATCH_START( iterations ) for ( U32 _it_ = 0; _it_ < iterations; ++_it_ ) { BENCHMARK_GUARD( BENCHMARK_BATCH_##iterations )
#define BENCHMARK_BATCH_END_AND_LOG( fileName ) Utils::ConsoleOutput( "Iteration %d completed", _it_ + 1 ); } BENCHMARK_LOG( fileName )

#else

#define BENCHMARK_GUARD( tag ) ( void )0
#define BENCHMARK_INCREMENTAL_LOG() ( void )0
#define BENCHMARK_BATCH_END_AND_LOG() ( void )0

#endif // BENCHMARKING