/*
 * Copyright 2023 The Ungine Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/UngineOfficial/Ungine/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef UNGINE_STRUCT
#define UNGINE_STRUCT

namespace ungine { namespace key { enum BUTTON {
    KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE      = 39,       // Key: '
    KEY_COMMA           = 44,       // Key: ,
    KEY_MINUS           = 45,       // Key: -
    KEY_PERIOD          = 46,       // Key: .
    KEY_SLASH           = 47,       // Key: /
    KEY_ZERO            = 48,       // Key: 0
    KEY_ONE             = 49,       // Key: 1
    KEY_TWO             = 50,       // Key: 2
    KEY_THREE           = 51,       // Key: 3
    KEY_FOUR            = 52,       // Key: 4
    KEY_FIVE            = 53,       // Key: 5
    KEY_SIX             = 54,       // Key: 6
    KEY_SEVEN           = 55,       // Key: 7
    KEY_EIGHT           = 56,       // Key: 8
    KEY_NINE            = 57,       // Key: 9
    KEY_SEMICOLON       = 59,       // Key: ;
    KEY_EQUAL           = 61,       // Key: =
    KEY_A               = 65,       // Key: A | a
    KEY_B               = 66,       // Key: B | b
    KEY_C               = 67,       // Key: C | c
    KEY_D               = 68,       // Key: D | d
    KEY_E               = 69,       // Key: E | e
    KEY_F               = 70,       // Key: F | f
    KEY_G               = 71,       // Key: G | g
    KEY_H               = 72,       // Key: H | h
    KEY_I               = 73,       // Key: I | i
    KEY_J               = 74,       // Key: J | j
    KEY_K               = 75,       // Key: K | k
    KEY_L               = 76,       // Key: L | l
    KEY_M               = 77,       // Key: M | m
    KEY_N               = 78,       // Key: N | n
    KEY_O               = 79,       // Key: O | o
    KEY_P               = 80,       // Key: P | p
    KEY_Q               = 81,       // Key: Q | q
    KEY_R               = 82,       // Key: R | r
    KEY_S               = 83,       // Key: S | s
    KEY_T               = 84,       // Key: T | t
    KEY_U               = 85,       // Key: U | u
    KEY_V               = 86,       // Key: V | v
    KEY_W               = 87,       // Key: W | w
    KEY_X               = 88,       // Key: X | x
    KEY_Y               = 89,       // Key: Y | y
    KEY_Z               = 90,       // Key: Z | z
    KEY_LEFT_BRACKET    = 91,       // Key: [
    KEY_BACKSLASH       = 92,       // Key: '\'
    KEY_RIGHT_BRACKET   = 93,       // Key: ]
    KEY_GRAVE           = 96,       // Key: `
    // Function keys
    KEY_SPACE           = 32,       // Key: Space
    KEY_ESCAPE          = 256,      // Key: Esc
    KEY_ENTER           = 257,      // Key: Enter
    KEY_TAB             = 258,      // Key: Tab
    KEY_BACKSPACE       = 259,      // Key: Backspace
    KEY_INSERT          = 260,      // Key: Ins
    KEY_DELETE          = 261,      // Key: Del
    KEY_RIGHT           = 262,      // Key: Cursor right
    KEY_LEFT            = 263,      // Key: Cursor left
    KEY_DOWN            = 264,      // Key: Cursor down
    KEY_UP              = 265,      // Key: Cursor up
    KEY_PAGE_UP         = 266,      // Key: Page up
    KEY_PAGE_DOWN       = 267,      // Key: Page down
    KEY_HOME            = 268,      // Key: Home
    KEY_END             = 269,      // Key: End
    KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    KEY_NUM_LOCK        = 282,      // Key: Num lock
    KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    KEY_PAUSE           = 284,      // Key: Pause
    KEY_F1              = 290,      // Key: F1
    KEY_F2              = 291,      // Key: F2
    KEY_F3              = 292,      // Key: F3
    KEY_F4              = 293,      // Key: F4
    KEY_F5              = 294,      // Key: F5
    KEY_F6              = 295,      // Key: F6
    KEY_F7              = 296,      // Key: F7
    KEY_F8              = 297,      // Key: F8
    KEY_F9              = 298,      // Key: F9
    KEY_F10             = 299,      // Key: F10
    KEY_F11             = 300,      // Key: F11
    KEY_F12             = 301,      // Key: F12
    KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    KEY_LEFT_CONTROL    = 341,      // Key: Control left
    KEY_LEFT_ALT        = 342,      // Key: Alt left
    KEY_LEFT_SUPER      = 343,      // Key: Super left
    KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    KEY_RIGHT_ALT       = 346,      // Key: Alt right
    KEY_RIGHT_SUPER     = 347,      // Key: Super right
    KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = 320,      // Key: Keypad 0
    KEY_KP_1            = 321,      // Key: Keypad 1
    KEY_KP_2            = 322,      // Key: Keypad 2
    KEY_KP_3            = 323,      // Key: Keypad 3
    KEY_KP_4            = 324,      // Key: Keypad 4
    KEY_KP_5            = 325,      // Key: Keypad 5
    KEY_KP_6            = 326,      // Key: Keypad 6
    KEY_KP_7            = 327,      // Key: Keypad 7
    KEY_KP_8            = 328,      // Key: Keypad 8
    KEY_KP_9            = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    KEY_KP_ADD          = 334,      // Key: Keypad +
    KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = 4,        // Key: Android back button
    KEY_MENU            = 5,        // Key: Android menu button
    KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace mouse { enum BUTTON {
    MOUSE_BUTTON_LEFT    = 0, // Mouse button left
    MOUSE_BUTTON_RIGHT   = 1, // Mouse button right
    MOUSE_BUTTON_MIDDLE  = 2, // Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE    = 3, // Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA   = 4, // Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = 5, // Mouse button forward (advanced mouse device)
    MOUSE_BUTTON_BACK    = 6, // Mouse button back (advanced mouse device)
};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace blend { enum MODE {

    BLEND_ALPHA= 0,           // Blend textures considering alpha (default)
    BLEND_ADDITIVE,           // Blend textures adding colors
    BLEND_MULTIPLIED,         // Blend textures multiplying colors
    BLEND_ADD_COLORS,         // Blend textures adding colors (alternative)
    BLEND_SUBTRACT_COLORS,    // Blend textures subtracting colors (alternative)
    BLEND_ALPHA_PREMULTIPLY,  // Blend premultiplied textures considering alpha
    BLEND_CUSTOM,             // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    BLEND_CUSTOM_SEPARATE     // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())

};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace window { enum FLAGS {

    WINDOW_VSYNC_HINT        = 0x00000040, // Set to try enabling V-Sync on GPU
    WINDOW_FULLSCREEN        = 0x00000002, // Set to run program in fullscreen
    WINDOW_RESIZABLE         = 0x00000004, // Set to allow resizable window
    WINDOW_UNDECORATED       = 0x00000008, // Set to disable window decoration (frame and buttons)
    WINDOW_HIDDEN            = 0x00000080, // Set to hide window
    WINDOW_MINIMIZED         = 0x00000200, // Set to minimize window (iconify)
    WINDOW_MAXIMIZED         = 0x00000400, // Set to maximize window (expanded to monitor)
    WINDOW_UNFOCUSED         = 0x00000800, // Set to window non focused
    WINDOW_TOPMOST           = 0x00001000, // Set to window always on top
    WINDOW_ALWAYS_RUN        = 0x00000100, // Set to allow windows running while minimized
    WINDOW_TRANSPARENT       = 0x00000010, // Set to allow transparent framebuffer
    WINDOW_HIGHDPI           = 0x00002000, // Set to support HighDPI
    WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when WINDOW_UNDECORATED
    WINDOW_BORDERLESS        = 0x00008000, // Set to run program in borderless windowed mode
    WINDOW_MSAA_4X_HINT      = 0x00000020, // Set to try enabling MSAA 4X
    WINDOW_INTERLACED_HINT   = 0x00010000  // Set to try enabling interlaced video format (for V3D)

};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace texture { enum FILTER {

    FILTER_TEXTURE_POINT=0,         // No filter, just pixel approximation
    FILTER_TEXTURE_BILINEAR,        // Linear filtering
    FILTER_TEXTURE_TRILINEAR,       // Trilinear filtering (linear with mipmaps)
    FILTER_TEXTURE_ANISOTROPIC_4X,  // Anisotropic filtering 4x
    FILTER_TEXTURE_ANISOTROPIC_8X,  // Anisotropic filtering 8x
    FILTER_TEXTURE_ANISOTROPIC_16X, // Anisotropic filtering 16x

};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace image { enum FORMAT {

    FORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    FORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    FORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    FORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    FORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    FORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    FORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    FORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    FORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    FORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    FORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    FORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    FORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    FORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    FORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    FORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    FORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    FORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    FORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    FORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    FORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    FORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    FORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    FORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp

};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace gpu { enum UNIFORM_TYPE {

    UNIFORM_BOOL  = rl::RL_SHADER_UNIFORM_INT  ,
    UNIFORM_INT   = rl::RL_SHADER_UNIFORM_INT  ,
    UNIFORM_UINT  = rl::RL_SHADER_UNIFORM_UINT ,
    UNIFORM_FLOAT = rl::RL_SHADER_UNIFORM_FLOAT,
    
    UNIFORM_BVEC2 = rl::RL_SHADER_UNIFORM_IVEC2,
    UNIFORM_IVEC2 = rl::RL_SHADER_UNIFORM_IVEC2,
    UNIFORM_UVEC2 = rl::RL_SHADER_UNIFORM_UIVEC2,
    UNIFORM_VEC2  = rl::RL_SHADER_UNIFORM_VEC2 ,
    
    UNIFORM_BVEC3 = rl::RL_SHADER_UNIFORM_IVEC3,
    UNIFORM_IVEC3 = rl::RL_SHADER_UNIFORM_IVEC3,
    UNIFORM_UVEC3 = rl::RL_SHADER_UNIFORM_UIVEC3,
    UNIFORM_VEC3  = rl::RL_SHADER_UNIFORM_VEC3 ,
    
    UNIFORM_BVEC4 = rl::RL_SHADER_UNIFORM_IVEC4,
    UNIFORM_IVEC4 = rl::RL_SHADER_UNIFORM_IVEC4,
    UNIFORM_UVEC4 = rl::RL_SHADER_UNIFORM_UIVEC4,
    UNIFORM_VEC4  = rl::RL_SHADER_UNIFORM_VEC4 ,

    UNIFORM_IMG2D = rl::RL_SHADER_UNIFORM_SAMPLER2D

};}}

namespace ungine { namespace gpu { enum ATTR_TYPE {

    ATTRIBUTE_FLOAT = rl::SHADER_ATTRIB_FLOAT  ,
    ATTRIBUTE_VEC2  = rl::RL_SHADER_ATTRIB_VEC2 ,
    ATTRIBUTE_VEC3  = rl::RL_SHADER_ATTRIB_VEC3 ,
    ATTRIBUTE_VEC4  = rl::RL_SHADER_ATTRIB_VEC4 ,

};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace camera { enum VIEW {
    PROJECTION_PERSPECTIVE=0, // Perspective  projection
    PROJECTION_ORTHOGRAPHIC   // Orthographic projection
}; }}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace render { enum MASK {
    VISIBILITY_MASK_NONE = 0b00000000,
    VISIBILITY_MASK_1    = 0b00000001,
    VISIBILITY_MASK_2    = 0b00000010,
    VISIBILITY_MASK_3    = 0b00000100,
    VISIBILITY_MASK_4    = 0b00001000,
    VISIBILITY_MASK_5    = 0b00010000,
    VISIBILITY_MASK_6    = 0b00100000,
    VISIBILITY_MASK_7    = 0b01000000,
    VISIBILITY_MASK_8    = 0b10000000,
    VISIBILITY_MASK_ALL  = 0b11111111
};}}

namespace ungine { namespace render { enum MODE {
    RENDER_MODE_UI, RENDER_MODE_2D, RENDER_MODE_3D
};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine {

    using color_t = rl::Color;

    using   mat_t = rl::Matrix; 
    using  vec2_t = rl::Vector2;
    using  vec3_t = rl::Vector3;
    using  vec4_t = rl::Vector4;
    using  rect_t = rl::Rectangle;

    struct bvec2_t { bool  x; bool y; };
    struct ivec2_t {  int  x;  int y; };
    struct uvec2_t { uint  x; uint y; };

    struct bvec3_t { bool  x; bool y; bool z; };
    struct ivec3_t {  int  x;  int y;  int z; };
    struct uvec3_t { uint  x; uint y; uint z; };

    struct bvec4_t { bool  x; bool y; bool z; bool w; };
    struct ivec4_t {  int  x;  int y;  int z;  int w; };
    struct uvec4_t { uint  x; uint y; uint z; uint w; };

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { struct transform_3D_t {

    struct priv {
        vec3_t rotation = { 0, 0, 0 };
        vec3_t position = { 0, 0, 0 };
        vec3_t scale    = { 1, 1, 1 };
    } translate = {0};

    vec3_t rotation = { 0, 0, 0 };
    vec3_t position = { 0, 0, 0 };
    vec3_t scale    = { 1, 1, 1 };
    vec3_t origin   = { 0, 0, 0 };

};}

namespace ungine { struct transform_2D_t {
    
    struct priv {
        float  rotation = .0f;
        vec2_t position = { 0, 0 };
        vec2_t scale    = { 1, 1 };
    } translate = {0};
    
    float  rotation = .0f;
    vec2_t position = { 0, 0 };
    vec2_t scale    = { 1, 1 };
    vec2_t origin   = { 0, 0 };

};}

namespace ungine { 
    using render_2D_t    = rl::RenderTexture;
    using transform_UI_t = transform_2D_t; 
    using camera_2D_t    = rl::Camera2D; 
    using camera_3D_t    = rl::Camera3D;
}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { struct visibility_t {
    uint mask    = render::MASK::VISIBILITY_MASK_ALL;
    uint mode    = render::MODE::RENDER_MODE_UI;
    bool visible = true;
};}

namespace ungine { struct viewport_t {
    uint mask = render::MASK::VISIBILITY_MASK_ALL;
    color_t background = { 0, 0, 0, 255 };
    ref_t<camera_2D_t> camera2D; 
    ref_t<camera_3D_t> camera3D;
    ref_t<render_2D_t> render  ;
};}

namespace ungine { struct render_queue_t {
    queue_t<event_t<>> event3D;
    queue_t<event_t<>> event2D;
    queue_t<event_t<>> eventUI;
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/