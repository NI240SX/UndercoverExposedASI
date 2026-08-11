#pragma once

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
            lea ecx, [eax + 0x54a8];            // ecar fields from CarRenderInfo
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

                quit :
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
