#pragma once

#ifdef NDEBUG
#define Assert(expression) ( (void) 0 )
#else
#define Assert(expression) ( (void) !!(expression) )
#endif