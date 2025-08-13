// dllmain.cpp : Définit le point d'entrée de l'application DLL.
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

auto getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x0;

std::random_device rd;
std::mt19937 gen(rd());

namespace Settings{
    static bool FixTruckTrailers = true;
    static bool verbose = true;
    static bool KitRideHeight = true;
    static int lastGoodGlobalB = 1242129289;

    static int GeomAllocBlockSize = 0x50000;

    static bool useDayNightCycle = false;
    static bool DayNightCycleRandomStart = true;
    static vector<vector<uint32_t>> VisualStyleCycles;
    static int DayNightCycleDuration = 24 * 60;
    //static int DayNightCycleDuration = 10;

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
                        else std::cout << "Can't access the Vlt hasher!";

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

    static void readConfig() {

        struct stat confEx;
        if (stat("UndercoverExposed.ini", &confEx) == -1) {
            std::ofstream configFile("UndercoverExposed.ini");
            configFile << "FixTruckTrailers = 1\n";
            configFile << "Verbose = 1\n";
            configFile << "EnableKitRideHeight = 1\n";
            configFile << "LastGoodGlobalB = 1242129289\n";
            configFile << "GeomAllocBlockSize = 0x500000\n"; //vanilla 0x50000
            configFile << "DayNightCycleEnabled = 1\n";
            configFile << "DayNightCycleRandomStart = 1\n";
            configFile << "DayNightCycleDuration = 1440\n";
            configFile << "DayNightCycle = [[sunny6_0,sunny4_22.5,sunny2_45,lessnukedsunny_90,wet_135,wet2_150,wet3_180,wet_180,sunny7_180,sunset_180,sunset3_180,night_4_180,night_2_210,night_3_240,night_7_270,night_1_300,night_6_330,night_5_360]]\n";
            configFile.flush();
            configFile.close();
        }
        std::ifstream configFile("UndercoverExposed.ini");
        std::string line;

        while (std::getline(configFile, line)) {
            getConfigSetting("FixTruckTrailers", line, &Settings::FixTruckTrailers);
            getConfigSetting("Verbose", line, &Settings::verbose);
            getConfigSetting("LastGoodGlobalB", line, &Settings::lastGoodGlobalB);
            getConfigSetting("GeomAllocBlockSize", line, &Settings::GeomAllocBlockSize);
            getConfigSetting("DayNightCycleEnabled", line, &Settings::useDayNightCycle);
            getConfigSetting("DayNightCycleDuration", line, &Settings::DayNightCycleDuration);
            getConfigSetting("DayNightCycleRandomStart", line, &Settings::DayNightCycleRandomStart);
            getConfigSettingVltHash2DArray("DayNightCycle", line, &Settings::VisualStyleCycles);
            getConfigSetting("EnableKitRideHeight", line, &Settings::KitRideHeight);
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
        std::cout << "Trying to load a model over the polycount limit (" << requestedSize / 6 << " > " << Settings::GeomAllocBlockSize / 6 << ").\nPreventing a memory leak; the affected model will not render.\n";
        if (Settings::verbose) MessageBoxA(NULL, ("Trying to load a model over the polycount limit (" + std::to_string(requestedSize / 6) + " > " + std::to_string(Settings::GeomAllocBlockSize / 6) + ").\nPreventing a memory leak; the affected model will not render.").c_str(), "Undercover Exposed", MB_ICONERROR);
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
    static const char* fatalerr = "Fatal error";
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

            MOV        ECX, dword ptr[EAX + 0x549c] // ecarValues
            MOVSS      XMM0, dword ptr[ECX + 0x44]  // MinFERideHeight
            MOVSS      XMM1, dword ptr[ECX + 0x4c]  // MaxFERideHeight

            push edx;
            push eax;

            mov ecx, dword ptr[eax + 0x12e0];   // ECX = kitnumber
            push ecx;
            push 0xDC04998F;                    // FXAngularDeviation
            lea ecx, [eax + 0x5498];            // ecar fields from the other class
            call getVltValuePointer;            // getVltValuePointer(ecarFields, FXAngularDeviation, kitnumber)


            test eax, eax; // check that we got something
            jz defaultToNormal; //use vanilla calculation if nothing

            // modified calculation
//            movss xmm3, dword ptr[eax];
            mov edx, eax;
            pop eax;

            MOV        ECX, dword ptr[EAX + 0x43c];

            //SUBSS      XMM1, XMM0;
            //movss xmm0, xmm1;
            //MULSS      XMM0, dword ptr[ECX + 0xad8];

            movss dword ptr[esp + 0x18], xmm0;
            cvtps2pd xmm0, xmm0;
            cvtps2pd xmm1, xmm1;
            subsd xmm1, xmm0;
            movss xmm0, dword ptr[ecx + 0xad8];
            cvtps2pd xmm0, xmm0;
            push 0x0;
            mulsd xmm1, xmm0;
            cvtpd2ps xmm0, xmm1;

            addss xmm0, dword ptr[edx]; // add offset
            //PUSH       0x0;
            pop edx;
            jmp FERideHeightDetourReturn;


        defaultToNormal:
            pop eax;
            MOV        ECX, dword ptr[EAX + 0x43c];

            //SUBSS      XMM1, XMM0;
            //movss xmm0, xmm1;
            //MULSS      XMM0, dword ptr[ECX + 0xad8];

            movss dword ptr[esp + 0x18], xmm0;
            cvtps2pd xmm0, xmm0;
            cvtps2pd xmm1, xmm1;
            subsd xmm1, xmm0;
            movss xmm0, dword ptr[ecx + 0xad8];
            cvtps2pd xmm0, xmm0;
            push 0x0;
            mulsd xmm1, xmm0;
            cvtpd2ps xmm0, xmm1;

            //PUSH       0x0;
            jmp FERideHeightDetourReturn;
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
            break;
        case 0x0087ba75: //UC 1.0.1.18
            getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x0045cb10;
            break;
        default:
            break;
        }

        Settings::readConfig();

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
        if (carsModuleInstalled) switch (entry) {
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
            break;
        default:
            std::cout << "Unsupported EXE, over polycount limit crash fix not applied.\n";
        }
        if (carsModuleInstalled && Settings::GeomAllocBlockSize != 0x50000) switch (entry) {
        case 0x008AEC55: //UC 1.0
            //alloc function
            utils::set<int>(0x0079dcbb, Settings::GeomAllocBlockSize);
            utils::set<int>(0x0079dcde, Settings::GeomAllocBlockSize);
            utils::set<int>(0x0079dd06, Settings::GeomAllocBlockSize);
            utils::set<int>(0x0079dd2c, Settings::GeomAllocBlockSize);

            //other function with the same value
            utils::set<int>(0x0077c307, Settings::GeomAllocBlockSize);
            utils::set<int>(0x0077c329, Settings::GeomAllocBlockSize);
            utils::set<int>(0x0077c350, Settings::GeomAllocBlockSize);
            //utils::set<int>(0x018fde44, 0x100000);
            break;
        case 0x0087ba75: //UC 1.0.1.18
            //alloc function
            utils::set<int>(0x00771e4b, Settings::GeomAllocBlockSize);
            utils::set<int>(0x00771e6e, Settings::GeomAllocBlockSize);
            utils::set<int>(0x00771e96, Settings::GeomAllocBlockSize);
            utils::set<int>(0x00771ebc, Settings::GeomAllocBlockSize);

            //other function with the same value
            utils::set<int>(0x00751117, Settings::GeomAllocBlockSize);
            utils::set<int>(0x00751139, Settings::GeomAllocBlockSize);
            utils::set<int>(0x00751160, Settings::GeomAllocBlockSize);
            //utils::set<int>(0x018fde44, 0x100000);
            break;
        default:
            std::cout << "Unsupported EXE, geometry allocation block change not applied.\n";
        } else if (!carsModuleInstalled) switch (entry) {
        case 0x008AEC55: //UC 1.0
            //alloc function
            utils::set<int>(0x0079dcbb, 0x40000);
            utils::set<int>(0x0079dcde, 0x40000);
            utils::set<int>(0x0079dd06, 0x40000);
            utils::set<int>(0x0079dd2c, 0x40000);

            //other function with the same value
            utils::set<int>(0x0077c307, 0x40000);
            utils::set<int>(0x0077c329, 0x40000);
            utils::set<int>(0x0077c350, 0x40000);
            //utils::set<int>(0x018fde44, 0x100000);
            break;
        case 0x0087ba75: //UC 1.0.1.18
            //alloc function
            utils::set<int>(0x00771e4b, 0x40000);
            utils::set<int>(0x00771e6e, 0x40000);
            utils::set<int>(0x00771e96, 0x40000);
            utils::set<int>(0x00771ebc, 0x40000);

            //other function with the same value
            utils::set<int>(0x00751117, 0x40000);
            utils::set<int>(0x00751139, 0x40000);
            utils::set<int>(0x00751160, 0x40000);
            //utils::set<int>(0x018fde44, 0x100000);
            break;
        }
#endif // 54k crash fix

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
        default:
            std::cout << "Unsupported EXE, day/night cycle not applied.\n";
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

                //FERideHeightDetourReturn = (void*)0x0083fc22;
                //utils::jump(0x0083fbe7, FERideHeightDetour_10);
                break;
            default:
                std::cout << "Unsupported EXE, kit-specific ride height not applied.\n";
        }
#endif // Per kit ride height
    }

    //for (auto c : Settings::VisualStyleCycles) {
    //    for (auto w : c) {
    //        std::cout << w << " ";
    //    }
    //    std::cout << "\n";
    //}



    return TRUE;
}
