// dllmain.cpp : Définit le point d'entrée de l'application DLL.
/*
build paths
desktop
D:\Jeux\UCE 1.0.1.18\scripts\UndercoverExposed.asi

laptop

C:\Program Files (x86)\EA Games\Need for Speed Undercover\scripts\UndercoverExposed.asi

*/
#pragma once
#include "pch.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "Injector.h"
//#include "tests.h"
//#include "nfs_structs.h"
#include <chrono>
#include <map>
#include "AttribEditor.h"
#include "MiscGameFuncs.h"


#include "Settings.h"
#include "CrashFixes.h"
#include "Inputs.h"
#include "Overlay.h"
#include "ucetesting.h"
#include "DayNight.h"
#include "KitRideHeight.h"
#include "Headlights.h"
#include "FEAftermarketMenu.h"
#include "Effects.h"
#include "FECarList.h"
#include "FreeroamMusic.h"
#include "VinylsSelectMenu.h"

using namespace std;

static uint32_t readInt(std::ifstream& input) {
    uint32_t a;
    input.read(reinterpret_cast<char*>(&a), sizeof(a));
    return a;
}

constexpr auto DEFAULT = 0xeec2271a;





void* Attrib::Instance::GetAttributePointerFromString(InstanceBase* this_, const char* key, DWORD stuff) {
    return Attrib::Instance::GetAttributePointer(this_, 0, getVltHash( key), stuff);
}

