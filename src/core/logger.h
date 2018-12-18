#pragma once

#include "core/utils.h"

#include <fstream>

namespace logr
{
    enum class LogTag
    {
        Info,
        Warning,
        Error
    };

    inline void write(LogTag _tag, const std::string& _msg)
    {
        static std::ofstream stream(path::get_log_dir() / ("swsrt_" + util::get_time_of_day() + ".log"), std::fstream::app);
        switch (_tag)
        {
            case LogTag::Info:    stream << "[INF] "; break;
            case LogTag::Warning: stream << "[WAR] "; break;
            case LogTag::Error:   stream << "[ERR] "; break;
        }
        stream << _msg << std::endl;
    }
}

#ifndef SWS_DISABLE_CHECKS
    #define _sws_fmt_msg(msg)         (util::str_fmt("[%s at %s] %s", __FILENAME__, __LINE__, msg))
    #define sws_log(msg)              (logr::write(logr::LogTag::Info, _sws_fmt_msg(msg))
    #ifdef _DEBUG
        #define sws_verify(exp)       sws_assert(exp)
        #define sws_verify2(exp, msg) sws_assert(exp)
    #else
        #define sws_verify(exp)       if (!(exp)) { logr::write(logr::LogTag::Error, _sws_fmt_msg("Unsatisfied expression: "#exp)); }
        #define sws_verify2(exp, msg) if (!(exp)) { logr::write(logr::LogTag::Error, _sws_fmt_msg(msg)); }
    #endif
#else
    #define sws_log(msg)              ( (void)0 )
    #define sws_verify(exp)           ( (void)0 )
    #define sws_verify2(exp)          ( (void)0 )
#endif