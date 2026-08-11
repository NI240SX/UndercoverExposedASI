#pragma once
#include "Injector.h"
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"

//Dynamic load of available cars from vlted, regardless of the save game //TODO
namespace FECarList {
    //auto Attrib::Instance::Get = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*, DWORD)) 0x004609a0;
    //auto Attrib::Attribute::GetLength = (short(__fastcall*)(DWORD * this_)) 0x0045ea80;
    //auto Attrib::Attribute::GetInternalPointer = (DWORD * (__fastcall*)(DWORD * this_, DWORD unaff, DWORD)) 0x0045eaa0;
    //auto Attrib::RefSpec::constructor = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*)) 0x0045ced0;
    //auto Attrib::Instance::GetCollectionKey = (DWORD(__fastcall*)(Attrib::Instance * this_)) 0x0045cbe0;
    //auto Attrib::RefSpec::Clean = (void(__fastcall*)(DWORD * this_)) 0x004628f0;

    bool* IgnorePlayerUsable_012bb1d8 = (bool*)0x012bb1d8;

    //FEPlayerCarDB
    auto CreateNewCarRecord = (FECarRecord * (__fastcall*)(void*)) 0x0050f220;
    auto CreateNewCustomizationRecord = (FECustomizationRecord * (__fastcall*)(void*)) 0x00513880;

    //FECarRecord
    auto GetType = (int(__fastcall*)(FECarRecord*)) 0x00507710;
    auto FECarRecord__GetNameHash_005079e0 = (DWORD(__cdecl*)(DWORD)) 0x005079e0;
    auto FECarRecord__GetManufacturerName_00507d10 = (char* (__cdecl*)(DWORD)) 0x00507d10;
    auto FECarRecord__GetManuLogoHash_00502230 = (DWORD(__cdecl*)(char*, bool)) 0x00502230;
    auto eastl__vector__InsertValue = (void(__fastcall*)(DWORD*, DWORD edx, DALVehicleCommands::CarInfo*)) 0x005199f0;

    FECarRecord* FECarRecordArray = NULL;
    int FECarRecordArraySize = 0;

    bool doLoadCarsFromPVehicle = false; //TODO add in config + breaks on the Reventon and Cayman 09

constexpr auto MAX_CARS_IN_SAVEGAME = 330;
constexpr auto fe_carlist = 0xCDCA884C;
constexpr auto StockCars = 0xA28B3B3D;
constexpr auto PlayerUsable = 0xF099B6AC;