int __cdecl return_zero() {
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        //game version detection, from SpeedReflect ASI
        auto base = (uintptr_t)GetModuleHandleA(NULL);
        auto dos = (IMAGE_DOS_HEADER*)(base);
        auto nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
        uint32_t entry = base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base);

        CURRENT_GAME_VERSION = entry;

        bool carsModuleInstalled = true; //really keep this?
        {
            struct stat carEx;
            if (stat("../CARS/LAM_DIA_SV_95/GEOMETRY.BIN", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/DAMAGE/LAM_DIA_SV_95.damage", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/HEN_VEN_GT/GEOMETRY.BIN", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/DAMAGE/HEN_VEN_GT.damage", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/LAN_DEL_INT/GEOMETRY.BIN", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/DAMAGE/LAN_DEL_INT.damage", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/MIT_ECL_GT_06/GEOMETRY.BIN", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/DAMAGE/MIT_ECL_GT_06.damage", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/SUB_BRZ/GEOMETRY.BIN", &carEx) == -1) carsModuleInstalled = false;
            if (stat("../CARS/DAMAGE/SUB_BRZ.damage", &carEx) == -1) carsModuleInstalled = false;
        }

        // user information about the vinyls mess
        struct stat vinFileMeta;
        stat("../CARS/VINYLS/VINYLS.BIN", &vinFileMeta);

        //SpeedReflect warning
        if (vinFileMeta.st_mtime > 1240521921) {
            //            std::cout << "UCE: Modded VINYLS.BIN file.\n"; // 1240421921 is 1.0.1.18
            struct stat SRMeta;
            if (stat("SpeedReflect.asi", &SRMeta) == -1) {
                MessageBoxA(NULL, "Critical: SpeedReflect ASI not present!\nYour game will crash due to being unable to load any vinyls. Please add SpeedReflect in your scripts folder or revert the CARS\\VINYLS\\VINYLS.BIN file to vanilla!", "Undercover Exposed", MB_ICONERROR);
            }
            else { //SpeedReflect present
                switch (entry) {
                case UC_1_0: //UC 1.0
                    break;
                case UC_1_21: //UC 1.0.1.18
                    if (SRMeta.st_mtime < 1741945898) MessageBoxA(NULL, "Critical: This version of SpeedReflect ASI doesn't support this game version!\nYour game will crash due to being unable to load any vinyls, please use the modified SpeedReflect provided with UCE.", "Undercover Exposed", MB_ICONERROR);
                    break;
                default:
                    MessageBoxA(NULL, "Critical: Game version not supported by the SpeedReflect ASI!\nYour game will crash due to being unable to load any vinyls.\nPlease use either the 10.0MB 1.0.0.1 EXE or the 21.6MB 1.1.2.1/1.0.1.18 EXE or revert the CARS\\VINYLS\\VINYLS.BIN file to vanilla.", "Undercover Exposed", MB_ICONERROR);
                }
            }
        }

        //sets most game function addresses used in the asi
        MiscGameFuncs::Register();
        nfs_structs::Register(CURRENT_GAME_VERSION);
        //functions from Headlights
        {
            using namespace Headlights;
            switch (entry) {
            case UC_1_0:
                //makeHeadlights
                tMatrix__TransformPosition = reinterpret_cast<decltype(tMatrix__TransformPosition)>(0x007436f0); //__cdecl indeed
                SDR__StartBatch = reinterpret_cast<decltype(SDR__StartBatch)>(0x0076d8f0);
                SDR__SetTexture = reinterpret_cast<decltype(SDR__SetTexture)>(0x0076d870);
                SDR__AddPoly = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x007816d0;
                SDR__EndBatch = (void* (__fastcall*)(DWORD this_))0x007a4970;
                WWorldPos__FindClosestFace = reinterpret_cast<decltype(WWorldPos__FindClosestFace)>(0x008963c0);
                normalizeVector_005d9950 = reinterpret_cast<decltype(normalizeVector_005d9950)>(0x005f3560);
                WWorldPos__HeightAtPoint = reinterpret_cast<decltype(WWorldPos__HeightAtPoint)>(0x0088eb40);

                //lightsUpdateReplacement
                eReplacementTextures__constructor = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00a1f0d0;
                eReplacementTextures__EntryHandle__SetNewNameHash = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00539170;
                GetCarTypeInfoFromHash = (char* (__cdecl*)(DWORD param))0x00843630;

                //asm injector
                //DAT_00b647ec = (float*)0x;
                DrawAmbientShadow = (void*)0x00851d50;
                FUN_00520d70 = (void*)0x00538e10;
                FUN_00538df0 = (void*)0x00538df0;
                break;
            case UC_1_21:
                //makeHeadlights
                tMatrix__TransformPosition = reinterpret_cast<decltype(tMatrix__TransformPosition)>(0x0071b820); //__cdecl indeed
                SDR__StartBatch = reinterpret_cast<decltype(SDR__StartBatch)>(0x00744290);
                SDR__SetTexture = reinterpret_cast<decltype(SDR__SetTexture)>(0x00744220);
                SDR__AddPoly = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x00755d80;
                SDR__EndBatch = (void* (__fastcall*)(DWORD this_))0x00777f80;
                WWorldPos__FindClosestFace = reinterpret_cast<decltype(WWorldPos__FindClosestFace)>(0x00864ef0);
                normalizeVector_005d9950 = reinterpret_cast<decltype(normalizeVector_005d9950)>(0x005d9950);
                WWorldPos__HeightAtPoint = reinterpret_cast<decltype(WWorldPos__HeightAtPoint)>(0x0085e900);

                //lightsUpdateReplacement
                eReplacementTextures__constructor = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00521090;
                eReplacementTextures__EntryHandle__SetNewNameHash = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00521060;
                GetCarTypeInfoFromHash = (char* (__cdecl*)(DWORD param))0x00817940;

                //asm injector
                //DAT_00b647ec = (float*)0x00b647ec;
                DrawAmbientShadow = (void*)0x00825430;
                FUN_00520d70 = (void*)0x00520d70;
                break;
            }
        }
        //functions from FEAftermarketMenu
        {
            using namespace FEAftermarketMenu;
            switch (entry) {
            case UC_1_0: //UC 1.0
                StandardSelectablePart__GetPartsList_005d1b00 = reinterpret_cast<decltype(StandardSelectablePart__GetPartsList_005d1b00)>(0x005eb120);
                AutosculptSelectablePart__GetPartsList_005d40e0 = reinterpret_cast<decltype(AutosculptSelectablePart__GetPartsList_005d40e0)>(0x005ed530);
                break;
            case UC_1_21: //UC 1.0.1.18
                StandardSelectablePart__GetPartsList_005d1b00 = reinterpret_cast<decltype(StandardSelectablePart__GetPartsList_005d1b00)>(0x005d1b00);
                AutosculptSelectablePart__GetPartsList_005d40e0 = reinterpret_cast<decltype(AutosculptSelectablePart__GetPartsList_005d40e0)>(0x005d40e0);
                break;
            }
        }
        //functions from FECarList
        {
            switch (entry) {
                using namespace FECarList;
            case UC_1_21:
                //FEPlayerCarDB
                CreateNewCarRecord = reinterpret_cast<decltype(CreateNewCarRecord)>(0x0050f220);
                CreateNewCustomizationRecord = reinterpret_cast<decltype(CreateNewCustomizationRecord)>(0x00513880);

                //FECarRecord
                GetType = reinterpret_cast<decltype(GetType)>(0x00507710);
                FECarRecord__GetNameHash_005079e0 = reinterpret_cast<decltype(FECarRecord__GetNameHash_005079e0)>(0x005079e0);
                FECarRecord__GetManufacturerName_00507d10 = reinterpret_cast<decltype(FECarRecord__GetManufacturerName_00507d10)>(0x00507d10);
                FECarRecord__GetManuLogoHash_00502230 = reinterpret_cast<decltype(FECarRecord__GetManuLogoHash_00502230)>(0x00502230);
                eastl__vector__InsertValue = reinterpret_cast<decltype(eastl__vector__InsertValue)>(0x005199f0);
                break;

            case UC_1_0:

                //FEPlayerCarDB
                CreateNewCarRecord = reinterpret_cast<decltype(CreateNewCarRecord)>(0x00524d20); //ok
                CreateNewCustomizationRecord = reinterpret_cast<decltype(CreateNewCustomizationRecord)>(0x00528ca0);//ok

                //FECarRecord
                GetType = reinterpret_cast<decltype(GetType)>(0x0051d8c0);//ok
                FECarRecord__GetNameHash_005079e0 = reinterpret_cast<decltype(FECarRecord__GetNameHash_005079e0)>(0x00525070);//ok
                FECarRecord__GetManufacturerName_00507d10 = reinterpret_cast<decltype(FECarRecord__GetManufacturerName_00507d10)>(0x0051dca0); //ok
                FECarRecord__GetManuLogoHash_00502230 = reinterpret_cast<decltype(FECarRecord__GetManuLogoHash_00502230)>(0x00518550); //ok
                eastl__vector__InsertValue = reinterpret_cast<decltype(eastl__vector__InsertValue)>(0x0052f570);//ok
                break;
            }
        }
        //functions from Effects
        {
            switch (entry) {
                using namespace CarRenderConn;
            case UC_1_21:
                TestVisibility = reinterpret_cast<decltype(TestVisibility)>(0x00826bc0);
                AcidEffect__Disable = reinterpret_cast<decltype(AcidEffect__Disable)>(0x00756710);

                Effect__Fire = reinterpret_cast<decltype(Effect__Fire)>(0x00839270); //draws MissShiftEffect
                Effect__Update = reinterpret_cast<decltype(Effect__Update)>(0x008269d0);

                UpdateEngineEffects = reinterpret_cast<decltype(UpdateEngineEffects)>(0x0082a120);
                UpdateWindowGlassShatterEffects_HD = reinterpret_cast<decltype(UpdateWindowGlassShatterEffects_HD)>(0x0082a050);
                FireParticles = reinterpret_cast<decltype(FireParticles)>(0x00829ec0);

                break;
            case UC_1_0:
                TestVisibility = reinterpret_cast<decltype(TestVisibility)>(0x008538a0);
                AcidEffect__Disable = reinterpret_cast<decltype(AcidEffect__Disable)>(0x007820f0);

                Effect__Fire = reinterpret_cast<decltype(Effect__Fire)>(0x00868040); //draws MissShiftEffect
                Effect__Update = reinterpret_cast<decltype(Effect__Update)>(0x008536b0);

                UpdateEngineEffects = reinterpret_cast<decltype(UpdateEngineEffects)>(0x008576f0);
                UpdateWindowGlassShatterEffects_HD = reinterpret_cast<decltype(UpdateWindowGlassShatterEffects_HD)>(0x00857620);
                FireParticles = reinterpret_cast<decltype(FireParticles)>(0x00857490);
                break;
            }
        }
        //functions from Music
        {
            switch (entry) {
                using namespace FreeroamMusic;
            case UC_1_21:
                //SFXCTL_MusicPlayback__QueueProject_00494650 = reinterpret_cast<decltype(//SFXCTL_MusicPlayback__QueueProject_00494650)>(0x00494650);
                //SFXCTL_MusicPlayback__QueueEvent_00494610 = reinterpret_cast<decltype(//SFXCTL_MusicPlayback__QueueEvent_00494610)>(0x00494610);
                SFXCTL_MusicPlayback__QueueValue = reinterpret_cast<decltype(SFXCTL_MusicPlayback__QueueValue)>(0x004a31f0);
                SFXCTL_MusicPlayback__GenRandomIndexFromBitfield = reinterpret_cast<decltype(SFXCTL_MusicPlayback__GenRandomIndexFromBitfield)>(0x00494740);
                //Sound__MusicLogic__CheckInteractiveToFEStateTrans_004a3750 = reinterpret_cast<decltype(//Sound__MusicLogic__CheckInteractiveToFEStateTrans_004a3750)>(0x004a3750);
                break;

            case UC_1_0:
                //SFXCTL_MusicPlayback__QueueProject_00494650 = reinterpret_cast<decltype(//SFXCTL_MusicPlayback__QueueProject_00494650)>(0x0049d030);
                //SFXCTL_MusicPlayback__QueueEvent_00494610 = reinterpret_cast<decltype(//SFXCTL_MusicPlayback__QueueEvent_00494610)>(0x0049cff0);
                SFXCTL_MusicPlayback__QueueValue = reinterpret_cast<decltype(SFXCTL_MusicPlayback__QueueValue)>(0x004ac1c0);
                SFXCTL_MusicPlayback__GenRandomIndexFromBitfield = reinterpret_cast<decltype(SFXCTL_MusicPlayback__GenRandomIndexFromBitfield)>(0x0049d120);
                //Sound__MusicLogic__CheckInteractiveToFEStateTrans_004a3750 = reinterpret_cast<decltype(//Sound__MusicLogic__CheckInteractiveToFEStateTrans_004a3750)>(0x004ac6e0);
                break;
            }
        }



        Settings::readConfig();

        //Inputs
        {
            using namespace Inputs;
            switch (entry) {
            case UC_1_0:
                AutosculptSliderSpeedFactor = (float*)0x00d2dce8; //0.2
                OptionsSliderSpeedFactor = (float*)0x00d2d9d8; //80
                PerfTuningSliderSpeedFactor = (float*)0x00d2dba4; //100
                FEVisualPaintSpeedFactor1 = (float*)0x00d2dbac; //150
                FEVisualPaintSpeedFactor2 = (float*)0x00d2dba8; //100
                EnableInterrupts = utils::call(0x006ac29b, frameHook); //in EMainRender event manager
                if (Settings::SliderEnhancements) {
                    utils::set(0x005685bb, &fakeFrameTime);
                    Inputs::VinylSelectablePart__SetTransformData = utils::call(0x005833ea, Inputs::VinylSelectablePart__SetTransformData_wrapper);
                }
                break;
            case UC_1_21:
                AutosculptSliderSpeedFactor = (float*)0x00cbad0c; //0.2
                OptionsSliderSpeedFactor = (float*)0x00cba9fc; //80
                PerfTuningSliderSpeedFactor = (float*)0x00cbabc8; //100
                FEVisualPaintSpeedFactor1 = (float*)0x00cbabd0; //150
                FEVisualPaintSpeedFactor2 = (float*)0x00cbabcc; //100
                EnableInterrupts = utils::call(0x0069060d, frameHook); //in EMainRender event manager
                if (Settings::SliderEnhancements) {
                    utils::set(0x0054f485, &fakeFrameTime);
                    // inputs speed with shift/ctrl
                    Inputs::VinylSelectablePart__SetTransformData = utils::call(0x0056998a, Inputs::VinylSelectablePart__SetTransformData_wrapper);
                }

                break;
            }
        }

        //GUI init
        if (Settings::EnableOverlay) {
            using namespace Overlay;
            switch (entry) {
            case UC_1_0:
                utils::call(0x007564c5, imgui_init);
                //beginSceneHookDest = utils::set(0x007b2fe8, beginSceneHook);
                endSceneHookDest = utils::set(0x007b3261, endSceneHook);
                WndProcHookDest = utils::set(0x0075630b, WndProcHook);
                D3DResetHookDest = utils::call(0x00755ff0, D3DResetHook);

                DirectInput8CreateDest = utils::set(0x00bd605c, DirectInput8CreateHook); //ImGUI input

                //hwnd = *(void**)0x00df1df8;
                //g_pd3dDevice = *(IDirect3DDevice9**)0x00ea0110;
                break;
            case UC_1_21:
                utils::call(0x0072e825, imgui_init);
                //beginSceneHookDest = utils::set(0x007863bd, beginSceneHook);
                endSceneHookDest = utils::set(0x00786624, endSceneHook);
                WndProcHookDest = utils::set(0x0072e66b, WndProcHook);
                D3DResetHookDest = utils::call(0x0072e360, D3DResetHook);

                DirectInput8CreateDest = utils::set(0x00b6105c, DirectInput8CreateHook); //ImGUI input

                //hwnd = *(void**)0x01304e98;
                //g_pd3dDevice = *(IDirect3DDevice9**)0x013b3190;
                break;
            }
        }

        //misc crash fixes
        if (Settings::CrashFixes) switch (entry) {
            using namespace CrashFixes;
        case UC_1_0: //UC 1.0
        {
            //remove kamikaze code, should be a duplicate of multifix
            {
                utils::nop(0x00401927, 10);
                utils::nop(0x004b3ced, 10);
                utils::nop(0x004b4c40, 10);
                utils::nop(0x004b79a6, 6);
                utils::nop(0x004b7b36, 10);
                utils::nop(0x004b7f81, 10);
                utils::nop(0x004b7fb8, 10);
                utils::nop(0x004b812a, 10);
                utils::nop(0x004b819b, 10);
                utils::nop(0x004b81e5, 10);
                utils::nop(0x0053cfca, 5);
                utils::nop(0x0053f18c, 6);
                utils::nop(0x005401fd, 10);
                utils::nop(0x005f6387, 10);
                utils::nop(0x005f6e35, 5);
                utils::nop(0x006391c5, 10);
                utils::nop(0x0066bafb, 10);
                utils::nop(0x00683161, 10);
                utils::nop(0x0069dea0, 10);
                utils::nop(0x006a1e12, 10);
                utils::nop(0x006a5204, 10);
                utils::nop(0x006a5354, 10);
                utils::nop(0x006a74aa, 10);
                utils::nop(0x006a74f2, 5);
                utils::nop(0x006accf1, 10);
                utils::nop(0x006ad382, 10);
                utils::nop(0x006ad574, 6);
                utils::nop(0x006ae40f, 6);
                utils::nop(0x006b13cb, 10);
                utils::nop(0x006b42fd, 6);
                utils::nop(0x006b459d, 6);
                utils::nop(0x007629d6, 10);
                utils::nop(0x00762ee8, 10);
                utils::nop(0x007716f3, 10);
                utils::nop(0x007893d0, 10);
                utils::nop(0x00789406, 10);
                utils::nop(0x0078949b, 10);
                utils::nop(0x007896ca, 10);
                utils::nop(0x0078c8c7, 6);
                utils::nop(0x0078c971, 6);
                utils::nop(0x0078cac5, 10);
                utils::nop(0x0078ccea, 10);
                utils::nop(0x0078d0ed, 10);
                utils::nop(0x0078de0e, 10);
                utils::nop(0x0078e2d1, 10);
                utils::nop(0x0078f396, 6);
                utils::nop(0x0078f44a, 6);
                utils::nop(0x0078f6f2, 10);
                utils::nop(0x0078fad5, 10);
                utils::nop(0x007965c9, 6);
                utils::nop(0x007965fb, 6);
                utils::nop(0x007969dd, 6);
                utils::nop(0x007d24eb, 10);
                utils::nop(0x008405f7, 10);
                utils::nop(0x0084069b, 10);
                utils::nop(0x0084fe78, 10);
                utils::nop(0x008506f1, 10);
                utils::nop(0x008509d6, 10);
                utils::nop(0x008510a7, 6);
                utils::nop(0x00851666, 10);
                utils::nop(0x008516ae, 10);
                utils::nop(0x00865dfc, 10);
                utils::nop(0x00867722, 10);
                utils::nop(0x0087bbcb, 10);
                utils::nop(0x0087bc03, 10);
                utils::nop(0x0087bc3b, 10);
                utils::nop(0x0087c01b, 6);
                utils::nop(0x00881d0c, 10);
                utils::nop(0x00883a46, 10);
                utils::nop(0x00887597, 5);
                utils::nop(0x00a0f398, 7);
            }

            //try to ignore int3 breakpoints (all of these are accompanied by a message box anyways)
            {
                utils::nop(0x006a4800, 1);
                utils::nop(0x006a4b40, 1);
                utils::nop(0x00a37200, 1);
                utils::nop(0x00a37a08, 1);
                utils::nop(0x00a37c40, 1);
                utils::nop(0x00a37cc6, 1);
                utils::nop(0x00a37fb1, 1);
                utils::nop(0x00a38068, 1);
                utils::nop(0x00a380f5, 1);
                utils::nop(0x00a38f1a, 1);
                utils::nop(0x00a38f44, 1);
                utils::nop(0x00a38f8a, 1);
                utils::nop(0x00a3903e, 1);
                utils::nop(0x00a39362, 1);
                utils::nop(0x00a393ec, 1);
                utils::nop(0x00a39432, 1);
                utils::nop(0x00a394c8, 1);
                utils::nop(0x00a3970b, 1);
                utils::nop(0x00a39798, 1);
                utils::nop(0x00a39a5f, 1);
                utils::nop(0x00a39af6, 1);
                utils::nop(0x00a39afd, 1);
                utils::nop(0x00a39ca8, 1);
                utils::nop(0x00a39d7e, 1);
                utils::nop(0x00a39e3c, 1);
                utils::nop(0x00a39f43, 1);
                utils::nop(0x00a3a508, 1);
            }


            //remove more kamikaze code
//            utils::set<char>(0x00883a25, 0xeb); //JMP instead of JNZ

            //remove function calls to seemingly broken CarLoader::PrintMemoryUsage
            {
                utils::nop(0x00881d00, 9);

                utils::set<char>(0x00883a27, 0x5f); //POP EDI
                utils::set<char>(0x00883a28, 0x67); //POP ESI
                utils::set<char>(0x00883a29, 0x5e); //
                utils::set<char>(0x00883a2a, 0x5d); //POP EBP
                utils::set<char>(0x00883a2b, 0xb8); //MOV EAX, 0x0
                utils::set<int>(0x00883a2c, 0x00000000); //
                utils::set<char>(0x00883a30, 0xc2); //RET 0x8
                utils::set<char>(0x00883a31, 0x08); //
                utils::set<char>(0x00883a32, 0x00); //
            }

            CrashFixes::CreateTextureDetourReturn = 0x0077c6aa;
            utils::jump(0x0077c6a5, CrashFixes::CreateTextureCrashFix);

            //GetAmountFree = reinterpret_cast<decltype(//GetAmountFree)>(0x004b6590);
            VerifyPoolIntegrity = reinterpret_cast<decltype(VerifyPoolIntegrity)>(0x004b7e20);
            GetAmountFree = utils::call(0x004b6644, GetAmountFree_wrapper);
            AddFreeMemory = utils::call(0x004b6ef2, AddFreeMemory_wrapper);
            utils::call(0x004b6f7b, AddFreeMemory_wrapper);
            utils::call(0x004b7759, AddFreeMemory_wrapper);
            utils::call(0x004b7d02, AddFreeMemory_wrapper);
            break;
        }
        case UC_1_21: //UC 1.0.1.18
        {
            //remove kamikaze code, should be a duplicate of multifix
            {
                utils::nop(0x00401647, 10); //*0 = 0;
                utils::nop(0x004a9e6d, 10); //*0 = 0;
                utils::nop(0x004aacf0, 10); //*0 = 0;
                utils::nop(0x004ada06, 6); //*0 = 0;
                utils::nop(0x004adb96, 10); //*0 = 0;
                utils::nop(0x004adfe1, 10); //*0 = 0;
                utils::nop(0x004ae018, 10); //*0 = 0;
                utils::nop(0x004ae18a, 10); //*0 = 0;
                utils::nop(0x004ae1fb, 10); //*0 = 0;
                utils::nop(0x004ae245, 10); //*0 = 0;
                utils::nop(0x00527aed, 10); //*0 = 0;
                utils::nop(0x005dbe17, 10); //*0 = 0;
                utils::nop(0x005dc775, 5); //*0 = 0;
                utils::nop(0x00620245, 10); //*0 = 0;
                utils::nop(0x0065028b, 10); //*0 = 0;
                utils::nop(0x006677a1, 10); //*0 = 0;
                utils::nop(0x00682770, 10); //*0 = 0;
                utils::nop(0x00686222, 10); //*0 = 0;
                utils::nop(0x00689484, 10); //*0 = 0;
                utils::nop(0x006895d4, 10); //*0 = 0;
                utils::nop(0x0068b82a, 10); //*0 = 0;
                utils::nop(0x0068b872, 5); //*0 = 0;
                utils::nop(0x00691071, 10); //*0 = 0;
                utils::nop(0x00691702, 10); //*0 = 0;
                utils::nop(0x006918f4, 6); //*0 = 0;
                utils::nop(0x00692c3f, 6); //*0 = 0;
                utils::nop(0x00695c2b, 10); //*0 = 0;
                utils::nop(0x00698d0d, 6); //*0 = 0;
                utils::nop(0x00698fad, 6); //*0 = 0;
                utils::nop(0x0073a506, 10); //*0 = 0;
                utils::nop(0x0073aa08, 10); //*0 = 0;
                utils::nop(0x00747783, 10); //*0 = 0;
                utils::nop(0x0075c8f0, 10); //*0 = 0;
                utils::nop(0x0075c926, 10); //*0 = 0;
                utils::nop(0x0075c9bb, 10); //*0 = 0;
                utils::nop(0x0075cbea, 10); //*0 = 0;
                utils::nop(0x00760907, 6); //*0 = 0;
                utils::nop(0x007609b1, 6); //*0 = 0;
                utils::nop(0x00760b05, 10); //*0 = 0;
                utils::nop(0x00760d2a, 10); //*0 = 0;
                utils::nop(0x0076112d, 10); //*0 = 0;
                utils::nop(0x00761d88, 10); //*0 = 0;
                utils::nop(0x007621a6, 10); //*0 = 0;
                utils::nop(0x007639b6, 6); //*0 = 0;
                utils::nop(0x00763a6a, 6); //*0 = 0;
                utils::nop(0x00763d12, 10); //*0 = 0;
                utils::nop(0x007640f5, 10); //*0 = 0;
                utils::nop(0x0076ac79, 6); //*0 = 0;
                utils::nop(0x0076acab, 6); //*0 = 0;
                utils::nop(0x0076b08d, 6); //*0 = 0;
                utils::nop(0x007a4ebb, 10); //*0 = 0;
                utils::nop(0x00814af7, 10); //*0 = 0;
                utils::nop(0x00814b9b, 10); //*0 = 0;
                utils::nop(0x008236b8, 10); //*0 = 0;
                utils::nop(0x00823e61, 10); //*0 = 0;
                utils::nop(0x00824146, 10); //*0 = 0;
                utils::nop(0x0082476e, 6); //*0 = 0;
                utils::nop(0x00824d38, 10); //*0 = 0;
                utils::nop(0x00824d80, 10); //*0 = 0;
                utils::nop(0x008372cc, 10); //*0 = 0;
                utils::nop(0x008388e2, 10); //*0 = 0;
                utils::nop(0x0084c5bb, 10); //*0 = 0;
                utils::nop(0x0084c5f3, 10); //*0 = 0;
                utils::nop(0x0084c62b, 10); //*0 = 0;
                utils::nop(0x0084ca02, 10); //*0 = 0;
                utils::nop(0x00852afc, 10); //*0 = 0;
                utils::nop(0x00854806, 10); //*0 = 0;
                utils::nop(0x009dc508, 7); //*0 = 0;
            }

            //try to ignore int3 breakpoints (all of these are accompanied by a message box anyways)
            {
                utils::nop(0x00688ad0, 1);
                utils::nop(0x00688e50, 1);
                utils::nop(0x00a044a0, 1);
                utils::nop(0x00a04ca8, 1);
                utils::nop(0x00a04ee0, 1);
                utils::nop(0x00a04f66, 1);
                utils::nop(0x00a05251, 1);
                utils::nop(0x00a05308, 1);
                utils::nop(0x00a05395, 1);
                utils::nop(0x00a061ba, 1);
                utils::nop(0x00a061e4, 1);
                utils::nop(0x00a0622a, 1);
                utils::nop(0x00a062de, 1);
                utils::nop(0x00a06602, 1);
                utils::nop(0x00a0668c, 1);
                utils::nop(0x00a066d2, 1);
                utils::nop(0x00a06768, 1);
                utils::nop(0x00a069ab, 1);
                utils::nop(0x00a06a38, 1);
                utils::nop(0x00a06cff, 1);
                utils::nop(0x00a06d96, 1);
                utils::nop(0x00a06d9d, 1);
                utils::nop(0x00a06f48, 1);
                utils::nop(0x00a0701e, 1);
                utils::nop(0x00a070dc, 1);
                utils::nop(0x00a071e3, 1);
                utils::nop(0x00a077a8, 1);
            }

            //remove more kamikaze code
//            utils::set<char>(0x008547e5, 0xeb); //JMP instead of JNZ

            //remove function calls to seemingly broken CarLoader::PrintMemoryUsage
            {
                utils::nop(0x00852af0, 9);

                utils::set<char>(0x008547e7, 0x5f); //POP EDI
                utils::set<char>(0x008547e8, 0x67); //POP ESI
                utils::set<char>(0x008547e9, 0x5e); //
                utils::set<char>(0x008547ea, 0x5d); //POP EBP
                utils::set<char>(0x008547eb, 0xb8); //MOV EAX, 0x0
                utils::set<int>(0x008547ec, 0x00000000); //
                utils::set<char>(0x008547f0, 0xc2); //RET 0x8
                utils::set<char>(0x008547f1, 0x08); //
                utils::set<char>(0x008547f2, 0x00); //
                /*
                * PATCHED
                * pop edi
                * pop esi
                * pop ebp
                * mov eax,0x0
                * ret 0x8
                */
                /*
                * VANILLA
                * push eax
                * mov ecx, edi
                * call 00844300
                * mov eax, dword ptr [edi + 0x58]
                * lea ecx, [edi + 0x58]
                */
            }

            //first car crash fix (only for 1.0.1.18 - contains hardcoded function addresses !)
            //utils::jump(0x005b0947, CrashFixes::HUDLoadedDetour); //old method
            utils::call(0x005715bc, CrashFixes::hashBINImpersonator);
            utils::call(0x0085a15a, CrashFixes::forcePlayerCarDetour); //old method, keeping it to prevent the game to try to load unexisting cars
            //new method changes shit directly in hashBINImpersonator

            CreateTextureDetourReturn = 0x007514ba;
            utils::jump(0x007514b5, CreateTextureCrashFix);

            //GetAmountFree = reinterpret_cast<decltype(//GetAmountFree)>(0x004ac600);
            VerifyPoolIntegrity = reinterpret_cast<decltype(VerifyPoolIntegrity)>(0x004ade80);
            GetAmountFree = utils::call(0x004ac6b4, GetAmountFree_wrapper);
            AddFreeMemory = utils::call(0x004acf62, AddFreeMemory_wrapper);
            utils::call(0x004acfeb, AddFreeMemory_wrapper);
            utils::call(0x004ad7b9, AddFreeMemory_wrapper);
            utils::call(0x004add62, AddFreeMemory_wrapper);
            break;
        }
        }

        if (Settings::SkipBootupChecks) switch (entry) {
        case UC_1_0: //UC 1.0
            utils::nop(0x00757986, 17); //removes the CD drive check
            utils::nop(0x00759948, 47); //prevents waking up all drives
            utils::nop(0x006af1d4, 54); //removes another bootup check "nfsc.exe" / "nfsc_link.exe"
            break;
        case UC_1_21: //UC 1.0.1.18
            utils::nop(0x0072fca6, 17); //removes the CD drive check 
            utils::nop(0x00731c88, 47); //prevents waking up all drives
            utils::nop(0x00693B14, 54); //removes another bootup check "nfsc.exe" / "nfsc_link.exe"
            break;
        }

        if (Settings::EnableAllCameras) switch (entry) {
        case UC_1_0:
            utils::set<char>(0x00529be5, 8); //change max cam to 8
            utils::set<char>(0x00529c13, 0x56); //push esi
            utils::set<char>(0x00528bac, 0xff); //remove force disabled drift cam
            break;
        case UC_1_21:
            utils::set<char>(0x005141d5, 8); //change max cam to 8
            utils::set<char>(0x00514203, 0x56); //push esi
            utils::set<char>(0x0051378c, 0xff); //remove force disabled drift cam
            break;
        }

        // trucks collisions fix
        if (carsModuleInstalled && Settings::FixTruckTrailers) {
            struct stat GlobalBMeta;
            stat("../GLOBAL/GlobalB.lzc", &GlobalBMeta);
            if (GlobalBMeta.st_mtime > Settings::lastGoodGlobalB) {
                std::ifstream input("../GLOBAL/GlobalB.lzc", std::ios::binary);

                uint32_t blockID = readInt(input);
                if (blockID == 1263552082 || blockID == 1514947658 || blockID == 1465336146) {
                    //input compressed
                    input.close();
                }
                else {
                    std::ofstream output("../GLOBAL/GlobalB.lzc.tmp", std::ios::binary);

                    while (true) {
                        output.write(reinterpret_cast<char*>(&blockID), sizeof(blockID));
                        if (blockID == 0x8003b900) {
                            //Collisions container
                            uint32_t blockSize = readInt(input);
                            output.write(reinterpret_cast<char*>(&blockSize), sizeof(blockSize));

                            char* blockData = new char[blockSize];
                            input.read(blockData, blockSize);

                            uint32_t carBlockOffset = 0;
                            while (true) {
                                carBlockOffset += 8;
                                int carBlockLength = ((int*)blockData + carBlockOffset / 4 - 1)[0];
                                char* carName = blockData + carBlockOffset + 104;
                                if (carBlockLength == 1848 && (strcmp(carName, "TRF_TRA_BOX_83") == 0 || strcmp(carName, "TRF_TRA_FLT_84") == 0 || strcmp(carName, "TRF_TRA_LOG_84") == 0)) {
                                    blockData[carBlockOffset + 393] = 0x04;
                                    blockData[carBlockOffset + 553] = 0x05;
                                    blockData[carBlockOffset + 697] = 0x05;
                                }
                                if (carBlockLength == 3880 && strcmp(carName, "TRF_TRK_SEM_95") == 0) { //check block size to avoid editing non-matching collisions
                                    blockData[carBlockOffset + 425] = 0x03;
                                }

                                carBlockOffset += carBlockLength;
                                if (carBlockOffset >= blockSize) break;
                            }

                            output.write(blockData, blockSize);
                            delete[] blockData;

                        }
                        else {
                            uint32_t blockSize = readInt(input);
                            output.write(reinterpret_cast<char*>(&blockSize), sizeof(blockSize));
                            char* blockData = new char[blockSize];
                            input.read(blockData, blockSize);
                            output.write(blockData, blockSize);
                            delete[] blockData;
                        }
                        if (input.peek() == EOF) break;
                        blockID = readInt(input);
                    }
                    input.close();
                    output.flush();
                    output.close();

                    remove("../GLOBAL/GlobalB.lzc");
                    rename("../GLOBAL/GlobalB.lzc.tmp", "../GLOBAL/GlobalB.lzc");


                    stat("../GLOBAL/GlobalB.lzc", &GlobalBMeta);
                    Settings::lastGoodGlobalB = GlobalBMeta.st_mtime;
                    Settings::updateConfig();
                }
            }
        }
       

