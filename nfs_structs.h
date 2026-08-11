#pragma once
#include <cstdint>
#include <sys/types.h>
#include <combaseapi.h>

typedef unsigned int    undefined4;
typedef unsigned int    undefined; //default
typedef unsigned char    byte;
typedef unsigned char MOD_const__T_RCHAR;
typedef void* pointer;

struct bVector3 {
    float x;
    float y;
    float z;
    float pad;
};
struct bVector4 {
    float x;
    float y;
    float z;
    float w;
};

struct bNode {
    bNode* prev;
    bNode* next;
};
template <typename t>
struct bTNode : bNode {
};
struct bList {
    struct bNode HeadNode; /* //bNode */
};
template <typename t>
struct bTList : bList {
};


enum eAemsUpgradeLevel {
    AEMS_LEVEL0 = 0,
    AEMS_LEVEL1 = 1,
    AEMS_LEVEL2 = 2,
    AEMS_LEVEL3 = 3,
    AEMS_MAX_LEVEL = 4,
    AEMS_NO_LEVEL = 255
};
enum eMAINMAPSTATES {
    eMM_MAIN = 0,
    eMM_PLAYERCAR = 1,
    eMM_AIRACECAR = 2,
    eMM_TRAFFIC = 3,
    eMM_MOMENT = 4,
    eMM_COLLISION = 5,
    eMM_DRIVEBY = 6,
    eMM_SIREN = 7,
    eMM_HELICOPTER = 8,
    eMM_MAX_MAIN_MIXSTATES = 9
};
enum Gear {
    REVERSE = 0,
    NEUTRAL = 1,
    FIRST_GEAR = 2,
    SECOND_GEAR = 3,
    THIRD_GEAR = 4,
    FOURTH_GEAR = 5,
    FIFTH_GEAR = 6,
    SIXTH_GEAR = 7,
    SEVENTH_GEAR = 8,
    SPORT_SHIFT = 254,
    AUTOMATIC = 255
};
enum DriverTypeEnum {
    DRIVER_TYPE_NONE = 0,
    DRIVER_TYPE_PLAYER = 1,
    DRIVER_TYPE_AI_RACER = 2,
    DRIVER_TYPE_TRAFFIC_CAR = 3,
    DRIVER_TYPE_PARKED_CAR = 4,
    DRIVER_TYPE_COP = 5,
    DRIVER_TYPE_SUPER_COP = 6
};
enum ClutchState {
    CLUTCH_STATE_ENGAGED = 0,
    CLUTCH_STATE_DISENGAGED = 1,
    CLUTCH_STATE_UPSHIFT = 2,
    CLUTCH_STATE_DOWNSHIFT = 3,
    CLUTCH_STATE_TOTAL = 4
};
enum CarRenderUsage {
    CarRenderUsage_Player = 0,
    CarRenderUsage_RemotePlayer = 1,
    CarRenderUsage_AIRacer = 2,
    CarRenderUsage_AICop = 3,
    CarRenderUsage_AITraffic = 4,
    CarRenderUsage_AIHeli = 5,
    carRenderUsage_NISCar = 6,
    CarRenderUsage_Ghost = 7,
    CarRenderUsage_Invalid = 8
};
enum ePaintMaterialIndex {
    ePaintMaterialWhite = 0,
    ePaintMaterialSilver = 1,
    ePaintMaterialBlack = 2,
    ePaintMaterialRed = 3,
    ePaintMaterialViolet = 4,
    ePaintMaterialIndigo = 5,
    ePaintMaterialBlue = 6,
    ePaintMaterialGreen = 7,
    ePaintMaterialYellow = 8,
    ePaintMaterialOrange = 9
};

enum eChyronType {
    CHYRON_NONE = 0,
    CHYRON_ONLINE_CONNECTION = 1,
    CHYRON_LOST_ONLINE_CONNECTION = 2,
    CHYRON_ACHIEVMENT_UNLOCKED = 3,
    CHYRON_MESSAGE = 4,
    CHYRON_FRIEND_REQUEST = 5,
    CHYRON_GAME_INVITE = 6,
    //CHYRON_ALERT = 7,
    CHYRON_NEW_RECORD = 7,
    CHYRON_WEB_REWARD = 8,
    CHYRON_NO_CHAT = 9,
    CHYRON_MUSIC = 10,
    CHYRON_MAX = 11
};

enum CAR_SLOT_ID {
    CARSLOTID_BADGING_BUMPER_SET_FRONT = 0,
    CARSLOTID_MODEL_FIRST = 0,
    CARSLOTID_BADGING_BUMPER_SET_REAR = 1,
    CARSLOTID_BADGING_FENDER_FRONT_LEFT = 2,
    CARSLOTID_DECAL_MODEL_NUM = 2,
    CARSLOTID_BADGING_FENDER_FRONT_RIGHT = 3,
    CARSLOTID_BADGING_FENDER_REAR_LEFT = 4,
    CARSLOTID_BADGING_FENDER_REAR_RIGHT = 5,
    CARSLOTID_BADGING_TRUNK = 6,
    CARSLOTID_BASE = 7,
    CARSLOTID_BODY = 8,
    CARSLOTID_BODY_DOORLINE = 9,
    CARSLOTID_BRAKE_CALIPER_FRONT = 10,
    CARSLOTID_BRAKE_CALIPER_REAR = 11,
    CARSLOTID_BRAKE_ROTOR_FRONT = 12,
    CARSLOTID_NONRENDER_NUM = 12,
    CARSLOTID_BRAKE_ROTOR_REAR = 13,
    CARSLOTID_BRAKELIGHT_GLASS_LEFT = 14,
    CARSLOTID_BRAKELIGHT_GLASS_RIGHT = 15,
    CARSLOTID_BRAKELIGHT_LEFT = 16,
    CARSLOTID_BRAKELIGHT_RIGHT = 17,
    CARSLOTID_BUMPER_FRONT = 18,
    CARSLOTID_BUMPER_FRONT_EXTRA = 19,
    CARSLOTID_BUMPER_REAR = 20,
    CARSLOTID_BUMPER_REAR_EXTRA = 21,
    CARSLOTID_CHASSIS = 22,
    CARSLOTID_DAMAGE_BODY = 23,
    CARSLOTID_DAMAGE_MODEL_FIRST = 23,
    CARSLOTID_DAMAGE_BRAKELIGHT_LEFT = 24,
    CARSLOTID_DAMAGE_BRAKELIGHT_RIGHT = 25,
    CARSLOTID_DAMAGE_BUMPER_FRONT = 26,
    CARSLOTID_DAMAGE_BUMPER_REAR = 27,
    CARSLOTID_DAMAGE_DOOR_LEFT = 28,
    CARSLOTID_DAMAGE_DOOR_REAR_LEFT = 29,
    CARSLOTID_DAMAGE_DOOR_REAR_RIGHT = 30,
    CARSLOTID_DAMAGE_DOOR_RIGHT = 31,
    CARSLOTID_DAMAGE_FENDER_FRONT_LEFT = 32,
    CARSLOTID_DAMAGE_FENDER_FRONT_RIGHT = 33,
    CARSLOTID_DAMAGE_MODEL_NUM = 33,
    CARSLOTID_DAMAGE_HEADLIGHT_LEFT = 34,
    CARSLOTID_DAMAGE_HEADLIGHT_RIGHT = 35,
    CARSLOTID_DAMAGE_HOOD = 36,
    CARSLOTID_DAMAGE_SIDEMIRROR_LEFT = 37,
    CARSLOTID_DAMAGE_SIDEMIRROR_RIGHT = 38,
    CARSLOTID_DAMAGE_TRUNK = 39,
    CARSLOTID_DAMAGE_TRUNK911 = 40,
    CARSLOTID_DAMAGE_WIDEBODY = 41,
    CARSLOTID_DAMAGE_WIDEBODY_BUMPER_FRONT = 42,
    CARSLOTID_DAMAGE_WIDEBODY_BUMPER_REAR = 43,
    CARSLOTID_DAMAGE_WIDEBODY_DOOR_LEFT = 44,
    CARSLOTID_DAMAGE_WIDEBODY_DOOR_REAR_LEFT = 45,
    CARSLOTID_DAMAGE_WIDEBODY_DOOR_REAR_RIGHT = 46,
    CARSLOTID_DAMAGE_WIDEBODY_DOOR_RIGHT = 47,
    CARSLOTID_DAMAGE_WIDEBODY_FENDER_FRONT_LEFT = 48,
    CARSLOTID_DAMAGE_WIDEBODY_FENDER_FRONT_RIGHT = 49,
    CARSLOTID_DAMAGE_WINDOW_FRONT = 50,
    CARSLOTID_DAMAGE_WINDOW_FRONT_LEFT = 51,
    CARSLOTID_DAMAGE_WINDOW_FRONT_RIGHT = 52,
    CARSLOTID_DAMAGE_WINDOW_REAR = 53,
    CARSLOTID_DAMAGE_WINDOW_REAR_LEFT = 54,
    CARSLOTID_DAMAGE_MODEL_LAST = 55,
    CARSLOTID_DAMAGE_WINDOW_REAR_RIGHT = 55,
    CARSLOTID_DECAL_FRONT_WINDOW = 56,
    CARSLOTID_DECAL_MODEL_FIRST = 56,
    CARSLOTID_DECAL_MODEL_LAST = 57,
    CARSLOTID_DECAL_REAR_WINDOW = 57,
    CARSLOTID_DOOR_LEFT = 58,
    CARSLOTID_DOOR_REAR_LEFT = 59,
    CARSLOTID_DOOR_REAR_RIGHT = 60,
    CARSLOTID_DOOR_RIGHT = 61,
    CARSLOTID_DOORHANDLE_FRONT_LEFT = 62,
    CARSLOTID_DOORHANDLE_FRONT_RIGHT = 63,
    CARSLOTID_DOORHANDLE_REAR_LEFT = 64,
    CARSLOTID_DOORHANDLE_REAR_RIGHT = 65,
    CARSLOTID_DRIVER = 66,
    CARSLOTID_ENGINE = 67,
    CARSLOTID_EXHAUST = 68,
    CARSLOTID_EXHAUST_TIPS_CENTER = 69,
    CARSLOTID_EXHAUST_TIPS_LEFT = 70,
    CARSLOTID_EXHAUST_TIPS_RIGHT = 71,
    CARSLOTID_FENDER_FRONT_LEFT = 72,
    CARSLOTID_FENDER_FRONT_RIGHT = 73,
    CARSLOTID_HEADLIGHT_GLASS_LEFT = 74,
    CARSLOTID_HEADLIGHT_GLASS_RIGHT = 75,
    CARSLOTID_HEADLIGHT_LEFT = 76,
    CARSLOTID_HEADLIGHT_RIGHT = 77,
    CARSLOTID_HOOD = 78,
    CARSLOTID_LICENSE_PLATE = 79,
    CARSLOTID_LIGHTBAR = 80,
    CARSLOTID_ROLL_CAGE = 81,
    CARSLOTID_ROOF = 82,
    CARSLOTID_ROOF_INSIDE = 83,
    CARSLOTID_ROOFSCOOP = 84,
    CARSLOTID_SEAT_LEFT = 85,
    CARSLOTID_SEAT_RIGHT = 86,
    CARSLOTID_SIDEMIRROR_LEFT = 87,
    CARSLOTID_SIDEMIRROR_RIGHT = 88,
    CARSLOTID_SKIRT_LEFT = 89,
    CARSLOTID_SKIRT_RIGHT = 90,
    CARSLOTID_SPOILER = 91,
    CARSLOTID_STEERINGWHEEL = 92,
    CARSLOTID_TRUNK = 93,
    CARSLOTID_UNIVERSAL_SPOILER_BASE = 94,
    CARSLOTID_WHEEL_FRONT = 95,
    CARSLOTID_WHEEL_REAR = 96,
    CARSLOTID_WINDOW_FRONT = 97,
    CARSLOTID_WINDOW_FRONT_LEFT = 98,
    CARSLOTID_WINDOW_FRONT_RIGHT = 99,
    CARSLOTID_WINDOW_REAR = 100,
    CARSLOTID_WINDOW_REAR_LEFT = 101,
    CARSLOTID_MODEL_LAST = 102,
    CARSLOTID_WINDOW_REAR_RIGHT = 102,
    CARSLOTID_BRAKE_PACKAGE = 103,
    CARSLOTID_FIRST_NONRENDER = 103,
    CARSLOTID_MODEL_NUM = 103,
    CARSLOTID_DRIVETRAIN_PACKAGE = 104,
    CARSLOTID_ENGINE_PACKAGE = 105,
    CARSLOTID_FORCED_INDUCTION_PACKAGE = 106,
    CARSLOTID_INTERIOR = 107,
    CARSLOTID_LIVERY = 108,
    CARSLOTID_NITROUS_PACKAGE = 109,
    CARSLOTID_SUSPENSION_PACKAGE = 110,
    CARSLOTID_TIRE_FRONT = 111,
    CARSLOTID_TIRE_PACKAGE = 112,
    CARSLOTID_TIRE_REAR = 113,
    CARSLOTID_LAST_NONRENDER = 114,
    CARSLOTID_VECTORVINYL = 114,
    CARSLOTID_NUM = 115,
    CARSLOTID_INVALID = 255
};

