#pragma once
#include <sys/types.h>
#include <iostream>
#include "Injector.h"
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "AttribEditor.h"
#include "DayNight.h"
#include "FreeroamMusic.h"
#include "Settings.h"
#include "FEAftermarketMenu.h"

namespace ucetesting {
    using namespace std;

    //ok mostly
    namespace prints {
        void restoreDisabledPrint0(int v1, int v2, char* text, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6) {
            if (v1 == 7) return; //removes MassiveAdClient bloat
            printf(text, arg1, arg2, arg3, arg4, arg5, arg6);
            printf("\n");
        }

        void* restoreDisabledPrintFixupReturn;
        __declspec(naked) void restoreDisabledPrintFixup() {
            __asm {
                push ebp
                mov ebp, esp
                push esi
                mov esi, dword ptr[ebp + 0x4]
                mov ecx, esi
                jmp restoreDisabledPrintFixupReturn;
            }
        }



        void __cdecl replacePrint_006d9570(DWORD param, char* text, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6) {
            //cout << param << " - " << text << " - " << args << "\n";
            printf(text, arg1, arg2, arg3, arg4, arg5, arg6);
            printf("\n");
        }

        void __cdecl aptDebugPrint(char* str1, char* str2, int int3) {
            std::cout << "[AptDebug] " << str1 << " | " << str2 << " | " << int3 << "\n";
        }

        void __cdecl printfSimpleCdecl(char* text, DWORD* arg1, DWORD* arg2, DWORD* arg3, DWORD* arg4, DWORD* arg5, DWORD* arg6) {
            //cout << text << "\n";
            printf(text, arg1, arg2, arg3, arg4, arg5, arg6);
        }

        void __cdecl Fesl_Debug_replacement(DWORD, DWORD, char* print, DWORD vals) {
            vprintf(print, (va_list)&vals);
            std::cout << "\n";
        }


        void __cdecl somethingPrintName(char, DWORD, char* name) {
            std::cout << name << "\n";
        }


        void(__fastcall* CarSkinManager__NextTask)(DWORD ecx);
        void __fastcall CarSkinManager__NextTask_wrapper(DWORD ecx) {
            using namespace CarSkinManager;
            CarSkinManager__NextTask(ecx);
            const char* pcVar1;
            switch (*(eCarSkinTask*)(ecx + 0x34)) { //mCurrentTask
            case eCST_Finished:
                pcVar1 = "Finished\n";
                break;
            case eCST_Started:
                pcVar1 = "Started\n";
                break;
            case eCST_CanvasBasePaintFill:
                pcVar1 = "CanvasBasePaintFill\n";
                break;
            case eCST_LowerLayersBasePaintFill:
                pcVar1 = "LowerLayersBasePaintFill\n";
                break;
            case eCST_LowerLayersInitialize:
                pcVar1 = "LowerLayersInitialize\n";
                break;
            case eCST_CanvasInitialize:
                pcVar1 = "CanvasInitialize\n";
                break;
            case eCST_CanvasInitializeCompressed:
                pcVar1 = "CanvasInitializeCompressed\n";
                break;
            case eCST_CompressLowerLayers:
                pcVar1 = "CompressLowerLayers\n";
                break;
            case eCST_StepRasterizer:
                pcVar1 = "StepRasterizer\n";
                break;
            case eCST_Finalize:
                pcVar1 = "Finalize\n";
                break;
            case eCST_CopyLowerLayersToCanvas:
                pcVar1 = "CopyLowerLayersToCanvas\n";
                break;
            case eCST_CopyLowerLayersToCanvasCompressed:
                pcVar1 = "CopyLowerLayersToCanvasCompressed\n";
                break;
            case eCST_CopyToDestination:
                pcVar1 = "CopyToDestination\n";
                break;
            case eCST_CopyToDestinationNoCompression:
                pcVar1 = "CopyToDestinationNoCompression\n";
                break;
            default:
                return;
            }
            printf("RENDERING - CarSkinTask: %s", pcVar1);
        }
    }

    //TODO broken quotes
    /*namespace quotes {


        uint32_t* quotes;
        void RandomizeQuotes() {
            //    std::cout << "Quotes table pointer: " << quotes << "\n";


            std::uniform_int_distribution<> quoteTableIndex(1, 6);
            switch (quoteTableIndex(gen)) {
            case 1:
                //keep vanilla
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_A_1");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_B");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_C");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_D");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET1_E");
                break;
            case 2:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_B");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_C");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_D");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_D_1");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET2_E");
                break;
            case 3:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_B");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_B_1");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_C");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_D");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET3_D_1");
                break;
            case 4:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_A_1");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_B");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_B_1");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_C");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET4_C_1");
                break;
            case 5:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_B");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_C");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_C_1");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_D");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_E");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET5_E_1");
                break;
            case 6:
                quotes[0] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_A");
                quotes[1] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_B");
                quotes[2] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_C");
                quotes[3] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_D");
                quotes[4] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_E");
                quotes[5] = getBinHash((char*)"LOADSCREEN_QUOTES_SET6_F");
                break;
            }



        }


        void* quotesDetourReturn;
        __declspec(naked) void QuotesDetour() {
            __asm {
                push eax;
                mov eax, dword ptr[esp + 0x8];
                mov quotes, eax;
                push ebx;
                push ecx;
                push edx;
                call RandomizeQuotes;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp quotesDetourReturn;
            }
        }



    } //quotes TODO broken*/

    //some day maybe
    namespace regionCops {

        void __cdecl printPreset(UINT* preset) { //using UINT* to display as hex
            //    cout << "presetride read: " << preset << "\n";
        }
        void* interceptPresetrideSpawnReturn;
        __declspec(naked) void interceptPresetrideSpawn() {
            __asm {
                push eax;
                push ebx;
                push edx;
                push ecx;
                call printPreset;
                pop ecx;
                pop edx;
                pop ebx;
                pop eax;
                PUSH       EAX
                    PUSH       ECX
                    PUSH       0x27e73952
                    jmp interceptPresetrideSpawnReturn;
            }
        }

        void printCopFcn() {
            std::cout << "cop related function (?) entered \n";
        }

        void* copPresetTestReturn;
        __declspec(naked) void copPresetTest() {
            __asm {
                PUSH       0x0
                PUSH       0xEA91BC7F;
                LEA        ECX, [ESP + 0x10]
                    push eax;
                push ebx;
                push ecx;
                push edx;
                call printCopFcn;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp copPresetTestReturn;
            }
        }

        void* copPresetTest2Return;
        __declspec(naked) void copPresetTest2() {
            __asm {
                PUSH       0x08FAC0E7;
                LEA        ECX, [ESP + 0x44]
                    MOV        dword ptr[ESP + 0x5c], 0x0


                    push eax;
                push ebx;
                push ecx;
                push edx;
                call printCopFcn;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp copPresetTestReturn;
            }
        }

        void* copPresetTest3Return;
        __declspec(naked) void copPresetTest3() {
            __asm {
                PUSH       0x0
                PUSH       0xEA91BC7F;
                LEA        ECX, [ESP + 0x10]
                    push eax;
                push ebx;
                push ecx;
                push edx;
                call printCopFcn;
                pop edx;
                pop ecx;
                pop ebx;
                pop eax;
                jmp copPresetTestReturn;
            }
        }

    }

