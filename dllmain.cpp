// dllmain.cpp : Définit le point d'entrée de l'application DLL.
/*
build paths
desktop
D:\Jeux\UCE 1.0.1.18\scripts\UndercoverExposed.asi

laptop

C:\Program Files (x86)\EA Games\Need for Speed Undercover\scripts\UndercoverExposed.asi

*/
#pragma once
#include "dllmain.h"
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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "nfs_structs.h"
#include <chrono>


using namespace std;

static uint32_t readInt(std::ifstream& input) {
    uint32_t a;
    input.read(reinterpret_cast<char*>(&a), sizeof(a));
    return a;
}



//useful shit
auto getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x0;
auto getBinHash = (uint32_t(__cdecl*)(char* toHash)) 0x0;
auto MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x0;
auto getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x0;

DWORD* MemBlock_Global;
DWORD* MemBlock_FE;
DWORD* MemBlock_Cars;

const DWORD zero = 0x00000000;


int* GameFlow;
int* Renderer;
DWORD* FEScreenManager;


DWORD* UserProfile_Player1;
DWORD* FECarHeaven;
//DWORD CurrentFECarPTR_012ac35c = 0x012ac35c;
DWORD* FEOptionsStateManager;
DWORD* FEPerformanceTuning;
DWORD* FEVisualPaint;
DWORD* FEVinylPaintScreen;
DWORD* FEAutosculptScreen;

DWORD* DebugVehicleSelection;

int* timer_world;

SunAndSky* theSunAndSky;

DWORD CURRENT_GAME_VERSION;
//void* hwnd = *(void**)0x01304e98;
//IDirect3DDevice9* g_pd3dDevice = *(IDirect3DDevice9**)0x013b3190;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


std::random_device rd;
std::mt19937 gen(rd());


auto FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
auto FE_getFeElement = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
auto AddToTree = (void* (__fastcall*)(DWORD this_, DWORD unaff, LinkedNode*, DWORD)) 0x005563c0;
auto FUN_00553aa0 = (void(__fastcall*)(LinkedNode * this_)) 0x00553aa0;
auto FUN_getLinkedListLength_004a76f0 = (DWORD(__fastcall*)(LinkedNode * this_, DWORD unaff, DWORD)) 0x004a76f0;




DALInterface** DALOptions;

#define DEFAULT 0xeec2271a


enum GAME_VERSION {
    UC_1_0 = 0x008AEC55,
    UC_1_21 = 0x0087ba75
};


namespace Settings{
    static bool CrashFixes = true;
    static bool FixTruckTrailers = true;
    static bool verbose = true;
    static bool KitRideHeight = true;
    static int lastGoodGlobalB = 1242129289;

    static int IndexBufferSize = 0x50000;
    static int CarsPoolSize = 0x4e20000;

    static bool useDayNightCycle = false;
    static bool DayNightCycleRandomStart = true;
    static vector<vector<uint32_t>> VisualStyleCycles;
    static int DayNightCycleDuration = 24 * 60;
    //static int DayNightCycleDuration = 10;
    static bool SkipBootupChecks = true;

    static bool HeadlightTweaksEnabled = true;
    static bool AutomaticHeadlights = true;
    static int AutomaticHeadlightsField = 0x60;
    static float AutomaticHeadlightsThresold = 0.5;
    static bool ProjectionCutoff = true;
    static int ProjectionCutoffField = 0x60;
    static float ProjectionCutoffThresold = 0.5;

    static bool ExtendAftermarketMenu = false;
    static vector<pair<DWORD, char*>> AftermarketMenu;

    static bool SliderEnhancements = true;
    static bool EnableAllCameras = true;
    static bool EnableOverlay = false;

    static bool HeadlightShatterFix = true;
    static bool EffectsEnhancements = true;

    static bool tests = false;

    static void getConfigSetting(string setting, string line, int* ret) {
        if (line.find(setting + " ") == 0) {
            int value = line.find("=") + 1;
            while (line.at(value) == ' ' || line.at(value) == '\t') value++;
            if (line.at(value) == '0' && line.at(value + 1) == 'x') {
                sscanf_s(line.substr(value, std::string::npos).c_str(), "%x", ret);
            }
            else {
                sscanf_s(line.substr(value, std::string::npos).c_str(), "%d", ret);
            }
            //std::cout << setting << " = " << *ret << "\n";
        }
    }

    static void getConfigSetting(string setting, string line, float* ret) {
        if (line.find(setting + " ") == 0) {
            int value = line.find("=") + 1;
            while (line.at(value) == ' ' || line.at(value) == '\t') value++;
            sscanf_s(line.substr(value, std::string::npos).c_str(), "%f", ret);
            //std::cout << setting << " = " << *ret << "\n";
        }
    }

    static void getConfigSetting(string setting, string line, bool* ret) {
        if (line.find(setting + " ") == 0) {
            int value = line.find("=") + 1;
            while (line.at(value) == ' ' || line.at(value) == '\t') value++;
            int i;
            sscanf_s(line.substr(value, std::string::npos).c_str(), "%d", &i);
//            printf("setting %s value %d\n", setting.c_str(), (i == 1));
            *ret = (i == 1);
        }
    }

    static void getConfigSettingVltHash(string setting, string line, DWORD* ret) {
        if (line.find(setting + " ") == 0) {
            int i = line.find("=") + 1;
            while (line.at(i) == ' ' || line.at(i) == '\t') i++;

            int start = i;

            // "reads" the string
            while (line.at(i) != ' ' && line.at(i) != '\t') i++;

            if (i != start) {
                string toHash = line.substr(start, i - start);
                //cout << "String to hash : " << toHash << "\n";
                //tempNames->push_back(toHash);

                if (getVltHash != (uint32_t(__cdecl*)(char* toHash))0x0) {
                    *ret = getVltHash((char*)toHash.c_str());
                }
                else std::cout << "Undercover Exposed: Can't access the Vlt hasher!";

            }
        }
    }

    static void getConfigSettingVltHash2DArray(string setting, string line, vector<vector<uint32_t>>* ret) {
        if (line.find(setting + " ") == 0) {

            //vector<string> *tempNames = new vector<string>();



            uint32_t i = line.find("=") + 1;
            while (line.at(i) == ' ' || line.at(i) == '\t') i++;
            if (line.at(i) != '[' && line.at(i) != '{' && line.at(i) != '(') {
                std::cout << "Undercover Exposed: Bad array format in configuration file!\n";
                return;
            }
            i++;

            //cout << "Array entered\n";
            while (i < line.size() && line.at(i) != ']' && line.at(i) != '}' && line.at(i) != ')') { //enter the outer brackets
                while (i < line.size() && (line.at(i) == ' ' || line.at(i) == '\t' || line.at(i) == ',' || line.at(i) == ';')) i++; //skip a blank
                if (line.at(i) == ']' || line.at(i) == '}' || line.at(i) == ')') break; //end of array
                if (line.at(i) != '[' && line.at(i) != '{' && line.at(i) != '(') { //start of inner array
                    std::cout << "Undercover Exposed: Bad array format in configuration file!\n";
                    //cout << line.at(i) << "\n";
                    return;
                }
                i++;
                vector<uint32_t> cycle;
                
                //cout << "Inner array entered\n";
                while (i < line.size() && line.at(i) != ']' && line.at(i) != '}' && line.at(i) != ')') { //browse inner brackets
                    while (line.at(i) == ' ' || line.at(i) == '\t' || line.at(i) == ',' || line.at(i) == ';' || line.at(i) == '[' || line.at(i) == '{' || line.at(i) == '(') i++;
                    int start = i;

                    // "reads" the string
                    while (line.at(i) != ' ' && line.at(i) != '\t' && line.at(i) != ',' && line.at(i) != ';' && i < line.size() && line.at(i) != ']' && line.at(i) != '}' && line.at(i) != ')') i++;

                    if (i != start) {
                        string toHash = line.substr(start, i-start);
                        //cout << "String to hash : " << toHash << "\n";
                        //tempNames->push_back(toHash);

                        if (getVltHash != (uint32_t(__cdecl*)(char* toHash))0x0) {
                            cycle.push_back(getVltHash((char*)toHash.c_str()));
                        }
                        else std::cout << "Undercover Exposed: Can't access the Vlt hasher!";

                    }
                    while (i < line.size() && (line.at(i) == ' ' || line.at(i) == '\t' || line.at(i) == ',' || line.at(i) == ';')) i++; //skip a blank
                    //std::cout << "finished reading word, next character : " << line.at(i) << "\n";
                }
                i++;
                //std::cout << "finished browsing inner brackets, next character : " << line.at(i) << "\n";
                ret->push_back(cycle);
                

                while (i < line.size() && (line.at(i) == ' ' || line.at(i) == '\t' || line.at(i) == ',' || line.at(i) == ';')) i++; //skip a blank
            }




            //std::cout << "Array read successfully i guess\n";

            //int count = 0;
            //for (int i = 0; i < ret->size(); i++) {
            //    std::cout << "Cycle no " << i << "\n";
            //    for (int j = 0; j < ret->at(i).size(); j++) {
            //        std::cout << tempNames->at(count) << " " << ret->at(i)[j] << "\n";
            //        count++;
            //    }
            //}
            //delete tempNames;

        }
    }

    static void getConfigSettingMenuItem(string setting, string line, vector<pair<DWORD, char*>> * Menu) {
        if (line.find(setting + " ") == 0) {

            //vector<string> *tempNames = new vector<string>();
            DWORD ItemKey;
            char* ItemName;


            uint32_t i = line.find("=") + 1;
            while (line.at(i) == ' ' || line.at(i) == '\t') i++;
            if (line.at(i) == '0' && line.at(i + 1) == 'x') {
                sscanf_s(line.substr(i, std::string::npos).c_str(), "%x", &ItemKey);
            }
            else {
                sscanf_s(line.substr(i, std::string::npos).c_str(), "%d", &ItemKey);
            }

            i = line.find(",") + 1;
            while (line.at(i) == ' ' || line.at(i) == '\t') i++;
            ItemName = new char [32];
            int j = 0;
            while (i < line.length() && j < 31 && (!(line.at(i) == '/' && line.at(i+1) == '/'))) {
                ItemName[j] = line.at(i);
                i++;
                j++;
            }
            while (ItemName[j] == ' ' || ItemName[j] == '\t') {
                ItemName[j] = 0;
                j--;
            }
            ItemName[j] = 0;

            //cout << "Key " << (DWORD*)ItemKey << " / Text " << ItemName << "\n";
            auto p = new pair<DWORD, char*>(ItemKey, ItemName);
            Menu->push_back(*p);
        }
    }

    static void readConfig() {

        struct stat confEx;
        if (stat("UndercoverExposed.ini", &confEx) == -1) {
            std::ofstream configFile("UndercoverExposed.ini");
            configFile << R"([Fixes]
Verbose = 1                             //includes additional debugging information
CrashFixes = 1                          //fixes various crashes, some specific to Exposed (removes intentional crashes, removes breakpoints, fixes the starter car crash in 1.18, prevents a crash when CreateTexture fails, removes calls to CarLoader::PrintMemoryUsage a seemingly broken function, prevents the crash for models over max polycount, enforces memory pool integrity checks)
IndexBufferSize = 0xA0000               //increases IndexBufferSize, vanilla = 0x50000
CarsPoolSize = 0x8000000                //increases Cars pool size, vanilla = 0x4e20000
FixTruckTrailers = 1                    //fixes tractor-trailer linking broken by Binary 2.8.3
LastGoodGlobalB = 1242129289

[Features: Miscellaneous]
KitRideHeightEnabled = 1                //enables per-kit ride height on some cars
SkipBootupChecks = 1                    //skips some unnecessary bootup checks
EnableAllCameras = 1                    //allows to pick any of the nine vanilla cameras: POV_BUMPER/HOOD, POV_OUTSIDE_CLOSE/FAR/DRIFT/STAGING/PAUSE/SLOTCAR/TUNING
EnableOverlay = 1                       //enables an in-game overlay

[Features: Slider enhancements]
SliderEnhancements = 1                  //fixes Autosculpt sliders speed and adds speed modifiers with Ctrl and Shift
SliderBoostMultiplier = 2.0             //slider speed multiplier when pressing Shift
SliderSlowMultiplier = 0.5              //slider speed multiplier when pressing Control

[Features: Car effects]
HeadlightShatterFix = 1                 //fixes the headlight shatter effect using brakelight break particles (no effect if EffectsEnhancements is enabled)
EffectsEnhancements = 1                 //enables effects for shifting backfire, brakelight nos trail and exhaust smoke
ExhaustSmokeEffect = uce_exhaust        //ecar field or emittergroup node to use as exhaust smoke emitter
NOSEffect = NOSEffect                   //ecar field or emittergroup node to use as NOS exhaust emitter
NOSTrailEffect = uce_nostrail           //ecar field or emittergroup node to use as NOS brakelight trail emitter
BackfireEffect = NOSEffect              //ecar field or emittergroup node to use as gearshift backfire emitter
MissShiftEffect = MissShiftEffect       //ecar field or emittergroup node to use as miss shift backfire emitter

[Features: Day and Night Cycle]
DayNightCycleEnabled = 1                //enables the day and night cycle
DayNightCycleRandomStart = 1            //starts the cycle at a random time instead of its beginning
DayNightCycleDuration = 1440            //total cycle duration in seconds, default is 1440 (24 minutes)
DayNightCycle = [ [sunny6_0,sunny4_22.5,sunny2_45,lessnukedsunny_90,wet_135,wet2_150,wet3_180,wet_180,sunny7_180,sunset_180,sunset3_180,night_4_180,night_2_210,night_3_240,night_7_270,night_1_300,night_6_330,night_5_360]]
//array of weather sets, all weather sets must have the same amount of visual zones for better results
//other example, uncomment to use
//DayNightCycleDuration = 300
//DayNightCycle = ((sunny6_0,sunny4_22.5),(sunny2_45,lessnukedsunny_90),(wet_135,wet2_150),(wet3_180,wet_180),(sunny7_180),(sunset_180,sunset3_180),(night_4_180,night_2_210),(night_3_240,night_7_270),(night_1_300,night_6_330),(night_5_360, night_1_300))

[Features: Headlights]
HeadlightTweaksEnabled = 1              //enables headlight projection and toggling emissive textures with headlights
AutomaticHeadlights = 1                 //toggles headlights on when it's dark and off when not (headlights are always on when set to zero)
AutomaticHeadlightsField = 0x60         //brightness thresold field offset in current visual zone (0x60/64/68/6c = SunColourR/G/B/A, 0x70/74/78/7c = AmbientColourR/G/B/A)
AutomaticHeadlightsThresold = 0.6       //brightness thresold value, each car will use a slightly different value near this
ProjectionCutoff = 1                    //controls whether headlights projection is turned off when it's too bright regardless of headlights state
ProjectionCutoffField = 0x60            //brightness thresold field offset in current visual zone (0x60/64/68/6c = SunColourR/G/B/A, 0x70/74/78/7c = AmbientColourR/G/B/A)
ProjectionCutoffThresold = 0.5          //brightness thresold value

[Features: Aftermarket Menu]
ExtendAftermarketMenu = 1               //include more parts in the Aftermarket menu (experimental)
AftermarketMenuOption = 0x93, Body      //CarPartID, button label (for CarPartID see https://github.com/SpeedReflect/Nikki/blob/master/Nikki/Reflection/Enum/SlotID/SlotUndercover.cs)
AftermarketMenuOption = 0x94, Hood      //IDs between 0x93-0x99 are specific to this menu and have hardcoded behavior
AftermarketMenuOption = 0x95, All Wheels
AftermarketMenuOption = 0x96, Front Wheels
AftermarketMenuOption = 0x97, Rear Wheels
AftermarketMenuOption = 0x98, Spoiler
AftermarketMenuOption = 0x99, Exhaust
AftermarketMenuOption = 0x17, - Extended (experimental) -
AftermarketMenuOption = 0x12, Front Bumper
AftermarketMenuOption = 0x14, Rear Bumper
AftermarketMenuOption = 0x59, Left Side Skirt
AftermarketMenuOption = 0x5A, Right Side Skirt
AftermarketMenuOption = 0x48, Front Left Fender
AftermarketMenuOption = 0x49, Front Right Fender
AftermarketMenuOption = 0x13, Pop-Up Headlights         //BUMPER_FRONT_EXTRA
AftermarketMenuOption = 0x51, Roll Cage
AftermarketMenuOption = 0x55, Left Seat
AftermarketMenuOption = 0x56, Right Seat
AftermarketMenuOption = 0x52, Roof
AftermarketMenuOption = 0x5D, Trunk
)";
            configFile.flush();
            configFile.close();
        }
        std::ifstream configFile("UndercoverExposed.ini");
        std::string line;

        float SliderBoostMultiplier = 2.0f, SliderSlowMultiplier = 0.5f;

        while (std::getline(configFile, line)) {
            try {
                getConfigSetting("Verbose", line, &Settings::verbose);
                getConfigSetting("CrashFixes", line, &CrashFixes);
                getConfigSetting("FixTruckTrailers", line, &Settings::FixTruckTrailers);
                getConfigSetting("LastGoodGlobalB", line, &Settings::lastGoodGlobalB);
                getConfigSetting("GeomAllocBlockSize", line, &Settings::IndexBufferSize);
                getConfigSetting("IndexBufferSize", line, &Settings::IndexBufferSize);
                getConfigSetting("CarsPoolSize", line, &Settings::CarsPoolSize);

                getConfigSetting("EnableKitRideHeight", line, &Settings::KitRideHeight);
                getConfigSetting("KitRideHeightEnabled", line, &Settings::KitRideHeight);
                getConfigSetting("SkipBootupChecks", line, &Settings::SkipBootupChecks);

                getConfigSetting("DayNightCycleEnabled", line, &Settings::useDayNightCycle);
                getConfigSetting("DayNightCycleDuration", line, &Settings::DayNightCycleDuration);
                getConfigSetting("DayNightCycleRandomStart", line, &Settings::DayNightCycleRandomStart);
                getConfigSettingVltHash2DArray("DayNightCycle", line, &Settings::VisualStyleCycles);

                getConfigSetting("HeadlightTweaksEnabled", line, &HeadlightTweaksEnabled);
                getConfigSetting("AutomaticHeadlights", line, &AutomaticHeadlights);
                getConfigSetting("AutomaticHeadlightsField", line, &AutomaticHeadlightsField);
                getConfigSetting("AutomaticHeadlightsThresold", line, &AutomaticHeadlightsThresold);
                getConfigSetting("ProjectionCutoff", line, &ProjectionCutoff);
                getConfigSetting("ProjectionCutoffField", line, &ProjectionCutoffField);
                getConfigSetting("ProjectionCutoffThresold", line, &ProjectionCutoffThresold);

                getConfigSetting("ExtendAftermarketMenu", line, &ExtendAftermarketMenu);
                getConfigSettingMenuItem("AftermarketMenuOption", line, &AftermarketMenu);

                getConfigSetting("SliderEnhancements", line, &SliderEnhancements);
                getConfigSetting("SliderBoostMultiplier", line, &SliderBoostMultiplier);
                getConfigSetting("SliderSlowMultiplier", line, &SliderSlowMultiplier);

                getConfigSetting("HeadlightShatterFix", line, &HeadlightShatterFix);
                getConfigSetting("EffectsEnhancements", line, &EffectsEnhancements);
                getConfigSettingVltHash("ExhaustSmokeEffect", line, &CarRenderConn::ExhaustSmokeEffect);
                getConfigSettingVltHash("NOSEffect", line, &CarRenderConn::NOSEffect);
                getConfigSettingVltHash("NOSTrailEffect", line, &CarRenderConn::NOSTrailEffect);
                getConfigSettingVltHash("BackfireEffect", line, &CarRenderConn::BackfireEffect);
                getConfigSettingVltHash("MissShiftEffect", line, &CarRenderConn::MissShiftEffect);

                getConfigSetting("EnableAllCameras", line, &EnableAllCameras);
                getConfigSetting("EnableOverlay", line, &EnableOverlay);

                getConfigSetting("TESTS", line, &tests);
            }
            catch (exception ex) {
                printf(ex.what());
            }
        }
        configFile.close();

        if (VisualStyleCycles.size() == 0) {
            vector<uint32_t> defaultCycle;
            defaultCycle.push_back(0x13FAB19E); //foggy
            defaultCycle.push_back(0xF7B5B148); //sunny2
            defaultCycle.push_back(0xCEDA4E4F); //sunset
            defaultCycle.push_back(0x2ECF6784); //night_1
            VisualStyleCycles.push_back(defaultCycle);
        }

        if (DayNightCycleRandomStart) {

            std::uniform_int_distribution<> distribOffsetTOD(0, VisualStyleCycles[0].size() - 1);
            int randint = distribOffsetTOD(gen);

            for (int i = 0; i < VisualStyleCycles.size(); i++) {
                vector<uint32_t>* newCycle = new vector<uint32_t>;
                for (int j = 0; j < VisualStyleCycles[i].size(); j++) {
                    newCycle->push_back(VisualStyleCycles[i][j]);
                }

                for (int j = 0; j < VisualStyleCycles[i].size(); j++) {
                    VisualStyleCycles[i][j] = newCycle->at((j + randint) % VisualStyleCycles[i].size());
                }

                delete newCycle;
            }
        }

        if (SliderEnhancements) {
            using namespace Inputs;
            AutosculptSliderSpeedBoost = SliderBoostMultiplier * AutosculptSliderSpeed;
            AutosculptSliderSpeedSlow = SliderSlowMultiplier * AutosculptSliderSpeed;
            OptionsSliderSpeedBoost = SliderBoostMultiplier * OptionsSliderSpeed;
            OptionsSliderSpeedSlow = SliderSlowMultiplier * OptionsSliderSpeed;
            PerfTuningSliderSpeedBoost = SliderBoostMultiplier * PerfTuningSliderSpeed;
            PerfTuningSliderSpeedSlow = SliderSlowMultiplier * PerfTuningSliderSpeed;
            FEVisualPaint1SliderSpeedBoost = SliderBoostMultiplier * FEVisualPaint1SliderSpeed;
            FEVisualPaint1SliderSpeedSlow = SliderSlowMultiplier * FEVisualPaint1SliderSpeed;
            FEVisualPaint2SliderSpeedBoost = SliderBoostMultiplier * FEVisualPaint2SliderSpeed;
            FEVisualPaint2SliderSpeedSlow = SliderSlowMultiplier * FEVisualPaint2SliderSpeed;
        }
    }

    static void updateConfig() {
        std::ifstream configFile("UndercoverExposed.ini");
        std::string line;
        std::ofstream configFileOut("UndercoverExposed.ini.tmp");

        while (std::getline(configFile, line)) {
            if (line.find("LastGoodGlobalB") != -1) {
                configFileOut << "LastGoodGlobalB = " << Settings::lastGoodGlobalB << "\n";
            }
            else {
                configFileOut << line << "\n";
            }
        }
        configFile.close();
        configFileOut.flush();
        configFileOut.close();

        remove("UndercoverExposed.ini");
        rename("UndercoverExposed.ini.tmp", "UndercoverExposed.ini");
    }

    
}

namespace GeomAlloc{
    static int Counter = 0; //used to prevent the oversized triangle allocation memory leak from happening
    static void* loopStart;
    static void* allocateNewBlock;
    static void* calculation;
    static void* forceSkipExit;
    void printMemoryLeakDetected(int requestedSize) {
        std::cout << "Trying to load a model over the polycount limit (" << requestedSize / 6 << " > " << Settings::IndexBufferSize / 6 << ").\nPreventing a memory leak; the affected model will not render.\n";
        if (Settings::verbose) MessageBoxA(NULL, ("Trying to load a model over the polycount limit (" + std::to_string(requestedSize / 6) + " > " + std::to_string(Settings::IndexBufferSize / 6) + ").\nPreventing a memory leak; the affected model will not render.").c_str(), "Undercover Exposed", MB_ICONERROR);
    }
    __declspec(naked) void CounterReset() {
        __asm {
            mov dword ptr[Counter], 0;
            //5 code bytes overridden by the jump
            push ebx;
            push ebp;
            push esi;
            push edi;
            mov esi, ecx;

            jmp loopStart;
        }
    }
    __declspec(naked) void AllocOrSkip() {
        __asm {
            add dword ptr[Counter], 1;
            cmp dword ptr[Counter], 3;
            jg skipAlloc;

            call allocateNewBlock;
            jmp loopStart;

        skipAlloc:
            push edx;
            push ecx;
            push eax;

            push[esp + 32];
            call printMemoryLeakDetected;
            pop ecx;

            pop eax;
            pop ecx;
            pop edx;

            mov ecx, dword ptr[esi + 0x14];
            sub ecx, dword ptr[esi + 0x10];
            mov eax, 0x2aaaaaab;
            imul ecx;
            sar edx, 0x1;
            mov eax, edx;
            shr eax, 0x1f;
            xor ebx, ebx;
            add eax, edx;
            //jz dword ptr[0x00777775];
            mov ebp, dword ptr[esp + 0x14];
            shr ebp, 0x1;
            xor edi, edi;

            mov EAX, dword ptr[ESI + 0x10];
            mov ECX, dword ptr[EAX + EDI * 0x1];
            add EAX, EDI;
            push EBP;
            call calculation;

            //mov eax, [esp - 20]; //actually fixes the 54k poly crash kinda sorta, instead the game will do weird shit

            mov eax, dword ptr[ESP + 0x20 - 0xc]; //different weird shit

            jmp forceSkipExit;//exits the loop
        }
    }
}

namespace DayNight {
    float timePerZone;

    int currentWeatherSet = 0;

    static uint32_t __cdecl getNextVisualStyle(char* toHash) {

        float nextID = ((int)((*timer_world) * 0.00025)) % (Settings::DayNightCycleDuration) / timePerZone;

        if (nextID == 0.0) {
            //kinda broken at the beginning
            //swap cycles
            std::uniform_int_distribution<> distribWeatherSet(0, Settings::VisualStyleCycles.size() - 1);
            currentWeatherSet = distribWeatherSet(gen);
//            std::cout << "New weather set : " << currentWeatherSet << "\n";
            timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[currentWeatherSet].size();
        }

        return Settings::VisualStyleCycles[currentWeatherSet][(int)(nextID)];
    }

