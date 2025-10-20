/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

 /*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_WINDOW
#define UNGINE_WINDOW

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace window {

    inline void set_position( ivec2_t pos )    { rl::SetWindowPosition( pos.x, pos.y ); }

    inline void set_icon( image_t image )      { rl::SetWindowIcon( image.get() ); }

    inline void set_title( string_t name )     { rl::SetWindowTitle( name.get() ); }

    inline vec2_t get_position()               { return rl::GetWindowPosition(); }

    inline void focus()                        { rl::SetWindowFocused(); }

    inline void restore()                      { rl::RestoreWindow(); }

    inline void set_opacity( float opacity )   { rl::SetWindowOpacity( opacity ); }

    inline void set_size( ivec2_t size )       { rl::SetWindowSize( size.x, size.y ); }

    inline void set_min_size( ivec2_t size )   { rl::SetWindowMinSize( size.x, size.y ); }

    inline void set_max_size( ivec2_t size )   { rl::SetWindowMaxSize( size.x, size.y ); }

    inline ivec2_t get_size() { return ivec2_t({ rl::GetRenderWidth(), rl::GetRenderHeight() }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace window {
    
    inline vec2_t normalized( vec2_t pos ) { return pos * vec2_t({ 
        type::cast<float>( rl::GetRenderWidth () ), 
        type::cast<float>( rl::GetRenderHeight() ) 
    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace monitor {

    inline ivec2_t get_size( int idx ) { return ivec2_t({ rl::GetMonitorWidth(idx), rl::GetMonitorHeight(idx) }); }

    inline int get_index()                   { return rl::GetCurrentMonitor(); }

    inline int count()                       { return rl::GetMonitorCount(); }

    inline void set( int idx )               { rl::SetWindowMonitor( idx ); }

    inline vec2_t get_scale_DPI()            { return rl::GetWindowScaleDPI(); }

    inline string_t get_name( int idx )      { return rl::GetMonitorName( idx ); }

    inline vec2_t get_position( int idx )    { return rl::GetMonitorPosition( idx ); }

    inline int get_refresh_rate( int idx )   { return rl::GetMonitorRefreshRate( idx ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace window { namespace mode {

    inline void set  ( uint flags ){ rl::SetWindowState( flags ); }

    inline void clear( uint flags ){ rl::ClearWindowState( flags ); }

    inline uint check( uint flags ){ return rl::IsWindowState( flags ); }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace window {

    inline bool is_resized()     { return rl::IsWindowResized(); }

    inline bool is_focused()     { return rl::IsWindowFocused(); }

    inline bool is_hidden()      { return mode::check( FLAGS::WINDOW_HIDDEN ); }

    inline bool is_maximized()   { return mode::check( FLAGS::WINDOW_MAXIMIZED ); }

    inline bool is_minimized()   { return mode::check( FLAGS::WINDOW_MINIMIZED ); }

    inline bool is_fullscreen()  { return mode::check( FLAGS::WINDOW_FULLSCREEN ); }

    inline bool is_undecorated() { return mode::check( FLAGS::WINDOW_UNDECORATED ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace clipboard {

    void set( string_t value ) { rl::SetClipboardText( value.get() ); }

    string_t get() /*-------*/ { return rl::GetClipboardText(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