enum ePaintSlot {
    CUSTOM_PAINT_SLOT_BODY = 0,
    CUSTOM_PAINT_SLOT_RIMS_FRONT = 1,
    CUSTOM_PAINT_SLOT_RIMS_REAR = 2,
    CUSTOM_PAINT_SLOT_ROLLCAGE = 3,
    CUSTOM_PAINT_SLOT_BUMPER_FRONT = 4,
    CUSTOM_PAINT_SLOT_BUMPER_REAR = 5,
    CUSTOM_PAINT_SLOT_SIDESKIRT = 6,
    CUSTOM_PAINT_SLOT_HOOD = 7,
    CUSTOM_PAINT_SLOT_MIRROR = 8,
    CUSTOM_PAINT_SLOT_SPOILER = 9,
    CUSTOM_PAINT_SLOT_WINDOW_TINT = 10,
    CUSTOM_PAINT_SLOT_ROOFSCOOP = 11,
    MAX_CUSTOM_PAINT_SLOTS = 12,
    INVALID_PAINT_SLOT = 13
};

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

enum VinylsAttrib__VinylFinish {
    VinylFinishMatte = 0,
    VinylFinishGloss = 1,
    VinylFinishChrome = 2
};

enum BluePrintType {
    BLUEPRINT_FIRST = 0,
    BLUEPRINT_GRIP = 0,
    BLUEPRINT_LAST = 0,
    BLUEPRINT_DRIFT = 1,
    BLUEPRINT_NUM = 1,
    BLUEPRINT_DRAG = 2,
    BLUEPRINT_SPEED_CHALLENGE = 3
};

enum KIT_TYPE {
    KIT_TYPE_BASE = 0,
    KIT_TYPE_STOCK = 1,
    KIT_TYPE_AUTOSCULPT = 2,
    KIT_TYPE_WIDEBODY = 3,
    NUM_KIT_TYPE = 4
};

enum eCustomTuningType {
    CTT_SETTING_1 = 0,
    CTT_SETTING_2 = 1,
    CTT_SETTING_3 = 2,
    NUM_CUSTOM_TUNINGS = 3
};

enum eOnlineBlueprintType {
    OBT_NONE = 0,
    OBT_SHARED = 1,
    OBT_DLC = 2
};

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

enum eLiveryID {
    LIVERYID_COP_CAR_MID_05_01 = 0,
    LIVERYID_COP_CAR_MUS = 1,
    LIVERYID_COP_CAR_GTR = 2,
    LIVERYID_COP_CAR_POR_911 = 3,
    LIVERYID_COP_SUV = 4,
    LIVERYID_BOSS_CARMEN = 5,
    LIVERYID_BOSS_CHASE = 6,
    LIVERYID_BOSS_ISF = 7,
    LIVERYID_BOSS_NICKEL = 8,
    LIVERYID_BOSS_ROSE = 9,
    LIVERYID_BOSS_ZACK = 10,
    LIVERYID_BOSS_INTRO = 11,
    LIVERYID_MEGANE = 12,
    LIVERYID_BOSS_GMAC = 13,
    LIVERYID_TERLINGUA = 14,
    LIVERYID_BOSS_HECTOR = 15,
    LIVERYID_DCAST_GT2 = 16,
    LIVERYID_DCAST_R8 = 17,
    LIVERYID_DCAST_CAMARO = 18,
    LIVERYID_DCAST_CHARGER = 19,
    LIVERYID_DCAST_VIPER = 20,
    LIVERYID_DCAST_MUSTANG = 21,
    LIVERYID_DCAST_MURCIELAGO = 22,
    LIVERYID_DCAST_ISF = 23,
    LIVERYID_DCAST_MPS3 = 24,
    LIVERYID_DCAST_GTO = 25,
    LIVERYID_DCAST_GTR = 26,
    LIVERYID_DCAST_911_TURBO = 27,
    LIVERYID_DCAST_GOLF = 28,
    LIVERYID_DCAST_CAMARO2 = 29,
    LIVERYID_BOSS_ROSES = 30,
    LIVERYID_MAX = 31,
    LIVERYID_NONE = 255
};

enum eMusicBitfields {
    PURSUIT = 0,
    CHASEDOWN = 1,
    RACE = 2,
    MAPSCREEN = 3,
    CARHEAVEN = 4
};


enum FEScreenID {
    main = 0,
    Options = 3,
    PostRace_0x4 = 4,
    PostRace_0x5 = 5,
    PostMissionWin = 6,
    PostMissionLoss = 7,
    PostRace_0x8 = 8,
    LevelUp = 9,
    LoadingScreen = 10,
    MissionLoadingScreen = 11,
    LongLoadingScreen = 12,
    OnlineChyron = 13,
    SaveIcon = 14,
    SaveIcon_Dialog = 15,
    PCCreateAccount = 16,
    OnlineGameRoom = 17,
    OnlineGRSettings = 18,
    OnlineTOS = 19,
    MessengerScreen = 20,
    MessengerChat = 21,
    Leaderboardscreen = 22,
    NewsScreen = 23,
    PlayerProfile = 24,
    PlayerFeedback = 25,
    OnlinePCLogin_0x1a = 26,
    OnlinePCLoginSelection = 27,
    OnlinePCLogin_0x1c = 28,
    PCOnlinePersona = 29,
    PCForgorPassword = 30,
    CountrySelection = 31,
    CodeEntry = 32,
    ControllerUnplugged = 34,
    GenericError = 35,
    NISOverlay = 36,
    FontTest = 37,
    DebugCarTest = 38,
    TreeMenu = 39,
    TreeMenu_Customize = 40,
    WidgetTest = 41,
    WidgetTest2 = 42,
    Dialog = 43,
    ColorPickerTest = 44,
    DropdownTest = 45,
    SliderTest = 46,
    ScrollbarTest = 47,
    TextListTest = 48,
    TickerTest = 49,
    ToggleTest = 50,
    ThumbnailTest = 51,
    ChyronTest = 52,
    TreeTest = 53,
    StaticItemScrollerTest = 54,
    Legal = 56,
    LanguageSelect = 57,
    MovieScreen = 58,
    SplashScreen = 59,
    PCSaveSelectMenu2 = 60,
    PSAScreen = 61,
    ChallengeSeries = 62,
    ChallengeSeriesSummary = 63,
    ChallengeRewardsStreet = 64,
    ChallengeRewardsPro = 65,
    ChallengeRewardsDominate = 66,
    ChallengeSeriesNumPlayers = 67,
    ChallengeKeyBoardPC = 68,
    CarSelectMenu = 69,
    CarsMenu = 70,
    MyCarsMenu = 72,
    PinkSlip = 73,
    CarShopMenu = 74,
    VinylSelectMenu = 75,
    VinylTransformMenu = 76,
    VinylPaintMenu = 77,
    PaintMenu = 78,
    AutosculptMenu = 79,
    PerformanceMenu = 80,
    PerformanceTuning = 81,
    PhotoMode = 82,
    DriverSkill = 83,
    PurchaseDialog = 84,
    Marketing = 85,
    DistrictView = 86,
    WorldView = 87,
    CustomizeQuickRace = 88,
    Sung = 89,
    Kyla = 90,
    Mimi = 91,
    Oscar = 92,
    Alex = 93,
    Danny = 94,
    Jesh = 95,
    VideoCalibration = 96,
    PCControllerConfig = 97,
    Credits = 98,
    HintsTips = 99
};

enum AttribTypes {
    Int64 = 0x935FF2F8,
    Int32 = 0x5763DA41,
    Int16 = 0x391C6E95,
    Int8 = 0x6F27B5BC,
    UInt64 = 0xADEC51AA,
    UInt32 = 0x939992BB,
    UInt16 = 0xE51A99C1,
    UInt8 = 0x671ECBE2,
    Char = 0x019C17B0,
    Bool = 0x064BEC37,
    Float = 0x3C16EC5E,
    Double = 0x9ABBEF67, //ok
    Text = 0xA3F0C234,
    Reference = 0xB552FF33,
    Type = 0x3487251A,
    KeyValueAttrib = 0xBA748E69,
    Blob = 0x7D8DC244,
    RefSpec = 0x2B936EB7,
    Definition = 0x2BB9E5B4,
    Symbol = 0x89D94738,
    Colour = 0xCCE98A52,
    FloatColour = 0xFEA2B990,
    ShaderColour = 0x6F325D4D,
    Matrix = 0xA1E54784,
    Quaternion = 0xE17F3FD7,
    Vector4 = 0x34FDE6BB,
    Vector3 = 0xD680287D,
    Vector2 = 0x56937AA4,


    StringKey = 0x5ED182D4,
    StringKey_ = 0xA502A824,









    AICollisionReactionRecord = 0xAA229CD7,
    AIDriverSubclass = 0xB08C8F40,
    AIOpponentGoal = 0x2648ABD6,
    AIOpponentGoal_ = 0x4853C461,
    AUDENGLOOPVOLUMESst = 0xCE9C0458,
    AUDENGRPMRANGEst = 0x8FD96119,
    AirSupport = 0xE411510A,
    AirSupportStrategy = 0x0363ED8B,
    Alias = 0xCF01CC82,
    Alias_ = 0xF25E609B,
    AtttributeRefSpec = 0xC616662D,
    AudioOptions = 0x4B1F5790,
    AudioOptions_ = 0x9D60F6DC,
    AutoSculptRegionID = 0x1B958CDF,
    AutoSculptZoneID = 0x46E3E51F,
    AxlePair = 0x4CB36381,
    AxlePairCurve = 0x9DF6FF3B,
    BattleMusicTuning = 0x59E599D8,
    BattleMusicTuning_ = 0x6D49C297,
    BinarySequence = 0x1E11A179,
    BinarySequence_ = 0x92CD53C4,
    BluePrintType = 0x742B6254,
    CAR_PART_ID = 0xF75C59C4,
    CAR_SLOT_ID = 0x6D90E3A1,
    CCarDamageEntry = 0xAB02FD7E,
    CCarDamageLevels = 0xFFE92A3B,
    CCarDamageMarkerEntry = 0xE981F513,
    CCarDamageThreshold = 0xB5AAB497,
    CCarKitSlotEntry = 0xD552C548,
    CCarSlotEntry = 0x3BD01332,
    COLLISIONSFX_LAYERS = 0xF1A59923,
    CPartsPackageEntry = 0x87C75A65,
    CWindTunnelPartEntry = 0x505CCF56,
    CameraCurveReactionRecord = 0x414C4280,
    CameraReactionRecord = 0x080BEB94,
    CameraReactionType = 0xE2BDD6C8,
    CameraSpeedReactionRecord = 0xE8E8CF0C,
    CarBlendingExtremum = 0xF3867192,
    CarBodyMotion = 0x1E9400AE,
    CarClassType = 0x99BA2AF0,
    CarPaintSwatch = 0xC40EA1D6,
    CareerDesignerStatus = 0xC33B80BA,
    CareerDesignerStatus_ = 0xEE2F7738,
    CareerElementDrawStyle = 0xBCC534A2,
    CareerElementDrawStyle_ = 0xF32D93B9,
    ChallengeCategory = 0xD8B47731,
    ChallengeCategory_ = 0xF96509A1,
    ChasedownMusicTuning = 0x31BFD7DC,
    ChasedownMusicTuning_ = 0x57081877,

