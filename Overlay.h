#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
//#include "Settings.h"
#include "ucetesting.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_internal.h"
#include "imgui/implot.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Overlay {
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

    void __cdecl imgui_init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
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


    static bool doDemoWindow = false;
    static bool doSunAndSkyWindow = false;
    static bool doAttribEditor = false;

    void ImGuiFrame() {

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (doDemoWindow) ImGui::ShowDemoWindow(&doDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            //const float label_width_base = ImGui::GetFontSize() * 12;               // Some amount of width for label, based on font size.
            //const float label_width_max = ImGui::GetContentRegionAvail().x * 0.40f; // ...but always leave some room for framed widgets.
            //const float label_width = IM_MIN(label_width_base, label_width_max);
            //ImGui::PushItemWidth(-label_width);                                     // Right-align: framed items will leave 'label_width' available for the label.

            if (!ImGui::GetIO().WantCaptureMouse) ImGui::SetWindowFocus(NULL);


            if (ImGui::Begin("UCE Experimental Tweaks Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

                float width = ImGui::CalcItemWidth();

                ImGui::Text("Ctrl+Delete to hide these overlays");
                if (Settings::tests) ImGui::Checkbox("Show ImGui Demo", &doDemoWindow);
                if (Settings::tests) ImGui::Checkbox("Show SunAndSky editor", &doSunAndSkyWindow);
                if (Settings::tests) ImGui::Checkbox("Show Attrib editor", &doAttribEditor);

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
                                *current = n - 1;
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

                //if (UserProfile_Player1 != NULL) ImGui::Text("Current camera: %d", *(ePlayerSettingsCameras*)(*UserProfile_Player1 + 0x4d5f0 + 0x7c + 0x20));
                //std::to_string(*(ePlayerSettingsCameras*)(*UserProfile_Player1 + 0x4d5f0 + 0x7c + 0x20))

                if (Settings::tests && *FECarHeaven != NULL && CURRENT_GAME_VERSION == UC_1_21) {
                    //float width = ImGui::CalcItemWidth();
                    static char presetRideName[64];
                    ImGui::SetNextItemWidth(width);
                    ImGui::InputText("##presetridename", presetRideName, sizeof(presetRideName));
                    ImGui::SetNextItemWidth(width);
                    if (ImGui::Button("Export current PresetRide")) {
                        ucetesting::DoExportPresetRide(presetRideName);
                    }
                    ImGui::SetNextItemWidth(width);
                    if (ImGui::Button("Replace car with PresetRide")) {
                        ucetesting::DoReplacePresetRide(presetRideName);
                    }
                }


                ImGuiIO& io = ImGui::GetIO(); (void)io;
                if (Settings::tests) ImGui::Text("%.1f FPS/%.3f ms", io.Framerate, 1000.0f / io.Framerate);

                //ImGui::Text("mMainViewNumSubmissions = %d",         **((DWORD**)(*Renderer + 0x24))); //not right and doesnt do shit anyways
                //ImGui::Text("mMainViewNumPolys = %d",               (*(DWORD**)(*Renderer + 0x24))[1]);
                //ImGui::Text("mMainViewWorldNumSubmissions = %d",    (*(DWORD**)(*Renderer + 0x24))[2]);
                //ImGui::Text("mMainViewWorldNumPolys = %d",          (*(DWORD**)(*Renderer + 0x24))[3]);
                //ImGui::Text("mShadowViewNumSubmissions = %d",       (*(DWORD**)(*Renderer + 0x24))[4]);
                //ImGui::Text("mShadowViewNumPolys = %d",             (*(DWORD**)(*Renderer + 0x24))[5]);
                //ImGui::Text("mShadowViewWorldNumSubmissions = %d",  (*(DWORD**)(*Renderer + 0x24))[6]);
                //ImGui::Text("mEnvMapViewNumSubmissions = %d",       (*(DWORD**)(*Renderer + 0x24))[7]);
                //ImGui::Text("mEnvMapViewNumPolys = %d",             (*(DWORD**)(*Renderer + 0x24))[8]);

                if (ImGui::TreeNode("Freeroam music controls")) {

                    using namespace FreeroamMusic;
                    if (Settings::tests && SFXCTL_MusicPlayback_addr != NULL) {
                        //ImGui::Text("SFXCTL_MusicPlayback at 0x%08X", SFXCTL_MusicPlayback_addr);
                        DWORD* MusicLogic = (DWORD*)(SFXCTL_MusicPlayback_addr + 0x30);
                        if (MusicLogic != NULL) {
                            //ImGui::Text("MusicLogic at 0x%08X", *MusicLogic);
                            if ((char**)(*MusicLogic + 0x1c) != NULL) ImGui::Text("MusicLogic state %s", *(char**)(*MusicLogic + 0x1c));
                        }
                    }


                    ImGui::SetNextItemWidth(width);
                    if (ImGui::Button("Next song")) {
                        NeedsInit = true;
                    }

                    ImGui::SetNextItemWidth(width / 2);
                    ImGui::Text("Next song index");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(width / 2);
                    if (ImGui::InputInt("##forceIdx", &forceNextSongIndex, 1, 100, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                        NeedsInit = true;
                    }

                    ImGui::SetNextItemWidth(width / 2);
                    ImGui::Text("Next song type");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(width / 2);
                    ImGui::Combo("##bitfield", (int*)&Settings::currentBitfield, "PURSUIT\0CHASEDOWN\0RACE\0MAPSCREEN\0CARHEAVEN");
                    //ImGui::InputInt("##bitfield", (int*) &Settings::currentBitfield, 1, 1, ImGuiInputTextFlags_AutoSelectAll);

                    //id changing breaks about everything, no fucking clue why
                    //ImGui::SetNextItemWidth(width / 2);
                    //ImGui::Text("Next song ID");
                    //ImGui::SameLine(width / 2);
                    //if (ImGui::InputInt("##forceId", &forceNextSongID, 1, 100, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                    //    NeedsInit = true;
                    //}


                    ImGui::TreePop();
                }

            }
            ImGui::End();
        }

        if (doSunAndSkyWindow) {
            if (ImGui::Begin("SunAndSky and visual treatment", &doSunAndSkyWindow)) {
                static char sunSkyCurKeyBuf[64], sunSkyDesiredKeyBuf[64];
                static DWORD sunSkyCurKey, sunSkyDesiredKey;

                if (ImGui::TreeNode("SunAndSky")) {

                    static bool preventZoneChange = false;
                    static bool preventZoneChangeOld = false;

                    if (sunSkyCurKey != theSunAndSky->mCurrentKey) {
                        snprintf(sunSkyCurKeyBuf, 64, "0x%08X", theSunAndSky->mCurrentKey);
                        sunSkyCurKey = theSunAndSky->mCurrentKey;
                    }
                    if (sunSkyDesiredKey != theSunAndSky->mDesiredKey) {
                        snprintf(sunSkyDesiredKeyBuf, 64, "0x%08X", theSunAndSky->mDesiredKey);
                        sunSkyDesiredKey = theSunAndSky->mDesiredKey;
                    }


                    if (ImGui::InputText("mCurrentKey", sunSkyCurKeyBuf, 64, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                        sunSkyCurKey = getVltHash(sunSkyCurKeyBuf);
                        theSunAndSky->mCurrentKey = sunSkyCurKey;
                    }
                    if (ImGui::InputText("mDesiredKey", sunSkyDesiredKeyBuf, 64, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                        sunSkyDesiredKey = getVltHash(sunSkyDesiredKeyBuf);
                        theSunAndSky->mDesiredKey = sunSkyDesiredKey;
                    }
                    ImGui::SliderFloat("mBlendAmount", &theSunAndSky->mBlendAmount, 0.0f, 1.0f);

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

                    //ImGui::Text("Warning: this may cause instabilities and unwanted changes!");

                    ImGui::Checkbox("Hide inherited attributes", &AttribEditor::HideHeritedAttributes);
                    ImGui::SameLine(); ImGui::Checkbox("Hide empty attributes", &AttribEditor::HideEmptyAttributes);

                    Attrib::Gen::zone_visual_style current;

                    Attrib::Gen::zone_visual_style::constructor(&current, 0, sunSkyCurKey, 0);
                    //if (current.mCollection == NULL) Attrib::Gen::zone_visual_style::constructor(&current, 0, DEFAULT, 0);

                    if (ImGui::InputText("Key", sunSkyCurKeyBuf, 64, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                        sunSkyCurKey = getVltHash(sunSkyCurKeyBuf);
                    }

                    //previous zone_visual_style editor
/*
                        if (current.mLayoutPtr != NULL && ImGui::TreeNode("Layout")) {
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current.mLayoutPtr->SunLatitude, "SunLatitude", 1.0f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &((float*)current.mLayoutPtr)[1], "SkyDomeRotation", 1.0f);

                            ImGui::TreePop();
                        }

                        if (current.mCollection != NULL && ImGui::TreeNode("Collection")) {
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "SunOcclusionResponseRate", 0.01f);
                            InputInt("zone_visual_style", sunSkyCurKeyBuf, &current, "SkyDomeStyle");
                            InputInt("zone_visual_style", sunSkyCurKeyBuf, &current, "ColourCube");
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "SunLongitude", 1.0f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "VisibleLongitude", 1.0f);
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "SunColour", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "SunIntensity", 0.01f);
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "AmbientColour", 0.01f);
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "SHColour", 0.01f);
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "FogColour", 0.01f);
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "FogColourFar", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "FogNear", 1.0f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "FogFar", 1.0f);
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "SunHazeColour", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "SunHazeDistanceBias", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "FogExponent", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "SunHazeSize", 0.1f);
                            InputInt("zone_visual_style", sunSkyCurKeyBuf, &current, "LensFlareIndex");
                            InputVector4("zone_visual_style", sunSkyCurKeyBuf, &current, "SpecularColour", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "BloomIntensity", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "BloomThresold", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "BloomExponent", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "BloomRecoveryBias", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "BloomRecoveryScale", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "RoadReflectionIntensity", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "RoadReflectionLighting", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "RoadReflectionLightingPower", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "ToneMappingContrast", 0.01f);
                            InputFloat("zone_visual_style", sunSkyCurKeyBuf, &current, "ToneMappingSaturation", 0.01f);
                            ImGui::TreePop();
                        }
*/
                    if (current.mCollection != NULL) AttribEditor::EditCollection(current.mCollection);

                    ImGui::TreePop();
                }

                if (Settings::useDayNightCycle && ImGui::TreeNode("UCE day/night cycle")) {
                    //if (ImGui::InputDouble("Time of day (sec)", &Settings::TimeOfDay, 1000.0, 10000.0, "%.0f", ImGuiInputTextFlags_AutoSelectAll)) ((bVector4*)theSunAndSky->mZoneBlender)->y = 0.0f;
                    static double zero_d = 0.0;
                    static double max = DayNight::TIME_OF_DAY_LENGTH;
                    //if (ImGui::DragScalar("Time of day (hours)", ImGuiDataType_Double, &Settings::TimeOfDay, 1.0f, &zero_d, &max)) ((bVector4*)theSunAndSky->mZoneBlender)->y = 0.0f;
                    if (ImGui::SliderScalar("Time of day (hours)", ImGuiDataType_Double, &Settings::TimeOfDay, &zero_d, &max)) ((bVector4*)theSunAndSky->mZoneBlender)->y = 0.0f;
                    ImGui::InputFloat("Time per zone (hours)", &DayNight::timePerZone, 1.0f, 100.0f, "%.3f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);
                    if (ImGui::InputInt("Cycle duration (sec)", &Settings::DayNightCycleDuration, 1, 100, ImGuiInputTextFlags_AutoSelectAll)) DayNight::timePerZone = (float)Settings::DayNightCycleDuration / Settings::VisualStyleCycles[DayNight::currentWeatherSet].size();
                    if (ImGui::InputInt("Current weather set", &DayNight::currentWeatherSet, 1, 1, ImGuiInputTextFlags_AutoSelectAll)) DayNight::currentWeatherSet %= Settings::VisualStyleCycles.size();
                    ImGui::TreePop();
                }


            }
            ImGui::End();
        }

        if (doAttribEditor) {
            AttribEditor::ShowAttribEditor(&doAttribEditor);
        }



        if (Settings::tests) ucetesting::imGuiStuff::ImGuiFrame();
    }


    //render and input hooks
    auto endSceneHookDest = (void(__cdecl*)())0x0;// 0758430;
    void __cdecl endSceneHook() {

        //TODO manage device lost
        if (Settings::ShowOverlay) {
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
