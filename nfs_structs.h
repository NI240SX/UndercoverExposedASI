#pragma once

typedef unsigned int    undefined4;
typedef unsigned char    byte;

struct bVector3 {
    float x;
    float y;
    float z;
};
struct bVector4 {
    float x;
    float y;
    float z;
    float w;
};

struct LinkedNode {
    LinkedNode* previous;
    LinkedNode* next;
};

struct FETreeDataNode {
    DWORD vftable;
    LinkedNode nodeLinking_0x4;
    //    DWORD field2_0x8;
    DWORD field3_0xc;
    DWORD NameHash_0x10;
    DWORD field5_0x14;
    DWORD id_0x18;
    DWORD field7_0x1c;
    char* label_0x20;
    DWORD field9_0x24;
    char field_0x28;
    char field_0x29;
    char field_0x2a;
    char field_0x2b;
};
DWORD FETreeDataNode_vftable;

enum CarPartID {
    BADGING_BUMPER_SET_FRONT = 0x00,
    BADGING_BUMPER_SET_REAR = 0x01,
    BADGING_FENDER_FRONT_LEFT = 0x02,
    BADGING_FENDER_FRONT_RIGHT = 0x03,
    BADGING_FENDER_REAR_LEFT = 0x04,
    BADGING_FENDER_REAR_RIGHT = 0x05,
    BADGING_TRUNK = 0x06,
    BASE = 0x07,
    BODY = 0x08,
    BODY_DOORLINE = 0x09,
    BRAKE_CALIPER_FRONT = 0x0A,
    BRAKE_CALIPER_REAR = 0x0B,
    BRAKE_ROTOR_FRONT = 0x0C,
    BRAKE_ROTOR_REAR = 0x0D,
    BRAKELIGHT_GLASS_LEFT = 0x0E,
    BRAKELIGHT_GLASS_RIGHT = 0x0F,
    BRAKELIGHT_LEFT = 0x10,
    BRAKELIGHT_RIGHT = 0x11,
    BUMPER_FRONT = 0x12,
    BUMPER_FRONT_EXTRA = 0x13,
    BUMPER_REAR = 0x14,
    BUMPER_REAR_EXTRA = 0x15,
    CHASSIS = 0x16,
    DAMAGE_BODY = 0x17,
    DAMAGE_BRAKELIGHT_LEFT = 0x18,
    DAMAGE_BRAKELIGHT_RIGHT = 0x19,
    DAMAGE_BUMPER_FRONT = 0x1A,
    DAMAGE_BUMPER_REAR = 0x1B,
    DAMAGE_DOOR_LEFT = 0x1C,
    DAMAGE_DOOR_REAR_LEFT = 0x1D,
    DAMAGE_DOOR_REAR_RIGHT = 0x1E,
    DAMAGE_DOOR_RIGHT = 0x1F,
    DAMAGE_FENDER_FRONT_LEFT = 0x20,
    DAMAGE_FENDER_FRONT_RIGHT = 0x21,
    DAMAGE_HEADLIGHT_LEFT = 0x22,
    DAMAGE_HEADLIGHT_RIGHT = 0x23,
    DAMAGE_HOOD = 0x24,
    DAMAGE_SIDEMIRROR_LEFT = 0x25,
    DAMAGE_SIDEMIRROR_RIGHT = 0x26,
    DAMAGE_TRUNK = 0x27,
    DAMAGE_TRUNK911 = 0x28,
    DAMAGE_WIDEBODY = 0x29,
    DAMAGE_WIDEBODY_BUMPER_FRONT = 0x2A,
    DAMAGE_WIDEBODY_BUMPER_REAR = 0x2B,
    DAMAGE_WIDEBODY_DOOR_LEFT = 0x2C,
    DAMAGE_WIDEBODY_DOOR_REAR_LEFT = 0x2D,
    DAMAGE_WIDEBODY_DOOR_REAR_RIGHT = 0x2E,
    DAMAGE_WIDEBODY_DOOR_RIGHT = 0x2F,
    DAMAGE_WIDEBODY_FENDER_FRONT_LEFT = 0x30,
    DAMAGE_WIDEBODY_FENDER_FRONT_RIGHT = 0x31,
    DAMAGE_WINDOW_FRONT = 0x32,
    DAMAGE_WINDOW_FRONT_LEFT = 0x33,
    DAMAGE_WINDOW_FRONT_RIGHT = 0x34,
    DAMAGE_WINDOW_REAR = 0x35,
    DAMAGE_WINDOW_REAR_LEFT = 0x36,
    DAMAGE_WINDOW_REAR_RIGHT = 0x37,
    DECAL_FRONT_WINDOW = 0x38,
    DECAL_REAR_WINDOW = 0x39,
    DOOR_LEFT = 0x3A,
    DOOR_REAR_LEFT = 0x3B,
    DOOR_REAR_RIGHT = 0x3C,
    DOOR_RIGHT = 0x3D,
    DOORHANDLE_FRONT_LEFT = 0x3E,
    DOORHANDLE_FRONT_RIGHT = 0x3F,
    DOORHANDLE_REAR_LEFT = 0x40,
    DOORHANDLE_REAR_RIGHT = 0x41,
    DRIVER = 0x42,
    ENGINE = 0x43,
    EXHAUST = 0x44,
    EXHAUST_TIPS_CENTER = 0x45,
    EXHAUST_TIPS_LEFT = 0x46,
    EXHAUST_TIPS_RIGHT = 0x47,
    FENDER_FRONT_LEFT = 0x48,
    FENDER_FRONT_RIGHT = 0x49,
    HEADLIGHT_GLASS_LEFT = 0x4A,
    HEADLIGHT_GLASS_RIGHT = 0x4B,
    HEADLIGHT_LEFT = 0x4C,
    HEADLIGHT_RIGHT = 0x4D,
    HOOD = 0x4E,
    LICENSE_PLATE = 0x4F,
    LIGHTBAR = 0x50,
    ROLL_CAGE = 0x51,
    ROOF = 0x52,
    ROOF_INSIDE = 0x53,
    ROOFSCOOP = 0x54,
    SEAT_LEFT = 0x55,
    SEAT_RIGHT = 0x56,
    SIDEMIRROR_LEFT = 0x57,
    SIDEMIRROR_RIGHT = 0x58,
    SKIRT_LEFT = 0x59,
    SKIRT_RIGHT = 0x5A,
    SPOILER = 0x5B,
    STEERINGWHEEL = 0x5C,
    TRUNK = 0x5D,
    UNIVERSAL_SPOILER_BASE = 0x5E,
    WHEEL_FRONT = 0x5F,
    WHEEL_REAR = 0x60,
    WINDOW_FRONT = 0x61,
    WINDOW_FRONT_LEFT = 0x62,
    WINDOW_FRONT_RIGHT = 0x63,
    WINDOW_REAR = 0x64,
    WINDOW_REAR_LEFT = 0x65,
    WINDOW_REAR_RIGHT = 0x66,
    BRAKE_PACKAGE = 0x67,
    DRIVETRAIN_PACKAGE = 0x68,
    ENGINE_PACKAGE = 0x69,
    FORCED_INDUCTION_PACKAGE = 0x6A,
    INTERIOR = 0x6B,
    LIVERY = 0x6C,
    NITROUS_PACKAGE = 0x6D,
    SUSPENSION_PACKAGE = 0x6E,
    TIRE_FRONT = 0x6F,
    TIRE_PACKAGE = 0x70,
    TIRE_REAR = 0x71,
    VECTORVINYL = 0x72
};

