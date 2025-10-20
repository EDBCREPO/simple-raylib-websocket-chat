/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_IMAGE
#define UNGINE_IMAGE

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class image_t : public global_t {
protected:

    struct NODE { rl::Image img; }; ptr_t<NODE> obj;

public:

    /*----*/ image_t() noexcept : global_t(), obj( new NODE() ) { /*--*/ }
    virtual ~image_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    /*─······································································─*/

    image_t( rl::Image image ) noexcept : global_t(), obj( new NODE() ) { obj->img = image; }

    template< class T >
    image_t( T& file, string_t ext ) noexcept : global_t(), obj( new NODE() ) {
        auto data = stream::await( file ); /*-------------------------------------------*/
        obj->img = rl::LoadImageFromMemory( ext.get(), (uchar*) data.get(), data.size() );
    }

    image_t( const rl::Texture2D texture ) noexcept : global_t(), obj( new NODE() ) {
        obj->img = rl::LoadImageFromTexture( texture );
    }

    image_t( string_t path ) noexcept : global_t(), obj( new NODE() ) {
        obj->img = rl::LoadImage( path.get() );
    }

    /*─······································································─*/

    void set_format( uint format ) const noexcept { rl::ImageFormat( &obj->img, format ); }

    image_t copy() const noexcept { return image_t( ImageCopy( obj->img ) ); }

    ptr_t<uchar> raw_image( string_t ext ) const noexcept {
        if( !is_valid() ){ return nullptr; } int size = 0;
        auto bff = rl::ExportImageToMemory( obj->img, ".png", &size );
        return ptr_t<uchar>( bff, type::cast<ulong>( size ) );
    }

    bool save_image( string_t path ) const noexcept {
        if( !is_valid() ) { return false; }
        return rl::ExportImage( obj->img, path.get() );
    }

    rl::Vector2 size() const noexcept {
        if( !is_valid() ){ return rl::Vector2({ 0, 0 }); }
        /*--------------*/ return { 
            type::cast<float>( obj->img.width  ), 
            type::cast<float>( obj->img.height ) 
        };
    }

    /*─······································································─*/

    bool is_valid() const noexcept { return rl::IsImageValid( obj->img ); }

    rl::Image* operator->() const noexcept { return &get(); }

    rl::Image& get() const noexcept { return obj->img; }

    /*─······································································─*/

    void free() const noexcept {
         if( !is_valid() ){ return; } rl::UnloadImage( obj->img );
    }


};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
