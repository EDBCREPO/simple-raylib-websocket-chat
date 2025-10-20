/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_IMPORT
#define UNGINE_IMPORT

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>
#include <nodepp/object.h>
#include <nodepp/event.h>
#include <nodepp/timer.h>
#include <nodepp/path.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { using namespace nodepp; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace rl {
    #include <raylib/raylib.h>
    #include <raylib/raygl.h>
    #include <raylib/raygui.h>
    #include <raylib/raymath.h>
}}

/*────────────────────────────────────────────────────────────────────────────*/

#include "struct.h"

#include "global.h"
#include "lock.h"
#include "math.h"
#include "struct.h"

#include "texture.h"
#include "model.h"
#include "image.h"
#include "sound.h"
#include "mesh.h"
#include "gui.h"
#include "vr.h"
#include "shader.h"

#include "input.h"
#include "window.h"
#include "engine.h"

#include "node.h"
#include "camera.h"
#include "render.h"
#include "collision.h"

#include "scene.h"

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