    /*void* updateFirstVisualStyleReturn;
    void* updateNextVisualStyleReturn;
    __declspec(naked) void updateNextVisualStyle() { __asm {
        push edx;
        push ecx;
        push eax;

        call getNextVisualStyle;
        mov dword ptr[ESI + 0x4], eax;

        pop eax;
        pop ecx;
        pop edx;

        jmp updateNextVisualStyleReturn;
    }}
    __declspec(naked) void updateFirstVisualStyle() { __asm {
        push edx;
        push ecx;
        push eax;

        call getNextVisualStyle; //keep it the same for now
        mov dword ptr[ESI + 0x4], eax;

        pop eax;
        pop ecx;
        pop edx;

        jmp updateFirstVisualStyleReturn;
    }}

    DWORD dat_012fa474;
    DWORD dat_012fa46c;
    //DWORD dat_00b694d0;
    DWORD float_1;

    __declspec(naked) void findTransitionSpeedMult() { __asm {
        PUSH       ECX;
        CMP        dword ptr[dat_012fa474], 0x0;
        JZ         LAB_008428f3;
        MOV        EAX, [dat_012fa46c];
        MOV        ECX, dword ptr[EAX];
        JMP        LAB_008428f5;

    LAB_008428f3:
        XOR        ECX, ECX;

    LAB_008428f5:
        TEST       ECX, ECX;
        XORPS      XMM1, XMM1;
        MOVAPS     XMM0, XMM1;
        JZ         LAB_00842914;
        MOV        EDX, dword ptr[ECX];
        MOV        EAX, dword ptr[EDX + 0xc8];
        CALL       EAX;
        FSTP       dword ptr[ESP];

    LAB_00842914: 
        fld dword ptr[float_1]; //1.0f static
        POP        ECX;
        RET;
    }}*/
    float* float_1;

    __declspec(naked) void ret1f() {
        __asm {
            fld dword ptr[float_1]; //1.0f static
            RET;
        }
    }

    float* __cdecl getTransitionTime() {
        return &timePerZone;
    }

    //void* transitionPercentageDetourReturn;
    //auto findTransitionPercentage = (void(__cdecl*)()) 0x008354c0;
    //__declspec(naked) void transitionPercentageDetour() { __asm {
    //    mov eax, dword ptr[timePerZone]; //force transition time
    //    mov dword ptr[ecx + 0x14], eax;
    //    call findTransitionPercentage;
    //    jmp transitionPercentageDetourReturn;
    //}}
}

namespace Verbose {
    static const char* fatalerr = "Memory Framework : Fatal error";
    void* __alloca_probe = (void*)0x0087b250;
    __declspec(naked) void DebugPrintReplacement() { __asm {
        MOV        EAX, 0x1000
        CALL       __alloca_probe  //undefined __alloca_probe(void)
        MOV        ECX, dword ptr[ESP + 0x1004]
        LEA        EAX, [ESP + 0x1008]
        PUSH       EAX             //va_list _Args for _vsnprintf
        PUSH       ECX             //char* _Format for _vsnprintf
        LEA        EDX, [ESP + 0x8]
        PUSH       0xfff           //size_t _Count for _vsnprintf
        PUSH       EDX             //char* _Dest for _vsnprintf
        CALL       _vsnprintf
        ADD        ESP, 0x10
        LEA        EAX, [ESP]
        PUSH       EAX
        CALL       OutputDebugStringA

        LEA        EAX, [ESP]
        push 0x00000010L;
        push fatalerr;
        push eax;
        push 0;
        call MessageBoxA;

        ADD        ESP, 0x1000
        RET
    }}
}

namespace KitRideHeight {
    void* RideHeightDetourReturn;
    __declspec(naked) void RideHeightDetour() {
        __asm {

            MOV        EAX, dword ptr[ESI + 0x40]
            TEST       EAX, EAX
            // check for CarRenderInfo existence

            // ESI = this (VehicleRenderConn)
            // EAX = CarRenderInfo

            MOV        ECX, dword ptr[ESI + 0x18]   // ECX = ecar.mLayoutPtr
            XORPS      XMM1, XMM1                   // XMM1 = 0
            MOVSS      XMM2, dword ptr[ECX + 0x40]  // reads ecar MinRideHeight
            MOVSS      dword ptr[ESP + 0x10], XMM1  // sets local var rideHeight to XMM1
            MOVSS      dword ptr[ESP + 0x18], XMM2  // sets local var minRideHeight to XMM2
            jz quit;


            push eax;
            push edx;

            mov ecx, dword ptr[eax + 0x12ec];   // ECX = kitnumber
            push ecx;
            push 0xDC04998F;                    // FXAngularDeviation
            lea ecx, [eax + 0x54a8];            // ecar fields from the other class
            call Attrib::Instance::GetAttributePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

            mov ecx, dword ptr[esi + 0x18]   // ECX = ecarValues
            pop edx;
            test eax, eax; // check that we got something
            jz defaultToNormal; //use vanilla calculation if nothing

            // modified calculation
            movss xmm3, dword ptr[eax];

            pop eax;

            MOVSS      XMM0, dword ptr[ECX + 0x48] //reads ecar MaxRideHeight
            MOV        ECX, dword ptr[EAX + 0x43c]
            SUBSS      XMM0, XMM2;                   // MaxRideHeight - MinRideHeight
            MULSS      XMM0, dword ptr[ECX + 0xe4c]; // (MaxRideHeight - MinRideHeight) * *(ECX+0xe4c)
//            MOVSS      dword ptr[ESP + 0x10], XMM0;  //rideHeight

            addss xmm3, xmm0; // add offset
            movss dword ptr[ESP + 0x10], xmm3;
            jmp RideHeightDetourReturn;


            defaultToNormal:
            pop eax;
            // normal calculation
            MOVSS      XMM0, dword ptr[ECX + 0x48]  // reads ecar MaxRideHeight
            MOV        ECX, dword ptr[EAX + 0x43c]
            SUBSS      XMM0, XMM2                   // MaxRideHeight - MinRideHeight
            MULSS      XMM0, dword ptr[ECX + 0xe4c] // (MaxRideHeight - MinRideHeight) * *(ECX+0xe4c)
            MOVSS      dword ptr[ESP + 0x10], XMM0  //rideHeight

            quit:
            jmp RideHeightDetourReturn;
        }
    }

    void* FERideHeightDetourReturn;
    __declspec(naked) void FERideHeightDetour() {
        __asm {

            MOV        ECX, dword ptr[EAX + 0x54ac] // ecarValues
            MOVSS      XMM1, dword ptr[ECX + 0x44]  // MinFERideHeight
            MOVSS      XMM0, dword ptr[ECX + 0x4c]  // MaxFERideHeight

            push eax;
            push edx;

            mov ecx, dword ptr[eax + 0x12ec];   // ECX = kitnumber
            push ecx;
            push 0xDC04998F;                    // FXAngularDeviation
            lea ecx, [eax + 0x54a8];            // ecar fields from the other class
            call Attrib::Instance::GetAttributePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

            pop edx;

            test eax, eax; // check that we got something
            jz defaultToNormal; //use vanilla calculation if nothing

            // modified calculation
            movss xmm3, dword ptr[eax];
            pop eax;

            MOV        ECX, dword ptr[EAX + 0x43c]
            SUBSS      XMM0, XMM1
            MULSS      XMM0, dword ptr[ECX + 0xe4c]
            addss xmm0, xmm3; // add offset
            PUSH       0x0
            jmp FERideHeightDetourReturn;


        defaultToNormal:
            pop eax;
            MOV        ECX, dword ptr[EAX + 0x43c]
            SUBSS      XMM0, XMM1
            MULSS      XMM0, dword ptr[ECX + 0xe4c]
            PUSH       0x0
            jmp FERideHeightDetourReturn;
        }
    }

    // 1.0 specific functions
    __declspec(naked) void RideHeightDetour_10() {
        __asm {

            MOV        EAX, dword ptr[ESI + 0x40]
            TEST       EAX, EAX
            // check if the other class is empty, if yes cancels ride height calculation

            // ESI = this (VehicleRenderConn)

            MOV        ECX, dword ptr[ESI + 0x18]   // ECX = ecarValues
            XORPS      XMM0, XMM0                   // XMM1 = 0
            MOVSS      XMM1, dword ptr[ECX + 0x40]  // reads ecar MinRideHeight
            MOVSS      dword ptr[ESP + 0x10], XMM0  // sets local var rideHeight to XMM1
            MOVSS      dword ptr[ESP + 0x18], XMM1  // sets local var minRideHeight to XMM2
            jz quit;


            push eax;
            push edx;

            mov ecx, dword ptr[eax + 0x12e0];   // ECX = kitnumber
            push ecx;
            push 0xDC04998F;                    // FXAngularDeviation
            lea ecx, [eax + 0x5498];            // ecar fields from the other class
            call Attrib::Instance::GetAttributePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

            mov ecx, dword ptr[esi + 0x18]   // ECX = ecarValues
                pop edx;
            test eax, eax; // check that we got something
            jz defaultToNormal; //use vanilla calculation if nothing

            // modified calculation
            movss xmm3, dword ptr[eax];

            pop eax;

            MOVSS      XMM2, dword ptr[ECX + 0x48]  // reads ecar MaxRideHeight
            MOV        ECX, dword ptr[EAX + 0x43c]
            CVTPS2PD XMM1, XMM1;
            CVTPS2PD XMM2, XMM2;
            SUBSD      XMM2, XMM1
            MOVSS      XMM1, dword ptr[ECX + 0xad8]
            CVTPS2PD XMM1, XMM1;
            MULSD XMM2, XMM1;
            CVTPD2PS XMM1, XMM2;

            addss xmm3, xmm1; // add offset
            movss dword ptr[ESP + 0x10], xmm3;
            jmp RideHeightDetourReturn;


        defaultToNormal:
            pop eax;
            // normal calculation
            MOVSS      XMM2, dword ptr[ECX + 0x48]  // reads ecar MaxRideHeight
            MOV        ECX, dword ptr[EAX + 0x43c]
            CVTPS2PD XMM1, XMM1;
            CVTPS2PD XMM2, XMM2;
            SUBSD      XMM2, XMM1                  
            MOVSS      XMM1, dword ptr[ECX + 0xad8]
            CVTPS2PD XMM1, XMM1;
            MULSD XMM2, XMM1;
            CVTPD2PS XMM1, XMM2;
            MOVSS      dword ptr[ESP + 0x10], XMM1  //rideHeight

                quit :
            jmp RideHeightDetourReturn;
        }
    }

    // 1.0 FE : i give up this version is ass
    __declspec(naked) void FERideHeightDetour_10() {
        __asm {
            /*
            VANILLA CODE BLOCk

            MOV        ECX,dword ptr [EAX + 0x549c] //ecar values
            MOVSS      XMM0,dword ptr [ECX + 0x44] //MinFERideHeight
            MOVSS      XMM1,dword ptr [ECX + 0x4c] //MaxFERideHeight

            MOV        ECX,dword ptr [EAX + 0x43c]
            MOVSS      dword ptr [ESP + 0x18],XMM0
            CVTPS2PD   XMM0,XMM0
            CVTPS2PD   XMM1,XMM1
            SUBSD      XMM1,XMM0
            MOVSS      XMM0,dword ptr [ECX + 0xad8]
            CVTPS2PD   XMM0,XMM0
            PUSH       0x0
            MULSD      XMM1,XMM0
            CVTPD2PS   XMM0,XMM1            
            
            */
            
            MOV        ECX, dword ptr[EAX + 0x549c] //ecar values
            MOVSS      XMM0, dword ptr[ECX + 0x44] //MinFERideHeight
            MOVSS      XMM1, dword ptr[ECX + 0x4c] //MaxFERideHeight

            push eax;
            push ebx;
            push ecx;
            push edx;

            mov ecx, dword ptr[eax + 0x12e0];   // ECX = kitnumber
            push ecx;
            push 0xDC04998F;                    // FXAngularDeviation
            lea ecx, [eax + 0x5498];            // ecar fields from the other class
            call Attrib::Instance::GetAttributePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

            test eax, eax; // check that we got something
            jz defaultToNormal; //use vanilla calculation if nothing

            movss xmm3, dword ptr[eax];

            pop edx;
            pop ecx;
            pop ebx;
            pop eax;


            MOV        ECX, dword ptr[EAX + 0x43c]
            MOVSS      dword ptr[ESP + 0x18], XMM0
            CVTPS2PD   XMM0, XMM0
            CVTPS2PD   XMM1, XMM1
            SUBSD      XMM1, XMM0
            MOVSS      XMM0, dword ptr[ECX + 0xad8]
            CVTPS2PD   XMM0, XMM0
            PUSH       0x0
            MULSD      XMM1, XMM0
            CVTPD2PS   XMM0, XMM1
            addss xmm0, xmm3; // add offset

            jmp FERideHeightDetourReturn;

        defaultToNormal:

            pop edx;
            pop ecx;
            pop ebx;
            pop eax;


            MOV        ECX, dword ptr[EAX + 0x43c]
            MOVSS      dword ptr[ESP + 0x18], XMM0
            CVTPS2PD   XMM0, XMM0
            CVTPS2PD   XMM1, XMM1
            SUBSD      XMM1, XMM0
            MOVSS      XMM0, dword ptr[ECX + 0xad8]
            CVTPS2PD   XMM0, XMM0
            PUSH       0x0
            MULSD      XMM1, XMM0
            CVTPD2PS   XMM0, XMM1

            jmp FERideHeightDetourReturn;
        }
    }
}

namespace Headlights {
#define bin_HEADLIGHT_LEFT 0xA7E6EA53
#define bin_HEADLIGHT_RIGHT 0xA532FC46

    char** HEADLIGHTS_ptr;

    DWORD CRIoff_MPointsArrayPtr = 0x4C0;

    bool hasXenonHeadlights(DWORD CarRenderInfo) {
        if (*(DWORD*)(CarRenderInfo + 0x4) == 0xAAAAAAAA) {
            //cout << "CarRenderInfo@" << (DWORD*)CarRenderInfo << "\n";
            //cout << "MpointsArrayPtr " << (DWORD*)*(DWORD*)(CarRenderInfo + 0x4C0) << "\n";
            DWORD MPointsArrayPtr = *(DWORD*)(CarRenderInfo + CRIoff_MPointsArrayPtr);
            DWORD CurMPoint = *(DWORD*)MPointsArrayPtr;
            while (*(DWORD*)CurMPoint != MPointsArrayPtr) {
                DWORD MPData = *(DWORD*)(CurMPoint + 0x8);
                DWORD hash = *(DWORD*)(MPData + 0x8);
                //cout << "MPoint @" << CurMPoint << "; hash " << (DWORD*)hash << "\n";
                if (hash == bin_HEADLIGHT_LEFT || hash == bin_HEADLIGHT_RIGHT) {
                    *(DWORD*)(CarRenderInfo + 0x4) = 1;
                    //cout << "is headlight\n";
                    return true;
                }
                CurMPoint = *(DWORD*)CurMPoint;
            }
            *(DWORD*)(CarRenderInfo + 0x4) = 0;
            //cout << "no xenon headlights\n";
        }

        return *(DWORD*)(CarRenderInfo + 0x4) & 1;
    }

