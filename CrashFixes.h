#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"


namespace CrashFixes {
    auto forcePlayerCar = (void(__fastcall*)(DWORD * DebugVehicleSelection, DWORD something, char* carname, DWORD, DWORD, DWORD))0x00858ce0;
    char* forceSpecificCar = 0;

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

namespace Verbose {
    static const char* fatalerr = "Memory Framework: Fatal error";
    /*void* __alloca_probe = (void*)0x0087b250;
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
    }}*/

    auto bReleasePrintf = (void(__cdecl*)(char* _format, DWORD * arg1, DWORD * arg2, DWORD * arg3, DWORD * arg4, DWORD * arg5, DWORD * arg6, DWORD * arg7, DWORD * arg8)) 0x0;

    void WriteLog(char* error) {
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto time_buf = new char[26];
        ctime_s(time_buf, 26, &time);

        printf("\a");

        std::ofstream log;
        log.open("GameErrors.log", std::ios_base::app | std::ios_base::out);
        log << "Error detected on " << time_buf;
        log << error << "\n\n";
        log.flush();
    }

    void WriteLog(char* _format, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6, DWORD* arg7, DWORD* arg8) {
        char error[4096];
        snprintf(error, sizeof(error), _format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        WriteLog(error);
    }

    void __cdecl DebugPrintReplacement(char* _format, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6, DWORD* arg7, DWORD* arg8) {
        char error[4096];
        snprintf(error, sizeof(error), _format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        OutputDebugStringA(error);
        MessageBoxA(NULL, error, fatalerr, 0x00000010L);
        WriteLog(error);
    }

    void __cdecl bReleasePrintf_ERR(char* _format, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6, DWORD* arg7, DWORD* arg8) {
        bReleasePrintf(_format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        char error[4096];
        snprintf(error, sizeof(error), _format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        WriteLog(error);
        MessageBoxA(NULL, error, fatalerr, 0x00000010L);
    }
}

namespace GeomAlloc {
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