    //cancelled VinylsTransformMenu (decals do the same job)
    /*namespace VinylsTransformMenu {

        //NOT INCLUDED

        auto VinylCanSkew = (bool(__fastcall*)(void* this_)) 0x005d4c80;
        auto VinylHasMirror = (bool(__fastcall*)(void* this_)) 0x005cdd50;
        auto VinylNotFullbody = (bool(__fastcall*)(void* this_)) 0x005d2670;
        auto VinylHasColours = (bool(__fastcall*)(void* this_)) 0x005cdd10;

        auto FETreeDataNode__CopyLabel = (void* (__fastcall*)(FETreeDataNode * this_, DWORD unaff, char* name))0x00536620;

#define CUST_VINYL_MOVE 0xAF236425
#define CUST_VINYL_SCALE 0x93F58F16
#define CUST_VINYL_ROTATE 0x1135ADDD
#define CUST_VINYL_SKEW 0xAF269B48
#define CUST_VINYL_FLIP 0xAF1F7F19
#define CUST_VINYL_MIRROR 0x51E7EA9
#define CUST_VINYL_COLOR 0x92DACACD

        void __fastcall SetupVinylTransformMenu(void* this_, DWORD) {
            FETreeDataNode* curTreeNode;
            char* pcVar4;
            //DWORD uVar9;
            //DWORD uVar10;
            bNode local_14;
            bNode* current;
            char local_4;

            bool cannotSkew = VinylCanSkew(*(void**)((int)this_ + 0x50));
            bool hasMirror = VinylHasMirror(*(void**)((int)this_ + 0x50));
            bool notFullbody = VinylNotFullbody(*(void**)((int)this_ + 0x50));
            bool hasColours = VinylHasColours(*(void**)((int)this_ + 0x50));

            local_4 = 0;

            local_14.prev = &local_14;
            local_14.next = local_14.prev;

            if (notFullbody) {
                local_14.next = local_14.prev;

                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 1;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_MOVE);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->mValue = 0;
                    curTreeNode->mLabelHash = 0xffffffff;
                    curTreeNode->mData = 0xffffffff;
                    curTreeNode->mId = 0x9f;
                    curTreeNode->mType = 0;
                    curTreeNode->mpLabel = (char*)0x0;
                    curTreeNode->mpStringVal = 0;
                    curTreeNode->mbEnabled = 1;
                    FETreeDataNode__CopyLabel(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (bNode*)0x0;
                }
                else {
                    current = &curTreeNode->node;
                }
                (local_14.next)->prev = current;
                current->next = local_14.next;
                current->prev = &local_14;
                local_14.next = current;



                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 2;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_SCALE);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->mValue = 0;
                    curTreeNode->mLabelHash = 0xffffffff;
                    curTreeNode->mData = 0xffffffff;
                    curTreeNode->mId = 0xa0;
                    curTreeNode->mType = 0;
                    curTreeNode->mpLabel = (char*)0x0;
                    curTreeNode->mpStringVal = 0;
                    curTreeNode->mbEnabled = 1;
                    FETreeDataNode__CopyLabel(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (bNode*)0x0;
                }
                else {
                    current = &curTreeNode->node;
                }
                (local_14.next)->prev = current;
                current->next = local_14.next;
                current->prev = &local_14;
                local_14.next = current;



                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 3;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_ROTATE);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->mValue = 0;
                    curTreeNode->mLabelHash = 0xffffffff;
                    curTreeNode->mData = 0xffffffff;
                    curTreeNode->mId = 0xa1;
                    curTreeNode->mType = 0;
                    curTreeNode->mpLabel = (char*)0x0;
                    curTreeNode->mpStringVal = 0;
                    curTreeNode->mbEnabled = 1;
                    FETreeDataNode__CopyLabel(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (bNode*)0x0;
                }
                else {
                    current = &curTreeNode->node;
                }
                (local_14.next)->prev = current;
                current->next = local_14.next;
                current->prev = &local_14;
                local_14.next = current;


                if (!cannotSkew) {
                    curTreeNode = (FETreeDataNode*)
                        MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                    local_4 = 4;
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        curTreeNode = (FETreeDataNode*)0x0;
                    }
                    else {
                        curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa2, getLocalizedString(CUST_VINYL_SKEW), 0xffffffff, 0xffffffff);
                    }
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        current = (bNode*)0x0;
                    }
                    else {
                        current = &curTreeNode->node;
                    }
                    (local_14.next)->prev = current;
                    current->next = local_14.next;
                    current->prev = &local_14;
                    local_14.next = current;
                }


                local_4 = 0;
                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 5;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_FLIP);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->mValue = 0;
                    curTreeNode->mLabelHash = 0xffffffff;
                    curTreeNode->mData = 0xffffffff;
                    curTreeNode->mId = 0xa3;
                    curTreeNode->mType = 0;
                    curTreeNode->mpLabel = (char*)0x0;
                    curTreeNode->mpStringVal = 0;
                    curTreeNode->mbEnabled = 1;
                    FETreeDataNode__CopyLabel(curTreeNode, 0, pcVar4);
                }

                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (bNode*)0x0;
                }
                else {
                    current = &curTreeNode->node;
                }
                (local_14.next)->prev = current;
                current->next = local_14.next;
                current->prev = &local_14;
                local_14.next = current;


                if (hasMirror) {
                    curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                    local_4 = 6;
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        curTreeNode = (FETreeDataNode*)0x0;
                    }
                    else {
                        curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa4, getLocalizedString(CUST_VINYL_MIRROR), 0xffffffff, 0xffffffff);
                    }
                    if (curTreeNode == (FETreeDataNode*)0x0) {
                        current = (bNode*)0x0;
                    }
                    else {
                        current = &curTreeNode->node;
                    }
                    (local_14.next)->prev = current;
                    current->next = local_14.next;
                    current->prev = &local_14;
                    local_14.next = current;
                }
            }



            local_4 = 0;
            if (hasColours) {
                curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
                local_4 = 7;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    curTreeNode = (FETreeDataNode*)0x0;
                }
                else {
                    pcVar4 = getLocalizedString(CUST_VINYL_COLOR);
                    curTreeNode->vftable = FETreeDataNode_vftable;
                    curTreeNode->mValue = 0;
                    curTreeNode->mLabelHash = 0xffffffff;
                    curTreeNode->mData = 0xffffffff;
                    curTreeNode->mId = 0xa5;
                    curTreeNode->mType = 0;
                    curTreeNode->mpLabel = (char*)0x0;
                    curTreeNode->mpStringVal = 0;
                    curTreeNode->mbEnabled = 1;
                    FETreeDataNode__CopyLabel(curTreeNode, 0, pcVar4);
                }
                local_4 = 0;
                if (curTreeNode == (FETreeDataNode*)0x0) {
                    current = (bNode*)0x0;
                }
                else {
                    current = &curTreeNode->node;
                }
                (local_14.next)->prev = current;
                current->next = local_14.next;
                current->prev = &local_14;
                local_14.next = current;
            }




            curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
            local_4 = 8;
            if (curTreeNode == (FETreeDataNode*)0x0) {
                curTreeNode = (FETreeDataNode*)0x0;
            }
            else {
                curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa6, (char*)"ROBER LE MENU ITEM", 0xffffffff, 0xffffffff);
            }
            if (curTreeNode == (FETreeDataNode*)0x0) {
                current = (bNode*)0x0;
            }
            else {
                current = &curTreeNode->node;
            }
            (local_14.next)->prev = current;
            current->next = local_14.next;
            current->prev = &local_14;
            local_14.next = current;


            curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
            local_4 = 9;
            if (curTreeNode == (FETreeDataNode*)0x0) {
                curTreeNode = (FETreeDataNode*)0x0;
            }
            else {
                curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, 0xa7, (char*)"this fucking sucks", 0xffffffff, 0xffffffff);
            }
            if (curTreeNode == (FETreeDataNode*)0x0) {
                current = (bNode*)0x0;
            }
            else {
                current = &curTreeNode->node;
            }
            (local_14.next)->prev = current;
            current->next = local_14.next;
            current->prev = &local_14;
            local_14.next = current;








            int iVar3 = bList__traverse__getlength(&local_14, 0, 0x0);
            *(DWORD*)((int)this_ + 0x14) = *(DWORD*)((int)this_ + 4);

            DWORD vftable = *(DWORD*)this_;


            if (iVar3 < 1) {
                *(DWORD*)((int)this_ + 4) = 9;

                //              pcVar1 = *(code **)(*this + 0x14c); (*pcVar1)(this); 
                //(**(code **)(*this + 0x14c))(this); 
                //(**(code **)(*this + 0x144))(this);

                ((void(__fastcall*)(void*)) * (DWORD*)(vftable + 0x14c))   (this_);
                ((void(__fastcall*)(void*)) * (DWORD*)(vftable + 0x144))   (this_); //show popup "This vinyl is not customizable"
            }
            else {
                *(DWORD*)((int)this_ + 4) = 3;
                //(**(code **)(*this + 0x14c))(this);
                ((void(__fastcall*)(void*)) * (DWORD*)(vftable + 0x14c))   (this_); //0x594990

                auto this_00 = FEScreenManager__GetScreen(*FEScreenManager, 0, TreeMenu_Customize);
                FE__AptMenuTree__AddLevel(this_00, 0, &local_14, 0); //fucking dies (this_00 == 0)
            }
            //    local_4 = 0xffffffff;
            bTList_destructor(&local_14);



        }

    }*/


    //nothing left
    namespace feCarsShopTroles {

        //auto FUN_addCarToRegion_00518220 = (void(__fastcall*)(DWORD this_, DWORD unaff, DWORD, FECarRecord*)) 0x00518220;
        //auto FUN_00507030 = (bool(__fastcall*)(DWORD * this_)) 0x00507030;
        //auto SavegameCar_update_00507610 = (void(__fastcall*)(FECarRecord * this_)) 0x00507610;
        //auto FUN_vltnode_clearFromMem_00462550 = (void(__fastcall*)(DWORD * this_)) 0x00462550; //Attrib::Instance::destructor

        /*
        FECarRecord* cachedTrollerCarRecord = new FECarRecord();
        DWORD lastIndex;

        DWORD FECarLotStateManager;

        FECarRecord* __fastcall MakeCarRecord(DWORD PlayerCarDB, DWORD, DWORD Handle) {
            if (FECarLotStateManager == NULL) return cachedTrollerCarRecord;
            if (lastIndex != *(int*)(FECarLotStateManager + 0xc4)) {
                lastIndex = *(int*)(FECarLotStateManager + 0xc4);
                printf("troles\n");

                DALVehicleCommands::CarInfo* carInfo = (DALVehicleCommands::CarInfo*)(**(int**)(FECarLotStateManager + 0x98) + *(int*)(FECarLotStateManager + 0xc4) * 0x28);

                Attrib::Gen::pvehicle pvehicle;
                Attrib::Gen::pvehicle::constructor(&pvehicle, 0, carInfo->carLogoHashpvehicleKey, 0);

                printf("pvehicle %s cost %d\n", pvehicle.mLayoutPtr->CollectionName, carInfo->carCost);

                //cachedTrollerCarRecord = new FECarRecord();
                cachedTrollerCarRecord->handle = carInfo->handle; //-1
                cachedTrollerCarRecord->frontend = pvehicle.mLayoutPtr->frontend.mCollectionKey;
                cachedTrollerCarRecord->pvehicle = Attrib::Instance::GetCollectionKey((Attrib::Instance*)&pvehicle);
                cachedTrollerCarRecord->preset = 0;
                cachedTrollerCarRecord->filterBits = carInfo->region | 0x1;
                cachedTrollerCarRecord->customization = -1;
                cachedTrollerCarRecord->careerHandle = -1;
                cachedTrollerCarRecord->isPresetSkin = 0;
                cachedTrollerCarRecord->isPinkSlip = 0;

                Attrib::Instance::destructor((Attrib::Instance*)&pvehicle);
            }

            return cachedTrollerCarRecord;
        }

        __declspec(naked) void MakeCarRecord_esi() {
            __asm {
                mov FECarLotStateManager, esi;
                jmp MakeCarRecord;
            }
        }
        */
    }

    //nothing left
    namespace raceMusic {

        //previous stuff
        /*
        DWORD MusicLogic__Ready_detour_back = 0x004a523d;
        DWORD MusicLogic__RaceMusic_halfway = 0x00468787;
        __declspec(naked) void MusicLogic__Ready_detour() {
            __asm {
                jz cont;
                jmp MusicLogic__Ready_detour_back;

            cont:
                //play music here

                pop esi;
                pop edi;
                push esi;
                mov esi, ecx;
                jmp MusicLogic__RaceMusic_halfway; //the idea is not too bad but it misses initialization ?
                //        ret 0x4;
            }
        }
        */

    }

    namespace unrestrictedCarTextures {

        int numTexturesInLastTPK = 0; //in last loaded tpk
        DWORD texturesInLastTPK[64];
        bool doTexListing = false;

        void __cdecl readTextures(DWORD ecx) {
            //if (!doTexListing) return;

        //    cout << "TPK TexList @" << (DWORD*)ecx << "\n";
            DWORD blockLength = *(DWORD*)(ecx + 4);
            //    cout << "Length " << blockLength << "\n";
            //    cout << "Num textures " << blockLength / 8 << "\n";

            numTexturesInLastTPK = blockLength / 8;
            if (numTexturesInLastTPK > 64) numTexturesInLastTPK = 64;

            int i = 0;
            for (int off = 8; off <= blockLength; off += 8) {
                //        cout << "Texture hash " << (DWORD*)(*(DWORD*)(ecx + off)) << "\n";
                texturesInLastTPK[i] = *(DWORD*)(ecx + off);
                i++;
                if (i > 63) break;
            }
        }

        DWORD TexList_detour_return = 0x0052a5d8;
        __declspec(naked) void TexList_detour() {
            __asm {
                pushad;
                push ecx;
                call readTextures;
                pop ecx;
                popad;
                lea eax, [ecx + 0x8];
                mov dword ptr[edi + 0x74], eax;
                mov eax, dword ptr[ecx + 0x4];
                shr eax, 0x3;
                mov ebx, eax;
                jmp TexList_detour_return;
            }
        }

