#pragma once

#include "core/types.h"
#include "core/assert.h"

#include <filesystem>
namespace fs = std::filesystem;

using Path = fs::path;

namespace path
{
    inline Path create_dir(const std::string& _path)
    {
        const Path path(_path);
        const bool created = fs::create_directory(path);
        sws_assert(created);
        return path;
    }

    inline Path get_dir(const std::string& _path)
    {
        const Path path(_path);
        const bool exists = fs::exists(path);
        sws_assert(exists);
        return path;
    }

    inline Path get_dat_dir()
    {
        static Path data_dir = path::get_dir("dat");
        return data_dir;
    }

    inline Path get_out_dir()
    {
        static Path out_dir = path::create_dir("out");
        return out_dir;
    }

    inline Path get_log_dir()
    {
        static Path out_dir = path::create_dir("log");
        return out_dir;
    }
}