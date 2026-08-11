#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"
#include "ucetesting.h"

using namespace Settings;

namespace Inputs {
    //float AutosculptSliderSpeed = 0.5f; //vanilla 0.2f
    //float AutosculptSliderSpeedBoost;
    //float AutosculptSliderSpeedSlow;

    //float OptionsSliderSpeed = 80.0f; //vanilla 80.0f
    //float OptionsSliderSpeedBoost;
    //float OptionsSliderSpeedSlow;

    //float PerfTuningSliderSpeed = 80.0f; //vanilla 100.0f
    //float PerfTuningSliderSpeedBoost;
    //float PerfTuningSliderSpeedSlow;

    //float FEVisualPaint1SliderSpeed = 150.0f; //vanilla 150.0f
    //float FEVisualPaint1SliderSpeedBoost;
    //float FEVisualPaint1SliderSpeedSlow;

    //float FEVisualPaint2SliderSpeed = 100.0f; //vanilla 100.0f
    //float FEVisualPaint2SliderSpeedBoost;
    //float FEVisualPaint2SliderSpeedSlow;



    float* AutosculptSliderSpeedFactor; //0.2
    float* OptionsSliderSpeedFactor; //80
    float* PerfTuningSliderSpeedFactor; //100
    float* FEVisualPaintSpeedFactor1; //150
    float* FEVisualPaintSpeedFactor2; //100

    static float fakeFrameTime = 0.01666666666667f;

    static int debouncer;
    static int vinylStepMultiplier = 1;

    static int TestsTimer = 0;

    void(__cdecl* EnableInterrupts)() = NULL;
    void __cdecl frameHook() {
        bool isControlDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
        bool isShiftDown = (GetAsyncKeyState(VK_SHIFT) & 0x8000);

        static bool wasControlDown = false, wasShiftDown = false;

        if (Settings::SliderEnhancements && (wasControlDown != isControlDown || wasShiftDown != isShiftDown)) {
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

            if (isShiftDown) vinylStepMultiplier = VinylFastStep;
            else if (isControlDown) vinylStepMultiplier = 1;
            else vinylStepMultiplier = VinylStep;

            wasControlDown = isControlDown;
            wasShiftDown = isShiftDown;
        }

        if (isControlDown && GetAsyncKeyState(VK_DELETE) && *(timer_world + 1) > debouncer) { //debouncer broken in 1.0
            if (Settings::ShowOverlay) FE__FeChyron__Summon((char*)"Use Ctrl+Del to bring back the overlay!", eChyronType::CHYRON_MESSAGE, 0, 0, 0);
            Settings::ShowOverlay = !Settings::ShowOverlay;
            debouncer = *(timer_world + 1) + 1000; //general time

            /*
            chyron pic types -
            0 = friend request
            1 = connected
            2 = connection lost
            3 = wheel check (achievements)
            4 = message
            5 = friend request
            6 = race incoming (game invite)
            7 = race wheel achievement smth ???
            8 = money money money
            9 = more money ?
            10 = music (only in FECarHeaven)
            11 = friend request (probably default)
            */
        }

        if (isControlDown && isShiftDown && GetAsyncKeyState(VK_SPACE) && *(timer_world + 1) > debouncer) {
            TestsTimer++;
            debouncer = *(timer_world + 1) + 1000; //general time
            if (TestsTimer == 5) {
                if (!ucetesting::initialized) {
                    Settings::tests = true;
                    FE__FeChyron__Summon((char*)"Testing functions enabled!", eChyronType::CHYRON_MESSAGE, 0, 0, 0);
                    ucetesting::doTests();
                }
            }
        }

        EnableInterrupts(); //call any potential other ASI hooked in the same place
    }

    auto VinylSelectablePart__SetTransformData = (void(__fastcall*)(DWORD, DWORD, DWORD, DWORD))0;
    void __fastcall VinylSelectablePart__SetTransformData_wrapper(DWORD vinylSelPart, DWORD edx, DWORD transformType, DWORD value) {
        value *= vinylStepMultiplier;
        VinylSelectablePart__SetTransformData(vinylSelPart, edx, transformType, value);
    }
}