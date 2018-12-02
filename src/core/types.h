#pragma once

#include <cstdint>
#include <cstddef>

using u8    = unsigned char;
using u16   = unsigned short;
using u32   = unsigned int;
using u64   = unsigned long long;
            
using s8    = signed char;
using s16   = signed short;
using s32   = signed int;
using s64   = signed long long;
            
using f32   = float;
using f64   = double;

using b32   = s32;

using utf8  = char;
using utf16 = char16_t;
using utf32 = char32_t;

using uptr  = std::uintptr_t;
using sptr  = std::intptr_t;

using std::size_t;
using usz = size_t;

#define U32_From_PTR(PTR) ((u32)(size_t)(PTR))
#define PTR_From_U32(TYPE, VALUE) ((TYPE *)((size_t)VALUE))

#define Offset_Of(TYPE, MEMBER) ((uptr)&(((TYPE *)0)->MEMBER))