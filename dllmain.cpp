// dllmain.cpp : Définit le point d'entrée de l'application DLL.
/*
build paths
desktop
D:\Jeux\UCE 1.0.1.18\scripts\UndercoverExposed.asi

laptop

C:\Program Files (x86)\EA Games\Need for Speed Undercover\scripts\UndercoverExposed.asi

*/
#include "pch.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "SpeedReflectUtils.h"

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

int* GameFlow;
int* Renderer;
DWORD* FEScreenManager;

DWORD CurrentVisualStyle;

const DWORD zero = 0x00000000;

std::random_device rd;
std::mt19937 gen(rd());

struct Vector3 {
    float x;
    float y;
    float z;
};
struct Vector4 {
    float x;
    float y;
    float z;
    float w;
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
            *ret = (i == 1);
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
CrashFixes = 1                          //fixes various crashes, some specific to Exposed
IndexBufferSize = 0xA0000               //increases IndexBufferSize, vanilla = 0x50000
CarsPoolSize = 0x8000000                //increases Cars pool size, vanilla = 0x4e20000
FixTruckTrailers = 1                    //fixes tractor-trailer linking broken by Binary 2.8.3
LastGoodGlobalB = 1242129289

[Features: Miscellaneous]
KitRideHeightEnabled = 1                //enables per-kit ride height on some cars
SkipBootupChecks = 1                    //skips some unnecessary bootup checks

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
            }
            catch (exception) {

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
    void printInt(int i) {
        std::cout << "int=" << i << "\n";
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
    int* timer1;
    float timePerZone;

    int currentWeatherSet = 0;

    static uint32_t getNextVisualStyle() {

        float nextID = ((int)((*timer1) * 0.00025)) % (Settings::DayNightCycleDuration) / timePerZone;

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

    void* updateFirstVisualStyleReturn;
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
    DWORD dat_00b647b4;

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
        fld dword ptr[dat_00b647b4]; //1.0f static
        POP        ECX;
        RET;
    }}

    void* transitionPercentageDetourReturn;
    auto findTransitionPercentage = (void(__cdecl*)()) 0x008354c0;
    __declspec(naked) void transitionPercentageDetour() { __asm {
        mov eax, dword ptr[timePerZone]; //force transition time
        mov dword ptr[ecx + 0x14], eax;
        call findTransitionPercentage;
        jmp transitionPercentageDetourReturn;
    }}
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
    void* getVltValuePointer;
    __declspec(naked) void RideHeightDetour() {
        __asm {

            MOV        EAX, dword ptr[ESI + 0x40]
            TEST       EAX, EAX
            // check if the other class is empty, if yes cancels ride height calculation

            // ESI = this (VehicleRenderConn)

            MOV        ECX, dword ptr[ESI + 0x18]   // ECX = ecarValues
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
            call getVltValuePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

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
            call getVltValuePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

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
            call getVltValuePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

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
            call getVltValuePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)

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

    char* HEADLIGHTS_storage = 0;
    void getHeadlightsTexture() {
        if (HEADLIGHTS_storage != 0) return;
        auto hash = getBinHash((char*)"HEADLIGHTS");
        HEADLIGHTS_storage = getTexture(hash, 1, 0);
        if (HEADLIGHTS_storage != 0) {
            HEADLIGHTS_storage[0x38] = 0;
            HEADLIGHTS_storage[0x35] = 3;
        }
    }

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
    auto D3DXVec3Transform = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x0071b820; //__cdecl indeed
    auto FUN_reset_00744290 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x00744290;
    auto FUN_UV_00744220 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x00744220;
    auto draw_square = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x00755d80;
    auto FUN_00777f80 = (void* (__fastcall*)(DWORD this_))0x00777f80;
    auto terrain_FUN_00864ef0 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, Vector4 * vec, char noRayCast)) 0x00864ef0;
    auto normalizeVector_005d9950 = (void* (__cdecl*)(Vector4 * param_1, Vector4 * param_2)) 0x005d9950;
    auto terrain_FUN_0085e900 = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, Vector4 * vec)) 0x0085e900;

    //lightsUpdateReplacement
    auto FUN_00521090 = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00521090;
    auto FUN_00521060 = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00521060;
    auto getXNAME_string_00817940 = (char* (__cdecl*)(DWORD param))0x00817940;

    //asm injector
    float* DAT_00b647ec = (float*)0x00b647ec;
    auto drawCarShadow_00825430 = (void*)0x00825430;
    void* FUN_00520d70 = (void*)0x00520d70;

    void* FUN_00538df0 = (void*)0x00538df0; //1.0 shit


    DWORD CRIoff_electricsState = 0x5308;
    DWORD CRIoff_lightsDamage = 0x530c;
    DWORD CRIoff_lightsGlassDamage = 0x5310;
    DWORD CRIoff_headlightTextures = 0x52c8;
    DWORD CRIoff_headlightGlassTextures = 0x52d4;
    DWORD CRIoff_brakelightTextures = 0x52e0;
    DWORD CRIoff_brakelightGlassTextures = 0x52ec;
    DWORD CRIoff_this_00 = 0x8d4;
    DWORD CRIoff_renderSettings = 0x54c8;
    DWORD CRIoff_terrainAdj = 0x4b8;

    DWORD Class43cOff_VehType = 0xe44;

    void makePlaneMeshBuffer(float* mb) {
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
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000001)) headlightL = 1.0; //color = 0x809090; //headlight left
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000002)) headlightR = 1.0; //color = 0x809090; //headlight right
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000008)) taillightL = 1.0; //
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000010)) taillightR = 1.0; //
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000008 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000008)) brakelightL = 1.0; //brake
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000010 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000010)) brakelightR = 1.0; //brake
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000040) color = 0xf0f0f0; //reverse

        //if (headlightL == 0 && headlightR == 0 && brakelightL == 0 && brakelightR == 0) return;
        
        //float* matrix_6 = (float*)*(int*)(CarRenderInfo + 0x4f1c);
        char meshbuffer[0x94]; // Temporary buffer for makePlaneMeshBuffer

        Vector4 bumperTip = { 0, 0, 0, 0 }; //{ *(float*)(CarRenderInfo + 0x380), 0, 0, 0 };
        D3DXVec3Transform((float*)&bumperTip, carPositionMatrix, (float*)&bumperTip);
        Vector4 bumperTip_bis = { -bumperTip.y, bumperTip.z, bumperTip.x, 0 };
        terrain_FUN_00864ef0((CarRenderInfo + 0x3f0), 0, 0, &bumperTip_bis, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
        float terrainHeight = terrain_FUN_0085e900((CarRenderInfo + 0x3f0), 0, &bumperTip_bis); //-y, z, x !!!

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

                    D3DXVec3Transform(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                    x += headlightsRacer_length / (headlightsRacer_grid_length - 1);
                    v -= 1.0 / (headlightsRacer_grid_length - 1);
                }
                y += *(float*)(CarRenderInfo + 0x384) * headlightsRacer_widthMinFactor * 2 / (headlightsRacer_grid_width - 1);
            }

            // project to terrain
            if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
                for (int i = 0; i < headlightsRacer_grid_length; i++) for (int j = 0; j < headlightsRacer_grid_width; j++) {
                    //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                    Vector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                    terrain_FUN_00864ef0((int)(CarRenderInfo + 0x3f0), 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                    float height = terrain_FUN_0085e900((int)(CarRenderInfo + 0x3f0), 0, &vector_4d0);

                    if (height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
                }
            }


            // send to renderer
            makePlaneMeshBuffer((float*)meshbuffer);
            //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
            //printMeshBuffer((float*)meshbuffer);

            FUN_reset_00744290(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
            *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
            FUN_UV_00744220(*(DWORD*)(*Renderer + 0x40), 0, HEADLIGHTS_storage);

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

                    draw_square(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
                }
            }

            FUN_00777f80(*(DWORD*)(*Renderer + 0x40));
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

                    D3DXVec3Transform(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                    x -= taillightsRacer_length / (taillightsRacer_grid_length - 1);
                    v -= 1.0 / (taillightsRacer_grid_length - 1);
                }
                y += *(float*)(CarRenderInfo + 0x384) * taillightsRacer_widthMinFactor * 2 / (taillightsRacer_grid_width - 1);
            }

            // project to terrain
            if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
                for (int i = 0; i < taillightsRacer_grid_length; i++) for (int j = 0; j < taillightsRacer_grid_width; j++) {
                    //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                    Vector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                    terrain_FUN_00864ef0((int)(CarRenderInfo + 0x3f0), 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                    float height = terrain_FUN_0085e900((int)(CarRenderInfo + 0x3f0), 0, &vector_4d0);

                    if (height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
                }
            }


            // send to renderer
            makePlaneMeshBuffer((float*)meshbuffer);
            //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
            //printMeshBuffer((float*)meshbuffer);

            FUN_reset_00744290(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
            *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
            FUN_UV_00744220(*(DWORD*)(*Renderer + 0x40), 0, HEADLIGHTS_storage);

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

                    draw_square(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
                }
            }

            FUN_00777f80(*(DWORD*)(*Renderer + 0x40));
        }
    }

    void makeHeadlightsTraffic(int CarRenderInfo, float* carPositionMatrix, float* matrix_6) {
        int color = 0x809090; //BGR
        float alphaL = 0;
        float alphaR = 0;
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000001)) alphaL = 1.0; //color = 0x809090; //headlight left
        if (*((int*)(CarRenderInfo + CRIoff_electricsState)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_lightsDamage)) & 0x00000002)) alphaR = 1.0; //color = 0x809090; //headlight right
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000008) color = 0x202090; //brake
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000040) color = 0xf0f0f0; //reverse

        if (alphaR == 0 && alphaL == 0) return;

        //float* matrix_6 = (float*)*(int*)(CarRenderInfo + 0x4f1c);

        float position[headlightsTraffic_grid_length][headlightsTraffic_grid_width * 4];
        float texcoord[headlightsTraffic_grid_length][headlightsTraffic_grid_width * 2];
        char meshbuffer[0x94]; // Temporary buffer for makePlaneMeshBuffer

        Vector4 bumperTip = { 0, 0, 0, 0 }; //{ *(float*)(CarRenderInfo + 0x380), 0, 0, 0 };
        D3DXVec3Transform((float*)&bumperTip, carPositionMatrix, (float*)&bumperTip);
        Vector4 bumperTip_bis = { -bumperTip.y, bumperTip.z, bumperTip.x, 0 };
        terrain_FUN_00864ef0(CarRenderInfo + 0x3f0, 0, 0, &bumperTip_bis, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
        float terrainHeight = terrain_FUN_0085e900(CarRenderInfo + 0x3f0, 0, &bumperTip_bis); //-y, z, x !!!

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

                D3DXVec3Transform(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                x += headlightsTraffic_length / (headlightsTraffic_grid_length - 1);
                v -= 1.0 / (headlightsTraffic_grid_length - 1);
            }
            y += *(float*)(CarRenderInfo + 0x384) * headlightsTraffic_widthMinFactor * 2 / (headlightsTraffic_grid_width - 1);
        }

        // project to terrain
        if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
            for (int i = 0; i < headlightsTraffic_grid_length; i++) for (int j = 0; j < headlightsTraffic_grid_width; j++) {
                //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                Vector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                terrain_FUN_00864ef0(CarRenderInfo + 0x3f0, 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                float height = terrain_FUN_0085e900(CarRenderInfo + 0x3f0, 0, &vector_4d0);

                if (height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
            }
        }

        // send to renderer
        makePlaneMeshBuffer((float*)meshbuffer);
        //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
        //printMeshBuffer((float*)meshbuffer);

        FUN_reset_00744290(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
        *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
        FUN_UV_00744220(*(DWORD*)(*Renderer + 0x40), 0, HEADLIGHTS_storage);

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

                draw_square(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
            }
        }
        FUN_00777f80(*(DWORD*)(*Renderer + 0x40));

    }

    float __stdcall ReplaceCarShadow(DWORD CarRenderInfo, Vector4* param_3, float* carPositionMatrix, float* matrix_5, float* mat_6, int param_7) {
        if (*GameFlow != 6) return 1.0f;
        if (Settings::ProjectionCutoff && (*((float*)(CurrentVisualStyle + Settings::ProjectionCutoffField)) > Settings::ProjectionCutoffThresold)) return 1.0f;
        if (HEADLIGHTS_storage == 0) return 1.0f;

        //field CarRenderInfo + 43c has other data -> towards pvehicle ?
        switch (*(int*)(*(int*)(CarRenderInfo + 0x43c) + Class43cOff_VehType)) { //0 player 1 ? 2 racer 3 cop 4 traffic 5 heli 6 ? 7 ?
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
            CALL       drawCarShadow_00825430
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
            CALL       drawCarShadow_00825430
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

        DWORD* electricsState = (DWORD*)(CarRenderInfo + CRIoff_electricsState);
        DWORD* lightsDamage = (DWORD*)(CarRenderInfo + CRIoff_lightsDamage);
        DWORD* lightsGlassDamage = (DWORD*)(CarRenderInfo + CRIoff_lightsGlassDamage);
        DWORD* headlightTextures = (DWORD*)(CarRenderInfo + CRIoff_headlightTextures);
        DWORD* headlightGlassTextures = (DWORD*)(CarRenderInfo + CRIoff_headlightGlassTextures);
        DWORD* brakelightTextures = (DWORD*)(CarRenderInfo + CRIoff_brakelightTextures);
        DWORD* brakelightGlassTextures = (DWORD*)(CarRenderInfo + CRIoff_brakelightGlassTextures);
        void* this_00 = (void*)(CarRenderInfo + CRIoff_this_00);

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
        pvVar3 = FUN_00521090(this_00, 0, buffer, 1);
        FUN_00521060(pvVar3, 0, texHeadL); //headlights 

        pvVar3 = FUN_00521090(this_00, 0, buffer, 2);
        FUN_00521060(pvVar3, 0, texHeadR); //headlights (only one necessary)

        pvVar3 = FUN_00521090(this_00, 0, buffer, 3);
        FUN_00521060(pvVar3, 0, texHGlassL); //headlight glass

        pvVar3 = FUN_00521090(this_00, 0, buffer, 4);
        FUN_00521060(pvVar3, 0, texHGlassR); //headlight glass (only one necessary)

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

        if (*(int*)(getXNAME_string_00817940(**(DWORD**)(CarRenderInfo + 0x43c)) + 0xf4) != 2) {

            pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 5);
            FUN_00521060(pvVar3, 0, brakelightTextures[(texIDBrakeL == 2) ? 2 : 0]);

            pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 6);
            FUN_00521060(pvVar3, 0, brakelightTextures[(texIDBrakeR == 2) ? 2 : 0]);

            pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 7);
            FUN_00521060(pvVar3, 0, brakelightTextures[(texIDBrakeC == 2) ? 2 : 0]);

            pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 8);
            FUN_00521060(pvVar3, 0, brakelightGlassTextures[texIDBGlassL == 2 ? 2 : 0]);

            pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 9);
            FUN_00521060(pvVar3, 0, brakelightGlassTextures[texIDBGlassR == 2 ? 2 : 0]);

            pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 10);
            FUN_00521060(pvVar3, 0, brakelightGlassTextures[texIDBrakeC == 2 ? 2 : 0]);


            //manage headlights
            if ((*electricsState & 0x00000003)) { //&& !(lightsDamage & 0x00000003)
                *(int*)(CarRenderInfo + CRIoff_renderSettings) |= 0x100000;
            }
            else {
                *(int*)(CarRenderInfo + CRIoff_renderSettings) &= ~0x100000;
            }
            //if (((texIDHeadL != 2) && (texIDHeadR != 2)) && ((texIDHGlassL != 0 && (texIDHGlassR != 0))) && (*electricsState & 0x00000003)) { //shuts off the whole car when headlights broken, not the wanted effect !
            //    *(int*)(CarRenderInfo + 0x54c8) |= 0x100000;
            //}
            //else {
            //    *(int*)(CarRenderInfo + 0x54c8) &= ~0x100000;
            //}

            //manage brakelights
            if (((texIDBrakeL != 2) && (texIDBrakeR != 2)) && ((texIDBGlassL != 0 && (texIDBGlassR != 0)))) {
                *(int*)(CarRenderInfo + CRIoff_renderSettings) |= 0x200000;
            }
            else {
                *(int*)(CarRenderInfo + CRIoff_renderSettings) &= ~0x200000;
            }
            return;
        }

        pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 5);
        FUN_00521060(pvVar3, 0, brakelightTextures[texIDBrakeL]);

        pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 6);
        FUN_00521060(pvVar3, 0, brakelightTextures[texIDBrakeR]);

        pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 7);
        FUN_00521060(pvVar3, 0, brakelightTextures[texIDBrakeC]);

        pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 8);
        FUN_00521060(pvVar3, 0, brakelightGlassTextures[texIDBGlassL]);

        pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 9);
        FUN_00521060(pvVar3, 0, brakelightGlassTextures[texIDBGlassR]);

        pvVar3 = (void*)FUN_00521090(this_00, 0, buffer, 10);
        FUN_00521060(pvVar3, 0, brakelightTextures[texIDBrakeC]);

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

        if (*((float*)(CurrentVisualStyle + Settings::AutomaticHeadlightsField)) > (Settings::AutomaticHeadlightsThresold * (1.0 + *(int16_t*)(this_ + 0x860) / 327670.0))) {
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

namespace FirstCarCrash {
    bool doFirstCarCrashFix = false;

    // FROM EXTRA OPTIONS
    DWORD _PlayerIPlayer = 0x012faa80;
    DWORD _PlayerPVehicle = 0x012fa478;
    int* NumCurrentIVehicleElements = (int*)0x012fa474;
    DWORD GetPlayerPVehicle()
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

    DWORD DebugVehicleSelection = 0x01862c5c;
    char* DebugSwitchCar = (char*)0x01862c60;
    auto forcePlayerCar = (void(__fastcall*)(DWORD DebugVehicleSelection, DWORD something, char* carname, DWORD, DWORD, DWORD))0x00858ce0;
    char* forceSpecificCar = 0;
    char _forceSpecificCar[32];

    void __fastcall forcePlayerCarDetour(DWORD DebugVehicleSel, DWORD something, char* carname, DWORD, DWORD, DWORD) {
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

    void __cdecl HUDLoadedDetour() {
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

    DWORD __cdecl hashBINImpersonator(char* string) {
        DWORD result = getBinHash(string);
        if (result == 0x16E8EF0D) { //hash for "PINK_SLIP_CAR_1"
            doFirstCarCrashFix = true;
            cout << "Preparing to prevent first car crash\n";
        }
        return result;
    }


}

namespace FEAftermarketMenu {

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

    auto FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
    auto FE_getFeElement = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
    auto AddToTree = (void* (__fastcall*)(DWORD this_, DWORD unaff, LinkedNode*, DWORD)) 0x005563c0;
    auto FUN_00553aa0 = (void(__fastcall*)(LinkedNode * this_)) 0x00553aa0;

    auto StandardSelectablePart__GetPartsList_005d1b00 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD)) 0x005d1b00;
    auto AutosculptSelectablePart__GetPartsList_005d40e0 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)) 0x005d40e0;
    auto FUN_getLinkedListLength_004a76f0 = (DWORD(__fastcall*)(LinkedNode * this_, DWORD unaff, DWORD)) 0x004a76f0;

    void __cdecl LoadAftermarketMenu() {
        LinkedNode local_14;
        LinkedNode* current;
        FETreeDataNode* curTreeNode;

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
            return 0;
        case BUMPER_REAR:
            return 1;
        case SKIRT_LEFT:
        case SKIRT_RIGHT:
            return 2;
        case WHEEL_FRONT:
            return 4;
        case WHEEL_REAR:
            return 5;
        case FENDER_FRONT_LEFT:
        case FENDER_FRONT_RIGHT:
            return 10;
        case BODY:
            return 12;
        case HOOD:
            return 13;
        case SPOILER:
            return 14;
        case ROOFSCOOP:
            return 15;
        case ROOF:
            return 16;
        case EXHAUST:
            return 17;
        case BUMPER_FRONT_EXTRA:
            return 29;
        default:
            return 3; //40 default but causes crashes
        }
    }

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

        bool carsModuleInstalled = true;
    
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

        // user information about the vinyls mess

        struct stat vinFileMeta;
        stat("../CARS/VINYLS/VINYLS.BIN", &vinFileMeta);

        if (vinFileMeta.st_mtime > 1240521921) {
            //            std::cout << "UCE: Modded VINYLS.BIN file.\n"; // 1240421921 is 1.0.1.18
            struct stat SRMeta;
            if (stat("SpeedReflect.asi", &SRMeta) == -1) {
                MessageBoxA(NULL, "Critical: SpeedReflect ASI not present!\nYour game will crash due to being unable to load any vinyls. Please add SpeedReflect in your scripts folder or revert the CARS\\VINYLS\\VINYLS.BIN file to vanilla!", "Undercover Exposed", MB_ICONERROR);
            }
            else { //SpeedReflect present
                switch (entry) {
                case 0x008AEC55: //UC 1.0
                    break;
                case 0x0087ba75: //UC 1.0.1.18
                    if (SRMeta.st_mtime < 1741945898) MessageBoxA(NULL, "Critical: This version of SpeedReflect ASI doesn't support this game version!\nYour game will crash due to being unable to load any vinyls, please use the modified SpeedReflect provided with UCE.", "Undercover Exposed", MB_ICONERROR);
                    break;
                default:
                    MessageBoxA(NULL, "Critical: Game version not supported by the SpeedReflect ASI!\nYour game will crash due to being unable to load any vinyls.\nPlease use either the 10.0MB 1.0.0.1 EXE or the 21.6MB 1.1.2.1/1.0.1.18 EXE or revert the CARS\\VINYLS\\VINYLS.BIN file to vanilla.", "Undercover Exposed", MB_ICONERROR);
                }
            }
        }

        //useful functions from the game (hashing...)
        switch (entry) {
        case 0x008AEC55: //UC 1.0
            getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x00463a80;
            getBinHash = (uint32_t(__cdecl*)(char* toHash)) 0x004b3ed0;
            MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x006a1d30;
            getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x0053cf30;

            MemBlock_Global = (DWORD*)0x00d3bd98;
            MemBlock_FE = (DWORD*)0x00d3bd9c;
            GameFlow = (int*)0x00da57b8;
            Renderer = (int*)0x00ea00e4;
            FEScreenManager = (DWORD*)0x00d99354;

            CurrentVisualStyle = 0x00d5ea40;
            break;
        case 0x0087ba75: //UC 1.0.1.18
            getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x0045cb10;
            getBinHash = (uint32_t(__cdecl*)(char* toHash)) 0x004aa050;
            MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x00686150;
            getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x005246d0;

            MemBlock_Global = (DWORD*)0x00cc9328;
            MemBlock_FE = (DWORD*)0x00cc932c;
            GameFlow = (int*)0x012b64f8;
            Renderer = (int*)0x013b3164;
            FEScreenManager = (DWORD*)0x012aa044;

            CurrentVisualStyle = 0x0126f100;
            break;
        default:
            std::cout << "Unsupported EXE, most tweaks will not be applied.\n";
            break;
        }

        Settings::readConfig();

        if (Settings::CrashFixes) switch (entry) {
        case 0x008AEC55: //UC 1.0
        //remove more kamikaze code
            utils::nop(0x004b4c40, 10); //
            utils::set<char>(0x00883a25, 0xeb); //JMP instead of JNZ
            utils::nop(0x00881d00, 9);
            break;
        case 0x0087ba75: //UC 1.0.1.18
        //remove more kamikaze code
            utils::nop(0x004aacf0, 10); //
            utils::set<char>(0x008547e5, 0xeb); //JMP instead of JNZ
            utils::nop(0x00852af0, 9);

            //first car crash fix (only for 1.0.1.18 - contains hardcoded function addresses !)
            utils::jump(0x005b0947, FirstCarCrash::HUDLoadedDetour);
            utils::call(0x005715bc, FirstCarCrash::hashBINImpersonator);
            utils::call(0x0085a15a, FirstCarCrash::forcePlayerCarDetour);
            break;
        }

        if (Settings::SkipBootupChecks) switch (entry) {
        case 0x008AEC55: //UC 1.0
            utils::nop(0x00757981, 22); //removes the CD drive check and prevents waking up all drives
            utils::nop(0x006af1d4, 54); //removes another bootup check "nfsc.exe" / "nfsc_link.exe"
            break;
        case 0x0087ba75: //UC 1.0.1.18
            utils::nop(0x0072fca1, 22); //removes the CD drive check and prevents waking up all drives
            utils::nop(0x00693B14, 54); //removes another bootup check "nfsc.exe" / "nfsc_link.exe"
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
                                if (strcmp(carName, "TRF_TRA_BOX_83") == 0 || strcmp(carName, "TRF_TRA_FLT_84") == 0 || strcmp(carName, "TRF_TRA_LOG_84") == 0) {
                                    blockData[carBlockOffset + 393] = 0x04;
                                    blockData[carBlockOffset + 553] = 0x05;
                                    blockData[carBlockOffset + 697] = 0x05;
                                }
                                if (strcmp(carName, "TRF_TRK_SEM_95") == 0) {
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
       

#if 1 // 54k crash fix
        if (carsModuleInstalled && Settings::CrashFixes) switch (entry) {
        case 0x008AEC55: //UC 1.0
            GeomAlloc::loopStart = (void*)0x007a4110;
            GeomAlloc::allocateNewBlock = (void*)0x0079dc60;
            GeomAlloc::calculation = (void*)0x0079b8d0;
            GeomAlloc::forceSkipExit = (void*)0x007a416e;
            utils::jump(0x007a4100, GeomAlloc::CounterReset);
            utils::jump(0x007a4167, GeomAlloc::AllocOrSkip);
            break;
        case 0x0087ba75: //UC 1.0.1.18
            GeomAlloc::loopStart = (void*) 0x00777720;
            GeomAlloc::allocateNewBlock = (void*) 0x00771df0;
            GeomAlloc::calculation = (void*) 0x0076fe00;
            GeomAlloc::forceSkipExit = (void*) 0x0077777e;
            utils::jump(0x00777710, GeomAlloc::CounterReset);
            utils::jump(0x00777777, GeomAlloc::AllocOrSkip);

            //cant do any wrong but probably doesnt work

            utils::nop(0x00852af4, 5);

            utils::set(0x008547e7, 0x5f); //POP EDI
            utils::set(0x008547e8, 0x67); //POP ESI
            utils::set(0x008547e9, 0x5e); //
            utils::set(0x008547ea, 0x5d); //POP EBP
            utils::set(0x008547eb, 0xb8); //MOV EAX, 0x0
            utils::set(0x008547ec, 0x00000000); //
            utils::set(0x008547f0, 0xc2); //RET 0x8
            utils::set(0x008547f1, 0x08); //
            utils::set(0x008547f2, 0x00); //

            break;
        default:
            std::cout << "Unsupported EXE, over polycount limit crash fix not applied.\n";
        }
        if (carsModuleInstalled && Settings::IndexBufferSize != 0x50000) switch (entry) {
        case 0x008AEC55: //UC 1.0
            //alloc function
            utils::set<int>(0x0079dcbb, Settings::IndexBufferSize);
            utils::set<int>(0x0079dcde, Settings::IndexBufferSize);
            utils::set<int>(0x0079dd06, Settings::IndexBufferSize);
            utils::set<int>(0x0079dd2c, Settings::IndexBufferSize);

            //other function with the same value
            utils::set<int>(0x0077c307, Settings::IndexBufferSize);
            utils::set<int>(0x0077c329, Settings::IndexBufferSize);
            utils::set<int>(0x0077c350, Settings::IndexBufferSize);
            //utils::set<int>(0x018fde44, 0x100000);
            break;
        case 0x0087ba75: //UC 1.0.1.18
            //alloc function
            utils::set<int>(0x00771e4b, Settings::IndexBufferSize);
            utils::set<int>(0x00771e6e, Settings::IndexBufferSize);
            utils::set<int>(0x00771e96, Settings::IndexBufferSize);
            utils::set<int>(0x00771ebc, Settings::IndexBufferSize);

            //other function with the same value
            utils::set<int>(0x00751117, Settings::IndexBufferSize);
            utils::set<int>(0x00751139, Settings::IndexBufferSize);
            utils::set<int>(0x00751160, Settings::IndexBufferSize);
            //utils::set<int>(0x018fde44, 0x100000);
            break;
        }
#endif // 54k crash fix

#if 1
        if (carsModuleInstalled && Settings::CarsPoolSize != 0x4e20000) switch (entry) {
        case 0x008AEC55: //UC 1.0
            utils::set(0x0069e819, Settings::CarsPoolSize);
            utils::set(0x0069e828, Settings::CarsPoolSize);
            utils::set(0x0069e888, Settings::CarsPoolSize);
            utils::set(0x005f77ac, Settings::CarsPoolSize);
            break;
        case 0x0087ba75: //UC 1.0.1.18
            utils::set(0x005dd0dc, Settings::CarsPoolSize);
            utils::set(0x00683188, Settings::CarsPoolSize);
            utils::set(0x00683128, Settings::CarsPoolSize);
            utils::set(0x00683119, Settings::CarsPoolSize);
            break;
        }
#endif //cars pool increase

#if 1 // verbose
        if (carsModuleInstalled && Settings::verbose) switch (entry) {
        case 0x0087ba75: //UC 1.0.1.18
            Verbose::__alloca_probe = (void*)0x0087b250;
            utils::jump(0x00a07820, Verbose::DebugPrintReplacement);
            break;
        case 0x008AEC55: //UC 1.0
            Verbose::__alloca_probe = (void*)0x008ae430;
            utils::jump(0x00a3a580, Verbose::DebugPrintReplacement);
            break;
        }
#endif //verbose

#if 1 // Day/night cycle
        if (carsModuleInstalled && Settings::useDayNightCycle) switch (entry) {
            using namespace DayNight;
        case 0x008AEC55: //UC 1.0
            timer1 = (int*)0x00da5654;
            timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[0].size();


            updateFirstVisualStyleReturn = (void*)(0x00875df6);
            updateNextVisualStyleReturn = (void*)(0x00875ef8);

            dat_012fa474 = *(DWORD*)0x00de93dc;
            dat_012fa46c = *(DWORD*)0x00de93d4;
            //dat_00b694d0 = *(DWORD*)0x00bd9c80;
            dat_00b647b4 = *(DWORD*)0x00bd97b0;

            transitionPercentageDetourReturn = (void*)0x00875e69;
            findTransitionPercentage = (void(__cdecl*)()) 0x00863fe0;

            utils::jump(0x00875dea, updateFirstVisualStyle); //getting into free roam
            utils::jump(0x00875eec, updateNextVisualStyle); //general update
            utils::jump(0x008720c0, findTransitionSpeedMult); //return 1.0f for transition duration (at least this was responsible for no transition when car no move)
            utils::jump(0x00875e64, transitionPercentageDetour); //force transition time


            break;
        case 0x0087ba75: //UC 1.0.1.18
            timer1 = (int*)0x012b6450;
            timePerZone = (float)Settings::DayNightCycleDuration/ Settings::VisualStyleCycles[0].size();

            updateFirstVisualStyleReturn = (void*)(0x008466ee);
            updateNextVisualStyleReturn = (void*)(0x008467da);

            dat_012fa474 = *(DWORD*)0x012fa474;
            dat_012fa46c = *(DWORD*)0x012fa46c;
            //dat_00b694d0 = *(DWORD*)0x00b694d0;
            dat_00b647b4 = *(DWORD*)0x00b647b4;

            transitionPercentageDetourReturn = (void*)0x00846756;
            findTransitionPercentage = (void(__cdecl*)()) 0x008354c0;

            utils::jump(0x008466e2, updateFirstVisualStyle); //getting into free roam
            utils::jump(0x008467ce, updateNextVisualStyle); //general update
            utils::jump(0x008428e0, findTransitionSpeedMult); //return 1.0f for transition duration (at least this was responsible for no transition when car no move)
            utils::jump(0x00846751, transitionPercentageDetour); //force transition time


            break;
        }
#endif // Day/night cycle

#if 1 // Per kit ride height
        if (carsModuleInstalled && Settings::KitRideHeight) switch (entry) {
            using namespace KitRideHeight;
            case 0x0087ba75: //UC 1.0.1.18

                getVltValuePointer = (void*)0x0045e7d0;

                RideHeightDetourReturn = (void*)0x0082a3b8;
                utils::jump(0x0082a37d, RideHeightDetour);

                FERideHeightDetourReturn = (void*)0x008141ab;
                utils::jump(0x00814187, FERideHeightDetour);
                break;
            case 0x008AEC55: //UC 1.0

                getVltValuePointer = (void*)0x004649b0;

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
        case 0x008AEC55: //UC 1.0
            //makeHeadlights
            D3DXVec3Transform = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x007436f0; //__cdecl indeed
            FUN_reset_00744290 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x0076d8f0;
            FUN_UV_00744220 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x0076d870;
            draw_square = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x007816d0;
            FUN_00777f80 = (void* (__fastcall*)(DWORD this_))0x007a4970;
            terrain_FUN_00864ef0 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, Vector4 * vec, char noRayCast)) 0x008963c0;
            normalizeVector_005d9950 = (void* (__cdecl*)(Vector4 * param_1, Vector4 * param_2)) 0x005f3560;
            terrain_FUN_0085e900 = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, Vector4 * vec)) 0x0088eb40;

            //lightsUpdateReplacement
            FUN_00521090 = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00a1f0d0;
            FUN_00521060 = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00539170;
            getXNAME_string_00817940 = (char* (__cdecl*)(DWORD param))0x00843630;

            //asm injector
            //DAT_00b647ec = (float*)0x;
            drawCarShadow_00825430 = (void*)0x00851d50;
            FUN_00520d70 = (void*)0x00538e10;
            FUN_00538df0 = (void*)0x00538df0;

            CRIoff_electricsState = 0x52f8;
            CRIoff_lightsDamage = 0x52fc;
            CRIoff_lightsGlassDamage = 0x5300;
            CRIoff_headlightTextures = 0x52b8;
            CRIoff_headlightGlassTextures = 0x52c4;
            CRIoff_brakelightTextures = 0x52d0;
            CRIoff_brakelightGlassTextures = 0x52dc;
            CRIoff_this_00 = 0x8c8;
            CRIoff_renderSettings = 0x54b8;
            CRIoff_terrainAdj = 0x4ac;
            CRIoff_MPointsArrayPtr = 0x4b4;

            Class43cOff_VehType = 0xad0;

            //projection
            utils::call(0x0087b23b, getHeadlightsTexture);
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
        case 0x0087ba75: //UC 1.0.1.18
            //makeHeadlights
            D3DXVec3Transform = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x0071b820; //__cdecl indeed
            FUN_reset_00744290 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x00744290;
            FUN_UV_00744220 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x00744220;
            draw_square = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x00755d80;
            FUN_00777f80 = (void* (__fastcall*)(DWORD this_))0x00777f80;
            terrain_FUN_00864ef0 = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, Vector4 * vec, char noRayCast)) 0x00864ef0;
            normalizeVector_005d9950 = (void* (__cdecl*)(Vector4 * param_1, Vector4 * param_2)) 0x005d9950;
            terrain_FUN_0085e900 = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, Vector4 * vec)) 0x0085e900;

            //lightsUpdateReplacement
            FUN_00521090 = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00521090;
            FUN_00521060 = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00521060;
            getXNAME_string_00817940 = (char* (__cdecl*)(DWORD param))0x00817940;

            //asm injector
            //DAT_00b647ec = (float*)0x00b647ec;
            drawCarShadow_00825430 = (void*)0x00825430;
            FUN_00520d70 = (void*)0x00520d70;

            //CarRenderInfo offsets
            CRIoff_electricsState = 0x5308;
            CRIoff_lightsDamage = 0x530c;
            CRIoff_lightsGlassDamage = 0x5310;
            CRIoff_headlightTextures = 0x52c8;
            CRIoff_headlightGlassTextures = 0x52d4;
            CRIoff_brakelightTextures = 0x52e0;
            CRIoff_brakelightGlassTextures = 0x52ec;
            CRIoff_this_00 = 0x8d4;
            CRIoff_renderSettings = 0x54c8;
            CRIoff_terrainAdj = 0x4b8;
            CRIoff_MPointsArrayPtr = 0x4C0;

            Class43cOff_VehType = 0xe44;

            //projection
            utils::call(0x0084bc2a, getHeadlightsTexture);
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
        case 0x008AEC55: //UC 1.0

            break;
        case 0x0087ba75: //UC 1.0.1.18

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


    } //dllmain

    //for (auto c : Settings::VisualStyleCycles) {
    //    for (auto w : c) {
    //        std::cout << w << " ";
    //    }
    //    std::cout << "\n";
    //}



    return TRUE;
}
