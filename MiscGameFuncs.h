#pragma once
#include <sys/types.h>
#include "nfs_structs.h"

enum GAME_VERSION {
    UC_1_0 = 0x008AEC55,
    UC_1_21 = 0x0087ba75
};
DWORD CURRENT_GAME_VERSION;

//should have proper addresses in all versions
//useful shit
auto getVltHash = (uint32_t(__cdecl*)(const char* toHash)) 0x0;
auto getBinHash = (uint32_t(__cdecl*)(const char* toHash)) 0x0;
auto MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x0;
auto getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x0;

DWORD* MemBlock_Global;
DWORD* MemBlock_FE;
DWORD* MemBlock_Cars;

const DWORD zero = 0x00000000;


int* GameFlow;
int* Renderer;
DWORD* FEScreenManager;


DWORD* UserProfile_Player1;
DWORD* FECarHeaven;
//DWORD CurrentFECarPTR_012ac35c = 0x012ac35c;
DWORD* FEOptionsStateManager;
DWORD* FEPerformanceTuning;
DWORD* FEVisualPaint;
DWORD* FEVinylPaintScreen;
DWORD* FEAutosculptScreen;

DWORD* DebugVehicleSelection;

int* timer_world;

SunAndSky* theSunAndSky;

DALInterface** DALOptions;


auto FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
auto FEScreenManager__GetScreen = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
auto FE__AptMenuTree__AddLevel = (void* (__fastcall*)(DWORD this_, DWORD unaff, bNode*, DWORD selectedTreeNodeID)) 0x005563c0;
auto FE__AptMenuTree__ReplaceLevel = (void* (__fastcall*)(DWORD this_, DWORD unaff, bNode* treeNodesList, DWORD levelToReplace, DWORD selectedTreeNodeID)) 0x00556560;
auto bTList_destructor = (void(__fastcall*)(bNode * this_)) 0x00553aa0;
auto bList__traverse__getlength = (DWORD(__fastcall*)(bNode * this_, DWORD unaff, DWORD)) 0x004a76f0;

auto FE__FeChyron__Summon = (void(__cdecl*)(char* text, enum eChyronType IconType, DWORD, bool ShowButtonHint, DWORD)) 0x0055ed40;


auto FE__FEScreenManager__QueueScreenLeave = (void* (__fastcall*)(DWORD this_)) 0x005648a0;

auto CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30 = (int(__fastcall*)(DWORD CarCustMgr, DWORD edx, bool isDecal))0x005cfb30;
auto CarCustomizeManager__GetInstalledVinylName = (DWORD(__fastcall*)(DWORD CarCustMgr, DWORD edx, int index, bool isDecal))0x005d3880;
auto CarCustomizeManager_012ac348 = 0x012ac348;

auto FEStateManager__showMessage = (void(__fastcall*)(FEStateManager * FEStMgr, DWORD edx, DWORD messageID)) 0x00539f00;
auto FEVinylStateManager__ChangeState = (void(__fastcall*)(FEVinylStateManager * FEVSMgr, DWORD edx, DWORD stateID))0x00539680;
auto FEVinylStateManager__7_HandleOptionSelected_005882d0 = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, DWORD, DWORD, DWORD)) 0x005882d0;
auto FEVinylStateManager__UpdateLayers = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, bool, DWORD)) 0;
auto FEVinylStateManager__AddVinyl = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, bool)) 0;

auto FEVinylSelectStateManager__constructor = (FEVinylSelectStateManager * (__fastcall*)(FEVinylSelectStateManager * this_)) 0x00567590;

auto FE__AptMenuTree__AddScreenLevel_00583140 = (DWORD(__fastcall*)(DWORD FEStMgr, DWORD edx, DWORD)) 0x00583140;
auto FECustomizationRecord__DeleteVinylSlot = (void(__fastcall*)(DWORD, DWORD edx, DWORD, bool))  0x0;



auto getLocalizedString = (char* (__cdecl*)(DWORD binHash)) 0x00575d60;

