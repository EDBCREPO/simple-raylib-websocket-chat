/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_VR_MODE
#define UNGINE_VR_MODE

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class vr_t : public global_t {
protected:

    struct NODE { 
        rl::VrDeviceInfo   device;
        rl::VrStereoConfig config;
        bool updated; /*--------*/
    };  ptr_t<NODE> obj;

public:

    /*─······································································─*/

    virtual ~vr_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    /*----*/ vr_t() noexcept : global_t(), obj( new NODE() ){

        obj->device.hResolution             = 2160;      // Horizontal resolution in pixels
        obj->device.vResolution             = 1200;      // Vertical resolution in pixels
        obj->device.hScreenSize             = 0.133793f; // Horizontal size in meters
        obj->device.vScreenSize             = 0.0669f;   // Vertical size in meters

        obj->device.eyeToScreenDistance     = 0.041f;     // Distance between eye and display in meters
        obj->device.lensSeparationDistance  = 0.07f;      // Lens separation distance in meters
        obj->device.interpupillaryDistance  = 0.07f;      // IPD (distance between pupils) in meters

        obj->device.lensDistortionValues[0] = 1.0f;       // Lens distortion constant parameter 0
        obj->device.lensDistortionValues[1] = 0.22f;      // Lens distortion constant parameter 1
        obj->device.lensDistortionValues[2] = 0.24f;      // Lens distortion constant parameter 2
        obj->device.lensDistortionValues[3] = 0.0f;       // Lens distortion constant parameter 3

        obj->device.chromaAbCorrection[0]   = 0.996f;      // Chromatic aberration correction parameter 0
        obj->device.chromaAbCorrection[1]   = -0.004f;     // Chromatic aberration correction parameter 1
        obj->device.chromaAbCorrection[2]   = 1.014f;      // Chromatic aberration correction parameter 2
        obj->device.chromaAbCorrection[3]   = 0.0f;        // Chromatic aberration correction parameter 3

    update_device(); }

    vr_t( rl::VrDeviceInfo device ) noexcept : global_t(), obj( new NODE() ){
          free(); obj->device = device; obj->updated=true; update_device();    
    }

    /*─······································································─*/

    rl::VrStereoConfig& device() const noexcept { return obj->config; }

    rl::VrDeviceInfo* operator->() const noexcept { return &get(); }

    rl::VrDeviceInfo& get() const noexcept { return obj->device; }

    /*─······································································─*/

    void update_device() const noexcept { 
         obj->config = rl::LoadVrStereoConfig( obj->device ); 
    }

    void emit( function_t<void> cb ) const noexcept {
         rl::BeginVrStereoMode( obj->config ); cb();
         rl::EndVrStereoMode  (); /*--------------*/
    }

    /*─······································································─*/

    void free() const noexcept { 
         if( !obj->updated ){ return; }
         rl::UnloadVrStereoConfig( obj->config ); 
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