#if 1 // 54k crash fix (goofy code and maybe unnecessary but oh well)
        if (carsModuleInstalled && Settings::CrashFixes) switch (entry) {
        case UC_1_0: //UC 1.0
            GeomAlloc::loopStart = (void*)0x007a4110;
            GeomAlloc::allocateNewBlock = (void*)0x0079dc60;
            GeomAlloc::calculation = (void*)0x0079b8d0;
            GeomAlloc::forceSkipExit = (void*)0x007a416e;
            utils::jump(0x007a4100, GeomAlloc::CounterReset);
            utils::jump(0x007a4167, GeomAlloc::AllocOrSkip);
            break;
        case UC_1_21: //UC 1.0.1.18
            GeomAlloc::loopStart = (void*) 0x00777720;
            GeomAlloc::allocateNewBlock = (void*) 0x00771df0;
            GeomAlloc::calculation = (void*) 0x0076fe00;
            GeomAlloc::forceSkipExit = (void*) 0x0077777e;
            utils::jump(0x00777710, GeomAlloc::CounterReset);
            utils::jump(0x00777777, GeomAlloc::AllocOrSkip);
            break;
        default:
            std::cout << "Unsupported EXE, over polycount limit crash fix not applied.\n";
        }
        if (carsModuleInstalled && Settings::IndexBufferSize != 0x50000) switch (entry) {
        case UC_1_0: //UC 1.0
            //alloc function
            utils::set<int>(0x0079dcbb, Settings::IndexBufferSize);
            utils::set<int>(0x0079dcde, Settings::IndexBufferSize);
            utils::set<int>(0x0079dd06, Settings::IndexBufferSize);
            utils::set<int>(0x0079dd2c, Settings::IndexBufferSize);

            //other function with the same value
            utils::set<int>(0x0077c307, Settings::IndexBufferSize);
            utils::set<int>(0x0077c329, Settings::IndexBufferSize);
            utils::set<int>(0x0077c350, Settings::IndexBufferSize);
            break;
        case UC_1_21: //UC 1.0.1.18
            //alloc function
            utils::set<int>(0x00771e4b, Settings::IndexBufferSize);
            utils::set<int>(0x00771e6e, Settings::IndexBufferSize);
            utils::set<int>(0x00771e96, Settings::IndexBufferSize);
            utils::set<int>(0x00771ebc, Settings::IndexBufferSize);

            //other function with the same value
            utils::set<int>(0x00751117, Settings::IndexBufferSize);
            utils::set<int>(0x00751139, Settings::IndexBufferSize);
            utils::set<int>(0x00751160, Settings::IndexBufferSize);
            break;
        }