    ClassRefSpec_adaptivedifficulty = 0x3B9EC747,
    ClassRefSpec_aggression = 0x99796B1F,
    ClassRefSpec_aicurves = 0xCBCC5FEC,
    ClassRefSpec_aiperformance = 0x0FE1FFBE,
    ClassRefSpec_aivehicle = 0x17D035E8,
    ClassRefSpec_aud_moment = 0xE520B426,
    ClassRefSpec_aud_wall_reverb = 0xFCE230CB,
    ClassRefSpec_aud_world_reverb = 0x275B0702,
    ClassRefSpec_audio_global_tuning = 0x430EED7B,
    ClassRefSpec_audio_moment_vo = 0x7D5BC5EE,
    ClassRefSpec_audio_revlimiter = 0x838D764F,
    ClassRefSpec_audioimpact = 0xA3D9C111,
    ClassRefSpec_audiosystem = 0x555ADADB,
    ClassRefSpec_camera = 0x896794C7,
    ClassRefSpec_camerabraking = 0x63A41DC3,
    ClassRefSpec_cameracorner = 0xF20F3983,
    ClassRefSpec_cameradodge = 0xF682F046,
    ClassRefSpec_cameradriftrace = 0x4F603439,
    ClassRefSpec_cameragearchange = 0x7B006A4B,
    ClassRefSpec_cameraimpact = 0xE4059F6B,
    ClassRefSpec_camerainfo = 0x824E28B4,
    ClassRefSpec_cameranos = 0xFC2EB0C4,
    ClassRefSpec_cameraracestart = 0xB1C38001,
    ClassRefSpec_cameratrackcarshake = 0xF0879BCB,
    ClassRefSpec_cameravibration = 0x9FAF8F0B,
    ClassRefSpec_car = 0x80560081,
    ClassRefSpec_car_art = 0x0DD6DFDE,
    ClassRefSpec_car_damage_materials = 0x1A05D004,
    ClassRefSpec_car_damage_objects = 0x5CFD66E0,
    ClassRefSpec_car_kit = 0x9A8FF3CF,
    ClassRefSpec_car_kits = 0x0003240E,
    ClassRefSpec_car_list = 0xC9BC36B2,
    ClassRefSpec_car_paint_group = 0xCE7C5CAE,
    ClassRefSpec_car_paint_swatch = 0x40958E27,
    ClassRefSpec_car_paintable_zone = 0x8EE1BB7A,
    ClassRefSpec_car_part = 0xCD16B4FB,
    ClassRefSpec_car_parts = 0xD2B95641,
    ClassRefSpec_car_tuning = 0xD8BC6F1D,
    ClassRefSpec_caraudio = 0x4CB0EDAE,
    ClassRefSpec_career_constants = 0x9C73F501,
    ClassRefSpec_channel_packing_samplers = 0xB8F51507,
    ClassRefSpec_chopperspecs = 0x7B7F3A33,
    ClassRefSpec_collisionreactions = 0x4D59838F,
    ClassRefSpec_collisionsfx_core = 0xE35281B3,
    ClassRefSpec_collisionsfx_damage = 0xCB437D01,
    ClassRefSpec_collisionsfx_object = 0x97A3B30A,
    ClassRefSpec_collisionsfx_surface = 0x8867D5CC,
    ClassRefSpec_collisionsfx_sweetener = 0x9F23BD21,
    ClassRefSpec_controller = 0xDCC57E4D,
    ClassRefSpec_cost_to_state = 0x505D2204,
    ClassRefSpec_damage_global = 0x9F06FA2E,
    ClassRefSpec_damage_morph_levels = 0xF736EE3D,
    ClassRefSpec_damagespecs = 0x9AABE8C9,
    ClassRefSpec_drag_shift_error = 0x0E59E9BA,
    ClassRefSpec_driver_assist = 0xA4AA2E20,
    ClassRefSpec_dynamicsurface = 0xC6FF657E,
    ClassRefSpec_ecar = 0xED80D77A,
    ClassRefSpec_effects = 0x2D792CD6,
    ClassRefSpec_emitterdata = 0xC4EA6E30,
    ClassRefSpec_emittergroup = 0xBE3E40F0,
    ClassRefSpec_emitteruv = 0x391E23EB,
    ClassRefSpec_encounters = 0x25F495FC,
    ClassRefSpec_engineaudio = 0xBBFFF5F5,
    ClassRefSpec_engineaudio_ng = 0xD35198D2,
    ClassRefSpec_enginedamage = 0x59243A3E,
    ClassRefSpec_explosion = 0x2D783096,
    ClassRefSpec_fe_global = 0xC2D53629,
    ClassRefSpec_fe_hints_tips = 0x7AB596D6,
    ClassRefSpec_fe_tuning_sliders = 0xAFB631C5,
    ClassRefSpec_fecameras = 0x139766B8,
    ClassRefSpec_fecooling = 0xC62159D3,
    ClassRefSpec_feicecameras = 0x0F26903B,
    ClassRefSpec_force_feedback = 0x829E2477,
    ClassRefSpec_forced_induction = 0xB32F3BB3,
    ClassRefSpec_frontend = 0x312D2407,
    ClassRefSpec_frontend_sfx = 0x224AD219,
    ClassRefSpec_fuelcell_effect = 0x4A7FA87A,
    ClassRefSpec_fuelcell_emitter = 0x00CAB17E,
    ClassRefSpec_gameplay = 0x6D5165E0,
    ClassRefSpec_geoheatlevels = 0xEFF9B12F,
    ClassRefSpec_glue = 0xC98B6ABA,
    ClassRefSpec_headlight_sequence = 0x9E912857,
    ClassRefSpec_heat = 0x6E0E8914,
    ClassRefSpec_highway_pattern = 0x626C8E46,
    ClassRefSpec_highway_pattern_sequence = 0xB25A93A3,
    ClassRefSpec_infraction_pursuit = 0x300B1F60,
    ClassRefSpec_lensflare = 0xF827D93D,
    ClassRefSpec_light_flares_cg = 0x93AC3662,
    ClassRefSpec_lightmaterials = 0x3417CB77,
    ClassRefSpec_lightshaders = 0x27E47039,
    ClassRefSpec_linemeshstyle = 0x1DB98C0D,
    ClassRefSpec_loadingscreen = 0x40DA2F28,
    ClassRefSpec_materials = 0xB838E033,
    ClassRefSpec_milestonetypes = 0x8AB35F23,
    ClassRefSpec_mipmapbias = 0xE8A72737,
    ClassRefSpec_mistakes = 0xA31D89C4,
    ClassRefSpec_music = 0x35967387,
    ClassRefSpec_nis = 0xB76D165B,
    ClassRefSpec_nis_car_sets = 0x9BD4D5A9,
    ClassRefSpec_nis_generic_cameras = 0x33E69930,
    ClassRefSpec_nosfx = 0x3BE28889,
    ClassRefSpec_ocean = 0x5F1F550E,
    ClassRefSpec_package = 0x9B9897F3,
    ClassRefSpec_part_art = 0x4E3C9B40,
    ClassRefSpec_part_audio = 0x049CF22D,
    ClassRefSpec_part_economics = 0x433FC24C,
    ClassRefSpec_part_fe_quickupgrade = 0xE2B95100,
    ClassRefSpec_part_fedata = 0x384F2C4A,
    ClassRefSpec_part_physics = 0x08E9027C,
    ClassRefSpec_partoptions = 0x0B60A748,
    ClassRefSpec_performance_merit_functions = 0xB95A463D,
    ClassRefSpec_performance_stats = 0x0D8E306C,
    ClassRefSpec_performance_tiers = 0x4C8ED6B1,
    ClassRefSpec_performancematch = 0xF6DCB84F,
    ClassRefSpec_physics_special_slider = 0x4DCC47A3,
    ClassRefSpec_physics_tuning = 0x974622FC,
    ClassRefSpec_physics_tuning_slider = 0xEBC7A7AA,
    ClassRefSpec_physics_tuning_slider_list = 0x6ACFA694,
    ClassRefSpec_physics_upgrade_set = 0xCB0A8920,
    ClassRefSpec_physicsdamage = 0x77B4609F,
    ClassRefSpec_postfx = 0x7999A426,
    ClassRefSpec_presetride = 0x75F2A913,
    ClassRefSpec_pursuit_threat_select = 0x88E31989,
    ClassRefSpec_pursuit_threat_select_xp = 0x721BBD0F,
    ClassRefSpec_pursuitaudio = 0x2D0383A1,
    ClassRefSpec_pursuitescalation = 0x97675A73,
    ClassRefSpec_pursuitescalationpatterns = 0x7DBFD896,
    ClassRefSpec_pursuitlevels = 0x172FDB17,
    ClassRefSpec_pursuitsupport = 0x0C3013EF,
    ClassRefSpec_pvehicle = 0xFC7F384B,
    ClassRefSpec_randompartrules = 0x761004F2,
    ClassRefSpec_render_materials = 0x5EF67081,
    ClassRefSpec_render_param_car_lighting = 0x89477655,
    ClassRefSpec_render_state_blend = 0x49832B19,
    ClassRefSpec_render_state_depthstencil = 0xFAB5447B,
    ClassRefSpec_render_state_rasterizer = 0x738452FA,
    ClassRefSpec_render_state_sampler = 0xC3A22BB4,
    ClassRefSpec_render_test_materials = 0x59A90A88,
    ClassRefSpec_rendertargets = 0x542CF0CD,
    ClassRefSpec_results = 0x1D9CD7E0,
    ClassRefSpec_resultsimulation = 0x930CC24A,
    ClassRefSpec_rewardcard = 0x264AE49F,
    ClassRefSpec_rewardcard_rewards = 0xBD63E15D,
    ClassRefSpec_rigidbodyspecs = 0xFF1A818C,
    ClassRefSpec_road_blocks = 0x95641288,
    ClassRefSpec_shader_distance_fx = 0x15F219E8,
    ClassRefSpec_shader_parameters = 0x93C6AE43,
    ClassRefSpec_shader_parameters_car_materials = 0xB024E2BF,
    ClassRefSpec_shader_paramters_procedural = 0xF058854C,
    ClassRefSpec_shaderball_material_params = 0xBBE0AA91,
    ClassRefSpec_shiftpattern = 0xA9B8D58E,
    ClassRefSpec_simsurface = 0x342581D2,
    ClassRefSpec_skidaudio = 0xDEBF204D,
    ClassRefSpec_smackable = 0xF1E33BA0,
    ClassRefSpec_smoke = 0x6A928053,
    ClassRefSpec_speech = 0x54FEB318,
    ClassRefSpec_speechtune = 0xC4535C38,
    ClassRefSpec_style_points = 0x9DA291D6,
    ClassRefSpec_sunandsky = 0xAD788A02,
    ClassRefSpec_system = 0xFEB6027C,
    ClassRefSpec_table_data = 0x398A2DA1,
    ClassRefSpec_table_data_array = 0xBEB302C1,
    ClassRefSpec_target_time_overrides = 0xE52C58C0,
    ClassRefSpec_testshaderparam = 0xE4465C07,
    ClassRefSpec_timeofdaylighting = 0xFDADA5E7,
    ClassRefSpec_traffic_engine = 0x6B7BFDF8,
    ClassRefSpec_traffic_horn = 0xF69EB0FF,
    ClassRefSpec_trafficpattern = 0x784A5D9B,
    ClassRefSpec_turbosfx = 0x877B2A6D,
    ClassRefSpec_visuallook = 0xAACC73D7,
    ClassRefSpec_visuallookeffect = 0x4204D0A4,
    ClassRefSpec_visuallooktransition = 0x6E12DE6C,
    ClassRefSpec_wheel_data = 0x585E4853,
    ClassRefSpec_world = 0xFF60D31F,
    ClassRefSpec_zone_visual_style = 0x05360FFF,

