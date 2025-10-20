/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_SHADER
#define UNGINE_SHADER

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef GPU_KERNEL
#define GPU_KERNEL(...) #__VA_ARGS__

#if defined(GRAPHICS_API_OPENGL_33)
    #define GLSL_VERSION "#version 330\n"
#elif defined(GRAPHICS_API_OPENGL_21)
    #define GLSL_VERSION "#version 120\n"
#else
    #define GLSL_VERSION "#version 100\nprecision highp float;\n"
#endif

#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace gpu { 

template< class T > struct gpu_type_id  { static constexpr uchar value = 0xff; };

template<> struct gpu_type_id<int>      { static constexpr uchar value = 0x01; };
template<> struct gpu_type_id<bool>     { static constexpr uchar value = 0x02; };
template<> struct gpu_type_id<uint>     { static constexpr uchar value = 0x03; };
template<> struct gpu_type_id<float>    { static constexpr uchar value = 0x04; };

template<> struct gpu_type_id<bvec2_t>  { static constexpr uchar value = 0x11; };
template<> struct gpu_type_id<ivec2_t>  { static constexpr uchar value = 0x12; };
template<> struct gpu_type_id<uvec2_t>  { static constexpr uchar value = 0x13; };
template<> struct gpu_type_id< vec2_t>  { static constexpr uchar value = 0x14; };

template<> struct gpu_type_id<bvec3_t>  { static constexpr uchar value = 0x21; };
template<> struct gpu_type_id<ivec3_t>  { static constexpr uchar value = 0x22; };
template<> struct gpu_type_id<uvec3_t>  { static constexpr uchar value = 0x23; };
template<> struct gpu_type_id< vec3_t>  { static constexpr uchar value = 0x24; };

template<> struct gpu_type_id<bvec4_t>  { static constexpr uchar value = 0x31; };
template<> struct gpu_type_id<ivec4_t>  { static constexpr uchar value = 0x32; };
template<> struct gpu_type_id<uvec4_t>  { static constexpr uchar value = 0x33; };
template<> struct gpu_type_id< vec4_t>  { static constexpr uchar value = 0x34; };