    //makeHeadlights
    auto tMatrix__TransformPosition = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x0071b820; //__cdecl indeed
    auto SDR__StartBatch = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x00744290;
    auto SDR__SetTexture = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x00744220;
    auto SDR__AddPoly = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x00755d80;
    auto SDR__EndBatch = (void* (__fastcall*)(DWORD this_))0x00777f80;
    auto WWorldPos__FindClosestFace = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, bVector4 * vec, char noRayCast)) 0x00864ef0;
    auto normalizeVector_005d9950 = (void* (__cdecl*)(bVector4 * param_1, bVector4 * param_2)) 0x005d9950;
    auto WWorldPos__HeightAtPoint = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, bVector4 * vec)) 0x0085e900;

    //lightsUpdateReplacement
    auto eReplacementTextures__constructor = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00521090;
    auto eReplacementTextures__EntryHandle__SetNewNameHash = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00521060;
    auto GetCarTypeInfoFromHash = (char* (__cdecl*)(DWORD param))0x00817940;

    //asm injector
    float* DAT_00b647ec = (float*)0x00b647ec;
    auto DrawAmbientShadow = (void*)0x00825430;
    void* FUN_00520d70 = (void*)0x00520d70;

    void* FUN_00538df0 = (void*)0x00538df0; //1.0 shit


    DWORD CRIoff_mOnLights = 0x5308;
    DWORD CRIoff_mBrokenLights = 0x530c;
    DWORD CRIoff_mCrackedLights = 0x5310;
    DWORD CRIoff_headlightTextures = 0x52c8;
    DWORD CRIoff_headlightGlassTextures = 0x52d4;
    DWORD CRIoff_brakelightTextures = 0x52e0;
    DWORD CRIoff_brakelightGlassTextures = 0x52ec;
    DWORD CRIoff_LightsReplacementTextureTable = 0x8d4;
    DWORD CRIoff_mSelfIlluminationFlags = 0x54c8;
    DWORD CRIoff_terrainAdj = 0x4b8;

    DWORD RideInfoOff_VehType = 0xe44;

    void makeEPoly(float* mb) {
        mb[0] = 0.0f;//pos0 x,y,z,w
        mb[1] = 0.0f;
        mb[2] = 0.0f;
        mb[3] = 0.0f;//not initialized in the game
        mb[4] = 64.0f;//pos1
        mb[5] = 0.0f;
        mb[6] = 0.0f;
        mb[7] = 0.0f;//not initialized
        mb[8] = 64.0f;//pos2
        mb[9] = 64.0f;
        mb[10] = 0.0f;
        mb[0xb] = 0.0f;//not initialized
        mb[0xc] = 0.0f;//pos3
        mb[0xd] = 64.0f;
        mb[0xe] = 0.0f;
        mb[0xf] = 0.0f;//not initialized
        mb[0x10] = 0.0f;//tex0 u,v
        mb[0x11] = 0.0f;
        mb[0x12] = 1.0f;//tex1
        mb[0x13] = 0.0f;
        mb[0x14] = 1.0f;//tex2
        mb[0x15] = 1.0f;
        mb[0x16] = 0.0f;//tex3
        mb[0x17] = 1.0f;
        mb[0x18] = 1.0f;//?
        ((uint32_t*)mb)[0x20] = 0x80808080;//color0
        ((uint32_t*)mb)[0x21] = 0x80808080;//color1
        ((uint32_t*)mb)[0x22] = 0x80808080;//color2
        ((uint32_t*)mb)[0x23] = 0x80808080;//color3
        ((uint32_t*)mb)[0x24] = 0;//?
    }

    static const int headlightsRacer_grid_length = 10;
    static const int headlightsRacer_grid_width = 5;
    static const float headlightsRacer_length = 25.0;
    static const float headlightsRacer_widthMinFactor = 0.8;
    static const float headlightsRacer_widthIncreaseFactor = 0.5;

    static const int taillightsRacer_grid_length = 2;
    static const int taillightsRacer_grid_width = 3;
    static const float taillightsRacer_length = 3.0;
    static const float taillightsRacer_widthMinFactor = 1.7;
    static const float taillightsRacer_widthIncreaseFactor = 0.0;

    static const int headlightsTraffic_grid_length = 5;
    static const int headlightsTraffic_grid_width = 5;
    static const float headlightsTraffic_length = 15.0;
    static const float headlightsTraffic_widthMinFactor = 0.8;
    static const float headlightsTraffic_widthIncreaseFactor = 0.5;

    static const float lights_snapToGroundThresold = -0.1; //-0.05; -0.5; -1
    static const float lights_snapToGroundBaseThresold = -1.5;

    void makeHeadlightsRacer(int CarRenderInfo, float* carPositionMatrix, float* matrix_6) {
        int color = hasXenonHeadlights(CarRenderInfo) ? 0x958680 : 0x809090; //BGR
        int colorB = 0x2020A0;
        float headlightL = 0;
        float headlightR = 0;
        float taillightL = 0;
        float taillightR = 0;
        float brakelightL = 0;
        float brakelightR = 0;
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000001)) headlightL = 1.0; //color = 0x809090; //headlight left
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000002)) headlightR = 1.0; //color = 0x809090; //headlight right
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000008)) taillightL = 1.0; //
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000010)) taillightR = 1.0; //
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000008 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000008)) brakelightL = 1.0; //brake
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000010 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000010)) brakelightR = 1.0; //brake
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000040) color = 0xf0f0f0; //reverse

        //if (headlightL == 0 && headlightR == 0 && brakelightL == 0 && brakelightR == 0) return;
        
        //float* matrix_6 = (float*)*(int*)(CarRenderInfo + 0x4f1c);
        char meshbuffer[0x94]; // Temporary buffer for makeEPoly

        bVector4 bumperTip = { 0, 0, 0, 0 }; //{ *(float*)(CarRenderInfo + 0x380), 0, 0, 0 };
        tMatrix__TransformPosition((float*)&bumperTip, carPositionMatrix, (float*)&bumperTip);
        bVector4 bumperTip_bis = { -bumperTip.y, bumperTip.z, bumperTip.x, 0 };
        WWorldPos__FindClosestFace((CarRenderInfo + 0x3f0), 0, 0, &bumperTip_bis, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
        float terrainHeight = WWorldPos__HeightAtPoint((CarRenderInfo + 0x3f0), 0, &bumperTip_bis); //-y, z, x !!!

        float baseHeight = (terrainHeight - bumperTip.z > lights_snapToGroundBaseThresold) ? terrainHeight - bumperTip.z + 0.05 : 0.05;
        //float baseHeight = *(float*)(CarRenderInfo + 0x378);


        if (headlightR != 0 || headlightL != 0) {
            float position[headlightsRacer_grid_length][headlightsRacer_grid_width * 4];
            float texcoord[headlightsRacer_grid_length][headlightsRacer_grid_width * 2];

            float x = 0;
            float y = -*(float*)(CarRenderInfo + 0x384) * headlightsRacer_widthMinFactor; //boundsMin * factor
            float u = 0;
            // compute vertex positions
            for (int j = 0; j < headlightsRacer_grid_width; j++) {
                float v = 1;

                x = *(float*)(CarRenderInfo + 0x380); //bounds max X

                for (int i = 0; i < headlightsRacer_grid_length; i++) {
                    u = 1.0 - j / (headlightsRacer_grid_width - 1.0); //causes wack ass shit tho
                    position[i][j * 4 + 0] = x;
                    position[i][j * 4 + 1] = y + headlightsRacer_widthIncreaseFactor * x * y; //*(1+ 0.2*x*i)
                    position[i][j * 4 + 2] = baseHeight; //i*i
                    position[i][j * 4 + 3] = 0;
                    texcoord[i][j * 2 + 0] = u;
                    texcoord[i][j * 2 + 1] = v;

                    tMatrix__TransformPosition(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                    x += headlightsRacer_length / (headlightsRacer_grid_length - 1);
                    v -= 1.0 / (headlightsRacer_grid_length - 1);
                }
                y += *(float*)(CarRenderInfo + 0x384) * headlightsRacer_widthMinFactor * 2 / (headlightsRacer_grid_width - 1);
            }

            // project to terrain
            if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
                for (int i = 0; i < headlightsRacer_grid_length; i++) for (int j = 0; j < headlightsRacer_grid_width; j++) {
                    //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                    bVector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                    WWorldPos__FindClosestFace((int)(CarRenderInfo + 0x3f0), 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                    float height = WWorldPos__HeightAtPoint((int)(CarRenderInfo + 0x3f0), 0, &vector_4d0);

                    if (height != 0.0f && height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
                }
            }


            // send to renderer
            makeEPoly((float*)meshbuffer);
            //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
            //printMeshBuffer((float*)meshbuffer);

            SDR__StartBatch(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
            *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
            SDR__SetTexture(*(DWORD*)(*Renderer + 0x40), 0, *HEADLIGHTS_ptr);

            for (int i = 0; i < headlightsRacer_grid_length - 1; i++) {
                for (int j = 0; j < headlightsRacer_grid_width - 1; j++) {
                    //position
                    for (int k = 0; k < 8; k++) {
                        ((float*)meshbuffer)[k] = position[i][j * 4 + k];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 8] = position[i + 1][j * 4 + k + 4];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 12] = position[i + 1][j * 4 + k];
                    }

                    //UV
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 16] = texcoord[i][j * 2 + k];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 20] = texcoord[i + 1][j * 2 + k + 2];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 22] = texcoord[i + 1][j * 2 + k];
                    }

                    ((float*)meshbuffer)[24] = 1.0;

                    //attempt at having depth dependent intensity
                    float mixDmg = (headlightL * (j) / headlightsRacer_grid_width + headlightR * (1.0 - 1.0 * (j) / headlightsRacer_grid_width));
                    float mixDmgp1 = (headlightL * (j + 1) / headlightsRacer_grid_width + headlightR * (1.0 - 1.0 * (j + 1) / headlightsRacer_grid_width));
                    float falloff = (1.0 - (1.0 * (i + 1) / headlightsRacer_grid_length));
                    float falloffp1 = (1.0 - (1.0 * (i + 2) / headlightsRacer_grid_length));

                    ((int*)meshbuffer)[32] = (char)((int)(150.0 * mixDmg * mixDmg * falloff * falloff)) << 0x18 | color; //ABGR
                    ((int*)meshbuffer)[33] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloff * falloff)) << 0x18 | color;
                    ((int*)meshbuffer)[34] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloffp1 * falloffp1)) << 0x18 | color;
                    ((int*)meshbuffer)[35] = (char)((int)(150.0 * mixDmg * mixDmg * falloffp1 * falloffp1)) << 0x18 | color;

                    SDR__AddPoly(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
                }
            }

            SDR__EndBatch(*(DWORD*)(*Renderer + 0x40));
        }

        if (taillightL != 0 || taillightR != 0 || brakelightR != 0 || brakelightL != 0) {
            float position[taillightsRacer_grid_length][taillightsRacer_grid_width * 4];
            float texcoord[taillightsRacer_grid_length][taillightsRacer_grid_width * 2];

            float x = 0;
            float y = -*(float*)(CarRenderInfo + 0x384) * taillightsRacer_widthMinFactor; //boundsMin * factor
            float u = 0;
            // compute vertex positions
            for (int j = 0; j < taillightsRacer_grid_width; j++) {
                float v = 1;

                x = *(float*)(CarRenderInfo + 0x370); //bounds min X

                for (int i = 0; i < taillightsRacer_grid_length; i++) {
                    u = 1.0 - j / (taillightsRacer_grid_width - 1.0); //causes wack ass shit tho
                    position[i][j * 4 + 0] = x;
                    position[i][j * 4 + 1] = y + taillightsRacer_widthIncreaseFactor * x * y; //*(1+ 0.2*x*i)
                    position[i][j * 4 + 2] = baseHeight; //i*i
                    position[i][j * 4 + 3] = 0;
                    texcoord[i][j * 2 + 0] = u;
                    texcoord[i][j * 2 + 1] = v;

                    tMatrix__TransformPosition(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                    x -= taillightsRacer_length / (taillightsRacer_grid_length - 1);
                    v -= 1.0 / (taillightsRacer_grid_length - 1);
                }
                y += *(float*)(CarRenderInfo + 0x384) * taillightsRacer_widthMinFactor * 2 / (taillightsRacer_grid_width - 1);
            }

            // project to terrain
            if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
                for (int i = 0; i < taillightsRacer_grid_length; i++) for (int j = 0; j < taillightsRacer_grid_width; j++) {
                    //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                    bVector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                    WWorldPos__FindClosestFace((int)(CarRenderInfo + 0x3f0), 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                    float height = WWorldPos__HeightAtPoint((int)(CarRenderInfo + 0x3f0), 0, &vector_4d0);

                    if (height != 0.0f && height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
                }
            }


            // send to renderer
            makeEPoly((float*)meshbuffer);
            //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
            //printMeshBuffer((float*)meshbuffer);

            SDR__StartBatch(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
            *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
            SDR__SetTexture(*(DWORD*)(*Renderer + 0x40), 0, *HEADLIGHTS_ptr);

            for (int i = 0; i < taillightsRacer_grid_length - 1; i++) {
                for (int j = 0; j < taillightsRacer_grid_width - 1; j++) {
                    //position
                    for (int k = 0; k < 8; k++) {
                        ((float*)meshbuffer)[k] = position[i][j * 4 + k];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 8] = position[i + 1][j * 4 + k + 4];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 12] = position[i + 1][j * 4 + k];
                    }

                    //UV
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 16] = texcoord[i][j * 2 + k];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 20] = texcoord[i + 1][j * 2 + k + 2];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 22] = texcoord[i + 1][j * 2 + k];
                    }

                    ((float*)meshbuffer)[24] = 1.0;

                    //attempt at having depth dependent intensity
                    float mixDmg = ((brakelightL + taillightL) * (j + 1) / taillightsRacer_grid_width + (brakelightR + taillightR) * (1 - 1.0 * (j + 1) / taillightsRacer_grid_width));
                    float mixDmgp1 = ((brakelightL + taillightL) * (j + 2) / taillightsRacer_grid_width + (brakelightR + taillightR) * (1 - 1.0 * (j + 2) / taillightsRacer_grid_width));
                    float falloff = (1.0 - (1.0 * (i + 1) / taillightsRacer_grid_length));
                    float falloffp1 = (1.0 - (1.0 * (i + 2) / taillightsRacer_grid_length));

                    ((int*)meshbuffer)[32] = (char)((int)(40.0 * mixDmg * mixDmg * falloff * falloff)) << 0x18 | colorB; //ABGR
                    ((int*)meshbuffer)[33] = (char)((int)(40.0 * mixDmgp1 * mixDmgp1 * falloff * falloff)) << 0x18 | colorB;
                    ((int*)meshbuffer)[34] = (char)((int)(40.0 * mixDmgp1 * mixDmgp1 * falloffp1 * falloffp1)) << 0x18 | colorB;
                    ((int*)meshbuffer)[35] = (char)((int)(40.0 * mixDmg * mixDmg * falloffp1 * falloffp1)) << 0x18 | colorB;

                    SDR__AddPoly(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
                }
            }

            SDR__EndBatch(*(DWORD*)(*Renderer + 0x40));
        }
    }

    void makeHeadlightsTraffic(int CarRenderInfo, float* carPositionMatrix, float* matrix_6) {
        int color = 0x809090; //BGR
        float alphaL = 0;
        float alphaR = 0;
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000001)) alphaL = 1.0; //color = 0x809090; //headlight left
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000002)) alphaR = 1.0; //color = 0x809090; //headlight right
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000008) color = 0x202090; //brake
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000040) color = 0xf0f0f0; //reverse

        if (alphaR == 0 && alphaL == 0) return;

        //float* matrix_6 = (float*)*(int*)(CarRenderInfo + 0x4f1c);

        float position[headlightsTraffic_grid_length][headlightsTraffic_grid_width * 4];
        float texcoord[headlightsTraffic_grid_length][headlightsTraffic_grid_width * 2];
        char meshbuffer[0x94]; // Temporary buffer for makeEPoly

        bVector4 bumperTip = { 0, 0, 0, 0 }; //{ *(float*)(CarRenderInfo + 0x380), 0, 0, 0 };
        tMatrix__TransformPosition((float*)&bumperTip, carPositionMatrix, (float*)&bumperTip);
        bVector4 bumperTip_bis = { -bumperTip.y, bumperTip.z, bumperTip.x, 0 };
        WWorldPos__FindClosestFace(CarRenderInfo + 0x3f0, 0, 0, &bumperTip_bis, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
        float terrainHeight = WWorldPos__HeightAtPoint(CarRenderInfo + 0x3f0, 0, &bumperTip_bis); //-y, z, x !!!

        float baseHeight = (terrainHeight - bumperTip.z > lights_snapToGroundBaseThresold) ? terrainHeight - bumperTip.z + 0.05 : 0.05;
        //float baseHeight = *(float*)(CarRenderInfo + 0x378);

        float x = 0;
        float y = -*(float*)(CarRenderInfo + 0x384) * headlightsTraffic_widthMinFactor; //boundsMin * factor
        float u = 0;

        // compute vertex positions
        for (int j = 0; j < headlightsTraffic_grid_width; j++) {
            float v = 1;

            x = *(float*)(CarRenderInfo + 0x380); //bounds max X

            for (int i = 0; i < headlightsTraffic_grid_length; i++) {
                u = 1.0 - j / (headlightsTraffic_grid_width - 1.0); //causes wack ass shit tho
                position[i][j * 4 + 0] = x;
                position[i][j * 4 + 1] = y + headlightsTraffic_widthIncreaseFactor * x * y; //*(1+ 0.2*x*i)
                position[i][j * 4 + 2] = baseHeight; //i*i
                position[i][j * 4 + 3] = 0;
                texcoord[i][j * 2 + 0] = u;
                texcoord[i][j * 2 + 1] = v;

                tMatrix__TransformPosition(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                x += headlightsTraffic_length / (headlightsTraffic_grid_length - 1);
                v -= 1.0 / (headlightsTraffic_grid_length - 1);
            }
            y += *(float*)(CarRenderInfo + 0x384) * headlightsTraffic_widthMinFactor * 2 / (headlightsTraffic_grid_width - 1);
        }

        // project to terrain
        if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
            for (int i = 0; i < headlightsTraffic_grid_length; i++) for (int j = 0; j < headlightsTraffic_grid_width; j++) {
                //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                bVector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                WWorldPos__FindClosestFace(CarRenderInfo + 0x3f0, 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                float height = WWorldPos__HeightAtPoint(CarRenderInfo + 0x3f0, 0, &vector_4d0);

                if (height != 0.0f && height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
            }
        }

        // send to renderer
        makeEPoly((float*)meshbuffer);
        //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
        //printMeshBuffer((float*)meshbuffer);

        SDR__StartBatch(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
        *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
        SDR__SetTexture(*(DWORD*)(*Renderer + 0x40), 0, *HEADLIGHTS_ptr);

        for (int i = 0; i < headlightsTraffic_grid_length - 1; i++) {
            for (int j = 0; j < headlightsTraffic_grid_width - 1; j++) {
                //position
                for (int k = 0; k < 8; k++) {
                    ((float*)meshbuffer)[k] = position[i][j * 4 + k];
                }
                for (int k = 0; k < 4; k++) {
                    ((float*)meshbuffer)[k + 8] = position[i + 1][j * 4 + k + 4];
                }
                for (int k = 0; k < 4; k++) {
                    ((float*)meshbuffer)[k + 12] = position[i + 1][j * 4 + k];
                }

                //UV
                for (int k = 0; k < 4; k++) {
                    ((float*)meshbuffer)[k + 16] = texcoord[i][j * 2 + k];
                }
                for (int k = 0; k < 2; k++) {
                    ((float*)meshbuffer)[k + 20] = texcoord[i + 1][j * 2 + k + 2];
                }
                for (int k = 0; k < 2; k++) {
                    ((float*)meshbuffer)[k + 22] = texcoord[i + 1][j * 2 + k];
                }

                ((float*)meshbuffer)[24] = 1.0;

                //attempt at having depth dependent intensity

                float mixDmg = (alphaL * (j) / headlightsTraffic_grid_width + alphaR * (1.0 - 1.0 * (j) / headlightsTraffic_grid_width));
                float mixDmgp1 = (alphaL * (j + 1) / headlightsTraffic_grid_width + alphaR * (1.0 - 1.0 * (j + 1) / headlightsTraffic_grid_width));
                float falloff = (1.0 - (1.0 * (i + 1) / headlightsTraffic_grid_length));
                float falloffp1 = (1.0 - (1.0 * (i + 2) / headlightsTraffic_grid_length));

                ((int*)meshbuffer)[32] = (char)((int)(150.0 * mixDmg * mixDmg * falloff * falloff)) << 0x18 | color; //ABGR
                ((int*)meshbuffer)[33] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloff * falloff)) << 0x18 | color;
                ((int*)meshbuffer)[34] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloffp1 * falloffp1)) << 0x18 | color;
                ((int*)meshbuffer)[35] = (char)((int)(150.0 * mixDmg * mixDmg * falloffp1 * falloffp1)) << 0x18 | color;

                SDR__AddPoly(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
            }
        }
        SDR__EndBatch(*(DWORD*)(*Renderer + 0x40));

    }

    float __stdcall ReplaceCarShadow(DWORD CarRenderInfo, bVector4* param_3, float* carPositionMatrix, float* matrix_5, float* mat_6, int param_7) {
        if (*GameFlow != 6) return 1.0f;
        if (Settings::ProjectionCutoff && (*((float*)((int)theSunAndSky + Settings::ProjectionCutoffField)) > Settings::ProjectionCutoffThresold)) return 1.0f;
        if (*HEADLIGHTS_ptr == 0) return 1.0f;

        //field CarRenderInfo + 43c has other data -> towards pvehicle ?
        switch (*(int*)(*(int*)(CarRenderInfo + 0x43c) + RideInfoOff_VehType)) { //0 player 1 ? 2 racer 3 cop 4 traffic 5 heli 6 ? 7 ?
        case 0: //player
        case 2: //racer
            //cout << "CarRenderInfo@" << (DWORD*)CarRenderInfo << "\n";
            //cout << "Lights combination (CRI + 0x5308) @ " << ((int*)(CarRenderInfo + 0x5308)) << " = " << (int*)*((int*)(CarRenderInfo + 0x5308)) << "\n";
            //cout << "Class???@" << (int*)*((int*)(CarRenderInfo + 0x43c)) << " (CRI + 43c) \n";
            //cout << "Class??? +1D00 " << (int*)*(int*)(*((int*)(CarRenderInfo + 0x43c)) + 0x1D00) << " \n";

            makeHeadlightsRacer(CarRenderInfo, carPositionMatrix, mat_6);
            break;
        case 3: //cop
        case 4: //traffic
            // avoid drawing headlights in front of truck trailers
            if (*(float*)(CarRenderInfo + 0x370) < -6.0) return 1.0f;

            makeHeadlightsTraffic(CarRenderInfo, carPositionMatrix, mat_6);
            break;
        }
        return 1.0f;
    }

    void* replaceCarshadowDetourReturn;
    __declspec(naked) void replaceCarshadowDetour() {
        __asm {

            //comparison stuff
            MOVSS      XMM0, dword ptr[ESI + 0x5840]
            COMISS     XMM0, zero
            JBE        LAB_008494fd

            MOV        ECX, EBX
            CALL       FUN_00520d70
            TEST       AL, AL
            JNZ        LAB_008494fd

            CMP        dword ptr[EBX + 0x4], 0x1c
            JZ         LAB_008494fd

            //\/ one of these checks that the car is not a heli
            MOV        EDX, dword ptr[ESI + 0x43c]
            MOV        EAX, dword ptr[EDX + 0xe44]
            CMP        EAX, 0x7
            JZ         LAB_008494fd

            CMP        EAX, 0x5
            JZ         LAB_008494fd
            //^ one of these checks that the car is not a heli

            CMP        dword ptr[EBP + 0x34], 0x0   //param_12
            JNZ        LAB_008494fd




            MOV        EAX, dword ptr[EBP + 0x24]   //param_8
            FLD        dword ptr[EBP + 0x38]          //param_13      [EBP+38] onto the FPU stack ST(0)
            MOV        ECX, dword ptr[ESI + 0x4f1c]
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]    // = > local_1d0
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]    // = > matrix_210
            PUSH       EAX
            PUSH       ECX
            FSTP       dword ptr[ESP]       // = > local_2a4        stores ST(0) to [ESP] ???
            LEA        ECX, [ESP + 0x64]    // = > vector4_240
            PUSH       ECX
            PUSH       EBX
            MOV        ECX, ESI
            CALL       DrawAmbientShadow
            FSTP       ST(0)


            //injected function
            //inject after carshadow for better render sort
            MOV        EAX, dword ptr[EBP + 0x24]   //param_8
            //FLD        dword ptr[EBP + 0x38]          //param_13      [EBP+38] onto the FPU stack ST(0)
            MOV        ECX, dword ptr[ESI + 0x4f1c]
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]    // = > local_1d0
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]    // = > matrix_210
            PUSH       EAX
            PUSH       ECX
            //FSTP       dword ptr[ESP]       // = > local_2a4        stores ST(0) to [ESP] ???
            //LEA        ECX, [ESP + 0x64]    // = > vector4_240
            //PUSH       ECX
            //PUSH       EBX
            PUSH       ESI                  // replaced from MOV ECX, ESI -> thiscall to stdcall
            CALL       ReplaceCarShadow          //float10 * 1_drawCarShadow_008254
            FSTP       ST(0)




        LAB_008494fd:
            jmp replaceCarshadowDetourReturn;

        }
    }
    __declspec(naked) void replaceCarshadowDetour_10() {
        __asm {

            MOV        ECX, EBX
            CALL       FUN_00538df0
            TEST       AL, AL
            JNZ        LAB_00878b19

            MOV        ECX, EBX
            CALL       FUN_00520d70 //FUN_00538e10
            TEST       AL, AL
            JNZ        LAB_00878b19

            MOV        EDX, dword ptr[ESI + 0x43c]
            MOV        EAX, dword ptr[EDX + 0xad0]
            CMP        EAX, 0x7
            JZ         LAB_00878b19

            CMP        EAX, 0x5
            JZ         LAB_00878b19

            CMP        dword ptr[EBP + 0x34], 0x0
            JNZ        LAB_00878b19

            MOV        EAX, dword ptr[EBP + 0x24]
            FLD        dword ptr[EBP + 0x38]
            MOV        ECX, dword ptr[ESI + 0x4f0c]
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]
            PUSH       EAX
            PUSH       ECX
            FSTP       dword ptr[ESP]
            LEA        ECX, [ESP + 0x64]
            PUSH       ECX
            PUSH       EBX
            MOV        ECX, ESI
            CALL       DrawAmbientShadow
            FSTP       ST(0)


            //injected function
            //inject after carshadow for better render sort
            MOV        EAX, dword ptr[EBP + 0x24]   //param_8
            //FLD        dword ptr[EBP + 0x38]          //param_13      [EBP+38] onto the FPU stack ST(0)
            MOV        ECX, dword ptr[ESI + 0x4f0c] //4f1c in 1.18
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]    // = > local_1d0
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]    // = > matrix_210
            PUSH       EAX
            PUSH       ECX
            //FSTP       dword ptr[ESP]       // = > local_2a4        stores ST(0) to [ESP] ???
            //LEA        ECX, [ESP + 0x64]    // = > vector4_240
            //PUSH       ECX
            //PUSH       EBX
            PUSH       ESI                  // replaced from MOV ECX, ESI -> thiscall to stdcall
            CALL       ReplaceCarShadow          //float10 * 1_drawCarShadow_008254
            FSTP       ST(0)




        LAB_00878b19:
            jmp replaceCarshadowDetourReturn;

        }
    }

    __declspec(naked) void getRawFlaresDetour() {
        __asm {

            mov ecx, dword ptr[esi + 0x84]; //PVehicle
            mov eax, dword ptr[ecx + 0x9c]; //light flares
            mov dword ptr[edi + 0xe0], eax;

            pop edi;
            pop esi;
            add esp, 0x1c;
            ret 0x4;
        }
    }
    __declspec(naked) void getRawFlaresDetour_10() {
        __asm {

            mov ecx, dword ptr[esi + 0x84]; //PVehicle
            mov eax, dword ptr[ecx + 0x9c]; //light flares
            mov dword ptr[edi + 0xe0], eax;

            pop edi;
            pop esi;
            add esp, 0x20;
            ret 0x4;
        }
    }

    void* setEmissiveLightsDetourReturn;
    __declspec(naked) void setEmissiveLightsDetour() {
        __asm {
            MOV     EAX, dword ptr[EBP + 0x18];

            TEST    dword ptr[EAX + 0x40], 0x40000000; // material flag for HEADLIGHTREFLECTOR
            JZ      notHeadlightReflector;

            TEST    EBX, 0x100000; //areHeadlightsOn (new)
            JNZ     lightOn;
            JMP     setEmissiveLightsDetourReturn;


        notHeadlightReflector:
            TEST    EBX, 0x200000; //areBrakelightsOn
            JNZ     lightOn;
            JMP     setEmissiveLightsDetourReturn;


        lightOn:                               // XREF[1]:     007612ed(j)
            MOVSS   dword ptr[ESI + 0xc], XMM1; // = > DAT_3f800000, emissive light on

            JMP     setEmissiveLightsDetourReturn;
        }
    }

    void __fastcall lightsUpdateReplacement(DWORD CarRenderInfo) {

        void* pvVar3;
        int texIDBrakeL;
        int texIDBrakeR;
        int texIDBGlassL;
        int texIDBGlassR;
        int texIDBrakeC;
        int buffer[2];

        DWORD* electricsState = (DWORD*)(CarRenderInfo + CRIoff_mOnLights);
        DWORD* lightsDamage = (DWORD*)(CarRenderInfo + CRIoff_mBrokenLights);
        DWORD* lightsGlassDamage = (DWORD*)(CarRenderInfo + CRIoff_mCrackedLights);
        DWORD* headlightTextures = (DWORD*)(CarRenderInfo + CRIoff_headlightTextures); //within UsedCarTextureInfo
        DWORD* headlightGlassTextures = (DWORD*)(CarRenderInfo + CRIoff_headlightGlassTextures);
        DWORD* brakelightTextures = (DWORD*)(CarRenderInfo + CRIoff_brakelightTextures);
        DWORD* brakelightGlassTextures = (DWORD*)(CarRenderInfo + CRIoff_brakelightGlassTextures);
        void* this_00 = (void*)(CarRenderInfo + CRIoff_LightsReplacementTextureTable);

        //get headlight texture index
        //can't be switched to zero on vanilla cars because they lack headlight off textures
        int texIDHeadL = 1;
        int texIDHeadR = 1;
        int texIDHGlassL = 1;
        int texIDHGlassR = 1;

        if ((*lightsGlassDamage & 0x00000001)) texIDHGlassL = 2;
        if ((*lightsDamage & 0x00000001)) { texIDHeadL = 2; texIDHGlassL = 2; }
        if ((*lightsGlassDamage & 0x00000002)) texIDHGlassR = 2;
        if ((*lightsDamage & 0x00000002)) { texIDHeadR = 2; texIDHGlassR = 2; }

        //removed unnecessary bit for headlights

        //get headlights textures
        int texHeadL = headlightTextures[texIDHeadL];
        int texHeadR = headlightTextures[texIDHeadR];
        int texHGlassL = headlightGlassTextures[texIDHGlassL];
        int texHGlassR = headlightGlassTextures[texIDHGlassR];

        //toggle headlight damage textures
        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 1);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHeadL); //headlights 

        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 2);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHeadR); //headlights (only one necessary)

        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 3);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHGlassL); //headlight glass

        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 4);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHGlassR); //headlight glass (only one necessary)

        /* checks lights damage */
        texIDBrakeL = 0;
        texIDBrakeR = 0;
        texIDBrakeC = 0;
        texIDBGlassL = 0;
        texIDBGlassR = 0;

        if (!(*lightsDamage & 0x00000008)) { //left brakelight
            if ((*electricsState & 0x00000008)) {
                texIDBrakeL = 1;
                texIDBGlassL = 1;
                if ((*lightsGlassDamage & 0x00000008)) {
                    texIDBGlassL = 2;
                }
            }
        }
        else {
            texIDBrakeL = 2;
            texIDBGlassL = 2;
        }

        if (!(*lightsDamage & 0x00000010)) { //right brakelight
            if ((*electricsState & 0x00000010)) {
                texIDBrakeR = 1;
                texIDBGlassR = 1;
                if ((*lightsGlassDamage & 0x00000010)) {
                    texIDBGlassR = 2;
                }
            }
        }
        else {
            texIDBrakeR = 2;
            texIDBGlassR = 2;
        }

        if (!(*lightsDamage & 0x00000020)) { //center brakelight
            if ((*electricsState & 0x00000020)) {
                texIDBrakeC = 1;
            }
        }
        else {
            texIDBrakeC = 2;
        }

        //removed unnecessary bit for brakelights

        if (*(int*)(GetCarTypeInfoFromHash(**(DWORD**)(CarRenderInfo + 0x43c)) + 0xf4) != 2) {

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 5);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[(texIDBrakeL == 2) ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 6);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[(texIDBrakeR == 2) ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 7);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[(texIDBrakeC == 2) ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 8);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassL == 2 ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 9);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassR == 2 ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 10);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBrakeC == 2 ? 2 : 0]);


            //manage headlights
            if ((*electricsState & 0x00000003)) { //&& !(lightsDamage & 0x00000003)
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) |= 0x100000;
            }
            else {
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) &= ~0x100000;
            }
            //if (((texIDHeadL != 2) && (texIDHeadR != 2)) && ((texIDHGlassL != 0 && (texIDHGlassR != 0))) && (*electricsState & 0x00000003)) { //shuts off the whole car when headlights broken, not the wanted effect !
            //    *(int*)(CarRenderInfo + 0x54c8) |= 0x100000;
            //}
            //else {
            //    *(int*)(CarRenderInfo + 0x54c8) &= ~0x100000;
            //}

            //manage brakelights
            if (((texIDBrakeL != 2) && (texIDBrakeR != 2)) && ((texIDBGlassL != 0 && (texIDBGlassR != 0)))) {
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) |= 0x200000;
            }
            else {
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) &= ~0x200000;
            }
            return;
        }

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 5);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeL]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 6);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeR]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 7);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeC]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 8);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassL]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 9);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassR]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 10);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeC]);

        return;
    }

    void __fastcall headlightsReplacement(DWORD this_, DWORD unused, DWORD arg1) {

        DWORD* PVehicle = (DWORD*)(this_ + 0xb8);

        if (*(DWORD*)(this_ + 0x860) == 9999) {
            *(char*)(this_ + 0x863) = 1; //auto mode
            PVehicle[0x27] = PVehicle[0x27] | 0x00000007;

            std::uniform_int_distribution<> headlightSensitivity(-INT16_MIN, INT16_MAX);
            *(int16_t*)(this_ + 0x860) = headlightSensitivity(gen);
        }

        if (*((float*)((int)theSunAndSky + Settings::AutomaticHeadlightsField)) > (Settings::AutomaticHeadlightsThresold * (1.0 + *(int16_t*)(this_ + 0x860) / 327670.0))) {
            //auto turn off
            if (*(char*)(this_ + 0x863) == 1) {
                PVehicle[0x27] = PVehicle[0x27] & ~0x00000007;
                *(char*)(this_ + 0x863) = 0;
            }
            //headlights forced on, do not turn off, set them back to auto if the player manually turns them off
            if (*(char*)(this_ + 0x863) == 2) {
                *(char*)(this_ + 0x863) = 0;
            }
        }
        else {
            if (*(char*)(this_ + 0x863) == 0) {
                //headlights already on, save that they're forced
                if (PVehicle[0x27] & 0x00000007) {
                    *(char*)(this_ + 0x863) = 2;
                }
                //auto turn on
                else {
                    PVehicle[0x27] = PVehicle[0x27] | 0x00000007;
                    *(char*)(this_ + 0x863) = 1;
                }
            }
        }
    }


}

namespace CrashFixes {
    bool doFirstCarCrashFix = false;

    // FROM EXTRA OPTIONS
    DWORD _PlayerIPlayer = 0x012faa80;
    DWORD _PlayerPVehicle = 0x012fa478;
    int* NumCurrentIVehicleElements = (int*)0x012fa474;
    static DWORD GetPlayerPVehicle()
    {
        DWORD* ISimableFromIPlayer, * ISimableFromIVehicle, * PlayerPVehicle;

        DWORD* PlayerIPlayer = *(DWORD**)_PlayerIPlayer; // Player 1

        for (int i = 0; i < *NumCurrentIVehicleElements; i++)
        {
            PlayerPVehicle = *(DWORD**)(_PlayerPVehicle + i * 4);

            if (PlayerIPlayer) ISimableFromIPlayer = (*(DWORD * (__thiscall**)(DWORD*))(PlayerIPlayer[0] + 0x4))(PlayerIPlayer); // RecordablePlayer::GetSimable
            else ISimableFromIPlayer = 0;
            if (PlayerPVehicle) ISimableFromIVehicle = (*(DWORD * (__thiscall**)(DWORD*))(PlayerPVehicle[0] + 0x4))(PlayerPVehicle); // PVehicle::GetSimable
            else ISimableFromIVehicle = 0;

            if (ISimableFromIPlayer && ISimableFromIVehicle && (ISimableFromIPlayer == ISimableFromIVehicle)) {
                //lastPlayerPVehicle = PlayerPVehicle;
                return (DWORD)PlayerPVehicle;
            }
        }

        return 0;
    }

    char* DebugSwitchCar = (char*)0x01862c60;
    auto forcePlayerCar = (void(__fastcall*)(DWORD* DebugVehicleSelection, DWORD something, char* carname, DWORD, DWORD, DWORD))0x00858ce0;
    char* forceSpecificCar = 0;
    char _forceSpecificCar[32];

    static void __fastcall forcePlayerCarDetour(DWORD* DebugVehicleSel, DWORD something, char* carname, DWORD, DWORD, DWORD) {
        if (forceSpecificCar != 0) {
            forcePlayerCar(DebugVehicleSel, something, forceSpecificCar, 0, 0, 0);
            forceSpecificCar = 0;
        }
        else {
            //cout << "Using Debug Car " << carname << "\n";
            if (!strcmp(carname, "por_cay_s_09")) carname = (char*)"por_cay_s_07"; //crash fix for 1.18
            if (!strcmp(carname, "lam_rev_stk_08")) carname = (char*)"lam_mur_lp_06";

            forcePlayerCar(DebugVehicleSel, something, carname, 0, 0, 0);
        }
    }

