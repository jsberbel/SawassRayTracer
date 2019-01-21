// ======================================================================
// File: profiler.h
// Revision: 1.0
// Creation: 06/01/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#ifdef SWS_PROFILE

#include "core/utils.h"

#include <unordered_map>
#include <fstream>
#include <utility>
#include <numeric>
#include <filesystem>
namespace fs = std::filesystem;

namespace prof
{
    struct ProfMark
    {
        inline void start();
        inline void end();

        std::chrono::time_point<std::chrono::high_resolution_clock> last_start;
        std::vector<u64> durations;
    };

    class ProfGuard
    {
        MOVABLE_ONLY(ProfGuard);
    public:
        inline ProfGuard(const std::string& _tag);
        ~ProfGuard();
    private:
        std::string m_tag;
    };

    struct ProfResult
    {
        std::string tag     = "";
        f64 total_seconds   = 0.;
        f64 avg_seconds     = 0.;
    };
    
    static inline void push_mark(const std::string& _tag);
    static inline void pop_mark(const std::string& _tag);
    static inline ProfGuard create_guard(const std::string& _tag);

    static inline void output_to_incremental_file();
    static inline void output_to_file(const std::string& _filename);

    static inline std::vector<ProfResult> compute_results();

    static inline std::unordered_map<std::string, ProfMark> g_marks;

    // -----------------------------------------------------------------

    // ProfMark

    inline void ProfMark::start()
    {
        last_start = std::chrono::high_resolution_clock::now();
    }

    inline void ProfMark::end()
    {
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last_start);
        durations.emplace_back(duration.count());
    }

    // ProfGuard

    inline ProfGuard::ProfGuard(const std::string& _tag)
        : m_tag(_tag)
    {
        push_mark(m_tag);
    }

    inline ProfGuard::~ProfGuard()
    {
        pop_mark(m_tag);
    }

    // prof methods

    inline void push_mark(const std::string& _tag)
    {
        g_marks[_tag].start();
    }

    inline void pop_mark(const std::string& _tag)
    {
        g_marks[_tag].end();
    }

    inline ProfGuard create_guard(const std::string& _tag)
    {
        return ProfGuard(_tag);
    }

    inline std::vector<ProfResult> compute_results()
    {
        std::vector<ProfResult> results;

        std::transform(g_marks.begin(), g_marks.end(), std::back_inserter(results), [](const auto& x)
        {
            const std::vector<u64>& durations = x.second.durations;
            const f64 total_ms = static_cast<f64>(std::accumulate(durations.begin(), durations.end(), 0ull)) / 1000'000.;
            const f64 average_ms = total_ms / durations.size();
            return ProfResult { x.first, total_ms / 1000., average_ms / 1000. };
        });

        g_marks.clear();

        std::sort(results.begin(), results.end(), [](const ProfResult& a, const ProfResult& b)
        {
            return a.total_seconds > b.total_seconds;
        });

        return results;
    }

    inline void output_to_incremental_file()
    {
        output_to_file(util::k_project_prefix + util::get_time_of_day());
    }

    inline void output_to_file(const std::string& _filename)
    {
        std::vector<ProfResult> results = compute_results();

        std::ofstream file_stream(path::get_log_path() / (_filename + ".txt"), std::fstream::app);
        sws_assert( file_stream.good() && file_stream.is_open() );

        for (const ProfResult& pr : results)
        {
            file_stream << util::str_fmt("%s => %fs (avg: %fs)",
                                         pr.tag.c_str(),
                                         pr.total_seconds,
                                         pr.avg_seconds);
        }

        file_stream.close();
    }
};

#define PROFILER_GUARD(TAG)                   const auto guard##tag = prof::create_guard(#TAG)
#define PROFILER_INCREMENTAL_LOG()            prof::output_to_incremental_file()
#define PROFILER_LOG(FILENAME)                prof::output_to_file(FILENAME)
#define PROFILER_BATCH_START(ITERATIONS)      for (u32 _it_ = 0; _it_ < (ITERATIONS); ++_it_) { PROFILER_GUARD(PROFILER_BATCH_##ITERATIONS)
#define PROFILER_BATCH_END_AND_LOG(FILENAME)  util::output_to_console("Iteration %d completed", _it_ + 1); } PROFILER_LOG(FILENAME)

#else

#define PROFILER_GUARD(TAG)                   (void)0
#define PROFILER_INCREMENTAL_LOG()            (void)0
#define PROFILER_LOG(FILENAME)                (void)0
#define PROFILER_BATCH_START(ITERATIONS)      (void)0
#define PROFILER_BATCH_END_AND_LOG(FILENAME)  (void)0

#endif // PROFILING