    CollisionReactionRecord = 0x63E3B021,
    CollisionStream = 0x57D382C9,
    ColorChannels = 0x718CB300,
    ColorChannels_ = 0xA55664DE,
    Context = 0x7AFC691D,
    Context_ = 0x93B61991,
    Context__ = 0xE84B42E6,
    Control = 0x31866691,
    Control_ = 0xBA089C9C,
    ControllerDataRecord = 0x1F5D923A,
    CopCountRecord = 0xFCAA46E2,
    CopDensity = 0xAD405270,
    CopDensity_ = 0xDBC08D32,
    CopFormationRecord = 0xB5A53D76,
    CornerMarker = 0x587D3FCB,
    CornerMarker_ = 0xFE4F45B2,
    CubeType = 0xDDB24C12,
    Curve = 0xB2D362A1,
    DamageScaleRecord = 0xD99B853C,
    DecalLayer = 0x4549C67A,
    DecalLayer_ = 0x7107CFD8,
    DefinitionKey = 0x15275D52,
    Difficulty = 0x72B813A5,
    Difficulty_ = 0x8D1F4774,
    DummyEnum = 0x2AA174B4,
    DynamicSizeArray = 0xEA9A10CB,
    EAIBreakerType = 0x49639447,
    EAILaneChangeType = 0xF4F814C4,
    EMomentChokeGroup = 0x0CA582F8,
    EMomentChokeGroup_ = 0x5E0820F5,
    EMomentPanning = 0x450C4028,
    EMomentPanning_ = 0xFDD4023E,
    EffectLinkageRecord = 0x341F03A0,
    EffectParticleAnimation = 0xF5149B3B,
    EffectParticleConstraint = 0xA0CB89D4,
    EmittedGeo = 0x3BDB80C4,
    EmittedGeometryRecord = 0x359E0302,
    EmitterDataRecord = 0xE9702BA9,
    EmitterPostProcessingEffect = 0x568082C1,
    EmitterShaderTechnique = 0xED7874BB,
    EndTransition = 0x281E65A9,
    EndTransition_ = 0xDC78ADE6,
    EngineLFOParams = 0xA575DBA0,
    EnginePosition = 0x009C0AE6,
    EventRegion = 0x76E55E0B,
    EventRegion_ = 0xCA77FFC6,
    FECarPartInfo = 0x04B6DAA0,
    FEDefaultUnlockable = 0x405AB2E7,
    FEHintsData = 0xAE88E5CA,
    FEMsgToSoundTrigger = 0xAF14A7A1,
    FEPartCamera = 0x6746B364,
    FEPartData = 0x3DD829BA,
    FEPartDetail = 0x51359682,
    FEPerfSliderTextures = 0xCF14B958,
    FEQuickUpgrade = 0x1C95C17F,
    FEQuickUpgradeEntry = 0xE8B52287,
    FESoundEvent = 0x6ADBD08F,
    FETuningSlider = 0x1DFE9335,
    FEVinylGroupInfo = 0x96702A62,
    FFBWaveRecord = 0x112CB2B1,
    FXROADNOISE_LOOP = 0xD89C6E4A,
    FXROADNOISE_TRANSITION = 0x7B36353A,
    FormationType = 0xBBA1B025,
    GBarrierInfo = 0xE1A8BDE6,
    GCollectionKey = 0x6CD4B70A,
    GMapCurve = 0xDA1597EE,
    GMapCurvePoint = 0x4050A8A0,
    GMapCurveRef = 0xA181DDF2,
    GMapRegionInfo = 0x16AB1E0C,
    GMapTriangle = 0x4EDDE921,
    GStringHash = 0x9F702B5E,
    GameMode = 0x5F101046,
    GameMode_ = 0x8E3162A7,
    GlueCurve = 0xC97ED338,
    GraphPoint = 0x4034DE8F,
    HeavySupport = 0xF57A6AD1,
    HeavySupportStrategy = 0x0BAAC53F,
    HighwayPatternCarData = 0x6A70DD0A,
    ICEContextIDs = 0xE7288AD9,
    IndexedBarrierName = 0x00E5F318,
    InputUpdateType = 0x78CE38D3,
    JittererParams = 0x2AF832F7,
    JittererParams_ = 0x8DD533BD,
    JunkmanMod = 0x51EAD18D,
    KIT_TYPE = 0xD3E0143E,
    Key = 0xE75FDCFE,
    LeaderSupport = 0x06885323,
    LeaderSupportStrategy = 0x2FDA332F,
    LightFlash = 0xB2705EB6,
    MiniRefSpec = 0xE1B276B8,
    MissionType = 0x6473E3C3,
    MissionType_ = 0x790E4706,
    ModifyScalarValue = 0xC7B8909E,
    MovieVolume = 0x10894738,
    MultiSampleMode = 0x6A3A7CE8,
    MultiSampleMode_ = 0x7DF4F1A0,
    NISActor = 0xBD76AE13,
    NISActor_ = 0xCEA9E5CD,
    NISCar = 0x2FC87F82,
    NISCar_ = 0x37D45D8F,
    NISLength = 0x06B79C29,
    NISLength_ = 0x53093F3D,
    PartType = 0x47BC0E93,
    ParticleAnimationInfo = 0x4021E2C0,
    ParticleTextureRecord = 0x3DFD8048,
    PathEventEnum = 0x73364C3D,
    PerformanceStatsRange = 0xC0A0D87D,
    PerformanceWeight = 0x371B3A60,
    PerformanceWeightWithDirection = 0x30034799,
    PhysicsTuningDescription = 0x7A310DE7,
    PhysicsTuningPreset = 0xE2B2CA41,
    PhysicsTuningSliderUnlock = 0x287029A1,
    PhysicsUpgrade = 0x59AA6FC2,
    PixelFormatType = 0x26CAB531,
    PixelFormatType_ = 0xB0B69EA3,
    PresetRideAutosculpt = 0x16C13F1D,
    PresetRidePaint = 0x16DCF3AF,
    PresetRidePart = 0x78AADAD6,
    PresetRideRandomPaint = 0x99CFB824,
    PursuitMusicTuning = 0x4CFF857F,
    PursuitMusicTuning_ = 0xF73B9CF2,
    RBElementType = 0x6C059076,
    ROLLFX_LOOP = 0x46CFF82B,
    RPMLOOPPOINTSst = 0x081E39B5,
    RUMBLEFX = 0xE84055D0,
    RWAUDIOSB_Ambience_AssetKeys = 0x7276D14A,
    RWAUDIOSB_CrashSweetners_AssetKeys = 0xD31DFFDD,
    RWAUDIOSB_LoadMusic_AssetKeys = 0xDB479526,
    RWAUDIOSB_MiscFx_AssetKeys = 0xE85C6318,
    RWAUDIOSB_NIS_AssetKeys = 0x2BC0C860,
    RWAUDIOSB_NIS_PLD_AssetKeys = 0x9F3A6700,
    RWAUDIOSB_OnlineMusic_AssetKeys = 0x576F8BA4,
    RWAUDIOSB_PursuitBreakers_AssetKeys = 0xFD35F53E,
    RWAUDIOSB_SpeedBreakers_AssetKeys = 0x584242E1,
    RaceCarClass = 0x4C753BB0,
    RaceCarClass_ = 0x78029580,
    RaceLigthingMode = 0x53508C1A,
    RaceLigthingMode_ = 0xF2D10201,
    RenderTargetMode = 0x05096AF8,
    RenderTargetMode_ = 0xCDD3FEB4,
    RenderVector = 0x5D1C480A,
    RenderVector_ = 0x84C51CCD,
    RewardType = 0x1763812B,
    RewardType_ = 0xF490DEC7,
    RoadNoiseRecord = 0xFFDB013B,
    RoadblockElement = 0x04CF7A40,
    RoadblockSetup = 0x4F0FB095,
    RwacBanks = 0x83340CE2,
    RwacSampleBankAsset = 0x8E1D2241,
    SCRAPE_SURFACES = 0xE28D189D,
    SPCHType_1_EventID = 0x79419C16,
    SPEECH_CAR_GENERATION = 0xA3BE6E73,
    SPEECH_RIVAL_MAKE = 0x7B77A602,
    SPLICE_DAMAGE = 0x82E18E30,
    SURFACE_SFX = 0x232A8DAA,
    SceneRoot = 0x35D5660E,
    SceneRoot_ = 0xCE2A4158,
    ScissorData = 0x25C740A9,
    ScissorData_ = 0x35A183C1,
    ScreenSizeMode = 0xBE776A2C,
    ScreenSizeMode_ = 0xD288D25A,
    ShortcutType = 0x84681293,
    ShortcutType_ = 0xED1C10B8,
    SirenMode = 0x54BC5731,
    SirenMode_ = 0xE9F5630E,
    SirenSequence = 0x665122ED,
    SirenSequence_ = 0x9DBE486F,
    SirenTuning = 0x1F464919,
    SirenTuning_ = 0x22535A69,
    Slider = 0x21877E90,
    Slider_ = 0x3A197637,
    SpecialSliderType = 0xA864824F,
    Splicer_Collision_Core = 0xD089F79B,
    Splicer_Collision_Object = 0x3BB40738,
    Splicer_Collision_Surface = 0xFC1DA2D3,
    Splicer_FrontEnd_Common = 0x413DA5D9,
    Splicer_FrontEnd_Common_ = 0x566F980B,
    State_BlendFunc = 0x8C4D0D20,
    State_BlendFunc_ = 0xA8F758FD,
    State_BlendInput = 0x23219802,
    State_BlendInput_ = 0x5A64C73E,
    State_BlendOp = 0xA2D932F7,
    State_BlendOp_ = 0xA3EF14DC,
    State_BlendRenderTargets = 0x2C555185,
    State_BlendRenderTargets_ = 0xCB51609B,
    State_Conv_PS3 = 0x5B10522A,
    State_Conv_PS3_ = 0xE1A7C173,
    State_GPUFunc = 0xE8DC3CCD,
    State_GPUFunc_ = 0xF3BAA571,
    State_GammaCorrection_PS3 = 0x55913B59,
    State_GammaCorrection_PS3_ = 0xA2B5E93F,
    State_RasterizerCullMode = 0x3C963F94,
    State_RasterizerCullMode_ = 0x69BF1078,
    State_RasterizerFillMode = 0x3B406E9A,
    State_RasterizerFillMode_ = 0x942529B4,
    State_RasterizerFrontFace = 0x5EAB480C,
    State_RasterizerFrontFace_ = 0x74297319,
    State_RasterizerShadeMode = 0x295040ED,
    State_RasterizerShadeMode_ = 0xC73DCB51,
    State_SamplerAddressingMode = 0x3A8A6CE7,
    State_SamplerAddressingMode_ = 0xE72B6001,
    State_SamplerFilterMode = 0x6B5C5464,
    State_SamplerFilterMode_ = 0x858A6D79,
    State_StencilOp = 0x9CB232E6,
    State_StencilOp_ = 0xAEF8ADA0,
    State_TrilinearThreshold_PS3 = 0x76B4555B,
    State_TrilinearThreshold_PS3_ = 0xD633592C,
    State_TrilinearThreshold_XENON = 0x0B7C6648,
    State_TrilinearThreshold_XENON_ = 0x730F94BF,
    State_XBOX2HiCompare = 0x36DE8ECC,
    State_XBOX2HiCompare_ = 0xB2DDDFA3,
    State_ZFunc_PS3 = 0x16379E68,
    State_ZFunc_PS3_ = 0xB66655AE,
    SteeringSensitivityParameter = 0xBF6C1E07,
    StitchCollisionVol = 0x4F0FDF2D,
    SurfaceEffectType = 0xDF7F20E2,
    TargetTimeOverrides = 0x0C379F75,
    TextureTypes = 0xDE061D64,
    TextureTypes_ = 0xE7DF191D,
    Tier = 0x52A448F9,
    Tier_ = 0x99290B31,
    TireCondition = 0x0955DDC6,
    TireEffectRecord = 0x681D219C,
    TireTimeEffectRecord = 0x537FB992,
    TrafficLevel = 0x7F05115E,
    TrafficLevel_ = 0xC64BC341,
    TrafficPatternRecord = 0x600994C4,
    Type_ = 0xA864BF9A,
    Type__ = 0xD3CFC82D,
    Type_car_color = 0xD52BF3B0,
    Type_car_type = 0x9F9286D3,
    Type_offroad_moment_id = 0x298A676B,
    Type_pursuit_breaker_type = 0x22BC5CFD,
    Type_vo_linenumber = 0x72454D07,
    UCrc32 = 0xF88CD37C,
    UnlockableState = 0x9AADD733,
    UnlockedState = 0xEC8170BA,
    UpgradeDescription = 0xBD74A9C1,
    UpgradeSpecs = 0x5FDE6463,
    VariableArray = 0x1C4A7057,
    VinylColor = 0x169BEC20,
    VinylColor_ = 0x30DF23FA,
    VinylFinish = 0x37CF5E49,
    VinylFinish_ = 0xF3E9CA99,
    VinylLayer = 0x9F52A35B,
    VinylLayer_ = 0xDD0C923B,
    VinylTransform = 0xB32636EF,
    VinylTransform_ = 0xE897C570,
    WORLDOBJECT_TYPES = 0x104D1B21,


    cBlendStateAttribDefinition = 0x28759E4C,
    cBlendStateAttribDefinition_ = 0xDF74586B,
    cDepthStencilStateAttribDefinition = 0x2AEA2C9D,
    cDepthStencilStateAttribDefinition_ = 0xC5A5D0B7,
    cRasterizerStateAttribDefinition = 0x1A94D460,
    cRasterizerStateAttribDefinition_ = 0xCA6F23DD,
    cRenderTargetAttribDefinition = 0x329C08A0,
    cRenderTargetAttribDefinition_ = 0x4B1D33AA,
    cSamplerStateAttribDefinition = 0x0A5BA7FD,
    cSamplerStateAttribDefinition_ = 0x5CE9017B,
    car_color = 0xF823EB26,
    car_type = 0xAE21258F,

