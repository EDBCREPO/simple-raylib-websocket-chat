/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_MODEL
#define UNGINE_MODEL

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { class model_t : public global_t {
protected:

    struct NODE { rl::Model mdl; }; ptr_t<NODE> obj;

public:

    /*----*/ model_t() noexcept : global_t(), obj( new NODE() ){ /*---*/ }
    virtual ~model_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    /*─······································································─*/

    model_t( string_t path ) noexcept : global_t(), obj( new NODE() ) {
        obj->mdl = rl::LoadModel( path.get() );
    }

    /*─······································································─*/

    bool is_valid() const noexcept { return rl::IsModelValid( obj->mdl ); }

    rl::Model* operator->() const noexcept { return &get(); }

    rl::Model& get() const noexcept { return obj->mdl; }

    /*─······································································─*/

    void draw( transform_3D_t trn ) const noexcept {
    rl::rlDisableBackfaceCulling();

        auto rot = rl::QuaternionFromEuler(
             trn.translate.rotation.x,
             trn.translate.rotation.y,
             trn.translate.rotation.z
        );
        
        auto scl = trn.translate.scale;
        auto pos = trn.translate.position;

        vec3_t axs ({ 0.0f, 0.0f, 0.0f }); float ang = 0.0f;

        rl::QuaternionToAxisAngle( rot, &axs, &ang );
        rl::DrawModelEx( obj->mdl, pos, axs, ang*RAD2DEG, scl, rl::WHITE );

    rl::rlEnableBackfaceCulling();
    }

    void draw_edges( transform_3D_t trn ) const noexcept {
    rl::rlDisableBackfaceCulling();

        auto rot = rl::QuaternionFromEuler(
             trn.translate.rotation.x,
             trn.translate.rotation.y,
             trn.translate.rotation.z
        );
        
        auto scl = trn.translate.scale;
        auto pos = trn.translate.position;

        vec3_t axs ({ 0.0f, 0.0f, 0.0f }); float ang = 0.0f;

        rl::QuaternionToAxisAngle( rot, &axs, &ang );
        rl::DrawModelWiresEx( obj->mdl, pos, axs, ang*RAD2DEG, scl, rl::WHITE );

    rl::rlEnableBackfaceCulling();
    }

    void draw_vertex( transform_3D_t trn ) const noexcept {
    rl::rlDisableBackfaceCulling();

        auto rot = rl::QuaternionFromEuler(
             trn.translate.rotation.x,
             trn.translate.rotation.y,
             trn.translate.rotation.z
        );
        
        auto scl = trn.translate.scale;
        auto pos = trn.translate.position;

        vec3_t axs ({ 0.0f, 0.0f, 0.0f }); float ang = 0.0f;

        rl::QuaternionToAxisAngle( rot, &axs, &ang );
        rl::DrawModelPointsEx( obj->mdl, pos, axs, ang*RAD2DEG, scl, rl::WHITE );

    rl::rlEnableBackfaceCulling();
    }

    /*─······································································─*/

    void free() const noexcept {
         if( !is_valid() ){ return; } rl::UnloadModel( obj->mdl );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