    void __fastcall DALVehicleCommands__GetStockCars__Execute(DWORD* param_1) {

        DALVehicleCommands::CarInfo local_28;

        param_1[3] = param_1[2]; //vector stuff


        if (doLoadCarsFromPVehicle) {
            Attrib::Class* clazz = Attrib::Database::GetClass(*Attrib::DatabasePrivate, 0, getVltHash("pvehicle"));

            int NumValidPVehicles = -1;

            for (int j = 0; j < clazz->mPrivates->mCollections.__inherit_0.mTableSize; j++) {

                Attrib::Gen::pvehicle pvehicle;
                Attrib::Gen::pvehicle::constructor(&pvehicle, 0, clazz->mPrivates->mCollections.__inherit_0.mTable[j].mKey, 0);

                Attrib::Gen::frontend frontend;
                if ((pvehicle.mCollection != NULL) && (((byte)pvehicle.mFlags & 1) == 0)) {
                    Attrib::Gen::frontend::constructor(&frontend, 0, &(pvehicle.mLayoutPtr)->frontend, 0);
                    auto pcVar1 = ((pvehicle.mLayoutPtr)->MODEL).mString;

                    //cout << "pvehicle exists: " << (DWORD*)(pvehicle[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                    bool bVar2 = true;
                    //bVar2 = FUN_00507030(pvehicle);
                    if ((bVar2) && (frontend.mCollection != NULL)) {
                        //cout << "frontend exists: "<< (DWORD*)(frontend[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";


                        if (((pcVar1 != NULL)) && (((*pcVar1 != '\0' && (((byte)pvehicle.mFlags & 1) == 0))))) {
                            NumValidPVehicles++;
                            auto PlayerUsable_00 = Attrib::Instance::GetAttributePointer((Attrib::Instance*)&pvehicle, 0, PlayerUsable, 0);
                            if (PlayerUsable_00 != NULL && !*IgnorePlayerUsable_012bb1d8) {
                                if (!*(bool*)PlayerUsable_00) {
                                    continue;
                                }
                            }
                            //cout << "player usable\n";

                            auto frontendKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&frontend);
                            auto pvehicleKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&pvehicle);

                            //cout << "pvehicle " << pvehicle.mLayoutPtr->CollectionName << "\n";
                            //cout << "frontend " << frontend.mLayoutPtr->CollectionName << "\n";

                            local_28.mode = DALVehicleCommands::RaceMode::MODE_MAX;
                            local_28.damage = 0.0;
                            local_28.carNameHash = FECarRecord__GetNameHash_005079e0(frontendKey); //__cdecl //FECarRecord::GetNameHash_0050fb30(this);
                            auto uVar3 = frontendKey; //FECarRecord::GetFEKey_00507900(this);
                            local_28.carManuLogoHash = FECarRecord__GetManuLogoHash_00502230(FECarRecord__GetManufacturerName_00507d10(frontendKey), false); //__cdecl __cdecl
                            local_28.carLogoHash_pvehicleKey = pvehicleKey;
                            local_28.handle = MAX_CARS_IN_SAVEGAME + NumValidPVehicles;
                            local_28.drivetrain = DALVehicleCommands::DriveTrain::DRIVETRAIN_FWD; //unused
                            switch (frontend.mLayoutPtr->region) {
                            case 0:
                                local_28.region = 0x10000;
                                break;
                            case 1:
                                local_28.region = 0x20000;
                                break;
                            case 2:
                                local_28.region = 0x40000;
                                break;
                            case 3:
                                local_28.region = 0x80000;
                                break;
                            default:
                                local_28.region = 0;
                            }
                            local_28.carCost = (frontend.mLayoutPtr)->Cost;
                            eastl__vector__InsertValue(param_1 + 2, 0, &local_28);
                        }
                    }
                    Attrib::Instance::destructor((Attrib::Instance*)&frontend);
                }
                Attrib::Instance::destructor((Attrib::Instance*)&pvehicle);
            }