    static void __cdecl HUDLoadedDetour() {
        if (doFirstCarCrashFix) {
            doFirstCarCrashFix = false;

            //carname 
            //find player pvehicle
            //class *(pvehicle+1c)
            //*(class+4) is uppercase xname
            //lowercase it and replace car

            DWORD PlayerPVehicle = GetPlayerPVehicle();

            _memccpy(_forceSpecificCar, *(char**)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4), 0, 31);

            for (int i = 0; i < 32; i++) {
                _forceSpecificCar[i] = std::tolower(_forceSpecificCar[i]);
            }

            cout << "Switching player car to prevent progression loss for first car crash, forcing " << _forceSpecificCar << "\n";

            *DebugSwitchCar = 1;
            forceSpecificCar = _forceSpecificCar;

        }

    }

    auto CarRenderInfoInitialize = (void(__fastcall*)(DWORD CarRenderInfo, DWORD edx, DWORD))0x0084ac40;
    static void __fastcall CarRenderInfoInitializeDetour(DWORD CarRenderInfo, DWORD edx, DWORD param) {
//        cout << "Forcing GameFlow 6 to prevent first car crash\n";
        *GameFlow = 6;
        utils::call(0x0084d19e, CarRenderInfoInitialize);
        CarRenderInfoInitialize(CarRenderInfo, edx, param);
    }

    static DWORD __cdecl hashBINImpersonator(char* string) {
        DWORD result = getBinHash(string);
        if (result == 0x16E8EF0D) { //hash for "PINK_SLIP_CAR_1"
            doFirstCarCrashFix = true;
//            cout << "Preparing to prevent first car crash\n";
            utils::call(0x0084d19e, CrashFixes::CarRenderInfoInitializeDetour);
        }
        return result;
    }



    auto CreateTextureDetourReturn = 0x007514ba;
    __declspec(naked) void CreateTextureCrashFix() {
        __asm {
            cmp eax, 0; //HRESULT should return 0
            jne notok;

            xor esi, esi;
            cmp dword ptr[ebp + 0xc], esi;

            jmp CreateTextureDetourReturn;

        notok:
            xor esi, esi;
            cmp esi, 0; //force skip what comes next
            jmp CreateTextureDetourReturn;
        }
    }


    auto GetAmountFree = (DWORD(__fastcall*)(DWORD MemoryPool)) 0x004ac600;
    auto AddFreeMemory = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, DWORD FreeBlock, DWORD, DWORD)) 0x0;
    auto VerifyPoolIntegrity = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, bool)) 0x004ade80;

    DWORD __fastcall GetAmountFree_wrapper(DWORD this_) {
        VerifyPoolIntegrity(this_, 0, true);
        return GetAmountFree(this_);
    }
    void __fastcall AddFreeMemory_wrapper(DWORD this_, DWORD edx, DWORD FreeBlock, DWORD param2, DWORD param3) {
        VerifyPoolIntegrity(this_, 0, true);
        return AddFreeMemory(this_, edx, FreeBlock, param2, param3);
    }
}

namespace FEAftermarketMenu {

    //auto FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
    //auto FE_getFeElement = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
    //auto AddToTree = (void* (__fastcall*)(DWORD this_, DWORD unaff, LinkedNode*, DWORD)) 0x005563c0;
    //auto FUN_00553aa0 = (void(__fastcall*)(LinkedNode * this_)) 0x00553aa0;
    //auto FUN_getLinkedListLength_004a76f0 = (DWORD(__fastcall*)(LinkedNode * this_, DWORD unaff, DWORD)) 0x004a76f0;

    auto StandardSelectablePart__GetPartsList_005d1b00 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD)) 0x005d1b00;
    auto AutosculptSelectablePart__GetPartsList_005d40e0 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)) 0x005d40e0;
    
    //auto vltGet_car_004de2a0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD carNameHash, DWORD)) 0x004de2a0;
    //auto lowercaseVltHash_0045d950 = (void(__cdecl*)(char* toHash)) 0x0045d950;
    //auto FUN_getCollectionFromPointer_005cd3c0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*, DWORD)) 0x005cd3c0;


    void __cdecl LoadAftermarketMenu() {
        LinkedNode local_14;
        LinkedNode* current;
        FETreeDataNode* curTreeNode;

        //TODO car-specific settings

        //if (CurrentFECarPTR_012ac35c != 0) {

            //DWORD car_PTR[4];
            //DWORD car_parts_PTR[4];
            //DWORD car_part_array[4];


            //auto playerCar = ((SavegameCar*)(*(DWORD*)UserProfile_Player1 + 0x88))[*(DWORD*)(FECarHeaven_012ac318 + 0x10)];


            //cout << "current pvehicle " << (DWORD*)playerCar.pvehicle << "\n";
            //vltGet_car_004de2a0(car_PTR, 0, playerCar.pvehicle, 0);

            //cout << "car@" << (DWORD*)car_PTR[1] << "\n";
            //FUN_getCollectionFromPointer_005cd3c0(car_parts_PTR, 0, (DWORD*)(car_PTR[1] + 0xc), 0);

            //cout << "car_parts@" << (DWORD*)car_parts_PTR[1] << "\n";
            //vlt_loadArray_004609a0(car_parts_PTR, 0, car_part_array, 0x4A1C84F4); //wheel_spoke_rear_paint_swatch

            //cout << "Array wheel_spoke_... : " << car_part_array[0] <<" ; " << car_part_array[1] <<" ; size=" << car_part_array[2] << "\n";
        //        for (int i = 0; i < car_part_array[2]; i++) cout << "[" << i << "] = " << (DWORD*)vlt_getArrayElement_0045eaa0(car_part_array, 0, i) <<"\n";
            //}

        local_14.next = &local_14;
        local_14.previous = &local_14;

        for (int i = 0; i < Settings::AftermarketMenu.size(); i++) {
            curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
            if (curTreeNode != NULL) {
                curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, Settings::AftermarketMenu.at(i).first, Settings::AftermarketMenu.at(i).second, 0xffffffff, 0xffffffff);
                current = &curTreeNode->nodeLinking_0x4;
                (local_14.next)->previous = current;
                current->next = local_14.next;
                current->previous = &local_14;
                local_14.next = current;
            }
        }

        AddToTree(FE_getFeElement(*FEScreenManager, 0, 0x28), 0, &local_14, 0);
        FUN_00553aa0(&local_14);

    }

    DWORD __fastcall getSelectedPartID(DWORD this_) {
        switch (*(DWORD*)(this_ + 0x58)) {
            //original menu
        case 0x93:
            return 8;
        case 0x94:
            return 0x4e;
        case 0x95:
        case 0x96:
            //wheel front and all wheels
            return 0x5f;
        case 0x97:
            return 0x60;
        case 0x98:
            return 0x5b;
        case 0x99:
            return 0x44;
        }
        return *(DWORD*)(this_ + 0x58);
    }

    DWORD __fastcall getPartsList(DWORD this_) {

        DWORD PartID = getSelectedPartID(this_);
        DWORD PartID2 = 0x73; //only works fine for wheels
        bool singlePart = true;

        LinkedNode* this_00 = (LinkedNode*)(this_ + 0x50);

        FUN_00553aa0(this_00);
        *(DWORD*)(this_ + 0x5c) = 0;

        if (PartID >= 0x100) {
            singlePart = false;
            PartID2 = (PartID >> 8) & 0xFF;
            PartID = PartID & 0xFF;
        }


        if (*(DWORD*)(this_ + 0x58) == 0x99) { //exhaust
            StandardSelectablePart__GetPartsList_005d1b00(PartID, (int)this_00, 0, 0);
        }
        else {
            if (*(DWORD*)(this_ + 0x58) == 0x95) {
                PartID2 = 0x60; //all wheels
                singlePart = false;
            }
            AutosculptSelectablePart__GetPartsList_005d40e0(PartID, (int)this_00, 0, 0, singlePart, PartID2);
        }

        return FUN_getLinkedListLength_004a76f0(this_00, 0, 0x0);
    }


    DWORD __cdecl getAutosculptZone(CarPartID id) {
        switch (id) {
        case BUMPER_FRONT:
            return RegionFrontBumper; //0 
        case BUMPER_REAR:
            return RegionRearBumper; //1
        case SKIRT_LEFT:
        case SKIRT_RIGHT:
            return RegionSkirtLeft; //2
        case WHEEL_FRONT:
            return RegionRimsFront; //4
        case WHEEL_REAR:
            return RegionRimsRear; //5
        case DOOR_LEFT:
        case DOOR_RIGHT:
            return RegionDoorLeft;
        case DOOR_REAR_LEFT:
        case DOOR_REAR_RIGHT:
            return RegionDoorRearLeft;
        case FENDER_FRONT_LEFT:
        case FENDER_FRONT_RIGHT:
            return RegionFenderFrontLeft; //10
        case BODY:
            return RegionBody; //12
        case HOOD:
            return RegionHoods; //13
        case SPOILER:
            return RegionSpoilers; //14
        case ROOFSCOOP:
            return RegionRoofScoops; //15
        case ROOF:
            return RegionRoofHeight; //16
        case EXHAUST:
            return RegionExhaust; //17
        case WINDOW_FRONT:
            return RegionFrontWindow;
        case WINDOW_REAR:
            return RegionRearWindow;
        case WINDOW_FRONT_LEFT:
            return RegionFrontLeftWindow;
        case WINDOW_FRONT_RIGHT:
            return RegionFrontRightWindow;
        case WINDOW_REAR_LEFT:
            return RegionRearLeftWindow;
        case WINDOW_REAR_RIGHT:
            return RegionRearRightWindow;
        case BUMPER_REAR_EXTRA:
            return RegionRearBumperExtra;
        case BUMPER_FRONT_EXTRA:
            return RegionFrontBumperExtra; //29
        default:
            return 3; //40 default but causes crashes
        }
    }

}

namespace Inputs {
    float* AutosculptSliderSpeedFactor; //0.2
    float* OptionsSliderSpeedFactor; //80
    float* PerfTuningSliderSpeedFactor; //100
    float* FEVisualPaintSpeedFactor1; //150
    float* FEVisualPaintSpeedFactor2; //100

    static float fakeFrameTime = 0.01666666666667f;

    static int debouncer;

    void(__cdecl* EnableInterrupts)() = NULL;
    void __cdecl frameHook() {
        bool isControlDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
        bool isShiftDown = (GetAsyncKeyState(VK_SHIFT) & 0x8000);

        if (Settings::SliderEnhancements) {
            if (*FEAutosculptScreen != NULL) {
                if (isControlDown) *AutosculptSliderSpeedFactor = AutosculptSliderSpeedSlow;
                else if (isShiftDown) *AutosculptSliderSpeedFactor = AutosculptSliderSpeedBoost;
                else *AutosculptSliderSpeedFactor = AutosculptSliderSpeed; //vanilla 0.2f
            }//autosculpt slider speed

            if (*FEOptionsStateManager != NULL) {
                if (isControlDown) *OptionsSliderSpeedFactor = OptionsSliderSpeedSlow;
                else if (isShiftDown) *OptionsSliderSpeedFactor = OptionsSliderSpeedBoost;
                else *OptionsSliderSpeedFactor = OptionsSliderSpeed; //vanilla 80.0f
            }//options slider speed

            if (*FEPerformanceTuning != NULL) {
                if (isControlDown) *PerfTuningSliderSpeedFactor = PerfTuningSliderSpeedSlow;
                else if (isShiftDown) *PerfTuningSliderSpeedFactor = PerfTuningSliderSpeedBoost;
                else *PerfTuningSliderSpeedFactor = PerfTuningSliderSpeed; //vanilla 100.0f
            }//perftuning slider speed

            if (*FEVisualPaint != NULL || *FEVinylPaintScreen != NULL) {
                if (isControlDown) *FEVisualPaintSpeedFactor1 = FEVisualPaint1SliderSpeedSlow;
                else if (isShiftDown) *FEVisualPaintSpeedFactor1 = FEVisualPaint1SliderSpeedBoost;
                else *FEVisualPaintSpeedFactor1 = FEVisualPaint1SliderSpeed;
                if (isControlDown) *FEVisualPaintSpeedFactor2 = FEVisualPaint2SliderSpeedSlow;
                else if (isShiftDown) *FEVisualPaintSpeedFactor2 = FEVisualPaint2SliderSpeedBoost;
                else *FEVisualPaintSpeedFactor2 = FEVisualPaint2SliderSpeed;
            }//paint inputs speed
        }

        if (isControlDown && GetAsyncKeyState(VK_DELETE) && *(timer_world + 1) > debouncer) { //debouncer broken in 1.0
            Overlay::doOverlay = !Overlay::doOverlay;
            debouncer = *(timer_world+1) + 1000; //general time
        }

        EnableInterrupts(); //call any potential other ASI hooked in the same place
    }
}

namespace Overlay {
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


    void __cdecl imgui_init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup scaling
        float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

        // Setup Platform/Renderer backends

