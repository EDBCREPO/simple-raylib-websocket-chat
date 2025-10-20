#pragma once

#include <nodepp/nodepp.h>
#include <express/http.h>
#include <nodepp/path.h>
#include <nodepp/fs.h>
#include <nodepp/ws.h>

namespace rl { namespace game {

    queue_t<ws_t> list; void server() {

        auto app = express::http::add();
        auto srv = app.listen( "localhost", 8000, [=]( socket_t ){
            console::log("server started at http://localhost:8000");
        }); ws::server( srv );

        app.USE( express::http::file( "./www" ) );
        srv.onConnect( [=]( ws_t cli ){

            list.push( cli ); auto ID = list.last();

            cli.onData([=]( string_t data ){
                forEach( x, list.data() ){ x.write( data ); }
            });

            cli.onClose([=](){
                console::log( "Disconnected" );
                list.erase( ID );
            });

            console::log( "Connected" );

        });

    }

}}