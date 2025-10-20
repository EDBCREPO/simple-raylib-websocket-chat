#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/ws.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { namespace scene {

    void scene_0( ptr_t<Scene> self ) {

        struct NODE {
            string_t Message = string_t( 1024, '\0' );
            array_t<string_t> MessageList;
            ws_t client;
        };  ptr_t<NODE> obj = new NODE();

    /*.........................................................................*/

        ws::client( "ws://localhost:8000" ).onConnect([=]( ws_t cli ){
            
            console::log( "Connected" ); obj->client = cli;
            
            cli.onClose([=](){ rl::Close(); }); 

            cli.onData([=]( string_t data ){ 
                obj->MessageList.push( data ); 
            });

        });

    /*.........................................................................*/

        self->onDraw([=](){

            float w = GetRenderWidth(), h = GetRenderHeight(); ClearBackground( RAYWHITE );

            uint add = 0; forEach( x, obj->MessageList ){
                GuiLabel( { 2*w/100, 2*h/100+add, 95*w/100, 1*h/100 }, x.get() ); add += 3*h/100;
            }

            auto x = GuiTextBox( { 0, 95*h/100, 88*w/100, 5*h/100 }, obj->Message.get(), obj->Message.size(), true );

            if( GuiButton( { 88*w/100, 95*h/100, 12*w/100, 5*h/100 }, "Send" ) && obj->Message[0] != '\0' ){
            if( obj->client.is_available() ){ obj->client.write( obj->Message );  } obj->Message.fill( '\0' );
            }

        });

    /*.........................................................................*/

        self->onLoop([=]( float delta ){

            float w = GetRenderWidth(), h = GetRenderHeight();

            uint add = 0; forEach( x, obj->MessageList ){
            if( add >= 90*h/100 ){ obj->MessageList.shift(); }
                add += 3*h/100;
            }

        });

    }

}}

/*────────────────────────────────────────────────────────────────────────────*/