/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_INPUT
#define UNGINE_INPUT

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace cursor { enum MODE {

    MODE_CURSOR_DEFAULT       = 0, // Default pointer shape
    MODE_CURSOR_ARROW         = 1, // Arrow shape
    MODE_CURSOR_IBEAM         = 2, // Text writing cursor shape
    MODE_CURSOR_CROSSHAIR     = 3, // Cross shape
    MODE_CURSOR_POINTING_HAND = 4, // Pointing hand cursor
    MODE_CURSOR_RESIZE_EW     = 5, // Horizontal resize/move arrow shape
    MODE_CURSOR_RESIZE_NS     = 6, // Vertical resize/move arrow shape
    MODE_CURSOR_RESIZE_NWSE   = 7, // Top-left to bottom-right diagonal resize/move arrow shape
    MODE_CURSOR_RESIZE_NESW   = 8, // The top-right to bottom-left diagonal resize/move arrow shape
    MODE_CURSOR_RESIZE_ALL    = 9, // The omnidirectional resize/move cursor shape
    MODE_CURSOR_NOT_ALLOWED   = 10 // The operation-not-allowed shape

};}}

namespace ungine { namespace cursor {

    inline void show()                  { rl::ShowCursor(); }

    inline void hide()                  { rl::HideCursor(); }

    inline void unlock()                { rl::EnableCursor(); }

    inline void lock()                  { rl::DisableCursor(); }

    inline void set_mode( uint mode )   { rl::SetMouseCursor( mode ); }

    inline bool is_hidden()             { return rl::IsCursorHidden(); }

    inline bool is_on_screen()          { return rl::IsCursorOnScreen(); }

    inline void set_scale( vec2_t pos ) { rl::SetMouseScale( pos.x, pos.y ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace mouse {

    inline bool   is_released( int button )  { return rl::IsMouseButtonReleased( button ); }

    inline bool   is_pressed( int button )   { return rl::IsMouseButtonPressed( button ); }

    inline bool   is_down( int button )      { return rl::IsMouseButtonDown( button ); }

    inline bool   is_up( int button )        { return rl::IsMouseButtonUp( button ); }

    inline vec2_t get_delta()                { return rl::GetMouseDelta(); }

    inline vec2_t get_position()             { return rl::GetMousePosition(); }

    inline vec2_t get_scroll()               { return rl::GetMouseWheelMoveV(); }

    inline void   set_offset  ( vec2_t pos ) { rl::SetMouseOffset( pos.x, pos.y ); }

    inline void   set_position( vec2_t pos ) { rl::SetMousePosition( pos.x, pos.y ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace key {

    inline bool is_released( int key ){ return rl::IsKeyReleased( key ); }

    inline bool is_pressed( int key ) { return rl::IsKeyPressed( key ); }

    inline void set_exit( int key )   { return rl::SetExitKey( key ); }

    inline bool is_down( int key )    { return rl::IsKeyDown( key ); }

    inline bool is_up( int key )      { return rl::IsKeyUp( key ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace touch {

    inline ptr_t<vec2_t> get_position() { queue_t <vec2_t> touch;

        for( auto x = rl::GetTouchPointCount(); x--; ){
             auto y = rl::GetTouchPointId( x ); /**/
             touch.push( rl::GetTouchPosition( y ) );
        }

        return touch.data();
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace gamepad {
// FIXME:
}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
