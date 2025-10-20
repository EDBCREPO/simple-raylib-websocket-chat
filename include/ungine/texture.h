/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_TEXTURE
#define UNGINE_TEXTURE

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class texture_t : public global_t {
protected:

    struct NODE { rl::Texture2D txt; }; ptr_t<NODE> obj;

public:

    /*----*/ texture_t() noexcept : global_t(), obj( new NODE() ){ /*---*/ }
    virtual ~texture_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    /*─······································································─*/

    texture_t( rl::Texture2D texture ) noexcept : global_t(), obj( new NODE() ) { obj->txt = texture; }

    template< class T >
    texture_t( T& file, string_t ext ) noexcept : global_t(), obj( new NODE() ) {
        auto data = stream::await( file ); /*--------------------------------------------*/
        auto img  = rl::LoadImageFromMemory( ext.get(), (uchar*) data.get(), data.size() );
        obj->txt  = rl::LoadTextureFromImage( img ); rl::UnloadImage( img );
    }

    texture_t( rl::Image image ) noexcept : global_t(), obj( new NODE() ) {
        obj->txt = rl::LoadTextureFromImage( image );
    }

    texture_t( string_t path ) noexcept : global_t(), obj( new NODE() ) {
        obj->txt = rl::LoadTexture( path.get() );
    }

    /*─······································································─*/

    rl::Vector2 size() const noexcept {
        if( !is_valid() ){ return rl::Vector2({ 0, 0 }); }
        /*--------------*/ return { 
            type::cast<float>( obj->txt.width  ), 
            type::cast<float>( obj->txt.height )
        };
    }

    void set_filter( uint filter ) const noexcept {
        rl::SetTextureFilter( obj->txt, filter );
    }

    /*─······································································─*/

    bool is_valid() const noexcept { return rl::IsTextureValid( obj->txt ); }

    rl::Texture* operator->() const noexcept { return &get(); }

    rl::Texture& get() const noexcept { return obj->txt; }

    /*─······································································─*/

    void draw( transform_2D_t pos, rect_t src ) const noexcept {
        rl::DrawTexturePro( obj->txt, src, rect_t({
                pos.translate.position.x, pos.translate.position.y,
                pos.translate.scale.x,    pos.translate.scale.y
            }), vec2_t({0,0}), pos.translate.rotation * RAD2DEG, rl::WHITE 
        );
    }

    void draw( transform_2D_t pos ) const noexcept {
        rl::DrawTexturePro( obj->txt, rect_t({
                0, 0, type::cast<float>( obj->txt.width  ), 
                /*-*/ type::cast<float>( obj->txt.height )
            }), rect_t({
                pos.translate.position.x, pos.translate.position.y,
                pos.translate.scale.x,    pos.translate.scale.y
            }), vec2_t({0,0}), pos.translate.rotation * RAD2DEG, rl::WHITE 
        );
    }

    /*─······································································─*/

    void draw( rect_t pos, rect_t src, float angle=0 ) const noexcept {
        rl::DrawTexturePro( obj->txt, src, rect_t({
                pos.x, pos.y, pos.width, pos.height
            }), vec2_t({0,0}), angle*RAD2DEG, rl::WHITE 
        );
    }

    void draw( rect_t pos, float angle=0 ) const noexcept {
        rl::DrawTexturePro( obj->txt, rect_t({
                0, 0, type::cast<float>( obj->txt.width  ), 
                /*-*/ type::cast<float>( obj->txt.height )
            }), rect_t({
                pos.x    , pos.y,
                pos.width, pos.height
            }), vec2_t({0,0}), angle*RAD2DEG, rl::WHITE 
        );
    }

    /*─······································································─*/

    void free() const noexcept {
         if( !is_valid() ){ return; } rl::UnloadTexture( obj->txt );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
