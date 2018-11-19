// ======================================================================
// File: profiler.h
// Revision: 1.0
// Creation: 06/01/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#ifdef PROFILING

#include "core/utils.h"

#include <unordered_map>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

class Profiler
{
    class ProfileGuard
    {
    public:
        inline ProfileGuard(const std::string& _tag);
        ~ProfileGuard();

        constexpr ProfileGuard(const ProfileGuard&) = delete;
        constexpr ProfileGuard& operator=(const ProfileGuard&) = delete;
        constexpr ProfileGuard(ProfileGuard&&) = default;
        constexpr ProfileGuard& operator=(ProfileGuard&&) = default;

    private:
        const std::string m_tag;
    };

    struct ProfileResult
    {
        std::string tag;
        f64 total_seconds;
        f64 ms_average;
    };
    
public:
    static inline void push_mark(const std::string& _tag);
    static inline void pop_mark(const std::string& _tag);
    static inline ProfileGuard create_guard(const std::string& _tag);

    static inline void output_to_incremental_file();
    static inline void output_to_file(const std::string& _filename);

private:
    static inline std::vector<ProfileResult> compute_results();
    inline void start();
    inline void end();

private:
    static inline std::unordered_map<std::string, Profiler> g_marks;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_start;
    std::vector<u64> m_durations;
};

inline void Profiler::start()
{
    m_last_start = std::chrono::high_resolution_clock::now();
}

inline void Profiler::end()
{
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_last_start);
    m_durations.emplace_back(duration.count());
}

inline void Profiler::push_mark(const std::string& _tag)
{
    Profiler::g_marks[_tag].start();
}

inline void Profiler::pop_mark(const std::string& _tag)
{
    Profiler::g_marks[_tag].end();
}

inline Profiler::ProfileGuard Profiler::create_guard(const std::string& _tag)
{
    return ProfileGuard(_tag);
}

inline Profiler::ProfileGuard::ProfileGuard(const std::string& _tag)
    : m_tag(_tag)
{
    push_mark(m_tag);
}

inline Profiler::ProfileGuard::~ProfileGuard()
{
    pop_mark(m_tag);
}

inline std::vector<Profiler::ProfileResult> Profiler::compute_results()
{
    std::vector<ProfileResult> results;

    std::transform(Profiler::g_marks.begin(), Profiler::g_marks.end(), std::back_inserter(results), [](const auto& x)
    {
        const std::vector<u64>& durations = x.second.m_durations;
        const f64 total_ms = static_cast<f64>(std::accumulate(durations.begin(), durations.end(), 0ull)) / 1000'000.;
        const f64 average_ms = total_ms / durations.size();
        return ProfileResult { x.first, total_ms / 1000., average_ms };
    });

    Profiler::g_marks.clear();

    std::sort(results.begin(), results.end(), [](const ProfileResult& a, const ProfileResult& b)
    {
        return a.total_seconds > b.total_seconds;
    });

    return results;
}

inline void Profiler::output_to_incremental_file()
{
    output_to_file("Profiler_" + utils::get_time_of_day());
}

inline void Profiler::output_to_file(const std::string& _filename)
{
    std::vector<ProfileResult> results = compute_results();

    if (!fs::exists("Profiler"))
        fs::create_directory("Profiler");

    std::ofstream file_stream("Profiler/" + _filename + ".txt", std::fstream::app);
    sws_assert( file_stream.good() && file_stream.is_open() );

    for (const ProfileResult& pr : results)
        file_stream << pr.tag << " => " << pr.total_seconds << "s (avg: " << pr.ms_average << "ms)" << std::endl;

    file_stream.close();
}

#define PROFILER_GUARD( tag ) const auto guard##tag = Profiler::create_guard( #tag )
#define PROFILER_INCREMENTAL_LOG() Profiler::output_to_incremental_file()
#define PROFILER_LOG( filename ) Profiler::output_to_file( filename )
#define PROFILER_BATCH_START( iterations ) for ( u32 _it_ = 0; _it_ < iterations; ++_it_ ) { PROFILER_GUARD( PROFILER_BATCH_##iterations )
#define PROFILER_BATCH_END_AND_LOG( filename ) utils::output_to_console( "Iteration %d completed", _it_ + 1 ); } PROFILER_LOG( filename )

#else

#define PROFILER_GUARD( tag ) ( void )0
#define PROFILER_INCREMENTAL_LOG() ( void )0
#define PROFILER_BATCH_END_AND_LOG() ( void )0

#endif // PROFILING
