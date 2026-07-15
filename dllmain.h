#pragma once
//#include <cstdint>
#include <Windows.h>
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
	}
}