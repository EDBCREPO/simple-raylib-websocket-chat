/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_GLOBAL
#define UNGINE_GLOBAL

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class global_t {
protected:

    struct ATTR { object_t obj; }; ptr_t<ATTR> attr;

public:

    /*----*/ global_t() noexcept : attr( new ATTR() ){}
    virtual ~global_t() noexcept { /*--------------*/ }

    /*─······································································─*/

    bool has_attribute   ( string_t name ) const noexcept { return attr->obj.has(name); }

    void remove_attribute( string_t name ) const noexcept { attr->obj.erase( name ); }

    /*─······································································─*/

    void set_attribute( string_t name, const char* value ) const noexcept {
        attr->obj[ name ] = type::bind( string::to_string( value ) ); 
    }

    template< class T >
    void set_attribute( string_t name, T value ) const noexcept {
        attr->obj[ name ] = type::bind( value );
    }

    /*─······································································─*/

    template< class T >
    ref_t<T> get_attribute( string_t name ) const { try {

        if( !attr->obj.has( name ) ){ return nullptr; }
        return attr->obj[ name ].as<ptr_t<T>>();

    } catch( except_t err ) { return nullptr; }}

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
