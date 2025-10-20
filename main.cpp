#include <nodepp/nodepp.h>

using namespace nodepp;

#include "assets/script/game.cpp"
#include "assets/script/server.cpp"
#include "assets/scenes/scene_0.cpp"

void onMain() {

    if( process::env::get("mode") == "server" ){
        
        rl::game::server();

    } else {

        rl::Init( { 300, 600 }, 60, "Raylib Chat" );

        rl::AppendScene( rl::scene::scene_0 );

        rl::onClose([](){
            console::log("Closed");
            rl::Close();
        });

    }

}