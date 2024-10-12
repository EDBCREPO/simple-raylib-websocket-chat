/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

#pragma once
#define RAYGUI_IMPLEMENTATION

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/encoder.h>
#include <nodepp/timer.h>
#include <nodepp/event.h>
#include <nodepp/json.h>
#include <nodepp/map.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl {
    #include <raylib/raylib.h>
    #include <raylib/raygui.h>
    #include <raylib/raymath.h>
}

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl {

    void RenderCanva( function_t<void> cb, RenderTexture2D& txt ){
        BeginTextureMode( txt ); cb(); EndTextureMode();
    }

    void RenderVR( function_t<void> cb, VrStereoConfig& vr ){
        BeginVrStereoMode( vr ); cb(); EndVrStereoMode();
    }

    void RenderScissor( function_t<void> cb, Rectangle& sc ){
        BeginScissorMode( sc.x, sc.y, sc.width, sc.height ); 
        cb(); EndScissorMode();
    }

    void RenderShader( function_t<void> cb, Shader& shd ){
        BeginShaderMode( shd ); cb(); EndShaderMode();
    }

    void Render3D( function_t<void> cb, Camera3D& cam ){
        BeginMode3D( cam ); cb(); EndMode3D();
    }

    void Render2D( function_t<void> cb, Camera2D& cam ){
        BeginMode2D( cam ); cb(); EndMode2D();
    }

    void RenderBlend( function_t<void> cb, int& mode ){
        BeginBlendMode( mode ); cb(); EndBlendMode();
    }

    void Render( function_t<void> cb ){
        BeginDrawing(); cb(); EndDrawing();
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { 

    event_t<>      onClose;
    event_t<>      onInit;
    event_t<>      onDraw;
    event_t<float> onLoop;

    void Close() { CloseWindow(); process::exit(1); }

    void Init( Vector2 size, uint fps, string_t title ) {

        InitWindow( size.x, size.y, title.get() ); 
        SetTargetFPS( fps ); process::add([=](){
        coStart
            while( !IsWindowReady() ){ coNext; } onInit.emit(); 
            while( !WindowShouldClose() ){ 
                onLoop.emit( GetFrameTime() ); BeginDrawing();
                onDraw.emit(); EndDrawing(); coNext;
            }   onClose.emit();
        coStop
        });

    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { class Item {
protected:

    struct NODE {
        bool    state = 0;
        object_t attr;
    }; ptr_t<NODE> obj;

public:

    event_t<>      onRemove;
    event_t<>      onDraw;
    event_t<float> onLoop;
    
    /*─······································································─*/

    template< class T, class... V >
    Item( T cb, V... args ) noexcept : obj( new NODE() ) {
        auto self = type::bind( this ); obj->state = 1;

        process::add([=](){ cb( self, args... ); return -1; });

        auto idl = rl::onLoop([=]( float delta ){ 
            if(!self->exists() || WindowShouldClose() )
              { self->free(); return; }
                self->onLoop.emit( delta );
        });

        auto idd = rl::onDraw([=](){ 
            if(!self->exists() || WindowShouldClose() )
              { return; } self->onDraw.emit(); 
        });

        auto idr = self->onRemove([=](){
            if( WindowShouldClose() ){ return; }
                rl::onDraw.off( idd );
                rl::onLoop.off( idl );
        });
        
    }

    Item() noexcept : obj( new NODE() ) {}

   ~Item(){ if( obj.count() > 1 ) { return; } free(); } 
    
    /*─······································································─*/

    void SetAttr( string_t name, object_t value ) const noexcept { 
        obj->attr[name] = value;
    }
    
    object_t GetAttr( string_t name ) const noexcept { 
        return obj->attr[name]; 
    }

    bool HasAttr( string_t name ) const noexcept {
        return obj->attr.has( name );
    }
    
    object_t GetAttr() const noexcept { 
        return obj->attr; 
    }

    /*─······································································─*/

    void free() const noexcept { close(); }

    void remove() const noexcept { close(); }

    bool exists() const noexcept { return obj->state != 0; }

    void close() const noexcept { if( !exists() ){ return; } obj->state = 0; onRemove.emit(); }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { class Scene {
protected:

    struct NODE {
        map_t<string_t,Item> items;
        object_t attr;
        bool state = 0;
    }; ptr_t<NODE> obj;

public:

    event_t<>      onRemove;
    event_t<>      onDraw;
    event_t<float> onLoop;
    
    /*─······································································─*/

    template< class T, class... V >
    Scene( T cb, V... args ) noexcept : obj( new NODE() ) {
        auto self = type::bind( this ); obj->state = 1;

        process::add([=](){ cb( self, args... ); return -1; });

        auto idl = rl::onLoop([=]( float delta ){ 
            if(!self->exists() || WindowShouldClose() )
              { self->free(); return; }
                self->onLoop.emit( delta );
        });

        auto idd = rl::onDraw([=](){ 
            if(!self->exists() || WindowShouldClose() )
              { return; } self->onDraw.emit(); 
        });

        auto idr = self->onRemove([=](){
            if( WindowShouldClose() ){ return; }
                rl::onDraw.off( idd ); 
                rl::onLoop.off( idl );
        });

    }

    Scene() noexcept : obj( new NODE() ){}

   ~Scene(){ if( obj.count() > 1 ) { return; } free(); } 
    
    /*─······································································─*/

    void SetAttr( string_t name, object_t value ) const noexcept { 
        obj->attr[name] = value;
    }
    
    object_t GetAttr( string_t name ) const noexcept { 
        return obj->attr[name]; 
    }

    bool HasAttr( string_t name ) const noexcept {
        return obj->attr.has( name );
    }
    
    object_t GetAttr() const noexcept { 
        return obj->attr; 
    }
    
    /*─······································································─*/

    template< class T, class... V >
    Item& AppendItem( string_t name, T cb, V... args ) const noexcept {
        auto item = Item( cb, args... ); if( name == nullptr )
        { name.resize(sizeof(item)); memcpy( name.get(), &item, sizeof(item) ); }
          obj->items[name] = item; return obj->items[name];
    }

    void RemoveItem( string_t name ) const noexcept {
        obj->items[name].close();
    }

    Item& GetItem( string_t name ) const noexcept {
        return obj->items[name];
    }

    bool HasItem( string_t name ) const noexcept {
        return obj->items.has( name );
    }

    void RemoveItem() const noexcept {
        for( auto &x: obj->items.data() )
           { x.second.close(); }
    }

    /*─······································································─*/

    void free() const noexcept { close(); }

    void remove() const noexcept { close(); }

    bool exists() const noexcept { return obj->state != 0; }

    /*─······································································─*/

    void close() const noexcept {
        if( !exists() ){ return; } obj->state = 0; 
        onRemove.emit(); RemoveItem();
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { 
    
    ptr_t<Scene> Room = new Scene();

    void RemoveScene(){ Room->close(); }

    Scene& GetScene() { return *Room; }

    void SetScene( Scene& Scene ) { *Room = Scene; }

    template< class T, class... V >
    Scene& AppendScene( T cb, V... args ){ 
        RemoveScene(); Room = type::bind( Scene( cb, args... ) );
        return *Room;
    }

}