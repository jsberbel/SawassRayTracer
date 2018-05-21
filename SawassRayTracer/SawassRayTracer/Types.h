#pragma once

using U8    = unsigned char;
using U16   = unsigned short;
using U32   = unsigned int;
using U64   = unsigned long long;
            
using S8    = signed char;
using S16   = signed short;
using S32   = signed int;
using S64   = signed long long;
            
using F32   = float;
using F64   = double;

using UTF8  = char;
using UTF16 = char16_t;
using UTF32 = char32_t;

using Byte  = unsigned char;
using Any   = void*;

#ifdef _WIN64
using Size = unsigned __int64;
#else
using Size = unsigned int;
#endif