enum AutoSculptRegionID {
    RegionFrontBumper = 0,
    RegionStart = 0,
    RegionRearBumper = 1,
    RegionSkirtLeft = 2,
    RegionSkirtRight = 3,
    RegionRimsFront = 4,
    RegionRimsRear = 5,
    RegionDoorLeft = 6,
    RegionDoorRight = 7,
    RegionDoorRearLeft = 8,
    RegionDoorRearRight = 9,
    RegionFenderFrontLeft = 10,
    RegionFenderFrontRight = 11,
    RegionBody = 12,
    RegionHoods = 13,
    RegionSpoilers = 14,
    RegionRoofScoops = 15,
    RegionRoofHeight = 16,
    RegionExhaust = 17,
    //    RegionPresetRideLast = 17,
    RegionFrontWindow = 18,
    RegionRearWindow = 19,
    RegionFrontLeftWindow = 20,
    RegionFrontRightWindow = 21,
    RegionRearLeftWindow = 22,
    RegionRearRightWindow = 23,
    RegionLeftDoor = 24,
    RegionRightDoor = 25,
    RegionInterior = 26,
    RegionDoorline = 27,

    RegionRearBumperExtra = 28,
    RegionFrontBumperExtra = 29
    //RegionWindTunnelStart = 28,
    //RegionWindTunnelType1 = 28,
    //RegionWindTunnelType2 = 29,
    //RegionWindTunnelType3 = 30,
    //RegionWindTunnelType4 = 31,
    //RegionWindTunnelType5 = 32,
    //RegionWindTunnelType6 = 33,
    //RegionWindTunnelType7 = 34,
    //RegionWindTunnelType8 = 35,
    //RegionWindTunnelType9 = 36,
    //RegionWindTunnelEnd = 37,
    //RegionWindTunnelType10 = 37,
    //RegionRearBumperExtra = 38,
    //RegionFrontBumperExtra = 39,
    //RegionEnd = 40,
    //RegionNum = 40,
    //RegionInvalid = 41
};

