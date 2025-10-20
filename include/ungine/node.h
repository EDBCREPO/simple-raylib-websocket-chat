/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_NODE
#define UNGINE_NODE

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node { struct NODE_TREE {
   void *node=nullptr, *parent=nullptr;
   map_t<string_t,NODE_TREE> node_list;
}; }}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class node_t : public global_t {
public:

    event_t<node_t*> onCollision;
    event_t<>        onNext  ;
    event_t<>        onOpen  ;
    event_t<>        onUIDraw;
    event_t<>        on2DDraw;
    event_t<>        on3DDraw;
    event_t<>        onDraw  ;
    event_t<float>   onLoop  ;
    event_t<>        onClose ;

protected:

    using DONE = node::NODE_TREE;

    struct NODE { DONE node ;
        void* evdr=nullptr, *evlp=nullptr;
        void* evcl=nullptr, *evnx=nullptr;
        bool state=false  ; /*----------*/
    };  ptr_t<NODE> obj;

    static void node_iterator( function_t<void,node_t*> cb, node_t* root, bool deep ) {
        
        if( root == nullptr ){ return; } node_t* node = root;
        if( deep ){ cb( type::cast<node_t>( node->obj->node.node ) ); }

        auto x = node->obj->node.node_list.raw().first(); while( x!=nullptr ){
        auto y = x->next; if( x->data.second.node==nullptr ){ goto NEXT; }
        if( deep ){ node_iterator( cb, (node_t*)( x->data.second.node ), deep ); }
        else /**/ { cb( type::cast<node_t>( x->data.second.node ) ); }
             
        NEXT:; x=y; }

    }

    void remove_from_parent() const noexcept {
         node_t* prnt=nullptr; if( (prnt=get_parent())==nullptr ){ return; }
         prnt->remove_child( *get_attribute<string_t>("name") );
    }

    ptr_t<node_t> init_() {
        auto self = type::bind( this );
        self->set_attribute( "name", "root" ); 
        self->obj->node.node=&self; return self;
    }

    void free_() const noexcept {

        if ( !exists() ) /*---------*/ { return; } obj->state= false; 
        for( auto x: get_children()   ){ x->obj->node.parent=nullptr; }
        if ( !process::should_close() ){ remove_from_parent(); }
        
        onCollision.clear(); onNext  .clear();
        on2DDraw   .clear(); onUIDraw.clear();
        onOpen     .clear(); on3DDraw.clear();
        onLoop     .clear(); onClose .emit ();
        
        if( !process::should_close() ){ 
            engine::onNext .off( obj->evnx );
            engine::onLoop .off( obj->evlp );
            engine::onDraw .off( obj->evdr );
            engine::onClose.off( obj->evcl );
        }

    }

public:

    virtual ~node_t() noexcept { if( obj.count()>1 ){ return; }free_(); }

    /*----*/ node_t() noexcept : global_t(), obj( new NODE ) { init_(); }

    /*─······································································─*/

    node_t( function_t<void,ref_t<node_t>> cb ) noexcept : global_t(), obj( new NODE ) {
        engine::locker.lock(); auto self = init_(); self->obj->state = true;

        obj->evcl = engine::onClose([=](){ self->free_(); });

        obj->evlp = engine::onLoop([=]( float delta ){
            if( !self->exists() ){ return; }
                 self->onLoop.emit( delta );
        });

        obj->evnx = engine::onNext([=]( /**/ ){
            if( !self->exists() ){ return; }
                 self->onNext.emit();
        });

        obj->evdr = engine::onDraw([=]( /**/ ){
            if( !self->exists() ){ return; }
                 self->onDraw.emit(); 
        });

        process::add([=](){
            cb( self ); self->onOpen.emit(); 
            engine::locker.unlock(); return -1;
        });

    }

    /*─······································································─*/

    bool has_child( string_t name ) const noexcept { 
        if( !exists() ) /*------------*/ { return false; }
        if( obj->node.node_list.empty() ){ return false; }
        return obj->node.node_list.has( name ); 
    }

    void append_child( string_t name, const node_t& value ) const noexcept {

        if( !exists() ) /*----------------*/ { return; }
        if( has_child( name ) ){ remove_child( name ); }

        value.set_attribute( "name", name );
        value.obj->node.parent   =obj->node.node;
        obj->node.node_list[name]=value.obj->node;
    }

    void append_child( const node_t& value ) const noexcept {
         append_child( encoder::key::generate(12), value );
    }

    /*─······································································─*/

    ptr_t<node_t*> get_children() const noexcept {
    ptr_t<node_t*> out( obj->node.node_list.size() ); ulong w=0;

        auto x = obj->node.node_list.raw().first(); while(x!=nullptr){
        auto z = type::cast<node_t>( x->data.second.node );
        auto y = x->next; out[w]=z; x=y; ++w; }

    return out; }

    node_t* get_child( string_t name ) const noexcept {
        if( !has_child( name ) ) /*----------*/ { return nullptr; }
        return type::cast<node_t>( obj->node.node_list[name].node );
    }

    void remove_child( string_t name ) const noexcept {
        node_t* child=nullptr; /*------------------------*/
        if(( child=get_child(name) )==nullptr ) { return; }
        obj->node.node_list.erase( name ); child->free(); 
    }
    
    /*─······································································─*/

    ptr_t<render_queue_t> get_render_queue() const noexcept {
        auto view = get_viewport();
        if ( view==nullptr ){ return nullptr; }
        auto que  = type::bind( render_queue_t() );

        get_root()->child_iterator([=]( node_t* node ){

        //  if( node->has_attribute( "viewport" ) ){ return; }

            if( node->has_attribute /*--------------------------*/ ("visibility") ){
                auto visibility = node->get_attribute<visibility_t>("visibility");
            if( !( visibility->visible && ( visibility->mask & view->mask ) ) )
              { return; }}

            if( !node->on3DDraw.empty() ){ que->event3D.push( node->on3DDraw ); }
            if( !node->on2DDraw.empty() ){ que->event2D.push( node->on2DDraw ); }
            if( !node->onUIDraw.empty() ){ que->eventUI.push( node->onUIDraw ); }

        }, true ); return que;

    }

    viewport_t* get_viewport() const noexcept {
        node_t* root = type::cast<node_t>( obj->node.node ); 

        do { if ( root->get_parent()==nullptr ){ break; }
        if ( root->has_attribute( "viewport" )){ break; }
             root=root->get_parent(); 
        } while ( root->get_parent()!=nullptr );

        if (!root->has_attribute( "viewport" )){ return nullptr; }
        return &root->get_attribute<viewport_t>( "viewport" );
    }

    viewport_t* get_root_viewport() const noexcept {
        node_t* root =get_root(); /*-------------------------*/
        return  root==nullptr ? nullptr : root->get_viewport();
    }

    node_t* get_parent() const noexcept { 
        void*  parent =obj->node.parent;
        return parent==nullptr? nullptr: type::cast<node_t>( parent ); 
    }

    node_t* get_root() const noexcept { 
        node_t* root = type::cast<node_t>( obj->node.node ); 

        do { if ( root->get_parent()==nullptr ){ break; }
             root=root->get_parent(); 
        } while ( root->get_parent()!=nullptr );

        return root;
    }

    void child_iterator( function_t<void,node_t*> cb, bool deep=false ) const noexcept {
         node_iterator ( cb, (node_t*)( obj->node.node ), deep );
    }

    /*─······································································─*/

    node_t* get_node( string_t node_path ) const noexcept {

        auto list = regex::split( path::normalize( node_path ), "/" );
        if( list.empty()   ) /*-*/ { return nullptr; }
        if( !has_child( list[0] ) ){ return nullptr; }
        if( list.size()==1 ) /*-*/ { return get_child( list[0] ); }

        return get_child( list[0] )->get_node( list.slice(1).join("/") );

    }

    node_t* get_node() const noexcept {  
        return type::cast<node_t>( obj->node.node );
    }

    /*─······································································─*/

    bool exists() const noexcept { return obj->state; }

    void free() const noexcept { auto self = type::bind( this );
        if( !self->exists() ) { return; } /*-----------*/
        process::add([=](){ self->free_(); return -1; }); 
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node { node_t node_UI( function_t<void,ref_t<node_t>> clb ){
return node_t([=]( ref_t<node_t> self ){

    visibility_t visibility; transform_2D_t transform ;
    /*--------*/ visibility.mode = render::MODE::RENDER_MODE_UI;

    self->set_attribute( "visibility", visibility );
    self->set_attribute( "transform" , transform  );
    self->set_attribute( "type"      , "NodeUI"   );

    self->onLoop([=]( float delta ){ 

        auto tr = self->get_attribute<transform_2D_t>( "transform" );
        auto pr = self->get_parent();

    if ( pr != nullptr ){
        auto pt = pr->get_attribute<transform_2D_t>( "transform" );
    if ( pt == nullptr ){ goto DEFAULT; }

        auto sc = pt->translate.scale    * tr->scale   ;
        auto rt = pt->translate.rotation + tr->rotation;

        auto ps = pt->translate.position 
        /*---*/ + rl::Vector2Rotate( tr->position * tr->scale, pt->translate.rotation )
        /*---*/ - rl::Vector2Rotate( tr->origin   + vec2_t({
            pt->translate.scale.x / 2,
            pt->translate.scale.y / 2
        }) , rt );

        tr->translate.position = ps * tr->scale;
        tr->translate.scale    = tr->scale;
        tr->translate.rotation = rt;

    } else { DEFAULT:;

        tr->translate.scale    = tr->scale;
        tr->translate.rotation = tr->rotation;
        tr->translate.position = tr->position
        /*------------------*/ - rl::Vector2Rotate( tr->origin + vec2_t({
            tr->translate.scale.x / 2,
            tr->translate.scale.y / 2
        }), tr->translate.rotation );

    }});

clb(self); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node { node_t node_2D( function_t<void,ref_t<node_t>> clb ){
return node_t([=]( ref_t<node_t> self ){

    visibility_t visibility; transform_2D_t transform ;
    /*--------*/ visibility.mode = render::MODE::RENDER_MODE_2D;

    self->set_attribute( "visibility", visibility );
    self->set_attribute( "transform" , transform  );
    self->set_attribute( "type"      , "Node2D"   );

    self->onLoop([=]( float ){ 

        auto tr = self->get_attribute<transform_2D_t>( "transform" );
        auto pr = self->get_parent();

    if ( pr != nullptr ){
        auto pt = pr->get_attribute<transform_2D_t>( "transform" );
    if ( pt == nullptr ){ goto DEFAULT; }

        auto sc = pt->translate.scale    * tr->scale   ;
        auto rt = pt->translate.rotation + tr->rotation;

        auto ps = pt->translate.position 
        /*---*/ + rl::Vector2Rotate( tr->position, pt->translate.rotation )
        /*---*/ - rl::Vector2Rotate( tr->origin + vec2_t({
            tr->translate.scale.x / 2,
            tr->translate.scale.y / 2
        }), rt );

        tr->translate.position = ps;
        tr->translate.rotation = rt;
        tr->translate.scale    = tr->scale; // sc

    } else { DEFAULT:;

        tr->translate.scale    = tr->scale;
        tr->translate.rotation = tr->rotation;
        tr->translate.position = tr->position
        /*------------------*/ - rl::Vector2Rotate( tr->origin + vec2_t({
            tr->translate.scale.x / 2,
            tr->translate.scale.y / 2
        }), tr->translate.rotation );

    }});

clb(self); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace node { node_t node_3D( function_t<void,ref_t<node_t>> clb ){
return node_t([=]( ref_t<node_t> self ){

    visibility_t visibility; transform_3D_t transform ;
    /*--------*/ visibility.mode = render::MODE::RENDER_MODE_3D;

    self->set_attribute( "visibility", visibility );
    self->set_attribute( "transform" , transform  );
    self->set_attribute( "type"      , "Node3D"   );

    self->onLoop([=]( float delta ){ 

        auto tr = self->get_attribute<transform_3D_t>( "transform" );
        auto pr = self->get_parent();

    if ( pr != nullptr ){
        auto pt = pr->get_attribute<transform_3D_t>( "transform" );
    if ( pt == nullptr ){ goto DEFAULT; }

        tr->translate.position = math::vec3      ::rotation  ( tr->position, 
        /*--------------------*/ math::quaternion::from_euler( pt->translate.rotation )) 
        /*------------------*/ + pt->translate.position;

        tr->translate.rotation = math::quaternion::to_euler  ( math::quaternion::multiply(
        /*--------------------*/ math::quaternion::from_euler( pt->translate.rotation ) 
        /*------------------*/ , math::quaternion::from_euler( tr->rotation )));

        tr->translate.scale    = tr->scale;

    } else { DEFAULT:;

        tr->translate.position = tr->position;
        tr->translate.rotation = tr->rotation;
        tr->translate.scale    = tr->scale;

    }});

clb(self); }); }}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