    eBustedLevel = 0xB170A516,
    eBustedLevel_ = 0xD19969B3,
    eCURVETYPE = 0xAD9820D4,
    eControllerHudType = 0x43EF89C7,
    eControllerTypeMask = 0xF1FCDFB9,
    eDRIVE_BY_TYPE = 0xDB9D3A16,
    eDecalType = 0x762DED91,
    eDifferentialType = 0x462FDD03,
    eDifferentialType_ = 0xF09060A3,
    eENGINE_GROUP = 0x9C2A3CD2,
    eEndTransitionType = 0x9D7CB654,
    eEndTransitionType_ = 0xDF20EC4E,
    eFEPartUpgradeLevels = 0xD3F69B06,
    eFinishOutcome = 0x01A64C32,
    eFinishOutcome_ = 0x84287366,
    eInductionType = 0x041A914B,
    eInductionType_ = 0xFE742442,
    eLiveryID = 0x14018358,
    eMenuSoundTriggers = 0xED385B12,
    eModifyValueType = 0x3C280051,
    eOnlineStatus = 0xF25643E2,
    eOnlineStatus_ = 0xF59ADED6,
    ePS_NIS_en_index = 0xB93E97DF,
    ePackageType = 0xEBEB9C20,
    ePaintMaterialIndex = 0x46681428,
    ePaintSlot = 0xC763DB47,
    ePaintUsage = 0x6EFAFAF8,
    ePartMaterialFlags = 0x1E1842E2,
    ePerformanceType = 0x6A9EE5D5,
    ePlayerUnderAIControl = 0x7C435EF9,
    ePlayerUnderAIControl_ = 0x9BC98E7E,
    ePresetCarFilter = 0xE9A0CD1F,
    eQuickUpgradeLevels = 0x43EE1EBF,
    eQuickUpgradePackages = 0x72971B74,
    eRPMLOOPSETTING = 0xDBB34901,
    eRaceType = 0x2FCE6BBB,
    eRaceType_ = 0x4E1CA94B,
    eRewardCardArea = 0x09DF5F33,
    eRewardCardConditionModifyer = 0x30D15302,
    eRewardCardConditions = 0x11A808AD,
    eRewardCardDifficulty = 0xBB07BFEF,
    eRewardCardOnlineStat = 0x6FF82534,
    eRewardCardUnit = 0x3B90A15A,
    eSFXOBJ_MOMENT_MIXINPUT = 0x0F74035E,
    eSFXOBJ_MOMENT_MIXOUT = 0x8239AE32,
    eSFX_STATE = 0xD064F6C3,
    eSNDCTLSTATE = 0xF5D9D3DE,
    eSNDDATAPATH = 0xDCCC5503,
    eSNDDATATYPE = 0xD3382D47,
    eSNDDATGRP = 0x17A3FE23,
    eSceneRoot = 0x0A170616,
    eSceneRoot_ = 0xE485AB93,
    eSkyStyle = 0x3A5E61BF,
    eSongPlayability = 0xD2632C29,
    eSteeringCurveStyle = 0xCCB275D9,
    eTEG_ParticleTextures = 0x7AA5FA4A,
    eTEMPALLOCLOCATION = 0xDACB4F8C,
    eType = 0x51FD8C4C,
    eType_ = 0xFC48D675,
    eUnlockableEntity = 0xE4F2BDC4,
    eVehicleCategory = 0x3583A862,
    eVehicleCategory_ = 0x98176DF0,
    offroad_moment_id = 0x561F5245,
    pursuit_breaker_type = 0x48910A81,
    stShiftPair = 0x640B1DD2,
    type_bStringHash = 0x6E4C9A77,
    vo_linenumber = 0x4855F8C0,

};





struct FETreeDataNode {
    DWORD vftable;
    bNode node;
    //    DWORD field2_0x8;
    float mValue;
    DWORD mLabelHash;
    DWORD mData;
    DWORD mId;
    DWORD mType;
    char* mpLabel;
    char* mpStringVal;
    bool mbEnabled;
    //char field_0x29;
    //char field_0x2a;
    //char field_0x2b;
};
//DWORD FETreeDataNode_vftable;

struct FECarRecord {
    DWORD handle; //-1 uninitialized, 0-329 or preset
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

struct PackedVinylColor {
    byte swatch; /* //T_UCHAR */
    byte saturation; /* //T_UCHAR */
    byte brightness; /* //T_UCHAR */
};

struct FEPaintSV {
    byte s; /* //T_UCHAR */
    byte v; /* //T_UCHAR */
};

struct CarPaintRecordPacked {
    int group; /* //T_UINT4 */
    byte hue; /* //T_UCHAR */
    float saturation; /* //T_REAL32 */
    float variance; /* //T_REAL32 */
};

struct CarPaintSwatch {
    UINT rgb;
    enum ePaintMaterialIndex material_a;
    enum ePaintMaterialIndex material_b;
    float blend;
    UINT speechColour; //Csis::Type_car_color
};

namespace VinylSystem {
    struct VinylTransformPacked{
    short translationX; /* //T_SHORT */
    short translationY; /* //T_SHORT */
    byte rotation; /* //T_UCHAR */
    byte scaleX; /* //T_UCHAR */
    byte scaleY; /* //T_UCHAR */
    byte proportionalScale : 1;
    byte shear : 7;
    };
}

struct PackedVinylLayer {
    short vinylPart; /* //T_SHORT */
    struct VinylSystem::VinylTransformPacked transform; /* //VinylSystem::VinylTransformPacked */
    struct PackedVinylColor colors[4]; /* // */
};

struct PackedDecalLayer {
    short decalPart; /* //T_SHORT */
    struct VinylSystem::VinylTransformPacked transform; /* //VinylSystem::VinylTransformPacked */
    struct PackedVinylColor color; /* //PackedVinylColor */
};

struct VehicleCustomizations {
    static struct InstalledPart {
        int part_key; /* //T_UINT4 */
        int kit_num; /* //T_UINT4 */
    };

    short GUID[5]; /* // */
    int creatorId; /* //T_UINT4 */
    byte creatorName[32]; /* // */
    int shareNumber; /* //T_UINT4 */
    struct InstalledPart InstalledParts[115]; /* // */
    byte customPaintsValid[12]; /* // */
    struct CarPaintRecordPacked customPaints[12]; /* // */
    byte partBaseMaterials[12]; /* // */
    enum BluePrintType bluePrintType; /* //BluePrintType */
    enum KIT_TYPE kitType; /* //KIT_TYPE */
    int kitNum; /* //T_UINT4 */
    float RideHeight; /* //T_REAL32 */
    struct PackedVinylLayer VinylLayers[20]; /* // */
    int VinylMirrorBits; /* //T_INT4 */
    int NumVinylLayers; /* //T_INT4 */
    struct PackedDecalLayer DecalLayers[20]; /* // */
    int DecalMirrorBits; /* //T_INT4 */
    int NumDecalLayers; /* //T_INT4 */
    int LiveryID; /* //T_INT4 */
    byte MorphValues[40][11]; /* // */
    float PhysicsTuning[32]; /* // */
    int Type; /* //T_INT4 */
    enum VinylsAttrib__VinylFinish vinylFinish; /* //VinylsAttrib::VinylFinish */
    int PresetRide; /* //T_UINT4 */
    enum CAR_SLOT_ID RemovedParts[4]; /* // */
};

struct FEKitInfo {
    enum KIT_TYPE installedType; /* //KIT_TYPE */
    int installedKitNum; /* //T_UINT4 */
    int inventory[4]; /* // */
};

namespace Physics {
    struct Tunings {
        float Value[7]; /* // */
    };
}

struct FECustomizationRecord {
    short FirstCarPartRecordIndex; /* //T_USHORT */
    short FirstVinylRecordIndex; /* //T_USHORT */
    struct VehicleCustomizations mBluePrint; /* //VehicleCustomizations */
    struct FEKitInfo mBodykitInfo; /* //FEKitInfo */
    struct Physics::Tunings Tunings[3]; /* // */
    enum eCustomTuningType ActiveTuning; /* //eCustomTuningType */
    int Preset; /* //T_INT4 */
    byte Handle; /* //T_UCHAR */
    int mType; /* //T_INT4 */
    struct FEPaintSV mPaint[12]; /* // */
    enum eOnlineBlueprintType mOnlineType; /* //eOnlineBlueprintType */
    byte mBlueprintTier; /* //T_UCHAR */
};

struct DALInterface {
    static struct DALInterface_vt {
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

    DALInterface_vt* vftable;
    int mInterfaceIndex;
};
//VALUES FOR 1.18, MAY BE DIFFERENT IN 1.0

//namespace DALManager {
//    bool(__fastcall* GetInt)(void* _this, DWORD, DWORD DALValue, int* out_ptr, int param_3, int param_4, int param_5);
//}

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


struct VariableArray {
    float* mArray; /* //T_32PREAL32 */
    int mLength; /* //T_UINT4 */
};

struct Curve {
    float xMin; /* //T_REAL32 */
    float xMax; /* //T_REAL32 */
    float yMin; /* //T_REAL32 */
    float yMax; /* //T_REAL32 */
    float graphScale; /* //T_REAL32 */
    struct VariableArray X; /* //VariableArray */
    struct VariableArray Y; /* //VariableArray */
    struct VariableArray Y2; /* //VariableArray */
    byte allocatedMemory; /* //T_BOOL08 */
};

struct AxlePairCurve {
    struct Curve Front; /* //Curve */
    struct Curve Rear; /* //Curve */
};

struct FEPartData {
    DWORD HAL_ID; /* //type_bStringHash */
    DWORD CF_HAL_ID; /* //type_bStringHash */
    int price; /* //T_INT4 */
    byte showroom_unlock; /* //T_UCHAR */
    int tier1_price; /* //T_INT4 */
    int tier2_price; /* //T_INT4 */
    int tier3_price; /* //T_INT4 */
    int tier4_price; /* //T_INT4 */
    byte tier; /* //T_UCHAR */
    byte tier_1_showroom_unlock; /* //T_UCHAR */
    byte tier_2_showroom_unlock; /* //T_UCHAR */
    byte tier_3_showroom_unlock; /* //T_UCHAR */
    byte tier_4_showroom_unlock; /* //T_UCHAR */
    DWORD BrandHALId; /* //type_bStringHash */
    DWORD LogoTextureId; /* //type_bStringHash */
    DWORD DetailHash; /* //type_bStringHash */
    struct FEPartDetail* PartDetails; /* //FEPartDetail* */
    MOD_const__T_RCHAR* OfferID; /* //MOD_const__T_RCHAR* */
    byte IsOnlineLockable; /* //T_BOOL08 */
};

struct FEPartDetail {
    DWORD Logo; /* //type_bStringHash */
    DWORD Name; /* //type_bStringHash */
};

struct Timer {
    int PackedTime; /* //T_INT4 */
};

namespace bstl {
    struct base_allocator {
        MOD_const__T_RCHAR* mName; /* //MOD_const__T_RCHAR* */
    };
    struct allocator {
        struct base_allocator __inherit_0; /* //bstl::base_allocator */
    };
}
namespace eastl {
    struct ListNodeBase;
    struct ListNodeBase {
        struct ListNodeBase* mpNext; /* //eastl::ListNodeBase* */
        struct ListNodeBase* mpPrev; /* //eastl::ListNodeBase* */
    };


    struct fixed_pool_base {
        static struct Link {
            struct Link* mpNext; /* //eastl::fixed_pool_base::Link* */
        };
        struct Link* mpHead; /* //eastl::fixed_pool_base::Link* */
        int mnCurrentSize; /* //T_UINT4 */
        int mnPeakSize; /* //T_UINT4 */
    };

    struct fixed_pool_with_overflow_bstl__allocator {
        struct fixed_pool_base __inherit_0; /* //eastl::fixed_pool_base */
        struct bstl::allocator mOverflowAllocator; /* //bstl::allocator */
        int mpPoolBegin; /* //T_32PVOID */
        int mpPoolEnd; /* //T_32PVOID */
        int mnNodeSize; /* //T_UINT4 */
    };

    template <typename t1, typename t2>
    struct VectorBase {
        int mpBegin; /* //T_32PUINT4 */
        int mpEnd; /* //T_32PUINT4 */
        int mpCapacity; /* //T_32PUINT4 */
        struct bstl::allocator mAllocator; /* //bstl::allocator */
    };