            return;
        }


        //DWORD carlist_node[4];
        //DWORD StockCarsArray[4];
        Attrib::Gen::car_list carlist_node;
        Attrib::Attribute StockCarsArray;

        Attrib::Gen::car_list::constructor(&carlist_node, 0, fe_carlist, 0);
        Attrib::Instance::Get(&carlist_node, 0, &StockCarsArray, StockCars);

        auto totalCars = Attrib::Attribute::GetLength(&StockCarsArray);

        //cout << "DALVehicleCommands::GetStockCars\n";
        //cout << "Total cars " << totalCars << "\n";

        for (int i = 0; i < totalCars; i++) {

            Attrib::RefSpec* currentVal = (Attrib::RefSpec*)Attrib::Attribute::GetInternalPointer(&StockCarsArray, 0, i);
            if (currentVal == 0) continue;

            //printf("Array value exists\n");

            //DWORD auStack_68[4];
            Attrib::RefSpec auStack_68;
            Attrib::RefSpec::constructor(&auStack_68, 0, currentVal);

            //printf("requested pvehicle 0x%08X\n", auStack_68[1]);

            //DebugBreak();

            Attrib::Gen::pvehicle pvehicle;
            Attrib::Gen::pvehicle::constructor(&pvehicle, 0, auStack_68.mCollectionKey, 0);

            Attrib::Gen::frontend frontend;


            //if (pvehicle.mLayoutPtr != NULL) printf("gotten pvehicle %s\n", pvehicle.mLayoutPtr->CollectionName);


            if ((pvehicle.mCollection != NULL) && (((byte)pvehicle.mFlags & 1) == 0)) {
                Attrib::Gen::frontend::constructor(&frontend, 0, &(pvehicle.mLayoutPtr)->frontend, 0);
                auto pcVar1 = ((pvehicle.mLayoutPtr)->MODEL).mString;

                //cout << "pvehicle exists: " << (DWORD*)(pvehicle[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                bool bVar2 = true;
                //bVar2 = FUN_00507030(pvehicle);
                if ((bVar2) && (frontend.mCollection != NULL)) {
                    //cout << "frontend exists: "<< (DWORD*)(frontend[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                    auto PlayerUsable_00 = Attrib::Instance::GetAttributePointer((Attrib::Instance*)&pvehicle, 0, PlayerUsable, 0);
                    if (PlayerUsable_00 != NULL && !*IgnorePlayerUsable_012bb1d8) {
                        if (!*(bool*)PlayerUsable_00) continue;
                    }
                    //cout << "player usable\n";

                    if (((pcVar1 != NULL)) && (((*pcVar1 != '\0' && (((byte)pvehicle.mFlags & 1) == 0))))) {

                        auto frontendKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&frontend);
                        auto pvehicleKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&pvehicle);

                        //cout << "pvehicle " << pvehicle.mLayoutPtr->CollectionName << "\n";
                        //cout << "frontend " << frontend.mLayoutPtr->CollectionName << "\n";


                        local_28.mode = DALVehicleCommands::RaceMode::MODE_MAX;
                        local_28.damage = 0.0;
                        // doesnt need a FECarRecord (use the inner function)
                        local_28.carNameHash = FECarRecord__GetNameHash_005079e0(frontendKey); //__cdecl //FECarRecord::GetNameHash_0050fb30(this);
                        // needs to be tweaked a bit
                        auto uVar3 = frontendKey; //FECarRecord::GetFEKey_00507900(this);
                        // doesnt need a FECarRecord
                        // doesnt need a FECarRecord
                        local_28.carManuLogoHash = FECarRecord__GetManuLogoHash_00502230(FECarRecord__GetManufacturerName_00507d10(frontendKey), false); //__cdecl __cdecl
                        // still doesnt need a FECarRecord
                        local_28.carLogoHash_pvehicleKey = pvehicleKey;
                        //local_28.carLogoHashpvehicleKey = FECarRecord__GetLogoHash_00507bd0(frontendKey); //unused anyways
                        // hmmmmmmmmmmmmmmmmmmm
                        local_28.handle = MAX_CARS_IN_SAVEGAME + i;
                        local_28.drivetrain = DALVehicleCommands::DriveTrain::DRIVETRAIN_FWD; //unused
                        // doable
                        switch (frontend.mLayoutPtr->region) {
                        case 0:
                            local_28.region = 0x10000;
                            break;
                        case 1:
                            local_28.region = 0x20000;
                            break;
                        case 2:
                            local_28.region = 0x40000;
                            break;
                        case 3:
                            local_28.region = 0x80000;
                            break;
                        default:
                            local_28.region = 0;
                        }
                        // who cares
                        //bVar7 = FECarRecord::IsPULCUnlocked_00508210(this);
                        //// who cares x2
                        //if ((bVar7) || (bVar7 = FECarRecord::IsCECar_00507f80(this), bVar7)) {
                        //    local_28.carCost = 0;
                        //}
                        //else {
                        //    local_28.carCost = (frontend.mLayoutPtr)->Cost;
                        //}
                        //if (iVar2 == CARNAME_ACURA_INTEGRALS) {
                        //    local_28.carCost = 0;
                        //}
                        local_28.carCost = (frontend.mLayoutPtr)->Cost;
                        eastl__vector__InsertValue(param_1 + 2, 0, &local_28);

                    }
                }
                Attrib::Instance::destructor((Attrib::Instance*)&frontend);
            }
            Attrib::Instance::destructor((Attrib::Instance*)&pvehicle);
            if (auStack_68.mCollectionPtr != 0) {
                Attrib::RefSpec::Clean(&auStack_68);
            }






        }

    }

    void fillInFECarRecords() {
        if (doLoadCarsFromPVehicle) {

            Attrib::Class* clazz = Attrib::Database::GetClass(*Attrib::DatabasePrivate, 0, getVltHash("pvehicle"));

            FECarRecordArray = new FECarRecord[clazz->mPrivates->mCollections.__inherit_0.mTableSize];

            int NumValidPVehicles = 0;

            for (int j = 0; j < clazz->mPrivates->mCollections.__inherit_0.mTableSize; j++) {

                Attrib::Gen::pvehicle pvehicle;
                Attrib::Gen::pvehicle::constructor(&pvehicle, 0, clazz->mPrivates->mCollections.__inherit_0.mTable[j].mKey, 0);

                Attrib::Gen::frontend frontend;
                if ((pvehicle.mCollection != NULL) && (((byte)pvehicle.mFlags & 1) == 0)) {
                    Attrib::Gen::frontend::constructor(&frontend, 0, &(pvehicle.mLayoutPtr)->frontend, 0);
                    auto pcVar1 = ((pvehicle.mLayoutPtr)->MODEL).mString;

                    //cout << "pvehicle exists: " << (DWORD*)(pvehicle[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                    bool bVar2 = true;
                    //bVar2 = FUN_00507030(pvehicle);
                    if ((bVar2) && (frontend.mCollection != NULL)) {
                        //cout << "frontend exists: "<< (DWORD*)(frontend[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                        //auto PlayerUsable_00 = Attrib::Instance::GetAttributePointer((Attrib::Instance*)&pvehicle, 0, PlayerUsable, 0);
                        //if (PlayerUsable_00 != NULL && !*IgnorePlayerUsable_012bb1d8) {
                        //    if (!*(bool*)PlayerUsable_00) continue;
                        //}
                        //cout << "player usable\n";

                        if (((pcVar1 != NULL)) && (((*pcVar1 != '\0' && (((byte)pvehicle.mFlags & 1) == 0))))) {

                            auto frontendKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&frontend);
                            auto pvehicleKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&pvehicle);

                            //cout << "pvehicle " << pvehicle.mLayoutPtr->CollectionName << "\n";
                            //cout << "frontend " << frontend.mLayoutPtr->CollectionName << "\n";
                            FECarRecordArray[NumValidPVehicles].handle = MAX_CARS_IN_SAVEGAME + NumValidPVehicles;
                            FECarRecordArray[NumValidPVehicles].frontend = frontendKey;
                            FECarRecordArray[NumValidPVehicles].pvehicle = pvehicleKey;
                            FECarRecordArray[NumValidPVehicles].preset = 0;
                            switch (frontend.mLayoutPtr->region) { //region | stock
                            case 0:
                                FECarRecordArray[NumValidPVehicles].filterBits = 0x10000 | 1;
                                break;
                            case 1:
                                FECarRecordArray[NumValidPVehicles].filterBits = 0x20000 | 1;
                                break;
                            case 2:
                                FECarRecordArray[NumValidPVehicles].filterBits = 0x40000 | 1;
                                break;
                            case 3:
                                FECarRecordArray[NumValidPVehicles].filterBits = 0x80000 | 1;
                                break;
                            default:
                                FECarRecordArray[NumValidPVehicles].filterBits = 0 | 1;
                            }
                            FECarRecordArray[NumValidPVehicles].customization = -1;
                            FECarRecordArray[NumValidPVehicles].careerHandle = -1;
                            FECarRecordArray[NumValidPVehicles].isPresetSkin = 0;
                            FECarRecordArray[NumValidPVehicles].isPinkSlip = 0;
                            NumValidPVehicles++;
                        }
                    }
                    Attrib::Instance::destructor((Attrib::Instance*)&frontend);
                }
                Attrib::Instance::destructor((Attrib::Instance*)&pvehicle);
            }

            FECarRecordArraySize = NumValidPVehicles;



            return;
        }


        //DWORD carlist_node[4];
        //DWORD StockCarsArray[4];
        Attrib::Gen::car_list carlist_node;
        Attrib::Attribute StockCarsArray;

        Attrib::Gen::car_list::constructor(&carlist_node, 0, fe_carlist, 0);
        Attrib::Instance::Get(&carlist_node, 0, &StockCarsArray, StockCars);

        auto totalCars = Attrib::Attribute::GetLength(&StockCarsArray);
        //cout << "Total cars " << totalCars << "\n";

        FECarRecordArray = new FECarRecord[totalCars];
        FECarRecordArraySize = totalCars;

        switch (CURRENT_GAME_VERSION) {
        case UC_1_21:
            utils::set(0x00516e69, FECarRecordArray); //replace array in GetCheapestStockCar, only ever used if the player has no pink slip cars and gets busted with all his cars (= impossible in uce)
            utils::nop(0x00516e6d, 5);
            utils::set(0x00516f40, FECarRecordArraySize);
            break;
        case UC_1_0:
            utils::set(0x0052cb26, FECarRecordArray); //replace array in GetCheapestStockCar, only ever used if the player has no pink slip cars and gets busted with all his cars (= impossible in uce)
            utils::nop(0x0052cb2a, 5);
            utils::set(0x0052cbfc, FECarRecordArraySize);
            break;
        }

        for (int i = 0; i < totalCars; i++) {

            Attrib::RefSpec* currentVal = (Attrib::RefSpec*)Attrib::Attribute::GetInternalPointer(&StockCarsArray, 0, i);
            if (currentVal == 0) continue;

            Attrib::RefSpec auStack_68;
            Attrib::RefSpec::constructor(&auStack_68, 0, currentVal);

            Attrib::Gen::pvehicle pvehicle;
            Attrib::Gen::pvehicle::constructor(&pvehicle, 0, auStack_68.mCollectionKey, 0);

            Attrib::Gen::frontend frontend;

            if ((pvehicle.mCollection != NULL) && (((byte)pvehicle.mFlags & 1) == 0)) {
                Attrib::Gen::frontend::constructor(&frontend, 0, &(pvehicle.mLayoutPtr)->frontend, 0);
                auto pcVar1 = ((pvehicle.mLayoutPtr)->MODEL).mString;

                //cout << "pvehicle exists: " << (DWORD*)(pvehicle[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                bool bVar2 = true;
                //bVar2 = FUN_00507030(pvehicle);
                if ((bVar2) && (frontend.mCollection != NULL)) {
                    //cout << "frontend exists: "<< (DWORD*)(frontend[0]) << " (key = " << (DWORD*)(*(DWORD*)(pvehicle[0] + 0x10)) << ")\n";

                    auto PlayerUsable_00 = Attrib::Instance::GetAttributePointer((Attrib::Instance*)&pvehicle, 0, PlayerUsable, 0);
                    if (PlayerUsable_00 != NULL && !*IgnorePlayerUsable_012bb1d8) {
                        if (!*(bool*)PlayerUsable_00) continue;
                    }
                    //cout << "player usable\n";

                    if (((pcVar1 != NULL)) && (((*pcVar1 != '\0' && (((byte)pvehicle.mFlags & 1) == 0))))) {

                        auto frontendKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&frontend);
                        auto pvehicleKey = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&pvehicle);

                        //cout << "pvehicle " << pvehicle.mLayoutPtr->CollectionName << "\n";
                        //cout << "frontend " << frontend.mLayoutPtr->CollectionName << "\n";

                        FECarRecordArray[i].handle = MAX_CARS_IN_SAVEGAME + i;
                        FECarRecordArray[i].frontend = frontendKey;
                        FECarRecordArray[i].pvehicle = pvehicleKey;
                        FECarRecordArray[i].preset = 0;
                        switch (frontend.mLayoutPtr->region) { //region | stock
                        case 0:
                            FECarRecordArray[i].filterBits = 0x10000 | 1;
                            break;
                        case 1:
                            FECarRecordArray[i].filterBits = 0x20000 | 1;
                            break;
                        case 2:
                            FECarRecordArray[i].filterBits = 0x40000 | 1;
                            break;
                        case 3:
                            FECarRecordArray[i].filterBits = 0x80000 | 1;
                            break;
                        default:
                            FECarRecordArray[i].filterBits = 0 | 1;
                        }
                        FECarRecordArray[i].customization = -1;
                        FECarRecordArray[i].careerHandle = -1;
                        FECarRecordArray[i].isPresetSkin = 0;
                        FECarRecordArray[i].isPinkSlip = 0;
                    }
                }
                Attrib::Instance::destructor((Attrib::Instance*)&frontend);
            }
            Attrib::Instance::destructor((Attrib::Instance*)&pvehicle);
            if (auStack_68.mCollectionPtr != 0) {
                Attrib::RefSpec::Clean(&auStack_68);
            }

        }
    }

    FECarRecord* __fastcall GetCarRecordByHandle(FECarRecord* FEPlayerCarDB, DWORD, int handle) {
        if (FECarRecordArray == NULL) fillInFECarRecords();
        for (int i = 0; i < FECarRecordArraySize; i++) {
            if (FECarRecordArray[i].handle == handle) return &FECarRecordArray[i];
        }
        //printf("existing car record\n");
        for (int i = 0; i < MAX_CARS_IN_SAVEGAME; i++) {
            if (FEPlayerCarDB[i].handle == handle) return &FEPlayerCarDB[i];
        }
        return NULL;
    }

    __declspec(naked) void GetCarRecordByHandle_safe_wrapper() { //fixes undefined behavior causing a crash due to ECX expected unchanged
        __asm {
            push ebx;
            push ecx;
            push edx;
            push esi;
            push edi;

            push edi;

            call GetCarRecordByHandle;

            pop edi;
            pop esi;
            pop edx;
            pop ecx;
            pop ebx;
            ret 0x4;
        }
    }

    FECarRecord* __stdcall GetCarRecordByHandleStatic(int handle) {
        return GetCarRecordByHandle((FECarRecord*)(*UserProfile_Player1 + 0x88), 0, handle);
    }

    FECarRecord* __fastcall GetCarRecordByFEKey(FECarRecord* FEPlayerCarDB, DWORD, int FEKey) {
        if (FECarRecordArray == NULL) fillInFECarRecords();
        for (int i = 0; i < FECarRecordArraySize; i++) {
            if (FECarRecordArray[i].frontend == FEKey) return &FECarRecordArray[i];
        }
        for (int i = 0; i < MAX_CARS_IN_SAVEGAME; i++) {
            if (FEPlayerCarDB[i].frontend == FEKey) return &FEPlayerCarDB[i];
        }
        return NULL;
    }

    FECarRecord* __fastcall GetStockCarRecord(FECarRecord* FEPlayerCarDB, DWORD, int pvehicle) {
        if (FECarRecordArray == NULL) fillInFECarRecords();
        for (int i = 0; i < FECarRecordArraySize; i++) {
            if (FECarRecordArray[i].pvehicle == pvehicle && !(((char)FECarRecordArray[i].filterBits) & 1)) return &FECarRecordArray[i];
        }
        for (int i = 0; i < MAX_CARS_IN_SAVEGAME; i++) {
            if (FEPlayerCarDB[i].pvehicle == pvehicle && !(((char)FEPlayerCarDB[i].filterBits) & 1)) return &FEPlayerCarDB[i];
        }
        return NULL;
    }

    FECarRecord* __fastcall GetCarByIndex(FECarRecord* FEPlayerCarDB, DWORD, int index) {
        if (FECarRecordArray == NULL) fillInFECarRecords();
        if (index < MAX_CARS_IN_SAVEGAME) return &FEPlayerCarDB[index];
        if (index < MAX_CARS_IN_SAVEGAME + FECarRecordArraySize) return FECarRecordArray + index - MAX_CARS_IN_SAVEGAME;
        return NULL;
    }

    FECarRecord* __fastcall CreateCar(FECarRecord* FEPlayerCarDB, DWORD, int handle, DWORD region) {
        FECarRecord* currentCar = GetCarRecordByHandle(FEPlayerCarDB, 0, handle);
        //printf("CreateCar called with handle %d, region %d, currentCar %p\n", handle, region, currentCar);
        if (currentCar != NULL) {
            FECarRecord* newCar = CreateNewCarRecord(FEPlayerCarDB);
            if (newCar != NULL) {
                //printf("Created new car record at %p\n", newCar);
                newCar->frontend = currentCar->frontend;
                newCar->pvehicle = currentCar->pvehicle;
                newCar->filterBits = currentCar->filterBits;
                FECustomizationRecord* customization = CreateNewCustomizationRecord(FEPlayerCarDB);
                if (customization != NULL) {
                    //printf("Created new customization record at %p\n", customization);
                    newCar->customization = customization->Handle;
                    newCar->filterBits = newCar->filterBits & 0xffff0000 | region;
                    customization->mType = GetType(currentCar);
                    (customization->mBluePrint).Type = GetType(currentCar);
                    //printf("Created new car with handle %d, customization handle %d, type %d\n", newCar->handle, customization->Handle, customization->mType);
                    return newCar;
                }
                newCar->handle = -1;
                //printf("Failed to create customization record, reverting new car handle to -1\n");
            }
            //printf("Failed to create new car record\n");
        }
        //printf("No existing car record found for handle %d, cannot create new car.",handle);
        return NULL;
    }

    bool __fastcall DALFeVehicle__GetCarNameHash(DWORD, DWORD, int* ret, int handle) {
        auto record = GetCarRecordByHandleStatic(handle);
        if (record != NULL) {
            *ret = FECarRecord__GetNameHash_005079e0(record->frontend);
            return true;
        }
        return false;
    }

    bool __fastcall DALFeVehicle__GetCarHandleByIndex(DWORD, DWORD, int* ret, int handle, int region) {
        auto record = GetCarRecordByHandleStatic(handle);
        if (record != NULL && record->handle != -1) { //TODO misses a region check
            *ret = record->handle;
            return true;
        }
        return false;
    }

    bool __fastcall DALFeVehicle__GetCarKeyByHandle(DWORD, DWORD, int* ret, int handle) {
        auto record = GetCarRecordByHandleStatic(handle);
        if (record != NULL) {
            if (record->preset != 0 || record->preset != -1) {
                *ret = record->preset;
            }
            else *ret = record->pvehicle;
            return true;
        }
        return false;
    }

    bool __fastcall DALFeVehicle__GetStockCarValue(DWORD, DWORD, int* ret, int handle) {
        auto record = GetCarRecordByHandleStatic(handle);
        if (record != NULL) {
            Attrib::Gen::frontend frontend;
            Attrib::Gen::frontend::constructor(&frontend, 0, (Attrib::RefSpec*)record->frontend, 0);
            *ret = frontend.mLayoutPtr->Cost;
            Attrib::Instance::destructor((Attrib::Instance*)&frontend);
            return true;
        }
        return false;
    }

    bool __fastcall DALFeVehicle__GetCarTier(DWORD, DWORD, int* ret, int handle) {
        auto record = GetCarRecordByHandleStatic(handle);
        if (record != NULL) {
            Attrib::Gen::pvehicle pvehicle;
            Attrib::Gen::pvehicle::constructor(&pvehicle, 0, record->pvehicle, 0);
            *ret = pvehicle.mLayoutPtr->RacingTier;
            Attrib::Instance::destructor((Attrib::Instance*)&pvehicle);
            return true;
        }
        return false;
    }

    auto CreateNewCustomCarDetourReturn = 0x005174a6;
    __declspec(naked) void CreateNewCustomCarDetour() {
        __asm {
            push eax;
            push ecx;
            push edx;

            //FEPlayerCarDB already in ecx
            push esi; //handle
            call GetCarRecordByHandle;

            mov edi, eax;

            pop edx;
            pop ecx;
            pop eax;

            xor ebx, ebx;
            jmp CreateNewCustomCarDetourReturn;
        }
    }

    auto BuildRideForPlayerDetourReturn = 0x0050fa12;
    __declspec(naked) void BuildRideForPlayerDetour() {
        __asm {
            //push ebx;
            push ecx;
            push edx;

            //FEPlayerCarDB already in ecx
            push ebp; //handle
            call GetCarRecordByHandle;

            mov esi, eax;

            pop edx;
            pop ecx;
            //pop ebx;

            mov ebx, ecx;

            jmp BuildRideForPlayerDetourReturn;
        }
    }

}