namespace MiscGameFuncs {
	void Register() {
		switch (CURRENT_GAME_VERSION) {
		case UC_1_0:
            getVltHash = reinterpret_cast<decltype(getVltHash)>(0x00463a80);
            getBinHash = reinterpret_cast<decltype(getBinHash)>(0x004b3ed0);
            MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x006a1d30;
            getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x0053cf30;

            FE__FeChyron__Summon = (void(__cdecl*)(char* text, enum eChyronType IconType, DWORD, bool ShowButtonHint, DWORD)) 0x00578e60;

            FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x005612f0;
            FEScreenManager__GetScreen = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x00551dc0;
            FE__AptMenuTree__AddLevel = (void* (__fastcall*)(DWORD, DWORD, bNode*, DWORD)) 0x0056f520;
            FE__AptMenuTree__ReplaceLevel = (void* (__fastcall*)(DWORD this_, DWORD unaff, bNode*, DWORD, DWORD)) 0x0056f6c0;
            bTList_destructor = (void(__fastcall*)(bNode * this_)) 0x00652910;
            bList__traverse__getlength = (DWORD(__fastcall*)(bNode * this_, DWORD unaff, DWORD)) 0x004b0760;

            FE__FEScreenManager__QueueScreenLeave = (void* (__fastcall*)(DWORD this_)) 0x0057e250;

            CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30 = (int(__fastcall*)(DWORD CarCustMgr, DWORD edx, bool isDecal))0x005e9460;
            CarCustomizeManager__GetInstalledVinylName = (DWORD(__fastcall*)(DWORD CarCustMgr, DWORD edx, int index, bool isDecal))0x005eccd0;
            CarCustomizeManager_012ac348 = 0x00d9b600;

            FEStateManager__showMessage = (void(__fastcall*)(FEStateManager * FEStMgr, DWORD edx, DWORD messageID)) 0x00552ac0;
            FEVinylStateManager__ChangeState = (void(__fastcall*)(FEVinylStateManager * FEVSMgr, DWORD edx, DWORD stateID))0x00552240;
            FEVinylStateManager__7_HandleOptionSelected_005882d0 = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, DWORD, DWORD, DWORD)) 0x0059e180;
            FEVinylStateManager__UpdateLayers = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, bool, DWORD)) 0x0058de80;
            FEVinylStateManager__AddVinyl = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, bool)) 0x0059de60;

            FEVinylSelectStateManager__constructor = (FEVinylSelectStateManager * (__fastcall*)(FEVinylSelectStateManager * this_)) 0x00580ea0;

            FE__AptMenuTree__AddScreenLevel_00583140 = (DWORD(__fastcall*)(DWORD FEStMgr, DWORD edx, DWORD)) 0x005973f0;
            FECustomizationRecord__DeleteVinylSlot = (void(__fastcall*)(DWORD, DWORD edx, DWORD, bool))  0x00518950;
            getLocalizedString = (char* (__cdecl*)(DWORD binHash)) 0x00586630;

            MemBlock_Global = (DWORD*)0x00d3bd98;
            MemBlock_FE = (DWORD*)0x00d3bd9c;
            MemBlock_Cars = (DWORD*)0x00d3bdb0;

            GameFlow = (int*)0x00da57b8;
            Renderer = (int*)0x00ea00e4;
            FEScreenManager = (DWORD*)0x00d99354;
            UserProfile_Player1 = (DWORD*)0x00d8e1a4;
            FECarHeaven = (DWORD*)0x00d9b5d0;
            DebugVehicleSelection = (DWORD*)0x01336488;

            FEOptionsStateManager = (DWORD*)0x00d99388;
            FEPerformanceTuning = (DWORD*)0x00d99654;
            FEVisualPaint = (DWORD*)0x00d9a4b0;
            FEVinylPaintScreen = (DWORD*)0x00d9a4c0;
            FEAutosculptScreen = (DWORD*)0x00d9a4b4;

            DALOptions = (DALInterface**)0x00d2a194;

            theSunAndSky = (SunAndSky*)0x00d5ea40;

            timer_world = (int*)0x00da531c;
			break;
		case UC_1_21:
            getVltHash = reinterpret_cast<decltype(getVltHash)>(0x0045cb10);
            getBinHash = reinterpret_cast<decltype(getBinHash)>(0x004aa050);
            MemoryAlloc = (DWORD(__cdecl*)(DWORD size, const char* name, DWORD baseAddress)) 0x00686150;
            getTexture = (char* (__cdecl*)(uint32_t textureHash, int param_2, int param_3)) 0x005246d0;

            FE__FeChyron__Summon = (void(__cdecl*)(char* text, enum eChyronType IconType, DWORD, bool ShowButtonHint, DWORD)) 0x0055ed40;

            FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
            FEScreenManager__GetScreen = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
            FE__AptMenuTree__AddLevel = (void* (__fastcall*)(DWORD this_, DWORD unaff, bNode*, DWORD)) 0x005563c0;
            FE__AptMenuTree__ReplaceLevel = (void* (__fastcall*)(DWORD this_, DWORD unaff, bNode*, DWORD, DWORD)) 0x00556560;
            bTList_destructor = (void(__fastcall*)(bNode * this_)) 0x00553aa0;
            bList__traverse__getlength = (DWORD(__fastcall*)(bNode * this_, DWORD unaff, DWORD)) 0x004a76f0;

            FE__FEScreenManager__QueueScreenLeave = (void* (__fastcall*)(DWORD this_)) 0x005648a0;

            CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30 = (int(__fastcall*)(DWORD CarCustMgr, DWORD edx, bool isDecal))0x005cfb30;
            CarCustomizeManager__GetInstalledVinylName = (DWORD(__fastcall*)(DWORD CarCustMgr, DWORD edx, int index, bool isDecal))0x005d3880;
            CarCustomizeManager_012ac348 = 0x012ac348;

            FEStateManager__showMessage = (void(__fastcall*)(FEStateManager * FEStMgr, DWORD edx, DWORD messageID)) 0x00539f00;
            FEVinylStateManager__ChangeState = (void(__fastcall*)(FEVinylStateManager * FEVSMgr, DWORD edx, DWORD stateID))0x00539680;
            FEVinylStateManager__7_HandleOptionSelected_005882d0 = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, DWORD, DWORD, DWORD)) 0x005882d0;
            FEVinylStateManager__UpdateLayers = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, bool, DWORD)) 0;
            FEVinylStateManager__AddVinyl = (void(__fastcall*)(FEVinylStateManager * FEStMgr, DWORD edx, bool)) 0;

            FEVinylSelectStateManager__constructor = (FEVinylSelectStateManager * (__fastcall*)(FEVinylSelectStateManager * this_)) 0x00567590;

            FE__AptMenuTree__AddScreenLevel_00583140 = (DWORD(__fastcall*)(DWORD FEStMgr, DWORD edx, DWORD)) 0x00583140;
            FECustomizationRecord__DeleteVinylSlot = (void(__fastcall*)(DWORD, DWORD edx, DWORD, bool))  0x0;
            getLocalizedString = (char* (__cdecl*)(DWORD binHash)) 0x00575d60;

            MemBlock_Global = (DWORD*)0x00cc9328;
            MemBlock_FE = (DWORD*)0x00cc932c;
            MemBlock_Cars = (DWORD*)0x00cc9340;

            GameFlow = (int*)0x012b64f8;
            Renderer = (int*)0x013b3164;
            FEScreenManager = (DWORD*)0x012aa044;
            UserProfile_Player1 = (DWORD*)0x0129ea64;
            FECarHeaven = (DWORD*)0x012ac318;
            DebugVehicleSelection = (DWORD*)0x01862c5c;

            FEOptionsStateManager = (DWORD*)0x012aa078;
            FEPerformanceTuning = (DWORD*)0x012aa344;
            FEVisualPaint = (DWORD*)0x012ab1b0;
            FEVinylPaintScreen = (DWORD*)0x012ab1c0;
            FEAutosculptScreen = (DWORD*)0x012ab1b4;

            DALOptions = (DALInterface**)0x00cb7194;

            theSunAndSky = (SunAndSky*)0x0126f100;

            timer_world = (int*)0x012b6450;
			break;
		}


	}
}


void AddFETreeDataNode(int ID, char* label, bNode& node) {
    auto curTreeNode = (FETreeDataNode*)MemoryAlloc(0x2c, "FETreeDataNode", *MemBlock_FE);
    if (curTreeNode != NULL) {
        curTreeNode = FETreeDataNode_constructor(curTreeNode, 0, ID, label, 0xffffffff, 0xffffffff);
        auto current = &curTreeNode->node;
        (node.next)->prev = current;
        current->next = node.next;
        current->prev = &node;
        node.next = current;
    }
}
void AddFETreeDataNode(int ID, const char* label, bNode& node) {
    AddFETreeDataNode(ID, (char*)label, node);
}