#endif // 54k crash fix

#if 1
        if (carsModuleInstalled && Settings::CarsPoolSize != 0x4e20000) switch (entry) {
        case UC_1_0: //UC 1.0
            utils::set(0x0069e819, Settings::CarsPoolSize);
            utils::set(0x0069e828, Settings::CarsPoolSize);
            utils::set(0x0069e888, Settings::CarsPoolSize);
            utils::set(0x005f77ac, Settings::CarsPoolSize);
            break;
        case UC_1_21: //UC 1.0.1.18
            utils::set(0x005dd0dc, Settings::CarsPoolSize);
            utils::set(0x00683188, Settings::CarsPoolSize);
            utils::set(0x00683128, Settings::CarsPoolSize);
            utils::set(0x00683119, Settings::CarsPoolSize);
            break;
        }
#endif //cars pool increase

#if 1 // verbose
        if (Settings::verbose) switch (entry) {
            using namespace Verbose;
        case UC_1_21: //UC 1.0.1.18
            //__alloca_probe = (void*)0x0087b250;
            utils::jump(0x00a07820, DebugPrintReplacement);

            bReleasePrintf = utils::call(0x004adfc0, bReleasePrintf_ERR); //MemoryPool::VerifyPoolIntegrity
            utils::call(0x004adfd0, bReleasePrintf_ERR);
            utils::call(0x004adf9a, bReleasePrintf_ERR);
            utils::call(0x004adfab, bReleasePrintf_ERR);
            utils::call(0x004ae010, bReleasePrintf_ERR);
            utils::call(0x004ae1f0, bReleasePrintf_ERR); //MemoryPool something
            utils::call(0x005dbe0c, bReleasePrintf_ERR); //Out of FastMem memory
            break;
        case UC_1_0: //UC 1.0
            //__alloca_probe = (void*)0x008ae430;
            utils::jump(0x00a3a580, DebugPrintReplacement);

            bReleasePrintf = utils::call(0x004b7f60, bReleasePrintf_ERR); //MemoryPool::VerifyPoolIntegrity
            utils::call(0x004b7f70, bReleasePrintf_ERR);
            utils::call(0x004b7f3a, bReleasePrintf_ERR);
            utils::call(0x004b7f4b, bReleasePrintf_ERR);
            utils::call(0x004b7f70, bReleasePrintf_ERR);
            utils::call(0x004b8190, bReleasePrintf_ERR); //MemoryPool something
            utils::call(0x005f637c, bReleasePrintf_ERR); //Out of FastMem memory
            break;
        }
