#pragma once
//#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"

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
