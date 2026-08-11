#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

//#include "Effects.h"
//#include "Inputs.h"

std::random_device rd;
std::mt19937 gen(rd());


using namespace std;


namespace Settings {
    static bool CrashFixes = true;
    static bool FixTruckTrailers = true;
    static bool verbose = true;
    static bool KitRideHeight = false;
    static time_t lastGoodGlobalB = 1242129289;

    static int IndexBufferSize = 0x50000;
    static int CarsPoolSize = 0x4e20000;

    static bool useDayNightCycle = false;
    static bool DayNightCycleRandomStart = true;
    static vector<vector<uint32_t>> VisualStyleCycles;
    static int DayNightCycleDuration = 24 * 60;
    //static int DayNightCycleDuration = 10;
    static bool SkipBootupChecks = true;

    static bool HeadlightTweaksEnabled = false;
    static bool AutomaticHeadlights = false;
    static int AutomaticHeadlightsField = 0x60;
    static float AutomaticHeadlightsThresold = 0.5;
    static bool ProjectionCutoff = true;
    static int ProjectionCutoffField = 0x60;
    static float ProjectionCutoffThresold = 0.5;

    static bool ExtendAftermarketMenu = false;
    static vector<pair<DWORD, char*>> AftermarketMenu;
	static bool AllowMoreCarbonParts = false;
	static bool AddMaterialToggle = false;
	static int MaterialsCount = 2;

    static bool SliderEnhancements = true;
    static bool EnableAllCameras = false;
    static bool EnableOverlay = false;
    static bool ShowOverlay = false;

    static bool HeadlightShatterFix = true;
    static bool EffectsEnhancements = false;

    static bool ReadCarListFromAttrib = false;
    static bool RemoveStockCarRecords = false;
    static bool Remove20CarLimit = false;

    static bool AddFreeroamMusic = false;
    static eMusicBitfields currentBitfield = RACE;

    static bool AddDecalsSelection = false;

	static bool AllowPerformanceDowngrade = false;

    static bool tests = false;




    DWORD MissShiftEffect, NOSEffect, ExhaustSmokeEffect, NOSTrailEffect, BackfireEffect;


    float AutosculptSliderSpeed = 0.5f; //vanilla 0.2f
    float AutosculptSliderSpeedBoost;
    float AutosculptSliderSpeedSlow;

    float OptionsSliderSpeed = 80.0f; //vanilla 80.0f
    float OptionsSliderSpeedBoost;
    float OptionsSliderSpeedSlow;

    float PerfTuningSliderSpeed = 80.0f; //vanilla 100.0f
    float PerfTuningSliderSpeedBoost;
    float PerfTuningSliderSpeedSlow;

    float FEVisualPaint1SliderSpeed = 150.0f; //vanilla 150.0f
    float FEVisualPaint1SliderSpeedBoost;
    float FEVisualPaint1SliderSpeedSlow;

    float FEVisualPaint2SliderSpeed = 100.0f; //vanilla 100.0f
    float FEVisualPaint2SliderSpeedBoost;
    float FEVisualPaint2SliderSpeedSlow;

    int VinylFastStep = 4;
    int VinylStep = 2;

    static void getConfigSetting(string setting, string line, int* ret) {
        if (line.find(setting + " ") == 0) {
            int value = line.find("=") + 1;
            while (line.at(value) == ' ' || line.at(value) == '\t') value++;
            if (line.at(value) == '0' && line.at(value + 1) == 'x')
                sscanf_s(line.substr(value, std::string::npos).c_str(), "%x", ret);
            else sscanf_s(line.substr(value, std::string::npos).c_str(), "%d", ret);
            //std::cout << setting << " = " << *ret << "\n";
        }
    }