#endif //verbose

#if 1 // Day/night cycle
        if (carsModuleInstalled && Settings::useDayNightCycle) DayNight::Inject();
#endif // Day/night cycle

#if 1 // Per kit ride height (bit goofy but works)
        if (carsModuleInstalled && Settings::KitRideHeight) switch (entry) {
            using namespace KitRideHeight;
            case UC_1_21: //UC 1.0.1.18
                RideHeightDetourReturn = (void*)0x0082a3b8;
                utils::jump(0x0082a37d, RideHeightDetour);

                FERideHeightDetourReturn = (void*)0x008141ab;
                utils::jump(0x00814187, FERideHeightDetour);
                break;
            case UC_1_0: //UC 1.0
                RideHeightDetourReturn = (void*)0x0085799c;
                utils::jump(0x00857950, RideHeightDetour_10); //not the exact same code as 1.0.1.18 !

                FERideHeightDetourReturn = (void*)0x0083fc22;
                utils::jump(0x0083fbe7, FERideHeightDetour_10);
                break;
        }
#endif // Per kit ride height

#if 1
        if (carsModuleInstalled && Settings::HeadlightTweaksEnabled) switch (entry) {
            using namespace Headlights;
        case UC_1_0: //UC 1.0
            CRIoff_mOnLights = 0x52f8;
            CRIoff_mBrokenLights = 0x52fc;
            CRIoff_mCrackedLights = 0x5300;
            CRIoff_headlightTextures = 0x52b8;
            CRIoff_headlightGlassTextures = 0x52c4;
            CRIoff_brakelightTextures = 0x52d0;
            CRIoff_brakelightGlassTextures = 0x52dc;
            CRIoff_LightsReplacementTextureTable = 0x8c8;
            CRIoff_mSelfIlluminationFlags = 0x54b8;
            CRIoff_terrainAdj = 0x4ac;
            CRIoff_MPointsArrayPtr = 0x4b4;

            RideInfoOff_VehType = 0xad0;

            //projection
            HEADLIGHTS_ptr = (char**)0x00ea0240;
            utils::jump(0x00878ab6, replaceCarshadowDetour_10); //seems similar enough to use the same detour ??? theres a few less tests though
            replaceCarshadowDetourReturn = (void*)0x00878b19;

            //get raw flare status without damage
            utils::set(0x72f223, (char)0xEB); //jmp short
            utils::set(0x72f224, (char)0x27); //jmp to 70826c (actually the same distance as in 1.18, shit is compiled the exact same)
            utils::jump(0x0072f864, getRawFlaresDetour_10);

            //link emissive headlights to flare headlights
            //THERES AN OFFSET IN CARRENDERINFO
            utils::nop(0x0078d290, 6); //remove early assembler instruction in cBBFXCar::...
            utils::jump(0x0078d2ad, setEmissiveLightsDetour);
            setEmissiveLightsDetourReturn = (void*)0x0078d2c0;
            utils::call(0x00878834, lightsUpdateReplacement); //replace bullshit function
            utils::set<char>(0x0087a5eb, 0x00); //fix stuck brakelights on car spawn (might be fixed by my other function anyways)

            //automatic headlights
            if (Settings::AutomaticHeadlights) utils::call(0x006d245c, headlightsReplacement);

            break;
        case UC_1_21: //UC 1.0.1.18
            //CarRenderInfo offsets
            CRIoff_mOnLights = 0x5308;
            CRIoff_mBrokenLights = 0x530c;
            CRIoff_mCrackedLights = 0x5310;
            CRIoff_headlightTextures = 0x52c8;
            CRIoff_headlightGlassTextures = 0x52d4;
            CRIoff_brakelightTextures = 0x52e0;
            CRIoff_brakelightGlassTextures = 0x52ec;
            CRIoff_LightsReplacementTextureTable = 0x8d4;
            CRIoff_mSelfIlluminationFlags = 0x54c8;
            CRIoff_terrainAdj = 0x4b8;
            CRIoff_MPointsArrayPtr = 0x4C0;

            RideInfoOff_VehType = 0xe44;

            //projection
            HEADLIGHTS_ptr = (char**)0x013b32c0;
            utils::jump(0x0084948e, replaceCarshadowDetour);
            replaceCarshadowDetourReturn = (void*)0x008494fd;

            //get raw flare status without damage
            utils::set(0x708243, (char)0xEB); //jmp short
            utils::set(0x708244, (char)0x27); //jmp to 70826c
            utils::jump(0x00708847, getRawFlaresDetour);

            //link emissive headlights to flare headlights
            utils::nop(0x007612d0, 6); //remove early assembler instruction in cBBFXCar::...
            utils::jump(0x007612ED, setEmissiveLightsDetour);
            setEmissiveLightsDetourReturn = (void*)0x00761300;
            utils::call(0x008491ed, lightsUpdateReplacement); //replace bullshit function
            utils::set<char>(0x0084b01f, 0x00); //fix stuck brakelights on car spawn (might be fixed by my other function anyways)

            //automatic headlights
            if (Settings::AutomaticHeadlights) utils::call(0x006b6e1c, headlightsReplacement);

            break;
        }
