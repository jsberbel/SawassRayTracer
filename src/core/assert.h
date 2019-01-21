// ======================================================================
// File: assert.h
// Revision: 1.0
// Creation: 05/21/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#if defined(_DEBUG) || !defined(SWS_ASSERTIONS_DISABLED)
    #ifdef _WIN32
        #include <intrin.h>
        #define sws_assert(exp) if (!(exp)) { __debugbreak(); }
    #else
        #define sws_assert(exp) if (!(exp)) { *(int*)0 = 0; }
    #endif
#else
    #define sws_assert(exp) ( (void)0 )
#endif