enum CarEffectPosition {
    CARFXPOS_NONE = 0,
    CARFXPOS_TIRE_FRONTS = 1,
    CARFXPOS_TIRE_REARS = 2,
    CARFXPOS_LEFT_TIRES = 3,
    CARFXPOS_RIGHT_TIRES = 4,
    CARFXPOS_TIRE_FL = 5,
    CARFXPOS_TIRE_FR = 6,
    CARFXPOS_TIRE_RR = 7,
    CARFXPOS_TIRE_RL = 8,
    CARFXPOS_ENGINE = 9,
    CARFXPOS_EXHAUST = 10,
    CARFXPOS_RADIATOR = 11,
    CARFXPOS_WINDSHIELD = 12,
    CARFXPOS_WINDOW_REAR = 13,
    CARFXPOS_WINDOW_LEFT_FRONT = 14,
    CARFXPOS_WINDOW_LEFT_REAR = 15,
    CARFXPOS_WINDOW_RIGHT_FRONT = 16,
    CARFXPOS_WINDOW_RIGHT_REAR = 17,
    CARFXPOS_HEADLIGHT = 18,
    CARFXPOS_HEADLIGHT_LEFT = 19,
    CARFXPOS_HEADLIGHT_RIGHT = 20,
    CARFXPOS_BRAKELIGHT = 21,
    CARFXPOS_BRAKELIGHT_LEFT = 22,
    CARFXPOS_BRAKELIGHT_RIGHT = 23,
    CARFXPOS_SIDE_MIRROR_LEFT = 24,
    CARFXPOS_SIDE_MIRROR_RIGHT = 25,
    CARFXPOS_LICENSE_PLATE_FRONT = 26,
    CARFXPOS_LICENSE_PLATE_REAR = 27,
    NUM_CARFXPOS = 28
};
enum ePlayerSettingsCameras {
    NUM_CAMERAS_IN_OPTIONS = 5,
    PSC_BUMPER = 0,
    PSC_CLOSE = 2,
    PSC_DEFAULT = 2,
    PSC_DRIFT = 4,
    PSC_FAR = 3,
    PSC_HOOD = 1,
    PSC_MAX = 4,
    PSC_MIN = 0,
    PSC_PAUSE = 7,
    PSC_SLOTCAR = 8,
    PSC_STAGING = 6,
    PSC_TUNING = 9
};

