#pragma once

#ifdef NDEBUG
#define Assert(expression) ( (void) 0 )
#else
#define Assert(expression) if (!(condition)) { *(int*)0 = 0; }
#endif
