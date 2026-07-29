#pragma once
#include <sys/types.h>
#include "nfs_structs.h"

//should have proper addresses in all versions
//useful shit
auto getVltHash = (uint32_t(__cdecl*)(char* toHash)) 0x0;
auto getBinHash = (uint32_t(__cdecl*)(char* toHash)) 0x0;
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

DWORD CURRENT_GAME_VERSION;

auto FETreeDataNode_constructor = (FETreeDataNode * (__fastcall*)(FETreeDataNode * this_, DWORD unaff, DWORD, char*, DWORD, DWORD)) 0x00548210;
auto FE_getFeElement = (DWORD(__fastcall*)(DWORD this_, DWORD unaff, DWORD)) 0x005391d0;
auto AddToTree = (void* (__fastcall*)(DWORD this_, DWORD unaff, LinkedNode*, DWORD)) 0x005563c0;
auto FUN_00553aa0 = (void(__fastcall*)(LinkedNode * this_)) 0x00553aa0;
auto FUN_getLinkedListLength_004a76f0 = (DWORD(__fastcall*)(LinkedNode * this_, DWORD unaff, DWORD)) 0x004a76f0;




//TODO
auto getLocalizedString = (char* (__cdecl*)(DWORD binHash)) 0x00575d60;