        if (CURRENT_GAME_VERSION == UC_1_21) { //i don't know why setting hwnd and pd3dDevice in globals crash this script but they do
            void* hwnd = *(void**)0x01304e98;
            IDirect3DDevice9* g_pd3dDevice = *(IDirect3DDevice9**)0x013b3190;

            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplDX9_Init(g_pd3dDevice);
        }
        else if (CURRENT_GAME_VERSION == UC_1_0) {

            void* hwnd = *(void**)0x00df1df8;
            IDirect3DDevice9* g_pd3dDevice = *(IDirect3DDevice9**)0x00ea0110;

            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplDX9_Init(g_pd3dDevice);
        }

    }

    void* di_keyboard = NULL;
    void* di_mouse = NULL;

    void ImGuiFrame() {
        if (Settings::tests) ucetesting::imGuiStuff::ImGuiFrame();
    }


    //render and input hooks
    auto endSceneHookDest = (void(__cdecl*)())0x0;// 0758430;
    void __cdecl endSceneHook() {

        //TODO manage device lost
        if (doOverlay) {
            ImGui_ImplDX9_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            ImGuiFrame();

            ImGui::EndFrame();
            //BeginScene
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            //EndScene
        }

        endSceneHookDest();
    }

    auto WndProcHookDest = (bool(__cdecl*)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam))0x0;// 072b340;
    bool __cdecl WndProcHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        return WndProcHookDest(hWnd, msg, wParam, lParam);
    }

    auto D3DResetHookDest = (HRESULT(__fastcall*)(DWORD _this, DWORD edx, DWORD param))NULL;
    HRESULT __fastcall D3DResetHook(DWORD _this, DWORD edx, DWORD param) {
        ImGui_ImplDX9_InvalidateDeviceObjects();

        HRESULT hr = D3DResetHookDest(_this, edx, param);

        if (SUCCEEDED(hr))
            ImGui_ImplDX9_CreateDeviceObjects();

        return hr;
    }


    auto CreateDevice = (HRESULT(WINAPI*) (void* self, REFGUID rguid, void* lplpDirectInputDevice, void* pUnkOuter)) NULL;
    auto GetDeviceState = (HRESULT(WINAPI*) (void* device, DWORD, LPVOID)) NULL;
    static const GUID GUID_SysKeyboard = { 0x6F1D2B61, 0xD5A0, 0x11CF, { 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
    static const GUID GUID_SysMouse = { 0x6F1D2B60, 0xD5A0, 0x11CF, { 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

    HRESULT WINAPI GetDeviceStateHook(void* device, DWORD size, LPVOID data)
    {
        HRESULT hr = GetDeviceState(device, size, data);

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if ((io.WantCaptureMouse //|| io.WantCaptureKeyboard
            ) && SUCCEEDED(hr))
        {
            ZeroMemory(data, size);
        }

        return hr;
    }

    HRESULT WINAPI CreateDeviceHook(void* self, REFGUID rguid, void* lplpDirectInputDevice, void* pUnkOuter) {

        auto hr = CreateDevice(self, rguid, lplpDirectInputDevice, pUnkOuter);

        if (SUCCEEDED(hr))
        {
            if (rguid == GUID_SysKeyboard) {
                //std::cout << "DInput8 device: keyboard\n";
                di_keyboard = lplpDirectInputDevice;
            }
            else if (rguid == GUID_SysMouse) {
                //std::cout << "DInput8 device: mouse\n";
                di_mouse = lplpDirectInputDevice;
            }
            else return hr;

            if (GetDeviceState == NULL) GetDeviceState = (HRESULT(WINAPI*) (void* device, DWORD, LPVOID)) * (DWORD*)(**(DWORD**)lplpDirectInputDevice + 9 * 4);
            utils::set((uint32_t)(**(DWORD**)lplpDirectInputDevice + 9 * 4), GetDeviceStateHook);
        }
        return hr;
    }

    auto DirectInput8CreateDest = (HRESULT(WINAPI*) (HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, void* punkOuter)) 0x008aa560;
    HRESULT WINAPI DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, void* punkOuter) {
        //std::cout << "New DInput8 interface at " << ppvOut << "\n";

        auto result = DirectInput8CreateDest(hinst, dwVersion, riidltf, ppvOut, punkOuter);

        if (result == S_OK && ppvOut != NULL) {
            //hook create device
            //std::cout << "vtable pointer at " << *(DWORD**)ppvOut << "\n";
            //std::cout << "vtable start at " << **(DWORD***)ppvOut << "\n";
            //std::cout << "vtable 4th func at " << (DWORD*)(**(DWORD**)ppvOut + 0xc) << "\n";

            if (CreateDevice == NULL) CreateDevice = (HRESULT(WINAPI*) (void* self, REFGUID rguid, void* lplpDirectInputDevice, void* pUnkOuter)) * (DWORD*)(**(DWORD**)ppvOut + 0xc);

            //std::cout << "CreateDevice @ " << (DWORD*)CreateDevice << "\n"; //doesnt work probably the wrong function
            utils::set((uint32_t)(**(DWORD**)ppvOut + 0xc), CreateDeviceHook);

        }

        return result;
    }

}

namespace CarRenderConn {
    auto TestVisibility = (DWORD(__fastcall*)(DWORD)) 0x00826bc0;
    auto AcidEffect__Disable = (void(__fastcall*)(void*)) 0x00756710;

    auto Effect__Fire = (void(__fastcall*)(DWORD * ths, DWORD edx, DWORD, DWORD, float, DWORD)) 0x00839270; //draws MissShiftEffect
    auto Effect__Update = (void(__fastcall*)(DWORD * ths, DWORD edx, DWORD, DWORD, DWORD, float, DWORD)) 0x008269d0;

    auto UpdateEngineEffects = (void(__fastcall*)(DWORD ths, DWORD edx, DWORD Pkt_Car_Service, DWORD)) 0x0082a120;
    auto UpdateWindowGlassShatterEffects_HD = (void(__fastcall*)(DWORD ths, DWORD edx, char)) 0x0082a050;
    auto FireParticles = (void(__fastcall*)(DWORD ths, DWORD edx, DWORD, DWORD)) 0x00829ec0;

    auto constructor = (DWORD(__fastcall*)(DWORD CarRenderConn, DWORD edx, DWORD DrawCar, DWORD p2, DWORD p3)) 0x00859240;
    auto OnLoaded = (void(__fastcall*)(DWORD CarRenderConn, DWORD edx, DWORD CarRenderInfo)) 0x00839620;

    float* DistanceFactor = (float*)0x00cdfd34;
    DWORD* DAT_0178c9f4;

    //make more space in CarRenderConn
    /*
    * 0x29c last vanilla thing
    * 0x2a0 brakelight left next
    * 0x2a4 brakelight left previous
    * 0x2a8
    * 0x2ac
    *
    * append constructor with initialization (BRAKELIGHT_LEFT = (DWORD*)&BRAKELIGHT_LEFT; or smth, or just one thing for all brakelight markers)
    * edit OnLoaded to copy brakelight marker data from CarRenderInfo
    */
#define CarRenderConnNewLength 0x2b0

    void copyCRIMarker(DWORD CarRenderConn, DWORD markerOffsetCRC, DWORD CarRenderInfo, DWORD markerOffsetCRI);

    DWORD __fastcall constructor_wrapper(DWORD CarRenderConn, DWORD edx, DWORD DrawCar, DWORD p2, DWORD p3) {
        *(DWORD*)(CarRenderConn + 0x2a0) = CarRenderConn + 0x2a0; //init next
        *(DWORD*)(CarRenderConn + 0x2a4) = CarRenderConn + 0x2a0; //init prev
        *(DWORD*)(CarRenderConn + 0x2a8) = CarRenderConn + 0x2a8; //init next
        *(DWORD*)(CarRenderConn + 0x2ac) = CarRenderConn + 0x2a8; //init prev
        return constructor(CarRenderConn, edx, DrawCar, p2, p3); //in that case the constructor call can be later without issues
    }

    void __fastcall OnLoaded_wrapper(DWORD CarRenderConn, DWORD edx, DWORD CarRenderInfo) {
        OnLoaded(CarRenderConn, edx, CarRenderInfo);

        switch (CURRENT_GAME_VERSION) {
        case UC_1_21:
            copyCRIMarker(CarRenderConn, 0x2a0, CarRenderInfo, 0x5d0); //brakelight left
            copyCRIMarker(CarRenderConn, 0x2a8, CarRenderInfo, 0x5d8); //brakelight right
            break;
        case UC_1_0:
            copyCRIMarker(CarRenderConn, 0x2a0, CarRenderInfo, 0x5c4); //brakelight left
            copyCRIMarker(CarRenderConn, 0x2a8, CarRenderInfo, 0x5cc); //brakelight right
            break;
        }
    }

    void copyCRIMarker(DWORD CarRenderConn, DWORD markerOffsetCRC, DWORD CarRenderInfo, DWORD markerOffsetCRI)
    {

        DWORD thisMarkerXList = *(DWORD*)(CarRenderConn + markerOffsetCRC);
        DWORD CRIMarker = *(DWORD*)(CarRenderInfo + markerOffsetCRI); //5d0 brakelight left

        DWORD iVar13;

        if ((*(DWORD*)thisMarkerXList == thisMarkerXList) && CRIMarker != (CarRenderInfo + markerOffsetCRI)) {
            //            iVar13 = *(int*)((int)*ThreadLocalStoragePointer + *_tls_index * 4);
            do {
                //                DWORD pvVar11 = *(DWORD*)(iVar13 + 4);

                //                float* CRCMarker = (float*)0x0;// FUN_allocate_FastMem_005dbdd0(pvVar11, 0, 0x60, 0);
                float* CRCMarker = (float*)MemoryAlloc(0x60, 0, 0);
                if (CRCMarker != 0x0) {
                    if (*(DWORD*)(CRIMarker + 0x10) == 0) {
                        CRCMarker[4] = 1.0f;
                        CRCMarker[5] = 0.0f;
                        CRCMarker[6] = 0.0f;
                        CRCMarker[7] = 0.0f;

                        CRCMarker[8] = 0.0f;
                        CRCMarker[9] = 1.0f;
                        CRCMarker[10] = 0.0f;
                        CRCMarker[11] = 0.0f;

                        CRCMarker[12] = 0.0f;
                        CRCMarker[13] = 0.0f;
                        CRCMarker[14] = 1.0f;
                        CRCMarker[15] = 0.0f;

                        CRCMarker[16] = *(float*)(CRIMarker + 4);
                        CRCMarker[17] = *(float*)(CRIMarker + 8);
                        CRCMarker[18] = *(float*)(CRIMarker + 12);
                        CRCMarker[19] = 1.0f;
                        CRCMarker[20] = 0.0f;
                        CRCMarker[21] = 0.0f;
                    }
                    else {
                        auto dVar10 = *(float**)(CRIMarker + 0x10);

                        CRCMarker[4] = dVar10[4];
                        CRCMarker[5] = dVar10[5];
                        CRCMarker[6] = dVar10[6];
                        CRCMarker[7] = dVar10[7];

                        CRCMarker[8] = dVar10[8];
                        CRCMarker[9] = dVar10[9];
                        CRCMarker[10] = dVar10[10];
                        CRCMarker[11] = dVar10[11];

                        CRCMarker[12] = dVar10[12];
                        CRCMarker[13] = dVar10[13];
                        CRCMarker[14] = dVar10[14];
                        CRCMarker[15] = dVar10[15];

                        CRCMarker[16] = dVar10[16];
                        CRCMarker[17] = dVar10[17];
                        CRCMarker[18] = dVar10[18];
                        CRCMarker[19] = dVar10[19];

                        CRCMarker[20] = 0.0f;
                        CRCMarker[21] = 0.0f;
                    }
                    ((DWORD*)CRCMarker)[0] = thisMarkerXList;
                    ((DWORD*)CRCMarker)[1] = *(DWORD*)(CarRenderConn + markerOffsetCRC + 4);

                    *(DWORD*)*(DWORD*)(CarRenderConn + markerOffsetCRC + 4) = (DWORD)CRCMarker;
                    *(DWORD*)(CarRenderConn + markerOffsetCRC + 4) = (DWORD)CRCMarker;
                }

                CRIMarker = *(DWORD*)CRIMarker;

            } while (CRIMarker != (CarRenderInfo + markerOffsetCRI));
        }
    }

    /* CarRenderConn stuff
    * flags +0x26c : 1 hidden 2 missshift 4 isPlayer 8 isRaining 16 blowOff
    * shifting +0x240
    * blowOffTimer +0x244
    *
    */

    void __fastcall UpdateHeadlightAndBrakelightShatterEffects(DWORD CarRenderConn, DWORD edx, char param_1) {
        DWORD* puVar1;
        DWORD shatterEffectHeadlights;
        DWORD shatterEffectBrakelights;
#define GlassShatterEffectHeadlights 0x7A656CCB
#define GlassShatterEffectBrakelights 0x925D4FFA

        puVar1 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, GlassShatterEffectHeadlights, 0);
        if (puVar1 == (DWORD*)0x0) puVar1 = (DWORD*)Attrib::DefaultDataArea(4);
        shatterEffectHeadlights = puVar1[1];

        puVar1 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, GlassShatterEffectBrakelights, 0);
        if (puVar1 == (DWORD*)0x0) puVar1 = (DWORD*)Attrib::DefaultDataArea(4);
        shatterEffectBrakelights = puVar1[1];
        if ((param_1 & 8)) FireParticles(CarRenderConn, 0, shatterEffectBrakelights, CARFXPOS_BRAKELIGHT_LEFT);
        if ((param_1 & 0x10) != 0) FireParticles(CarRenderConn, 0, shatterEffectBrakelights, CARFXPOS_BRAKELIGHT_RIGHT);
        if ((param_1 & 1) != 0) FireParticles(CarRenderConn, 0, shatterEffectHeadlights, CARFXPOS_HEADLIGHT_LEFT);
        if ((param_1 & 2) != 0) FireParticles(CarRenderConn, 0, shatterEffectHeadlights, CARFXPOS_HEADLIGHT_RIGHT);
        return;
    }

    void __fastcall UpdateEffects(DWORD CarRenderConn, DWORD edx, DWORD Pkt_Car_Service, DWORD param_4, DWORD param_5, char lightsGlassDamage, char windowsDamage) {
        int* piVar1;
        DWORD* marker;
        DWORD* puVar5;
        int* piVar6;
        float fVar7;

        DWORD _MissShiftEffect, _NOSEffect, _ExhaustSmokeEffect, _NOSTrailEffect, _BackfireEffect;


        fVar7 = *DistanceFactor * 80.0;
        if (((*DAT_0178c9f4 != 0) || (TestVisibility(CarRenderConn) != 0)) ||
            ((*(DWORD*)(CarRenderConn + 0x230) <= *(int*)(CarRenderConn + 0x234) + 2U  //LastRenderFrame    LastVisibleFrame
                && ((*(int*)(CarRenderConn + 0x234) != 0 && (*(float*)(CarRenderConn + 0x238) <= fVar7)))))) { //DistanceToView


            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, MissShiftEffect, 0);
            if (puVar5 == (DWORD*)0x0) _MissShiftEffect = MissShiftEffect; //if it doesn't exist as a field, force an emittergroup node
            else _MissShiftEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, NOSEffect, 0);
            if (puVar5 == (DWORD*)0x0) _NOSEffect = NOSEffect; //if it doesn't exist as a field, force an emittergroup node
            else _NOSEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, ExhaustSmokeEffect, 0);
            if (puVar5 == (DWORD*)0x0) _ExhaustSmokeEffect = ExhaustSmokeEffect; //if it doesn't exist as a field, force an emittergroup node
            else _ExhaustSmokeEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, NOSTrailEffect, 0);
            if (puVar5 == (DWORD*)0x0) _NOSTrailEffect = NOSTrailEffect; //if it doesn't exist as a field, force an emittergroup node
            else _NOSTrailEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::_Instance*)(CarRenderConn + 0x14), 0, BackfireEffect, 0);
            if (puVar5 == (DWORD*)0x0) _BackfireEffect = BackfireEffect; //if it doesn't exist as a field, force an emittergroup node
            else _BackfireEffect = puVar5[1];
            /*
            * 0x250 = EXHAUST_FX
            * 0x258 = ENGINE_FX
            * 0x2a0 = BRAKELIGHT_LEFT
            * 0x2a8 = BRAKELIGHT_RIGHT
            */

            if (!*(bool*)(Pkt_Car_Service + 0x101)) { //Nos
                if ((*(DWORD*)(CarRenderConn + 0x26c) & 2) == 0) { //Flags MissShift

                    //if (((*(DWORD*)(CarRenderConn + 0x26c) & 0x10) != 0) && (0.0 < *(float*)(CarRenderConn + 0x244))) { //Flags Blowoff, never set, but we can use the value from Shifting instead
                    if ((*(float*)(CarRenderConn + 0x240) != 0) && (0.0 < *(float*)(CarRenderConn + 0x244))) {
                        //gearshift backfire
                        for (marker = *(DWORD**)(CarRenderConn + 0x250); marker != (DWORD*)(CarRenderConn + 0x250); marker = (DWORD*)*marker) {
                            if (_BackfireEffect != 0) Effect__Update(marker, 0, ((*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40), _BackfireEffect, param_4, 1.0, *(DWORD*)(CarRenderConn + 0x34));
                            else if ((void*)marker[0x14] != (void*)0x0) AcidEffect__Disable((void*)marker[0x14]);
                            //} else if ((void*)marker[0x14] != (void*)0x0) {
                            //    AcidEffect__Disable((void*)marker[0x14]);
                            //    //cout << "No effect\n";
                        }

                    }
                    else {
                        //nothing
                        for (marker = *(DWORD**)(CarRenderConn + 0x250); marker != (DWORD*)(CarRenderConn + 0x250); marker = (DWORD*)*marker) {
                            if (_ExhaustSmokeEffect != 0) Effect__Update(marker, 0, ((*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40), _ExhaustSmokeEffect, param_4, 1.0, *(DWORD*)(CarRenderConn + 0x34));
                            else if ((void*)marker[0x14] != (void*)0x0) AcidEffect__Disable((void*)marker[0x14]);
                        }
                    }
                }
                else {
                    //MissShift
                    for (marker = *(DWORD**)(CarRenderConn + 0x250); marker != (DWORD*)(CarRenderConn + 0x250); marker = (DWORD*)*marker) {
                        //Effect__Update(marker, 0, ((*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40), _MissShiftEffect, param_4, 1.0, *(DWORD*)(CarRenderConn + 0x34));
                        Effect__Fire(marker, 0, (*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40, _MissShiftEffect, 1.0, *(DWORD*)(CarRenderConn + 0x34)); //broken function for lasting effects
                    }
                }
            }
            else {
                //Nos

                for (marker = *(DWORD**)(CarRenderConn + 0x250); marker != (DWORD*)(CarRenderConn + 0x250); marker = (DWORD*)*marker) {
                    Effect__Update(marker, 0, ((*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40), _NOSEffect, param_4, 1.0, *(DWORD*)(CarRenderConn + 0x34));
                }

                if (_NOSTrailEffect != 0) {
                    for (marker = *(DWORD**)(CarRenderConn + 0x2a0); marker != (DWORD*)(CarRenderConn + 0x2a0); marker = (DWORD*)*marker) {
                        Effect__Update(marker, 0, ((*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40), _NOSTrailEffect, param_4, 1.0, *(DWORD*)(CarRenderConn + 0x34));
                    }
                    for (marker = *(DWORD**)(CarRenderConn + 0x2a8); marker != (DWORD*)(CarRenderConn + 0x2a8); marker = (DWORD*)*marker) {
                        Effect__Update(marker, 0, ((*(DWORD*)(CarRenderConn + 0x1d0)) + 0x40), _NOSTrailEffect, param_4, 1.0, *(DWORD*)(CarRenderConn + 0x34));
                    }
                }
            }

            UpdateEngineEffects(CarRenderConn, 0, Pkt_Car_Service, param_4);
            UpdateWindowGlassShatterEffects_HD(CarRenderConn, 0, windowsDamage);
            UpdateHeadlightAndBrakelightShatterEffects(CarRenderConn, 0, lightsGlassDamage);
            *(DWORD*)(CarRenderConn + 0x26c) = *(DWORD*)(CarRenderConn + 0x26c) & 0xfffffffd; //flags ~MissShift


        }
        else { //don't draw
            for (marker = *(DWORD**)(CarRenderConn + 0x250); marker != (DWORD*)(CarRenderConn + 0x250); marker = (DWORD*)*marker) {
                if ((void*)marker[0x14] != (void*)0x0) {
                    AcidEffect__Disable((void*)marker[0x14]);
                }
            }
            piVar1 = (int*)(CarRenderConn + 600);
            piVar6 = (int*)*piVar1;
            if (piVar6 != piVar1) {
                do {
                    if ((void*)piVar6[0x14] != (void*)0x0) {
                        AcidEffect__Disable((void*)piVar6[0x14]);
                    }
                    piVar6 = (int*)*piVar6;
                } while (piVar6 != piVar1);
            }
        }
    }

    //void __cdecl SFXObj_Drivetrain_appendix() {
    //    doShiftEffect = 30;
    //}
}

namespace ucetesting {
    using namespace std;

    std::ofstream modscript;

    //ok mostly
    namespace prints {
        void restoreDisabledPrint0(int v1, int v2, char* text, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6) {
            if (v1 == 7) return; //removes MassiveAdClient bloat
            printf(text, arg1, arg2, arg3, arg4, arg5, arg6);
            printf("\n");
        }

        void* restoreDisabledPrintFixupReturn;
        __declspec(naked) void restoreDisabledPrintFixup() {
            __asm {
                push ebp
                mov ebp, esp
                push esi
                mov esi, dword ptr[ebp + 0x4]
                mov ecx, esi
                jmp restoreDisabledPrintFixupReturn;
            }
        }



        void __cdecl replacePrint_006d9570(DWORD param, char* text, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6) {
            //cout << param << " - " << text << " - " << args << "\n";
            printf(text, arg1, arg2, arg3, arg4, arg5, arg6);
            printf("\n");
        }

        void __cdecl aptDebugPrint(char* str1, char* str2, int int3) {
            std::cout << "[AptDebug] " << str1 << " | " << str2 << " | " << int3 << "\n";
        }

        void __cdecl printfSimpleCdecl(char* text, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6) {
            //cout << text << "\n";
            printf(text, arg1, arg2, arg3, arg4, arg5, arg6);
        }

        void __cdecl Fesl_Debug_replacement(DWORD, DWORD, char* print, DWORD vals) {
            vprintf(print, (va_list)&vals);
            std::cout << "\n";
        }


        void __cdecl somethingPrintName(char, DWORD, char* name) {
            std::cout << name << "\n";
        }
    }

    //TODO broken
    namespace quotes {


        uint32_t* quotes;
        void RandomizeQuotes() {
            //    std::cout << "Quotes table pointer: " << quotes << "\n";


            std::uniform_int_distribution<> quoteTableIndex(1, 6);
            switch (quoteTableIndex(gen)) {
            case 1:
                //keep vanilla
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_A_1");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_B");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_C");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_D");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_E");
                break;
            case 2:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_B");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_C");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_D");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_D_1");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_E");
                break;
            case 3:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_B");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_B_1");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_C");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_D");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_D_1");
                break;
            case 4:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_A_1");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_B");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_B_1");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_C");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_C_1");
                break;
            case 5:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_B");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_C");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_C_1");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_D");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_E");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_E_1");
                break;
            case 6:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_B");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_C");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_D");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_E");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_F");
                break;
            }



        }


        void* quotesDetourReturn;
        __declspec(naked) void QuotesDetour() {
            __asm {
                push eax;
                mov eax, dword ptr[esp + 0x8];
                mov quotes, eax;
                push ebx;
                push ecx;
                push edx;
                call RandomizeQuotes;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp quotesDetourReturn;
            }
        }



    } //quotes TODO broken

    //some day maybe
    namespace regionCops {

        void __cdecl printPreset(UINT* preset) { //using UINT* to display as hex
            //    cout << "presetride read: " << preset << "\n";
        }
        void* interceptPresetrideSpawnReturn;
        __declspec(naked) void interceptPresetrideSpawn() {
            __asm {
                push eax;
                push ebx;
                push edx;
                push ecx;
                call printPreset;
                pop ecx;
                pop edx;
                pop ebx;
                pop eax;
                PUSH       EAX
                    PUSH       ECX
                    PUSH       0x27e73952
                    jmp interceptPresetrideSpawnReturn;
            }
        }

        void printCopFcn() {
            std::cout << "cop related function (?) entered \n";
        }

        void* copPresetTestReturn;
        __declspec(naked) void copPresetTest() {
            __asm {
                PUSH       0x0
                PUSH       0xEA91BC7F;
                LEA        ECX, [ESP + 0x10]
                    push eax;
                push ebx;
                push ecx;
                push edx;
                call printCopFcn;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp copPresetTestReturn;
            }
        }

        void* copPresetTest2Return;
        __declspec(naked) void copPresetTest2() {
            __asm {
                PUSH       0x08FAC0E7;
                LEA        ECX, [ESP + 0x44]
                    MOV        dword ptr[ESP + 0x5c], 0x0


                    push eax;
                push ebx;
                push ecx;
                push edx;
                call printCopFcn;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp copPresetTestReturn;
            }
        }

        void* copPresetTest3Return;
        __declspec(naked) void copPresetTest3() {
            __asm {
                PUSH       0x0
                PUSH       0xEA91BC7F;
                LEA        ECX, [ESP + 0x10]
                    push eax;
                push ebx;
                push ecx;
                push edx;
                call printCopFcn;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp copPresetTestReturn;
            }
        }

    }

    //TODO
    namespace vinylsMenu {

        //NOT INCLUDED

        auto VinylCanSkew = (bool(__fastcall*)(void* this_)) 0x005d4c80;
        auto VinylHasMirror = (bool(__fastcall*)(void* this_)) 0x005cdd50;
        auto VinylNotFullbody = (bool(__fastcall*)(void* this_)) 0x005d2670;
        auto VinylHasColours = (bool(__fastcall*)(void* this_)) 0x005cdd10;
        auto getLocalizedString = (char* (__cdecl*)(DWORD binHash)) 0x00575d60;

        auto FETreeDataNode_assign_label = (void* (__fastcall*)(FETreeDataNode * this_, DWORD unaff, char* name))0x00536620;

#define CUST_VINYL_MOVE 0xAF236425
#define CUST_VINYL_SCALE 0x93F58F16
#define CUST_VINYL_ROTATE 0x1135ADDD
#define CUST_VINYL_SKEW 0xAF269B48
#define CUST_VINYL_FLIP 0xAF1F7F19
#define CUST_VINYL_MIRROR 0x51E7EA9
#define CUST_VINYL_COLOR 0x92DACACD

        void __fastcall CreateVinylEditMenu(void* this_, DWORD) {
            FETreeDataNode* curTreeNode;
            char* pcVar4;
            DWORD uVar9;
            DWORD uVar10;
            LinkedNode local_14;
            LinkedNode* current;
            char local_4;

            bool cannotSkew = VinylCanSkew(*(void**)((int)this_ + 0x50));
            bool hasMirror = VinylHasMirror(*(void**)((int)this_ + 0x50));
            bool notFullbody = VinylNotFullbody(*(void**)((int)this_ + 0x50));
            bool hasColours = VinylHasColours(*(void**)((int)this_ + 0x50));

            local_4 = 0;

            local_14.previous = &local_14;
            local_14.next = local_14.previous;

            if (notFullbody) {
                local_14.next = local_14.previous;

                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 1;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_MOVE);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->field3_0xc = 0;
                    curTreeNode->NameHash_0x10 = 0xffffffff;
                    curTreeNode->field5_0x14 = 0xffffffff;
                    curTreeNode->id_0x18 = 0x9f;
                    curTreeNode->field7_0x1c = 0;
                    curTreeNode->label_0x20 = (char*)0x0;
                    curTreeNode->field9_0x24 = 0;
                    curTreeNode->field_0x28 = 1;
                    FETreeDataNode_assign_label(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (LinkedNode*)0x0;
                }
                else {
                    current = &curTreeNode->nodeLinking_0x4;
                }
                (local_14.next)->previous = current;
                current->next = local_14.next;
                current->previous = &local_14;
                local_14.next = current;



                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 2;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_SCALE);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->field3_0xc = 0;
                    curTreeNode->NameHash_0x10 = 0xffffffff;
                    curTreeNode->field5_0x14 = 0xffffffff;
                    curTreeNode->id_0x18 = 0xa0;
                    curTreeNode->field7_0x1c = 0;
                    curTreeNode->label_0x20 = (char*)0x0;
                    curTreeNode->field9_0x24 = 0;
                    curTreeNode->field_0x28 = 1;
                    FETreeDataNode_assign_label(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (LinkedNode*)0x0;
                }
                else {
                    current = &curTreeNode->nodeLinking_0x4;
                }
                (local_14.next)->previous = current;
                current->next = local_14.next;
                current->previous = &local_14;
                local_14.next = current;



                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 3;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_ROTATE);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->field3_0xc = 0;
                    curTreeNode->NameHash_0x10 = 0xffffffff;
                    curTreeNode->field5_0x14 = 0xffffffff;
                    curTreeNode->id_0x18 = 0xa1;
                    curTreeNode->field7_0x1c = 0;
                    curTreeNode->label_0x20 = (char*)0x0;
                    curTreeNode->field9_0x24 = 0;
                    curTreeNode->field_0x28 = 1;
                    FETreeDataNode_assign_label(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (LinkedNode*)0x0;
                }
                else {
                    current = &curTreeNode->nodeLinking_0x4;
                }
                (local_14.next)->previous = current;
                current->next = local_14.next;
                current->previous = &local_14;
                local_14.next = current;


                if (!cannotSkew) {
                    curTreeNode = (FETreeDataNode*)
                        MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                    local_4 = 4;
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        curTreeNode = (FETreeDataNode*)0x0;
                    }
                    else {
                        curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa2, getLocalizedString(CUST_VINYL_SKEW), 0xffffffff, 0xffffffff);
                    }
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        current = (LinkedNode*)0x0;
                    }
                    else {
                        current = &curTreeNode->nodeLinking_0x4;
                    }
                    (local_14.next)->previous = current;
                    current->next = local_14.next;
                    current->previous = &local_14;
                    local_14.next = current;
                }


                local_4 = 0;
                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 5;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_FLIP);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->field3_0xc = 0;
                    curTreeNode->NameHash_0x10 = 0xffffffff;
                    curTreeNode->field5_0x14 = 0xffffffff;
                    curTreeNode->id_0x18 = 0xa3;
                    curTreeNode->field7_0x1c = 0;
                    curTreeNode->label_0x20 = (char*)0x0;
                    curTreeNode->field9_0x24 = 0;
                    curTreeNode->field_0x28 = 1;
                    FETreeDataNode_assign_label(curTreeNode, 0, pcVar4);
                }

                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (LinkedNode*)0x0;
                }
                else {
                    current = &curTreeNode->nodeLinking_0x4;
                }
                (local_14.next)->previous = current;
                current->next = local_14.next;
                current->previous = &local_14;
                local_14.next = current;


                if (hasMirror) {
                    curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                    local_4 = 6;
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        curTreeNode = (FETreeDataNode*)0x0;
                    }
                    else {
                        curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa4, getLocalizedString(CUST_VINYL_MIRROR), 0xffffffff, 0xffffffff);
                    }
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        current = (LinkedNode*)0x0;
                    }
                    else {
                        current = &curTreeNode->nodeLinking_0x4;
                    }
                    (local_14.next)->previous = current;
                    current->next = local_14.next;
                    current->previous = &local_14;
                    local_14.next = current;
                }
            }



            local_4 = 0;
            if (hasColours) {
                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 7;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_COLOR);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->field3_0xc = 0;
                    curTreeNode->NameHash_0x10 = 0xffffffff;
                    curTreeNode->field5_0x14 = 0xffffffff;
                    curTreeNode->id_0x18 = 0xa5;
                    curTreeNode->field7_0x1c = 0;
                    curTreeNode->label_0x20 = (char*)0x0;
                    curTreeNode->field9_0x24 = 0;
                    curTreeNode->field_0x28 = 1;
                    FETreeDataNode_assign_label(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (LinkedNode*)0x0;
                }
                else {
                    current = &curTreeNode->nodeLinking_0x4;
                }
                (local_14.next)->previous = current;
                current->next = local_14.next;
                current->previous = &local_14;
                local_14.next = current;
            }




            curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
            local_4 = 8;
            if (curTreeNode == (FETreeDataNode*)0x0) {
                curTreeNode = (FETreeDataNode*)0x0;
            }
            else {
                curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa6, (char*)"ROBER LE MENU ITEM", 0xffffffff, 0xffffffff);
            }
            if (curTreeNode == (FETreeDataNode*)0x0) {
                current = (LinkedNode*)0x0;
            }
            else {
                current = &curTreeNode->nodeLinking_0x4;
            }
            (local_14.next)->previous = current;
            current->next = local_14.next;
            current->previous = &local_14;
            local_14.next = current;


            curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
            local_4 = 9;
            if (curTreeNode == (FETreeDataNode*)0x0) {
                curTreeNode = (FETreeDataNode*)0x0;
            }
            else {
                curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa7, (char*)"this fucking sucks", 0xffffffff, 0xffffffff);
            }
            if (curTreeNode == (FETreeDataNode*)0x0) {
                current = (LinkedNode*)0x0;
            }
            else {
                current = &curTreeNode->nodeLinking_0x4;
            }
            (local_14.next)->previous = current;
            current->next = local_14.next;
            current->previous = &local_14;
            local_14.next = current;








            int iVar3 = FUN_getLinkedListLength_004a76f0(&local_14, 0, 0x0);
            *(DWORD*)((int)this_ + 0x14) = *(DWORD*)((int)this_ + 4);

            DWORD vftable = *(DWORD*)this_;


            if (iVar3 < 1) {
                *(DWORD*)((int)this_ + 4) = 9;

                //              pcVar1 = *(code **)(*this + 0x14c); (*pcVar1)(this); 
                //(**(code **)(*this + 0x14c))(this); 
                //(**(code **)(*this + 0x144))(this);

                ((void(__fastcall*)(void*)) * (DWORD*)(vftable + 0x14c))   (this_);
                ((void(__fastcall*)(void*)) * (DWORD*)(vftable + 0x144))   (this_); //show popup "This vinyl is not customizable"
            }
            else {
                *(DWORD*)((int)this_ + 4) = 3;
                //(**(code **)(*this + 0x14c))(this);
                ((void(__fastcall*)(void*)) * (DWORD*)(vftable + 0x14c))   (this_); //0x594990

                auto this_00 = FE_getFeElement(*FEScreenManager, 0, 0x28);
                AddToTree(this_00, 0, &local_14, 0); //fucking dies (this_00 == 0)
            }
            //    local_4 = 0xffffffff;
            FUN_00553aa0(&local_14);



        }

    }

    //TODO its majorly fucked
    namespace feCarsShopTroles {

        auto FUN_addCarToRegion_00518220 = (void(__fastcall*)(DWORD this_, DWORD unaff, DWORD, FECarRecord*)) 0x00518220;
        auto FUN_load_carlist_00506a80 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x00506a80;
        auto vlt_loadArray_004609a0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*, DWORD)) 0x004609a0;
        auto FUN_getArraySize_0045ea80 = (short(__fastcall*)(DWORD * this_)) 0x0045ea80;
        auto vlt_getArrayElement_0045eaa0 = (DWORD * (__fastcall*)(DWORD * this_, DWORD unaff, DWORD)) 0x0045eaa0;
        auto FUN_0045ced0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*)) 0x0045ced0;
        auto vltget_pvehicle_00451d40 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x00451d40;
        auto vltget_frontend_005019b0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x005019b0;
        auto FUN_00507030 = (bool(__fastcall*)(DWORD * this_)) 0x00507030;
        auto getVltValuePointer_0045e7d0 = (DWORD(__fastcall*)(DWORD * this_, DWORD unaff, DWORD field, DWORD arrayIndex))0x0045e7d0;
        auto FUN_0045cbe0 = (DWORD(__fastcall*)(DWORD * this_)) 0x0045cbe0;
        auto SavegameCar_update_00507610 = (void(__fastcall*)(FECarRecord * this_)) 0x00507610;
        auto FUN_vltnode_clearFromMem_00462550 = (void(__fastcall*)(DWORD * this_)) 0x00462550;
        auto FUN_004628f0 = (void(__fastcall*)(DWORD * this_)) 0x004628f0;


        auto FUN_00462c80 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x00462c80;
        auto FUN_00464a20 = (DWORD(__cdecl*)(DWORD, DWORD)) 0x00464a20;



        void __fastcall FUN_load_carlist_fixup_1_0(DWORD* carlist_node, DWORD unaff, DWORD, DWORD) {
            auto uVar7 = FUN_00464a20(0x776f8686, 0xcdca884c);
            FUN_00462c80(carlist_node, 0, uVar7, 0);
        }

        void __fastcall vltget_pvehicle_00451d40_fixup_1_0(DWORD* pvehicle, DWORD unaff, DWORD data, DWORD) {
            auto uVar7 = FUN_00464a20(0x4a97ec8f, data);
            FUN_00462c80(pvehicle, 0, uVar7, 0);
        }






        void __fastcall FECarsShopMenuDetourCPP(DWORD ecx, DWORD edx) {

            DWORD carlist_node[4];
            DWORD StockCarsArray[4];

#define fe_carlist 0xCDCA884C
#define StockCars 0xA28B3B3D
#define PlayerUsable 0xF099B6AC

            FUN_load_carlist_00506a80(carlist_node, 0, fe_carlist, 0);
            vlt_loadArray_004609a0(carlist_node, 0, StockCarsArray, StockCars);

            auto totalCars = FUN_getArraySize_0045ea80(StockCarsArray);

            auto playerCars = (FECarRecord*)(*(DWORD*)UserProfile_Player1 + 0x88);

            for (int i = 0; i < totalCars; i++) {

                DWORD* currentVal = vlt_getArrayElement_0045eaa0(StockCarsArray, 0, i);
                if (currentVal == 0) continue;

                DWORD auStack_68[4];
                DWORD pvehicle[4];
                DWORD frontend[4];
                FUN_0045ced0(auStack_68, 0, currentVal);
                vltget_pvehicle_00451d40(pvehicle, 0, auStack_68[1], 0);

                if ((pvehicle[0] != 0) && (((char)pvehicle[3] & 1) == 0)) {
                    vltget_frontend_005019b0(frontend, 0, pvehicle[1] + 8, 0);
                    auto pcVar1 = *(char**)(pvehicle[1] + 4);

                    //cout << "pvehicle exists: " << (DWORD*)(pvehicle[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                    bool bVar2 = true;
                    //bVar2 = FUN_00507030(pvehicle);
                    if ((bVar2) && (frontend[0] != 0)) {
                        //cout << "frontend exists: "<< (DWORD*)(frontend[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                        auto PlayerUsable_00 = getVltValuePointer_0045e7d0(pvehicle, 0, PlayerUsable, 0);
                        if (PlayerUsable_00 != 0) {
                            if (!*(bool*)PlayerUsable_00) continue;
                        }
                        //cout << "player usable\n";

                        if (((pcVar1 != (char*)0x0)) && (((*pcVar1 != '\0' && (((char)pvehicle[3] & 1) == 0))))) {

                            FECarRecord* pSVar1 = NULL;
                            auto newCarFrontend = FUN_0045cbe0(frontend);
                            auto newCarPVehicle = FUN_0045cbe0(pvehicle);


                            for (int i = 0; i < 330; i++) {
                                if (playerCars[i].pvehicle == newCarPVehicle) {
                                    pSVar1 = &playerCars[i];
                                    cout << "already in save game\n";
                                    break;
                                }
                            }

                            if (pSVar1 == NULL) {
                                pSVar1 = new FECarRecord();
                                pSVar1->id = i;

                                pSVar1->frontend = newCarFrontend;
                                pSVar1->pvehicle = newCarPVehicle;
                                SavegameCar_update_00507610(pSVar1);
                            }


                            cout << "id=" << (DWORD*)pSVar1->id << "\n";
                            cout << "frontend=" << (DWORD*)pSVar1->frontend << "\n";
                            cout << "pvehicle=" << (DWORD*)pSVar1->pvehicle << "\n";
                            cout << "preset=" << (DWORD*)pSVar1->preset << "\n";
                            cout << "region=" << (DWORD*)pSVar1->filterBits << "\n";
                            cout << "customization=" << (DWORD*)pSVar1->customization << "\n";
                            cout << "careerHandle=" << (DWORD*)pSVar1->careerHandle << "\n";
                            cout << "isPresetSkin=" << (DWORD*)pSVar1->isPresetSkin << "\n";
                            cout << "isPinkSlip=" << (DWORD*)pSVar1->isPresetSkin << "\n";


                            //add to FE
                            DWORD this_00 = (ecx + 0x48); //america
                            if (pSVar1->filterBits & 0x10000) {
                                this_00 = (ecx + 0x48);
                            }
                            else if (pSVar1->filterBits & 0x20000) {
                                this_00 = (ecx + 0x58);
                            }
                            else if (pSVar1->filterBits & 0x40000) {
                                this_00 = (ecx + 0x68);
                            }
                            //else continue;

                            DWORD puVar1 = ((DWORD*)this_00)[1];
                            if (puVar1 < ((DWORD*)this_00)[2]) {
                                ((DWORD*)this_00)[1] = puVar1 + 0x28;

                                if (puVar1 != 0x0) {
                                    ((DWORD*)puVar1)[0] = pSVar1->id;
                                    ((DWORD*)puVar1)[1] = pSVar1->frontend;
                                    ((DWORD*)puVar1)[2] = pSVar1->pvehicle;
                                    ((DWORD*)puVar1)[3] = pSVar1->preset;
                                    ((DWORD*)puVar1)[4] = pSVar1->filterBits;
                                    *(char*)(puVar1 + 20) = pSVar1->customization;
                                    *(char*)(puVar1 + 21) = pSVar1->careerHandle;
                                    *(char*)(puVar1 + 22) = pSVar1->isPresetSkin;
                                    *(char*)(puVar1 + 23) = pSVar1->padding;
                                    *(char*)(puVar1 + 24) = pSVar1->isPinkSlip;
                                    *(char*)(puVar1 + 25) = pSVar1->pad1;
                                    *(char*)(puVar1 + 26) = pSVar1->pad2;
                                    *(char*)(puVar1 + 27) = pSVar1->pad3;
                                }
                            }
                            else {
                                FUN_addCarToRegion_00518220(this_00, 0, puVar1, pSVar1);

                            }
                            cout << "car added to region " << (DWORD*)(this_00 - ecx) << "\n";
                            //add to FE








                        }
                    }
                    FUN_vltnode_clearFromMem_00462550(frontend);
                }
                FUN_vltnode_clearFromMem_00462550(pvehicle);
                if (auStack_68[2] != 0) {
                    FUN_004628f0(auStack_68);
                }






            }


        }

        DWORD FECarsShopMenuDetourReturn = 0x0059661e;
        __declspec(naked) void FECarsShopMenuDetour() {
            __asm {
                mov ecx, esi;
                call FECarsShopMenuDetourCPP;

                jmp FECarsShopMenuDetourReturn;
            }
        }

    }

    namespace raceMusic {

        DWORD MusicLogic__Ready_detour_back = 0x004a523d;
        DWORD MusicLogic__RaceMusic_halfway = 0x00468787;
        __declspec(naked) void MusicLogic__Ready_detour() {
            __asm {
                jz cont;
                jmp MusicLogic__Ready_detour_back;

            cont:
                //play music here

                pop esi;
                pop edi;
                push esi;
                mov esi, ecx;
                jmp MusicLogic__RaceMusic_halfway;
                //        ret 0x4;
            }
        }



    }

    namespace unrestrictedCarTextures {

        int numTexturesInLastTPK = 0; //in last loaded tpk
        DWORD texturesInLastTPK[64];
        bool doTexListing = false;

        void __cdecl readTextures(DWORD ecx) {
            //if (!doTexListing) return;

        //    cout << "TPK TexList @" << (DWORD*)ecx << "\n";
            DWORD blockLength = *(DWORD*)(ecx + 4);
            //    cout << "Length " << blockLength << "\n";
            //    cout << "Num textures " << blockLength / 8 << "\n";

            numTexturesInLastTPK = blockLength / 8;
            if (numTexturesInLastTPK > 64) numTexturesInLastTPK = 64;

            int i = 0;
            for (int off = 8; off <= blockLength; off += 8) {
                //        cout << "Texture hash " << (DWORD*)(*(DWORD*)(ecx + off)) << "\n";
                texturesInLastTPK[i] = *(DWORD*)(ecx + off);
                i++;
                if (i > 63) break;
            }
        }

        DWORD TexList_detour_return = 0x0052a5d8;
        __declspec(naked) void TexList_detour() {
            __asm {
                pushad;
                push ecx;
                call readTextures;
                pop ecx;
                popad;
                lea eax, [ecx + 0x8];
                mov dword ptr[edi + 0x74], eax;
                mov eax, dword ptr[ecx + 0x4];
                shr eax, 0x3;
                mov ebx, eax;
                jmp TexList_detour_return;
            }
        }

        auto getXNAME_String = (char* (__cdecl*)(DWORD)) 0x00817940;
        void __cdecl loadCarTexturesReplacement(DWORD storage, DWORD CarRenderClass43c, DWORD) {
            memset((void*)storage, 0, 0x3e4);
            int texCount = 0;

            char* xname = getXNAME_String(*(DWORD*)CarRenderClass43c);
            char cbuf[128];
            sprintf_s(cbuf, "%s_SKIN1", xname);
            *(DWORD*)(storage + 0x330) = getBinHash(cbuf);

            ((DWORD*)storage)[texCount] = getBinHash(cbuf);
            texCount++;

            for (int i = 0; i < numTexturesInLastTPK; i++) {
                ((DWORD*)storage)[texCount] = texturesInLastTPK[i];
                texCount++;
            }




            *(DWORD*)(storage + 0x32c) = texCount * 4;
            *(DWORD*)(storage + 0x328) = texCount;


        }

        void __fastcall enableTexListingDetour(DWORD ecx, DWORD edx, DWORD v1) {
            doTexListing = true;
        }
        void __fastcall disableTexListingDetour(DWORD ecx, DWORD edx, DWORD v1) {
            doTexListing = false;
        }

    }

    //nothing left
    namespace experimentalCrashFixes {


        //implemented but slightly differently
        /*
        auto GetAmountFree_004ac600 = (DWORD(__fastcall*)(DWORD MemoryPool)) 0x004ac600;
        auto EA__Thread__Mutex__Lock_00977480 = (void(__fastcall*)(DWORD, DWORD edx, DWORD)) 0x00977480;
        auto EA__Thread__Mutex__Unlock_00977540 = (void(__fastcall*)(DWORD)) 0x00977540;
        auto FUN_MemoryPoolDebug_004ad7d0 = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, DWORD min, DWORD max)) 0x004ad7d0;
        auto VerifyPoolIntegrity_004ade80 = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, bool)) 0x004ade80;

        auto DAT_00b6d85c = 0x00b6d85c;

        bool crashDebuggingStarted = false;

        DWORD __fastcall GetAmountFree_004ac600_wrapper(DWORD this_) {
            DWORD puVar2;
            DWORD iVar1;
            DWORD ppFVar1;

            EA__Thread__Mutex__Lock_00977480(*(DWORD*)(this_ + 0x78), 0, DAT_00b6d85c);
            puVar2 = *(DWORD*)(this_ + 4);
            iVar1 = 0;
            while ((DWORD*)puVar2 != (DWORD*)(this_ + 4)) {
                ppFVar1 = puVar2 + 8;
                // game crashed here during a race
                // region related to memory

                if (
                    puVar2 == 0 || //puVar2 < 0x400000 || puVar2 > 0x40000000
                    IsBadReadPtr((DWORD*)puVar2, 1)
                    ) {
                    //BAD POINTER
                    if (!crashDebuggingStarted) {
                        crashDebuggingStarted = true;
                        cout << "Bad pointer in memory pool stuff\a\n";

                        //                FUN_MemoryPoolDebug_004ad7d0(this_, 0, 0, 100);
                        VerifyPoolIntegrity_004ade80(this_, 0, true);
                        crashDebuggingStarted = false;
                    }
                    break;
                }

                puVar2 = *(DWORD*)puVar2;
                if (iVar1 < *(DWORD*)ppFVar1) {
                    iVar1 = *(DWORD*)ppFVar1;
                }
            }
            EA__Thread__Mutex__Unlock_00977540(*(DWORD*)(this_ + 0x78));
            return iVar1;
        }
        */

    }

    //ok
    namespace imGuiStuff {


        static bool doDemoWindow = false;
        static bool doSunAndSkyWindow = false;

        //bool show_another_window = false
        //auto beginSceneHookDest = (void(__cdecl*)())0x0;//076a190;
        //void __cdecl beginSceneHook() {
        //
        //
        //    beginSceneHookDest();
        //}


        void InputInt(const char* className, char* collectionName, int* value, const char* name, int step = 1) {
            if (value == NULL) return;
            auto valueOld = *value;
            if (ImGui::InputInt(name, value, step, 100*step, ImGuiInputTextFlags_AutoSelectAll) && valueOld != *value) {
                modscript << "update_field " << className << " " << collectionName << " " << name << " " << *value << "\n";
                modscript.flush();
            }
        }
        void InputInt(const char* className, char* collectionName, Attrib::_Instance* inst, const char* name, int step = 1) {
            auto value = (int*)Attrib::Instance::GetAttributePointerFromString(inst, name, 0);
            InputInt(className, collectionName, value, name, step);
        }

        void InputFloat(const char* className, char* collectionName, float*value, const char* name, float step = 0.0f) {
            if (value == NULL) return;
            auto valueOld = *value;
            if (ImGui::InputFloat(name, value, step, 100.0f*step, "%.3f", ImGuiInputTextFlags_AutoSelectAll) && valueOld != *value) {
                modscript << "update_field " << className << " " << collectionName << " " << name << " " << *value << "\n";
                modscript.flush();
            }
        }
        void InputFloat(const char* className, char* collectionName, Attrib::_Instance* inst, const char* name, float step=0.0f) {
            auto value = (float*)Attrib::Instance::GetAttributePointerFromString(inst, name, 0);
            InputFloat(className, collectionName, value, name, step);
        }

        void InputVector4(const char* className, char* collectionName, float* value, const char* name, float step = 0.0f) {
            if (value == NULL) return;
            float valueOld[4];
            valueOld[0] = value[0];
            valueOld[1] = value[1];
            valueOld[2] = value[2];
            valueOld[3] = value[3];
            float step_fast = 100.0f * step;
            if (//ImGui::InputFloat4(name, value, "%.3f", ImGuiInputTextFlags_AutoSelectAll)
                ImGui::InputScalarN(name, ImGuiDataType_Float, value, 4, (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL), "%.3f", ImGuiInputTextFlags_AutoSelectAll)
                ) {
                if (valueOld[0] != value[0]) modscript << "update_field " << className << " " << collectionName << " " << name << " X " << value[0] << "\n";
                if (valueOld[1] != value[1]) modscript << "update_field " << className << " " << collectionName << " " << name << " Y " << value[1] << "\n";
                if (valueOld[2] != value[2]) modscript << "update_field " << className << " " << collectionName << " " << name << " Z " << value[2] << "\n";
                if (valueOld[3] != value[3]) modscript << "update_field " << className << " " << collectionName << " " << name << " W " << value[3] << "\n";
                modscript.flush();
            }
        }
        void InputVector4(const char* className, char* collectionName, Attrib::_Instance* inst, const char* name, float step = 0.0f) {
            auto value = (float*)Attrib::Instance::GetAttributePointerFromString(inst, name, 0);
            InputVector4(className, collectionName, value, name, step);
        }

        char* debugCarName = NULL;
        char* debugPresetName = NULL;

        void ImGuiFrame()
        {
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (doDemoWindow) ImGui::ShowDemoWindow(&doDemoWindow);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                //        static float f = 0.0f;
                //static int counter = 0;

                const float label_width_base = ImGui::GetFontSize() * 12;               // Some amount of width for label, based on font size.
                const float label_width_max = ImGui::GetContentRegionAvail().x * 0.40f; // ...but always leave some room for framed widgets.
                const float label_width = IM_MIN(label_width_base, label_width_max);
                ImGui::PushItemWidth(-label_width);                                     // Right-align: framed items will leave 'label_width' available for the label.

                if (!ImGui::GetIO().WantCaptureMouse) ImGui::SetWindowFocus(NULL);


                ImGui::Begin("french troles");
                ImGui::Text("Ctrl+Delete to hide these overlays");
                ImGui::Checkbox("Show ImGui Demo window", &doDemoWindow);
                ImGui::Checkbox("Show SunAndSky window", &doSunAndSkyWindow);

                if (*(DWORD*)DebugVehicleSelection != NULL) {
                    int count = (UINT)(*(UINT*)(*(DWORD*)DebugVehicleSelection + 0x20) - *(UINT*)(*(DWORD*)DebugVehicleSelection + 0x1c) >> 2);
                    auto piVar1 = (int*)(*(DWORD*)DebugVehicleSelection + 0x1c);
                    auto current = (int*)(*(DWORD*)DebugVehicleSelection + 0x18);

                    //auto uVar2 = (*current + 1) % count;

                    ImGui::Text("Debug Vehicle Selection");
                    //ImGui::Text("ID: "); ImGui::SameLine(); ImGui::InputInt("", current);

                    if (ImGui::BeginCombo("##DebugVehiclePicker", *(char**)(*piVar1 + *current * 4), ImGuiComboFlags_HeightLargest | ImGuiComboFlags_WidthFitPreview)) {
                        for (int n = 0; n < count; n++)
                            if (ImGui::Selectable(*(char**)(*piVar1 + n * 4), n == *current)) {
                                *current = n-1;
                                *current %= count;
                                *(char*)(DebugVehicleSelection + 1) = 1; //i hate DWORD + 1 being actually + 4 bytes
                            }
                        ImGui::EndCombo();
                    }

                    //ImGui::Text("Car Name: %s", *(char**)(*piVar1 + uVar2 * 4));
                    //if (ImGui::Button("Switch")) *(char*)(DebugVehicleSelection + 1) = 1; //i hate DWORD + 1 being actually + 4 bytes

                }

                //if (Overlay::di_keyboard != NULL) ImGui::Text("DInput8: Keyboard found");
                //if (Overlay::di_mouse != NULL) ImGui::Text("DInput8: Mouse found");

                //        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                //        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                //if (UserProfile_Player1 != NULL) ImGui::Text("Current camera: %d", *(ePlayerSettingsCameras*)(*UserProfile_Player1 + 0x4d5f0 + 0x7c + 0x20));
                //std::to_string(*(ePlayerSettingsCameras*)(*UserProfile_Player1 + 0x4d5f0 + 0x7c + 0x20))


                ImGuiIO& io = ImGui::GetIO(); (void)io;
                ImGui::Text("%.1f FPS/%.3f ms", io.Framerate, 1000.0f / io.Framerate);

                //ImGui::Text("mMainViewNumSubmissions = %d",         **((DWORD**)(*Renderer + 0x24))); //not right and doesnt do shit anyways
                //ImGui::Text("mMainViewNumPolys = %d",               (*(DWORD**)(*Renderer + 0x24))[1]);
                //ImGui::Text("mMainViewWorldNumSubmissions = %d",    (*(DWORD**)(*Renderer + 0x24))[2]);
                //ImGui::Text("mMainViewWorldNumPolys = %d",          (*(DWORD**)(*Renderer + 0x24))[3]);
                //ImGui::Text("mShadowViewNumSubmissions = %d",       (*(DWORD**)(*Renderer + 0x24))[4]);
                //ImGui::Text("mShadowViewNumPolys = %d",             (*(DWORD**)(*Renderer + 0x24))[5]);
                //ImGui::Text("mShadowViewWorldNumSubmissions = %d",  (*(DWORD**)(*Renderer + 0x24))[6]);
                //ImGui::Text("mEnvMapViewNumSubmissions = %d",       (*(DWORD**)(*Renderer + 0x24))[7]);
                //ImGui::Text("mEnvMapViewNumPolys = %d",             (*(DWORD**)(*Renderer + 0x24))[8]);



                ImGui::End();
            }

            if (doSunAndSkyWindow) {

                static bool preventZoneChange = false;
                static bool preventZoneChangeOld = false;

                static char curKeyBuf[64], desiredKeyBuf[64];
                static DWORD curKey, desiredKey;

                if (curKey != theSunAndSky->mCurrentKey) {
                    snprintf(curKeyBuf, 64, "0x%08X", theSunAndSky->mCurrentKey);
                    curKey = theSunAndSky->mCurrentKey;
                }
                if (desiredKey != theSunAndSky->mDesiredKey) {
                    snprintf(desiredKeyBuf, 64, "0x%08X", theSunAndSky->mDesiredKey);
                    desiredKey = theSunAndSky->mDesiredKey;
                }

                ImGui::Begin("SunAndSky");

                if (ImGui::TreeNode("SunAndSky")) {
                    if (ImGui::InputText("mCurrentKey", curKeyBuf, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
                        curKey = getVltHash(curKeyBuf);
                        theSunAndSky->mCurrentKey = curKey;
                    }
                    if (ImGui::InputText("mDesiredKey", desiredKeyBuf, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
                        desiredKey = getVltHash(desiredKeyBuf);
                        theSunAndSky->mDesiredKey = desiredKey;
                    }
                    ImGui::InputFloat("mBlendAmount", &theSunAndSky->mBlendAmount);

                    ImGui::Checkbox("Prevent automatic changing", &preventZoneChange);

                    if (preventZoneChange != preventZoneChangeOld) {
                        if (preventZoneChange) {
                            switch (CURRENT_GAME_VERSION) {
                            case UC_1_0:
                                utils::set<UINT32>(0x00875f44, 0x1046d966); //fld mBlendAmount (dword ptr [ESI + 0x10]) AKA do nothing
                                utils::nop(0x00875e91, 2); //prevent current = next
                                utils::nop(0x00875eec, 3); //prevent updating next
                                break;
                            case UC_1_21:
                                utils::set<UINT32>(0x00846826, 0x1046d966); //fld mBlendAmount (dword ptr [ESI + 0x10]) AKA do nothing
                                utils::nop(0x00846773, 2); //prevent current = next
                                utils::nop(0x008467ce, 3); //prevent updating next
                                break;
                            }
                        }
                        else {
                            switch (CURRENT_GAME_VERSION) {
                            case UC_1_0:
                                utils::set<UINT32>(0x00875f44, 0x0c2444d9); //fld transition percentage (dword ptr [ESP + 0xc])
                                utils::set<UINT16>(0x00875e91, 0x0689); //current = next
                                utils::set<UINT32>(0x00875eec, 0xeb044689); //update next
                                break;
                            case UC_1_21:
                                utils::set<UINT32>(0x00846826, 0x0c2444d9); //fld transition percentage (dword ptr [ESP + 0xc])
                                utils::set<UINT16>(0x00846773, 0x0689); //current = next
                                utils::set<UINT32>(0x008467ce, 0xeb044689); //update next
                                break;
                            }
                        }

                        preventZoneChangeOld = preventZoneChange;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Current zone_visual_style")) {
                    ImGui::Text("Warning: this may edit parent Attrib nodes!");
                    
                    Attrib::_Instance next; //Attrib::Gen::zone_visual_style
                    Attrib::_Instance current;

                    Attrib::Gen::zone_visual_style__constructor(&current, 0, theSunAndSky->mCurrentKey, 0);
                    if (current.mCollection == NULL) Attrib::Gen::zone_visual_style__constructor(&current, 0, DEFAULT, 0);
                    Attrib::Gen::zone_visual_style__constructor(&next, 0, theSunAndSky->mDesiredKey, 0);
                    if (next.mCollection == NULL) Attrib::Gen::zone_visual_style__constructor(&next, 0, DEFAULT, 0);


                    if (ImGui::TreeNode("Layout")) {
                        InputFloat("zone_visual_style", curKeyBuf, &((float*)current.mLayoutPtr)[0], "SunLatitude", 1.0f);
                        InputFloat("zone_visual_style", curKeyBuf, &((float*)current.mLayoutPtr)[1], "SkyDomeRotation", 1.0f);
                        ImGui::TreePop();
                    }

                    if (current.mCollection != NULL && ImGui::TreeNode("Collection")) {
                        InputFloat("zone_visual_style", curKeyBuf, &current, "SunOcclusionResponseRate", 0.01f);
                        InputInt("zone_visual_style", curKeyBuf, &current, "SkyDomeStyle");
                        InputInt("zone_visual_style", curKeyBuf, &current, "ColourCube");
                        InputFloat("zone_visual_style", curKeyBuf, &current, "SunLongitude", 1.0f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "VisibleLongitude", 1.0f);
                        InputVector4("zone_visual_style", curKeyBuf, &current, "SunColour", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "SunIntensity", 0.01f);
                        InputVector4("zone_visual_style", curKeyBuf, &current, "AmbientColour", 0.01f);
                        InputVector4("zone_visual_style", curKeyBuf, &current, "SHColour", 0.01f);
                        InputVector4("zone_visual_style", curKeyBuf, &current, "FogColour", 0.01f);
                        InputVector4("zone_visual_style", curKeyBuf, &current, "FogColourFar", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "FogNear",1.0f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "FogFar",1.0f);
                        InputVector4("zone_visual_style", curKeyBuf, &current, "SunHazeColour", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "SunHazeDistanceBias", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "FogExponent", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "SunHazeSize", 0.1f);
                        InputInt("zone_visual_style", curKeyBuf, &current, "LensFlareIndex");
                        InputVector4("zone_visual_style", curKeyBuf, &current, "SpecularColour", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "BloomIntensity", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "BloomThresold", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "BloomExponent", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "BloomRecoveryBias", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "BloomRecoveryScale", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "RoadReflectionIntensity", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "RoadReflectionLighting", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "RoadReflectionLightingPower", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "ToneMappingContrast", 0.01f);
                        InputFloat("zone_visual_style", curKeyBuf, &current, "ToneMappingSaturation", 0.01f);
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                
                if(Settings::useDayNightCycle && ImGui::TreeNode("UCE day/night cycle")) {
                    if (ImGui::InputInt("Cycle duration", &Settings::DayNightCycleDuration, 1, 100, ImGuiInputTextFlags_AutoSelectAll)) DayNight::timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[DayNight::currentWeatherSet].size();
                    ImGui::InputFloat("Time per zone", &DayNight::timePerZone, 1.0f, 100.0f, "%.3f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);
                    if (ImGui::InputInt("Current weather set", &DayNight::currentWeatherSet, 1, 1, ImGuiInputTextFlags_AutoSelectAll)) DayNight::currentWeatherSet %= Settings::VisualStyleCycles.size();
                    ImGui::TreePop();
                }

                ImGui::End();
            }

            //Autosculpt menu
            
        //    if (*FEAutosculptScreen != 0) {
        //
        //        auto part = *(DWORD*)(*FEAutosculptScreen + 0xb0);
        //
        //        auto numAutosculptZones = *(DWORD*)(*FEAutosculptScreen + 0xc0);
        //
        //        ImGui::Begin("Autosculpt Menu");
        //        ImGui::Text("%d Autosculpt Zones", numAutosculptZones);
        //        ImGui::Text(getLocalizedString( ((UINT(__fastcall*)(DWORD)) ((*(DWORD**)part)[7])) (part) )); //part name
        //
        //        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        //        if (ImGui::BeginTabBar("AutosculptTabs", tab_bar_flags))
        //        {
        //            for (int tabIdx = 0; tabIdx < *(DWORD*)(*FEAutosculptScreen + 0xbc); tabIdx++) {
        //                if (ImGui::BeginTabItem(*(char**)(*FEAutosculptScreen + 0xd0 + 4*tabIdx)))
        //                {
        //
        //                    *(DWORD*)(*FEAutosculptScreen + 0xb4) = *(DWORD*)(*FEAutosculptScreen + 0xc4 + 4 * tabIdx);
        //
        //                    char buf[64];
        //                    for (int i = 0; i < numAutosculptZones; i++) {
        //                        sprintf_s(buf, "CUST_AUTOSCULPT_ZONE%d", i + 1);
        //                        ImGui::Text(getLocalizedString(getBinHash(buf)));
        //                        ImGui::PushID(i);
        //                        ImGui::SliderFloat("", (float*)(part + ((*(DWORD*)(*FEAutosculptScreen + 0xb4) + 1) * 0xb + i) * 4), 0.0f, 1.0f);
        ////                        ImGui::SliderFloat("", (float*)(part + ((*(DWORD*)(*FEAutosculptScreen + 0xc4 + 4*tabIdx) + 1) * 0xb + i) * 4), 0.0f, 1.0f);
        //                        ImGui::PopID();
        //                    }
        //
        //
        //                    ImGui::EndTabItem();
        //                }
        //
        //
        //            }
        //            ImGui::EndTabBar();
        //        }
        //
        //        //pFVar1 = FEAutosculptScreen_012ab1b4;
        //        //FUN_005ccc10(&DAT_012ac348, 1);
        //        //AutosculptSelectablePart::3_SetZoneValue_005cd290
        //        //(pFVar1->autosculptPart_0xb0, pFVar1->selectedZone_0xb8, (float)autosculptValue,
        //        //    pFVar1->selectedPartOrKit_0xb4);
        //        //FUN_update_car_005d7840(&DAT_012ac348, '\0');
        //        //pFVar2 = get_FECarHeaven_005cc840();
        //        //pFVar2->field_0xac = 1;
        //
        //
        //        ImGui::End();
        //    }

            if (*FECarHeaven != NULL && CURRENT_GAME_VERSION == UC_1_21 && *DebugVehicleSelection != NULL) {

                if (*(DWORD*)0x012ab164 == NULL) {
                    *(DWORD*)0x012ab164 = MemoryAlloc(0x4000, "FEDebugCarTestStateManager", *MemBlock_Cars);
                    memset(*(void**)0x012ab164, 0, 0x4000);

                    auto RideInfo = (void(__fastcall*)(DWORD this_)) 0x00853cd0;
                    auto RideInfo_ConstructorCopy = (void(__fastcall*)(DWORD this_, DWORD, DWORD copyFrom, bool)) 0x008515b0;

                    //RideInfo((*(DWORD*)0x012ab164 + 0x80));
                    //RideInfo((*(DWORD*)0x012ab164 + 0x1d20));

                    RideInfo_ConstructorCopy((*(DWORD*)0x012ab164 + 0x80), 0, (*(DWORD*)(*(DWORD*)FECarHeaven + 0x8) + 0x10), true);
                    RideInfo_ConstructorCopy((*(DWORD*)0x012ab164 + 0x1d20), 0, (*(DWORD*)(*(DWORD*)FECarHeaven + 0x8) + 0x10), true);


                    //            *(DWORD*)(*(DWORD*)0x012ab164 + 0x39d0) = -1;
                    *(DWORD*)(*(DWORD*)0x012ab164 + 0x39d8) = MemoryAlloc(0x30, "FEDebugCarTestStateManager stuff", *MemBlock_Cars); //0x18
                    memset(*(void**)(*(DWORD*)0x012ab164 + 0x39d8), 0, 0x30);




                    DWORD CarsCount = (UINT)(*(UINT*)(*DebugVehicleSelection + 0x20) - *(UINT*)(*(DWORD*)DebugVehicleSelection + 0x1c) >> 2);
                    *(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) = MemoryAlloc(CarsCount, "FEDebugCarTestStateManager cars list", *MemBlock_Cars);
                    *(DWORD*)(*(DWORD*)0x012ab164 + 0x60) = *(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) + CarsCount * 4;

                    for (int i = 0; i < CarsCount; i++) {
                        auto carname = (*(char***)(*DebugVehicleSelection + 0x1c))[i];
                        std::cout << carname << endl;
                        (*(DWORD**)(*(DWORD*)0x012ab164 + 0x5c))[i] = getVltHash(carname);
                    }



                    //DWORD CarsCount = 330;
                    //*(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) = MemoryAlloc(CarsCount, "FEDebugCarTestStateManager cars list", *MemBlock_Cars);
                    //*(DWORD*)(*(DWORD*)0x012ab164 + 0x60) = *(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) + CarsCount * 4 - 4;
                    //
                    //FECarRecord* userProfileCars = (FECarRecord*)(*(DWORD*)0x0129ea64 + 0x88);
                    //
                    //for (int i = 0; i < 330; i++) {
                    //    DWORD carHash;
                    //    if (userProfileCars[i].preset != NULL) carHash = userProfileCars[i].preset;
                    //    else carHash = userProfileCars[i].pvehicle;
                    //
                    //    (*(DWORD**)(*(DWORD*)0x012ab164 + 0x5c))[i] = carHash;
                    //}
                    //seems all right but crasj



                    //(FECarRecord*)(*(DWORD*)0x0129ea64 + 0x88); //userprofile cars, FECarRecord[330]
                }

                ImGui::Begin("FE Debug Car Test");
                if (debugPresetName != NULL) {
                    ImGui::Text("Preset Name:");
                    ImGui::SameLine();
                    ImGui::Text(debugPresetName);
                }
                if (debugCarName != NULL) {
                    ImGui::Text("Car Name:");
                    ImGui::SameLine();
                    ImGui::Text(debugCarName);
                }
                if (0 <= *(DWORD*)(*(DWORD*)0x012ab164 + 0x74) && *(DWORD*)(*(DWORD*)0x012ab164 + 0x74) < 0x73) {
                    ImGui::Text("Part ID:");
                    ImGui::SameLine();
                    ImGui::Text(((char**)0x0126f35c)[3 * *(DWORD*)(*(DWORD*)0x012ab164 + 0x74)]);
                }
                ImGui::InputInt("BodykitType", (int*)(*(DWORD*)0x012ab164 + 0x6c));
                ImGui::InputInt("BodykitNumber", (int*)(*(DWORD*)0x012ab164 + 0x70));
                ImGui::InputInt("CurrentCar", (int*)(*(DWORD*)0x012ab164 + 0x39e4));




                //static bool b = true;
                //if (ImGui::Checkbox("Hijack PhotoMode", &b)) {
                //    *(DWORD*)(0x00cbb290) = -1; //PhotoMode menu, normally 0x52
                //    *(DWORD*)(0x00cbb038) = 0x52; //DebugCarTest, normally 0x26
                //}
                //else {
                //    *(DWORD*)(0x00cbb290) = 0x52; //PhotoMode menu, normally 0x52
                //    *(DWORD*)(0x00cbb038) = 0x26; //DebugCarTest, normally 0x26
                //}
                //if (ImGui::Button("ScrollNextCar")) ((void(__cdecl*)()) 0x0058b4b0)();
                //if (ImGui::Button("ScrollNextCarSlot")) ((void(__cdecl*)()) 0x0057f360)();
                //if (ImGui::Button("ScrollNextPresetRide")) ((void(__cdecl*)()) 0x0059a150)();
                //if (ImGui::Button("ScrollNextBodyKitType")) ((void(__cdecl*)()) 0x00569a10)();
                //if (ImGui::Button("ScrollNextPart")) ((void(__cdecl*)()) 0x005723d0)();
                //if (ImGui::Button("ScrollNextKit")) ((void(__cdecl*)()) 0x005723f0)();
                //if (ImGui::Button("ScrollNextBodyKitNumber")) ((void(__cdecl*)()) 0x00569a50)();
                ImGui::End();
            } //FEDebugCarTest fuckeries
        }

        




        //for debug car test
        DWORD __fastcall FeScreenManagerGetScreen_spoof(DWORD this_, DWORD edx, DWORD menuID) {
            return 0x012ab1c4;
        }
        void __stdcall SetDebugCarName2(char* p1, char* p2) {
            debugCarName = p1;
            debugPresetName = p2;
        }
        void __stdcall SetDebugCarName1(char* p1) {
            debugCarName = p1;
            debugPresetName = NULL;
        }




    }

    namespace brokenIdleCam {


        int lastMovement = 0;
        int camBackup = -1;

        int idleCamera = 5; //Staging/StagingSplit
        float idleCameraTimeout = 5.0f;

        auto EventSys__Event__Allocate = (DWORD(__cdecl*)(int size, int)) 0x0052c9b0;
        auto EHudFadeOff__constructor = (void(__fastcall*)(DWORD this_, DWORD, DWORD))0x0064daa0;
        auto LocalPlayer__SetHud = (void(__fastcall*)(DWORD this_, DWORD, DWORD)) 0x007896e0;


        auto CDActionDrive__Update_004d7490 = (void(__fastcall*)(DWORD this_, DWORD edx, DWORD time)) 0x004d7490;
        void __fastcall CDActionDrive__Update_wrapper(DWORD this_, DWORD edx, DWORD time) {
            if (*(float*)(*(DWORD*)(this_ + 0x34) + 0x78) < 0.1 &&
                *(float*)(*(DWORD*)(this_ + 0x34) + 0x7c) < 0.1 &&
                *(float*)(*(DWORD*)(this_ + 0x34) + 0x80) < 0.1) {
                //std::cout << "not moving all that much!!! " << (*timer_world - lastMovement) * 0.00025 << endl;

                if (camBackup == -1 && (*timer_world - lastMovement) * 0.00025 > idleCameraTimeout) {
                    //std::cout << "\n\n\nSWITCHING CAM\n\n\n";
                    (*DALOptions)->vftable->GetInt(*DALOptions, 0, DAL_OPTIONS_CAMERA, &camBackup, -1, -1, -1);
                    (*DALOptions)->vftable->SetInt(*DALOptions, 0, DAL_OPTIONS_CAMERA, idleCamera, -1, -1, -1);

                    //auto _pEVar26 = EventSys__Event__Allocate(0xc, 0);
                    //if (_pEVar26 != NULL) {
                    //    EHudFadeOff__constructor(_pEVar26, 0, 1);
                    //}
                    LocalPlayer__SetHud(*(DWORD*)0x012faa50, 0, 0);
                }
            }
            else {
                lastMovement = *timer_world;
                if (camBackup != -1) {
                    (*DALOptions)->vftable->SetInt(*DALOptions, 0, DAL_OPTIONS_CAMERA, camBackup, -1, -1, -1);
                    LocalPlayer__SetHud(*(DWORD*)0x012faa50, 0, 1);
                    camBackup = -1;
                }
            }

            CDActionDrive__Update_004d7490(this_, edx, time);
        }

    }

    //nothing
    namespace misc {
        //DWORD GCareerHub;
        //auto GHub_function = (void* (__fastcall*)(DWORD GHub)) 0x005f25a0; //0x006054e0;
        // 0x005f25a0 = force save

        //DWORD GGameStatus = 0x012f7e34;




        void testFunction() {
            std::cout << "Test function called ! \a\n";
            //GHub_function(GCareerHub);

            //*DebugSwitchCar = 1;
            //forceSpecificCar = (char*)"nis_350_z_05";




            //DWORD PlayerPVehicle = GetPlayerPVehicle();

            //_memccpy(_forceSpecificCar, *(char**)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4), 0, 31);

            //for (int i = 0; i < 32; i++) {
            //    _forceSpecificCar[i] = std::tolower(_forceSpecificCar[i]);
            //}

            //cout << "Player PVehicle at : " << (DWORD*)PlayerPVehicle << "\n";
            //cout << "Class at : " << *(DWORD*)(PlayerPVehicle + 0x1c) << "\n";
            //cout << "Carname at : " << *(DWORD*)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4) << "\n";
            //cout << "Carname : " << *(char**)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4) << "\n";
            //cout << "Player car detected : " << _forceSpecificCar << "\n";
        }

    }

    static UINT APT_AQUARIUS_MEDIUM;
    static UINT APT_AQUARIUS_MEDIUM_brk;
    static UINT APT_AQUARIUS_MEDIUM48;
    static UINT APT_AQUARIUS_EXTRABOLD;
    static UINT HELV_NEUE_STD_CN;
    static UINT HELV_NEUE_STD_CN_BLD;
    static UINT APT_AQUARIUS_EXTRABOLD_NUM;




    void doTests(uint32_t entry) {

        using namespace prints;
        using namespace quotes;
        using namespace regionCops;
        using namespace vinylsMenu;
        using namespace feCarsShopTroles;
        using namespace raceMusic;
        using namespace unrestrictedCarTextures;
        using namespace experimentalCrashFixes;
        using namespace imGuiStuff;
        using namespace brokenIdleCam;
        using namespace misc;


        modscript.open("generated.nfsms", std::ios_base::app | std::ios_base::out);

        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto time_buf = new char[26];
        ctime_s(time_buf, 26, &time);

        modscript << "\n#Generated script during session from " << time_buf << "\nThis script may not work as is!\n";

        //loading screen quotes randomizer (BROKEN)
        switch (entry) {
        case UC_1_0: //UC 1.0
            //utils::jump(0x0058d32d, QuotesDetour);
            //quotesDetourReturn = (void*)0x0058d35d;
            break;
        case UC_1_21: //UC 1.0.1.18
            //utils::jump(0x0057C00d, QuotesDetour);
            //quotesDetourReturn = (void*)0x0057c03d;
            break;
        default:
            break;
        }

        //other stuff
        switch (entry) {
        case UC_1_0: //UC 1.0
        {

            //fuck this game version it fucking sucks

            //utils::jump(0x005b5192, FECarsShopMenuDetour);
            //FECarsShopMenuDetourReturn = 0x005b52de;

            //UserProfile_Player1 = (DWORD*)0x00d8e1a4;

            //FUN_addCarToRegion_00518220 = (void(__fastcall*)(DWORD this_, DWORD unaff, DWORD, SavegameCar*)) 0x0052e050;
            //FUN_load_carlist_00506a80 = FUN_load_carlist_fixup_1_0;
            //vlt_loadArray_004609a0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*, DWORD)) 0x00466bb0;
            //FUN_getArraySize_0045ea80 = (short(__fastcall*)(DWORD * this_)) 0x00464c60;
            //vlt_getArrayElement_0045eaa0 = (DWORD * (__fastcall*)(DWORD * this_, DWORD unaff, DWORD)) 0x00464c80;
            //FUN_0045ced0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*)) 0x00463020;
            //vltget_pvehicle_00451d40 = vltget_pvehicle_00451d40_fixup_1_0;
            //vltget_frontend_005019b0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x00466b20;
            //FUN_00507030 = (bool(__fastcall*)(DWORD * this_)) 0x0051d340;
            //getVltValuePointer_0045e7d0 = (DWORD(__fastcall*)(DWORD * this_, DWORD unaff, DWORD field, DWORD arrayIndex)) 0x004649b0;
            //FUN_0045cbe0 = (DWORD(__fastcall*)(DWORD * this_)) 0x00462d20;
            //SavegameCar_update_00507610 = (void(__fastcall*)(SavegameCar * this_)) 0x0051d7c0;
            //FUN_vltnode_clearFromMem_00462550 = (void(__fastcall*)(DWORD * this_)) 0x00468820;
            //FUN_004628f0 = (void(__fastcall*)(DWORD * this_)) 0x00468bc0;
        }
            break;

        case UC_1_21: //UC 1.0.1.18

        {
            UserProfile_Player1 = (DWORD*)0x0129ea64;

            FUN_addCarToRegion_00518220 = (void(__fastcall*)(DWORD this_, DWORD unaff, DWORD, FECarRecord*)) 0x00518220;
            FUN_load_carlist_00506a80 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x00506a80;
            vlt_loadArray_004609a0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*, DWORD)) 0x004609a0;
            FUN_getArraySize_0045ea80 = (short(__fastcall*)(DWORD * this_)) 0x0045ea80;
            vlt_getArrayElement_0045eaa0 = (DWORD * (__fastcall*)(DWORD * this_, DWORD unaff, DWORD)) 0x0045eaa0;
            FUN_0045ced0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*)) 0x0045ced0;
            vltget_pvehicle_00451d40 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x00451d40;
            vltget_frontend_005019b0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD, DWORD)) 0x005019b0;
            FUN_00507030 = (bool(__fastcall*)(DWORD * this_)) 0x00507030;
            getVltValuePointer_0045e7d0 = (DWORD(__fastcall*)(DWORD * this_, DWORD unaff, DWORD field, DWORD arrayIndex))0x0045e7d0;
            FUN_0045cbe0 = (DWORD(__fastcall*)(DWORD * this_)) 0x0045cbe0;
            SavegameCar_update_00507610 = (void(__fastcall*)(FECarRecord * this_)) 0x00507610;
            FUN_vltnode_clearFromMem_00462550 = (void(__fastcall*)(DWORD * this_)) 0x00462550;
            FUN_004628f0 = (void(__fastcall*)(DWORD * this_)) 0x004628f0;




            //utils::jump(0x0040ed78, copPresetTest);
            //copPresetTestReturn = (void*)0x0040ed7f;

            //utils::jump(0x006b7aaa, copPresetTest2); //crasj
            //copPresetTest2Return = (void*)0x006b7ab7;

            //utils::jump(0x0040ee10, copPresetTest3);
            //copPresetTest3Return = (void*)0x0040ee17;

            //utils::jump(0x0040e790, interceptPresetrideSpawn);
            //interceptPresetrideSpawnReturn = (void*)0x0040e797;

            // skips pre-loading presetrides
            //utils::nop(0x00513ab1, 5); //DO.NOT.EVER.FUCKING.UNCOMMENT.THIS
            //utils::nop(0x0054193d, 5); //NECESSARY FOR CREATING A NEW SAVE (albeit maybe unoptimized because it loads every single fucking preset not just the relevant ones)





            //breaks things further instead of fixing them
            //APT_AQUARIUS_MEDIUM = 0x00bbac38;
            //APT_AQUARIUS_MEDIUM_brk = 0x00bbac20;
            //APT_AQUARIUS_MEDIUM48 = 0x00bbdc48;
            //APT_AQUARIUS_EXTRABOLD = 0x00bbac04;
            //HELV_NEUE_STD_CN = 0x00bbabe8;
            //HELV_NEUE_STD_CN_BLD = 0x00bbabc8;
            //APT_AQUARIUS_EXTRABOLD_NUM = 0x00bbaba8;

            //vanilla
            //utils::set<UINT>(0x01269fc8, APT_AQUARIUS_MEDIUM);
            //utils::set<UINT>(0x01269fcc, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fd0, APT_AQUARIUS_EXTRABOLD);
            //utils::set<UINT>(0x01269fd4, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fd8, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fdc, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fe0, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe4, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe8, HELV_NEUE_STD_CN_BLD);
            //utils::set<UINT>(0x01269fec, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269ff0, APT_AQUARIUS_EXTRABOLD_NUM);
            //utils::set<UINT>(0x01269ff4, APT_AQUARIUS_MEDIUM_brk);


            //utils::set<UINT>(0x01269fc8, APT_AQUARIUS_MEDIUM);
            //utils::set<UINT>(0x01269fcc, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fd0, APT_AQUARIUS_EXTRABOLD);
            //utils::set<UINT>(0x01269fd4, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fd8, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fdc, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fe0, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe4, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe8, HELV_NEUE_STD_CN_BLD);
            //utils::set<UINT>(0x01269fec, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269ff0, APT_AQUARIUS_EXTRABOLD_NUM);
            //utils::set<UINT>(0x01269ff4, APT_AQUARIUS_MEDIUM48);



            //utils::set<int>(0x01305338, 1); // enable -safemode



            //IMPLEMENTED IN THE UCE ASI
    //            utils::call(0x0084bc2a, getHeadlightsTexture);
    //            //HEADLIGHTS_storage = (char*) *((char*)0x013b32c0); //the game should load the texture there but cant access it ???
    //
    //            // REPLACE CARSHADOW VERSION
    //            utils::jump(0x0084948e, replaceCarshadowDetour);
    //            replaceCarshadowDetourReturn = (void*)0x008494fd;
    //
    //            //get raw flare status without damage
    ////            utils::jump(0x708243, 0x70826c);
    //            utils::set(0x708243, (char)0xEB); //jmp short
    //            utils::set(0x708244, (char)0x27); //jmp to 70826c
    //            utils::jump(0x00708847, getRawFlaresDetour);
    //
    //            //link emissive headlights to flare headlights
    //            utils::nop(0x007612d0, 6); //remove early assembler instruction in cBBFXCar::...
    //            utils::jump(0x007612ED, setEmissiveLightsDetour);
    //            setEmissiveLightsDetourReturn = (void*)0x00761300;
    //            utils::call(0x008491ed, lightsUpdateReplacement); //replace bullshit function
    //            utils::set<char>(0x0084b01f, 0x00); //fix stuck brakelights on car spawn (might be fixed by my other function anyways)
    //
    //            //automatic headlights
    //            //if (*(float*)(theSunAndSky + 0x70) > 0.09) headlightsOff(); //0x60 = SunColourR / 0x70 = AmbientColourR - thresold to adapt or toggle headlights depending on it or smth
    //            utils::call(0x006b6e1c, headlightsReplacement);
    //
    //            utils::nop(0x0072fca1, 22); //removes the CD drive check and prevents waking up all drives
    //            utils::nop(0x00693B14, 54); //removes another bootup check "nfsc.exe" / "nfsc_link.exe"
    //
    //            utils::jump(0x005b0947, HUDLoadedDetour);
    //            utils::call(0x005715bc, hashBINImpersonator);
    //
    //            utils::call(0x0085a15a, forcePlayerCarDetour);









                //if (utils::get<char>(0x006b6a44) != 0xc7) cout << "ExOpts detected\n";



                //utils::set(0x00826188, 0xFFFFFFFF); //changes rendering settings for carshadow

                //REPLACE UG2 FUNCTION VERSION - ONLY WORKS WITH THE SIGNATURE WITH JUST CARRENDERINFO (! - REVERSE ASSEMBLER BYTES IN 2- AND 4-BYTE UINTS)
                //utils::set<uint8_t>(0x008373b6, 0x57); //push edi
                //utils::call(0x008373b7, ReplaceCarShadow); //call
                //utils::set<uint16_t>(0x008373bc, 0xd8dd); //fstp st(0)
                //utils::set<uint8_t>(0x008373be, 0xE9); //jmp
                //utils::set<uint32_t>(0x008373bf, 0x000000D5); //jmp 00837498



                //utils::jump(0x0060c5ab, harvestGCareerHub);


                //utils::set<UINT>(0x007fed33, 0x19860); //vanilla size for Track Streaming Pool
                //utils::set<UINT>(0x007fed33, 0x100000); 





            //restore prints
            {

                //is it ok or does it need to be disabled again
                utils::jump(0x0090c209, restoreDisabledPrint0); //might also cause random crashes (or not ?)
                restoreDisabledPrintFixupReturn = (void*)0x0090c213;
                utils::call(0x009048cb, restoreDisabledPrintFixup);

                //utils::jump(0x006d9570, restoreDsbPrint_006d9570); //works SOMETIMES but hacky and crashy
                //utils::setFun(0x009e899c, aptDebugPrint); //replaces the apt debug print function

                //utils::set<char>(0x8104d1, 0x24); //apt sets Debug = 1
                //utils::set<char>(0x8104e5, 0x24); //apt sets CafeTrace = 1


                //lua ?
                utils::jump(0x00490591, somethingPrintName);

                utils::set(0x00bc3338, Fesl_Debug_replacement);
            }

            //replace prints
            {
                utils::call(0x0085865a, replacePrint_006d9570); //CarRenderInfo or smth
                utils::call(0x00858693, replacePrint_006d9570);
                utils::call(0x008587c8, replacePrint_006d9570);
                utils::call(0x008587fa, replacePrint_006d9570);

                utils::call(0x0040f790, replacePrint_006d9570); //GPS
                utils::call(0x00429966, replacePrint_006d9570); //SurveyManager
                utils::call(0x00438acc, replacePrint_006d9570); //AICopManager
                utils::call(0x0043cb85, replacePrint_006d9570); //AIThink
                utils::call(0x00441bc3, replacePrint_006d9570);
                utils::call(0x00441f41, replacePrint_006d9570);
                utils::call(0x00445d58, replacePrint_006d9570);
                utils::call(0x00445d66, replacePrint_006d9570);
                utils::call(0x00445d74, replacePrint_006d9570);
                utils::call(0x00449990, replacePrint_006d9570);
                utils::call(0x0044cf30, replacePrint_006d9570);
                utils::call(0x00450982, replacePrint_006d9570);
                utils::call(0x45b5d2, replacePrint_006d9570);
                utils::call(0x00523328, replacePrint_006d9570);//decompress
                utils::call(0x52a4d9, replacePrint_006d9570);
                utils::call(0x52a7f9, replacePrint_006d9570);
                utils::call(0x52c510, replacePrint_006d9570);
                utils::call(0x00540C88, printfSimpleCdecl);//memcard stuff
                utils::call(0x540CBC, printfSimpleCdecl);
                utils::call(0x540CF2, printfSimpleCdecl);
                utils::call(0x541956, printfSimpleCdecl);
                utils::call(0x5519B8, printfSimpleCdecl);
                utils::call(0x551A50, printfSimpleCdecl);
                utils::call(0x551A78, printfSimpleCdecl);
                utils::call(0x551AC7, printfSimpleCdecl);
                utils::call(0x551b2d, printfSimpleCdecl);
                utils::call(0x551BB8, printfSimpleCdecl);
                utils::call(0x551BF8, printfSimpleCdecl);
                utils::call(0x551C35, printfSimpleCdecl);
                utils::call(0x551C45, printfSimpleCdecl);
                utils::call(0x551C65, printfSimpleCdecl);
                utils::call(0x551CA5, printfSimpleCdecl);
                utils::call(0x551CF0, printfSimpleCdecl);
                utils::call(0x551D7A, printfSimpleCdecl);
                utils::call(0x551DCE, printfSimpleCdecl);
                utils::call(0x551E0E, printfSimpleCdecl);
                utils::call(0x551E6E, printfSimpleCdecl);
                utils::call(0x551E9D, printfSimpleCdecl);
                utils::call(0x551ED0, printfSimpleCdecl);
                utils::call(0x551F06, printfSimpleCdecl);
                utils::call(0x551F3F, printfSimpleCdecl);
                utils::call(0x551F70, printfSimpleCdecl);
                utils::call(0x551FA0, printfSimpleCdecl);
                utils::call(0x55F1E7, printfSimpleCdecl);
                utils::call(0x55F25B, printfSimpleCdecl);
                utils::call(0x55F2E8, printfSimpleCdecl);
                utils::call(0x55F6B9, printfSimpleCdecl);
                utils::call(0x55F6E5, printfSimpleCdecl);
                utils::call(0x55F8D4, printfSimpleCdecl);
                utils::call(0x5BA123, replacePrint_006d9570);
                ////maybe something around 5DC968
                utils::call(0x6012B4, replacePrint_006d9570);
                utils::call(0x6021CE, replacePrint_006d9570);
                utils::call(0x60CC0E, replacePrint_006d9570);
                utils::call(0x60CCC0, replacePrint_006d9570);

                utils::call(0x689616, replacePrint_006d9570);
                utils::call(0x689FF5, replacePrint_006d9570);
                utils::call(0x68B8AE, replacePrint_006d9570);
                utils::call(0x68E958, replacePrint_006d9570);
                utils::call(0x68FEE6, replacePrint_006d9570);
                utils::call(0x68FF02, replacePrint_006d9570);
                utils::call(0x691C1D, replacePrint_006d9570);

                utils::call(0x6CB115, replacePrint_006d9570);
                utils::call(0x6cbd39, replacePrint_006d9570);
                utils::call(0x6cbd7c, replacePrint_006d9570);
                utils::call(0x6cbdc0, replacePrint_006d9570);
                utils::call(0x6cc065, replacePrint_006d9570);
                utils::call(0x6cc4cc, replacePrint_006d9570);
                utils::call(0x6cc4f4, replacePrint_006d9570);
                utils::call(0x6cd98a, replacePrint_006d9570);
                utils::call(0x6cd9b1, replacePrint_006d9570);
                utils::call(0x6cdb18, replacePrint_006d9570);
                utils::call(0x6cddcf, replacePrint_006d9570);
                utils::call(0x6d016b, replacePrint_006d9570);
                utils::call(0x6d01ba, replacePrint_006d9570);
                utils::call(0x6d01e7, replacePrint_006d9570);
                utils::call(0x6d03b0, replacePrint_006d9570);
                utils::call(0x6d049b, replacePrint_006d9570);
                utils::call(0x6d04c1, replacePrint_006d9570);
                utils::call(0x6d04f7, replacePrint_006d9570);
                utils::call(0x6d05ba, replacePrint_006d9570);
                utils::call(0x6d0619, replacePrint_006d9570);

                //new
                utils::call(0x6d076e, replacePrint_006d9570);
                //utils::call(0x6d9570, replacePrint_006d9570);
                utils::call(0x6d0aa9, replacePrint_006d9570);
                utils::call(0x6d0ba4, replacePrint_006d9570);
                utils::call(0x6d0cf5, replacePrint_006d9570);
                utils::call(0x6d0e39, replacePrint_006d9570);
                utils::call(0x6d0ec2, replacePrint_006d9570);
                utils::call(0x6d1103, replacePrint_006d9570);
                utils::call(0x6d1161, replacePrint_006d9570);
                utils::call(0x6d118a, replacePrint_006d9570);
                utils::call(0x6d124a, replacePrint_006d9570);
                utils::call(0x6d12a9, replacePrint_006d9570);
                utils::call(0x6d144f, replacePrint_006d9570);
                utils::call(0x6d1551, replacePrint_006d9570);
                utils::call(0x6d166b, replacePrint_006d9570);
                utils::call(0x6d1752, replacePrint_006d9570);
                utils::call(0x6d1881, replacePrint_006d9570);
                utils::call(0x6d3f91, replacePrint_006d9570);
                utils::call(0x6d40b8, replacePrint_006d9570);
                utils::call(0x6d4170, replacePrint_006d9570);
                utils::call(0x6d41ca, replacePrint_006d9570);
                utils::call(0x6d430a, replacePrint_006d9570);
                utils::call(0x6d438a, replacePrint_006d9570);
                utils::call(0x6d43d5, replacePrint_006d9570);
                utils::call(0x6d7216, replacePrint_006d9570);
                ////maybe at 6db126
                //utils::call(0x6dbabd, replacePrint_006d9570);

                utils::call(0x708d42, replacePrint_006d9570);
                utils::call(0x70bedb, replacePrint_006d9570);
                utils::call(0x7141ad, replacePrint_006d9570);
                utils::call(0x84bc2a, replacePrint_006d9570); //CarRenderInfo::CarRenderInfo
                utils::call(0x854724, replacePrint_006d9570); //RideInfo::SetStockParts()
                utils::call(0x869825, replacePrint_006d9570);
                utils::call(0x873bb4, replacePrint_006d9570);
                //utils::setFun(0x01888504, replacePrint_006d9570); //rw::core::filesystem printer
                utils::call(0x981a39, printfSimpleCdecl);//not sure if these are 1st or 2nd print type
                utils::call(0x981ab0, replacePrint_006d9570);
                utils::call(0x981b10, replacePrint_006d9570);
                utils::call(0x981b56, replacePrint_006d9570);
                utils::call(0x981bc5, replacePrint_006d9570);
                utils::call(0x981c2a, replacePrint_006d9570);
                utils::call(0x981c9e, replacePrint_006d9570);
                utils::call(0x981fcc, replacePrint_006d9570);
                utils::call(0x982018, replacePrint_006d9570);
                utils::call(0x9820e6, replacePrint_006d9570);
                utils::call(0x982153, replacePrint_006d9570);
                utils::call(0x982199, replacePrint_006d9570);
                utils::call(0x9821fa, replacePrint_006d9570);
                utils::call(0x982268, replacePrint_006d9570);
                utils::call(0x98232a, replacePrint_006d9570);
                utils::call(0x982361, replacePrint_006d9570);
                utils::call(0x9823cc, replacePrint_006d9570);//end of not sure
                utils::call(0xb1adc9, replacePrint_006d9570);//unwind
                utils::call(0xb1b2a1, replacePrint_006d9570);
                utils::call(0xb1b2b3, replacePrint_006d9570);
                utils::call(0xb1b2c5, replacePrint_006d9570);
                utils::call(0xb1b331, replacePrint_006d9570);
                utils::call(0xb1b343, replacePrint_006d9570);
                utils::call(0xb21a39, replacePrint_006d9570);
                utils::call(0xb21ca9, replacePrint_006d9570);
                utils::call(0xb21f09, replacePrint_006d9570);
                utils::call(0xb21f39, replacePrint_006d9570);
                utils::call(0xb21fb9, replacePrint_006d9570);
                utils::call(0xb22019, replacePrint_006d9570);
                utils::call(0xb2207b, replacePrint_006d9570);
                utils::call(0xb2208d, replacePrint_006d9570);
                utils::call(0xb22339, replacePrint_006d9570);
                utils::call(0xb22369, replacePrint_006d9570);
                utils::call(0xb223f9, replacePrint_006d9570);
                utils::call(0xb24749, replacePrint_006d9570);
                utils::call(0xb2533C, replacePrint_006d9570);
                utils::call(0xB25351, replacePrint_006d9570);
                //a billion more AptMapItem
                utils::call(0xb26e69, replacePrint_006d9570);
                utils::call(0xb27C2A, replacePrint_006d9570);
                utils::call(0xB27C3F, replacePrint_006d9570);
                //...
                utils::call(0xb2fcc1, replacePrint_006d9570);
                //billion more unwinds... 

                utils::call(0x84bc2a, replacePrint_006d9570);
            }







            ////utils::nop(0x0057d0db, 6); //remove vinyl color restriction, useless
            //utils::nop(0x0057cdd7, 6); //remove vinyl fullbody restriction
            //utils::nop(0x0057d06e, 2); //remove vinyl mirror restriction

            //add items to the vinyl edit menu - entire function replacement
            utils::call(0x00588519, CreateVinylEditMenu);



            //utils::set<char>(0x00b77d2c, 'S'); //HideCarInfo
            //utils::set<char>(0x00b77d2d, 'h');
            //utils::set<char>(0x00b77d2e, 'o');
            //utils::set<char>(0x00b77d2f, 'w');

            //utils::set<char>(0x00b780dc, 'S'); //HideCashBalance
            //utils::set<char>(0x00b780dd, 'h');
            //utils::set<char>(0x00b780de, 'o');
            //utils::set<char>(0x00b780df, 'w');

            //utils::set<char>(0x00b77d94, 'S'); //HideCashCost
            //utils::set<char>(0x00b77d95, 'h');
            //utils::set<char>(0x00b77d96, 'o');
            //utils::set<char>(0x00b77d97, 'w');

            //utils::set<char>(0x00b77d6c, 'S'); //HideCurrentLevel
            //utils::set<char>(0x00b77d6d, 'h');
            //utils::set<char>(0x00b77d6e, 'o');
            //utils::set<char>(0x00b77d6f, 'w');

            //utils::set<char>(0x00b78128, 'S'); //HideHeatMeter
            //utils::set<char>(0x00b78129, 'h');
            //utils::set<char>(0x00b7812a, 'o');
            //utils::set<char>(0x00b7812b, 'w');

            //utils::set<char>(0x00b77d44, 'S'); //HidePerformanceBars
            //utils::set<char>(0x00b77d45, 'h');
            //utils::set<char>(0x00b77d46, 'o');
            //utils::set<char>(0x00b77d47, 'w');






            //racer count fuckeries
            //utils::set<char>(0x62AD02, 0x6C); //increase function stack size
            //utils::set<char>(0x62AE1F, 0x6C); //increase function stack size
            //other shit dont work


            /*
                    008547e7 5f              POP        EDI
                    008547e8 67 5e           POP        ESI
                    008547ea 5d              POP        EBP
                    008547eb b8 00 00        MOV        EAX, 0x0
                             00 00
                    008547f0 c2 08 00        RET        0x8
                                         ********** FUN_008547c0 Exit **********


            */


            //utils::set(0x0068fba2, MemPool_World_Size);
            //utils::set(0x005dd0c6, MemPool_World_Size);

            //utils::set(0x007fed33, MemPool_TS_Size);


            //utils::jump(0x005964fc, FECarsShopMenuDetour); //maybee deal with this from somewhere else ? add missing cars upon save load ?



            /*
            * custom parts list
            *
            * replace 3_FE_LoadAftermarketMenu_00588af0 to build a custom MenuTree
            *
            * vanilla menu as following
            *
            * CUST_OPTION_AFTERMARKET (0x92, parent 0x8E)
            * |- CUST_SLOT_BODY 0x93
            * |- CUST_SLOT_HOOD 0x94
            * |- CUST_SLOT_ALL_WHEELS 0x95
            * |- CUST_SLOT_FRONT_WHEELS 0x96
            * |- CUST_SLOT_REAR_WHEELS 0x97
            * |- CUST_SLOT_SPOILER 0x98
            * |- CUST_SLOT_EXHAUST 0x99
            *
            * custom menu following vlted data (field and stuff to decide)
            *
            * use for vlt car name hash :
            * dword car_PTR [4];
            * vltGet_car_004de2a0(&car_PTR,lowercaseVltHash_0045d950(GetCarTypeInfoFromHash(carNameHash)),0);
            * FUN_getCollectionFromPointer_005cd3c0(&car_parts_PTR,(uint *)((int)car_PTR.DataPointer_0x4 + 0xc),0);
            * vlt_loadArray_004609a0(&car_parts_PTR,array,arrayFieldHash);
            *
            *
            *
            * change FUN_getPartsList_00567c30 (open list for autosculpt or standard parts)
            * replace selectedButton_0x58 with the actual PartID (or PartID+offset or whatever, offset 0x100 would be okay (highest menu ID 0xDB))
            * replace 3_getSelectedPartID_0053e1f0 (return SelectedButton if using the method above)
            * change setPartCamera_0054e460
            *
            * load two parts : put PartIds next to each other (eg 0x1011 for both brakelights) and manage that in getPartsList
            *
            */
            //utils::call(0x00595021, LoadAftermarketMenu);

            //utils::call(0x0054e4a5, getSelectedPartID);
            //utils::call(0x00567c35, getSelectedPartID);

            //utils::call(0x00588fc3, getPartsList);

            //utils::call(0x005cfc9f, getAutosculptZone); //crash fix
            //utils::call(0x005d074d, getAutosculptZone);
            //utils::call(0x005d082e, getAutosculptZone);




            //utils::jump(0x004a5237, MusicLogic__Ready_detour); //sorta works but needs the nops then softlocks the game trying to go to Cars menu
            //probably misses an initialization of some kind
            //utils::nop(0x00468799, 2);
            //utils::nop(0x004687a3, 2);



            //            utils::jump(0x0052a585, TexList_detour);
            ////            cout << "textures list at " << &texturesList << "\n";
            //            utils::call(0x8442B2, loadCarTexturesReplacement);
            //            utils::call(0x858744, loadCarTexturesReplacement);
            //            utils::call(0x84827e, loadCarTexturesReplacement);
            //            utils::call(0x8460bb, loadCarTexturesReplacement);

                        //utils::jump(0x008461f3, disableTexListingDetour);
                        //utils::jump(0x0083d3d9, enableTexListingDetour);


                        //force per-part rendering in game world
            //            utils::nop(0x0084b2b2, 2); //maybe not needed

            //            utils::nop(0x0084bba1, 2); // force compiled rendering in FE
            //            utils::nop(0x0084bbac, 6); // force per part rendering in world (damage needs to be 



            //proof of concept
            //utils::jump(0x004786d5, SFXObj_Drivetrain_appendix);

            //linking
            //SFXObj_Drivetrain > CARSFX > SFX_BASE > SndBase >* EAXCar >* DriverInfo > RideInfo
            //                 0x0      0x0        0x0       0x1c      0x84          0x8
            //CarRenderConn > VehicleRenderConn >* RideInfo
            //             0x0                 0x34
            //use BluePrintType in RideInfo? or an unused LOD thing?

            //CarRenderConn::OnEvent ??





            //utils::call(0x004ac6b4, GetAmountFree_004ac600_wrapper);
            //utils::call(0x004ad86e, GetAmountFree_004ac600_wrapper);








            *(DWORD*)(0x00cbb290) = -1; //PhotoMode menu, normally 0x52
            *(DWORD*)(0x00cbb038) = 0x52; //DebugCarTest, normally 0x26

            utils::call(0x00551399, FeScreenManagerGetScreen_spoof);
            utils::call(0x005513e8, FeScreenManagerGetScreen_spoof);
            utils::call(0x00551435, FeScreenManagerGetScreen_spoof);
            utils::call(0x00551500, FeScreenManagerGetScreen_spoof);
            utils::call(0x0057931b, FeScreenManagerGetScreen_spoof);

            utils::jump(0x00551390, SetDebugCarName2);
            utils::jump(0x005513e0, SetDebugCarName1);

            //*(DWORD*)0x00cbb294 = 0x00b72dd8; //push DebugCarTest string instead of PhotoMode one (PhotoMode is 0x00b72af8)
            //*(DWORD*)0x00cbb298 = 0x0055ec90; //push DebugCarTest init function instead of PhotoMode one (PhotoMode is 0x0055fe20)

            //*(DWORD*)0x00cbb324 = 0x00b72dd8; //CodeEntry
            //*(DWORD*)0x00cbb328 = 0x0055ec90;



            //idle cam
            //CDActionDrive__Update_004d7490 = utils::set(0x00b6f790, CDActionDrive__Update_wrapper);

        }
            break;
        default:
            break;
        }

        std::cout << "UCE testing section enabled - built " << __DATE__ << " " << __TIME__ << "\n";
    }

}

