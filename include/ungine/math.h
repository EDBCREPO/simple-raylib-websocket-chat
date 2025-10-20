#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math { namespace quaternion {

    vec4_t from_vec3_vec3( vec3_t from, vec3_t to ) {
        return rl::QuaternionFromVector3ToVector3( from, to );
    }

    vec4_t from_axis( type::pair<vec3_t,float> input ) {
        return rl::QuaternionFromAxisAngle( 
            input.first, input.second 
        );
    }

    vec4_t from_matrix( mat_t input ) {
        return rl::QuaternionFromMatrix( input );
    }

    vec4_t from_euler( vec3_t input ) {
        return rl::QuaternionFromEuler(
            input.x, input.y, input.z
        );
    }

    vec3_t to_euler( vec4_t input ) {
    vec3_t out = rl::QuaternionToEuler(input);
        return vec3_t({ out.x, out.y, out.z });
    }

    mat_t to_matrix( vec4_t input ) {
        return rl::QuaternionToMatrix( input );
    }

    type::pair<vec3_t,float> to_axis( vec4_t input ) {
    type::pair<vec3_t,float> out = {0}; /*-----------------------*/
        rl::QuaternionToAxisAngle( input, &out.first, &out.second );
        return out;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math { namespace vec3 {

    vec3_t rotation( vec3_t origin, type::pair<vec3_t,float> axis ) {
        return rl::Vector3RotateByAxisAngle( origin, axis.first, axis.second );
    }

    vec3_t rotation( vec3_t origin, vec4_t quaternion ) {
        return rl::Vector3RotateByQuaternion( origin, quaternion );
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math { namespace matrix {

    mat_t rotation( vec4_t input ) {
        return rl::MatrixRotateXYZ( math::quaternion::to_euler( input ) );
    }

    mat_t rotation( type::pair<vec3_t,float> input ) {
        return rl::MatrixRotate( input.first, input.second );
    }

    mat_t rotation( vec3_t input ) {
        return rl::MatrixRotateXYZ( input );
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t normalize( vec4_t value ){ return rl::Vector4Normalize( value ); }

    vec3_t normalize( vec3_t value ){ return rl::Vector3Normalize( value ); }

    vec2_t normalize( vec2_t value ){ return rl::Vector2Normalize( value ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t negate( vec4_t value ){ return rl::Vector4Negate( value ); }

    vec3_t negate( vec3_t value ){ return rl::Vector3Negate( value ); }

    vec2_t negate( vec2_t value ){ return rl::Vector2Negate( value ); }

    float  negate( float  value ){ return -value; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t move_toward( vec4_t from, vec4_t to, float distance ){
        return rl::Vector4MoveTowards( from, to, distance ); 
    }

    vec3_t move_toward( vec3_t from, vec3_t to, float distance ){
        return rl::Vector3MoveTowards( from, to, distance ); 
    }

    vec2_t move_toward( vec2_t from, vec2_t to, float distance ){
        return rl::Vector2MoveTowards( from, to, distance ); 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math { namespace quaternion {

    vec4_t multiply( vec4_t a, vec4_t b ) {
        return rl::QuaternionMultiply( a, b );
    }

    vec4_t normalize( vec4_t a ) {
        return rl::QuaternionNormalize( a );
    }

    vec4_t add( vec4_t a, vec4_t b ) {
        return rl::QuaternionAdd( a, b );
    }

    vec4_t negate( vec4_t a ) {
        return rl::Vector4Negate( a );
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math { namespace matrix {

    mat_t from_vec3_vec3( vec3_t from, vec3_t to ) {
        vec4_t out = math::quaternion::from_vec3_vec3( from, to );
        return /*-*/ math::quaternion::to_matrix( out );
    }

    mat_t from_axis( type::pair<vec3_t,float> input ) {
        vec4_t out = math::quaternion::from_axis( input );
        return /*-*/ math::quaternion::to_matrix( out );
    }

    mat_t from_euler( vec3_t input ) {
        vec4_t out = math::quaternion::from_euler( input );
        return /*-*/ math::quaternion::to_matrix ( out );
    }

    mat_t from_quaternion( vec4_t input ) {
        return math::quaternion::to_matrix( input );
    }

    vec4_t to_quaternion( mat_t input ) {
        return math::quaternion::from_matrix( input );
    }

    vec3_t to_euler( mat_t input ) {
        vec4_t out = math::quaternion::from_matrix(input);
        return /*-*/ math::quaternion::to_euler   ( out );
    }

    type::pair<vec3_t,float> to_axis( mat_t input ) {
        vec4_t out = math::quaternion::from_matrix(input);
        return /*-*/ math::quaternion::to_axis    ( out );
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math { namespace euler {

    vec3_t from_vec3_vec3( vec3_t from, vec3_t to ) {
        vec4_t out = math::quaternion::from_vec3_vec3( from, to );
        return /*-*/ math::quaternion::to_euler( out );
    }

    vec3_t from_axis( type::pair<vec3_t,float> input ) {
        vec4_t out = math::quaternion::from_axis( input );
        return /*-*/ math::quaternion::to_euler ( out );
    }

    vec3_t from_matrix( mat_t input ) {
        vec4_t out = math::quaternion::from_matrix( input );
        return /*-*/ math::quaternion::to_euler   ( out );
    }

    vec3_t from_quaternion( vec4_t input ) {
        return math::quaternion::to_euler( input );
    }

    vec4_t to_quaternion( vec3_t input ) {
        return math::quaternion::from_euler( input );
    }

    mat_t to_matrix( vec3_t input ) {
        vec4_t out = math::quaternion::from_euler(input);
        return /*-*/ math::quaternion::to_matrix ( out );
    }

    type::pair<vec3_t,float> to_axis( vec3_t input ) {
        vec4_t out = math::quaternion::from_euler(input);
        return /*-*/ math::quaternion::to_axis   ( out );
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t normalize( vec4_t value, float min, float max ){
           value.x = rl::Normalize( value.x, min, max );
           value.y = rl::Normalize( value.y, min, max );
           value.z = rl::Normalize( value.z, min, max );
           value.w = rl::Normalize( value.w, min, max );
    return value; }

    vec3_t normalize( vec3_t value, float min, float max ){
           value.x = rl::Normalize( value.x, min, max );
           value.y = rl::Normalize( value.y, min, max );
           value.z = rl::Normalize( value.z, min, max );
    return value; }

    vec2_t normalize( vec2_t value, float min, float max ){
           value.x = rl::Normalize( value.x, min, max );
           value.y = rl::Normalize( value.y, min, max );
    return value; }

    float normalize( float value, float min, float max ){
           value = rl::Normalize( value, min, max );
    return value; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t wrap( vec4_t value, float min, float max ){
           value.x = rl::Wrap( value.x, min, max );
           value.y = rl::Wrap( value.y, min, max );
           value.z = rl::Wrap( value.z, min, max );
           value.w = rl::Wrap( value.w, min, max );
    return value; }

    vec3_t wrap( vec3_t value, float min, float max ){
           value.x = rl::Wrap( value.x, min, max );
           value.y = rl::Wrap( value.y, min, max );
           value.z = rl::Wrap( value.z, min, max );
    return value; }

    vec2_t wrap( vec2_t value, float min, float max ){
           value.x = rl::Wrap( value.x, min, max );
           value.y = rl::Wrap( value.y, min, max );
    return value; }

    float wrap( float value, float min, float max ){
           value = rl::Wrap( value, min, max );
    return value; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t lerp( vec4_t value, float min, float max ){
           value.x = rl::Lerp( value.x, min, max );
           value.y = rl::Lerp( value.y, min, max );
           value.z = rl::Lerp( value.z, min, max );
           value.w = rl::Lerp( value.w, min, max );
    return value; }

    vec3_t lerp( vec3_t value, float min, float max ){
           value.x = rl::Lerp( value.x, min, max );
           value.y = rl::Lerp( value.y, min, max );
           value.z = rl::Lerp( value.z, min, max );
    return value; }

    vec2_t lerp( vec2_t value, float min, float max ){
           value.x = rl::Lerp( value.x, min, max );
           value.y = rl::Lerp( value.y, min, max );
    return value; }

    float lerp( float value, float min, float max ){
           value = rl::Lerp( value, min, max );
    return value; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace math {

    vec4_t clamp( vec4_t value, float min, float max ){
           value.x = rl::Clamp( value.x, min, max );
           value.y = rl::Clamp( value.y, min, max );
           value.z = rl::Clamp( value.z, min, max );
           value.w = rl::Clamp( value.w, min, max );
    return value; }

    vec3_t clamp( vec3_t value, float min, float max ){
           value.x = rl::Clamp( value.x, min, max );
           value.y = rl::Clamp( value.y, min, max );
           value.z = rl::Clamp( value.z, min, max );
    return value; }

    vec2_t clamp( vec2_t value, float min, float max ){
           value.x = rl::Clamp( value.x, min, max );
           value.y = rl::Clamp( value.y, min, max );
    return value; }

    float clamp( float value, float min, float max ){
           value = rl::Clamp( value, min, max );
    return value; }

}}

/*────────────────────────────────────────────────────────────────────────────*/