    static void getConfigSetting(string setting, string line, time_t* ret) {
        if (line.find(setting + " ") == 0) {
            int value = line.find("=") + 1;
            while (line.at(value) == ' ' || line.at(value) == '\t') value++;
            if (line.at(value) == '0' && line.at(value + 1) == 'x')
                sscanf_s(line.substr(value, std::string::npos).c_str(), "%x", ret);
            else sscanf_s(line.substr(value, std::string::npos).c_str(), "%d", ret);
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
                if (toHash.length() < 3) {
                    *ret = 0;
                }
                else {
                    if (toHash[0] == '0' && toHash[1] == 'x') {
                        sscanf_s(toHash.c_str(), "%x", ret);
                    }
                    else
                        if (getVltHash != nullptr) {
                            *ret = getVltHash(toHash.c_str());
                        }
                        else std::cout << "Undercover Exposed: Can't access the Vlt hasher!";
                }
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
                        string toHash = line.substr(start, i - start);
                        //cout << "String to hash : " << toHash << "\n";
                        //tempNames->push_back(toHash);

                        if (getVltHash != nullptr) {
                            cycle.push_back(getVltHash(toHash.c_str()));
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

    static void getConfigSettingMenuItem(string setting, string line, vector<pair<DWORD, char*>>* Menu) {
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
            ItemName = new char[32];
            int j = 0;
            while (i < line.length() && j < 31 && (!(line.at(i) == '/' && line.at(i + 1) == '/'))) {
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
Verbose = 1                             //includes additional debugging information and logs detected errors in GameErrors.log
CrashFixes = 1                          //fixes various crashes, some specific to Exposed (removes intentional crashes, removes breakpoints, fixes the starter car crash in 1.18, prevents a crash when CreateTexture fails, removes calls to CarLoader::PrintMemoryUsage a seemingly broken function, prevents the crash for models over max polycount, enforces memory pool integrity checks)
IndexBufferSize = 0xA0000               //increases IndexBufferSize, vanilla = 0x50000
CarsPoolSize = 0x8000000                //increases Cars pool size, vanilla = 0x4e20000
FixTruckTrailers = 1                    //fixes tractor-trailer linking broken by Binary 2.8.3
LastGoodGlobalB = 1242129289

[Features: Miscellaneous]
KitRideHeightEnabled = 1                //enables per-kit ride height on some cars
SkipBootupChecks = 1                    //skips some unnecessary bootup checks
EnableAllCameras = 0                    //allows to pick any of the nine vanilla cameras: POV_BUMPER/HOOD, POV_OUTSIDE_CLOSE/FAR/DRIFT/STAGING/PAUSE/SLOTCAR/TUNING
EnableOverlay = 0                       //enables an in-game overlay - most features are only in TESTS
ShowOverlayOnGameStart = 0              //shows the overlay on game bootup - else use Ctrl + Delete to bring it up
AddFreeroamMusic = 1                    //adds music during free roam
FreeroamMusicType = 2                   //available types: 0 = PURSUIT, 2 = RACE, 3 = MAPSCREEN, 4 = CARHEAVEN
RestoreDecalsEditing = 1                //add options to add and edit decals in the Vinyls menu
AllowPerformanceDowngrade = 1           //allows to select an upgrade lower than the current one in the Performance menu - stock is still not selectable

[Features: Car list]
ReadCarListFromAttrib = 0               //forces the game to read the car list from Attrib data instead of the save, removing the need to make a new save for each new car
RemoveStockCarRecords = 0               //removes stock cars from new saves when the above option is selected
Remove20CarLimit = 0                    //removes the 20 car limit in Career and My Cars - the actual limit is 70 total!

[Features: Slider enhancements]
SliderEnhancements = 1                  //fixes Autosculpt sliders speed and adds speed modifiers with Ctrl and Shift
SliderBoostMultiplier = 2.0             //slider speed multiplier when pressing Shift
SliderSlowMultiplier = 0.5              //slider speed multiplier when pressing Control
VinylStep = 2                           //vinyl editing step (the smallest step of 1 is always applied when pressing Control)
VinylFastStep = 4                       //vinyl editing step when pressing Shift

[Features: Car effects]
HeadlightShatterFix = 1                 //fixes the headlight shatter effect using brakelight break particles (no effect if EffectsEnhancements is enabled)
EffectsEnhancements = 0                 //enables effects for shifting backfire, brakelight nos trail and exhaust smoke
ExhaustSmokeEffect = uce_exhaust        //ecar field or emittergroup node to use as exhaust smoke emitter
NOSEffect = NOSEffect                   //ecar field or emittergroup node to use as NOS exhaust emitter
NOSTrailEffect = uce_nostrail           //ecar field or emittergroup node to use as NOS brakelight trail emitter
BackfireEffect = NOSEffect              //ecar field or emittergroup node to use as gearshift backfire emitter
MissShiftEffect = MissShiftEffect       //ecar field or emittergroup node to use as miss shift backfire emitter

[Features: Day and Night Cycle]
DayNightCycleEnabled = 0                //enables the day and night cycle
DayNightCycleRandomStart = 1            //starts the cycle at a random time instead of its beginning
DayNightCycleDuration = 1440            //total cycle duration in seconds, default is 1440 (24 minutes)
DayNightCycle = [ [sunny6_0,sunny4_22.5,sunny2_45,lessnukedsunny_90,wet_135,wet2_150,wet3_180,wet_180,sunny7_180,sunset_180,sunset3_180,night_4_180,night_2_210,night_3_240,night_7_270,night_1_300,night_6_330,night_5_360]]
//array of weather sets, all weather sets must have the same amount of visual zones for better results
//other example, uncomment to use
//DayNightCycleDuration = 300
//DayNightCycle = ((sunny6_0,sunny4_22.5),(sunny2_45,lessnukedsunny_90),(wet_135,wet2_150),(wet3_180,wet_180),(sunny7_180),(sunset_180,sunset3_180),(night_4_180,night_2_210),(night_3_240,night_7_270),(night_1_300,night_6_330),(night_5_360, night_1_300))

[Features: Headlights]
HeadlightTweaksEnabled = 0              //enables headlight projection and toggling emissive textures with headlights
AutomaticHeadlights = 0                 //toggles headlights on when it's dark and off when not (headlights are always on when set to zero)
AutomaticHeadlightsField = 0x60         //brightness thresold field offset in current visual zone (0x60/64/68/6c = SunColourR/G/B/A, 0x70/74/78/7c = AmbientColourR/G/B/A)
AutomaticHeadlightsThresold = 0.6       //brightness thresold value, each car will use a slightly different value near this
ProjectionCutoff = 1                    //controls whether headlights projection is turned off when it's too bright regardless of headlights state
ProjectionCutoffField = 0x60            //brightness thresold field offset in current visual zone (0x60/64/68/6c = SunColourR/G/B/A, 0x70/74/78/7c = AmbientColourR/G/B/A)
ProjectionCutoffThresold = 0.5          //brightness thresold value

[Features: Aftermarket Menu]
ExtendAftermarketMenu = 0               //include more parts in the Aftermarket menu (experimental)
AllowMoreCarbonParts = 0                //includes carbon variants in the Aftermarket menu
AddMaterialToggle = 0                   //use button 2 in the Aftermarket menu to toggle between materials (NONE, CARBON, CHROME, MAGSILVER, GLASS, GUNMETAL, UNUSED3, UNUSED4, UNUSED5)
MaterialsCount = 2                      //material list cap
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
				getConfigSetting("AllowMoreCarbonParts", line, &AllowMoreCarbonParts);
                getConfigSetting("AddMaterialToggle", line, &AddMaterialToggle);
				getConfigSetting("MaterialsCount", line, &MaterialsCount);

                getConfigSetting("SliderEnhancements", line, &SliderEnhancements);
                getConfigSetting("SliderBoostMultiplier", line, &SliderBoostMultiplier);
                getConfigSetting("SliderSlowMultiplier", line, &SliderSlowMultiplier);
                getConfigSetting("VinylFastStep", line, &VinylFastStep);
                getConfigSetting("VinylStep", line, &VinylStep);

                getConfigSetting("HeadlightShatterFix", line, &HeadlightShatterFix);
                getConfigSetting("EffectsEnhancements", line, &EffectsEnhancements);
                getConfigSettingVltHash("ExhaustSmokeEffect", line, &ExhaustSmokeEffect);
                getConfigSettingVltHash("NOSEffect", line, &NOSEffect);
                getConfigSettingVltHash("NOSTrailEffect", line, &NOSTrailEffect);
                getConfigSettingVltHash("BackfireEffect", line, &BackfireEffect);
                getConfigSettingVltHash("MissShiftEffect", line, &MissShiftEffect);

                getConfigSetting("EnableAllCameras", line, &EnableAllCameras);
                getConfigSetting("EnableOverlay", line, &EnableOverlay);
                getConfigSetting("ShowOverlayOnGameStart", line, &ShowOverlay);

                getConfigSetting("ReadCarListFromAttrib", line, &ReadCarListFromAttrib);
                getConfigSetting("RemoveStockCarRecords", line, &RemoveStockCarRecords);
                getConfigSetting("Remove20CarLimit", line, &Remove20CarLimit);

                getConfigSetting("AddFreeroamMusic", line, &AddFreeroamMusic);
                getConfigSetting("FreeroamMusicType", line, (int*)&currentBitfield);

                getConfigSetting("RestoreDecalsEditing", line, &AddDecalsSelection);

				getConfigSetting("AllowPerformanceDowngrade", line, &AllowPerformanceDowngrade);

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
            //using namespace Inputs;
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