void* Attrib::Instance::GetAttributePointerFromString(_Instance* this_, const char* key, DWORD stuff) {
    return Attrib::Instance::GetAttributePointer(this_, 0, getVltHash((char*) key), stuff);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
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

        //useful functions from the game (hashing...)
        switch (entry) {
        case UC_1_0: //UC 1.0
            getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x00463a80;
            getBinHash = (uint32_t(__cdecl*)(char* toHash)) 0x004b3ed0;
            MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x006a1d30;
            getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x0053cf30;

            MemBlock_Global = (DWORD*)0x00d3bd98;
            MemBlock_FE = (DWORD*)0x00d3bd9c;
            MemBlock_Cars = (DWORD*)0x00d3bdb0;

            GameFlow = (int*)0x00da57b8;
            Renderer = (int*)0x00ea00e4;
            FEScreenManager = (DWORD*)0x00d99354;
            UserProfile_Player1 = (DWORD*)0x00d8e1a4;
            FECarHeaven = (DWORD*)0x00d9b5d0;
            DebugVehicleSelection = (DWORD*)0x01336488;

            FEOptionsStateManager = (DWORD*)0x00d99388;
            FEPerformanceTuning = (DWORD*)0x00d99654;
            FEVisualPaint = (DWORD*)0x00d9a4b0;
            FEVinylPaintScreen = (DWORD*)0x00d9a4c0;
            FEAutosculptScreen = (DWORD*)0x00d9a4b4;

            DALOptions = (DALInterface**)0x00d2a194;

            theSunAndSky = (SunAndSky*)0x00d5ea40;

            timer_world = (int*)0x00da531c;

            Attrib::Gen::zone_visual_style__constructor = (void(__fastcall*)(Attrib::_Instance * this_, DWORD edx, DWORD node, DWORD stuff))0x0084f1b0;
            Attrib::Instance::GetAttributePointer = (void* (__fastcall*)(Attrib::_Instance * this_, DWORD edx, DWORD key, DWORD stuff))0x004649b0;
            Attrib::DefaultDataArea = (void* (__cdecl*)(DWORD length))0x00676490;
            break;
        case UC_1_21: //UC 1.0.1.18
            getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x0045cb10;
            getBinHash = (uint32_t(__cdecl*)(char* toHash)) 0x004aa050;
            MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x00686150;
            getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x005246d0;

            MemBlock_Global = (DWORD*)0x00cc9328;
            MemBlock_FE = (DWORD*)0x00cc932c;
            MemBlock_Cars = (DWORD*)0x00cc9340;

            GameFlow = (int*)0x012b64f8;
            Renderer = (int*)0x013b3164;
            FEScreenManager = (DWORD*)0x012aa044;
            UserProfile_Player1 = (DWORD*)0x0129ea64;
            FECarHeaven = (DWORD*)0x012ac318;
            DebugVehicleSelection = (DWORD*)0x01862c5c;

            FEOptionsStateManager = (DWORD*)0x012aa078;
            FEPerformanceTuning = (DWORD*)0x012aa344;
            FEVisualPaint = (DWORD*)0x012ab1b0;
            FEVinylPaintScreen = (DWORD*)0x012ab1c0;
            FEAutosculptScreen = (DWORD*)0x012ab1b4;


            DALOptions = (DALInterface**)0x00cb7194;

            theSunAndSky = (SunAndSky*)0x0126f100;

            timer_world = (int*)0x012b6450;

            Attrib::Gen::zone_visual_style__constructor = (void(__fastcall*)(Attrib::_Instance * this_, DWORD edx, DWORD node, DWORD stuff))0x00822a40;
            Attrib::Instance::GetAttributePointer = (void* (__fastcall*)(Attrib::_Instance * this_, DWORD edx, DWORD key, DWORD stuff))0x0045e7d0;
            Attrib::DefaultDataArea = (void* (__cdecl*)(DWORD length))0x0065abd0;
            break;
        default:
            std::cout << "Unsupported EXE, most tweaks will not be applied.\n";
            break;
        }

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
                if (Settings::SliderEnhancements) utils::set(0x005685bb, &fakeFrameTime);

                break;
            case UC_1_21:
                AutosculptSliderSpeedFactor = (float*)0x00cbad0c; //0.2
                OptionsSliderSpeedFactor = (float*)0x00cba9fc; //80
                PerfTuningSliderSpeedFactor = (float*)0x00cbabc8; //100
                FEVisualPaintSpeedFactor1 = (float*)0x00cbabd0; //150
                FEVisualPaintSpeedFactor2 = (float*)0x00cbabcc; //100
                EnableInterrupts = utils::call(0x0069060d, frameHook); //in EMainRender event manager
                if (Settings::SliderEnhancements) utils::set(0x0054f485, &fakeFrameTime);

                break;
            }
        }

        Settings::readConfig();

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

            //GetAmountFree = (DWORD(__fastcall*)(DWORD MemoryPool)) 0x004b6590;
            VerifyPoolIntegrity = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, bool)) 0x004b7e20;
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

            //GetAmountFree = (DWORD(__fastcall*)(DWORD MemoryPool)) 0x004ac600;
            VerifyPoolIntegrity = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, bool)) 0x004ade80;
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
        if (carsModuleInstalled && Settings::verbose) switch (entry) {
        case UC_1_21: //UC 1.0.1.18
            Verbose::__alloca_probe = (void*)0x0087b250;
            utils::jump(0x00a07820, Verbose::DebugPrintReplacement);
            break;
        case UC_1_0: //UC 1.0
            Verbose::__alloca_probe = (void*)0x008ae430;
            utils::jump(0x00a3a580, Verbose::DebugPrintReplacement);
            break;
        }
