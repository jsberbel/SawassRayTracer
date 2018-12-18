#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

using u8    = unsigned char;
using u16   = unsigned short;
using u32   = unsigned int;
using u64   = unsigned long long;

using s8    = signed char;
using i16   = signed short;
using s32   = signed int;
using s64   = signed long long;

using f32   = float;
using f64   = double;

using uchar = unsigned char;
using utf8  = char;
using utf16 = char16_t;
using utf32 = char32_t;

using byte  = u8;

using uptr  = std::uintptr_t;
using sptr  = std::intptr_t;
using usize = std::size_t;

//#define SWS_FLOAT_AS_DOUBLE
#ifdef SWS_FLOAT_AS_DOUBLE
using fx    = f64;
#else
using fx    = f32;
#endif

using ix    = s32;

#define sws_u32_from_ptr(ptr) ((u32)(usize)(ptr))
#define sws_ptr_from_u32(type, value) ((type *)((usize)(value)))

#define sws_offset_of(type, member) ((uptr)&(((type *)0)->member))
