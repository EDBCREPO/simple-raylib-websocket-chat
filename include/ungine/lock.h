/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_LOCKER
#define UNGINE_LOCKER

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class lock_t : public global_t {
protected:

    struct NODE { uint value = 0; }; ptr_t<NODE> obj;

public:

    /*----*/ lock_t() noexcept : global_t(), obj( new NODE() ){}
    virtual ~lock_t() noexcept { /*--------------*/ }

    void      lock() const noexcept { obj->value -= 1; }
    void    unlock() const noexcept { obj->value += 1; }

    bool is_locked() const noexcept { return obj->value != 0; }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