        auto getXNAME_String = (char* (__cdecl*)(DWORD)) 0x00817940;
        void __cdecl loadCarTexturesReplacement(DWORD storage, DWORD CarRenderClass43c, DWORD) {
            memset((void*)storage, 0, 0x3e4);
            int texCount = 0;

            char* xname = getXNAME_String(*(DWORD*)CarRenderClass43c);
            char cbuf[128];
            sprintf_s(cbuf, "%s_SKIN1", xname);
            *(DWORD*)(storage + 0x330) = getBinHash(cbuf);

            ((DWORD*)storage)[texCount] = getBinHash(cbuf);
            texCount++;

            for (int i = 0; i < numTexturesInLastTPK; i++) {
                ((DWORD*)storage)[texCount] = texturesInLastTPK[i];
                texCount++;
            }




            *(DWORD*)(storage + 0x32c) = texCount * 4;
            *(DWORD*)(storage + 0x328) = texCount;


        }

        void __fastcall enableTexListingDetour(DWORD ecx, DWORD edx, DWORD v1) {
            doTexListing = true;
        }
        void __fastcall disableTexListingDetour(DWORD ecx, DWORD edx, DWORD v1) {
            doTexListing = false;
        }

    }

    //nothing left
    namespace experimentalCrashFixes {


        //implemented but slightly differently
        /*
        auto GetAmountFree_004ac600 = (DWORD(__fastcall*)(DWORD MemoryPool)) 0x004ac600;
        auto EA__Thread__Mutex__Lock_00977480 = (void(__fastcall*)(DWORD, DWORD edx, DWORD)) 0x00977480;
        auto EA__Thread__Mutex__Unlock_00977540 = (void(__fastcall*)(DWORD)) 0x00977540;
        auto FUN_MemoryPoolDebug_004ad7d0 = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, DWORD min, DWORD max)) 0x004ad7d0;
        auto VerifyPoolIntegrity_004ade80 = (void(__fastcall*)(DWORD MemoryPool, DWORD edx, bool)) 0x004ade80;

        auto DAT_00b6d85c = 0x00b6d85c;

        bool crashDebuggingStarted = false;

        DWORD __fastcall GetAmountFree_004ac600_wrapper(DWORD this_) {
            DWORD puVar2;
            DWORD iVar1;
            DWORD ppFVar1;

            EA__Thread__Mutex__Lock_00977480(*(DWORD*)(this_ + 0x78), 0, DAT_00b6d85c);
            puVar2 = *(DWORD*)(this_ + 4);
            iVar1 = 0;
            while ((DWORD*)puVar2 != (DWORD*)(this_ + 4)) {
                ppFVar1 = puVar2 + 8;
                // game crashed here during a race
                // region related to memory

                if (
                    puVar2 == 0 || //puVar2 < 0x400000 || puVar2 > 0x40000000
                    IsBadReadPtr((DWORD*)puVar2, 1)
                    ) {
                    //BAD POINTER
                    if (!crashDebuggingStarted) {
                        crashDebuggingStarted = true;
                        cout << "Bad pointer in memory pool stuff\a\n";

                        //                FUN_MemoryPoolDebug_004ad7d0(this_, 0, 0, 100);
                        VerifyPoolIntegrity_004ade80(this_, 0, true);
                        crashDebuggingStarted = false;
                    }
                    break;
                }

                puVar2 = *(DWORD*)puVar2;
                if (iVar1 < *(DWORD*)ppFVar1) {
                    iVar1 = *(DWORD*)ppFVar1;
                }
            }
            EA__Thread__Mutex__Unlock_00977540(*(DWORD*)(this_ + 0x78));
            return iVar1;
        }
        */

    }




    //UC 1.18
    auto VehicleCustomizations__ReadFrom = (void(__fastcall*)(void* VehicleCustomizations, DWORD edx, void* RideInfo)) 0x00856750;
    auto VehicleCustomizations__ReadFrom_presetride = (void(__fastcall*)(void* VehicleCustomizations, DWORD edx, Attrib::Gen::presetride * presetride)) 0x008552b0;
    auto VehicleCustomizations__constructor = (void(__fastcall*)(void* VehicleCustomizations)) 0x00513350;
    auto VehicleCustomizations__WriteTo = (void(__fastcall*)(void* VehicleCustomizations, DWORD edx, void* RideInfo)) 0x008560d0;
    auto FECarHeaven_GetRideInfo = (void* (__fastcall*)(DWORD FECarHeaven)) 0x005cf280;
    auto FECarHeaven__SetRideInfo = (void(__fastcall*)(DWORD FECarHeaven, DWORD edx, void* RideInfo)) 0x005cf0a0;
    auto GetCarTypeInfoFromHash = (void* (__cdecl*)(DWORD))0x00817940;
    auto RideInfo__Init = (void* (__fastcall*)(void* RideInfo, DWORD edx, int CarType, DWORD CarRenderUsage, DWORD param_3, DWORD param_4, int param_5, int param_6)) 0x00851b60;
    DWORD VinylSystem__VinylManager = 0x01270694;


    void DoReplacePresetRide(char* presetName) {

        void* RideInfo = ucetesting::FECarHeaven_GetRideInfo(*FECarHeaven);

        //started crashing in render thread ???
        unsigned char VehCustBuf[0x9ec];
        unsigned char* VehicleCustomizations = VehCustBuf;

        Attrib::Gen::presetride presetride;
        Attrib::Gen::presetride::constructor(&presetride, 0, getVltHash(presetName), 0);

        if (presetride.mCollection != NULL) {
            //RideInfo__Init = 0x00851b60 ??
            //BIN hash needed
            VehicleCustomizations__constructor(VehicleCustomizations);
            VehicleCustomizations__ReadFrom_presetride(VehicleCustomizations, 0, &presetride);
            //*((int*)(Attrib::Instance::GetAttributePointer(&presetride, 0, getVltHash("CarType"), 0)) + 1) //WRONG
            RideInfo__Init(RideInfo, 0, *(int*)(VehicleCustomizations + 0x9d0), 0, 0, 0, 0, 0);
            VehicleCustomizations__WriteTo(VehicleCustomizations, 0, RideInfo);
            FECarHeaven__SetRideInfo(*FECarHeaven, 0, RideInfo);
        }

        Attrib::Instance::destructor(&presetride);
    }