    template <typename t1, typename t2>
    struct vector {
        struct VectorBase<t1, t2> __inherit_0; /* //eastl::VectorBase<unsigned int,bstl::allocator> */
    };
}

namespace Hermes {
    struct SmartHandle {
        void* mHandle;
        //struct Hermes::_h_HHANDLER__* mHandle; /* //Hermes::_h_HHANDLER__* */
    };
}

struct SndAssetRefHolder {
    pointer __vt;
    MOD_const__T_RCHAR* mpName; /* //MOD_const__T_RCHAR* */
};
struct AudioMemBase {
    pointer __vt;
};
struct SndBase{
    struct AudioMemBase __inherit_0; /* //AudioMemBase */
    void* m_pNextSFX; /* //SndBase* */
    byte m_bIsEnabled; /* //T_BOOL08 */
    struct SndAssetRefHolder mRefHolder; /* //SndAssetRefHolder */
    void* m_pDMixIO; /* //Nicotine::DMixIO* */
    void* m_pStateBase; /* //CSTATE_Base* */
    void* m_pEAXCar; /* //EAXCar* */
    short m_refCount; /* //T_USHORT */
    short m_allocatorIndex; /* //T_USHORT */
    int objectID; /* //T_INT4 */
};
struct SFXCTL {
    struct SndBase __inherit_0; /* //SndBase */
    enum eAemsUpgradeLevel m_UGL; /* //eAemsUpgradeLevel */
};
struct eastl__ListBase_Music__PathNamedValuePair_eastl__fixed_node_pool_16_32_4_0_1_bstl_allocator_ {
    struct eastl::ListNodeBase mNode; /* //eastl::ListNodeBase */
    int mSize; /* //T_UINT4 */
    struct eastl::fixed_pool_with_overflow_bstl__allocator mPool; /* //eastl::fixed_node_pool<16,32,4,0,1,bstl::allocator> */
};
struct eastl__fixed_list_Music__PathNamedValuePair_32_1_bstl__allocator {
    struct eastl__ListBase_Music__PathNamedValuePair_eastl__fixed_node_pool_16_32_4_0_1_bstl_allocator_ __inherit_0; /* //eastl::list<Music::PathNamedValuePair,eastl::fixed_node_pool<16,32,4,0,1,bstl::allocator> > */
    byte mBuffer[528]; /* // */
};
struct PathValQueue {
    struct eastl__fixed_list_Music__PathNamedValuePair_32_1_bstl__allocator __inherit_4; /* //eastl::fixed_list<Music::PathNamedValuePair,32,1,bstl::allocator> */
    struct AudioMemBase __inherit_0; /* //AudioMemBase */
};

struct SFXCTL_MusicPlayback;

namespace Sound {
    enum MusicSignal {
        StartState = -3,
        EndState = -2,
        normal = -1,
        FE_READY = 0,
        FE_PAUSE = 1,
        FE_MAPSCREEN = 2,
        FE_CARSELECT = 3,
        FE_CREDITS = 4,
        FE_START = 5,
        PS_INIT = 6,
        PS_THREAT_HI = 7,
        PS_THREAT_LO = 8,
        PS_EXCESS_TIME = 9,
        PS_LURKING = 10,
        PS_REACQUIRED = 11,
        PS_TERMINAL = 12,
        PS_EVADING = 13,
        PS_SAFE = 14,
        PS_UNSAFE = 15,
        PS_POSTRACE = 16,
        CD_INIT = 17,
        CD_LOW = 18,
        CD_MED = 19,
        CD_HI = 20,
        CD_TERMINATE = 21,
        CD_POSTRACE = 22,
        RC_INIT = 23,
        RC_RACING = 24,
        RC_POSTRACE = 25,
        RC_RESTART = 26,
        RC_TERMINAL = 27,
        RC_CUSTOM = 28,
        HB_INIT = 29,
        HB_LOW = 30,
        HB_MED = 31,
        HB_HI = 32,
        HB_FAIL = 33,
        HB_TERMINATE = 34,
        HB_POSTRACE = 35
    };
    enum Context {
        CONTEXT_PLAYER = 0,
        CONTEXT_AIRACER = 1,
        CONTEXT_COP = 2,
        CONTEXT_TRAFFIC = 3,
        CONTEXT_ONLINE = 4,
        CONTEXT_TRACTOR = 5,
        CONTEXT_TRAILER = 6,
        CONTEXT_HELICOPTER = 7,
        CONTEXT_MAX_CONTEXTS = 8
    };
    enum ControlSource {
        CONTROL_NONE = 0,
        CONTROL_HUMAN = 1,
        CONTROL_AI = 2,
        CONTROL_NIS = 3,
        CONTROL_ONLINE = 4
    };

    struct MusicLogic { /* Sound::MusicLogic extends Sound::FSM (0x10) extends AudioMemBase (0x4) */
        static struct vftable {
            pointer destructor;
            pointer FSM__Init;
            void* (__fastcall* Dispatch)(struct MusicLogic*, DWORD edx, enum Sound::MusicSignal);
            pointer FSM__Update;
            pointer FSM__GetElapsed;
            pointer FSM__GetFrameCount;
            void* (__fastcall* Tran)(struct MusicLogic*, DWORD edx, void* nextStateFunc);
        };

        struct vftable* __vt; /* //Sound::FSM */
        void* (__fastcall* mState)(struct MusicLogic*, DWORD edx, enum Sound::MusicSignal); /* //null* */
        struct Timer mTimer; /* //Timer */
        int mFrameCounter; /* //T_UINT4 */
        pointer mLastState; /* //null* */
        byte mFlags; /* //T_UCHAR */
        float mMaxTime; /* //T_REAL32 */
        char* mStateName; /* //MOD_const__T_RCHAR* */
        struct SFXCTL_MusicPlayback* mSFXCTL; /* //SFXCTL_MusicPlayback* */
        float mPlayerTopSpeed; /* //T_REAL32 */
        struct Timer mPlayerAheadTimer; /* //Timer */
        struct Timer mPlayerBehindTimer; /* //Timer */
    };

    struct BattleMusicTuning {
        float TimeAhead_HiToMed; /* //T_REAL32 */
        float TimeAhead_MedToLo; /* //T_REAL32 */
        float TimeBehind_HiToFail; /* //T_REAL32 */
        float TimeAhead_FailToHi; /* //T_REAL32 */
    };

    struct PursuitMusicTuning {
        float PlayerSpeedUR; /* //T_REAL32 */
        float PursuitUR; /* //T_REAL32 */
        float PursuitDistThresh; /* //T_REAL32 */
        float Evade2OutrunSpeedThresh; /* //T_REAL32 */
        float HighTimeLimit[2]; /* // */
        float LowTimeLimit[2]; /* // */
        float EvadeTimeLimit[2]; /* // */
        float NormalTimeLimit[2]; /* // */
        float OutrunTimeLimit[2]; /* // */
        float SafeTimeLimit[2]; /* // */
        float UnsafeTimeLimit[2]; /* // */
        float StartupDelay; /* //T_REAL32 */
        float PctMaxVel_Low; /* //T_REAL32 */
        float MinTimeLost; /* //T_REAL32 */
        float MinTimeLastCrashed; /* //T_REAL32 */
        float PctMaxVel_High; /* //T_REAL32 */
        float MaxPursuitDist; /* //T_REAL32 */
        float MinCopCohesion; /* //T_REAL32 */
    };

    struct ChasedownMusicTuning {
        float StartTimeLimit[2]; /* // */
        float LowTimeLimit[2]; /* // */
        float FailureTimeLimit[2]; /* // */
        float MediumTimeLimit[2]; /* // */
        float HighTimeLimit[2]; /* // */
        float OpponentDamageThreshold[2]; /* // */
    };
}

namespace Music {
    enum MusicProject {
        MUSICPROJ_FE_MISC = 0,
        MUSICPROJ_PURSUIT = 1,
        MUSICPROJ_HIGHWAY = 2,
        MUSICPROJ_RACE = 3,
        MUSICPROJ_NONE = 255
    };

    struct PursuitData {
        struct Sound::PursuitMusicTuning Tuning; /* //Sound::PursuitMusicTuning */
        float AvgPursuitDist; /* //T_REAL32 */
        float AvgNumCopsLOS; /* //T_REAL32 */
        int AvgNumCopsInFormation; /* //T_REAL32 */ //incorrect
        float AvgPlayerSpeed; /* //T_REAL32 */
        int SoundAIState; /* //T_INT4 */
        int CopAIState; /* //T_INT4 */
        int Flags; /* //T_UINT4 */
    };

    struct MiscGameData {
        int FEScreenID; /* //T_INT4 */
        int FEMode; /* //T_INT4 */
        int EventType; /* //T_INT4 */
        int GameModeFlag; /* //T_UINT4 */
        float RacePercentComplete; /* //T_REAL32 */
        int unk1;
        int unk2;
        struct bVector3 StartPosition; /* //bVector3 */
        struct Sound::ChasedownMusicTuning ChaseTune; /* //Sound::ChasedownMusicTuning */
        struct Sound::BattleMusicTuning BattleTune; /* //Sound::BattleMusicTuning */
    };

    struct SaveState {
        int SongID; /* //T_INT4 */
        int PathNode; /* //T_INT4 */
        int PathEvent; /* //T_INT4 */
        int PathProjID; /* //T_INT4 */
        pointer SavedState; /* //null* */
        struct Timer Timestamp; /* //Timer */
    };
}

struct SFXCTL_MusicPlayback {
    struct SFXCTL __inherit_0; /* //SFXCTL */
    int mRefCount; /* //T_INT4 */
    struct MusicLogic* mLogic; /* //Sound::MusicLogic* */
    enum Music::MusicProject mSwapProjectID; /* //T_INT4 */
    struct PathValQueue mPathValQueue; /* //PathValQueue */
    struct Music::PursuitData mPursuitData; /* //Music::PursuitData */
    struct Music::MiscGameData mGameData; /* //Music::MiscGameData */
    struct Music::SaveState mSaveState; /* //Music::SaveState */
    struct Hermes::SmartHandle mMsgTerminateMusic; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgCountdownDone; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgNISDone; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgSetWaitState; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgRestartRace; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgRestartMission; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgCredits; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgTriumph; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgEndMission; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgQuit; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgRaceAbandoned; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgCustom; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgStopCustom; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgForceRace; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgAudition; /* //Hermes::SmartHandle */
    struct Hermes::SmartHandle mMsgLoadingMusic; /* //Hermes::SmartHandle */
    int mCurrPursuitSongID; /* //T_INT4 */
    int mCurrChasedownID; /* //T_INT4 */
    int mCurrRaceIndex; /* //T_INT4 */
    int mEventID; /* //T_INT4 */
    int mCustomEventID; /* //T_INT4 */
    int mFEMode_FrameCounter; /* //T_INT4 */
    int mFEMode_cached; /* //T_INT4 */
};


namespace DALVehicleCommands {
    enum DriveTrain {
        DRIVETRAIN_FWD = 0,
        DRIVETRAIN_RWD = 1,
        DRIVETRAIN_AWD = 2
    };

    enum RaceMode {
        MODE_GRIP = 0,
        MODE_DRIFT = 1,
        MODE_DRAG = 2,
        MODE_SPEED = 3,
        MODE_MAX = 4
    };

    struct CarInfo {
        int carNameHash; /* //T_UINT4 */
        int carManuLogoHash; /* //T_UINT4 */
        int carLogoHash_pvehicleKey; /* //T_UINT4 */
        int carCost; /* //T_UINT4 */
        int region; /* //T_UINT4 */
        enum DriveTrain drivetrain; /* //DALVehicleCommands::DriveTrain */
        int handle; /* //T_UINT4 */
        enum RaceMode mode; /* //DALVehicleCommands::RaceMode */
        float damage; /* //T_REAL32 */
    };
}

namespace GRace {
    enum Tier {
        kRaceTier_None = 0,
        kRaceTier_1 = 1,
        kRaceTier_2 = 2,
        kRaceTier_3 = 3,
        kRaceTier_4 = 4,
        kRaceTier_NumTiers = 5
    };
}

namespace FE {
    struct PAINTSLOTINFO {
        enum CAR_SLOT_ID id; /* //CAR_SLOT_ID */
        enum ePaintSlot paintRegion; /* //ePaintSlot */
        const char* name;
    };
}

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
        //struct Array* mArray; /* //Attrib::Array* */
        //int mValue; /* //T_UINT4 */
        //int mOffset; /* //T_UINT4 */
        short mTypeIndex; /* //T_USHORT */
        byte mMax; /* //T_UCHAR */
        byte mFlags; /* //T_UCHAR */

        static enum Flags {
            NotAnArray = 2,
			InLayout = 0x10,
            Static = 0x20,
            DirectPointer = 0x40
        };
    };

    struct StringKey {
        int mHash32; /* //T_UINT4 */
        MOD_const__T_RCHAR* mString; /* //MOD_const__T_RCHAR* */
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
        static enum Flags {
            kArray = 1,
            kInLayout = 2,
            kIsBound = 4,
            kIsNotSearchable = 8,
            kIsStatic = 16,
            kHasHandler = 32
        };

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

        static Attrib::Definition* (__cdecl* GetDefinition)(Attrib::Definition* defStart, Attrib::Definition* defEnd, Attrib::Definition* match);
        //= (Attrib::Definition * (__cdecl*)(Attrib::Definition * defStart, Attrib::Definition * defEnd, Attrib::Definition * match))0x0;
    };

    struct ClassTable__Node {
        int mKey; /* //T_UINT4 */
        struct Class* mPtr; /* //Attrib::Class* */
        int mMax; /* //T_UINT4 */
    };

    struct Collection;
    //actually not in this namespace
    struct VecHashMap_uint_Collection_TablePolicy_1_96 {
        static struct Node {
            int mKey; /* //T_UINT4 */
            struct Attrib::Collection* mPtr; /* //Attrib::Collection* */
            int mMax; /* //T_UINT4 */
        };

        struct Node* mTable; /* //VecHashMap<unsigned int,Attrib::Collection,Attrib::Class::TablePolicy,1,96>::Node* */
        short mTableSize; /* //T_USHORT */
        short mNumEntries; /* //T_USHORT */
        short mFixedAlloc; /* //T_USHORT */
        short mWorstCollision; /* //T_USHORT */

    };