template<> struct gpu_type_id<mat_t>    { static constexpr uchar value = 0x50; };
template<> struct gpu_type_id<texture_t>{ static constexpr uchar value = 0x51; };

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { struct shader_t{
protected:

    struct SVAR { any_t value; uchar type; };

    struct NODE {
        map_t<string_t,SVAR> f_vars;
        map_t<string_t,SVAR> v_vars;
        ptr_t<rl::Shader> ctx;
        string_t fs, vs; 
    };  ptr_t<NODE> obj;

    /*─······································································─*/

    template< class T >
    void set_variable( string_t name, int flag, const T& value ) const noexcept {
         int sid = rl::GetShaderLocation( *obj->ctx, name.get() );
         SetShaderValueV( *obj->ctx, sid, &value, flag, 1 );
    }

    void set_texture( string_t name, const texture_t& value ) const noexcept {
         int sid = rl::GetShaderLocation( *obj->ctx, name.get() );
         SetShaderValueTexture( *obj->ctx, sid, value.get() );        
    }

    void set_matrix( string_t name, const mat_t& value ) const noexcept {
         int sid = rl::GetShaderLocation( *obj->ctx, name.get() );
         SetShaderValueMatrix( *obj->ctx, sid, value );        
    }

    /*─······································································─*/

    void set_fragment_variables() const /*noexcept*/ {
    if ( obj->ctx.null() ) /*----------*/ { throw except_t("invalid shader"); }
    if ( !rl::IsShaderValid( *obj->ctx ) ){ throw except_t("Invalid Shader"); }
    for( auto x: obj->f_vars.data() )/*-*/{ switch( x.second.type ){

        case 0x01: set_variable( x.first, gpu::UNIFORM_BOOL , x.second.value.as<bool> ());   break;
        case 0x02: set_variable( x.first, gpu::UNIFORM_INT  , x.second.value.as<int>  ());   break;
        case 0x03: set_variable( x.first, gpu::UNIFORM_UINT , x.second.value.as<uint> ());   break;
        case 0x04: set_variable( x.first, gpu::UNIFORM_FLOAT, x.second.value.as<float>());   break;

        case 0x11: set_variable( x.first, gpu::UNIFORM_BVEC2, x.second.value.as<bvec2_t>()); break;
        case 0x12: set_variable( x.first, gpu::UNIFORM_IVEC2, x.second.value.as<ivec2_t>()); break;
        case 0x13: set_variable( x.first, gpu::UNIFORM_UVEC2, x.second.value.as<uvec2_t>()); break;
        case 0x14: set_variable( x.first, gpu::UNIFORM_VEC2 , x.second.value.as< vec2_t>()); break;

        case 0x21: set_variable( x.first, gpu::UNIFORM_BVEC3, x.second.value.as<bvec3_t>()); break;
        case 0x22: set_variable( x.first, gpu::UNIFORM_IVEC3, x.second.value.as<ivec3_t>()); break;
        case 0x23: set_variable( x.first, gpu::UNIFORM_UVEC3, x.second.value.as<uvec3_t>()); break;
        case 0x24: set_variable( x.first, gpu::UNIFORM_VEC3 , x.second.value.as< vec3_t>()); break;

        case 0x31: set_variable( x.first, gpu::UNIFORM_BVEC4, x.second.value.as<bvec4_t>()); break;
        case 0x32: set_variable( x.first, gpu::UNIFORM_IVEC4, x.second.value.as<ivec4_t>()); break;
        case 0x33: set_variable( x.first, gpu::UNIFORM_UVEC4, x.second.value.as<uvec4_t>()); break;
        case 0x34: set_variable( x.first, gpu::UNIFORM_VEC4 , x.second.value.as< vec4_t>()); break;

        case 0x50: set_matrix  ( x.first, x.second.value.as<mat_t>() ); /*----------------*/ break;
        case 0x51: set_texture ( x.first, x.second.value.as<texture_t>() ); /*------------*/ break;
        default  : throw except_t( "invalid uniform variable" ); /*-----------------------*/ break; 

    }}}

    void set_vertex_variables() const /*noexcept*/ {
    if ( obj->ctx.null() ) /*----------*/ { throw except_t("invalid shader"); }
    if ( !rl::IsShaderValid( *obj->ctx ) ){ throw except_t("Invalid Shader"); }
    for( auto x: obj->v_vars.data() )/*-*/{ switch( x.second.type ){

        case 0x04: set_variable( x.first, gpu::ATTRIBUTE_FLOAT, x.second.value.as<float>());   break;
        case 0x14: set_variable( x.first, gpu::ATTRIBUTE_VEC2 , x.second.value.as< vec2_t>()); break;
        case 0x24: set_variable( x.first, gpu::ATTRIBUTE_VEC3 , x.second.value.as< vec3_t>()); break;
        case 0x34: set_variable( x.first, gpu::ATTRIBUTE_VEC4 , x.second.value.as< vec4_t>()); break;
        default  : throw except_t( "invalid vertex variable" ); /*--------------------------*/ break; 

    }}}

    /*─······································································─*/

    string_t format_framge_shader() const noexcept {
        if( obj->fs.empty() ) /*---*/ { return nullptr; }
        return regex::format( "${0}\n${1}", GLSL_VERSION, obj->fs );
    }

    string_t format_vertex_shader() const noexcept {
        if( obj->vs.empty() ) /*---*/ { return nullptr; }
        return regex::format( "${0}\n${1}", GLSL_VERSION, obj->vs );
    }

    void compile_shader() /*const noexcept*/ {
        if(!obj->ctx.null() ){ rl::UnloadShader( *obj->ctx ); }

        obj->ctx = type::bind( rl::LoadShaderFromMemory( 
            format_framge_shader().get(),
            format_vertex_shader().get()
        ));
        
        if( !is_valid() ){ throw except_t("Invalid Shader"); }
    }

public:

    shader_t() noexcept : obj( new NODE() ) { /*--------------*/ }
    virtual ~shader_t() { if( obj.count()>1 ){ return; } free(); }

    /*─······································································─*/

    void set_fragment_shader( string_t code ) const noexcept { obj->fs=code; }

    template< class T >
    void append_uniform( string_t name, T value ) const noexcept {
         obj->f_vars[name] = SVAR({ gpu::gpu_type_id<T>::value, value });
    }

    void remove_uniform( string_t name ) const noexcept {
         obj->f_vars.erase(name);
    }

    /*─······································································─*/

    void set_vertex_shader( string_t code ) const noexcept { obj->vs=code; }

    template< class T >
    void append_attribute( string_t name, T value ) const noexcept {
         obj->v_vars[name] = SVAR({ gpu::gpu_type_id<T>::value, value });
    }

    void remove_attribute( string_t name ) const noexcept {
         obj->v_vars.erase(name);
    }

    /*─······································································─*/

    bool is_valid() const noexcept { return rl::IsShaderValid( *obj->ctx ); }

    rl::Shader* operator->() const noexcept { return &get(); }

    rl::Shader& get() const noexcept { return *obj->ctx; }

    /*─······································································─*/

    void emit( function_t<void> cb ) /*const noexcept*/ {

        if( obj->ctx.null() ){ compile_shader(); }

        rl::BeginShaderMode(*obj->ctx); set_vertex_variables  ();
        /*---------------------------*/ set_fragment_variables(); cb();
        rl::EndShaderMode();

    }

    /*─······································································─*/

    void free() const noexcept {
         if( !is_valid() ){ return; } rl::UnloadShader( *obj->ctx );
    }

}; }

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/