#endif // Headlights

#if 1
        if (carsModuleInstalled && Settings::ExtendAftermarketMenu) switch (entry) {
            using namespace FEAftermarketMenu;
        case UC_1_0: //UC 1.0
            utils::call(0x005a99d1, LoadAftermarketMenu);

            utils::call(0x005676a5, GetCurrentSlotID);
            utils::call(0x00581545, GetCurrentSlotID);

            utils::call(0x0059f053, getPartsList);

            utils::call(0x005e95cf, getAutosculptZone); //crash fix
            utils::call(0x005ea07d, getAutosculptZone);
            utils::call(0x005ea15e, getAutosculptZone);

            utils::nop(0x005eacd9, 14); //remove StandardSelectablePart::SetSecondarySlot restriction to only wheels
            utils::call(0x005eaf25, SetSecondarySlot_PartsSelector);
            utils::jump(0x005eaf2a, 0x005eaf7e);

            if (Settings::AllowMoreCarbonParts) {
                utils::nop(0x005ed8a5, 2); //CF
                utils::nop(0x005ed8aa, 2);
                RideInfo__FindPaintSlotForPartSlot = reinterpret_cast<decltype(RideInfo__FindPaintSlotForPartSlot)>(0x00843bf0);
                DBCarPart_IsNotPaintable = (bool(__fastcall*)(DBCarPart*))utils::call(0x005ed8b0, DBCarPart_IsNotPaintable_cf_allslots_wrapper); //cast needed because a parameter is added with the following push esi
                utils::set<char>(0x005ed8a7, 0x67); //push esi
                utils::set<char>(0x005ed8a8, 0xff);
                utils::set<char>(0x005ed8a9, 0xf6);
            }
            if (Settings::AddMaterialToggle) {
                FEAftermarketStateManager__GetPart = (DWORD(__fastcall*)(DWORD th, DWORD, DWORD)) 0x00575890;
                utils::set(0x00bf1a34, FEAftermarketCycleMaterials); //button 2 handler in FEAftermarketStateManager
            }
            break;
        case UC_1_21: //UC 1.0.1.18
            utils::call(0x00595021, LoadAftermarketMenu);

            utils::call(0x0054e4a5, GetCurrentSlotID);
            utils::call(0x00567c35, GetCurrentSlotID);

            utils::call(0x00588fc3, getPartsList);

            utils::call(0x005cfc9f, getAutosculptZone); //crash fix
            utils::call(0x005d074d, getAutosculptZone);
            utils::call(0x005d082e, getAutosculptZone);

            utils::nop(0x005d150c, 14); //remove StandardSelectablePart::SetSecondarySlot restriction to only wheels
            //utils::jump(0x005d1908, 0x005d1994); //other parts work
            //utils::jump(0x005d1908, 0x005d1958); //seats and wheels work
            //utils::nop(0x005d1902, 6); //remove StandardSelectablePart::SetSecondarySlot restriction to parts with different names and slots
            utils::call(0x005d18f8, SetSecondarySlot_PartsSelector);
            utils::jump(0x005d18fd, 0x005d1951);

            if (Settings::AllowMoreCarbonParts) {
                utils::nop(0x005d4455, 2); //CF
                utils::nop(0x005d445a, 2);
                RideInfo__FindPaintSlotForPartSlot = reinterpret_cast<decltype(RideInfo__FindPaintSlotForPartSlot)>(0x00817f10);
                DBCarPart_IsNotPaintable = (bool(__fastcall*)(DBCarPart*))utils::call(0x005d4460, DBCarPart_IsNotPaintable_cf_allslots_wrapper); //cast needed because a parameter is added with the following push esi
                utils::set<char>(0x005d4457, 0x67); //push esi
                utils::set<char>(0x005d4458, 0xff);
                utils::set<char>(0x005d4459, 0xf6);
            }
            if (Settings::AddMaterialToggle) {
                FEAftermarketStateManager__GetPart = (DWORD(__fastcall*)(DWORD th, DWORD, DWORD)) 0x0055b7e0;
                utils::set(0x00b7c6cc, FEAftermarketCycleMaterials); //button 2 handler in FEAftermarketStateManager
                /*
                MATERIAL_NONE
                MATERIAL_CARBON
                MATERIAL_CHROME
                MATERIAL_MAGSILVER
                MATERIAL_GLASS (unused)
                MATERIAL_GUNMETAL
                MATERIAL_UNUSED3
                MATERIAL_UNUSED4
                MATERIAL_UNUSED5
                */
            }
            break;
        }
