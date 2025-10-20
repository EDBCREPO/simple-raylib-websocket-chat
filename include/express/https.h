/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_EXPRESS_HTTPS
#define NODEPP_EXPRESS_HTTPS

/*────────────────────────────────────────────────────────────────────────────*/

#define MIDDL function_t<void,express_https_t&,function_t<void>>
#define CALBK function_t<void,express_https_t&>
#define MIMES express_tls_t

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>

#include <nodepp/optional.h>
#include <nodepp/cookie.h>
#include <nodepp/crypto.h>
#include <nodepp/stream.h>
#include <nodepp/https.h>
#include <nodepp/path.h>
#include <nodepp/json.h>
#include <nodepp/zlib.h>
#include <nodepp/url.h>
#include <nodepp/fs.h>
#include <nodepp/os.h>

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef EXPRESS_SERVER_SIDE_RENDERING
#define EXPRESS_SERVER_SIDE_RENDERING

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace express {

GENERATOR( ssr ) {
protected:

    struct NODE {
        int wait = 0; string_t borrow;
        queue_t<string_t> /*--*/ list;
    };

    generator::file::write write;
    ptr_t<zlib_t> zlb; ptr_t<NODE> obj;

public:

    ssr( bool ) noexcept : obj( new NODE() ) { zlb = new zlib_t(15|16); }
    ssr() /*-*/ noexcept : obj( new NODE() ) { zlb = nullptr; }

    template< class T >
    queue_t<string_t> render( T& str, string_t path ) noexcept {

        static regex_t reg0 = regex_t( "<°[ ]*|[ ]*°>" );
        bool b   = false; string_t out; ulong off=0;
        auto idx = reg0.search_all( path );
        queue_t<string_t> list;

        do{ if ( idx.empty()  ){ list.push( path ); break; }
            for( auto &x: idx ){
            list.push( path.slice( off, x[0] ) ); off=x[1];
        }   list.push( path.slice( off ) ); } while(0);

        auto x=list.first(); while( x!=nullptr ){
        auto y=x->next; /*---------------*/ b=!b;

        if( x->data.empty() ){ x=y; continue; }
        if( b == true )/*--*/{ x=y; continue; }

        if( x->data.size() <= MAX_PATH ){

            if( str->params.has( x->data ) ){
                x->data = str->params[ x->data ];
            } elif( fs::exists_file( x->data ) ){
                x->data = fs::read_file( x->data );
            }

        } x=y; }

    return list; }

    template< class T >
    coEmit( T& str, string_t path ){ void* addr = nullptr;
    coBegin;

        obj->list = render( str, path ); addr = obj->list.first();

        while( addr!=nullptr ){ do {

            auto x = obj->list.as(addr);
            obj->borrow = x->data;
            addr = x->next;

        } while(0);

            if( !zlb.has_value() ){ coWait( write( &str, obj->borrow )  ==1 ); }
          else{ coWait( write( &str, zlb->update_deflate( obj->borrow ))==1 ); }

        }

    coFinish }

};

/*────────────────────────────────────────────────────────────────────────────*/

GENERATOR( post_parser ){
public:

    template< class T >
    coEmit( const T& file, const ptr_t<object_t>& done, string_t raw ) {
        static regex_t reg8 = regex_t( "Content-Disposition", false );
        static regex_t reg7 = regex_t( "\r\n\r\n", true );
        if( raw.empty() ){ _state_=0; return -1; }
    coBegin

        try{ if( !reg8.test( raw ) ){ throw 0; }

            auto pos = reg7.search( raw );
            if ( pos.empty() ){ throw 0; }

            auto hdr = raw.slice( 0, pos[0] );
            auto bdy = raw.slice(/**/pos[1] );
            auto sby = bdy.slice( 0, 1024   ); header_t obj;

            ptr_t<regex_t> regs ({
                regex_t( "filename=\"([^\"]+)\"", true ),
                regex_t( "content-type: (\\n+)" , true ),
                regex_t( "name=\"([^\"]+)\""    , true ),
                regex_t( "^([^\r]+)\r\n"        , true )
          });

          regs[0].search(hdr); if( !regs[0].get_memory().empty() ){ obj["filename"]=regs[0].get_memory()[0]; }
          regs[1].search(hdr); if( !regs[1].get_memory().empty() ){ obj["mimetype"]=regs[1].get_memory()[0]; }
          regs[2].search(hdr); if( !regs[2].get_memory().empty() ){ obj["name"]    =regs[2].get_memory()[0]; }
          regs[3].search(sby); if( !regs[3].get_memory().empty() ){ sby /*------*/ =regs[3].get_memory()[0]; }
          /*----------------*/ else /*-------------------------*/ { sby.clear(); }

            if( !obj.has("filename") ){ (*done)[obj["name"]] = sby; coEnd; } else {
                auto sha = crypto::hash::SHA256();  sha.update( obj["mimetype"] );
                     sha.update( encoder::key::generate("0123456789abcdef",32) );
                     sha.update( obj["filename"] ); sha.update( obj["name"] );
                     sha.update( string::to_string( process::now() ) );
                obj["path"] = path::join( os::tmp(), sha.get() + ".tmp" );
                *file = fs::writable( obj["path"] );
            }

            if( !(*done)[obj["name"]].has_value() ){ (*done)[obj["name"]] = array_t<object_t>(); }
            auto list=(*done)[obj["name"]].as<array_t<object_t>>(); auto name = obj["name"];
            obj.erase("name"); list.push( json::parse( obj ) ); (*done)[name] = list;
            file->write( bdy );

        } catch(...) { coEnd; } do { coNext; file->write(raw); } while(1);

    coFinish }

};

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class express_https_t : public https_t {
protected:

    struct NODE {
        header_t _headers;
        cookie_t _cookies;
        uint  status= 200;
        int    state= 0;
    };  ptr_t<NODE> exp;

public: query_t params;

    express_https_t ( https_t& cli ) noexcept : https_t( cli ), exp( new NODE() ) { exp->state = 1; }
    virtual ~express_https_t () noexcept { if( exp.count() > 1 ){ return; } exp->state = 0; free(); }
    express_https_t () noexcept : exp( new NODE() ) { exp->state = 0; }

    /*.........................................................................*/

    bool is_express_available() const noexcept { return exp->state >  0; }
    bool is_express_closed()    const noexcept { return exp->state <= 0; }

    /*.........................................................................*/

    promise_t<object_t,except_t> parse_stream() const noexcept {

        auto read = type::bind( generator::file::until() );
        auto tsk  = type::bind( express::post_parser() );
        auto done = type::bind( object_t() );
        auto file = type::bind( file_t() );
        auto self = type::bind( this );

    return promise_t<object_t,except_t>( [=]( function_t<void,object_t> res, function_t<void,except_t> rej ){
        if( !self->headers.has("Content-Length") ){ rej( except_t( "content length mismatch" ) ); return; }

        static regex_t reg6 = regex_t( "boundary=[^ ]+", true );
        auto len = type::bind( string::to_ulong( self->headers["Content-Length"] ) );
        auto bon = "--" + reg6.match( self->headers["Content-Type"] ).slice(9);
        if ( bon.empty() ){ res(json::parse(query::parse(url::normalize("?"+self->read())))); return; }

        process::add( coroutine::add( COROUTINE(){
            if( self->is_closed() ){ rej("something went wrong"); return -1; }
        coBegin

            while( *len>0 && self->is_available() ) {
           coWait((*read)( &self, bon )==1 ); *len-=min( (ulong) read->state,*len );
               if( read->state<= 0         ){ coGoto(1); }
               if( read->data != bon       ){
                        (*tsk)(file,done,read->data);
               } else { (*tsk)(file,done,nullptr   ); }
           }

            res(*done); coEnd; coYield(1); do {
            for( auto x: done->keys() ){
            if ((*done)[x].is<array_t<object_t>>() ){
            for( auto y:(*done)[x].as<array_t<object_t>>() ){
                 fs::remove_file( y["path"].as<string_t>() );
            }}}} while(0); rej("something went wrong");

        coFinish
        }));

    }); }

    /*.........................................................................*/

    const express_https_t& send( string_t msg ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
        header( "Content-Length", string::to_string(msg.size()) );
        if( regex::test( headers["Accept-Encoding"], "gzip" ) && msg.size()>UNBFF_SIZE ){
            header( "Content-Encoding", "gzip" ); send();
            write( zlib::gzip::get( msg ) ); close();
        } else {
            send(); write( msg ); close();
        }   exp->state =0; return (*this);
    }

    const express_https_t& send_file( string_t dir ) const noexcept {
        if( exp->state == 0 ){ return (*this); } if( fs::exists_file( dir ) == false )
          { status(404).send("file does not exist"); } file_t file ( dir, "r" );
            header( "Content-Length", string::to_string(file.size()) );
            header( "Content-Type", path::mimetype(dir) );
        if( regex::test( headers["Accept-Encoding"], "gzip" ) ){
            header( "Content-Encoding", "gzip" ); send();
            zlib::gzip::pipe( file, *this );
        } else {
            send(); stream::pipe( file, *this );
        }   exp->state = 0; return (*this);
    }

    const express_https_t& send_json( object_t json ) const noexcept {
        if( exp->state == 0 ){ return (*this); } auto data = json::stringify(json);
        header( "content-length", string::to_string(data.size()) );
        header( "content-type", path::mimetype(".json") );
        send( data ); exp->state = 0; return (*this);
    }

    const express_https_t& cache( ulong time ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
        header( "Cache-Control",string::format( "public, max-age=%lu",time) );
        return (*this);
    }

    const express_https_t& cookie( string_t name, string_t value ) const noexcept {
        if( exp->state == 0 ){ return (*this); } exp->_cookies[ name ] = value;
        header( "Set-Cookie", cookie::format( exp->_cookies ) );
        return (*this);
    }

    const express_https_t& header( string_t name, string_t value ) const noexcept {
        if( exp->state == 0 )    { return (*this); }
        exp->_headers[name]=value; return (*this);
    }

    const express_https_t& redirect( uint value, string_t url ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
        header( "location",url ); status( value );
        send(); exp->state = 0; return (*this);
    }

    template< class T >
    const express_https_t& send_stream( T readableStream ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
        if( regex::test( headers["Accept-Encoding"], "gzip" ) ){
            header( "Content-Encoding", "gzip" ); send();
            zlib::gzip::pipe( readableStream, *this );
        } else { send();
            stream::pipe( readableStream, *this );
        }   exp->state = 0; return (*this);
    }

    const express_https_t& header( header_t headers ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
        auto x=headers.raw().first(); while( x!=nullptr ){
        auto y=x->next; /*-------------------------------*/
             exp->_headers[x->data.first] = x->data.second;
        x=y; } return (*this);
    }

    const express_https_t& redirect( string_t url ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
        return redirect( 302, url );
    }

    const express_https_t& render( string_t path ) const noexcept {
        if( exp->state == 0 ){ return (*this); }

        auto self = type::bind( this );

        if( regex::test( headers["Accept-Encoding"], "gzip" ) ){
            auto task = express::ssr( true );
            header( "Content-Encoding", "gzip" );
            send(); process::add( task, self, path );
        } else {
            auto task = express::ssr();
            send(); process::add( task, self, path );
        }

        return (*this);
    }

    const express_https_t& render( file_t file ) const noexcept {
        if( exp->state == 0 ){ return (*this); }

        auto self = type::bind( this );

        if( regex::test( headers["Accept-Encoding"], "gzip" ) ){
            auto task = express::ssr( true );
            header( "Content-Encoding", "gzip" );
            send(); process::add( task, self, stream::await( file ) );
        } else {
            auto task = express::ssr();
            send(); process::add( task, self, stream::await( file ) );
        }

        return (*this);
    }

    const express_https_t& status( uint value ) const noexcept {
        if( exp->state == 0 ){ return (*this); }
            exp->status=value; return (*this);
    }

    const express_https_t& clear_cookies() const noexcept {
        if( exp->state == 0 ){ return (*this); }
        header( "Clear-Site-Data", "\"cookies\"" );
        return (*this);
    }

    const express_https_t& send() const noexcept {
        if( exp->state == 0 ){ return (*this); }
        write_header(exp->status,exp->_headers);
        exp->state = 0; return (*this);
    }

    const express_https_t& done() const noexcept {
        if( exp->state == 0 ){ return (*this); }
        exp->state = 0; return (*this);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class express_tls_t {
protected:

    struct express_item_t {
        optional_t<MIDDL> middleware;
        optional_t<CALBK> callback;
        optional_t<MIMES> router;
        string_t          method;
        string_t          path;
    };

    struct NODE {
        queue_t<express_item_t> list;
        ptr_t<agent_t> agent=nullptr;
        ptr_t<ssl_t>   ssl  =nullptr;
        string_t       path =nullptr;
        tls_t fd;
    };  ptr_t<NODE> obj;

    void execute( string_t path, express_item_t& data, express_https_t& cli, function_t<void>& next ) const noexcept {
        if  ( data.middleware.has_value() ){ data.middleware.value()( cli, next ); return; }
        elif( data.callback.has_value()   ){ data.callback.value()( cli ); /*-----------*/ }
        elif( data.router.has_value()     ){ data.router.value().run( path, cli ); /*---*/ }
    next(); }

    bool path_match( express_https_t& cli, string_t base, string_t path ) const noexcept {
        string_t pathname = normalize( base, path );

        array_t<string_t> _path[2] = {
            string::split( cli.path, '/' ),
            string::split( pathname, '/' )
        };

        if( regex::test( cli.path, "^"+pathname ) ){ return true;  }
        if( _path[0].size() != _path[1].size() )   { return false; }

        for ( ulong x=0; x<_path[0].size(); ++x ){
        if  ( _path[1][x] ==nullptr ){ continue; } /*--------------------------*/
        elif( _path[1][x][0] == ':' ){ if( _path[0][x].empty() ){ return false; }
              cli.params[_path[1][x].slice(1)] = url::normalize( _path[0][x] ); }
        elif( _path[1][x].empty() /*--*/ ){ continue;     }
        elif( _path[1][x] != _path[0][x] ){ return false; }}

        return true;
    }

    void run( string_t path, express_https_t& cli ) const noexcept {

        auto n     = obj->list.first();
        auto _base = normalize( path, obj->path );
        function_t<void> next = [&](){ n = n->next; };

        while ( n!=nullptr ) {
            if( !cli.is_available() || cli.is_express_closed() ){ break; }
            if(( n->data.path.empty() && obj->path.empty() ) /*-----------*/
            || ( path_match( cli, _base, n->data.path ) ) /*--------------*/
            || ( n->data.path.empty() && regex::test( cli.path, "^"+_base ))
        ){  if ( n->data.method.empty() || n->data.method==cli.method ){
                 execute( _base, n->data, cli, next );
        } else { next(); }} else { next(); }}

    }

    string_t normalize( string_t base, string_t path ) const noexcept {
        auto new_path =  base.empty() ? ("/"+path) : path.empty() ? /*------*/
        /*---------------------------*/ ("/"+base) : path::join( base, path );
        return path::normalize( new_path );
    }

public:

    express_tls_t( ssl_t* ssl=nullptr, agent_t* agent=nullptr ) noexcept : obj( new NODE() ){ 
        obj->agent = type::bind( agent ); 
        obj->ssl   = type::bind( ssl );
    }

    /*.........................................................................*/

    void     set_path( string_t path ) const noexcept { obj->path = path; }
    string_t get_path() /*----------*/ const noexcept { return obj->path; }

    /*.........................................................................*/

    bool is_closed() const noexcept { return obj->fd.is_closed(); }
    void     close() const noexcept { obj->fd.close(); }
    tls_t   get_fd() const noexcept { return obj->fd; }

    /*.........................................................................*/

    const express_tls_t& RAW( string_t _method, string_t _path, CALBK cb ) const noexcept {
        express_item_t item; // memset( (void*) &item, 0, sizeof(item) );
        item.path     = _path.empty() ? nullptr : _path;
        item.method   = _method;
        item.callback = cb;
        obj->list.push( item ); return (*this);
    }

    const express_tls_t& RAW( string_t _method, CALBK cb ) const noexcept {
        return RAW( _method, nullptr, cb );
    }

    const express_tls_t& RAW( CALBK cb ) const noexcept {
        return RAW( nullptr, nullptr, cb );
    }

    /*.........................................................................*/

    const express_tls_t& USE( string_t _path, express_tls_t cb ) const noexcept {
        express_item_t item; // memset( (void*) &item, 0, sizeof(item) );
        cb.set_path( normalize( obj->path, _path ) );
        item.method     = nullptr; /*---------*/
        item.path       = nullptr; /*---------*/
        item.router     = optional_t<MIMES>(cb);
        obj->list.push( item ); return (*this);
    }

    const express_tls_t& USE( express_tls_t cb ) const noexcept {
        return USE( nullptr, cb );
    }

    /*.........................................................................*/

    const express_tls_t& USE( string_t _path, MIDDL cb ) const noexcept {
        express_item_t item; // memset( (void*) &item, 0, sizeof(item) );
        item.path       = _path.empty() ? nullptr : _path;
        item.middleware = optional_t<MIDDL>(cb);
        item.method     = nullptr; /*---------*/
        obj->list.push( item ); return (*this);
    }

    const express_tls_t& USE( MIDDL cb ) const noexcept {
        return USE( nullptr, cb );
    }

    /*.........................................................................*/

    const express_tls_t& ALL( string_t _path, CALBK cb ) const noexcept {
        return RAW( nullptr, _path, cb );
    }

    const express_tls_t& ALL( CALBK cb ) const noexcept {
        return RAW( cb );
    }

    /*.........................................................................*/

    const express_tls_t& GET( string_t _path, CALBK cb ) const noexcept {
        return RAW( "GET", _path, cb );
    }

    const express_tls_t& GET( CALBK cb ) const noexcept {
        return RAW( "GET", cb );
    }

    /*.........................................................................*/

    const express_tls_t& POST( string_t _path, CALBK cb ) const noexcept {
        return RAW( "POST", _path, cb );
    }

    const express_tls_t& POST( CALBK cb ) const noexcept {
        return RAW( "POST", cb );
    }

    /*.........................................................................*/

    const express_tls_t& QUERY( string_t _path, CALBK cb ) const noexcept {
        return RAW( "QUERY", _path, cb );
    }

    const express_tls_t& QUERY( CALBK cb ) const noexcept {
        return RAW( "QUERY", cb );
    }

    /*.........................................................................*/

    const express_tls_t& REMOVE( string_t _path, CALBK cb ) const noexcept {
        return RAW( "DELETE", _path, cb );
    }

    const express_tls_t& REMOVE( CALBK cb ) const noexcept {
        return RAW( "DELETE", cb );
    }

    /*.........................................................................*/

    const express_tls_t& PUT( string_t _path, CALBK cb ) const noexcept {
        return RAW( "PUT", _path, cb );
    }

    const express_tls_t& PUT( CALBK cb ) const noexcept {
        return RAW( "PUT", cb );
    }

    /*.........................................................................*/

    const express_tls_t& HEAD( string_t _path, CALBK cb ) const noexcept {
        return RAW( "HEAD", _path, cb );
    }

    const express_tls_t& HEAD( CALBK cb ) const noexcept {
        return RAW( "HEAD", cb );
    }

    /*.........................................................................*/

    const express_tls_t& TRACE( string_t _path, CALBK cb ) const noexcept {
        return RAW( "TRACE", _path, cb );
    }

    const express_tls_t& TRACE( CALBK cb ) const noexcept {
        return RAW( "TRACE", cb );
    }

    /*.........................................................................*/

    const express_tls_t& PATCH( string_t _path, CALBK cb ) const noexcept {
        return RAW( "PATCH", _path, cb );
    }

    const express_tls_t& PATCH( CALBK cb ) const noexcept {
        return RAW( "PATCH", cb );
    }

    /*.........................................................................*/

    const express_tls_t& OPTIONS( string_t _path, CALBK cb ) const noexcept {
        return RAW( "OPTION", _path, cb );
    }

    const express_tls_t& OPTIONS( CALBK cb ) const noexcept {
        return RAW( "OPTION", cb );
    }

    /*.........................................................................*/

    const express_tls_t& CONNECT( string_t _path, CALBK cb ) const noexcept {
        return RAW( "CONNECT", _path, cb );
    }

    const express_tls_t& CONNECT( CALBK cb ) const noexcept {
        return RAW( "CONNECT", cb );
    }

    /*.........................................................................*/

    template<class... T>
    tls_t& listen( const T&... args ) const noexcept {
        auto self = type::bind( this );

        function_t<void,https_t> cb = [=]( https_t cli ){
            express_https_t res(cli); if( cli.headers.has("Params") ){
                res.params= query::parse( cli.headers["Params"] );
            }   self->run( nullptr, res );
        };

        obj->fd=https::server( cb, &obj->ssl, &obj->agent );
        obj->fd.listen( args... ); return obj->fd;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace express { namespace https {

    template< class... T > express_tls_t add( T... args ) { return express_tls_t(args...); }

    express_tls_t file( string_t base ) { express_tls_t app;

        app.ALL([=]( express_https_t& cli ){ try {

            auto pth = regex::replace( cli.path, app.get_path().slice(1), "/" );
                 pth = regex::replace_all( pth, "\\.[.]+/", "" );

            auto dir = pth.empty()? path::join( base, "" ):
            /*-------------------*/ path::join( base,pth );

            if( dir.empty() ) /*-----*/ { dir = base; }
            if( fs::exists_folder(dir) ){
            if(!fs::exists_file( dir + "index.html" ) )
              { cli.send("file not found 404"); return; }
                dir += "index.html";
            }

            auto str = fs::readable( dir );

            if( !cli.headers.has("Range") ){

                if( regex::test(path::mimetype(dir),"audio|video",true) )
                  { cli.send(); return; }

                cli.header( "Cache-Control", "public, max-age=604800" );
                cli.header( "Content-Type" , path::mimetype( dir ) );
                cli.send_stream( str );

            } else {

                array_t<string_t> range = regex::match_all(cli.headers["Range"],"\\d+",true);
                   ulong rang[3]; rang[0] = string::to_ulong( range[0] );
                         rang[1] =min(rang[0]+CHUNK_MB(10),str.size()-1);
                         rang[2] =min(rang[0]+CHUNK_MB(10),str.size()  );

                cli.header( "Content-Range", string::format("bytes %lu-%lu/%lu",rang[0],rang[1],str.size()) );
                cli.header( "Content-Type",  path::mimetype(dir) ); cli.header( "Accept-Range", "bytes" );
                cli.header( "Cache-Control", "public, max-age=604800" );

                str.set_range( rang[0], rang[2] );
                cli.status(206).send_stream( str );

            }

        } catch(...) { cli.status(404).send("file not found"); } });

        return app;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

#undef MIMES
#undef CALBK
#undef MIDDL
#endif
