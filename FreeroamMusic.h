#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"


namespace FreeroamMusic {
    auto CSTATEMGR_Base__CreateSFXCTL = (DWORD(__fastcall*)(DWORD _this, DWORD, DWORD p1, DWORD p2))0x0;
    DWORD SFXCTL_MusicPlayback_vftable = 0x00b6d458;
    DWORD SFXCTL_MusicPlayback_addr;

    DWORD* Music_PathfinderControl = (DWORD*)0x012946a8;
    Music::MusicProject* MusicProject = (Music::MusicProject*)0x00cb2da0;
    DWORD* EAXSound = (DWORD*)0x01294590;

    void* Pause = (void*)0x004a4be0;
    void* MapScreen = (void*)0x004a3300;
    void* CarSelect = (void*)0x004a3460;
    void* Credits = (void*)0x004a35c0;
    void* Start = (void*)0x00494930;
    void* StartPursuit = (void*)0x0048ece0;
    void* StartChase = (void*)0x0048b4a0;
    void* RaceInit = (void*)0x0046f300;
    void* Custom = (void*)0x004a52d0;
    void* StartBattle = (void*)0x0048cc90;

    //auto SFXCTL_MusicPlayback__QueueProject_00494650 = (void(__fastcall*)(SFXCTL_MusicPlayback*, DWORD edx, DWORD)) 0x00494650;
    //auto SFXCTL_MusicPlayback__QueueEvent_00494610 = (void(__fastcall*)(SFXCTL_MusicPlayback*, DWORD edx, DWORD)) 0x00494610;
    auto SFXCTL_MusicPlayback__GenRandomIndexFromBitfield = (DWORD(__fastcall*)(SFXCTL_MusicPlayback*, DWORD edx, DWORD)) 0x00494740;
    auto SFXCTL_MusicPlayback__QueueValue = (DWORD(__fastcall*)(SFXCTL_MusicPlayback*, DWORD edx, const char*, DWORD)) 0x0;
    //auto Attrib::Instance::GetRaceTracks = (DWORD * (__fastcall*)(Attrib::InstanceBase*, DWORD edx, DWORD)) 0x00463b70;
    //auto Sound__MusicLogic__CheckInteractiveToFEStateTrans_004a3750 = (bool(__fastcall*)(Sound::MusicLogic*, DWORD edx, DWORD)) 0x004a3750;

    static bool NeedsInit = true;
    static int forceNextSongIndex = -1;
    //static int forceNextSongID = -1;

    bool* SkipChyron;

    DWORD __fastcall CSTATEMGR_Base__CreateSFXCTL_wrapper(DWORD _this, DWORD, DWORD p1, DWORD p2) {
        auto ret = CSTATEMGR_Base__CreateSFXCTL(_this, 0, p1, p2);
        if (*(DWORD*)ret == SFXCTL_MusicPlayback_vftable) {
            SFXCTL_MusicPlayback_addr = ret;
        }
        return ret;
    }