    void DoExportPresetRide(char* presetName) {

        unsigned char* RideInfo = (unsigned char*)ucetesting::FECarHeaven_GetRideInfo(*FECarHeaven);

        unsigned char VehCustBuf[0x9ec];
        unsigned char* VehicleCustomizations = VehCustBuf;

        VehicleCustomizations__constructor(VehicleCustomizations);
        VehicleCustomizations__ReadFrom(VehicleCustomizations, 0, RideInfo);

        using namespace AttribEditor;
        if (!modscriptInitialized) initializeModscript();

        {
            char CarType[64];
            snprintf(CarType, sizeof(CarType), "%s", GetCarTypeInfoFromHash(*(int*)(VehicleCustomizations + 0x9d0)));
            //snprintf(CarType, sizeof(CarType), "%s", GetCarTypeInfoFromHash(*(int*)(RideInfo + 0x0)));
            for (int i = 0; i < sizeof(CarType); i++) {
                CarType[i] = std::tolower(CarType[i]);
            }

            modscript << "\n# PRESET RIDE EXPORT - " << presetName << " - " << CarType << " \n# PLEASE FILL IN THE TIER !\n\n";
            modscript << "add_node presetride " << presetName << "\n";
            modscript << "update_field presetride " << presetName << " Tier 1\n";

            modscript << "add_field presetride " << presetName << " CarType\n";
            modscript << "update_field presetride " << presetName << " CarType Class pvehicle\n";
            modscript << "update_field presetride " << presetName << " CarType Collection " << CarType << "\n";
            //printf("CarType: 0x%08x\n", getVltHash(CarType));

            char BINPresetName[64];
            snprintf(BINPresetName, sizeof(BINPresetName), "%s", presetName);
            for (int i = 0; i < sizeof(BINPresetName); i++) {
                BINPresetName[i] = std::toupper(BINPresetName[i]);
            }
            modscript << "update_field presetride " << presetName << " PresetName " << BINPresetName << "\n";
            modscript << "update_field presetride " << presetName << " PresetCar 0x" << (DWORD*)getBinHash(BINPresetName) << "\n";
        }

        //printf("BluePrintType: %d\n", *(int*)(VehicleCustomizations + 0x4a4)); //unused in uc
        //printf("KitType: %d\n", *(int*)(VehicleCustomizations + 0x4a8));
        //printf("KitNum: %d\n", *(int*)(VehicleCustomizations + 0x4ac));
        modscript << "update_field presetride " << presetName << " BluePrint " << *(int*)(VehicleCustomizations + 0x4a4) << "\n";
        modscript << "update_field presetride " << presetName << " KitType " << *(int*)(VehicleCustomizations + 0x4a8) << "\n";
        modscript << "update_field presetride " << presetName << " KitNumber " << *(int*)(VehicleCustomizations + 0x4ac) << "\n";
        modscript << "update_field presetride " << presetName << " RideHeight " << *(float*)(VehicleCustomizations + 0x4b0) << "\n";

        //printf("BluePrintType: %d\n", *(int*)(RideInfo + 0x1b14)); //unused in uc
        //printf("KitType: %d\n", *(int*)(RideInfo + 0x1b0c));
        //printf("KitNum: %d\n", *(int*)(RideInfo + 0x1b10));
        //modscript << "update_field presetride " << presetName << " BluePrint " << *(int*)(RideInfo + 0x1b14) << "\n";
        //modscript << "update_field presetride " << presetName << " KitType " << *(int*)(RideInfo + 0x1b0c) << "\n";
        //modscript << "update_field presetride " << presetName << " KitNumber " << *(int*)(RideInfo + 0x1b10) << "\n";
        //modscript << "update_field presetride " << presetName << " RideHeight " << *(float*)(RideInfo + 0xe4c) << "\n";

        modscript << "update_field presetride " << presetName << " Livery -1\n";

        if (*(int*)(VehicleCustomizations + 0x9d4) != 1) {
            modscript << "add_field presetride " << presetName << " VinylFinish\n";
            modscript << "update_field presetride " << presetName << " VinylFinish " << *(int*)(VehicleCustomizations + 0x9d4) << "\n";
        }
        //if (*(int*)(RideInfo + 0x8) != 1) {
        //    modscript << "add_field presetride " << presetName << " VinylFinish\n";
        //    modscript << "update_field presetride " << presetName << " VinylFinish " << *(int*)(RideInfo + 0x8) << "\n";
        //}


        int attribSlotIdx = 0;

        //parts
        {
            VehicleCustomizations::InstalledPart* vehCustParts = (VehicleCustomizations::InstalledPart*)(VehicleCustomizations + 0x34);
            for (int i = 0; i < 115; i++) {
                if (vehCustParts[i].part_key != -1) {
                    attribSlotIdx++;
                }
            }

            if (attribSlotIdx > 0) modscript << "add_field presetride " << presetName << " Parts " << attribSlotIdx << "\n";
            attribSlotIdx = 0;
            for (int i = 0; i < 115; i++) {
                if (vehCustParts[i].part_key != -1) {
                    modscript << "update_field presetride " << presetName << " Parts[" << attribSlotIdx << "] SlotID " << i << "\n";
                    modscript << "update_field presetride " << presetName << " Parts[" << attribSlotIdx << "] PartClassKey car_part\n";
                    modscript << "update_field presetride " << presetName << " Parts[" << attribSlotIdx << "] PartCollectionKey 0x" << (DWORD*)(vehCustParts[i].part_key) << "\n";
                    modscript << "update_field presetride " << presetName << " Parts[" << attribSlotIdx << "] KitNumber " << vehCustParts[i].kit_num << "\n";
                    attribSlotIdx++;
                }
            }
        }


        //paints
        {
            bool* customPaintsValid = (bool*)(VehicleCustomizations + 0x3cc);
            CarPaintRecordPacked* customPaints = (CarPaintRecordPacked*)(VehicleCustomizations + 0x3d8);
            char* partBaseMaterials = (char*)(VehicleCustomizations + 0x498);
            attribSlotIdx = 0;
            for (int i = 0; i < 12; i++) {
                if (customPaintsValid[i]) attribSlotIdx++;
            }

            if (attribSlotIdx > 0) modscript << "add_field presetride " << presetName << " Paints " << attribSlotIdx << "\n";
            attribSlotIdx = 0;
            for (int i = 0; i < 12; i++) {
                if (customPaintsValid[i]) {
                    customPaints[i];
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 1 " << i << "\n"; //SlotID
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 2 car_paint_group\n";
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 3 0x" << (DWORD*)(customPaints[i].group) << "\n";
                    //4 stays zero (pointer)
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 5 0x" << (DWORD*)(customPaints[i].hue) << "\n";
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 6 0x" << *(DWORD**)(((int)&customPaints[i]) + 8) << "\n"; //saturation
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 7 " << (customPaints[i].variance) << "\n";
                    modscript << "update_field presetride " << presetName << " Paints[" << attribSlotIdx << "] 8 " << (int)(partBaseMaterials[i]) << "\n";
                    attribSlotIdx++;
                }
            }
        }


        //removed parts
        {
            int* removedParts = (int*)(VehicleCustomizations + 0x9dc);
            attribSlotIdx = 0;
            for (int i = 0; i < 4; i++) {
                if (removedParts[i] != -1) attribSlotIdx++;
            }

            if (attribSlotIdx > 0) modscript << "add_field presetride " << presetName << " RemovedParts " << attribSlotIdx << "\n";
            attribSlotIdx = 0;
            for (int i = 0; i < 4; i++) {
                if (removedParts[i] != -1) {
                    modscript << "update_field presetride " << presetName << " RemovedParts[" << attribSlotIdx << "] " << removedParts[i] << "\n";
                    attribSlotIdx++;
                }
            }
        }


        //autosculpt
        {
            unsigned char* morphValues = (unsigned char*)(VehicleCustomizations + 0x798);
            attribSlotIdx = 0;
            for (int i = 0; i < 40; i++) {
                //if (28 <= i && i <= 37) continue; //skip windtunnel
                if (morphValues[11 * i + 0] != 0 || morphValues[11 * i + 1] != 0 || morphValues[11 * i + 2] != 0 || morphValues[11 * i + 3] != 0
                    || morphValues[11 * i + 4] != 0 || morphValues[11 * i + 5] != 0 || morphValues[11 * i + 6] != 0 || morphValues[11 * i + 7] != 0
                    || morphValues[11 * i + 8] != 0 || morphValues[11 * i + 9] != 0 || morphValues[11 * i + 10] != 0)
                    attribSlotIdx++;
            }

            if (attribSlotIdx > 0) modscript << "add_field presetride " << presetName << " Autosculpt " << attribSlotIdx << "\n";
            attribSlotIdx = 0;
            for (int i = 0; i < 40; i++) {
                //if (28 <= i && i <= 37) continue; //skip windtunnel
                if (morphValues[11 * i + 0] != 0 || morphValues[11 * i + 1] != 0 || morphValues[11 * i + 2] != 0 || morphValues[11 * i + 3] != 0
                    || morphValues[11 * i + 4] != 0 || morphValues[11 * i + 5] != 0 || morphValues[11 * i + 6] != 0 || morphValues[11 * i + 7] != 0
                    || morphValues[11 * i + 8] != 0 || morphValues[11 * i + 9] != 0 || morphValues[11 * i + 10] != 0) {

                    modscript << "update_field presetride " << presetName << " Autosculpt[" << attribSlotIdx << "] RegionID " << i << "\n";
                    for (int j = 0; j < 11; j++) if (morphValues[11 * i + j] != 0) {
                        modscript << "update_field presetride " << presetName << " Autosculpt[" << attribSlotIdx << "] Zone" << j + 1 << " " << (float)(morphValues[11 * i + j]) / 100.0f << "\n";
                    }
                    attribSlotIdx++;
                }
            }
        }


        //vinyl layers
        {
            int* numVinylLayers = (int*)(VehicleCustomizations + 0x670);
            PackedVinylLayer* vinylLayers = (PackedVinylLayer*)(VehicleCustomizations + 0x4b4);
            UINT* mirrorBits = (UINT*)(VehicleCustomizations + 0x66c);

            attribSlotIdx = 0; //reusing for vinyl mirror bits
            if (*numVinylLayers > 0) modscript << "add_field presetride " << presetName << " VinylLayers " << *numVinylLayers << "\n";

            for (int i = 0; i < *numVinylLayers; i++) {
                //get hash from VinylSystem::VinylManager
                //why did they have to inline the function...
                if (-1 < vinylLayers[i].vinylPart && vinylLayers[i].vinylPart < *(DWORD*)(VinylSystem__VinylManager + 0xc)) {
                    //printf("Vinyl layer at %X\n", &vinylLayers[i]);

                    //key
                    //modscript << "update_field presetride " << presetName << " VinylLayers[" << attribSlotIdx << "] 1 0x" << (DWORD*)((*(DWORD**)(VinylSystem__VinylManager + 8))[vinylLayers[i].vinylPart * 2]) << "\n";

                    //now the rest... gonn pack it then commit war crimes (vlted)
                    DWORD dat[7];
                    int data = (int)dat;

                    //printf("Vinyl part idx %d, hash %08x\n", vinylLayers[i].vinylPart, (*(DWORD**)(VinylSystem__VinylManager + 8))[vinylLayers[i].vinylPart * 2]);
                    *(DWORD*)data = (*(DWORD**)(VinylSystem__VinylManager + 8))[vinylLayers[i].vinylPart * 2];
                    //printf("%s\n", (*mirrorBits & (1 << (attribSlotIdx & 0x1f))) != 0 ? "Is mirrored" : "Not mirrored");
                    *(bool*)(data + 4) = (*mirrorBits & (1 << (attribSlotIdx & 0x1f))) != 0;

                    //printf("tx = %d, ty = %d\n", vinylLayers[i].transform.translationX, vinylLayers[i].transform.translationY);
                    *(INT16*)(data + 6) = vinylLayers[i].transform.translationX;
                    *(INT16*)(data + 8) = vinylLayers[i].transform.translationY;
                    *(INT8*)(data + 10) = vinylLayers[i].transform.rotation;
                    *(INT8*)(data + 11) = vinylLayers[i].transform.scaleX;
                    *(INT8*)(data + 12) = vinylLayers[i].transform.scaleY;
                    *(bool*)(data + 13) = vinylLayers[i].transform.proportionalScale;
                    *(INT8*)(data + 14) = vinylLayers[i].transform.shear;

                    for (int col = 0; col < 4; col++) {
                        *(UINT8*)(data + 16 + 3 * col) = vinylLayers[i].colors[col].swatch;
                        *(UINT8*)(data + 17 + 3 * col) = vinylLayers[i].colors[col].saturation;
                        *(UINT8*)(data + 18 + 3 * col) = vinylLayers[i].colors[col].brightness;
                    }

                    for (int i = 0; i < 7; i++) {
                        modscript << "update_field presetride " << presetName << " VinylLayers[" << attribSlotIdx << "] " << (i + 1) << " 0x" << ((DWORD**)data)[i] << "\n";
                    }

                }

                attribSlotIdx++;
            }
        }


        //ignore decal layers (unsupported by vlted)

        modscript.flush();
    }

    //ok
    namespace imGuiStuff {


        //static bool doDemoWindow = false;
        //static bool doSunAndSkyWindow = false;
        //static bool doAttribEditor = false;

        //old inputs from previous zone_visual_style editor



        char* debugCarName = NULL;
        char* debugPresetName = NULL;