    namespace Database {
        //static Class* (__fastcall* GetNextClass)(DWORD Database, DWORD edx, DWORD index);
        extern Class* (__fastcall* GetClass)(DWORD Database, DWORD edx, DWORD classKey);
    }

    struct ClassPrivate {
        static struct CollectionHashMap {
            struct VecHashMap_uint_Collection_TablePolicy_1_96 __inherit_0; /* //VecHashMap<unsigned int,Attrib::Collection,Attrib::Class::TablePolicy,1,96> */
        };


        struct Class __inherit_0; /* //Attrib::Class */
        struct HashMap mLayoutTable; /* //Attrib::HashMap */
        struct Attrib::ClassPrivate::CollectionHashMap mCollections; /* //Attrib::ClassPrivate::CollectionHashMap */
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

        static int(__fastcall* GetData)(Collection* this_, DWORD edx, DWORD key, DWORD index);
        static bool(__fastcall* Contains)(Collection* this_, DWORD edx, DWORD key);
        static Node* (__fastcall* GetNode)(Collection* this_, DWORD edx, DWORD key, DWORD* keyptr);
        static bool(__fastcall* AddAttribute)(Collection* this_, DWORD edx, DWORD key, DWORD arraySize);
        static bool(__fastcall* RemoveAttribute)(Collection* this_, DWORD edx, DWORD key);
        static struct Attribute* (__fastcall* Get)(Collection* this_, DWORD edx, struct Attribute* storage, struct Instance* inst, DWORD key);
    };

    struct RefSpec {
        int mClassKey; /* //T_UINT4 */
        int mCollectionKey; /* //T_UINT4 */
        Collection* mCollectionPtr; /* //MOD_const__Attrib::Collection* */

        static void(__fastcall* constructor)(RefSpec* this_, DWORD unaff, RefSpec*);
        static void(__fastcall* Clean)(RefSpec* this_);
    };

    interface InstanceBase {};

    struct Instance : InstanceBase {
        struct Collection* mCollection; /* //MOD_const__Attrib::Collection* */
        void* mLayoutPtr; /* //T_32PVOID */
        int mMsgPort; /* //T_UINT4 */
        int mFlags; /* //T_UINT4 */

        static void* GetAttributePointerFromString(InstanceBase* this_, const char* key, DWORD stuff);
        static void* (__fastcall* GetAttributePointer)(InstanceBase* this_, DWORD edx, DWORD key, DWORD stuff);

        static void(__fastcall* constructor)(InstanceBase* this_, DWORD edx, Collection* node, DWORD stuff);
        static void(__fastcall* destructor)(InstanceBase* this_);

        static void(__fastcall* Get)(InstanceBase* this_, DWORD unaff, struct Attribute*, DWORD hash);
        static DWORD(__fastcall* GetCollectionKey)(Attrib::Instance* this_);
        static bool(__fastcall* Contains)(InstanceBase* this_, DWORD edx, DWORD key);
        static DWORD* (__fastcall* GetRaceTracks)(InstanceBase*, DWORD edx, DWORD);

    };

    struct Attribute {
        Instance* mInstance; /* //MOD_const__Attrib::Instance* */
        Collection* mCollection; /* //MOD_const__Attrib::Collection* */
        struct Attrib::Node* mInternal; /* //Attrib::Node* */
        int mDataPointer; /* //T_32PVOID */

        static bool(__fastcall* SetLength)(Attribute* this_, DWORD edx, DWORD length);
        static int(__fastcall* GetLength)(Attribute* this_);
        static void* (__fastcall* GetInternalPointer)(Attribute* this_, DWORD unaff, DWORD);
    };

    namespace Gen {



        //typedef Instance<frontend::_LayoutStruct> frontend;
        struct frontend : InstanceBase {
            static struct _LayoutStruct {
                MOD_const__T_RCHAR* CollectionName; /* //MOD_const__T_RCHAR* */
                int region; /* //T_UINT4 */
                int manufacturer; /* //T_UINT4 */
                byte IsCustomizable; /* //T_BOOL08 */
                int Cost; /* //T_INT4 */
                byte UnlockedAt; /* //T_UCHAR */
            };

            struct Collection* mCollection; /* //MOD_const__Attrib::Collection* */
            _LayoutStruct* mLayoutPtr; /* //T_32PVOID */
            int mMsgPort; /* //T_UINT4 */
            int mFlags; /* //T_UINT4 */

            static void(__fastcall* constructor)(frontend *this_, DWORD unaff, Attrib::RefSpec*, DWORD);
        };


        struct pvehicle : InstanceBase {
            static struct _LayoutStruct {
                struct Attrib::StringKey MODEL; /* //Attrib::StringKey */
                struct Attrib::RefSpec frontend; /* //Attrib::RefSpec */
                MOD_const__T_RCHAR* DefaultPresetRide; /* //MOD_const__T_RCHAR* */
                MOD_const__T_RCHAR* CollectionName; /* //MOD_const__T_RCHAR* */
                byte RandomOpponent; /* //T_BOOL08 */
                enum GRace::Tier RacingTier; /* //GRace::Tier */
                byte isRateable; /* //T_BOOL08 */
            };

            struct Collection* mCollection; /* //MOD_const__Attrib::Collection* */
            _LayoutStruct* mLayoutPtr; /* //T_32PVOID */
            int mMsgPort; /* //T_UINT4 */
            int mFlags; /* //T_UINT4 */

            static void(__fastcall* constructor)(pvehicle * this_, DWORD unaff, DWORD, DWORD);
        };
        //typedef Instance<pvehicle::_LayoutStruct> _pvehicle;
        //struct _pvehicle : _Instance<pvehicle::_LayoutStruct> {};

        struct zone_visual_style : InstanceBase {
            static struct _LayoutStruct {
                float SunLatitude;
                float SkyDomeRotation;
            };

            struct Collection* mCollection; /* //MOD_const__Attrib::Collection* */
            _LayoutStruct* mLayoutPtr; /* //T_32PVOID */
            int mMsgPort; /* //T_UINT4 */
            int mFlags; /* //T_UINT4 */

            static void(__fastcall* constructor)(zone_visual_style *this_, DWORD edx, DWORD node, DWORD stuff);
        };
        //typedef Instance<zone_visual_style::_LayoutStruct> _zone_visual_style;

        struct presetride : InstanceBase {
            static struct _LayoutStruct {
                MOD_const__T_RCHAR* PresetName; /* //MOD_const__T_RCHAR* */
                MOD_const__T_RCHAR* CollectionName; /* //MOD_const__T_RCHAR* */
                float RideHeight; /* //T_REAL32 */
                int PresetCar; /* //type_bStringHash */
                enum eLiveryID Livery; /* //eLiveryID */
                enum KIT_TYPE KitType; /* //KIT_TYPE */
                int KitNumber; /* //T_UINT4 */
                enum BluePrintType BluePrint; /* //BluePrintType */
                byte Tier; /* //T_UCHAR */
            };
            Collection* mCollection; /* //Attrib::Instance */
            struct _LayoutStruct* mLayoutPtr; /* //T_32PVOID */
            int mMsgPort; /* //T_UINT4 */
            int mFlags; /* //T_UINT4 */

            static void(__fastcall* constructor)(presetride* this_, DWORD unaff, DWORD, DWORD);
        };

        struct car_list : InstanceBase {
            static struct _LayoutStruct {
                MOD_const__T_RCHAR* Name; /* //MOD_const__T_RCHAR* */
            };
            Collection* mCollection; /* //Attrib::Instance */
            struct _LayoutStruct* mLayoutPtr; /* //T_32PVOID */
            int mMsgPort; /* //T_UINT4 */
            int mFlags; /* //T_UINT4 */

            static void(__fastcall* constructor)(car_list* this_, DWORD unaff, DWORD, DWORD);
        };

		struct generic : InstanceBase {
			struct Collection* mCollection; /* //MOD_const__Attrib::Collection* */
			void* mLayoutPtr; /* //T_32PVOID */
			int mMsgPort; /* //T_UINT4 */
			int mFlags; /* //T_UINT4 */

			static void constructor(InstanceBase* this_, DWORD clazz, DWORD node);
		};
    }

    extern bool(__fastcall* AddAttribute)(Collection* this_, DWORD edx, DWORD key, DWORD arraySizeMaybe);
    extern void* (__cdecl* DefaultDataArea)(DWORD length);
    extern Collection* (__cdecl* FindCollection)(DWORD classKey, DWORD collectionKey, char* collectionName);
    extern DWORD* DatabasePrivate;
}

struct DBCarPart {
    bool isValid;
    unsigned char CarTypeNameHashIndex;
    unsigned short AttributeTableOffset;
    Attrib::Collection* partAttributes;
    unsigned short kitNum;
    unsigned short materialFlags;
    unsigned char pad;
};

struct FEStateOption {
    struct bTNode<FEStateOption> __inherit_0; /* //bTNode<FEStateOption> */
    int mState; /* //T_INT4 */
    int mOption; /* //T_INT4 */
};
    
interface FEStateManager {
    static struct vftable {
        undefined(__fastcall *destructor)(FEStateManager* _this, DWORD edx, byte);
        undefined(__fastcall *OnMovieComplete)(FEStateManager* _this);
        undefined(__fastcall *OnNotifySound)(FEStateManager* _this);
        undefined(__fastcall *OnOnlineDisconnect)(FEStateManager* _this);
        undefined(__fastcall *OnOnlineGameDisconnect)(FEStateManager* _this);
        undefined(__fastcall *OnPadButton)(FEStateManager* _this, DWORD edx, undefined4);
        undefined(__fastcall *OnPadButtonRelease)(FEStateManager* _this, DWORD edx, undefined4);
        undefined(__fastcall *OnScreenTick)(FEStateManager* _this);
        undefined(__fastcall *OnMouseMoveLeft)(FEStateManager* _this);
        undefined(__fastcall *OnMouseMoveRight)(FEStateManager* _this);
        undefined(__fastcall* SetCanSkipMovie)(FEStateManager* _this, DWORD edx, bool);
        bool(__fastcall* GetCanSkipMovie)(FEStateManager* _this);
        undefined(__fastcall *Push)(FEStateManager* _this, DWORD edx, int, int, int);
        undefined(__fastcall *Start)(FEStateManager* _this);
        void (__fastcall *SetInitialOption)(FEStateManager* _this, DWORD edx, char*);
        void (__fastcall *HandleAutosave)(FEStateManager* _this);
        void (__fastcall *HandleScreenLeave)(FEStateManager* _this, DWORD edx, char*);
        undefined(__fastcall* HandleButtonPressed)(FEStateManager* _this, DWORD edx, int);
        void (__fastcall *HandleButtonHighlight)(FEStateManager* _this, DWORD edx, char*);
        void (__fastcall *HandleChildFlowStart)(FEStateManager* _this);
        undefined(__fastcall* HandleChildFlowDone)(FEStateManager* _this, DWORD edx, int);
        void (__fastcall *HandleEasterEggActivated)(FEStateManager* _this, DWORD edx, char*);
        void (__fastcall *HandleEnterComplete)(FEStateManager* _this);
        undefined(__fastcall* HandleEvent)(FEStateManager* _this, DWORD edx, int);
        undefined(__fastcall *HandleExitAll)(FEStateManager* _this);
        undefined(__fastcall *HandleClearStack)(FEStateManager* _this);
        void (__fastcall *HandleExitComplete)(FEStateManager* _this);
        undefined(__fastcall* HandleFlowDone)(FEStateManager* _this, DWORD edx, undefined4);
        undefined(__fastcall *HandleIsGameMode)(FEStateManager* _this);
        void (__fastcall *HandleMouseChanged)(FEStateManager* _this);
        void (__fastcall *HandleMouseWheelDown)(FEStateManager* _this);
        void (__fastcall *HandleMouseWheelUp)(FEStateManager* _this);
        void (__fastcall *HandleMovieComplete)(FEStateManager* _this);
        void (__fastcall *HandleMovieStarted)(FEStateManager* _this);
        undefined4(__fastcall* HandleNotifySound)(FEStateManager* _this, DWORD edx, undefined4, undefined4);
        void (__fastcall *HandleOnlineDisconnect)(FEStateManager* _this);
        void (__fastcall *HandleOnlineGameDisconnect)(FEStateManager* _this);
        undefined(__fastcall *HandleOptionHighlighted)(FEStateManager* _this, DWORD edx, char*);
        void(__fastcall* HandleOptionSelected)(FEStateManager* _this, DWORD edx, DWORD, undefined4, undefined4); //FETreeNodeID
        void (__fastcall *HandlePadAccept)(FEStateManager* _this);
        undefined(__fastcall* HandlePadBack)(FEStateManager* _this, DWORD edx, undefined4);
        void (__fastcall *HandlePadDown)(FEStateManager* _this);
        undefined(__fastcall* HandlePadButton0)(FEStateManager* _this);
        undefined(__fastcall* HandlePadButton1)(FEStateManager* _this);
        void (__fastcall *HandlePadButton2)(FEStateManager* _this);
        void (__fastcall *HandlePadButton3)(FEStateManager* _this);
        undefined(__fastcall* HandlePadButton4)(FEStateManager* _this);
        void (__fastcall *HandlePadButton5)(FEStateManager* _this);
        void (__fastcall *HandlePadButton6)(FEStateManager* _this);
        void (__fastcall *HandlePadButton7)(FEStateManager* _this);
        void (__fastcall *HandlePadButton8)(FEStateManager* _this);
        void (__fastcall *HandlePadButton9)(FEStateManager* _this);
        void (__fastcall *HandlePadButton0Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton1Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton2Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton3Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton4Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton5Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton6Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton7Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton8Release)(FEStateManager* _this);
        void (__fastcall *HandlePadButton9Release)(FEStateManager* _this);
        void (__fastcall *HandleLTrigger)(FEStateManager* _this);
        void (__fastcall *HandleLTriggerRelease)(FEStateManager* _this);
        undefined(__fastcall* HandlePadLeft)(FEStateManager* _this);
        void (__fastcall *HandlePadUp)(FEStateManager* _this);
        void(__fastcall* HandlePadRight)(FEStateManager* _this, DWORD edx, char*);
        undefined(__fastcall* HandleTreePadRight)(FEStateManager* _this, DWORD edx, undefined4, undefined4);
        undefined(__fastcall* HandlePadStart)(FEStateManager* _this, DWORD edx);
        void (__fastcall *HandlePadQuit)(FEStateManager* _this);
        void (__fastcall *HandleRefresh)(FEStateManager* _this);
        void (__fastcall *HandleRTrigger)(FEStateManager* _this);
        void (__fastcall *HandleRTriggerRelease)(FEStateManager* _this);
        void (__fastcall *HandleScreenConstructed)(FEStateManager* _this);
        void (__fastcall *HandleScreenDestructed)(FEStateManager* _this);
        void (__fastcall *HandleScreenDestructing)(FEStateManager* _this);
        void (__fastcall *HandleScreenTick)(FEStateManager* _this);
        void (__fastcall *HandleGameTick)(FEStateManager* _this);
        void (__fastcall *HandleMouseMoveLeft)(FEStateManager* _this);
        void (__fastcall *HandleMouseMoveRight)(FEStateManager* _this);
        void (__fastcall *HandleScreenTimeout)(FEStateManager* _this);
        undefined(__fastcall* HandleShowDialog)(FEStateManager* _this, DWORD edx, void*);
        void (__fastcall *HandleSignInComplete)(FEStateManager* _this);
        undefined(__fastcall* HandleStateChange)(FEStateManager* _this);
        void (__fastcall *HandleWidgetValueChanged)(FEStateManager* _this, DWORD edx, undefined, undefined);
    };

    vftable* __vt;
    int mCurState; /* //T_INT4 */
    int mEntryPoint; /* //T_INT4 */
    int mExitPoint; /* //T_INT4 */
    int mNextState; /* //T_INT4 */
    int mPrevState; /* //T_INT4 */
    int mSubState; /* //T_INT4 */
    int mLastScreenPushed; /* //T_INT4 */
    int mNextScreen; /* //T_INT4 */
    byte mNextStateValid; /* //T_BOOL08 */
    byte mExitingAllStateManagers; /* //T_BOOL08 */
    int mNumScreensPushed; /* //T_INT4 */
    int mNumScreensToPop; /* //T_INT4 */
    struct FEStateManager* mParentManager; /* //FEStateManager* */
    struct FEStateManager* mChildManager; /* //FEStateManager* */
    struct FEStateManager* mNextManager; /* //FEStateManager* */
    struct bTList<FEStateOption> mStateOptions; /* //bTList<FEStateOption> */
    byte mCanSkipMovie; /* //T_BOOL08 */
};


struct FEVinylStateManager : FEStateManager {
    static enum eState {
        STATE_MAIN = 0,
        STATE_VINYL_ADD = 1,
        STATE_VINYL_LIST = 2,
        STATE_VINYL_TRANSFORM_LIST = 3,
        STATE_VINYL_TRANSFORM = 4,
        STATE_VINYL_COLOR = 5,
        STATE_VINYL_FINISH = 6,
        STATE_DIALOG_CONFIRM_DELETE = 7,
        STATE_DIALOG_TOO_MANY_LAYERS = 8,
        STATE_DIALOG_UNEDITABLE_VINYL = 9
    };

    //struct FEStateManager __inherit_0; /* //FEStateManager */
    //struct UTL::Singleton<FEVinylStateManager> __inherit_72; /* //UTL::Singleton<FEVinylStateManager> */
    int mTelemetryTime; /* //T_UINT4 */
    int mCurrentLayer; /* //T_INT4 */
    struct VinylSelectablePart* mPart; /* //VinylSelectablePart* */
    struct eastl::vector<UINT, bstl::allocator> mThumbnailHashes; /* //eastl::vector<unsigned int,bstl::allocator> */
    int mSelectedGroup; /* //T_INT4 */
    int mSelectedVinyl; /* //T_INT4 */
};

struct FEVinylSelectStateManager : FEStateManager {
    //struct FEStateManager __inherit_0; /* //FEStateManager */
    float mMouseInputDeltaTime;
    bool mPlayPurchaseSound;
};

struct FEPerformanceStateManager : FEStateManager {
    static enum ePackageUpgrade {
        UPGRADE_NONE = 0,
        UPGRADE_ENGINE = 1,
        UPGRADE_NOS = 2,
        UPGRADE_INDUCTION = 3,
        UPGRADE_SUSPENSION = 4,
        UPGRADE_DRIVETRAIN = 5,
        UPGRADE_BRAKES = 6,
        UPGRADE_TIRES = 7,
        UPGRADE_POWER = 8,
        UPGRADE_HANDLING = 9
    };
    static enum eUpgradeLevel {
        LEVEL_STOCK = 0,
        LEVEL_STREET = 1,
        LEVEL_PRO = 2,
        LEVEL_RACE = 3,
        LEVEL_1 = 4,
        LEVEL_2 = 5,
        LEVEL_3 = 6,
        LEVEL_4 = 7,
        LEVEL_NONE = 255
    };

    bool mScreenConstructed;
    enum ePackageUpgrade mCurrentUpgrade;
    enum eUpgradeLevel mCurrentLevel;
    struct FECarRecord* mCurrentCar;
    int mCurrentPartCost;
    float mCurrentDeltas[3];
    int mCarTier;
    DWORD mPULCError;
    char mCurrentRideInfo[0x1ca0];
    bTList<struct ISelectablePart> mPartList;
    DWORD mInput[11];
    bool mShouldExitOnBack;
    bool mCurrentPartIsLocked;
    int mTelemetryTime;
    bool mShouldRestoreRideInfo;
    bool mShouldAutoSave;
    bool mShouldPlayPurchaseSound;
};


namespace FeCust{
    struct ColourPartData {
        byte s; /* //T_UCHAR */
        byte b; /* //T_UCHAR */
        byte hue; /* //T_UCHAR */
    };
}



struct ISelectablePart{
    static struct vftable {
        void* (__fastcall *vector_deleting_destructor)(ISelectablePart* _this, DWORD edx, DWORD);
        DWORD(__fastcall *CartComparisonValue)(ISelectablePart* _this);
        ISelectablePart* (__fastcall *CopyPart)(ISelectablePart* _this);
        bool (__fastcall*Install)(ISelectablePart* _this, DWORD edx, struct VinylSelectablePart*, struct FECustomizationRecord*);
        bool (__fastcall*Install_1)(ISelectablePart* _this, DWORD edx, struct VinylSelectablePart*, struct FECustomizationRecord*);
        void (__fastcall*Preview)(ISelectablePart* _this, DWORD edx, bool);
        undefined(__fastcall*GetSculptZoneAmoun)(ISelectablePart* _this); //t@@UBAMW4AutoSculptZoneID@@W4SELECTABLE_REGION@FeCust@@@Z
        DWORD(__fastcall*GetHALIDHash)(ISelectablePart* _this);
        undefined(__fastcall*GetActionLanguageHash)(ISelectablePart* _this);
        DWORD(__fastcall*GetCategoryHash)(ISelectablePart* _this);
        undefined(__fastcall*GetPrice)(ISelectablePart* _this);
        int (__fastcall*GetPrice_1)(ISelectablePart* _this);
        undefined(__fastcall*GetTier)(ISelectablePart* _this);
        undefined(__fastcall*GetBrandNameHash)(ISelectablePart* _this);
        undefined(__fastcall*GetLogoHash)(ISelectablePart* _this);
        bool (__fastcall *GetIsUnlocked)(ISelectablePart* _this);
        undefined(__fastcall*GetIsTierUnlocked)(ISelectablePart* _this);
        undefined(__fastcall*IsValidForMode)(ISelectablePart* _this, DWORD edx, enum DALVehicleCommands::RaceMode);
        bool (__fastcall*GetIsStock)(ISelectablePart* _this);
        bool (__fastcall*GetIsInInventory)(ISelectablePart* _this, DWORD edx, struct FECustomizationRecord*);
        undefined(__fastcall*GetIsInstalled)(ISelectablePart* _this, DWORD edx, struct RideInfo*);
        bool (__fastcall*GetIsInstalled_1)(ISelectablePart* _this, DWORD edx, struct FECustomizationRecord*);
        struct DBCarPart*(__fastcall*GetPart)(ISelectablePart* _this);
        enum CAR_SLOT_ID(__fastcall*GetSlotId)(ISelectablePart* _this);
        pointer Unknown_24;
        DWORD(__fastcall*GetUniquePartID)(ISelectablePart* _this);
        undefined(__fastcall*IsCarbon)(ISelectablePart* _this);
        struct FeCust::ColourPartData*(__fastcall*GetColourPartData)(ISelectablePart* _this);
        undefined(__fastcall*ResetAutosculptValues)(ISelectablePart* _this);
        undefined(__fastcall*GetDetailHash)(ISelectablePart* _this);
        struct FEPartDetail*(__fastcall*GetPartDetails)(ISelectablePart* _this);
        undefined(__fastcall*IsReallyAutosculptable)(ISelectablePart* _this);
        undefined(__fastcall*IsPartInPackage)(ISelectablePart* _this, DWORD edx, struct DBCarPart*);
        undefined(__fastcall*IsSlotInPackage)(ISelectablePart* _this, DWORD edx, enum CAR_SLOT_ID);
        undefined(__fastcall*GetOfferId)(ISelectablePart* _this);
        bool (__fastcall*IsUnlockedByPULC)(ISelectablePart* _this);
        DWORD(__fastcall*GetLabelNameHash)(ISelectablePart* _this);
    };

    struct vftable * __vt;
    struct bTNode<ISelectablePart> __inherit_4; /* //bTNode<ISelectablePart> */
    int mOverrideNameHash; /* //T_UINT4 */
};



struct VinylSelectablePart : ISelectablePart {
    int mCurrentEditingLayer; /* //T_INT4 */
    struct DBCarPart mCurrentPart; /* //DBCarPart */
    bool mFastVinyl; /* //T_BOOL08 */
    struct VinylSystem::VinylTransformPacked mTransform; /* //VinylSystem::VinylTransformPacked */
    struct FeCust::ColourPartData mColorData[4]; /* // */
    bool mIsMirrored; /* //T_BOOL08 */
    bool mIsEditingLayer; /* //T_BOOL08 */
    bool mIsEmptyFromDeletion; /* //T_BOOL08 */
    bool mIsDecal; /* //T_BOOL08 */
    bool mNewPart; /* //T_BOOL08 */
};

namespace CarSkinManager {
    enum eCarSkinTask {
        eCST_Finished = 0,
        eCST_Started = 1,
        eCST_CanvasBasePaintFill = 2,
        eCST_LowerLayersBasePaintFill = 3,
        eCST_LowerLayersInitialize = 4,
        eCST_CanvasInitialize = 5,
        eCST_CanvasInitializeCompressed = 6,
        eCST_CompressLowerLayers = 7,
        eCST_StepRasterizer = 8,
        eCST_Finalize = 9,
        eCST_CopyLowerLayersToCanvas = 10,
        eCST_CopyLowerLayersToCanvasCompressed = 11,
        eCST_CopyToDestination = 12,
        eCST_CopyToDestinationNoCompression = 13
    };
}




namespace nfs_structs {
    enum GAME_VERSION {
        UC_1_0 = 0x008AEC55,
        UC_1_21 = 0x0087ba75
    };

    void Register(int GAME_VERSION);
}