#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"

using namespace Settings;

//Effects
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
constexpr auto CarRenderConnNewLength = 0x2b0;

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

        //DWORD iVar13;

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

    constexpr auto GlassShatterEffectHeadlights = 0x7A656CCB;
    constexpr auto GlassShatterEffectBrakelights = 0x925D4FFA;

    void __fastcall UpdateHeadlightAndBrakelightShatterEffects(DWORD CarRenderConn, DWORD edx, char param_1) {
        DWORD* puVar1;
        DWORD shatterEffectHeadlights;
        DWORD shatterEffectBrakelights;

        puVar1 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, GlassShatterEffectHeadlights, 0);
        if (puVar1 == (DWORD*)0x0) puVar1 = (DWORD*)Attrib::DefaultDataArea(4);
        shatterEffectHeadlights = puVar1[1];

        puVar1 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, GlassShatterEffectBrakelights, 0);
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


            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, MissShiftEffect, 0);
            if (puVar5 == (DWORD*)0x0) _MissShiftEffect = MissShiftEffect; //if it doesn't exist as a field, force an emittergroup node
            else _MissShiftEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, NOSEffect, 0);
            if (puVar5 == (DWORD*)0x0) _NOSEffect = NOSEffect; //if it doesn't exist as a field, force an emittergroup node
            else _NOSEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, ExhaustSmokeEffect, 0);
            if (puVar5 == (DWORD*)0x0) _ExhaustSmokeEffect = ExhaustSmokeEffect; //if it doesn't exist as a field, force an emittergroup node
            else _ExhaustSmokeEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, NOSTrailEffect, 0);
            if (puVar5 == (DWORD*)0x0) _NOSTrailEffect = NOSTrailEffect; //if it doesn't exist as a field, force an emittergroup node
            else _NOSTrailEffect = puVar5[1];

            puVar5 = (DWORD*)Attrib::Instance::GetAttributePointer((Attrib::Instance*)(CarRenderConn + 0x14), 0, BackfireEffect, 0);
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