        void ImGuiFrame()
        {
            

            //Autosculpt menu

        //    if (*FEAutosculptScreen != 0) {
        //
        //        auto part = *(DWORD*)(*FEAutosculptScreen + 0xb0);
        //
        //        auto numAutosculptZones = *(DWORD*)(*FEAutosculptScreen + 0xc0);
        //
        //        ImGui::Begin("Autosculpt Menu");
        //        ImGui::Text("%d Autosculpt Zones", numAutosculptZones);
        //        ImGui::Text(getLocalizedString( ((UINT(__fastcall*)(DWORD)) ((*(DWORD**)part)[7])) (part) )); //part name
        //
        //        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        //        if (ImGui::BeginTabBar("AutosculptTabs", tab_bar_flags))
        //        {
        //            for (int tabIdx = 0; tabIdx < *(DWORD*)(*FEAutosculptScreen + 0xbc); tabIdx++) {
        //                if (ImGui::BeginTabItem(*(char**)(*FEAutosculptScreen + 0xd0 + 4*tabIdx)))
        //                {
        //
        //                    *(DWORD*)(*FEAutosculptScreen + 0xb4) = *(DWORD*)(*FEAutosculptScreen + 0xc4 + 4 * tabIdx);
        //
        //                    char buf[64];
        //                    for (int i = 0; i < numAutosculptZones; i++) {
        //                        sprintf_s(buf, "CUST_AUTOSCULPT_ZONE%d", i + 1);
        //                        ImGui::Text(getLocalizedString(getBinHash(buf)));
        //                        ImGui::PushID(i);
        //                        ImGui::SliderFloat("", (float*)(part + ((*(DWORD*)(*FEAutosculptScreen + 0xb4) + 1) * 0xb + i) * 4), 0.0f, 1.0f);
        ////                        ImGui::SliderFloat("", (float*)(part + ((*(DWORD*)(*FEAutosculptScreen + 0xc4 + 4*tabIdx) + 1) * 0xb + i) * 4), 0.0f, 1.0f);
        //                        ImGui::PopID();
        //                    }
        //
        //
        //                    ImGui::EndTabItem();
        //                }
        //
        //
        //            }
        //            ImGui::EndTabBar();
        //        }
        //
        //        //pFVar1 = FEAutosculptScreen_012ab1b4;
        //        //FUN_005ccc10(&DAT_012ac348, 1);
        //        //AutosculptSelectablePart::3_SetZoneValue_005cd290
        //        //(pFVar1->autosculptPart_0xb0, pFVar1->selectedZone_0xb8, (float)autosculptValue,
        //        //    pFVar1->selectedPartOrKit_0xb4);
        //        //FUN_update_car_005d7840(&DAT_012ac348, '\0');
        //        //pFVar2 = get_FECarHeaven_005cc840();
        //        //pFVar2->field_0xac = 1;
        //
        //
        //        ImGui::End();
        //    }

            if (*FECarHeaven != NULL && CURRENT_GAME_VERSION == UC_1_21 && *DebugVehicleSelection != NULL) {

                if (*(DWORD*)0x012ab164 == NULL) {
                    *(DWORD*)0x012ab164 = MemoryAlloc(0x4000, "FEDebugCarTestStateManager", *MemBlock_Cars);
                    memset(*(void**)0x012ab164, 0, 0x4000);

                    auto RideInfo = (void(__fastcall*)(DWORD this_)) 0x00853cd0;
                    auto RideInfo_ConstructorCopy = (void(__fastcall*)(DWORD this_, DWORD, DWORD copyFrom, bool)) 0x008515b0;

                    //RideInfo((*(DWORD*)0x012ab164 + 0x80));
                    //RideInfo((*(DWORD*)0x012ab164 + 0x1d20));

                    RideInfo_ConstructorCopy((*(DWORD*)0x012ab164 + 0x80), 0, (*(DWORD*)(*(DWORD*)FECarHeaven + 0x8) + 0x10), true);
                    RideInfo_ConstructorCopy((*(DWORD*)0x012ab164 + 0x1d20), 0, (*(DWORD*)(*(DWORD*)FECarHeaven + 0x8) + 0x10), true);


                    //            *(DWORD*)(*(DWORD*)0x012ab164 + 0x39d0) = -1;
                    *(DWORD*)(*(DWORD*)0x012ab164 + 0x39d8) = MemoryAlloc(0x30, "FEDebugCarTestStateManager stuff", *MemBlock_Cars); //0x18
                    memset(*(void**)(*(DWORD*)0x012ab164 + 0x39d8), 0, 0x30);




                    DWORD CarsCount = (UINT)(*(UINT*)(*DebugVehicleSelection + 0x20) - *(UINT*)(*(DWORD*)DebugVehicleSelection + 0x1c) >> 2);
                    *(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) = MemoryAlloc(CarsCount, "FEDebugCarTestStateManager cars list", *MemBlock_Cars);
                    *(DWORD*)(*(DWORD*)0x012ab164 + 0x60) = *(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) + CarsCount * 4;

                    for (int i = 0; i < CarsCount; i++) {
                        auto carname = (*(char***)(*DebugVehicleSelection + 0x1c))[i];
                        std::cout << carname << endl;
                        (*(DWORD**)(*(DWORD*)0x012ab164 + 0x5c))[i] = getVltHash(carname);
                    }



                    //DWORD CarsCount = 330;
                    //*(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) = MemoryAlloc(CarsCount, "FEDebugCarTestStateManager cars list", *MemBlock_Cars);
                    //*(DWORD*)(*(DWORD*)0x012ab164 + 0x60) = *(DWORD*)(*(DWORD*)0x012ab164 + 0x5c) + CarsCount * 4 - 4;
                    //
                    //FECarRecord* userProfileCars = (FECarRecord*)(*(DWORD*)0x0129ea64 + 0x88);
                    //
                    //for (int i = 0; i < 330; i++) {
                    //    DWORD carHash;
                    //    if (userProfileCars[i].preset != NULL) carHash = userProfileCars[i].preset;
                    //    else carHash = userProfileCars[i].pvehicle;
                    //
                    //    (*(DWORD**)(*(DWORD*)0x012ab164 + 0x5c))[i] = carHash;
                    //}
                    //seems all right but crasj



                    //(FECarRecord*)(*(DWORD*)0x0129ea64 + 0x88); //userprofile cars, FECarRecord[330]
                }

                ImGui::Begin("FE Debug Car Test");
                if (debugPresetName != NULL) {
                    ImGui::Text("Preset Name:");
                    ImGui::SameLine();
                    ImGui::Text(debugPresetName);
                }
                if (debugCarName != NULL) {
                    ImGui::Text("Car Name:");
                    ImGui::SameLine();
                    ImGui::Text(debugCarName);
                }
                if (0 <= *(DWORD*)(*(DWORD*)0x012ab164 + 0x74) && *(DWORD*)(*(DWORD*)0x012ab164 + 0x74) < 0x73) {
                    ImGui::Text("Part ID:");
                    ImGui::SameLine();
                    ImGui::Text(((char**)0x0126f35c)[3 * *(DWORD*)(*(DWORD*)0x012ab164 + 0x74)]);
                }
                ImGui::InputInt("BodykitType", (int*)(*(DWORD*)0x012ab164 + 0x6c));
                ImGui::InputInt("BodykitNumber", (int*)(*(DWORD*)0x012ab164 + 0x70));
                ImGui::InputInt("CurrentCar", (int*)(*(DWORD*)0x012ab164 + 0x39e4));




                //static bool b = true;
                //if (ImGui::Checkbox("Hijack PhotoMode", &b)) {
                //    *(DWORD*)(0x00cbb290) = -1; //PhotoMode menu, normally 0x52
                //    *(DWORD*)(0x00cbb038) = 0x52; //DebugCarTest, normally 0x26
                //}
                //else {
                //    *(DWORD*)(0x00cbb290) = 0x52; //PhotoMode menu, normally 0x52
                //    *(DWORD*)(0x00cbb038) = 0x26; //DebugCarTest, normally 0x26
                //}
                //if (ImGui::Button("ScrollNextCar")) ((void(__cdecl*)()) 0x0058b4b0)();
                //if (ImGui::Button("ScrollNextCarSlot")) ((void(__cdecl*)()) 0x0057f360)();
                //if (ImGui::Button("ScrollNextPresetRide")) ((void(__cdecl*)()) 0x0059a150)();
                //if (ImGui::Button("ScrollNextBodyKitType")) ((void(__cdecl*)()) 0x00569a10)();
                //if (ImGui::Button("ScrollNextPart")) ((void(__cdecl*)()) 0x005723d0)();
                //if (ImGui::Button("ScrollNextKit")) ((void(__cdecl*)()) 0x005723f0)();
                //if (ImGui::Button("ScrollNextBodyKitNumber")) ((void(__cdecl*)()) 0x00569a50)();
                ImGui::End();
            } //FEDebugCarTest fuckeries
        }






        //for debug car test
        DWORD __fastcall FeScreenManagerGetScreen_spoof(DWORD this_, DWORD edx, DWORD menuID) {
            return 0x012ab1c4;
        }
        void __stdcall SetDebugCarName2(char* p1, char* p2) {
            debugCarName = p1;
            debugPresetName = p2;
        }
        void __stdcall SetDebugCarName1(char* p1) {
            debugCarName = p1;
            debugPresetName = NULL;
        }




    }

    namespace brokenIdleCam {


        int lastMovement = 0;
        int camBackup = -1;

        int idleCamera = 5; //Staging/StagingSplit
        float idleCameraTimeout = 5.0f;

        auto EventSys__Event__Allocate = (DWORD(__cdecl*)(int size, int)) 0x0052c9b0;
        auto EHudFadeOff__constructor = (void(__fastcall*)(DWORD this_, DWORD, DWORD))0x0064daa0;
        auto LocalPlayer__SetHud = (void(__fastcall*)(DWORD this_, DWORD, DWORD)) 0x007896e0;


        auto CDActionDrive__Update_004d7490 = (void(__fastcall*)(DWORD this_, DWORD edx, DWORD time)) 0x004d7490;
        void __fastcall CDActionDrive__Update_wrapper(DWORD this_, DWORD edx, DWORD time) {
            if (*(float*)(*(DWORD*)(this_ + 0x34) + 0x78) < 0.1 &&
                *(float*)(*(DWORD*)(this_ + 0x34) + 0x7c) < 0.1 &&
                *(float*)(*(DWORD*)(this_ + 0x34) + 0x80) < 0.1) {
                //std::cout << "not moving all that much!!! " << (*timer_world - lastMovement) * 0.00025 << endl;

                if (camBackup == -1 && (*timer_world - lastMovement) * 0.00025 > idleCameraTimeout) {
                    //std::cout << "\n\n\nSWITCHING CAM\n\n\n";
                    (*DALOptions)->vftable->GetInt(*DALOptions, 0, DAL_OPTIONS_CAMERA, &camBackup, -1, -1, -1);
                    (*DALOptions)->vftable->SetInt(*DALOptions, 0, DAL_OPTIONS_CAMERA, idleCamera, -1, -1, -1);

                    //auto _pEVar26 = EventSys__Event__Allocate(0xc, 0);
                    //if (_pEVar26 != NULL) {
                    //    EHudFadeOff__constructor(_pEVar26, 0, 1);
                    //}
                    LocalPlayer__SetHud(*(DWORD*)0x012faa50, 0, 0);
                }
            }
            else {
                lastMovement = *timer_world;
                if (camBackup != -1) {
                    (*DALOptions)->vftable->SetInt(*DALOptions, 0, DAL_OPTIONS_CAMERA, camBackup, -1, -1, -1);
                    LocalPlayer__SetHud(*(DWORD*)0x012faa50, 0, 1);
                    camBackup = -1;
                }
            }

            CDActionDrive__Update_004d7490(this_, edx, time);
        }

    }

    //nothing
    namespace misc {
        //DWORD GCareerHub;
        //auto GHub_function = (void* (__fastcall*)(DWORD GHub)) 0x005f25a0; //0x006054e0;
        // 0x005f25a0 = force save

        //DWORD GGameStatus = 0x012f7e34;




        void testFunction() {
            std::cout << "Test function called ! \a\n";
            //GHub_function(GCareerHub);

            //*DebugSwitchCar = 1;
            //forceSpecificCar = (char*)"nis_350_z_05";




            //DWORD PlayerPVehicle = GetPlayerPVehicle();

            //_memccpy(_forceSpecificCar, *(char**)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4), 0, 31);

            //for (int i = 0; i < 32; i++) {
            //    _forceSpecificCar[i] = std::tolower(_forceSpecificCar[i]);
            //}

            //cout << "Player PVehicle at : " << (DWORD*)PlayerPVehicle << "\n";
            //cout << "Class at : " << *(DWORD*)(PlayerPVehicle + 0x1c) << "\n";
            //cout << "Carname at : " << *(DWORD*)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4) << "\n";
            //cout << "Carname : " << *(char**)(*(DWORD*)(PlayerPVehicle + 0x1c) + 0x4) << "\n";
            //cout << "Player car detected : " << _forceSpecificCar << "\n";
        }