#endif //verbose

#if 1 // Day/night cycle
        if (carsModuleInstalled && Settings::useDayNightCycle) switch (entry) {
            using namespace DayNight;
        case UC_1_0: //UC 1.0
            timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[0].size();


            //updateFirstVisualStyleReturn = (void*)(0x00875df6);
            //updateNextVisualStyleReturn = (void*)(0x00875ef8);

            //dat_012fa474 = *(DWORD*)0x00de93dc;
            //dat_012fa46c = *(DWORD*)0x00de93d4;
            //dat_00b694d0 = *(DWORD*)0x00bd9c80;
            float_1 = (float*)*(DWORD*)0x00bd97b0; //1.0f constant

            //transitionPercentageDetourReturn = (void*)0x00875e69;
            //findTransitionPercentage = (void(__cdecl*)()) 0x00863fe0;

            //utils::jump(0x00875dea, updateFirstVisualStyle); //getting into free roam
            //utils::jump(0x00875eec, updateNextVisualStyle); //general update

            utils::call(0x00875de2, getNextVisualStyle); //replace Attrib::StringToKey
            utils::call(0x00875ee4, getNextVisualStyle); //replace Attrib::StringToKey

            //utils::jump(0x008720c0, findTransitionSpeedMult); //return 1.0f for transition duration (at least this was responsible for no transition when car no move)
            utils::call(0x00875e58, ret1f);
            utils::call(0x00875f25, getTransitionTime);
            //utils::jump(0x00875e64, transitionPercentageDetour); //force transition time

            break;
        case UC_1_21: //UC 1.0.1.18
            timePerZone = (float)Settings::DayNightCycleDuration/ Settings::VisualStyleCycles[0].size();

            //updateFirstVisualStyleReturn = (void*)(0x008466ee);
            //updateNextVisualStyleReturn = (void*)(0x008467da);

            //dat_012fa474 = *(DWORD*)0x012fa474;
            //dat_012fa46c = *(DWORD*)0x012fa46c;
            //dat_00b694d0 = *(DWORD*)0x00b694d0;
            float_1 = (float*)*(DWORD*)0x00b647b4;

            //transitionPercentageDetourReturn = (void*)0x00846756;
            //findTransitionPercentage = (void(__cdecl*)()) 0x008354c0; //VisualLookEffectTarget::Update

            //utils::jump(0x008466e2, updateFirstVisualStyle); //getting into free roam
            //utils::jump(0x008467ce, updateNextVisualStyle); //general update

            utils::call(0x008466da, getNextVisualStyle); //replace Attrib::StringToKey
            utils::call(0x008467c6, getNextVisualStyle); //replace Attrib::StringToKey

            //utils::jump(0x008428e0, findTransitionSpeedMult); //return 1.0f for transition duration (at least this was responsible for no transition when car no move)
            utils::call(0x00846745, ret1f);
            utils::call(0x00846807, getTransitionTime);
            //utils::jump(0x00846751, transitionPercentageDetour); //force transition time
            break;
        }
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
            //makeHeadlights
            tMatrix__TransformPosition = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x007436f0; //__cdecl indeed
            SDR__StartBatch = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x0076d8f0;
            SDR__SetTexture = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x0076d870;
            SDR__AddPoly = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x007816d0;
            SDR__EndBatch = (void* (__fastcall*)(DWORD this_))0x007a4970;
            WWorldPos__FindClosestFace = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, bVector4 * vec, char noRayCast)) 0x008963c0;
            normalizeVector_005d9950 = (void* (__cdecl*)(bVector4 * param_1, bVector4 * param_2)) 0x005f3560;
            WWorldPos__HeightAtPoint = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, bVector4 * vec)) 0x0088eb40;

            //lightsUpdateReplacement
            eReplacementTextures__constructor = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00a1f0d0;
            eReplacementTextures__EntryHandle__SetNewNameHash = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00539170;
            GetCarTypeInfoFromHash = (char* (__cdecl*)(DWORD param))0x00843630;

            //asm injector
            //DAT_00b647ec = (float*)0x;
            DrawAmbientShadow = (void*)0x00851d50;
            FUN_00520d70 = (void*)0x00538e10;
            FUN_00538df0 = (void*)0x00538df0;

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
            //makeHeadlights
            tMatrix__TransformPosition = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x0071b820; //__cdecl indeed
            SDR__StartBatch = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x00744290;
            SDR__SetTexture = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x00744220;
            SDR__AddPoly = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x00755d80;
            SDR__EndBatch = (void* (__fastcall*)(DWORD this_))0x00777f80;
            WWorldPos__FindClosestFace = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, bVector4 * vec, char noRayCast)) 0x00864ef0;
            normalizeVector_005d9950 = (void* (__cdecl*)(bVector4 * param_1, bVector4 * param_2)) 0x005d9950;
            WWorldPos__HeightAtPoint = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, bVector4 * vec)) 0x0085e900;

            //lightsUpdateReplacement
            eReplacementTextures__constructor = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00521090;
            eReplacementTextures__EntryHandle__SetNewNameHash = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00521060;
            GetCarTypeInfoFromHash = (char* (__cdecl*)(DWORD param))0x00817940;

            //asm injector
            //DAT_00b647ec = (float*)0x00b647ec;
            DrawAmbientShadow = (void*)0x00825430;
            FUN_00520d70 = (void*)0x00520d70;

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

            FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x005612f0;
            FE_getFeElement = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x00551dc0;
            AddToTree = (void* (__fastcall*)(DWORD, DWORD, LinkedNode*, DWORD)) 0x0056f520;
            FUN_00553aa0 = (void(__fastcall*)(LinkedNode * this_)) 0x00652910;

            StandardSelectablePart__GetPartsList_005d1b00 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD)) 0x005eb120;
            AutosculptSelectablePart__GetPartsList_005d40e0 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)) 0x005ed530;
            FUN_getLinkedListLength_004a76f0 = (DWORD(__fastcall*)(LinkedNode * this_, DWORD unaff, DWORD)) 0x004b0760;


            utils::call(0x005a99d1, LoadAftermarketMenu);

            utils::call(0x005676a5, getSelectedPartID);
            utils::call(0x00581545, getSelectedPartID);

            utils::call(0x0059f053, getPartsList);

            utils::call(0x005e95cf, getAutosculptZone); //crash fix
            utils::call(0x005ea07d, getAutosculptZone);
            utils::call(0x005ea15e, getAutosculptZone);

            break;
        case UC_1_21: //UC 1.0.1.18

            FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
            FE_getFeElement = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
            AddToTree = (void* (__fastcall*)(DWORD this_, DWORD unaff, LinkedNode*, DWORD)) 0x005563c0;
            FUN_00553aa0 = (void(__fastcall*)(LinkedNode * this_)) 0x00553aa0;

            StandardSelectablePart__GetPartsList_005d1b00 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD)) 0x005d1b00;
            AutosculptSelectablePart__GetPartsList_005d40e0 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)) 0x005d40e0;
            FUN_getLinkedListLength_004a76f0 = (DWORD(__fastcall*)(LinkedNode * this_, DWORD unaff, DWORD)) 0x004a76f0;


            utils::call(0x00595021, LoadAftermarketMenu);

            utils::call(0x0054e4a5, getSelectedPartID);
            utils::call(0x00567c35, getSelectedPartID);

            utils::call(0x00588fc3, getPartsList);

            utils::call(0x005cfc9f, getAutosculptZone); //crash fix
            utils::call(0x005d074d, getAutosculptZone);
            utils::call(0x005d082e, getAutosculptZone);
            break;
        }
