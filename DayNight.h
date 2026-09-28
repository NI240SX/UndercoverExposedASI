#pragma once
//#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"

namespace DayNight {

    constexpr double TIME_OF_DAY_LENGTH = 24.0; //let's use hours to be able to set it up somewhat easily

    float timePerZone;
    double lastTick = 0;
    int currentWeatherSet = 0;

    void (__fastcall* SunAndSky_Update)(void* thisptr, void* edx, bool param);

	void __fastcall SunAndSky_Update_Wrapper(void* thisptr, void* edx, bool param) {
        using namespace Settings;
        TimeOfDay += ((*timer_world * 0.00025) - lastTick) * (TIME_OF_DAY_LENGTH / Settings::DayNightCycleDuration);
        lastTick = *timer_world * 0.00025;
        //TimeOfDay += TimeOfDaySpeed * *FrameTime;
        if (TimeOfDay > TIME_OF_DAY_LENGTH) {
			TimeOfDay = 0.0;

            //kinda broken at the beginning
            //swap cycles
            std::uniform_int_distribution<> distribWeatherSet(0, VisualStyleCycles.size() - 1);
            currentWeatherSet = distribWeatherSet(gen);
            //            std::cout << "New weather set : " << currentWeatherSet << "\n";
            timePerZone = Settings::DayNightCycleDuration * 1.0 / VisualStyleCycles[currentWeatherSet].size();
		}
		SunAndSky_Update(thisptr, edx, param);
	}

    //old getNextVisualStyle
/*    static uint32_t __cdecl getNextVisualStyle(char* toHash) {

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
    }*/