        auto DALManager__GetInt = (bool(__fastcall*)(void* _this, DWORD, DWORD DALValue, int* out_ptr, int param_3, int param_4, int param_5)) 0x005031e0;

        void* TheDALManager = (void*)0x00cb7178;

        auto FEPerformanceStateManager__updateRideInfo = (void(__fastcall*)(FEPerformanceStateManager*))0x0053eb30;


        void __cdecl FEPerformanceStateManager__GetRoot(DWORD, bNode* node) {
            //TODO add perf tuning, packages and single parts back like vanilla

            AddFETreeDataNode(CARSLOTID_ENGINE_PACKAGE, "Engine", *node);
            AddFETreeDataNode(CARSLOTID_NITROUS_PACKAGE, "Nitrous Oxide", *node);
            AddFETreeDataNode(CARSLOTID_FORCED_INDUCTION_PACKAGE, "Forced Induction", *node);
            AddFETreeDataNode(CARSLOTID_SUSPENSION_PACKAGE, "Suspension", *node);
            AddFETreeDataNode(CARSLOTID_DRIVETRAIN_PACKAGE, "Drivetrain", *node);
            AddFETreeDataNode(CARSLOTID_BRAKE_PACKAGE, "Brakes (you won't need them, trust me)", *node);
            AddFETreeDataNode(CARSLOTID_TIRE_PACKAGE, "Tires", *node);
        }


        void __fastcall FEPerformanceStateManager__HandleOptionSelected(FEPerformanceStateManager* _this, DWORD edx, DWORD treeNodeID, DWORD, DWORD ePadButton) {
            if (*(DWORD*)(*FECarHeaven + 0x20) == 0) return; //FECarHeaven->mCarState

            bNode local_14;
            local_14.next = &local_14;
            local_14.prev = &local_14;

            int cash;
            DALManager__GetInt(TheDALManager, 0, 6007, &cash, -1, -1, -1);

            if (CAR_SLOT_ID::CARSLOTID_FIRST_NONRENDER <= treeNodeID && treeNodeID <= CAR_SLOT_ID::CARSLOTID_LAST_NONRENDER) {
                //new method like in visuals menu

                bTList_destructor(&_this->mPartList.HeadNode);
                FEAftermarketMenu::StandardSelectablePart__GetPartsList_005d1b00(CARSLOTID_DRIVETRAIN_PACKAGE, (DWORD)&_this->mPartList, 0, 0);
                _this->mCurrentUpgrade = FEPerformanceStateManager::UPGRADE_DRIVETRAIN;
                FEPerformanceStateManager__updateRideInfo(_this);

                bNode* curPart = _this->mPartList.HeadNode.next;
                while (curPart != &_this->mPartList.HeadNode) {
                    ISelectablePart* part = (ISelectablePart*)((DWORD)curPart - 4);
                    AddFETreeDataNode(part->__vt->GetSlotId(part), getLocalizedString(part->__vt->GetHALIDHash(part)), local_14);
                    curPart = curPart->next;
                }

                //FETreeData::GetChildren_00583090(treeNodeID, &local_14.HeadNode);
                FE__AptMenuTree__AddLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &local_14, 0);

                bTList_destructor(&local_14);
                return;
            }






        }


