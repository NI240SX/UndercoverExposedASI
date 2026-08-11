#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"
#include "Injector.h"

//vinyls/decals
namespace VinylsSelectMenu {

    constexpr auto TreeNode_Decal_Add = 0x101;
    constexpr auto TreeNode_Decal_Edit = 0x102;
    constexpr auto MAX_VINYL_LAYERS = 20;

    bool isDecal = false;

	bool wasInDecalSelect = false;

    void __fastcall FEVinylStateManager__SetupRootMenu(FEVinylStateManager* FEVinylStateManager, DWORD edx, bool doReplaceLevel, DWORD levelToReplace) {

        bNode node;
        node.next = &node;
        node.prev = &node;


        //TreeNode IDs for 1.18
        //0x9c: Vinyls Add Layer
        //0x9d: Vinyls Edit Layers
        //0x9e: Vinyls Finish

        int GVOffset = 0;
        switch (CURRENT_GAME_VERSION) {
        case UC_1_0:
            GVOffset = -1;
            break;
        }

        AddFETreeDataNode(GVOffset + 0x9c, getLocalizedString(getBinHash((char*)"CUST_VINYL_ADDLAYER")), node);
        if (CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30(CarCustomizeManager_012ac348, 0, false) > 0) { //vinyls
            AddFETreeDataNode(GVOffset + 0x9d, getLocalizedString(getBinHash((char*)"CUST_VINYL_EDITLAYERS")), node);
        }

        AddFETreeDataNode(TreeNode_Decal_Add, (char*)"Add Decal", node);
        if (CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30(CarCustomizeManager_012ac348, 0, true) > 0) { //decals
            AddFETreeDataNode(TreeNode_Decal_Edit, (char*)"Edit Decals", node);
        }

        

        AddFETreeDataNode(GVOffset + 0x9e, getLocalizedString(getBinHash((char*)"CUST_VINYL_SETFINISH")), node);

        if (doReplaceLevel) {
            if (wasInDecalSelect) {
				wasInDecalSelect = false;
                FE__AptMenuTree__ReplaceLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &node, levelToReplace, TreeNode_Decal_Edit);
            } else
            FE__AptMenuTree__ReplaceLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &node, levelToReplace, 0);
        }
        else {
            FE__AptMenuTree__AddLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &node, 0);
        }

        bTList_destructor(&node);

    }

    void __fastcall FEVinylStateManager__UpdateDecalLayers(FEVinylStateManager* _this, DWORD edx, bool doReplaceLevel, DWORD levelToReplace) {
        bNode node;
        node.next = &node;
        node.prev = &node;

        auto numDecals = CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30(CarCustomizeManager_012ac348, 0, true);

        for (int i = 0; i < numDecals; i++) {
            DWORD VinylLanguageHash = CarCustomizeManager__GetInstalledVinylName(CarCustomizeManager_012ac348, 0, i, true);
            AddFETreeDataNode(i, getLocalizedString(VinylLanguageHash), node);
        }

        //not same as the code but should do the same ???
        if (doReplaceLevel) FE__AptMenuTree__ReplaceLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &node, levelToReplace, 0);
        else FE__AptMenuTree__AddLevel(FEScreenManager__GetScreen(*FEScreenManager, 0, 0x28), 0, &node, 0);

        bTList_destructor(&node);
    }

    void __fastcall FEVinylStateManager__AddDecal(FEVinylStateManager* _this, DWORD edx, bool param_1) {
		wasInDecalSelect = true;
        if (param_1) {
            auto pFVar3 = (FEVinylSelectStateManager*)MemoryAlloc(0x50, "FEVinylSelectStateManager", *MemBlock_FE);
            if (pFVar3 == NULL) {
                pFVar3 = NULL;
            }
            else {
                pFVar3 = FEVinylSelectStateManager__constructor(pFVar3);
                //printf("Vinyl Select State Manager allocated\n");
                pFVar3->mParentManager = _this;
                _this->mPrevState = _this->mCurState;
                _this->mCurState = FEVinylStateManager::eState::STATE_VINYL_ADD;
                (*(_this->__vt)->HandleStateChange)(_this);
                _this->mNextManager = (FEStateManager*)pFVar3;
                _this->mNumScreensToPop = 1;
                FE__FEScreenManager__QueueScreenLeave(*FEScreenManager);
            }
            return;
        }
        auto pFVar3 = (FEVinylSelectStateManager*)MemoryAlloc(0x50, "FEVinylSelectStateManager", *MemBlock_FE);
        if (pFVar3 == NULL) {
            pFVar3 = NULL;
        }
        else {
            pFVar3 = FEVinylSelectStateManager__constructor(pFVar3);
            //printf("Vinyl Select State Manager allocated\n");
            pFVar3->mParentManager = _this;
            _this->mPrevState = _this->mCurState;

            _this->mCurState = FEVinylStateManager::eState::STATE_VINYL_ADD;
            (*(_this->__vt)->HandleStateChange)(_this);

            _this->mChildManager = (FEStateManager*)pFVar3;
            (*(_this->__vt)->HandleChildFlowStart)(_this);
            //printf("_this->mChildManager @ %08X", _this->mChildManager);
            (*(_this->mChildManager)->__vt->Start)(_this->mChildManager);
        }
    }

    void __fastcall FEVinylStateManager__UpdateLayers_wrapper(FEVinylStateManager* _this, DWORD edx, bool doReplaceLevel, DWORD levelToReplace) {
        if (isDecal) FEVinylStateManager__UpdateDecalLayers(_this, edx, doReplaceLevel, levelToReplace);
        else FEVinylStateManager__UpdateLayers(_this, edx, doReplaceLevel, levelToReplace);
    }

    void __fastcall FEVinylStateManager__AddVinyl_wrapper(FEVinylStateManager* _this, DWORD edx, bool param_1) {
        if (isDecal) FEVinylStateManager__AddDecal(_this, edx, param_1);
        else FEVinylStateManager__AddVinyl(_this, edx, param_1);
    }

    //TODO
    //thumbnails might stay fucked

    void __fastcall FEVinylStateManager__7_HandleOptionSelected_005882d0_wrapper(FEVinylStateManager* _this, DWORD edx, DWORD selectedTreeNode, DWORD param_2, DWORD param_3) {

        //printf("----------------------------------------------------------------------------------------------------\n");
        //printf("FEVinylStateManager__7_HandleOptionSelected_005882d0_wrapper\n");
        //printf("FEVinylStateMgr: vinyl layer %d, isDecal %s, sel group 0x%08x, sel vinyl 0x%08x\n",_this->mCurrentLayer, isDecal ? "true" : "false", _this->mSelectedGroup, _this->mSelectedVinyl);
        //if (_this->mPart != NULL)
        //    printf("PART: vinyl layer %d, decal %s, kitnum %d\n", _this->mPart->mCurrentEditingLayer, _this->mPart->mIsDecal ? "true" : "false", _this->mPart->mCurrentPart.kitNum);
        //printf("----------------------------------------------------------------------------------------------------\n");

        if (_this->mCurState == FEVinylStateManager::eState::STATE_MAIN) {
            if (selectedTreeNode == TreeNode_Decal_Add || selectedTreeNode == TreeNode_Decal_Edit) {
                isDecal = true;
                //set up decal purchase
            }
            else {
                isDecal = false;
                //reset vinyl settings
            }
            switch (CURRENT_GAME_VERSION) {
            case UC_1_21:
                utils::set<bool>(0x005cd900, isDecal); //isDecal for FECustomizationRecord::SetInstalledVinyl in VinylSelectablePart::Install
                utils::set<bool>(0x005cd911, isDecal); //isDecal for FECustomizationRecord::SetVinylTransform in VinylSelectablePart::Install
                utils::set<bool>(0x005cd926, isDecal); //isDecal for FECustomizationRecord::SetVinylMirror in VinylSelectablePart::Install
                utils::set<bool>(0x005cd939, isDecal); //isDecal for FECustomizationRecord::SetVinylSlotColour in VinylSelectablePart::Install

                utils::set<bool>(0x0054e10d, isDecal); //isDecal for CarCustomizeManager::GetInstalledCarVinylPart in FEVinylStateManager::SetCurrentLayer
                utils::set<bool>(0x00588585, isDecal); //mDecal for CarCustomizeManager
                utils::set<bool>(0x00588601, isDecal); //mDecal for CarCustomizeManager

                utils::set<bool>(0x0053dea6, isDecal); //isDecal for CarCustomizeManager::GetInstalledCarVinylPart in FEVinylStateManager::ResetCurrentPart (from HandleChildFlowDone)
                utils::set<bool>(0x0057d1f4, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::MoveVinyl
                utils::set<bool>(0x0057d258, isDecal); //isDecal for FECustomizationRecord::MoveVinylLayer in FEVinylStateManager::MoveVinyl

                utils::set<bool>(0x0053dee6, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::HandlePadButton4

                utils::set<bool>(0x00594a64, isDecal); //isDecal for CarCustomizeManager::GetInstalledVinylName in FEVinylStateManager::HandleShowDialog (DIALOG_CONFIRM_DELETE)

                utils::set<bool>(0x0054e136, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::UpdateVinylThumbnail (ShowVinylControls - Vinyls up/down)

                utils::set<bool>(0x005948c6, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::LoadThumbnails
                utils::set<bool>(0x005948f1, isDecal); //isDecal for CarCustomizeManager::GetInstalledCarVinylPart in FEVinylStateManager::LoadThumbnails
                break;
            case UC_1_0:
                utils::set<bool>(0x005e7140, isDecal); //isDecal for FECustomizationRecord::SetInstalledVinyl in VinylSelectablePart::Install
                utils::set<bool>(0x005e7151, isDecal); //isDecal for FECustomizationRecord::SetVinylTransform in VinylSelectablePart::Install
                utils::set<bool>(0x005e7166, isDecal); //isDecal for FECustomizationRecord::SetVinylMirror in VinylSelectablePart::Install
                utils::set<bool>(0x005e7179, isDecal); //isDecal for FECustomizationRecord::SetVinylSlotColour in VinylSelectablePart::Install

                utils::set<bool>(0x0056730d, isDecal); //isDecal for CarCustomizeManager::GetInstalledCarVinylPart in FEVinylStateManager::SetCurrentLayer
                utils::set<bool>(0x0059e435, isDecal); //mDecal for CarCustomizeManager
                utils::set<bool>(0x0059e4b1, isDecal); //mDecal for CarCustomizeManager

                utils::set<bool>(0x005568a6, isDecal); //isDecal for CarCustomizeManager::GetInstalledCarVinylPart in FEVinylStateManager::ResetCurrentPart (from HandleChildFlowDone)
                utils::set<bool>(0x0058e544, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::MoveVinyl
                utils::set<bool>(0x0058e5a8, isDecal); //isDecal for FECustomizationRecord::MoveVinylLayer in FEVinylStateManager::MoveVinyl

                utils::set<bool>(0x005568f6, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::HandlePadButton4

                utils::set<bool>(0x005b42b4, isDecal); //isDecal for CarCustomizeManager::GetInstalledVinylName in FEVinylStateManager::HandleShowDialog (DIALOG_CONFIRM_DELETE)

                utils::set<bool>(0x00567336, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::UpdateVinylThumbnail (ShowVinylControls - Vinyls up/down)

                utils::set<bool>(0x005b4116, isDecal); //isDecal for CarCustomizeManager::GetNumVinylInstalledLayers in FEVinylStateManager::LoadThumbnails
                utils::set<bool>(0x005b4141, isDecal); //isDecal for CarCustomizeManager::GetInstalledCarVinylPart in FEVinylStateManager::LoadThumbnails
                break;
            }
        }

        FEVinylStateManager__7_HandleOptionSelected_005882d0(_this, edx, selectedTreeNode, param_2, param_3);

        //printf("----------------------------------------------------------------------------------------------------\n");
        //printf("FEVinylStateManager__7_HandleOptionSelected_005882d0_wrapper END\n");
        //printf("FEVinylStateMgr: vinyl layer %d, isDecal %s, sel group 0x%08x, sel vinyl 0x%08x\n", _this->mCurrentLayer, isDecal ? "true" : "false", _this->mSelectedGroup, _this->mSelectedVinyl);
        //if (_this->mPart != NULL)
        //    printf("PART: vinyl layer %d, decal %s, kitnum %d\n", _this->mPart->mCurrentEditingLayer, _this->mPart->mIsDecal ? "true" : "false", _this->mPart->mCurrentPart.kitNum);
        //printf("----------------------------------------------------------------------------------------------------\n");

    }

    void __fastcall FECustomizationRecord__DeleteVinylSlot_wrapper(DWORD FECr, DWORD edx, DWORD layer, bool decal) {
        FECustomizationRecord__DeleteVinylSlot(FECr, edx, layer, isDecal);
    }

    void __fastcall CarCustomizeManager__GetNumVinylInstalledLayers_wrapper(DWORD CarCustMgr, DWORD edx, bool decal) {
        CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30(CarCustMgr, edx, isDecal);
    }



    void __stdcall HandleOptSel_STATE_MAIN_DetourCPP(FEVinylStateManager* _this, DWORD SelectedTreeNode) {
        switch (CURRENT_GAME_VERSION) {
        case UC_1_21:
            SelectedTreeNode += 0x9e;
            break;
        case UC_1_0:
            SelectedTreeNode += 0x9d;
            break;
        }
        if (SelectedTreeNode == TreeNode_Decal_Add) {
            if (CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30(CarCustomizeManager_012ac348, 0, true) >= MAX_VINYL_LAYERS) {
                //TODO change message to decals
                FEStateManager__showMessage(_this, 0, FEVinylStateManager::eState::STATE_DIALOG_TOO_MANY_LAYERS);
                return;
            }

            //TODO fix stuff
            auto pAVar2 = FEScreenManager__GetScreen(*FEScreenManager, 0, TreeMenu_Customize);
            FE__AptMenuTree__AddScreenLevel_00583140(pAVar2, 0, getBinHash((char*)"COMMON_SELECT"));

            FEVinylStateManager__AddDecal(_this, 0, false);

        }
        else if (SelectedTreeNode == TreeNode_Decal_Edit) {
            if (CarCustomizeManager__GetNumVinylInstalledLayers_005cfb30(CarCustomizeManager_012ac348, 0, true) > 0) {
                FEVinylStateManager__ChangeState(_this, 0, FEVinylStateManager::eState::STATE_VINYL_LIST); //TODO make new state for decals ?
                //update layers -> need to make a new function
                FEVinylStateManager__UpdateDecalLayers(_this, 0, false, 0);
            }
        }


    }

    //TODO troll other states
    DWORD HandleOptSel_STATE_MAIN_Detour_return;
    __declspec (naked) void HandleOptSel_STATE_MAIN_Detour() {
        __asm {
            push eax;
            push esi;
            call HandleOptSel_STATE_MAIN_DetourCPP;
            jmp HandleOptSel_STATE_MAIN_Detour_return;
        }
    }










    void Inject() {
        switch (CURRENT_GAME_VERSION) {
        case UC_1_21:
            utils::jump(0x00587fc0, FEVinylStateManager__SetupRootMenu);
            FEVinylStateManager__7_HandleOptionSelected_005882d0 = utils::set(0x00b7bfd4, FEVinylStateManager__7_HandleOptionSelected_005882d0_wrapper);
            //TODO troll other states -> not necessary?
            HandleOptSel_STATE_MAIN_Detour_return = utils::set<std::uint32_t>(0x00588325 + 2, std::uint32_t(HandleOptSel_STATE_MAIN_Detour) - 0x00588325 - 6) + 0x00588325 + 6;
            utils::nop(0x005887b9, 6); //remove setting CarCustomizeManager.mDecal to false in FEVinylStateManager::HandleChildFlowDone
            utils::nop(0x00588751, 6); //remove setting CarCustomizeManager.mDecal to false in FEVinylStateManager::HandleChildFlowDone
            utils::call(0x0058808d, CarCustomizeManager__GetNumVinylInstalledLayers_wrapper); //couldn't force the bool directly in FEVinylStateManager::DeleteVinyl
            FECustomizationRecord__DeleteVinylSlot = utils::call(0x005880b9, FECustomizationRecord__DeleteVinylSlot_wrapper); //couldn't force the bool directly in FEVinylStateManager::DeleteVinyl
            FEVinylStateManager__UpdateLayers = utils::call(0x00588128, FEVinylStateManager__UpdateLayers_wrapper); //go to either decal or vinyl layers in FEVinylStateManager::DeleteVinyl
            utils::call(0x0057d293, FEVinylStateManager__UpdateLayers_wrapper); //in FEVinylStateManager::MoveVinyl
            utils::call(0x0055e5e0, CarCustomizeManager__GetNumVinylInstalledLayers_wrapper); //in FE::FEVinylSelectScreen::FEVinylSelectScreen
            FEVinylStateManager__AddVinyl = utils::jump(0x005886f3, FEVinylStateManager__AddVinyl_wrapper);
            break;
        case UC_1_0:
            utils::jump(0x0059dda0, FEVinylStateManager__SetupRootMenu);
            FEVinylStateManager__7_HandleOptionSelected_005882d0 = utils::set(0x00bf133c, FEVinylStateManager__7_HandleOptionSelected_005882d0_wrapper);
            HandleOptSel_STATE_MAIN_Detour_return = utils::set<std::uint32_t>(0x0059e1d5 + 2, std::uint32_t(HandleOptSel_STATE_MAIN_Detour) - 0x0059e1d5 - 6) + 0x0059e1d5 + 6;
            utils::nop(0x0059e669, 6); //remove setting CarCustomizeManager.mDecal to false in FEVinylStateManager::HandleChildFlowDone
            utils::nop(0x0059e601, 6); //remove setting CarCustomizeManager.mDecal to false in FEVinylStateManager::HandleChildFlowDone
            utils::call(0x0059df3d, CarCustomizeManager__GetNumVinylInstalledLayers_wrapper); //couldn't force the bool directly in FEVinylStateManager::DeleteVinyl
            FECustomizationRecord__DeleteVinylSlot = utils::call(0x0059df69, FECustomizationRecord__DeleteVinylSlot_wrapper); //couldn't force the bool directly in FEVinylStateManager::DeleteVinyl
            FEVinylStateManager__UpdateLayers = utils::call(0x0059dfd8, FEVinylStateManager__UpdateLayers_wrapper); //go to either decal or vinyl layers in FEVinylStateManager::DeleteVinyl
            utils::call(0x0058e5e3, FEVinylStateManager__UpdateLayers_wrapper); //in FEVinylStateManager::MoveVinyl
            utils::call(0x00578700, CarCustomizeManager__GetNumVinylInstalledLayers_wrapper); //in FE::FEVinylSelectScreen::FEVinylSelectScreen
            FEVinylStateManager__AddVinyl = utils::jump(0x0059e5a3, FEVinylStateManager__AddVinyl_wrapper);
            break;
        }
    }
}