#endif //FE aftermarket menu

#if 1
        if (carsModuleInstalled && Settings::ReadCarListFromAttrib) switch (entry) {
            using namespace FECarList;
        case UC_1_21:
            IgnorePlayerUsable_012bb1d8 = (bool*)0x012bb1d8;

            CreateNewCustomCarDetourReturn = 0x005174a6;
            BuildRideForPlayerDetourReturn = 0x0050fa12;

            utils::set(0x00b71fe4, DALVehicleCommands__GetStockCars__Execute);

            //namespace FEPlayerCarDB
            utils::jump(0x00501ff0, GetCarRecordByHandle);
            utils::jump(0x00502030, GetCarRecordByFEKey);
            utils::jump(0x00502070, GetStockCarRecord);
            utils::jump(0x005020f0, GetCarByIndex);
            utils::jump(0x005138d0, CreateCar);
            utils::jump(0x00517481, CreateNewCustomCarDetour);
            utils::jump(0x0050f9f0, BuildRideForPlayerDetour);

            utils::jump(0x005107c0, DALFeVehicle__GetCarNameHash);
            utils::jump(0x00503b80, DALFeVehicle__GetCarHandleByIndex);
            utils::jump(0x0050a970, DALFeVehicle__GetCarKeyByHandle);
            utils::jump(0x0050abc0, DALFeVehicle__GetStockCarValue);
            utils::jump(0x0050ac40, DALFeVehicle__GetCarTier);

            //TODO DALVehicleCommands::GetCheapestStockCar::Execute
            //necessary in DALVehicleCommands::GetCustomizableCars ?
            //DALVehicleCommands::GetManufacturerCar::Execute likely unneeded

            utils::call(0x005125e9, GetCarRecordByHandle_safe_wrapper); //fixes undefined behavior causing a crash due to ECX expected unchanged

            if (Settings::RemoveStockCarRecords) utils::jump(0x00513afa, 0x00513c4c); //skips loading the car list in the save
            if (Settings::Remove20CarLimit) utils::call(0x0054e709, return_zero);
            break;

        case UC_1_0:

            //OF FUCKING COURSE
            //IT DOES NOT RECOGNIZE THE CAR RECORD AFTER PURCHASE
            //WHY
            IgnorePlayerUsable_012bb1d8 = (bool*)0x00daa165; //ok

            utils::set(0x00be7420, DALVehicleCommands__GetStockCars__Execute);//ok

            //namespace FEPlayerCarDB
            utils::jump(0x00518310, GetCarRecordByHandle); //ok
            utils::jump(0x00518350, GetCarRecordByFEKey);//ok
            utils::jump(0x00518390, GetStockCarRecord); //ok
            utils::jump(0x00518410, GetCarByIndex);//ok
            utils::jump(0x00528cf0, CreateCar);//ok

            CreateNewCustomCarDetourReturn = 0x0052d176; //ok
            BuildRideForPlayerDetourReturn = 0x00524f52; //ok
            utils::jump(0x0052d151, CreateNewCustomCarDetour); //ok
            utils::jump(0x00524f30, BuildRideForPlayerDetour); //ok

            utils::jump(0x0052a210, DALFeVehicle__GetCarNameHash);//ok
            utils::jump(0x00519dc0, DALFeVehicle__GetCarHandleByIndex);
            utils::jump(0x005203a0, DALFeVehicle__GetCarKeyByHandle);//ok
            utils::jump(0x00520600, DALFeVehicle__GetStockCarValue);//ok
            utils::jump(0x00520680, DALFeVehicle__GetCarTier);//ok

            //TODO DALVehicleCommands::GetCheapestStockCar::Execute
            //necessary in DALVehicleCommands::GetCustomizableCars ?
            //DALVehicleCommands::GetManufacturerCar::Execute likely unneeded

            utils::call(0x00527989, GetCarRecordByHandle_safe_wrapper); //ok, fixes undefined behavior causing a crash due to ECX expected unchanged

            if (Settings::RemoveStockCarRecords) utils::jump(0x00529551, 0x0052974d); //skips loading the car list in the save
            if (Settings::Remove20CarLimit) utils::call(0x005678f9, return_zero);
            break;
        }
