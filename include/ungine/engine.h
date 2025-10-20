/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_ENGINE
#define UNGINE_ENGINE

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace engine {

    event_t<>      onExit ;
    event_t<>      onOpen ;
    event_t<>      onNext ;
    event_t<float> onLoop ;
    event_t<>      onClose;
    event_t<>      onDraw ;
    global_t       global ;
    lock_t         locker ;

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace engine {

    bool is_ready() /*---*/ { return rl::IsWindowReady() || !locker.is_locked(); }

    bool  should_close() { return rl::WindowShouldClose(); }

    float get_delta() /*--*/ { return rl::GetFrameTime(); }

    void  set_fps( int fps ) { rl::SetTargetFPS( fps ); }

    int   get_fps() /*----*/ { return rl::GetFPS(); }

    void close() {
        static bool b=0; if( b ){ return; } b=1;
        onClose.emit(); rl::CloseWindow(); 
        onExit.emit (); process::exit(1);
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace engine {

    void start( int width, int height, string_t title ) {

        rl::InitWindow( width, height, title.get() );
        process::onSIGEXIT([](){ close(); });

    // rl::SetExitKey(0);

        process::add( coroutine::add( COROUTINE(){
        coBegin ; coWait( !is_ready() );

            onOpen.emit(); while( !should_close() ){

                coWait/*-*/( !is_ready() );
                onLoop.emit( get_delta() );
                onDraw.emit( /*-------*/ );

            coNext; } close();

        coFinish
        }));

        process::add( coroutine::add( COROUTINE(){
        coBegin ; while( is_ready() ) {

            coWait/*-*/( !is_ready() );
            onNext.emit( /*-------*/ );

        coDelay(10); } coFinish
        }));

    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
