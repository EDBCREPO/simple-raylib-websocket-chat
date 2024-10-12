#pragma once

#include <nodepp/nodepp.h>
#include <nodepp/path.h>
#include <nodepp/fs.h>
#include <nodepp/ws.h>

namespace rl { namespace game {

    queue_t<ws_t> list;
    
    tcp_t server() {

        auto server = http::server([=]( http_t cli ){ 

            string_t dir = "www/index.html";

            if( cli.path != "/" )
              { dir = path::join( "www", cli.path ); }

            if( !fs::exists_file(dir) ){
                cli.write_header( 404, header_t({ 
                    { "content-type", "text/plain" }
                }) );
                cli.write("Oops: 404 Error"); 
                cli.close(); return;
            }

            auto str = fs::readable( dir );

            cli.write_header( 200, header_t({
                { "Content-Length", string::to_string(str.size()) },
            //  { "Cache-Control", "public, max-age=3600" },
                { "Content-Type",   path::mimetype(dir) }
            }));

            stream::pipe( str, cli );

        });

        ws::server( server );

        server.onConnect( [=]( ws_t cli ){

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

        server.listen( "localhost", 8000, [=]( socket_t server ){
            console::log("server started at http://localhost:8000");
        });

        return server;

    }

}}