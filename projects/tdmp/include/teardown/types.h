#ifndef TDMP_TEARDOWN_TYPES
#define TDMP_TEARDOWN_TYPES

#include "pch.h"
#include "teardown/containers/td_containers.h"
#include "teardown/td_script_core.h"

struct script_core_t;

namespace mp::teardown::types {

    // Forward some stuff
    //------------------------------------------------------------------------
    struct script_t;

    // Enums
    //------------------------------------------------------------------------
    enum class log_level : uint8_t {
        debug = 0,
        info = 1,
        warning = 2,
        error = 3
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h & me
    enum class game_state : int {
        none,
        splash,
        menu,
        ui,
        loading,
        menu_loading,
        play,
        edit,
        quit
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    enum light_type : uint8_t {
        light_sphere = 1,
        capsule,
        light_cone,
        area,
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    enum trigger_type : int {
        trigger_sphere = 1,
        trigger_box,
        trigger_polygon,
    };

    // General Structures
    //------------------------------------------------------------------------
    struct vector2_t { float x, y; };
    struct vector3_t { float x, y, z; };
    struct quat_t    { float x, y, z, w; };
    struct rgba_t    { float r, g, b, a; };

    struct transform_t {
        vector3_t pos;
        quat_t    rot;
    };

    // Paste generated structures here
    struct vehicle_t {
        uint8_t padding_0[12]; // 0x0
        int32_t handle; // 0xc
    }; // Size: 0x10

    struct shape_t {
        uint8_t padding_0[12]; // 0x0
        int32_t handle; // 0xc
    }; // Size: 0x10

    // This is actually part of player
    struct something {
        uint8_t padding_0[777]; // 0x0
        bool respawnFlag; // 0x309
    }; // Size: 0x30a


    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct entity_t {
        void* class_ptr;
        uint8_t type;
        uint16_t flags;
        uint32_t handle;	// 0x0C
        entity_t* parent;
        entity_t* sibling;
        entity_t* unk_tree;	// the holy spirit?
        entity_t* child;
        uint8_t padding[0x10];
    }; // 0x40

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class body_t : public entity_t {
    public:
        types::transform_t transform;
        uint8_t padding1[0x7C];
        types::vector3_t velocity;				// 0xD8
        types::vector3_t angular_velocity;		// 0xE4
        uint8_t padding2[0x48];
        bool dynamic;				// 0x138
        uint8_t padding3[7];
        uint8_t active;				// 0x140
        float friction;
        float restitution;
        uint8_t friction_mode;
        uint8_t restitution_mode;
    }; // 0x170

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class light_t : public entity_t {
    public:
        bool enabled;
        uint8_t padding1[3];
        light_type type;			// 0x44
        types::transform_t transform;
        uint8_t padding2[0x44];
        types::rgba_t color;				// 0xA8
        float scale;
        float reach;
        float radius;
        float unshadowed;
        float cos_half_angle_rad;
        float penumbra;
        float fogiter;
        float fogscale;
        float half_width;
        float half_height;
        float half_length;
        float glare;			// 0xE4
        uint8_t padding3[4];
        types::vector3_t position;			// 0xEC
        uint8_t index;
        float flickering;
        //td_string sound_path;
        //float sound_volume;
    }; // 0x1450

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class water_t : public entity_t {
    public:
        types::transform_t transform;
        float depth;
        td::td_vector<types::vector2_t> vertices;	// 0x60
        uint8_t padding1[0x388];	// the mariana trench?
        float wave;					// 0x3F8
        float ripple;
        float motion;
        float foam;
        float visibility;
        types::rgba_t color;
    }; // 0x420

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class wheel_t : public entity_t {
    public:
        vehicle_t* _vehicle;
        body_t* vehicleBody;
        body_t* body;
        shape_t* shape;
        shape_t* ground_shape;
        uint32_t ground_voxel_pos[3];
        bool on_ground;
        types::transform_t transform;
        types::transform_t transform2;
        float steer;
        float drive;
        float travel_up;
        float travel_down;
        float radius;
        float width;
        float stance;			// 0xC8
        uint8_t padding1[0xC];
        float angular_speed;	// 0xD8
        uint8_t padding2[0x24];
        float vertical_offset;	// 0x100
    }; // 0x118

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class screen_t : public entity_t {
    public:
        types::transform_t transform;
        types::vector2_t size;
        float bulge;
        uint32_t resolution_x;
        uint32_t resolution_y;
        td::td_string script;		// 0x70
        uint8_t padding[0x20];
        bool enabled;			// 0xB0
        bool interactive;
        float emissive;
        float fxraster;
        float fxca;
        float fxnoise;
        float fxglitch;
    }; // 0x2198

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class trigger_t : public entity_t {
    public:
        types::transform_t transform;
        trigger_type type;
        float sphere_size;
        types::vector3_t half_box_size;
        td::td_vector<types::vector2_t> vertices;	// 0x70
        uint8_t padding1[0x80];
        float polygon_size;			// 0x100
        uint8_t padding2[4];
        td::td_string sound_path;		// 0x108
        float sound_ramp;
        uint8_t sound_type;
        float sound_volume;
    }; // 0x138

    enum joint_type : int {
        Ball = 1,
        Hinge,
        Prismatic,
        JointRope,
        JointCone,
    }; // 0x04

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct rope {
        float zero;
        float segment_length;
        float slack;
        types::rgba_t color;			// 0x0C
        uint8_t padding1[4];
        float strength;		// 0x20
        float maxstretch;
        uint8_t padding2[5];
        uint8_t active;		// 0x2D
        uint32_t segments_count;
    }; // ???

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class joint_t : public entity_t {
    public:
        shape_t* shape1;
        shape_t* shape2;		// 0x48
        uint8_t padding1[0x10];
        joint_type type;		// 0x60
        float size;
        bool collide;
        bool connected;
        float rotstrength;
        float rotspring;	// 0x70
        uint8_t padding2[0xC];
        types::vector3_t position1;		// 0x80
        types::vector3_t position2;
        types::vector3_t axis1;
        types::vector3_t axis2;
        types::quat_t hinge_rot;
        float limits[2];	// 0xC0
        uint8_t padding3[4];
        float max_velocity;	// 0xCC
        float strength;		// 0xD0
        uint8_t padding4[4];
        rope* _rope;			// 0xD8
        bool sound;
        bool autodisable;
        float connection_strength;
        float disconnect_dist;
    }; // 0xF0


    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct fire_t {
        shape_t* shape;
        types::vector3_t position;
        float max_time;
        float time;
        bool painted;
        bool broken;
        uint32_t spawned_count;
        uint8_t padding1[0x24];
    }; // 0x48

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct environment {}; // ??
    struct fire_system_t {
        uint8_t padding[8];
        td::td_vector<fire_t> fires;
    };

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class animator_core {}; // 0x120
    class animator_t : public entity_t {
    public:
        types::transform_t transform;
        uint8_t padding[4];
        animator_core* anim_core;
        td::td_string name;
        td::td_string path;
    }; // 0xB0

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    class location_t : public entity_t {
        types::transform_t transform;
    }; // 0x60

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h
    struct scene_t {
        uint8_t padding1[0x38];
        fire_system_t* firesystem;
        td::td_vector<void*> projectiles;
        environment* environment;
        uint8_t padding2[8];
        types::transform_t spawnpoint;		        // 0x60
        uint8_t padding3[4];
        light_t* flashlight;				        // 0x80
        script_t* explosion_lua;
        script_t* achievements_lua;
        script_t* characters_lua;			        // 0x98
        uint8_t padding4[0x50];
        types::vector3_t sv_size;			        // 0xF0
        uint8_t padding5[0x4C];
        td::td_vector<body_t*> bodies;                // 0x148
        td::td_vector<shape_t*> shapes;               // 0x158
        td::td_vector<light_t*> lights;               // 0x168
        td::td_vector<location_t*> locations;         // 0x178
        td::td_vector<water_t*> waters;               // 0x188
        td::td_vector<joint_t*> joints;               // 0x198
        td::td_vector<vehicle_t*> vehicles;           // 0x1A8
        td::td_vector<wheel_t*> wheels;               // 0x1B8
        td::td_vector<screen_t*> screens;             // 0x1C8
        td::td_vector<trigger_t*> triggers;           // 0x1D8
        td::td_vector<script_t*> scripts;             // 0x1E8
        td::td_vector<animator_t*> animators;         // 0x1F8
        td::td_vector<entity_t*> top_level;           // 0x208
        uint8_t padding6[0x378];                    
        td::td_vector<types::vector2_t> boundary;		// 0x590
        uint8_t padding7[0x428];
        td::td_vector<entity_t*> entities;	        // 0x9C8
    }; // 0xA70

    // Generated structures
    //------------------------------------------------------------------------
    struct player_t {
        teardown::types::vector3_t pos;      // 0x0
        teardown::types::quat_t rot;
        //uint8_t padding_12[128];             // 0xc
        uint8_t padding_12[112];             // 0xc
        teardown::types::vector3_t velocity; // 0x8c
        uint8_t padding_152[284];            // 0x98
        float walkingSpeed;                  // 0x1b4
        uint8_t padding_440[8];              // 0x1b8
        body_t* grabbedBody;                 // 0x1c0
        uint8_t padding_456[64];             // 0x1c8
        float health;                        // 0x208
        uint8_t padding_524[3188];           // 0x20c
        bool isGrounded;                     // 0xe80
        uint8_t padding_3713[175];           // 0xe81
        shape_t* grabbedShape;               // 0xf30
    }; // Size: 0xf38

    // Script Stuff
    //------------------------------------------------------------------------

    struct script_t : public entity_t {
        td::td_string scriptName;
        td::td_string scriptPath;
        td::script_core_t scriptCore;
    };

    struct game_t {
        uint32_t dword0;
        uint32_t dword4;
        game_state state;
        float splashProgress;
        uint16_t word14;
        uint64_t qword18;
        uint8_t byte20;
        uint8_t gap21[7];
        uint64_t qword28;
        uint64_t qword30;
        uint64_t gameInput;
        uint64_t qword40;
        uint64_t qword48;
        scene_t* scene;
        uint64_t qword58;
        uint64_t qword60;
        uint64_t editor;
        uint64_t qword70;
        uint64_t hud0;
        uint64_t hud1;
        uint64_t hud2;
        uint64_t hud3;
        uint64_t hud4;
        uint64_t qwordA0;
        uint64_t qwordA8;
        uint64_t qwordB0;
        player_t* player;
        uint64_t qwordC0;
        uint64_t qwordC8;
        uint8_t** registry;
        uint64_t qwordD8;
        uint64_t qwordE0;
        uint64_t qwordE8;
        uint64_t qwordF0;
        uint64_t qwordF8;
        uint64_t qword100;
        uint64_t qword108;
        uint64_t qword110;
        uint64_t qword118;
        uint64_t qword120;
        uint64_t qword128;
        uint64_t qword130;
        uint64_t qword138;
        uint64_t qword140;
        uint32_t dword148;
        uint8_t gap14C[4];
        uint64_t qword150;
        uint32_t dword158;
        uint8_t gap15C[4];
        char char160;
        uint8_t gap161[31];
        char char180;
        uint8_t gap181[31];
        uint32_t dword1A0;
        uint16_t word1A4;
        uint64_t qword1A8;
        uint32_t dword1B0;
        uint64_t qword1B4;
        uint64_t qword1BC;
        uint64_t qword1C4;
        uint64_t qword1CC;
        uint8_t gap1D4[20];
        uint8_t byte1E8;
        uint64_t qword1EC;
        uint32_t dword1F4;
        uint8_t gap1F8[272];
        uint32_t dword308;
        uint64_t qword30C;
        uint64_t qword314;
        uint64_t qword31C;
        uint32_t dword324;
        uint32_t dword328;
        uint32_t dword32C;
        uint32_t dword330;
        uint32_t dword334;
        uint32_t dword338;
        uint32_t dword33C;
        uint32_t dword340;
        uint32_t dword344;
        uint32_t dword348;
        uint32_t dword34C;
        uint32_t dword350;
        uint32_t dword354;
        uint32_t dword358;
        uint32_t dword35C;
        uint64_t qword360;
        uint32_t dword368;
        uint32_t dword36C;
        uint64_t qword370;
        uint32_t dword378;
        uint32_t dword37C;
        uint8_t gap380[576];
        uint8_t byte5C0;
        uint8_t gap5C1[31];
        uint8_t byte5E0;
        uint8_t gap5E1[255];
        char char6E0;
        uint8_t gap6E1[31];
        uint8_t byte700;
        uint8_t gap701[7];
        char char708;
        uint8_t gap709[31];
        uint64_t qword728;
        uint8_t byte730;
        uint8_t gap731[7];
        uint64_t qword738;
        uint64_t qword740;
        uint64_t qword748;
        uint64_t qword750;
        uint8_t gap758[16];
        uint64_t qword768;
        uint64_t qword770;
        uint16_t word778;
        uint8_t byte77A;
        uint32_t dword77C;
        char char780;
        uint8_t gap781[31];
        uint8_t byte7A0;
        uint8_t gap7A1[7];
        uint64_t qword7A8;
        uint64_t qword7B0;
        uint32_t dword7B8;
        uint8_t byte7BC;
        uint64_t qword7C0;
        uint32_t dword7C8;
    };

}

#endif // TDMP_TEARDOWN_TYPES