struct FECarRecord {
    DWORD id; //-1 uninitialized, 0-329 or preset
    DWORD frontend;
    DWORD pvehicle;
    DWORD preset; //can be zero if stock car or career
    DWORD filterBits; //00010001 america 00020001 europe 00040001 japan
    uint8_t customization;
    uint8_t careerHandle;
    uint8_t isPresetSkin;
    uint8_t padding;
    uint8_t isPinkSlip;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t pad3;
    //DWORD field_0x14 = 0xAA00FFFF; //0xAAAAFFFF when empty, 0xAA00FFFF when used, 0xAA00[i][i] for career car i, i<256
    //DWORD field_0x18 = 0xAAAAAAAA; //ends with 00 or 01 when career car
};

struct DALInterface; //i hate c++ so much
struct DALInterface_vt {
    void* DALInterface_destructor;
    bool(__fastcall* IsInterfaceValid)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* InitCachedIntValues)(DALInterface* ecx, DWORD edx, DWORD);
    bool(__fastcall* InitCachedFloatValues)(DALInterface* ecx, DWORD edx, DWORD);
    bool(__fastcall* GetFloat)(DALInterface* ecx, DWORD edx, DWORD, float*, DWORD, DWORD, DWORD);
    bool(__fastcall* SetFloat)(DALInterface* ecx, DWORD edx, DWORD, float, DWORD, DWORD, DWORD);
    bool(__fastcall* IncrementFloat)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* DecrementFloat)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* GetInt)(DALInterface* ecx, DWORD edx, DWORD, int*, DWORD, DWORD, DWORD);
    bool(__fastcall* SetInt)(DALInterface* ecx, DWORD edx, DWORD, int, DWORD, DWORD, DWORD);
    bool(__fastcall* IncrementInt)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* DecrementInt)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* GetString)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* SetString)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* GetDefaultInt)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD);
    bool(__fastcall* GetDefaultFloat)(DALInterface* ecx, DWORD edx, DWORD, DWORD, DWORD, DWORD, DWORD);
    void* HandleCommand;
    void* CancelCommand;
};
struct DALInterface {
    DALInterface_vt* vftable;
    int mInterfaceIndex;
};
//VALUES FOR 1.18, MAY BE DIFFERENT IN 1.0
enum DAL_OPTIONS {
    DAL_OPTIONS_HIGHLIGHT_CAM = 5000,
    DAL_OPTIONS_FE_SCALE = 5001,
    DAL_OPTIONS_SCREEN_OFFSET_X = 5002,
    DAL_OPTIONS_SCREEN_OFFSET_Y = 5003,
    DAL_OPTIONS_TIME_OF_DAY = 5004,
    DAL_OPTIONS_BRIGHTNESS = 5005,
    DAL_OPTIONS_MASTER_VOL = 5006,
    DAL_OPTIONS_SPEECH_VOL = 5007,
    DAL_OPTIONS_MASTERED_SPEECH_VOL = 5008,
    DAL_OPTIONS_FE_MUSIC_VOL = 5009,
    DAL_OPTIONS_MASTERED_FE_MUSIC_VOL = 5010,
    DAL_OPTIONS_IG_MUSIC_VOL = 5011,
    DAL_OPTIONS_MASTERED_IG_MUSIC_VOL = 5012,
    DAL_OPTIONS_SOUND_EFFECTS_VOL = 5013,
    DAL_OPTIONS_MASTERED_SOUND_EFFECTS_VOL = 5014,
    DAL_OPTIONS_ENGINE_VOL = 5015,
    DAL_OPTIONS_MASTERED_ENGINE_VOL = 5016,
    DAL_OPTIONS_CAR_VOL = 5017,
    DAL_OPTIONS_MASTERED_CAR_VOL = 5018,
    DAL_OPTIONS_AMBIENT_VOL = 5019,
    DAL_OPTIONS_MASTERED_AMBIENT_VOL = 5020,
    DAL_OPTIONS_SPEED_VOL = 5021,
    DAL_OPTIONS_MASTERED_SPEED_VOL = 5022,
    DAL_OPTIONS_ONLINE_AUTO_LOGIN = 5023,
    DAL_OPTIONS_ONLINE_EXPAND_SEARCH = 5024,
    DAL_OPTIONS_IS_RESOLUTION_HD = 5025,
    DAL_OPTIONS_GAUGES_ON = 5026,
    DAL_OPTIONS_POSITION_ON = 5027,
    DAL_OPTIONS_LAP_INFO_ON = 5028,
    DAL_OPTIONS_SCORE_ON = 5029,
    DAL_OPTIONS_RUMBLE_ON = 5030,
    DAL_OPTIONS_LEADERBOARD_ON = 5031,
    DAL_OPTIONS_HIP_MUSIC_STREAMER_ON = 5032,
    DAL_OPTIONS_CREW_INFO_ON = 5033,
    DAL_OPTIONS_TRANSMISSION_PROMPT_ON = 5034,
    DAL_OPTIONS_DRIVE_WITH_ANALOG = 5035,
    DAL_OPTIONS_SENSITIVITY_SETTING = 5036,
    DAL_OPTIONS_CONTROLLER_CONFIG = 5037,
    DAL_OPTIONS_WHEEL_SENSITIVITY = 5038,
    DAL_OPTIONS_WHEEL_OVERALL_FORCE = 5039,
    DAL_OPTIONS_CAMERA = 5040,
    DAL_OPTIONS_SPLIT_TIME_TYPE = 5041,
    DAL_OPTIONS_TRANSMISSION = 5042,
    DAL_OPTIONS_HANDLING = 5043,
    DAL_OPTIONS_AUTOSAVE_ON = 5044,
    DAL_OPTIONS_REARVIEW_ON = 5045,
    DAL_OPTIONS_PIP_ON = 5046,
    DAL_OPTIONS_DAMAGE_ON = 5047,
    DAL_OPTIONS_SPEEDO_UNITS = 5048,
    DAL_OPTIONS_CURR_MINI_MAP_MODE = 5049,
    DAL_OPTIONS_RACING_MINI_MAP_MODE = 5050,
    DAL_OPTIONS_EXPLORING_MINI_MAP_MODE = 5051,
    DAL_OPTIONS_MAP_ITEMS = 5052,
    DAL_OPTIONS_IS_MAP_ITEM_ENABLED = 5053,
    DAL_OPTIONS_LAST_MAP_ZOOM = 5054,
    DAL_OPTIONS_LAST_PURSUIT_MAP_ZOOM = 5055,
    DAL_OPTIONS_LAST_MAP_VIEW = 5056,
    DAL_OPTIONS_MAP_HELP_SHOWN = 5057,
    DAL_OPTIONS_MAP_RANDENC_HELP_SHOWN = 5058,
    DAL_OPTIONS_JUMP_CAM_ON = 5059,
    DAL_OPTIONS_WIDESCREEN = 5060,
    DAL_OPTIONS_AUDIO_MODE = 5061,
    DAL_OPTIONS_INTERACTIVE_MUSIC_MODE = 5062,
    DAL_OPTIONS_EA_TRAX_MODE = 5063,
    DAL_OPTIONS_EA_TRAX_CLASS_FILTER = 5064,
    DAL_OPTIONS_PLAY_STATE = 5065,
    DAL_OPTIONS_AUDIO_QUALITY = 5066,
    DAL_OPTIONS_QUICKRACE_LAPS = 5067,
    DAL_OPTIONS_QUICKRACE_NUM_AI = 5068,
    DAL_OPTIONS_QUICKRACE_TRAFFIC = 5069,
    DAL_OPTIONS_QUICKRACE_DIFFICULTY = 5070,
    DAL_OPTIONS_QUICKRACE_RANKED = 5071,
    DAL_OPTIONS_QUICKRACE_CATCH_UP = 5072,
    DAL_OPTIONS_QUICKRACE_NOS = 5073,
    DAL_OPTIONS_PC_PAD_INDEX = 5074,
    DAL_OPTIONS_PC_DEVICE_TYPE = 5075,
    DAL_PC_DISPLAY_APPLY_SETTINGS = 5076,
    DAL_PC_DISPLAY_PERFORMANCE_LEVEL = 5077,
    DAL_PC_DISPLAY_SCREEN_RESOLUTION = 5078,
    DAL_PC_DISPLAY_AA = 5079,
    DAL_PC_DISPLAY_TEXTURE_FILTERING = 5080,
    DAL_PC_DISPLAY_SHADER_MODEL = 5081,
    DAL_PC_DISPLAY_V_SYNC = 5082,
    DAL_PC_DISPLAY_WORLD_LOD = 5083,
    DAL_PC_DISPLAY_ROAD_REFLECTION_LOD = 5084,
    DAL_PC_DISPLAY_CAR_LOD = 5085,
    DAL_PC_DISPLAY_CAR_EFFECTS_LEVEL = 5086,
    DAL_PC_DISPLAY_CAR_REFELCTION_LOD = 5087,
    DAL_PC_DISPLAY_CAR_REFLECTION_UPDATE_RATE = 5088,
    DAL_PC_DISPLAY_RAIN_ENABLED = 5089,
    DAL_PC_DISPLAY_OVER_BRIGHT_ENABLED = 5090,
    DAL_PC_DISPLAY_VISUAL_TREATEMENT_ENABLED = 5091,
    DAL_PC_DISPLAY_PARTICLE_SYSTEM_ENABLED = 5092,
    DAL_PC_DISPLAY_MOTION_BLUR_ENABLED = 5093,
    DAL_PC_DISPLAY_BRIGHTNESS = 5094,
    DAL_PC_DISPLAY_WORLD_SHADOW_LEVEL = 5095,
    DAL_PC_DISPLAY_WORLD_FX_LEVEL = 5096,
    DAL_PC_DISPLAY_SMOKE = 5097,
    DAL_INIT_INT = 5098,
    DAL_INIT_FLOAT = 5099
};


