/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_ENV
#define NODEPP_ENV

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL_ == NODEPP_KERNEL_WINDOWS
    #include "file.h"
    #include "windows/env.cpp"
#elif _KERNEL_ == NODEPP_KERNEL_POSIX
    #include "file.h"
    #include "posix/env.cpp"
#else
    #error "This OS Does not support env.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif