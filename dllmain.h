#pragma once
#include <cstdint>
#include <sys/types.h>
#include "nfs_structs.h"
//#include <sys/types.h>
//#include <string>
//#include <random>

//C++ FUCKING SUCKS I HATE IT

//using hasher_t = uint32_t(__cdecl*)(char* toHash);
//using MemoryAlloc_t = DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress);
//using getTexture_t = char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3);
//
//extern hasher_t getVltHash;
//extern hasher_t getBinHash;
//extern MemoryAlloc_t MemoryAlloc;
//extern getTexture_t getTexture;
//
//extern DWORD* MemBlock_Global;
//extern DWORD* MemBlock_FE;
//extern DWORD* MemBlock_Cars;
//
//extern int* GameFlow;
//extern int* Renderer;
//extern DWORD* FEScreenManager;
//extern DWORD* UserProfile_Player1;
//extern DWORD* FECarHeaven;
////extern DWORD CurrentFECarPTR_012ac35c = 0x012ac35c;
////extern DWORD* FEAutosculptScreen = (DWORD*) 0x012ab1b4;
//extern DWORD* DebugVehicleSelection;
//
//extern int* timer_world;
//
//extern DWORD theSunAndSky;
//
//extern std::random_device rd;
//extern std::mt19937 gen;
//
//extern struct bVector3;
//
//extern struct bVector4;
//
//extern struct LinkedNode;
//
//extern struct FETreeDataNode;
//
//extern DWORD FETreeDataNode_vftable;
//
//using FEDataTreeNodeConstructor_t = FETreeDataNode * (__fastcall*)(FETreeDataNode* this_, DWORD unaff, DWORD, char*, DWORD, DWORD);
//using FE_getFeElement_t = DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD);
//using AddToTree_t = void*(__fastcall*)(DWORD, DWORD, LinkedNode*, DWORD);
//using FUN0553aa0_t = void(__fastcall*)(LinkedNode* this_);
//using getLinkedListLength_t = DWORD(__fastcall*)(LinkedNode* this_, DWORD unaff, DWORD);
//
//
//extern FEDataTreeNodeConstructor_t FETreeDataNode_constructor;
//extern FE_getFeElement_t FE_getFeElement;
//extern AddToTree_t AddToTree;
//extern FUN0553aa0_t FUN_00553aa0;
//extern getLinkedListLength_t FUN_getLinkedListLength_004a76f0;
//
//extern enum CarPartID;
//
//extern enum AutoSculptRegionID;
//
//extern enum ePlayerSettingsCameras;
//
//extern struct FECarRecord;
//
//extern struct DALInterface; //i hate c++ so much
//extern struct DALInterface_vt;
//extern struct DALInterface {
//    DALInterface_vt* vftable;
//    int mInterfaceIndex;
//};
////VALUES FOR 1.18, MAY BE DIFFERENT IN 1.0
//extern enum DAL_OPTIONS;
//
//extern DALInterface** DALOptions;


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
/*
EA::Reflection::Int64
EA::Reflection::Int32
EA::Reflection::Int16
EA::Reflection::Int8
EA::Reflection::UInt64
EA::Reflection::UInt32
EA::Reflection::UInt16
EA::Reflection::UInt8
EA::Reflection::Char
EA::Reflection::Bool
EA::Reflection::Float
EA::Reflection::Double
EA::Reflection::Text
EA::Reflection::Reference
EA::Reflection::Type
EA::Reflection::KeyValueAttrib
Attrib::Blob
Attrib::RefSpec
Attrib::Definition
Attrib::Types::Symbol
Attrib::Types::Colour
Attrib::Types::FloatColour
Attrib::Types::ShaderColour
Attrib::Types::Matrix
Attrib::Types::Quaternion
Attrib::Types::Vector4
Attrib::Types::Vector3
Attrib::Types::Vector2
*/

namespace Inputs {
    float AutosculptSliderSpeed = 0.5f; //vanilla 0.2f
    float AutosculptSliderSpeedBoost;
    float AutosculptSliderSpeedSlow;

    float OptionsSliderSpeed = 80.0f; //vanilla 80.0f
    float OptionsSliderSpeedBoost;
    float OptionsSliderSpeedSlow;

    float PerfTuningSliderSpeed = 80.0f; //vanilla 100.0f
    float PerfTuningSliderSpeedBoost;
    float PerfTuningSliderSpeedSlow;

    float FEVisualPaint1SliderSpeed = 150.0f; //vanilla 150.0f
    float FEVisualPaint1SliderSpeedBoost;
    float FEVisualPaint1SliderSpeedSlow;

    float FEVisualPaint2SliderSpeed = 100.0f; //vanilla 100.0f
    float FEVisualPaint2SliderSpeedBoost;
    float FEVisualPaint2SliderSpeedSlow;
}

namespace Overlay {
    static bool doOverlay = true;
}

namespace CarRenderConn {
    DWORD MissShiftEffect, NOSEffect, ExhaustSmokeEffect, NOSTrailEffect, BackfireEffect;
}

namespace ucetesting {

	namespace imGuiStuff {
		void ImGuiFrame();
        void EditCollection(Attrib::Collection*& col);
    }
}