    void __fastcall Sound__MusicLogic__Ready(Sound::MusicLogic* _this, DWORD edx, Sound::MusicSignal param_1) {
        int iVar2;
        UINT uVar3;
        using namespace Sound;

        //printf("troles MusicLogic Ready: entering function\nMusicLogic at %08X, MusicSignal %d\n", _this, param_1);

        _this->mStateName = (char*)"Sound::MusicLogic::Ready";
        switch (param_1) {
        case FE_PAUSE:
            (*_this->__vt->Tran)(_this, 0, Pause);
            return;
        case FE_MAPSCREEN:
            (*_this->__vt->Tran)(_this, 0, MapScreen);
            return;
        case FE_CARSELECT:
            (*_this->__vt->Tran)(_this, 0, CarSelect);
            return;
        case FE_CREDITS:
            (*_this->__vt->Tran)(_this, 0, Credits);
            return;
        case FE_START:
            (*_this->__vt->Tran)(_this, 0, Start);
            return;
        case PS_INIT:
            (*_this->__vt->Tran)(_this, 0, StartPursuit);
            return;
        case CD_INIT:
            (*_this->__vt->Tran)(_this, 0, StartChase);
            return;
        case RC_INIT:
            (*_this->__vt->Tran)(_this, 0, RaceInit);
            return;
        case RC_CUSTOM:
            (*_this->__vt->Tran)(_this, 0, Custom);
            return;
        case HB_INIT:
            (*_this->__vt->Tran)(_this, 0, StartBattle);
            return;
        case StartState:
            //printf("troles MusicLogic Ready: StartState\n");
            //if (((*Music_PathfinderControl >> 4 & 1) != 0) &&
            //    (*Music_PathfinderControl = *Music_PathfinderControl & 0xffffffef,
            //        *MusicProject != Music::MUSICPROJ_RACE)) {
            //    SFXCTL_MusicPlayback__QueueProject_00494650(_this->mSFXCTL, 0, Music::MUSICPROJ_RACE);
            //    *Music_PathfinderControl = *Music_PathfinderControl | 0x1000;
            //    return;
            //}
            *SkipChyron = false; //force hide any chyron message (eg fe music)
            NeedsInit = true;
            break;
        case EndState:
            //printf("troles MusicLogic Ready: EndState\n");
            //*MusicProject = (Music::MusicProject) - 1;
            //*Music_PathfinderControl = *Music_PathfinderControl | ~0xffffefff;
            //*Music_PathfinderControl = *Music_PathfinderControl & ~4;
            //*Music_PathfinderControl = *Music_PathfinderControl & ~8;

            //*Music_PathfinderControl = *Music_PathfinderControl | 32;
            //*Music_PathfinderControl = *Music_PathfinderControl & 0xfffee3ef | 0x220;
            //*Music_PathfinderControl = 0;
            //_this->mSFXCTL->mEventID = 0;
            //_this->mSFXCTL->mSwapProjectID = (Music::MusicProject) -1;

            _this->mLastState = _this->mState;
            return;
        case normal:
            //printf("troles MusicLogic Ready: Normal\n");
            if ((*Music_PathfinderControl >> 0xe & 1) == 0) {
                auto pSVar1 = _this->mSFXCTL;
                iVar2 = (pSVar1->mGameData).FEScreenID;
                switch (iVar2) {
                case 9:
                    (*_this->__vt->Dispatch)(_this, 0, FE_START);
                    return;
                case 10:
                    (*_this->__vt->Dispatch)(_this, 0, FE_PAUSE);
                    return;
                case 0xb:
                    (*_this->__vt->Dispatch)(_this, 0, FE_CARSELECT);
                    return;
                case 0xc:
                    (*_this->__vt->Dispatch)(_this, 0, FE_MAPSCREEN);
                    return;
                }
                if ((*Music_PathfinderControl & 0x438380) == 0) {
                    if ((*Music_PathfinderControl >> 0xd & 1) != 0) {
                        (*_this->__vt->Dispatch)(_this, 0, FE_CREDITS);
                        return;
                    }
                    if (iVar2 != 6) {
                        if (((*Music_PathfinderControl >> 0x12 & 1) != 0) ||
                            ((*Music_PathfinderControl >> 0x13 & 1) != 0)) {
                            (*_this->__vt->Dispatch)(_this, 0, RC_CUSTOM);
                            return;
                        }
                        uVar3 = (pSVar1->mGameData).EventType;
                        if (((uVar3 & 1) != 0) && ((pSVar1->mGameData).FEMode == 2)) {
                            (*_this->__vt->Dispatch)(_this, 0, CD_INIT);
                            return;
                        }
                        uVar3 = uVar3 & 2;
                        if ((uVar3 != 0) && ((pSVar1->mGameData).FEMode == 5)) {
                            (*_this->__vt->Dispatch)(_this, 0, HB_INIT);
                            return;
                        }
                        if (uVar3 != 0) {
                            (*_this->__vt->Dispatch)(_this, 0, RC_INIT);
                            return;
                        }
                        if ((pSVar1->mPursuitData).AvgNumCopsInFormation != 2) { //it's wrong but surely this cannot go wrong....... it fucking did (was a float)
                            (*_this->__vt->Dispatch)(_this, 0, PS_INIT);
                            return;
                        }
                    }
                }
                else {
                    *Music_PathfinderControl = *Music_PathfinderControl & 0xffbc7c7f | 32; //STOP
                }
            }
        }



        //if (*MusicProject != 3) { //RaceInit StartState
        //    SFXCTL_MusicPlayback__QueueProject_00494650(_this->mSFXCTL, 0, Music::MusicProject::MUSICPROJ_RACE);
        //    *Music_PathfinderControl = *Music_PathfinderControl | 0x1000;
        //    return;
        //}


        //if (((*Music_PathfinderControl >> 4 & 1) != 0) && //RaceMusic StartState
        //    (*Music_PathfinderControl = *Music_PathfinderControl & 0xffffffef, *MusicProject != 3)) {
        //    SFXCTL_MusicPlayback__QueueProject_00494650(_this->mSFXCTL, 0, Music::MUSICPROJ_RACE);
        //    *Music_PathfinderControl = *Music_PathfinderControl | 0x1000;
        //    return;
        //}



        //if ((*Music_PathfinderControl >> 0xc & 1) != 0) { //RaceMusic standard
        //if (-1 < (_this->mSFXCTL->mGameData).FEMode) {

        if (*GameFlow == 6 &&
            //!Sound__MusicLogic__CheckInteractiveToFEStateTrans_004a3750(_this, 0, 10) && 
            NeedsInit
            ) {
            *Music_PathfinderControl = *Music_PathfinderControl & 0xffffefff;
            //printf("trying to do the troll\n");

            DWORD index;
            if (forceNextSongIndex == -1) index = SFXCTL_MusicPlayback__GenRandomIndexFromBitfield(_this->mSFXCTL, 0, Settings::currentBitfield); //2=RACE
            else {
                index = forceNextSongIndex;
            }

            *SkipChyron = false; //skip chyron, not really meant for that but it works good enough without other chyron messages

            *Music_PathfinderControl = *Music_PathfinderControl | 8; //SWAP_PENDING

            char buf[300];

            switch (Settings::currentBitfield) {
            case PURSUIT:
                _this->mSFXCTL->mSwapProjectID = Music::MUSICPROJ_PURSUIT;
                SFXCTL_MusicPlayback__QueueValue(_this->mSFXCTL, 0, "psongID", index);
                _this->mSFXCTL->mEventID = 0x26dc944;

                snprintf(buf, sizeof(buf), "MUSIC_PURSUIT_%d", index);
                //snprintf(buf, sizeof(buf), "NOW PLAYING: %s (Pursuit song idx %d)", getLocalizedString(getBinHash(buf)), index);
                snprintf(buf, sizeof(buf), "NOW PLAYING: %s", getLocalizedString(getBinHash(buf)), index);

                break;
                //case CHASEDOWN:
                //    _this->mSFXCTL->mSwapProjectID = Music::MUSICPROJ_HIGHWAY;
                //    SFXCTL_MusicPlayback__QueueValue(_this->mSFXCTL, 0, "chasesongID", index);
                //    _this->mSFXCTL->mEventID = 0x4b87ae9;

                //    snprintf(buf, sizeof(buf), "NOW PLAYING: (Chasedown song idx %d)", index);

                //    break;
            case MAPSCREEN:
                _this->mSFXCTL->mSwapProjectID = Music::MUSICPROJ_FE_MISC;
                SFXCTL_MusicPlayback__QueueValue(_this->mSFXCTL, 0, "mpsongID", index);
                _this->mSFXCTL->mEventID = 0x18a0e57;

                snprintf(buf, sizeof(buf), "MUSIC_MAP_%d", index);
                //snprintf(buf, sizeof(buf), "NOW PLAYING: %s (Map Screen song idx %d)", getLocalizedString(getBinHash(buf)), index);
                snprintf(buf, sizeof(buf), "NOW PLAYING: %s", getLocalizedString(getBinHash(buf)), index);

                break;
            case CARHEAVEN:
                _this->mSFXCTL->mSwapProjectID = Music::MUSICPROJ_FE_MISC;
                SFXCTL_MusicPlayback__QueueValue(_this->mSFXCTL, 0, "chsongID", index);
                _this->mSFXCTL->mEventID = 0x1bd608a;

                snprintf(buf, sizeof(buf), "MUSIC_CARHEAVEN_%d", index);
                //snprintf(buf, sizeof(buf), "NOW PLAYING: %s (Car Heaven song idx %d)", getLocalizedString(getBinHash(buf)), index);
                snprintf(buf, sizeof(buf), "NOW PLAYING: %s", getLocalizedString(getBinHash(buf)), index);

                break;
            case RACE:
                auto puVar3 = Attrib::Instance::GetRaceTracks(*(Attrib::Instance**)(*EAXSound + 0x9c), 0, index);
                _this->mSFXCTL->mEventID = *puVar3;
                _this->mSFXCTL->mSwapProjectID = Music::MUSICPROJ_RACE;

                snprintf(buf, sizeof(buf), "MUSIC_RACE_%d", index);
                //snprintf(buf, sizeof(buf), "NOW PLAYING: %s (Race song idx %d ID %d)", getLocalizedString(getBinHash(buf)), index, *puVar3);
                snprintf(buf, sizeof(buf), "NOW PLAYING: %s", getLocalizedString(getBinHash(buf)), index, *puVar3);

                break;
            }

            *Music_PathfinderControl |= 4; //EVENT_PENDING

            FE__FeChyron__Summon(buf, eChyronType::CHYRON_MUSIC, 0, 0, 0);

            forceNextSongIndex = -1;
            //forceNextSongID = -1;
            NeedsInit = false;
            return;
        }

        if ((*Music_PathfinderControl >> 0xc & 1) != 0) { //Start
            //*Music_PathfinderControl = *Music_PathfinderControl & 0xffffefff;
            (*_this->__vt->Dispatch)(_this, 0, StartState);
            return;
        }


        return;
    }

}