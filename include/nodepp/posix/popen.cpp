/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once
#include <unistd.h>
#include <sys/wait.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class popen_t : public generator_t {
protected:

    void kill() const noexcept { 
    if( obj->fd != -1 ){
        ::kill( obj->fd, SIGKILL );
    }   obj->state |= FILE_STATE::KILL; }

    using _read_ = generator::file::read;

    bool is_state( uchar value ) const noexcept {
        if( obj->state & value ){ return true; }
    return false; }

    void set_state( uchar value ) const noexcept {
    if( obj->state & KILL ){ return; }
        obj->state = value;
    }

    enum FILE_STATE {
        UNKNOWN = 0b00000000,
        OPEN    = 0b00000001,
        CLOSE   = 0b00000010,
        KILL    = 0b00000100,
        REUSE   = 0b00001000,
        DISABLE = 0b00001110
    };

protected:

    ptr_t<_read_> _read1 = new _read_();
    ptr_t<_read_> _read2 = new _read_();

    struct NODE {
        uchar       state=FILE_STATE::CLOSE;
        int       fd = -1;
        file_t  std_input;
        file_t  std_error;
        file_t  std_output;
    };  ptr_t<NODE> obj;

    template< class T >
    void _init_( const string_t& path, T& arg, T& env ) {

        int fda[2]; ::pipe( fda );
        int fdb[2]; ::pipe( fdb );
        int fdc[2]; ::pipe( fdc ); obj->fd = ::fork();

        if( obj->fd == 0 ){
            ::dup2( fda[0], STDIN_FILENO  ); ::close( fda[1] );
            ::dup2( fdb[1], STDOUT_FILENO ); ::close( fdb[0] ); arg.push( nullptr );
            ::dup2( fdc[1], STDERR_FILENO ); ::close( fdc[0] ); env.push( nullptr );
            ::execvpe( path.c_str(), (char**)arg.data(),(char**)env.data() );
            throw except_t("while spawning new popen"); process::exit(1);
        } elif ( obj->fd > 0 ){
            obj->std_input  = file_t( fda[1] ); ::close( fda[0] );
            obj->std_output = file_t( fdb[0] ); ::close( fdb[1] );
            obj->std_error  = file_t( fdc[0] ); ::close( fdc[1] );
            set_state( FILE_STATE::OPEN );
        } else {
            ::close( fda[0] ); ::close( fda[1] );
            ::close( fdb[0] ); ::close( fdb[1] );
            ::close( fdc[0] ); ::close( fdc[1] );
            set_state( FILE_STATE::CLOSE );
        }

    }

public:

    event_t<>          onResume;
    event_t<except_t>  onError;
    event_t<>          onClose;
    event_t<>          onStop;
    event_t<>          onDrain;
    event_t<>          onOpen;

    event_t<string_t>  onData;
    event_t<string_t>  onDout;
    event_t<string_t>  onDerr;

    popen_t( const string_t& path, const initializer_t<string_t>& args, const initializer_t<string_t>& envs )
    : obj( new NODE() ) { if( path.empty() ){ throw except_t("invalid command"); }
        array_t<const char*> arg; array_t<const char*> env;
        for( auto x : args ) { arg.push( x.get() ); } /*---------------------*/
        for( auto x : envs ) { env.push( x.get() ); } _init_( path, arg, env );
    }

    popen_t( const string_t& path ) 
    : obj( new NODE() ) { if( path.empty() ){ throw except_t("invalid command"); }
        array_t<const char*> arg; array_t<const char*> env; auto cmd = regex::match_all( path, "[^ ]+" );
        for( auto x: cmd ){ arg.push( x.get() ); } _init_( cmd[0], arg, env ); /*----------------------*/
    }

    popen_t( const string_t& path, const initializer_t<string_t>& args ) : obj( new NODE() ) { 
        if ( path.empty() ){ throw except_t("invalid command"); }
        array_t<const char*> arg; array_t<const char*> env;
        for( auto x : args ) { arg.push( x.get() ); }
        _init_( path, arg, env ); /*---------------*/
    }

    popen_t() noexcept : obj( new NODE() ) {}

    virtual ~popen_t() noexcept { if( obj.count() > 1 ){ return; } free(); }

    /*─······································································─*/

    void free() const noexcept {
        
        if( is_state( FILE_STATE::REUSE ) && obj.count()>1 ){ resume(); return; }
        if( is_state( FILE_STATE::KILL  ) ){ return; } close(); kill();

    /*
        obj->std_error.close(); obj->std_output.close();
        obj->std_input.close();
    */

        onResume.clear(); onError.clear(); 
        onStop  .clear(); onOpen .clear();
        onData  .clear(); onDout .clear(); 
        onDerr  .clear(); /*------------*/ onClose.emit();

    }

    /*─······································································─*/

    inline int next() noexcept {
    coBegin ; onOpen.emit(); coYield(1);

        if( is_closed() ){ free(); coNext; do { int status=0;
        if( ::waitpid( obj->fd, &status, WNOHANG ) == -1 )
          { return 1; }} while(0); coEnd; }

        if((*_read1)(&std_output())==1){ coGoto(2); }
        if(  _read1->state <= 0 )      { coGoto(2); }
        onData.emit(_read1->data);
        onDout.emit(_read1->data);       coYield(2);

        if((*_read2)(&std_error())==1 ){ coGoto(1); }
        if(  _read2->state <= 0 )      { coGoto(1); }
        onData.emit(_read2->data);
        onDerr.emit(_read2->data);
        
    coGoto(1); coFinish
    }

    /*─······································································─*/

    bool is_alive() const noexcept {
        if( ::kill( obj->fd , 0 ) == -1 ){ return false; }
        if( std_error ().is_available() ){ return true;  }
        if( std_output().is_available() ){ return true;  } return false; 
    }

    bool is_closed()    const noexcept { return is_state( FILE_STATE::DISABLE ) || !is_alive(); }
    bool is_available() const noexcept { return is_closed() == false; }
    int  get_fd()       const noexcept { return obj->fd; }

    /*─······································································─*/

    void resume() const noexcept { if(is_state(FILE_STATE::OPEN) ){ return; } set_state(FILE_STATE::OPEN ); onResume.emit(); }
    void  close() const noexcept { if(is_state(FILE_STATE::CLOSE)){ return; } set_state(FILE_STATE::CLOSE); onDrain .emit(); }
    void   stop() const noexcept { if(is_state(FILE_STATE::REUSE)){ return; } set_state(FILE_STATE::REUSE); onStop  .emit(); }
    void  flush() const noexcept { std_input().flush(); std_output().flush(); std_error().flush(); }

    /*─······································································─*/

    template< class... T >
    int write( const T&... args )     const noexcept { return std_input().write( args... ); }

    template< class... T >
    string_t read( const T&... args ) const noexcept { return std_output().read( args... ); }

    /*─······································································─*/

    template< class... T >
    int _write( const T&... args ) const noexcept { return std_input()._write( args... ); }

    template< class... T >
    int _read( const T&... args )  const noexcept { return std_output()._read( args... ); }

    /*─······································································─*/

    file_t& std_error()  const noexcept { return obj->std_error;  }
    file_t& std_output() const noexcept { return obj->std_output; }
    file_t& std_input()  const noexcept { return obj->std_input;  }

};}

/*────────────────────────────────────────────────────────────────────────────*/