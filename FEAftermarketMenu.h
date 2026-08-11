#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"

namespace FEAftermarketMenu {

    //auto FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
    //auto FEScreenManager__GetScreen = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
    //auto FE__AptMenuTree__AddLevel = (void* (__fastcall*)(DWORD this_, DWORD unaff, bNode*, DWORD)) 0x005563c0;
    //auto bTList_destructor = (void(__fastcall*)(bNode * this_)) 0x00553aa0;
    //auto bList__traverse__getlength = (DWORD(__fastcall*)(bNode * this_, DWORD unaff, DWORD)) 0x004a76f0;

    auto StandardSelectablePart__GetPartsList_005d1b00 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD)) 0x005d1b00;
    auto AutosculptSelectablePart__GetPartsList_005d40e0 = (void(__cdecl*)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)) 0x005d40e0;

    //auto vltGet_car_004de2a0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD carNameHash, DWORD)) 0x004de2a0;
    //auto lowercaseVltHash_0045d950 = (void(__cdecl*)(char* toHash)) 0x0045d950;
    //auto FUN_getCollectionFromPointer_005cd3c0 = (void(__fastcall*)(DWORD * this_, DWORD unaff, DWORD*, DWORD)) 0x005cd3c0;


    void __cdecl LoadAftermarketMenu() {
        bNode local_14;
        bNode* current;
        FETreeDataNode* curTreeNode;

        //TODO car-specific settings

        //if (CurrentFECarPTR_012ac35c != 0) {

            //DWORD car_PTR[4];
            //DWORD car_parts_PTR[4];
            //DWORD car_part_array[4];


            //auto playerCar = ((SavegameCar*)(*(DWORD*)UserProfile_Player1 + 0x88))[*(DWORD*)(FECarHeaven_012ac318 + 0x10)];


            //cout << "current pvehicle " << (DWORD*)playerCar.pvehicle << "\n";
            //vltGet_car_004de2a0(car_PTR, 0, playerCar.pvehicle, 0);

            //cout << "car@" << (DWORD*)car_PTR[1] << "\n";
            //FUN_getCollectionFromPointer_005cd3c0(car_parts_PTR, 0, (DWORD*)(car_PTR[1] + 0xc), 0);

            //cout << "car_parts@" << (DWORD*)car_parts_PTR[1] << "\n";
            //Attrib::Instance::Get(car_parts_PTR, 0, car_part_array, 0x4A1C84F4); //wheel_spoke_rear_paint_swatch

            //cout << "Array wheel_spoke_... : " << car_part_array[0] <<" ; " << car_part_array[1] <<" ; size=" << car_part_array[2] << "\n";
        //        for (int i = 0; i < car_part_array[2]; i++) cout << "[" << i << "] = " << (DWORD*)Attrib::Attribute::GetInternalPointer(car_part_array, 0, i) <<"\n";
            //}

        local_14.next = &local_14;
        local_14.prev = &local_14;

        for (int i = 0; i < Settings::AftermarketMenu.size(); i++) {
            curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
            if (curTreeNode != NULL) {
                curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, Settings::AftermarketMenu.at(i).first, Settings::AftermarketMenu.at(i).second, 0xffffffff, 0xffffffff);
                current = &curTreeNode->node;
                (local_14.next)->prev = current;
                current->next = local_14.next;
                current->prev = &local_14;
                local_14.next = current;
            }
        }

        FE__AptMenuTree__AddLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &local_14, 0);
        bTList_destructor(&local_14);

    }

    DWORD __fastcall GetCurrentSlotID(DWORD this_) {
        switch (*(DWORD*)(this_ + 0x58)) {
            //original menu
        case 0x93:
            return 8;
        case 0x94:
            return 0x4e;
        case 0x95:
        case 0x96:
            //wheel front and all wheels
            return 0x5f;
        case 0x97:
            return 0x60;
        case 0x98:
            return 0x5b;
        case 0x99:
            return 0x44;
        }
        return *(DWORD*)(this_ + 0x58);
    }

    DWORD __fastcall getPartsList(DWORD this_) {

        DWORD PartID = GetCurrentSlotID(this_);
        DWORD PartID2 = 0x73; //only works fine for wheels
        bool singlePart = true;

        bNode* this_00 = (bNode*)(this_ + 0x50);

        bTList_destructor(this_00);
        *(DWORD*)(this_ + 0x5c) = 0;

        if (PartID >= 0x100) {
            singlePart = false;
            PartID2 = (PartID >> 8) & 0xFF;
            PartID = PartID & 0xFF;
        }


        if (*(DWORD*)(this_ + 0x58) == 0x99 //exhaust
            || (0x67 <= *(DWORD*)(this_ + 0x58) && *(DWORD*)(this_ + 0x58) < 0x73)) { //non body parts
            StandardSelectablePart__GetPartsList_005d1b00(PartID, (int)this_00, 0, 0);
        }
        else {
            if (*(DWORD*)(this_ + 0x58) == 0x95) {
                PartID2 = 0x60; //all wheels
                singlePart = false;
            }
            AutosculptSelectablePart__GetPartsList_005d40e0(PartID, (int)this_00, 0, 0, singlePart, PartID2);
        }

        return bList__traverse__getlength(this_00, 0, 0x0);
    }


    DWORD __cdecl getAutosculptZone(CarPartID id) {
        switch (id) {
        case BUMPER_FRONT:
            return RegionFrontBumper; //0 
        case BUMPER_REAR:
            return RegionRearBumper; //1
        case SKIRT_LEFT:
        case SKIRT_RIGHT:
            return RegionSkirtLeft; //2
        case WHEEL_FRONT:
            return RegionRimsFront; //4
        case WHEEL_REAR:
            return RegionRimsRear; //5
        case DOOR_LEFT:
        case DOOR_RIGHT:
            return RegionDoorLeft;
        case DOOR_REAR_LEFT:
        case DOOR_REAR_RIGHT:
            return RegionDoorRearLeft;
        case FENDER_FRONT_LEFT:
        case FENDER_FRONT_RIGHT:
            return RegionFenderFrontLeft; //10
        case BODY:
            return RegionBody; //12
        case HOOD:
            return RegionHoods; //13
        case SPOILER:
            return RegionSpoilers; //14
        case ROOFSCOOP:
            return RegionRoofScoops; //15
        case ROOF:
            return RegionRoofHeight; //16
        case EXHAUST:
            return RegionExhaust; //17
        case WINDOW_FRONT:
            return RegionFrontWindow;
        case WINDOW_REAR:
            return RegionRearWindow;
        case WINDOW_FRONT_LEFT:
            return RegionFrontLeftWindow;
        case WINDOW_FRONT_RIGHT:
            return RegionFrontRightWindow;
        case WINDOW_REAR_LEFT:
            return RegionRearLeftWindow;
        case WINDOW_REAR_RIGHT:
            return RegionRearRightWindow;
        case BUMPER_REAR_EXTRA:
            return RegionRearBumperExtra;
        case BUMPER_FRONT_EXTRA:
            return RegionFrontBumperExtra; //29
        default:
            return 3; //40 default but causes crashes
        }
    }



    int __cdecl SetSecondarySlot_PartsSelector(char* s1, char* s2) { //I DONT KNOOOOOOWWWW
        if (!strncmp(s1, "SKIRT", 5)) return 0;
        if (!strncmp(s1, "BRAKEL", 6)) return 0;
        if (!strncmp(s1, "HEADL", 5)) return 0;
        if (!strncmp(s1, "DOOR", 4)) return 0;
        if (!strncmp(s1, "SIDE", 4)) return 0;

        if ((strcmp(s1, s2))) {
            //if (!strncmp(s1, "WHEEL", 5) || !strncmp(s2, "WHEEL", 5)) { //how does this not work ?????????????????????????????????????????????????????????????????????
            if (
                (strcmp(s1, "WHEEL") || strcmp(s2, "WHEEL_REAR")) &&
                (strcmp(s1, "WHEEL_REAR") || strcmp(s2, "WHEEL"))
                )
                return 1; //wheels
            //}

            if (!strncmp(s1, "SEAT", 4) || !strncmp(s2, "SEAT", 4)) return 1; //seats
        }

        return 0; //most parts
    }

    bool(__fastcall* DBCarPart_IsNotPaintable)(DBCarPart*);
    DWORD(__cdecl* RideInfo__FindPaintSlotForPartSlot)(int slotID, int hitCount) = (DWORD(__cdecl*)(int slotID, int hitCount)) 0; //ePaintSlot
    bool __fastcall DBCarPart_IsNotPaintable_cf_allslots_wrapper(DBCarPart* _this, DWORD, int slotID) { //slot is added!!!
        auto paintSlot = RideInfo__FindPaintSlotForPartSlot(slotID, 0);
        if (paintSlot == ePaintSlot::INVALID_PAINT_SLOT
            || paintSlot == ePaintSlot::CUSTOM_PAINT_SLOT_RIMS_FRONT
            || paintSlot == ePaintSlot::CUSTOM_PAINT_SLOT_RIMS_REAR
            || paintSlot == ePaintSlot::CUSTOM_PAINT_SLOT_WINDOW_TINT
            || paintSlot == ePaintSlot::CUSTOM_PAINT_SLOT_ROLLCAGE
            ) return true;
        return DBCarPart_IsNotPaintable(_this);
    }



    auto FEAftermarketStateManager__GetPart = (DWORD(__fastcall*)(DWORD th, DWORD, DWORD)) 0;

    void __fastcall FEAftermarketCycleMaterials(DWORD FEAftermarketStateManager) {

        DWORD part = FEAftermarketStateManager__GetPart(FEAftermarketStateManager, 0, *(DWORD*)(FEAftermarketStateManager + 0x5c));
        if (part == NULL) return;

        DBCarPart* carpart = *(DBCarPart**)(part + 0x10);
        carpart->materialFlags++;
        if (carpart->materialFlags > Settings::MaterialsCount) carpart->materialFlags = 0;

        (**(void(__fastcall**)(DWORD part, DWORD edx, bool))((*(DWORD*)part + 0x14)))(part, 0, true); //Preview


        //int RideInfo = (int)imGuiStuff::FECarHeaven_GetRideInfo(*FECarHeaven);
        //printf("RideInfo %08X", RideInfo);

        //int slot = FEAftermarketMenu::GetCurrentSlotID(FEAftermarketStateManager);
        //DBCarPart* partsTable = (DBCarPart*)(RideInfo + 0x3d0);
        //partsTable[slot].materialFlags++;
        //if (partsTable[slot].materialFlags > 8) partsTable[slot].materialFlags = 0;

        //imGuiStuff::FECarHeaven__SetRideInfo(*FECarHeaven, 0, (void*)RideInfo);
    }
}