enum eSkyStyle {
    SKY_STYLE_01 = 1,
    SKY_STYLE_02 = 2,
    SKY_STYLE_03 = 3,
    SKY_STYLE_04 = 4,
    SKY_STYLE_05 = 5,
    SKY_STYLE_06 = 6,
    SKY_STYLE_07 = 7,
    SKY_STYLE_08 = 8,
    SKY_STYLE_09 = 9,
    SKY_STYLE_10 = 10,
    SKY_STYLE_11 = 11,
    SKY_STYLE_12 = 12,
    SKY_STYLE_13 = 13,
    SKY_STYLE_14 = 14,
    SKY_STYLE_15 = 15,
    SKY_STYLE_NUM = 16
};
enum CubeType {
    CubeType_FILTER0 = 0,
    CubeType_TRANSFORMERS = 1,
    CubeType_GONE_IN_60_SECONDS = 2,
    CubeType_BOYS_IN_THE_HOOD = 3,
    CubeType_INTHEZONE = 4,
    CubeType_COPINTRO = 5,
    CubeType_LEVELUP = 6,
    CubeType_WHITEOVERLAY = 7,
    CubeType_FE = 8,
    CubeType_NUM = 9
};

struct SunAndSky {
    DWORD mCurrentKey; /* //T_UINT4 */
    DWORD mDesiredKey; /* //T_UINT4 */
    DWORD mZoneMap;//struct GMap* mZoneMap; /* //GMap* */
    DWORD mZoneBlender;//struct VisualLookEffectTarget* mZoneBlender; /* //VisualLookEffectTarget* */
    float mBlendAmount; /* //T_REAL32 */
    bool mInitialized; /* //T_BOOL08 */
    undefined4 field6_0x18;
    undefined4 field7_0x1c;
    struct bVector4 mSunDirection; /* //bVector4 */
    struct bVector4 mVisibleSunDirection; /* //bVector4 */
    struct bVector4 mSunPosition; /* //bVector4 */
    struct bVector4 mVisibleSunPosition; /* //bVector4 */
    struct bVector4 mSunColour; /* //bVector4 */
    struct bVector4 mAmbientColour; /* //bVector4 */
    struct bVector4 mCarSunColour; /* //bVector4 */
    struct bVector4 mCarAmbientColour; /* //bVector4 */
    struct bVector4 mEnvMapTint; /* //bVector4 */
    struct bVector4 mFogColour; /* //bVector4 */
    struct bVector4 mFogColourFar; /* //bVector4 */
    float mFogNear; /* //T_REAL32 */
    float mFogFar; /* //T_REAL32 */
    undefined4 field21_0xd8;
    undefined4 field22_0xdc;
    struct bVector4 mSunHazeColour; /* //bVector4 */
    struct bVector4 mSpecularColour; /* //bVector4 */
    struct bVector4 mSHColour; /* //bVector4 */
    float mFogExponent; /* //T_REAL32 */
    bool mOverrideSunPosition; /* //T_BOOL08 */
    float mLatitudeOverride; /* //T_REAL32 */
    float mLongitudeOverride; /* //T_REAL32 */
    float mSkyLatitudeOverride; /* //T_REAL32 */
    bool mOverrideFogFar; /* //T_BOOL08 */
    float mFogFarOverride; /* //T_REAL32 */
    int mLensFlareIndex; /* //T_UINT4 */
    float mSkyDomeRotation; /* //T_REAL32 */
    float mSpecularSize; /* //T_REAL32 */
    float mRoadSpecularSize; /* //T_REAL32 */
    float mSpecularIntensity; /* //T_REAL32 */
    struct bVector4 mSpecularColourAndIntensity; /* //bVector4 */
    struct bVector4 mWorldAndRoadSpecularSizes; /* //bVector4 */
    float field40_0x160;
    float field41_0x164;
    undefined4 field42_0x168;
    undefined4 field43_0x16c;
    struct bVector4 mRoadReflectionParams; /* //bVector4 */
    float mShadowMixAmount; /* //T_REAL32 */
    float mShadowSoftness; /* //T_REAL32 */
    float mBloomIntensity; /* //T_REAL32 */
    float mBloomThreshold; /* //T_REAL32 */
    float mBloomExponent; /* //T_REAL32 */
    float mBloomRecoveryBias; /* //T_REAL32 */
    float mBloomRecoveryScale; /* //T_REAL32 */
    float mSunHazeDistanceBias; /* //T_REAL32 */
    float mSunHazeSize; /* //T_REAL32 */
    enum eSkyStyle mSkyStyle; /* //eSkyStyle */
    enum CubeType mColourCube; /* //CubeType */
    enum CubeType mTransitioningCube; /* //CubeType */
    float mCubeBlendAmount; /* //T_REAL32 */
    float mToneMappingContrast; /* //T_REAL32 */
    float mToneMappingSaturation; /* //T_REAL32 */
    float mSunOcclusionResponseRate; /* //T_REAL32 */
};