        void __fastcall FEPerformanceStateManager__HandleOptionHighlighted(FEPerformanceStateManager* _this, DWORD edx, DWORD treeNodeID, DWORD, DWORD ePadButton) {

        }

    }









    static UINT APT_AQUARIUS_MEDIUM;
    static UINT APT_AQUARIUS_MEDIUM_brk;
    static UINT APT_AQUARIUS_MEDIUM48;
    static UINT APT_AQUARIUS_EXTRABOLD;
    static UINT HELV_NEUE_STD_CN;
    static UINT HELV_NEUE_STD_CN_BLD;
    static UINT APT_AQUARIUS_EXTRABOLD_NUM;

    static FE::PAINTSLOTINFO allSlots[] = {
        {CARSLOTID_BODY, INVALID_PAINT_SLOT, "~CUST_PAINT_ALL_REGIONS"},
        {CARSLOTID_BODY, CUSTOM_PAINT_SLOT_BODY, "~CUST_SLOT_BODY"},
        {CARSLOTID_HOOD, CUSTOM_PAINT_SLOT_HOOD, "~CUST_SLOT_HOOD"},
        {CARSLOTID_BUMPER_FRONT, CUSTOM_PAINT_SLOT_BUMPER_FRONT, "~CUST_REGION_FRONT_BUMPER"},
        {CARSLOTID_WHEEL_FRONT, CUSTOM_PAINT_SLOT_RIMS_FRONT, "~CUST_SLOT_FRONT_WHEELS"},
        {CARSLOTID_WHEEL_REAR, CUSTOM_PAINT_SLOT_RIMS_REAR, "~CUST_SLOT_REAR_WHEELS"},
        {CARSLOTID_BUMPER_REAR, CUSTOM_PAINT_SLOT_BUMPER_REAR, "~CUST_REGION_REAR_BUMPER"},
        {CARSLOTID_SPOILER, CUSTOM_PAINT_SLOT_SPOILER, "~CUST_SLOT_SPOILER"},
        {CARSLOTID_ROLL_CAGE, CUSTOM_PAINT_SLOT_ROLLCAGE, "^Roll Cage"},
        {CARSLOTID_SKIRT_LEFT, CUSTOM_PAINT_SLOT_SIDESKIRT, "^Side Skirts" },
        {CARSLOTID_SIDEMIRROR_LEFT, CUSTOM_PAINT_SLOT_MIRROR, "^Mirrors"},
        {CARSLOTID_WINDOW_FRONT, CUSTOM_PAINT_SLOT_WINDOW_TINT, "^Windows"}
    };

    void(__cdecl* FUN_009ef390)(DWORD*, DWORD*);
    void __cdecl FUN_009ef390_wrapper(DWORD* p1, DWORD* p2) {
        if (p1 == NULL || p2 == NULL) {
            printf("UCE: Preventing a crash in the vinyl rasterizer\a\n");
            return;
        }
        FUN_009ef390(p1, p2);
    }

    void* __cdecl CopyTexture_memcpy_wrapperCPP(DWORD* stack, DWORD* _Dst, DWORD* _Src, size_t Size) {
        //DWORD local[1];
        //printf("CopyTexture local at %08X\n", &local);
        //auto stack = &(_Dst);
        //printf("CopyTexture param addresses on stack %08X %08X %08X\n", &_Dst, &_Src, &Size);
        //printf("CopyTexture stack on function entry : return %08X, _Dst = %08X, _Src = %08X, Size = %08X\n",(&local + 1), (&local + 2), * (&local + 3), * (&local + 4));
        //printf("CopyTexture stack on function entry : return %08X, _Dst = %08X, _Src = %08X, Size = %08X\n", *(DWORD*)(((DWORD)stack) + 4), *(stack), *(DWORD*)(((DWORD)stack) - 4), *(DWORD*)(((DWORD)stack) - 8));
        printf("CopyTexture stack on function entry : return %08X, _Dst = %08X, _Src = %08X, Size = %08X\n", *(stack - 2), *(stack), *(stack + 1), *(stack + 2));
        printf("CopyTexture : memcpy(%08X, %08X, %d)\n", _Dst, _Src, Size);
        if (_Dst == NULL || _Src == NULL) DebugBreak();
        auto ret = memcpy(_Dst, _Src, Size);

        //printf("CopyTexture stack on function return : return %08X, _Dst = %08X, _Src = %08X, Size = %08X\n", *(DWORD*)(((DWORD)stack) + 4), *(stack), *(DWORD*)(((DWORD)stack) - 4), *(DWORD*)(((DWORD)stack) - 8));
        //printf("CopyTexture stack on function return : return %08X, _Dst = %08X, _Src = %08X, Size = %08X\n", (&local + 1), (&local + 2), *(&local + 3), *(&local + 4));
        printf("CopyTexture stack on function return : return %08X, _Dst = %08X, _Src = %08X, Size = %08X\n", *(stack - 2), *(stack), *(stack + 1), *(stack + 2));
        return ret;
    }

    __declspec(naked) void CopyTexture_memcpy_wrapper() {
        __asm {
            pop ebp; //old return
            push esp; //stack pointer
            call CopyTexture_memcpy_wrapperCPP;
            pop edx;
            push ebp;
            ret;
        }
    }

    void __cdecl printDxtOob() {
        printf("Array out of bounds in EA::Jobs::Dxt::EncoderJob, preventing crash?\a\n");
    }

    __declspec(naked) void DxtEncoderJobArrayCheck() {
        __asm {
            mov eax, dword ptr[ESP + 0x68];
            cmp eax, 100;
            jae skip; //jae is signed jbe

            mov edx, 0x00aec740;
            call edx;

            push 0x009eecc4;
            ret;

        skip:
            //            mov al, 0x1;
            //            mov edx, 0x009eebf0;
            //            jmp edx;
            call printDxtOob;
            push 0x009eebf0;
            ret;
        }
    }









    static bool initialized = false;

    void doTests() {

        using namespace prints;
        //using namespace quotes;
        using namespace regionCops;
        //using namespace VinylsTransformMenu;
        using namespace feCarsShopTroles;
        using namespace raceMusic;
        using namespace unrestrictedCarTextures;
        using namespace experimentalCrashFixes;
        using namespace imGuiStuff;
        using namespace brokenIdleCam;
        using namespace misc;

        AttribEditor::InitHashMaps();

        //loading screen quotes randomizer (BROKEN)
        switch (CURRENT_GAME_VERSION) {
        case UC_1_0: //UC 1.0
            //utils::jump(0x0058d32d, QuotesDetour);
            //quotesDetourReturn = (void*)0x0058d35d;
            break;
        case UC_1_21: //UC 1.0.1.18
            //utils::jump(0x0057C00d, QuotesDetour);
            //quotesDetourReturn = (void*)0x0057c03d;
            break;
        default:
            break;
        }

        //other stuff
        switch (CURRENT_GAME_VERSION) {
        case UC_1_0: //UC 1.0
        {

            //fuck this game version it fucking sucks

            //utils::jump(0x005b5192, FECarsShopMenuDetour);
            //FECarsShopMenuDetourReturn = 0x005b52de;

            //FUN_addCarToRegion_00518220 = (void(__fastcall*)(DWORD this_, DWORD unaff, DWORD, SavegameCar*)) 0x0052e050;
            //FUN_00507030 = (bool(__fastcall*)(DWORD * this_)) 0x0051d340;
            //SavegameCar_update_00507610 = (void(__fastcall*)(SavegameCar * this_)) 0x0051d7c0;
        }
        break;

        case UC_1_21: //UC 1.0.1.18

        {
            //FUN_addCarToRegion_00518220 = (void(__fastcall*)(DWORD this_, DWORD unaff, DWORD, FECarRecord*)) 0x00518220;
            //FUN_00507030 = (bool(__fastcall*)(DWORD * this_)) 0x00507030;
            //SavegameCar_update_00507610 = (void(__fastcall*)(FECarRecord * this_)) 0x00507610;




            //utils::jump(0x0040ed78, copPresetTest);
            //copPresetTestReturn = (void*)0x0040ed7f;

            //utils::jump(0x006b7aaa, copPresetTest2); //crasj
            //copPresetTest2Return = (void*)0x006b7ab7;

            //utils::jump(0x0040ee10, copPresetTest3);
            //copPresetTest3Return = (void*)0x0040ee17;

            //utils::jump(0x0040e790, interceptPresetrideSpawn);
            //interceptPresetrideSpawnReturn = (void*)0x0040e797;

            // skips pre-loading presetrides
            //utils::nop(0x00513ab1, 5); //DO.NOT.EVER.FUCKING.UNCOMMENT.THIS
            //utils::nop(0x0054193d, 5); //NECESSARY FOR CREATING A NEW SAVE (albeit maybe unoptimized because it loads every single fucking preset not just the relevant ones)





            //breaks things further instead of fixing them
            //APT_AQUARIUS_MEDIUM = 0x00bbac38;
            //APT_AQUARIUS_MEDIUM_brk = 0x00bbac20;
            //APT_AQUARIUS_MEDIUM48 = 0x00bbdc48;
            //APT_AQUARIUS_EXTRABOLD = 0x00bbac04;
            //HELV_NEUE_STD_CN = 0x00bbabe8;
            //HELV_NEUE_STD_CN_BLD = 0x00bbabc8;
            //APT_AQUARIUS_EXTRABOLD_NUM = 0x00bbaba8;

            //vanilla
            //utils::set<UINT>(0x01269fc8, APT_AQUARIUS_MEDIUM);
            //utils::set<UINT>(0x01269fcc, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fd0, APT_AQUARIUS_EXTRABOLD);
            //utils::set<UINT>(0x01269fd4, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fd8, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fdc, APT_AQUARIUS_MEDIUM_brk);
            //utils::set<UINT>(0x01269fe0, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe4, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe8, HELV_NEUE_STD_CN_BLD);
            //utils::set<UINT>(0x01269fec, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269ff0, APT_AQUARIUS_EXTRABOLD_NUM);
            //utils::set<UINT>(0x01269ff4, APT_AQUARIUS_MEDIUM_brk);


            //utils::set<UINT>(0x01269fc8, APT_AQUARIUS_MEDIUM);
            //utils::set<UINT>(0x01269fcc, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fd0, APT_AQUARIUS_EXTRABOLD);
            //utils::set<UINT>(0x01269fd4, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fd8, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fdc, APT_AQUARIUS_MEDIUM48);
            //utils::set<UINT>(0x01269fe0, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe4, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269fe8, HELV_NEUE_STD_CN_BLD);
            //utils::set<UINT>(0x01269fec, HELV_NEUE_STD_CN);
            //utils::set<UINT>(0x01269ff0, APT_AQUARIUS_EXTRABOLD_NUM);
            //utils::set<UINT>(0x01269ff4, APT_AQUARIUS_MEDIUM48);



            //utils::set<int>(0x01305338, 1); // enable -safemode



                //if (utils::get<char>(0x006b6a44) != 0xc7) cout << "ExOpts detected\n";



                //utils::set(0x00826188, 0xFFFFFFFF); //changes rendering settings for carshadow

                //REPLACE UG2 FUNCTION VERSION - ONLY WORKS WITH THE SIGNATURE WITH JUST CARRENDERINFO (! - REVERSE ASSEMBLER BYTES IN 2- AND 4-BYTE UINTS)
                //utils::set<uint8_t>(0x008373b6, 0x57); //push edi
                //utils::call(0x008373b7, ReplaceCarShadow); //call
                //utils::set<uint16_t>(0x008373bc, 0xd8dd); //fstp st(0)
                //utils::set<uint8_t>(0x008373be, 0xE9); //jmp
                //utils::set<uint32_t>(0x008373bf, 0x000000D5); //jmp 00837498



                //utils::jump(0x0060c5ab, harvestGCareerHub);


                //utils::set<UINT>(0x007fed33, 0x19860); //vanilla size for Track Streaming Pool
                //utils::set<UINT>(0x007fed33, 0x100000); 





            //restore prints
            {
                //is it ok or does it need to be disabled again
                utils::jump(0x0090c209, restoreDisabledPrint0); //might also cause random crashes (or not ?)
                restoreDisabledPrintFixupReturn = (void*)0x0090c213;
                utils::call(0x009048cb, restoreDisabledPrintFixup);

                //utils::jump(0x006d9570, restoreDsbPrint_006d9570); //works SOMETIMES but hacky and crashy
                //utils::setFun(0x009e899c, aptDebugPrint); //replaces the apt debug print function

                //utils::set<char>(0x8104d1, 0x24); //apt sets Debug = 1
                //utils::set<char>(0x8104e5, 0x24); //apt sets CafeTrace = 1


                //lua ?
                utils::jump(0x00490591, somethingPrintName);

                utils::set(0x00bc3338, Fesl_Debug_replacement);

                CarSkinManager__NextTask = utils::call(0x008504b9, CarSkinManager__NextTask_wrapper);
            }

            //replace prints
            {
                utils::call(0x0085865a, replacePrint_006d9570); //CarRenderInfo or smth
                utils::call(0x00858693, replacePrint_006d9570);
                utils::call(0x008587c8, replacePrint_006d9570);
                utils::call(0x008587fa, replacePrint_006d9570);

                utils::call(0x0040f790, replacePrint_006d9570); //GPS
                utils::call(0x00429966, replacePrint_006d9570); //SurveyManager
                utils::call(0x00438acc, replacePrint_006d9570); //AICopManager
                utils::call(0x0043cb85, replacePrint_006d9570); //AIThink
                utils::call(0x00441bc3, replacePrint_006d9570);
                utils::call(0x00441f41, replacePrint_006d9570);
                utils::call(0x00445d58, replacePrint_006d9570);
                utils::call(0x00445d66, replacePrint_006d9570);
                utils::call(0x00445d74, replacePrint_006d9570);
                utils::call(0x00449990, replacePrint_006d9570);
                utils::call(0x0044cf30, replacePrint_006d9570);
                utils::call(0x00450982, replacePrint_006d9570);
                utils::call(0x45b5d2, replacePrint_006d9570);
                utils::call(0x00523328, replacePrint_006d9570);//decompress
                utils::call(0x52a4d9, replacePrint_006d9570);
                utils::call(0x52a7f9, replacePrint_006d9570);
                utils::call(0x52c510, replacePrint_006d9570);
                utils::call(0x00540C88, printfSimpleCdecl);//memcard stuff
                utils::call(0x540CBC, printfSimpleCdecl);
                utils::call(0x540CF2, printfSimpleCdecl);
                utils::call(0x541956, printfSimpleCdecl);
                utils::call(0x5519B8, printfSimpleCdecl);
                utils::call(0x551A50, printfSimpleCdecl);
                utils::call(0x551A78, printfSimpleCdecl);
                utils::call(0x551AC7, printfSimpleCdecl);
                utils::call(0x551b2d, printfSimpleCdecl);
                utils::call(0x551BB8, printfSimpleCdecl);
                utils::call(0x551BF8, printfSimpleCdecl);
                utils::call(0x551C35, printfSimpleCdecl);
                utils::call(0x551C45, printfSimpleCdecl);
                utils::call(0x551C65, printfSimpleCdecl);
                utils::call(0x551CA5, printfSimpleCdecl);
                utils::call(0x551CF0, printfSimpleCdecl);
                utils::call(0x551D7A, printfSimpleCdecl);
                utils::call(0x551DCE, printfSimpleCdecl);
                utils::call(0x551E0E, printfSimpleCdecl);
                utils::call(0x551E6E, printfSimpleCdecl);
                utils::call(0x551E9D, printfSimpleCdecl);
                utils::call(0x551ED0, printfSimpleCdecl);
                utils::call(0x551F06, printfSimpleCdecl);
                utils::call(0x551F3F, printfSimpleCdecl);
                utils::call(0x551F70, printfSimpleCdecl);
                utils::call(0x551FA0, printfSimpleCdecl);
                utils::call(0x55F1E7, printfSimpleCdecl);
                utils::call(0x55F25B, printfSimpleCdecl);
                utils::call(0x55F2E8, printfSimpleCdecl);
                utils::call(0x55F6B9, printfSimpleCdecl);
                utils::call(0x55F6E5, printfSimpleCdecl);
                utils::call(0x55F8D4, printfSimpleCdecl);
                utils::call(0x5BA123, replacePrint_006d9570);
                ////maybe something around 5DC968
                utils::call(0x6012B4, replacePrint_006d9570);
                utils::call(0x6021CE, replacePrint_006d9570);
                utils::call(0x60CC0E, replacePrint_006d9570);
                utils::call(0x60CCC0, replacePrint_006d9570);

                utils::call(0x689616, replacePrint_006d9570);
                utils::call(0x689FF5, replacePrint_006d9570);
                utils::call(0x68B8AE, replacePrint_006d9570);
                utils::call(0x68E958, replacePrint_006d9570);
                utils::call(0x68FEE6, replacePrint_006d9570);
                utils::call(0x68FF02, replacePrint_006d9570);
                utils::call(0x691C1D, replacePrint_006d9570);

                utils::call(0x6CB115, replacePrint_006d9570);
                utils::call(0x6cbd39, replacePrint_006d9570);
                utils::call(0x6cbd7c, replacePrint_006d9570);
                utils::call(0x6cbdc0, replacePrint_006d9570);
                utils::call(0x6cc065, replacePrint_006d9570);
                utils::call(0x6cc4cc, replacePrint_006d9570);
                utils::call(0x6cc4f4, replacePrint_006d9570);
                utils::call(0x6cd98a, replacePrint_006d9570);
                utils::call(0x6cd9b1, replacePrint_006d9570);
                utils::call(0x6cdb18, replacePrint_006d9570);
                utils::call(0x6cddcf, replacePrint_006d9570);
                utils::call(0x6d016b, replacePrint_006d9570);
                utils::call(0x6d01ba, replacePrint_006d9570);
                utils::call(0x6d01e7, replacePrint_006d9570);
                utils::call(0x6d03b0, replacePrint_006d9570);
                utils::call(0x6d049b, replacePrint_006d9570);
                utils::call(0x6d04c1, replacePrint_006d9570);
                utils::call(0x6d04f7, replacePrint_006d9570);
                utils::call(0x6d05ba, replacePrint_006d9570);
                utils::call(0x6d0619, replacePrint_006d9570);

                //new
                utils::call(0x6d076e, replacePrint_006d9570);
                //utils::call(0x6d9570, replacePrint_006d9570);
                utils::call(0x6d0aa9, replacePrint_006d9570);
                utils::call(0x6d0ba4, replacePrint_006d9570);
                utils::call(0x6d0cf5, replacePrint_006d9570);
                utils::call(0x6d0e39, replacePrint_006d9570);
                utils::call(0x6d0ec2, replacePrint_006d9570);
                utils::call(0x6d1103, replacePrint_006d9570);
                utils::call(0x6d1161, replacePrint_006d9570);
                utils::call(0x6d118a, replacePrint_006d9570);
                utils::call(0x6d124a, replacePrint_006d9570);
                utils::call(0x6d12a9, replacePrint_006d9570);
                utils::call(0x6d144f, replacePrint_006d9570);
                utils::call(0x6d1551, replacePrint_006d9570);
                utils::call(0x6d166b, replacePrint_006d9570);
                utils::call(0x6d1752, replacePrint_006d9570);
                utils::call(0x6d1881, replacePrint_006d9570);
                utils::call(0x6d3f91, replacePrint_006d9570);
                utils::call(0x6d40b8, replacePrint_006d9570);
                utils::call(0x6d4170, replacePrint_006d9570);
                utils::call(0x6d41ca, replacePrint_006d9570);
                utils::call(0x6d430a, replacePrint_006d9570);
                utils::call(0x6d438a, replacePrint_006d9570);
                utils::call(0x6d43d5, replacePrint_006d9570);
                utils::call(0x6d7216, replacePrint_006d9570);
                ////maybe at 6db126
                //utils::call(0x6dbabd, replacePrint_006d9570);

                utils::call(0x708d42, replacePrint_006d9570);
                utils::call(0x70bedb, replacePrint_006d9570);
                utils::call(0x7141ad, replacePrint_006d9570);
                utils::call(0x84bc2a, replacePrint_006d9570); //CarRenderInfo::CarRenderInfo
                utils::call(0x854724, replacePrint_006d9570); //RideInfo::SetStockParts()
                utils::call(0x869825, replacePrint_006d9570);
                utils::call(0x873bb4, replacePrint_006d9570);
                //utils::setFun(0x01888504, replacePrint_006d9570); //rw::core::filesystem printer
                utils::call(0x981a39, printfSimpleCdecl);//not sure if these are 1st or 2nd print type
                utils::call(0x981ab0, replacePrint_006d9570);
                utils::call(0x981b10, replacePrint_006d9570);
                utils::call(0x981b56, replacePrint_006d9570);
                utils::call(0x981bc5, replacePrint_006d9570);
                utils::call(0x981c2a, replacePrint_006d9570);
                utils::call(0x981c9e, replacePrint_006d9570);
                utils::call(0x981fcc, replacePrint_006d9570);
                utils::call(0x982018, replacePrint_006d9570);
                utils::call(0x9820e6, replacePrint_006d9570);
                utils::call(0x982153, replacePrint_006d9570);
                utils::call(0x982199, replacePrint_006d9570);
                utils::call(0x9821fa, replacePrint_006d9570);
                utils::call(0x982268, replacePrint_006d9570);
                utils::call(0x98232a, replacePrint_006d9570);
                utils::call(0x982361, replacePrint_006d9570);
                utils::call(0x9823cc, replacePrint_006d9570);//end of not sure
                utils::call(0xb1adc9, replacePrint_006d9570);//unwind
                utils::call(0xb1b2a1, replacePrint_006d9570);
                utils::call(0xb1b2b3, replacePrint_006d9570);
                utils::call(0xb1b2c5, replacePrint_006d9570);
                utils::call(0xb1b331, replacePrint_006d9570);
                utils::call(0xb1b343, replacePrint_006d9570);
                utils::call(0xb21a39, replacePrint_006d9570);
                utils::call(0xb21ca9, replacePrint_006d9570);
                utils::call(0xb21f09, replacePrint_006d9570);
                utils::call(0xb21f39, replacePrint_006d9570);
                utils::call(0xb21fb9, replacePrint_006d9570);
                utils::call(0xb22019, replacePrint_006d9570);
                utils::call(0xb2207b, replacePrint_006d9570);
                utils::call(0xb2208d, replacePrint_006d9570);
                utils::call(0xb22339, replacePrint_006d9570);
                utils::call(0xb22369, replacePrint_006d9570);
                utils::call(0xb223f9, replacePrint_006d9570);
                utils::call(0xb24749, replacePrint_006d9570);
                utils::call(0xb2533C, replacePrint_006d9570);
                utils::call(0xB25351, replacePrint_006d9570);
                //a billion more AptMapItem
                utils::call(0xb26e69, replacePrint_006d9570);
                utils::call(0xb27C2A, replacePrint_006d9570);
                utils::call(0xB27C3F, replacePrint_006d9570);
                //...
                utils::call(0xb2fcc1, replacePrint_006d9570);
                //billion more unwinds... 

                utils::call(0x84bc2a, replacePrint_006d9570);
            }



            ////utils::nop(0x0057d0db, 6); //remove vinyl color restriction, useless
            //utils::nop(0x0057cdd7, 6); //remove vinyl fullbody restriction
            //utils::nop(0x0057d06e, 2); //remove vinyl mirror restriction

            //add items to the vinyl edit menu - entire function replacement
            //utils::call(0x00588519, SetupVinylTransformMenu);





            //racer count fuckeries
            //utils::set<char>(0x62AD02, 0x6C); //increase function stack size
            //utils::set<char>(0x62AE1F, 0x6C); //increase function stack size
            //other shit dont work


            /*
                    008547e7 5f              POP        EDI
                    008547e8 67 5e           POP        ESI
                    008547ea 5d              POP        EBP
                    008547eb b8 00 00        MOV        EAX, 0x0
                             00 00
                    008547f0 c2 08 00        RET        0x8
                                         ********** FUN_008547c0 Exit **********


            */


            //utils::set(0x0068fba2, MemPool_World_Size);
            //utils::set(0x005dd0c6, MemPool_World_Size);

            //utils::set(0x007fed33, MemPool_TS_Size);






            //            utils::jump(0x0052a585, TexList_detour);
            ////            cout << "textures list at " << &texturesList << "\n";
            //            utils::call(0x8442B2, loadCarTexturesReplacement);
            //            utils::call(0x858744, loadCarTexturesReplacement);
            //            utils::call(0x84827e, loadCarTexturesReplacement);
            //            utils::call(0x8460bb, loadCarTexturesReplacement);

                        //utils::jump(0x008461f3, disableTexListingDetour);
                        //utils::jump(0x0083d3d9, enableTexListingDetour);


                        //force per-part rendering in game world
            //            utils::nop(0x0084b2b2, 2); //maybe not needed

            //            utils::nop(0x0084bba1, 2); // force compiled rendering in FE
            //            utils::nop(0x0084bbac, 6); // force per part rendering in world (damage needs to be 






            //*(DWORD*)(0x00cbb290) = -1; //PhotoMode menu, normally 0x52
            //*(DWORD*)(0x00cbb038) = 0x52; //DebugCarTest, normally 0x26

            //utils::call(0x00551399, FeScreenManagerGetScreen_spoof);
            //utils::call(0x005513e8, FeScreenManagerGetScreen_spoof);
            //utils::call(0x00551435, FeScreenManagerGetScreen_spoof);
            //utils::call(0x00551500, FeScreenManagerGetScreen_spoof);
            //utils::call(0x0057931b, FeScreenManagerGetScreen_spoof);

            //utils::jump(0x00551390, SetDebugCarName2);
            //utils::jump(0x005513e0, SetDebugCarName1);

            ////*(DWORD*)0x00cbb294 = 0x00b72dd8; //push DebugCarTest string instead of PhotoMode one (PhotoMode is 0x00b72af8)
            ////*(DWORD*)0x00cbb298 = 0x0055ec90; //push DebugCarTest init function instead of PhotoMode one (PhotoMode is 0x0055fe20)

            ////*(DWORD*)0x00cbb324 = 0x00b72dd8; //CodeEntry
            ////*(DWORD*)0x00cbb328 = 0x0055ec90;



            //idle cam
            //CDActionDrive__Update_004d7490 = utils::set(0x00b6f790, CDActionDrive__Update_wrapper);


            //utils::nop(0x00855a3a, 5); //allow to have more than 3 removed parts from a presetride -> NO! likely memory stomp


            //fe position troles, only moves the car, quite silly
            //utils::set(0x005d7365, "PB_Monorail");
            //utils::set(0x005d7389, "PB_Monorail");
            //utils::set(0x005d739d, "PB_Monorail");
            //utils::set(0x005d73a9, "PB_Monorail");



            //troll paint slots
            utils::set(0x005990f2, allSlots);
            utils::set(0x005990fc, IM_COUNTOF(allSlots));

            utils::set(0x005990dc, allSlots);
            utils::set(0x005990e6, IM_COUNTOF(allSlots));

            utils::set(0x0059910e, allSlots);
            utils::set(0x00599118, IM_COUNTOF(allSlots));

            utils::set(0x0059912e, allSlots);
            utils::set(0x00599124, IM_COUNTOF(allSlots));


            //using namespace VinylsSelectMenu;




            // vinyls rasterizer crashes
            FUN_009ef390 = utils::call(0x009f0a44, FUN_009ef390_wrapper); //try to fix the random crash in vinyls rasterizer
            utils::call(0x009f0a9c, FUN_009ef390_wrapper);

            utils::call(0x00818afa, CopyTexture_memcpy_wrapper);
            utils::call(0x00818b34, CopyTexture_memcpy_wrapper);

            utils::jump(0x009eecbf, DxtEncoderJobArrayCheck);








            






            //i cba this is way too hardcoded and annoying
            //utils::set(0x00b7cedc, FEPerformanceStateManager__HandleOptionSelected);
            //utils::call(0x00598a4b, FEPerformanceStateManager__GetRoot);
            //utils::call(0x00598a85, FEPerformanceStateManager__GetRoot);






            utils::nop(0x00a07469, 2); //force allocator auto validate
            utils::nop(0x00a07176, 2);



        }
        break;
        default:
            break;
        }

        initialized = true;
        std::cout << "UCE testing section enabled - built " << __DATE__ << " " << __TIME__ << "\n";
    }

}