/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_SCENE
#define UNGINE_SCENE

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node { 
node_t node_render( function_t<void,ref_t<node_t>> clb ) {
return node_t([=]( ref_t<node_t> self ){

    self->set_attribute( "viewport", viewport_t() );
    self->set_attribute( "type"    , "Render" );

    auto rnd = type::bind( render_t() ); self->onDraw([=](){

        auto siz = window::get_size(); rnd->set_size( siz.x, siz.y );
        auto vpt = self->get_viewport(); if(vpt==nullptr){ return; }
             vpt->render = type::bind( &rnd->get() );
             
        auto que = self->get_render_queue();

    render::emit_render( &vpt->render, [&](){
        rl::ClearBackground( vpt->background );

        if( !que->event3D.empty() ){
        if( !vpt->camera3D.null() ){
        render::emit_3D( &vpt->camera3D, [&](){

            auto x=que->event3D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); }}

        if( !que->event2D.empty() ){ 
        if( !vpt->camera2D.null() ){
        render::emit_2D( &vpt->camera2D, [&](){

            auto x=que->event2D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); }}
        
        if( !que->eventUI.empty() ){

            auto x=que->eventUI.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }

    }); });

clb( self ); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node { 
node_t node_vr_render( function_t<void,ref_t<node_t>> clb ) {
return node_t([=]( ref_t<node_t> self ){

    self->set_attribute( "viewport", viewport_t() );
    self->set_attribute( "type"    , "Render" );
    self->set_attribute( "vr"      , vr_t() );

    auto rnd = type::bind( render_t() ); self->onDraw([=](){

        auto siz = window::get_size(); rnd->set_size( siz.x, siz.y );
        auto vpt = self->get_viewport(); if(vpt==nullptr){ return; }
             vpt->render = type::bind( &rnd->get() ); 
             
        auto vr  = self->get_attribute<vr_t>( "vr" );
        auto que = self->get_render_queue();

    render::emit_render( &vpt->render, [&](){ vr->emit([&](){
        rl::ClearBackground( vpt->background );

        if( !que->event3D.empty() ){
        if( !vpt->camera3D.null() ){
        render::emit_3D( &vpt->camera3D, [&](){

            auto x=que->event3D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); }}

        if( !que->event2D.empty() ){ 
        if( !vpt->camera2D.null() ){
        render::emit_2D( &vpt->camera2D, [&](){

            auto x=que->event2D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); }}
        
        if( !que->eventUI.empty() ){

            auto x=que->eventUI.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }

    }); }); });

clb( self ); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node {
node_t node_scene( function_t<void,ref_t<node_t>> clb ) {
return node_t([=]( ref_t<node_t> self ){

    self->set_attribute( "viewport", viewport_t() );
    self->set_attribute( "type"    , "Render" );

    auto rnd = type::bind( render_t() ); self->onDraw([=](){

        auto siz = window::get_size(); rnd->set_size( siz.x, siz.y );
        auto vpt = self->get_viewport(); if(vpt==nullptr){ return; }
             vpt->render = type::bind( &rnd->get() ); 
             
        auto que = self->get_render_queue();

    render::emit_render( &vpt->render, [&](){
        rl::ClearBackground( vpt->background );

        if( !que->event3D.empty() ){
        if( !vpt->camera3D.null() ){
        render::emit_3D( &vpt->camera3D, [&](){

            auto x=que->event3D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); }}

        if( !que->event2D.empty() ){ 
        if( !vpt->camera2D.null() ){
        render::emit_2D( &vpt->camera2D, [&](){

            auto x=que->event2D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); } else {

            auto x=que->event2D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }}
        
        if( !que->eventUI.empty() ){ 

            auto x=que->eventUI.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }

    });

    render::emit( [&](){ if( rnd.null() ){ return; }
        
        auto txt = rnd->get().texture; 
        auto src = rect_t({ 0, 0, type::cast<float>( txt.width ), type::cast<float>(-txt.height ) });
        auto dst = rect_t({ 0, 0, type::cast<float>( txt.width ), type::cast<float>( txt.height ) });

        rl::DrawTexturePro ( txt, src, dst, vec2_t({ 0, 0 }), .0f, rl::WHITE );

    }); });

clb( self ); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node {
node_t node_vr_scene( function_t<void,ref_t<node_t>> clb ) {
return node_t([=]( ref_t<node_t> self ){

    self->set_attribute( "viewport", viewport_t() );
    self->set_attribute( "type"    , "Render" );
    self->set_attribute( "vr"      , vr_t() );

    auto rnd = type::bind( render_t() ); self->onDraw([=](){

        auto siz = window::get_size(); rnd->set_size( siz.x, siz.y );
        auto vpt = self->get_viewport(); if(vpt==nullptr){ return; }
             vpt->render = type::bind( &rnd->get() ); 
             
        auto vr  = self->get_attribute<vr_t>( "vr" );
        auto que = self->get_render_queue();

    render::emit_render( &vpt->render, [&](){ vr->emit([&](){
        rl::ClearBackground( vpt->background );

        if( !que->event3D.empty() ){
        if( !vpt->camera3D.null() ){
        render::emit_3D( &vpt->camera3D, [&](){

            auto x=que->event3D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); }}

        if( !que->event2D.empty() ){ 
        if( !vpt->camera2D.null() ){
        render::emit_2D( &vpt->camera2D, [&](){

            auto x=que->event2D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }); } else {

            auto x=que->event2D.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }
        }}
        
        if( !que->eventUI.empty() ){ 

            auto x=que->eventUI.first(); while( x!=nullptr ){
            auto y=x->next; x->data.emit(); x=y; }

        }

    });});

    render::emit( [&](){ if( rnd.null() ){ return; }
        
        auto txt = rnd->get().texture; 
        auto src = rect_t({ 0, 0, type::cast<float>( txt.width ), type::cast<float>(-txt.height ) });
        auto dst = rect_t({ 0, 0, type::cast<float>( txt.width ), type::cast<float>( txt.height ) });

        rl::DrawTexturePro ( txt, src, dst, vec2_t({ 0, 0 }), .0f, rl::WHITE );

    }); });

clb( self ); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