namespace Attrib {

    struct Array {
        short mAlloc; /* //T_USHORT */
        short mCount; /* //T_USHORT */
        short mSize; /* //T_USHORT */
        short mEncodedTypeAndPad; /* //T_USHORT */
    };

    struct Node {
        int mKey; /* //T_UINT4 */
        int mPtr; /* //T_32PVOID */
        struct Array* mArray; /* //Attrib::Array* */
        int mValue; /* //T_UINT4 */
        int mOffset; /* //T_UINT4 */
        short mTypeIndex; /* //T_USHORT */
        byte mMax; /* //T_UCHAR */
        byte mFlags; /* //T_UCHAR */
    };

    struct HashMap {
        struct Node* mTable; /* //Attrib::Node* */
        short mTableSize; /* //T_USHORT */
        short mNumEntries; /* //T_USHORT */
        short mRefCount; /* //T_USHORT */
        byte mWorstCollision; /* //T_UCHAR */
        byte mKeyShift; /* //T_UCHAR */
    };


    struct Vault {
        long mVersion; /* //T_UQUAD */
        int mUserID; /* //T_UINT4 */
        int mRefCount; /* //T_UINT4 */
        void* mExportMgr; /* //Attrib::ExportManager* */
        void* mGC; /* //Attrib::IGarbageCollector* */
        void* mDependencies; /* //Attrib::Vault::DependencyNode* */
        void* mDepData; /* //Attrib::Vault::DataBlock* */
        int mDepIDs; /* //T_32PUINT4 */
        int mNumDependencies; /* //T_UINT4 */
        int mResolvedCount; /* //T_UINT4 */
        void* mPointers; /* //Attrib::Vault::PointerNode* */
        int mTransientData; /* //T_32PUCHAR */
        void* mExports; /* //Attrib::Vault::ExportNode* */
        void* mExportData; /* //Attrib::Vault::DataBlock* */
        int mExportIDs; /* //T_32PUINT4 */
        int mNumExports; /* //T_UINT4 */
        int mNumAllocExports; /* //T_UINT4 */
        int mNumLoadedExports; /* //T_UINT4 */
        byte mInited; /* //T_BOOL08 */
        byte mDeinited; /* //T_BOOL08 */
    };

