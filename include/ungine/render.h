/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_RENDER
#define UNGINE_RENDER

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class render_t : public global_t {
protected:

    struct NODE { rl::RenderTexture txt; }; ptr_t<NODE> obj;

public:

    /*─······································································─*/

    /*----*/ render_t() noexcept : global_t(), obj( new NODE() ){ /*---*/ }
    virtual ~render_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    /*─······································································─*/

    render_t( rl::RenderTexture texture ) noexcept : global_t(), obj( new NODE() ){
        obj->txt = texture;
    }

    render_t( int width, int height ) noexcept : global_t(), obj( new NODE() ){
        obj->txt = rl::LoadRenderTexture( width, height );
    }

    /*─······································································─*/

    bool is_valid() const noexcept { return rl::IsRenderTextureValid( obj->txt ); }

    rl::RenderTexture* operator->() const noexcept { return &get(); }

    rl::RenderTexture& get() const noexcept { return obj->txt; }

    /*─······································································─*/

    rl::Vector2 size() const noexcept {
        if( !is_valid() ){ return rl::Vector2({ 0, 0 }); }
        /*--------------*/ return { 
            type::cast<float>( obj->txt.texture.width  ), 
            type::cast<float>( obj->txt.texture.height ) 
        };
    }

    rl::Texture& get_texture() const noexcept { return obj->txt.texture; }

    rl::Texture& get_depth  () const noexcept { return obj->txt.depth; }

    /*─······································································─*/

    void draw( transform_2D_t pos, rect_t src ) const noexcept {
        rl::DrawTexturePro( 
            obj->txt.texture, src, rect_t({
                pos.translate.position.x, pos.translate.position.y,
                pos.translate.scale.x,    pos.translate.scale.y
            }), vec2_t({0,0}), pos.translate.rotation * RAD2DEG, rl::WHITE 
        );
    }

    void draw( transform_2D_t pos ) const noexcept {
        rl::DrawTexturePro( 
            obj->txt.texture, rect_t({
                0, 0, type::cast<float>( obj->txt.texture.width  ), 
                /*-*/ type::cast<float>( obj->txt.texture.height )
            }), rect_t({
                pos.translate.position.x, pos.translate.position.y,
                pos.translate.scale.x,    pos.translate.scale.y
            }), vec2_t({0,0}), pos.translate.rotation * RAD2DEG, rl::WHITE 
        );
    }

    /*─······································································─*/

    void emit( function_t<void> cb ) const noexcept {
         rl::BeginTextureMode( obj->txt ); cb();
         rl::EndTextureMode  (); /*-----------*/
    }

    /*─······································································─*/

    vec2_t get_size() const noexcept { return vec2_t({ 
        type::cast<float>( obj->txt.texture.width  ), 
        type::cast<float>( obj->txt.texture.height ) 
    }); }

    void set_size( int width, int height ) const noexcept {
        if( obj->txt.texture.width==width && obj->txt.texture.height==height )
          { return; } free(); obj->txt = rl::LoadRenderTexture( width,height );
    }

    /*─······································································─*/

    void free() const noexcept {
         if( !is_valid() ){ return; } rl::UnloadRenderTexture( obj->txt );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace render {

    void emit_scissor( rect_t scissor, function_t<void> callback ) {
         rl::BeginScissorMode( scissor.x, scissor.y, scissor.width, scissor.height );
         callback(); rl::EndScissorMode(); /*-------------------------------------*/
    }

    void emit_render( render_2D_t* render, function_t<void> callback ) {
         if( render == nullptr ){ return; }
         rl::BeginTextureMode( *render ); callback();
         rl::EndTextureMode  (); /*----------------*/
    }

    void emit_shader( shader_t* shader, function_t<void> callback ) {
         if( shader == nullptr ){ return; }
         rl::BeginShaderMode( shader->get() ); callback();
         rl::EndShaderMode  ();
    }

    void emit_blend( uint blend_mode, function_t<void> callback ) {
         rl::BeginBlendMode( blend_mode ); callback();
         rl::EndBlendMode  (); /*-------------------*/
    }

    void emit_2D( camera_2D_t* cam, function_t<void> callback ) {
         if( cam == nullptr ){ return; }
         rl::BeginMode2D( *cam ); callback();
         rl::EndMode2D  (); /*-------------*/
    }

    void emit_3D( camera_3D_t* cam, function_t<void> callback ) {
         if( cam == nullptr ){ return; }
         rl::BeginMode3D( *cam ); callback();
         rl::EndMode3D  (); /*-------------*/
    }

    void emit_vr( vr_t* device, function_t<void> callback ) {
         if( device == nullptr ){ return; }
         rl::BeginVrStereoMode( device->device() ); callback();
         rl::EndVrStereoMode  (); /*-------------------------*/
    }

    void emit( function_t<void> callback ) {
         rl::BeginDrawing(); callback();
         rl::EndDrawing  (); /*-------*/
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
