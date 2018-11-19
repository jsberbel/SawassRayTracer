// ======================================================================
// File: assert.h
// Revision: 1.0
// Creation: 05/21/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#ifdef _DEBUG
#define sws_assert(expression) if (!(expression)) { *(int*)0 = 0; }
#else
#define sws_assert(expression) ( (void) 0 )
#endif