#endif //FE aftermarket menu


        if (!CarRenderConn::MissShiftEffect) CarRenderConn::MissShiftEffect = getVltHash((char*)"MissShiftEffect");
        if (!CarRenderConn::NOSEffect) CarRenderConn::NOSEffect = getVltHash((char*)"NOSEffect");

        if (carsModuleInstalled) switch (entry) {
            using namespace CarRenderConn;
        case UC_1_21:
            if (Settings::HeadlightShatterFix) utils::call(0x0084260a, UpdateHeadlightAndBrakelightShatterEffects); //fix headlight break effect using the breakelight break fx (called from the previous function)
            if (Settings::EffectsEnhancements) {
                utils::call(0x00859c06, UpdateEffects);
                utils::set<uint32_t>(0x0085a23a, CarRenderConnNewLength); //CarRenderConn alloc size
                constructor = utils::call(0x0085a258, constructor_wrapper);
                OnLoaded = utils::set(0x00bbee00, OnLoaded_wrapper);



                TestVisibility = (DWORD(__fastcall*)(DWORD)) 0x00826bc0;
                AcidEffect__Disable = (void(__fastcall*)(void*)) 0x00756710;

                Effect__Fire = (void(__fastcall*)(DWORD * ths, DWORD edx, DWORD, DWORD, float, DWORD)) 0x00839270; //draws MissShiftEffect
                Effect__Update = (void(__fastcall*)(DWORD * ths, DWORD edx, DWORD, DWORD, DWORD, float, DWORD)) 0x008269d0;

                UpdateEngineEffects = (void(__fastcall*)(DWORD ths, DWORD edx, DWORD Pkt_Car_Service, DWORD)) 0x0082a120;
                UpdateWindowGlassShatterEffects_HD = (void(__fastcall*)(DWORD ths, DWORD edx, char)) 0x0082a050;
                FireParticles = (void(__fastcall*)(DWORD ths, DWORD edx, DWORD, DWORD)) 0x00829ec0;

                //constructor = (DWORD(__fastcall*)(DWORD CarRenderConn, DWORD edx, DWORD DrawCar, DWORD p2, DWORD p3)) 0x00859240;
                //OnLoaded = (void(__fastcall*)(DWORD CarRenderConn, DWORD edx, DWORD CarRenderInfo)) 0x00839620;

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

                TestVisibility = (DWORD(__fastcall*)(DWORD)) 0x008538a0;
                AcidEffect__Disable = (void(__fastcall*)(void*)) 0x007820f0;

                Effect__Fire = (void(__fastcall*)(DWORD * ths, DWORD edx, DWORD, DWORD, float, DWORD)) 0x00868040; //draws MissShiftEffect
                Effect__Update = (void(__fastcall*)(DWORD * ths, DWORD edx, DWORD, DWORD, DWORD, float, DWORD)) 0x008536b0;

                UpdateEngineEffects = (void(__fastcall*)(DWORD ths, DWORD edx, DWORD Pkt_Car_Service, DWORD)) 0x008576f0;
                UpdateWindowGlassShatterEffects_HD = (void(__fastcall*)(DWORD ths, DWORD edx, char)) 0x00857620;
                FireParticles = (void(__fastcall*)(DWORD ths, DWORD edx, DWORD, DWORD)) 0x00857490;

                //constructor = (DWORD(__fastcall*)(DWORD CarRenderConn, DWORD edx, DWORD DrawCar, DWORD p2, DWORD p3)) 0x;
                //OnLoaded = (void(__fastcall*)(DWORD CarRenderConn, DWORD edx, DWORD CarRenderInfo)) 0x;

                DistanceFactor = (float*)0x00cdfd34;
                DAT_0178c9f4 = (DWORD*)0x0127995c;
            }
            break;
        }

        if (Settings::tests) {
            ucetesting::doTests(entry);
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