#endif //FE car list

        if (!MissShiftEffect) MissShiftEffect = getVltHash("MissShiftEffect");
        if (!NOSEffect) NOSEffect = getVltHash("NOSEffect");
        //effects
        if (carsModuleInstalled) switch (entry) {
            using namespace CarRenderConn;
        case UC_1_21:
            if (Settings::HeadlightShatterFix) utils::call(0x0084260a, UpdateHeadlightAndBrakelightShatterEffects); //fix headlight break effect using the breakelight break fx (called from the previous function)
            if (Settings::EffectsEnhancements) {
                utils::call(0x00859c06, UpdateEffects);
                utils::set<uint32_t>(0x0085a23a, CarRenderConnNewLength); //CarRenderConn alloc size
                constructor = utils::call(0x0085a258, constructor_wrapper);
                OnLoaded = utils::set(0x00bbee00, OnLoaded_wrapper);

                DistanceFactor = (float*)0x00cdfd34;
                DAT_0178c9f4 = (DWORD*)0x0178c9f4;
            }
            break;
        case UC_1_0:
            if (Settings::HeadlightShatterFix) utils::call(0x00871d9a, UpdateHeadlightAndBrakelightShatterEffects); //fix headlight break effect using the breakelight break fx (called from the previous function)
            if (Settings::EffectsEnhancements) {
                utils::call(0x00888ea4, UpdateEffects);
                utils::set<uint32_t>(0x008894b5, CarRenderConnNewLength); //CarRenderConn alloc size
                constructor = utils::call(0x008894d3, constructor_wrapper);
                OnLoaded = utils::set(0x00c32ce8, OnLoaded_wrapper);

                DistanceFactor = (float*)0x00cdfd34;
                DAT_0178c9f4 = (DWORD*)0x0127995c;
            }
            break;
        }

        if (Settings::AddFreeroamMusic) switch (entry) {
            using namespace FreeroamMusic;
        case UC_1_21:
            SFXCTL_MusicPlayback_vftable = 0x00b6d458;

            Music_PathfinderControl = (DWORD*)0x012946a8;
            MusicProject = (Music::MusicProject*)0x00cb2da0;
            EAXSound = (DWORD*)0x01294590;

            Pause = (void*)0x004a4be0;
            MapScreen = (void*)0x004a3300;
            CarSelect = (void*)0x004a3460;
            FreeroamMusic::Credits = (void*)0x004a35c0;
            Start = (void*)0x00494930;
            StartPursuit = (void*)0x0048ece0;
            StartChase = (void*)0x0048b4a0;
            RaceInit = (void*)0x0046f300;
            Custom = (void*)0x004a52d0;
            StartBattle = (void*)0x0048cc90;

            SkipChyron = (bool*)0x012aa3a0;
            
            CSTATEMGR_Base__CreateSFXCTL = utils::call(0x00475068, CSTATEMGR_Base__CreateSFXCTL_wrapper);
            utils::jump(0x004a5050, Sound__MusicLogic__Ready);

            utils::set<char>(0x0057f6cb, 0xeb); //jmp instead of jnz for chyron music

            utils::jump(0x0048f7c8, TransitionStopChyron);

            utils::call(0x004687db, GetRaceTracks_wrapper_chyron); //RaceMusic
            utils::call(0x0046f446, GetRaceTracks_wrapper_chyron); //RaceInit
            break;

        case UC_1_0:
            SFXCTL_MusicPlayback_vftable = 0x00be28a8;

            Music_PathfinderControl = (DWORD*)0x00d83de8;
            MusicProject = (Music::MusicProject*)0x00d25da0;
            EAXSound = (DWORD*)0x00d83cd0;

            Pause = (void*)0x004adb80;
            MapScreen = (void*)0x004ac290;
            CarSelect = (void*)0x004ac3f0;
            FreeroamMusic::Credits = (void*)0x004ac550;
            Start = (void*)0x0049d310;
            StartPursuit = (void*)0x00496ca0;
            StartChase = (void*)0x00493320;
            RaceInit = (void*)0x00475d90;
            Custom = (void*)0x004ae290;
            StartBattle = (void*)0x00494af0;

            SkipChyron = (bool*)0x00d996b0;

            CSTATEMGR_Base__CreateSFXCTL = utils::call(0x0047be18, CSTATEMGR_Base__CreateSFXCTL_wrapper);
            utils::jump(0x004ae010, Sound__MusicLogic__Ready);

            utils::set<char>(0x005a165b, 0xeb); //jmp instead of jnz for chyron music

            utils::jump(0x00497788, TransitionStopChyron);

            utils::call(0x0046ef9b, GetRaceTracks_wrapper_chyron); //RaceMusic
            utils::call(0x00475ec6, GetRaceTracks_wrapper_chyron); //RaceInit
            break;
        }

        if (Settings::AddDecalsSelection) VinylsSelectMenu::Inject();

        if (Settings::tests) {
            ucetesting::doTests();
        }

        if (Settings::AllowPerformanceDowngrade) switch (entry) {
        case UC_1_21:
            utils::set<char>(0x005a5375, 0xeb); //allow downgrading, not showing stock though
            utils::nop(0x005a5412, 6);
            utils::nop(0x005a545d, 6);
            utils::nop(0x005a549d, 6);
            utils::nop(0x005a54e1, 6);
            break;
        case UC_1_0:
            utils::set<char>(0x005bea25, 0xeb); //allow downgrading, not showing stock though
            utils::nop(0x005beac2, 6);
            utils::nop(0x005beb0d, 6);
            utils::nop(0x005beb4d, 6);
            utils::nop(0x005beb91, 6);
            break;
        }

        if (Settings::FixGPSPath) {
            switch (entry) {
            case UC_1_21:
                utils::set<char>(0x005a88fb, 0xeb); //fix gps path disappearing when engaging a pursuit mid-race
                break;
            }
        }
    } //dllmain


    //for (auto c : Settings::VisualStyleCycles) {
    //    for (auto w : c) {
    //        std::cout << w << " ";
    //    }
    //    std::cout << "\n";
    //}



    return TRUE;
}


