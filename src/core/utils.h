#pragma once

#include "core/math/v3.h"

#include <random>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
namespace fs = std::filesystem;

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define NON_COPYABLE( CLS ) \
    public: \
    constexpr CLS (const CLS &) noexcept            = delete; \
    constexpr CLS (CLS &&) noexcept                 = delete; \
    constexpr CLS & operator=(const CLS &) noexcept = delete; \
    constexpr CLS & operator=(CLS &&) noexcept      = delete \

#define MOVABLE_ONLY( CLS ) \
    public: \
    constexpr CLS (const CLS &) noexcept            = delete; \
    constexpr CLS (CLS &&) noexcept                 = default; \
    constexpr CLS & operator=(const CLS &) noexcept = delete; \
    constexpr CLS & operator=(CLS &&) noexcept      = default \

namespace util
{
    template <class T, ENABLE_IF(IS_POINTER(T))>
    constexpr void safe_del(T* _ptr)
    {
        if (_ptr != nullptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
    }

    template <class T, ENABLE_IF(IS_REAL(T) || IS_INT(T))>
    constexpr T rnd(T _beg, T _end)
    {
        static std::random_device rnd_device;
        static std::mt19937_64 mt_engine(rnd_device());

        if constexpr(IS_REAL(T))
        {
            std::uniform_real_distribution<T> distrib(_beg, _end);
            return distrib(mt_engine);
        }
        else if constexpr(IS_INT(T))
        {
            std::uniform_int_distribution<T> distrib(_beg, _end);
            return distrib(mt_engine);
        }
        return T(0);
    }

    constexpr f32 rnd_01()
    {
        return rnd(0.f, 1.f);
    }
    
    constexpr fv3 rnd_unit_fv3()
    {
        return fv3(rnd_01(), rnd_01(), rnd_01());
    }
    
    constexpr fv3 rnd_point_in_unit_sphere()
    {
        fv3 point;
        do point = 2.f * fv3(rnd_01(), rnd_01(), 0.f) - fv3(1.f, 1.f, 0.f);
        while (math::dot(point, point) >= 1.f);
        return point;
    }
    
    constexpr fv3 rnd_point_in_unit_disk()
    {
        fv3 point;
        do point = 2.f * rnd_unit_fv3() - fv3(1.f);
        while (point.get_sqrlength() >= 1.f);
        return point;
    }

    inline std::string get_time_of_day()
    {
        const std::time_t now = std::time(nullptr);
        std::tm local_buf = {};
        const errno_t err = gmtime_s(&local_buf, &now);
        sws_assert(err == 0);
        std::stringstream ss;
        ss << std::put_time(&local_buf, "%y%m%d_%H%M%S");
        return ss.str();
    }

    inline std::string str_fmt(const utf8* const _fmt, ...)
    {
        sws_assert(_fmt);

        static constexpr utf8 end_line_chars[]{ '\r', '\n', '\0' };
        static constexpr u32 out_buf_sz = 2048u;
        utf8 out_buf[out_buf_sz];

        utf8* p = out_buf;
        {
            va_list args;
            va_start(args, _fmt);
            const s32 n = _vsnprintf_s(out_buf, out_buf_sz - sizeof(end_line_chars) + 1,
                                       out_buf_sz - sizeof(end_line_chars), _fmt, args);
            va_end(args);
            sws_assert(n >= 0);

            p += static_cast<size_t>(n);
        }

        for (const auto& c : end_line_chars)
            *p++ = c;

        return out_buf;
    }

    template <class... Args>
    constexpr void output_to_console(const std::string& _fmt, Args&&... _args)
    {
        printf((_fmt + "\n").c_str(), std::forward<Args>(_args)...);
    }

    inline b32 output_img_to_file(const std::string& _filename, u32 _width, u32 _height, const std::vector<rgb>& _data)
    {
        if (!fs::exists("output"))
            fs::create_directory("output");

        const std::string filepath = "output/" + _filename + ".png";
        return stbi_write_png(filepath.c_str(), _width, _height, 3, &_data[0], 0);
    }

    inline b32 output_img_to_incremental_file(u32 _width, u32 _height, const std::vector<rgb>& _data)
    {
        return output_img_to_file(get_time_of_day(), _width, _height, _data);
    }
}