    struct Definition {
        int mKey; /* //T_UINT4 */
        int mType; /* //T_UINT4 */
        short mOffset; /* //T_USHORT */
        short mSize; /* //T_USHORT */
        short mMaxCount; /* //T_USHORT */
        byte mFlags; /* //T_UCHAR */
        byte mAlignment; /* //T_UCHAR */
    };

    struct ClassPrivate;

    struct Class {
        int mKey; /* //T_UINT4 */
        struct ClassPrivate* mPrivates; /* //Attrib::ClassPrivate* */
    };

    struct ClassPrivate {
        struct Class __inherit_0; /* //Attrib::Class */
        struct HashMap mLayoutTable; /* //Attrib::HashMap */
        void* mCollections; /* //Attrib::ClassPrivate::CollectionHashMap */
        short mLayoutSize; /* //T_USHORT */
        short mNumDefinitions; /* //T_USHORT */
        struct Definition* mDefinitions; /* //Attrib::Definition* */
        struct Vault* mSource; /* //Attrib::Vault* */
        int mStaticData; /* //T_32PVOID */
    };

    struct Collection {
        struct HashMap mTable; /* //Attrib::HashMap */
        Collection* mParent; /* //MOD_const__Attrib::Collection* */
        int mKey; /* //T_UINT4 */
        struct Class* mClass; /* //Attrib::Class* */
        int mLayout; /* //T_32PVOID */
        struct Vault* mSource; /* //Attrib::Vault* */
    };

    struct _Instance {
        struct Collection* mCollection; /* //MOD_const__Attrib::Collection* */
        int mLayoutPtr; /* //T_32PVOID */
        int mMsgPort; /* //T_UINT4 */
        int mFlags; /* //T_UINT4 */
    };

    namespace Instance {
        void* GetAttributePointerFromString(_Instance* this_, const char* key, DWORD stuff);
        auto GetAttributePointer = (void* (__fastcall*)(_Instance * this_, DWORD edx, DWORD key, DWORD stuff))0x0;
    }

    namespace Gen {
        auto zone_visual_style__constructor = (void(__fastcall*)(_Instance * this_, DWORD edx, DWORD node, DWORD stuff))0x0;
    }

    auto DefaultDataArea = (void* (__cdecl*)(DWORD length))0x0;

}