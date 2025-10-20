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
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { enum POLL_STATE {
    READ = 1, WRITE = 2, DUPLEX = 3
};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using KPOLLFD = struct kevent; }
namespace nodepp { class poll_t : public generator_t {
private:

    using NODE_CLB = function_t<int>;
    using DATA     = type::pair<int,NODE_CLB>;

    struct waiter { bool blk; bool out; }; 
    
protected:

    struct NODE {
        queue_t<DATA>  queue;
        int      y,len,pd;
        ptr_t<KPOLLFD> ev;
    };  ptr_t<NODE>   obj;

    /*─······································································─*/
    
    bool listen( const int fd, const int flags, void* ptr ) noexcept { bool x=1, y=1;
        if  ( flags & POLL_STATE::READ  ){ x = append( fd, EVFILT_READ , ptr )!=-1; }
        elif( flags & POLL_STATE::WRITE ){ y = append( fd, EVFILT_WRITE, ptr )!=-1; }
        return ( x && y );
    }

    int append( const int fd, const int flags, void* ptr ) const noexcept {
        KPOLLFD event; /*--------------------------------------*/
        EV_SET( &event, fd, flags, EV_ADD|EV_ENABLE, 0, 0, ptr );
        return kevent( obj->pd, &event, 1, NULL, 0, NULL );
    }

    int remove( void* ptr ) const noexcept {
        auto pt = obj->queue.as( ptr ); KPOLLFD event;
        auto fd = pt->data.first; obj->queue.erase(pt);
        EV_SET( &event, fd, 0, EV_DELETE|EV_DISABLE, 0, 0, NULL );
        return kevent( obj->pd, &event, 1, NULL, 0, NULL );
    }

    /*─······································································─*/

    template< class T, class... V >
    void* push( int fd, T cb, const V&... arg ) const noexcept {

        ptr_t<waiter> tsk = new waiter();
        auto clb=type::bind( cb );
        tsk->blk=0; tsk->out=1; 

        obj->queue.push({ fd, [=](){
            if( tsk->out==0 ){ return -1; }
            if( tsk->blk==1 ){ return  1; } 
                tsk->blk =1; int rs=(*clb)( arg... );
            if( clb.null () ){ return -1; }  
                tsk->blk =0; return !tsk->out ? -1 : rs;
        } }); 
        
        return (void*) &tsk->out;
    }

    /*─······································································─*/

    void clear( void* address ){
         if( address == nullptr ){ return; }
         memset( address, 0, sizeof(bool) );
    }

public:

    virtual ~poll_t() noexcept { if( obj.count() > 1 ){ return; } close( obj->pd ); }

    poll_t() : obj( new NODE() ) {
        obj->pd = kqueue(0); if( obj->pd==-1 )
      { throw except_t("Can't open more kqueue fd"); }
        obj->ev.resize( limit::get_soft_fileno() );
    }

    /*─······································································─*/

    void clear() const noexcept { /*--*/ obj->queue.clear(); }

    ulong size() const noexcept { return obj->queue.size (); }

    bool empty() const noexcept { return obj->queue.empty(); }

    /*─······································································─*/

    template< class T, class U, class... W >
    void* add( T& inp, uchar imode, U cb, const W&... args ) noexcept {
    auto addr = push( inp.get_fd(), cb, args... ); /*----------*/
    bool    x = listen( inp.get_fd(), imode, obj->queue.last() );
    if( !x ){ obj->queue.pop(); } return addr; }

    /*─······································································─*/

    inline int next() noexcept {
    coBegin

        if((obj->len=kevent( obj->pd, NULL, 0, &obj->ev, obj->ev.size(), 0 ))<=0 )
          { coEnd; } obj->y=0; coNext;

        while( obj->y < obj->len ){ do { 
            
            auto x = obj->ev[ obj->y ];
            auto y = obj->queue.as( x.udata );

            if( x.flags & ( EV_ERROR    | EV_EOF       ) &&
              ( x.filter& ( EVFILT_READ | EVFILT_WRITE ))==0
            ) { remove( y ); ++obj->y; break; }
            
            switch( type::cast<NODE_CLB>( y->data.second ).emit() ){
                case -1: remove( y ); ++obj->y; break;
                case  1: /*--------*/ ++obj->y; break;
                default: /*------------------*/ break;
            }

        } while(0); coNext; }

    coFinish }

};}

/*────────────────────────────────────────────────────────────────────────────*/