    static uint32_t __cdecl getNextVisualStyle(char* toHash) {
        return Settings::VisualStyleCycles[currentWeatherSet][(int)(Settings::TimeOfDay / (timePerZone * TIME_OF_DAY_LENGTH / Settings::DayNightCycleDuration))];
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

    char* __cdecl getLocalizedString_wrapper(UINT key) {
        char* text = getLocalizedString(key);
        
        int dividerAt=0;        
		bool isPM = false;
		while (text[dividerAt] && text[dividerAt + 1]) {
			if (text[dividerAt] == 'P' && text[dividerAt + 1] == 'M') {
				isPM = true;
				break;
			}
			dividerAt++;
		}

        dividerAt = 0;
        while (text[dividerAt] && text[dividerAt] != ':') {
            dividerAt++;
        }


        int hour = 0;
        int minute = 0;
        sscanf_s(text, "%d", &hour);
		if (isPM) hour += 12;
        if (text[dividerAt]) sscanf_s(text + dividerAt + 1, "%d", &minute);

        Settings::TimeOfDay = hour + minute / 60.0;

        return text;
    }

	float skyboxLatitude = 0.0f;
    void __stdcall fixSkyboxRotation(SunAndSky* sunAndSky, Attrib::Gen::zone_visual_style* current, Attrib::Gen::zone_visual_style* next) {
        float fVar16 = (current->mLayoutPtr)->SkyDomeRotation;
        //if (-fVar16 <= 180.0) {
        //    if (180.0 < fVar16) {
        //        fVar16 -= 360.0;
        //    }
        //}
        //else {
        //    fVar16 += 360.0;
        //}
        if (0.0 <= fVar16) {
            if (360.0 <= fVar16) {
                fVar16 = fVar16 - 360.0;
            }
        }
        else {
            fVar16 = fVar16 + 360.0;
        }
		sunAndSky->mSkyDomeRotation = fVar16;
        //*(float*)(sunAndSky + 0x130) = fVar16; //current


        fVar16 = (next->mLayoutPtr)->SkyDomeRotation;
        //if (-fVar16 <= 180.0) {
        //    if (180.0 < fVar16) {
        //        fVar16 -= 360.0;
        //    }
        //}
        //else {
        //    fVar16 += 360.0;
        //}
        if (0.0 <= fVar16) {
            if (360.0 <= fVar16) {
                fVar16 = fVar16 - 360.0;
            }
        }
        else {
            fVar16 = fVar16 + 360.0;
        }
        sunAndSky->nextSkyDomeRotation = fVar16;
        //*(float*)(sunAndSky + 0x134) = fVar16; //next

		skyboxLatitude = sunAndSky->mSkyDomeRotation + sunAndSky->mBlendAmount * (sunAndSky->nextSkyDomeRotation - sunAndSky->mSkyDomeRotation);
    }

	void* fixSkyboxRotation_detour_ret;
    __declspec(naked) void fixSkyboxRotation_detour() {
        __asm {
            mov edx, dword ptr[eax];
            mov dword ptr[esi + 0x1a4], edx; //mSkyStyle for previous cutoff code

            lea eax, [esp + 0x24]; //next
            lea ecx, [esp + 0x34]; //current
            push eax;
            push ecx;
            push esi;
			call fixSkyboxRotation;



			jmp fixSkyboxRotation_detour_ret;
        }
    }

    //float* SkyLatitude = (float*)0x01862d28;
    void(__cdecl* eCreateRotationZ)(float* out, short angle);
	void __cdecl eCreateRotationZ_wrapper(float* out, short angle) {
		//eCreateRotationZ(out, (short)(int)(*SkyLatitude * 182.0444488525391));
        eCreateRotationZ(out, (short)(int)(skyboxLatitude * 182.0444488525391));
	}

    //void* transitionPercentageDetourReturn;
    //auto findTransitionPercentage = (void(__cdecl*)()) 0x008354c0;
    //__declspec(naked) void transitionPercentageDetour() { __asm {
    //    mov eax, dword ptr[timePerZone]; //force transition time
    //    mov dword ptr[ecx + 0x14], eax;
    //    call findTransitionPercentage;
    //    jmp transitionPercentageDetourReturn;
    //}}

    void Inject() {
        switch (CURRENT_GAME_VERSION) {
        case UC_1_0: //UC 1.0
            timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[0].size();


            //updateFirstVisualStyleReturn = (void*)(0x00875df6);
            //updateNextVisualStyleReturn = (void*)(0x00875ef8);

            //dat_012fa474 = *(DWORD*)0x00de93dc;
            //dat_012fa46c = *(DWORD*)0x00de93d4;
            //dat_00b694d0 = *(DWORD*)0x00bd9c80;
            float_1 = (float*)*(DWORD*)0x00bd97b0; //1.0f constant

            //transitionPercentageDetourReturn = (void*)0x00875e69;
            //findTransitionPercentage = reinterpret_cast<decltype(//findTransitionPercentage)>(0x00863fe0);

            //utils::jump(0x00875dea, updateFirstVisualStyle); //getting into free roam
            //utils::jump(0x00875eec, updateNextVisualStyle); //general update

            //NEW
            SunAndSky_Update = utils::call(0x005f7d1e, SunAndSky_Update_Wrapper);
            utils::call(0x00788f73, SunAndSky_Update_Wrapper);

            utils::call(0x00875de2, getNextVisualStyle); //replace Attrib::StringToKey
            utils::call(0x00875ee4, getNextVisualStyle); //replace Attrib::StringToKey

            //utils::jump(0x008720c0, findTransitionSpeedMult); //return 1.0f for transition duration (at least this was responsible for no transition when car no move)
            utils::call(0x00875e58, ret1f);
            utils::call(0x00875f25, getTransitionTime);
            //utils::jump(0x00875e64, transitionPercentageDetour); //force transition time

            if (Settings::SetMissionTime) utils::call(0x0058d279, getLocalizedString_wrapper); //force TOD on missions which have a description string
            break;
        case UC_1_21: //UC 1.0.1.18
            timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[0].size();

            //updateFirstVisualStyleReturn = (void*)(0x008466ee);
            //updateNextVisualStyleReturn = (void*)(0x008467da);

            //dat_012fa474 = *(DWORD*)0x012fa474;
            //dat_012fa46c = *(DWORD*)0x012fa46c;
            //dat_00b694d0 = *(DWORD*)0x00b694d0;
            float_1 = (float*)*(DWORD*)0x00b647b4;

            //transitionPercentageDetourReturn = (void*)0x00846756;
            //findTransitionPercentage = reinterpret_cast<decltype(//findTransitionPercentage)>(0x008354c0); //VisualLookEffectTarget::Update

            //utils::jump(0x008466e2, updateFirstVisualStyle); //getting into free roam
            //utils::jump(0x008467ce, updateNextVisualStyle); //general update

            //NEW
            SunAndSky_Update = utils::call(0x005dd62e, SunAndSky_Update_Wrapper);
            utils::call(0x0075c63d, SunAndSky_Update_Wrapper);

            utils::call(0x008466da, getNextVisualStyle); //replace Attrib::StringToKey
            utils::call(0x008467c6, getNextVisualStyle); //replace Attrib::StringToKey

            //utils::jump(0x008428e0, findTransitionSpeedMult); //return 1.0f for transition duration (at least this was responsible for no transition when car no move)
            utils::call(0x00846745, ret1f);
            utils::call(0x00846807, getTransitionTime);
            //utils::jump(0x00846751, transitionPercentageDetour); //force transition time

            if (Settings::SetMissionTime) utils::call(0x0057bf59, getLocalizedString_wrapper); //force TOD on missions which have a description string

            //fix skybox rotation
			utils::jump(0x00839cef, fixSkyboxRotation_detour);
			fixSkyboxRotation_detour_ret = (void*)0x00839d5f;
            utils::nop(0x00839d6a, 8);
            eCreateRotationZ = utils::call(0x0083b326, eCreateRotationZ_wrapper);
            break;
        }
    }
}
