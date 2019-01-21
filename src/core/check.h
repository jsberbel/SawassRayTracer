#pragma once

#include "core/utils.h"

#include <fstream>

namespace check
{
    enum class LogTag
    {
        Info,
        Warning,
        Error
    };

    template<LogTag _tag>
    inline void log(const std::string& _msg)
    {
        static std::ofstream stream(path::get_log_dir() / (util::k_project_prefix + util::get_time_of_day() + ".log"), std::fstream::app);
        if constexpr (_tag == LogTag::Info)     stream << "[INF] ";
        if constexpr (_tag == LogTag::Warning)  stream << "[WRN] ";
        if constexpr (_tag == LogTag::Error)    stream << "[ERR] ";
        stream << _msg << std::endl;
    }
}

#ifndef SWS_CHECKS_DISABLED
    #define _sws_fmt_msg(msg)         (util::str_fmt("[%s at %s] %s", __FILENAME__, __LINE__, msg))
    #define sws_log(msg)              (check::write<check::LogTag::Info>(_sws_fmt_msg(msg))
    #ifndef SWS_ASSERTIONS_DISABLED
        #define sws_verify(exp)       sws_assert(exp)
        #define sws_verify2(exp, msg) sws_assert(exp)
    #else
        #define sws_verify(exp)       if (!(exp)) { check::write<check::LogTag::Error>(_sws_fmt_msg("Unsatisfied expression: "#exp)); }
        #define sws_verify2(exp, msg) if (!(exp)) { check::write<check::LogTag::Error>(_sws_fmt_msg(msg)); }
    #endif
#else
    #define sws_log(msg)              ( (void)0 )
    #define sws_verify(exp)           ( (void)0 )
    #define sws_verify2(exp)          ( (void)0 )
#endif