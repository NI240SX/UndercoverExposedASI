#pragma once
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include "Settings.h"


namespace Headlights {
#define bin_HEADLIGHT_LEFT 0xA7E6EA53
#define bin_HEADLIGHT_RIGHT 0xA532FC46

    char** HEADLIGHTS_ptr;

    DWORD CRIoff_MPointsArrayPtr = 0x4C0;

    bool hasXenonHeadlights(DWORD CarRenderInfo) {
        if (*(DWORD*)(CarRenderInfo + 0x4) == 0xAAAAAAAA) {
            //cout << "CarRenderInfo@" << (DWORD*)CarRenderInfo << "\n";
            //cout << "MpointsArrayPtr " << (DWORD*)*(DWORD*)(CarRenderInfo + 0x4C0) << "\n";
            DWORD MPointsArrayPtr = *(DWORD*)(CarRenderInfo + CRIoff_MPointsArrayPtr);
            DWORD CurMPoint = *(DWORD*)MPointsArrayPtr;
            while (*(DWORD*)CurMPoint != MPointsArrayPtr) {
                DWORD MPData = *(DWORD*)(CurMPoint + 0x8);
                DWORD hash = *(DWORD*)(MPData + 0x8);
                //cout << "MPoint @" << CurMPoint << "; hash " << (DWORD*)hash << "\n";
                if (hash == bin_HEADLIGHT_LEFT || hash == bin_HEADLIGHT_RIGHT) {
                    *(DWORD*)(CarRenderInfo + 0x4) = 1;
                    //cout << "is headlight\n";
                    return true;
                }
                CurMPoint = *(DWORD*)CurMPoint;
            }
            *(DWORD*)(CarRenderInfo + 0x4) = 0;
            //cout << "no xenon headlights\n";
        }

        return *(DWORD*)(CarRenderInfo + 0x4) & 1;
    }

    //makeHeadlights
    auto tMatrix__TransformPosition = (void* (__cdecl*)(float* vec1, float* mat, float* vec2)) 0x0071b820; //__cdecl indeed
    auto SDR__StartBatch = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, int param_2)) 0x00744290;
    auto SDR__SetTexture = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, char* texturePtr)) 0x00744220;
    auto SDR__AddPoly = (void* (__fastcall*)(DWORD this_, DWORD unaff_edx, float* meshBuffer, float* matrix))0x00755d80;
    auto SDR__EndBatch = (void* (__fastcall*)(DWORD this_))0x00777f80;
    auto WWorldPos__FindClosestFace = (void(__fastcall*)(DWORD this_, DWORD unaff_edx, int param_1, bVector4 * vec, char noRayCast)) 0x00864ef0;
    auto normalizeVector_005d9950 = (void* (__cdecl*)(bVector4 * param_1, bVector4 * param_2)) 0x005d9950;
    auto WWorldPos__HeightAtPoint = (float(__fastcall*)(DWORD this_, DWORD unaff_edx, bVector4 * vec)) 0x0085e900;

    //lightsUpdateReplacement
    auto eReplacementTextures__constructor = (void* (__fastcall*)(void* this_, DWORD unaff, int*, DWORD))0x00521090;
    auto eReplacementTextures__EntryHandle__SetNewNameHash = (void* (__fastcall*)(void* this_, DWORD unaff, DWORD))0x00521060;
    auto GetCarTypeInfoFromHash = (char* (__cdecl*)(DWORD param))0x00817940;

    //asm injector
    float* DAT_00b647ec = (float*)0x00b647ec;
    auto DrawAmbientShadow = (void*)0x00825430;
    void* FUN_00520d70 = (void*)0x00520d70;

    void* FUN_00538df0 = (void*)0x00538df0; //1.0 shit


    DWORD CRIoff_mOnLights = 0x5308;
    DWORD CRIoff_mBrokenLights = 0x530c;
    DWORD CRIoff_mCrackedLights = 0x5310;
    DWORD CRIoff_headlightTextures = 0x52c8;
    DWORD CRIoff_headlightGlassTextures = 0x52d4;
    DWORD CRIoff_brakelightTextures = 0x52e0;
    DWORD CRIoff_brakelightGlassTextures = 0x52ec;
    DWORD CRIoff_LightsReplacementTextureTable = 0x8d4;
    DWORD CRIoff_mSelfIlluminationFlags = 0x54c8;
    DWORD CRIoff_terrainAdj = 0x4b8;

    DWORD RideInfoOff_VehType = 0xe44;

    void makeEPoly(float* mb) {
        mb[0] = 0.0f;//pos0 x,y,z,w
        mb[1] = 0.0f;
        mb[2] = 0.0f;
        mb[3] = 0.0f;//not initialized in the game
        mb[4] = 64.0f;//pos1
        mb[5] = 0.0f;
        mb[6] = 0.0f;
        mb[7] = 0.0f;//not initialized
        mb[8] = 64.0f;//pos2
        mb[9] = 64.0f;
        mb[10] = 0.0f;
        mb[0xb] = 0.0f;//not initialized
        mb[0xc] = 0.0f;//pos3
        mb[0xd] = 64.0f;
        mb[0xe] = 0.0f;
        mb[0xf] = 0.0f;//not initialized
        mb[0x10] = 0.0f;//tex0 u,v
        mb[0x11] = 0.0f;
        mb[0x12] = 1.0f;//tex1
        mb[0x13] = 0.0f;
        mb[0x14] = 1.0f;//tex2
        mb[0x15] = 1.0f;
        mb[0x16] = 0.0f;//tex3
        mb[0x17] = 1.0f;
        mb[0x18] = 1.0f;//?
        ((uint32_t*)mb)[0x20] = 0x80808080;//color0
        ((uint32_t*)mb)[0x21] = 0x80808080;//color1
        ((uint32_t*)mb)[0x22] = 0x80808080;//color2
        ((uint32_t*)mb)[0x23] = 0x80808080;//color3
        ((uint32_t*)mb)[0x24] = 0;//?
    }

    static const int headlightsRacer_grid_length = 10;
    static const int headlightsRacer_grid_width = 5;
    static const float headlightsRacer_length = 25.0;
    static const float headlightsRacer_widthMinFactor = 0.8;
    static const float headlightsRacer_widthIncreaseFactor = 0.5;

    static const int taillightsRacer_grid_length = 2;
    static const int taillightsRacer_grid_width = 3;
    static const float taillightsRacer_length = 3.0;
    static const float taillightsRacer_widthMinFactor = 1.7;
    static const float taillightsRacer_widthIncreaseFactor = 0.0;

    static const int headlightsTraffic_grid_length = 5;
    static const int headlightsTraffic_grid_width = 5;
    static const float headlightsTraffic_length = 15.0;
    static const float headlightsTraffic_widthMinFactor = 0.8;
    static const float headlightsTraffic_widthIncreaseFactor = 0.5;

    static const float lights_snapToGroundThresold = -0.1; //-0.05; -0.5; -1
    static const float lights_snapToGroundBaseThresold = -1.5;

    void makeHeadlightsRacer(int CarRenderInfo, float* carPositionMatrix, float* matrix_6) {
        int color = hasXenonHeadlights(CarRenderInfo) ? 0x958680 : 0x809090; //BGR
        int colorB = 0x2020A0;
        float headlightL = 0;
        float headlightR = 0;
        float taillightL = 0;
        float taillightR = 0;
        float brakelightL = 0;
        float brakelightR = 0;
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000001)) headlightL = 1.0; //color = 0x809090; //headlight left
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000002)) headlightR = 1.0; //color = 0x809090; //headlight right
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000008)) taillightL = 1.0; //
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000010)) taillightR = 1.0; //
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000008 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000008)) brakelightL = 1.0; //brake
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000010 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000010)) brakelightR = 1.0; //brake
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000040) color = 0xf0f0f0; //reverse

        //if (headlightL == 0 && headlightR == 0 && brakelightL == 0 && brakelightR == 0) return;

        //float* matrix_6 = (float*)*(int*)(CarRenderInfo + 0x4f1c);
        char meshbuffer[0x94]; // Temporary buffer for makeEPoly

        bVector4 bumperTip = { 0, 0, 0, 0 }; //{ *(float*)(CarRenderInfo + 0x380), 0, 0, 0 };
        tMatrix__TransformPosition((float*)&bumperTip, carPositionMatrix, (float*)&bumperTip);
        bVector4 bumperTip_bis = { -bumperTip.y, bumperTip.z, bumperTip.x, 0 };
        WWorldPos__FindClosestFace((CarRenderInfo + 0x3f0), 0, 0, &bumperTip_bis, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
        float terrainHeight = WWorldPos__HeightAtPoint((CarRenderInfo + 0x3f0), 0, &bumperTip_bis); //-y, z, x !!!

        float baseHeight = (terrainHeight - bumperTip.z > lights_snapToGroundBaseThresold) ? terrainHeight - bumperTip.z + 0.05 : 0.05;
        //float baseHeight = *(float*)(CarRenderInfo + 0x378);


        if (headlightR != 0 || headlightL != 0) {
            float position[headlightsRacer_grid_length][headlightsRacer_grid_width * 4];
            float texcoord[headlightsRacer_grid_length][headlightsRacer_grid_width * 2];

            float x = 0;
            float y = -*(float*)(CarRenderInfo + 0x384) * headlightsRacer_widthMinFactor; //boundsMin * factor
            float u = 0;
            // compute vertex positions
            for (int j = 0; j < headlightsRacer_grid_width; j++) {
                float v = 1;

                x = *(float*)(CarRenderInfo + 0x380); //bounds max X

                for (int i = 0; i < headlightsRacer_grid_length; i++) {
                    u = 1.0 - j / (headlightsRacer_grid_width - 1.0); //causes wack ass shit tho
                    position[i][j * 4 + 0] = x;
                    position[i][j * 4 + 1] = y + headlightsRacer_widthIncreaseFactor * x * y; //*(1+ 0.2*x*i)
                    position[i][j * 4 + 2] = baseHeight; //i*i
                    position[i][j * 4 + 3] = 0;
                    texcoord[i][j * 2 + 0] = u;
                    texcoord[i][j * 2 + 1] = v;

                    tMatrix__TransformPosition(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                    x += headlightsRacer_length / (headlightsRacer_grid_length - 1);
                    v -= 1.0 / (headlightsRacer_grid_length - 1);
                }
                y += *(float*)(CarRenderInfo + 0x384) * headlightsRacer_widthMinFactor * 2 / (headlightsRacer_grid_width - 1);
            }

            // project to terrain
            if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
                for (int i = 0; i < headlightsRacer_grid_length; i++) for (int j = 0; j < headlightsRacer_grid_width; j++) {
                    //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                    bVector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                    WWorldPos__FindClosestFace((int)(CarRenderInfo + 0x3f0), 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                    float height = WWorldPos__HeightAtPoint((int)(CarRenderInfo + 0x3f0), 0, &vector_4d0);

                    if (height != 0.0f && height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
                }
            }


            // send to renderer
            makeEPoly((float*)meshbuffer);
            //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
            //printMeshBuffer((float*)meshbuffer);

            SDR__StartBatch(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
            *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
            SDR__SetTexture(*(DWORD*)(*Renderer + 0x40), 0, *HEADLIGHTS_ptr);

            for (int i = 0; i < headlightsRacer_grid_length - 1; i++) {
                for (int j = 0; j < headlightsRacer_grid_width - 1; j++) {
                    //position
                    for (int k = 0; k < 8; k++) {
                        ((float*)meshbuffer)[k] = position[i][j * 4 + k];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 8] = position[i + 1][j * 4 + k + 4];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 12] = position[i + 1][j * 4 + k];
                    }

                    //UV
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 16] = texcoord[i][j * 2 + k];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 20] = texcoord[i + 1][j * 2 + k + 2];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 22] = texcoord[i + 1][j * 2 + k];
                    }

                    ((float*)meshbuffer)[24] = 1.0;

                    //attempt at having depth dependent intensity
                    float mixDmg = (headlightL * (j) / headlightsRacer_grid_width + headlightR * (1.0 - 1.0 * (j) / headlightsRacer_grid_width));
                    float mixDmgp1 = (headlightL * (j + 1) / headlightsRacer_grid_width + headlightR * (1.0 - 1.0 * (j + 1) / headlightsRacer_grid_width));
                    float falloff = (1.0 - (1.0 * (i + 1) / headlightsRacer_grid_length));
                    float falloffp1 = (1.0 - (1.0 * (i + 2) / headlightsRacer_grid_length));

                    ((int*)meshbuffer)[32] = (char)((int)(150.0 * mixDmg * mixDmg * falloff * falloff)) << 0x18 | color; //ABGR
                    ((int*)meshbuffer)[33] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloff * falloff)) << 0x18 | color;
                    ((int*)meshbuffer)[34] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloffp1 * falloffp1)) << 0x18 | color;
                    ((int*)meshbuffer)[35] = (char)((int)(150.0 * mixDmg * mixDmg * falloffp1 * falloffp1)) << 0x18 | color;

                    SDR__AddPoly(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
                }
            }

            SDR__EndBatch(*(DWORD*)(*Renderer + 0x40));
        }

        if (taillightL != 0 || taillightR != 0 || brakelightR != 0 || brakelightL != 0) {
            float position[taillightsRacer_grid_length][taillightsRacer_grid_width * 4];
            float texcoord[taillightsRacer_grid_length][taillightsRacer_grid_width * 2];

            float x = 0;
            float y = -*(float*)(CarRenderInfo + 0x384) * taillightsRacer_widthMinFactor; //boundsMin * factor
            float u = 0;
            // compute vertex positions
            for (int j = 0; j < taillightsRacer_grid_width; j++) {
                float v = 1;

                x = *(float*)(CarRenderInfo + 0x370); //bounds min X

                for (int i = 0; i < taillightsRacer_grid_length; i++) {
                    u = 1.0 - j / (taillightsRacer_grid_width - 1.0); //causes wack ass shit tho
                    position[i][j * 4 + 0] = x;
                    position[i][j * 4 + 1] = y + taillightsRacer_widthIncreaseFactor * x * y; //*(1+ 0.2*x*i)
                    position[i][j * 4 + 2] = baseHeight; //i*i
                    position[i][j * 4 + 3] = 0;
                    texcoord[i][j * 2 + 0] = u;
                    texcoord[i][j * 2 + 1] = v;

                    tMatrix__TransformPosition(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                    x -= taillightsRacer_length / (taillightsRacer_grid_length - 1);
                    v -= 1.0 / (taillightsRacer_grid_length - 1);
                }
                y += *(float*)(CarRenderInfo + 0x384) * taillightsRacer_widthMinFactor * 2 / (taillightsRacer_grid_width - 1);
            }

            // project to terrain
            if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
                for (int i = 0; i < taillightsRacer_grid_length; i++) for (int j = 0; j < taillightsRacer_grid_width; j++) {
                    //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                    bVector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                    WWorldPos__FindClosestFace((int)(CarRenderInfo + 0x3f0), 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                    float height = WWorldPos__HeightAtPoint((int)(CarRenderInfo + 0x3f0), 0, &vector_4d0);

                    if (height != 0.0f && height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
                }
            }


            // send to renderer
            makeEPoly((float*)meshbuffer);
            //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
            //printMeshBuffer((float*)meshbuffer);

            SDR__StartBatch(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
            *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
            SDR__SetTexture(*(DWORD*)(*Renderer + 0x40), 0, *HEADLIGHTS_ptr);

            for (int i = 0; i < taillightsRacer_grid_length - 1; i++) {
                for (int j = 0; j < taillightsRacer_grid_width - 1; j++) {
                    //position
                    for (int k = 0; k < 8; k++) {
                        ((float*)meshbuffer)[k] = position[i][j * 4 + k];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 8] = position[i + 1][j * 4 + k + 4];
                    }
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 12] = position[i + 1][j * 4 + k];
                    }

                    //UV
                    for (int k = 0; k < 4; k++) {
                        ((float*)meshbuffer)[k + 16] = texcoord[i][j * 2 + k];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 20] = texcoord[i + 1][j * 2 + k + 2];
                    }
                    for (int k = 0; k < 2; k++) {
                        ((float*)meshbuffer)[k + 22] = texcoord[i + 1][j * 2 + k];
                    }

                    ((float*)meshbuffer)[24] = 1.0;

                    //attempt at having depth dependent intensity
                    float mixDmg = ((brakelightL + taillightL) * (j + 1) / taillightsRacer_grid_width + (brakelightR + taillightR) * (1 - 1.0 * (j + 1) / taillightsRacer_grid_width));
                    float mixDmgp1 = ((brakelightL + taillightL) * (j + 2) / taillightsRacer_grid_width + (brakelightR + taillightR) * (1 - 1.0 * (j + 2) / taillightsRacer_grid_width));
                    float falloff = (1.0 - (1.0 * (i + 1) / taillightsRacer_grid_length));
                    float falloffp1 = (1.0 - (1.0 * (i + 2) / taillightsRacer_grid_length));

                    ((int*)meshbuffer)[32] = (char)((int)(40.0 * mixDmg * mixDmg * falloff * falloff)) << 0x18 | colorB; //ABGR
                    ((int*)meshbuffer)[33] = (char)((int)(40.0 * mixDmgp1 * mixDmgp1 * falloff * falloff)) << 0x18 | colorB;
                    ((int*)meshbuffer)[34] = (char)((int)(40.0 * mixDmgp1 * mixDmgp1 * falloffp1 * falloffp1)) << 0x18 | colorB;
                    ((int*)meshbuffer)[35] = (char)((int)(40.0 * mixDmg * mixDmg * falloffp1 * falloffp1)) << 0x18 | colorB;

                    SDR__AddPoly(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
                }
            }

            SDR__EndBatch(*(DWORD*)(*Renderer + 0x40));
        }
    }

    void makeHeadlightsTraffic(int CarRenderInfo, float* carPositionMatrix, float* matrix_6) {
        int color = 0x809090; //BGR
        float alphaL = 0;
        float alphaR = 0;
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000001 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000001)) alphaL = 1.0; //color = 0x809090; //headlight left
        if (*((int*)(CarRenderInfo + CRIoff_mOnLights)) & 0x00000002 && !(*((int*)(CarRenderInfo + CRIoff_mBrokenLights)) & 0x00000002)) alphaR = 1.0; //color = 0x809090; //headlight right
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000008) color = 0x202090; //brake
        //if (*((int*)(CarRenderInfo + 0x5308)) & 0x00000040) color = 0xf0f0f0; //reverse

        if (alphaR == 0 && alphaL == 0) return;

        //float* matrix_6 = (float*)*(int*)(CarRenderInfo + 0x4f1c);

        float position[headlightsTraffic_grid_length][headlightsTraffic_grid_width * 4];
        float texcoord[headlightsTraffic_grid_length][headlightsTraffic_grid_width * 2];
        char meshbuffer[0x94]; // Temporary buffer for makeEPoly

        bVector4 bumperTip = { 0, 0, 0, 0 }; //{ *(float*)(CarRenderInfo + 0x380), 0, 0, 0 };
        tMatrix__TransformPosition((float*)&bumperTip, carPositionMatrix, (float*)&bumperTip);
        bVector4 bumperTip_bis = { -bumperTip.y, bumperTip.z, bumperTip.x, 0 };
        WWorldPos__FindClosestFace(CarRenderInfo + 0x3f0, 0, 0, &bumperTip_bis, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
        float terrainHeight = WWorldPos__HeightAtPoint(CarRenderInfo + 0x3f0, 0, &bumperTip_bis); //-y, z, x !!!

        float baseHeight = (terrainHeight - bumperTip.z > lights_snapToGroundBaseThresold) ? terrainHeight - bumperTip.z + 0.05 : 0.05;
        //float baseHeight = *(float*)(CarRenderInfo + 0x378);

        float x = 0;
        float y = -*(float*)(CarRenderInfo + 0x384) * headlightsTraffic_widthMinFactor; //boundsMin * factor
        float u = 0;

        // compute vertex positions
        for (int j = 0; j < headlightsTraffic_grid_width; j++) {
            float v = 1;

            x = *(float*)(CarRenderInfo + 0x380); //bounds max X

            for (int i = 0; i < headlightsTraffic_grid_length; i++) {
                u = 1.0 - j / (headlightsTraffic_grid_width - 1.0); //causes wack ass shit tho
                position[i][j * 4 + 0] = x;
                position[i][j * 4 + 1] = y + headlightsTraffic_widthIncreaseFactor * x * y; //*(1+ 0.2*x*i)
                position[i][j * 4 + 2] = baseHeight; //i*i
                position[i][j * 4 + 3] = 0;
                texcoord[i][j * 2 + 0] = u;
                texcoord[i][j * 2 + 1] = v;

                tMatrix__TransformPosition(&position[i][j * 4], carPositionMatrix, &position[i][j * 4]);

                x += headlightsTraffic_length / (headlightsTraffic_grid_length - 1);
                v -= 1.0 / (headlightsTraffic_grid_length - 1);
            }
            y += *(float*)(CarRenderInfo + 0x384) * headlightsTraffic_widthMinFactor * 2 / (headlightsTraffic_grid_width - 1);
        }

        // project to terrain
        if (*(int*)(CarRenderInfo + CRIoff_terrainAdj) != 0) {
            for (int i = 0; i < headlightsTraffic_grid_length; i++) for (int j = 0; j < headlightsTraffic_grid_width; j++) {
                //vector_4d0 = { -vbuf[i].y, vbuf[i].z, vbuf[i].x };
                bVector4 vector_4d0 = { -position[i][j * 4 + 1], position[i][j * 4 + 2], position[i][j * 4 + 0] };

                WWorldPos__FindClosestFace(CarRenderInfo + 0x3f0, 0, 0, &vector_4d0, 1); // had *(int*)(CarRenderInfo + 0x4b8) instead of 0, it was causing the rays to tweak somehow
                float height = WWorldPos__HeightAtPoint(CarRenderInfo + 0x3f0, 0, &vector_4d0);

                if (height != 0.0f && height - position[i][j * 4 + 2] > lights_snapToGroundThresold) position[i][j * 4 + 2] = height + 0.05; //0.5* i / grid_length
            }
        }

        // send to renderer
        makeEPoly((float*)meshbuffer);
        //std::cout << "mesh buffer initialized on stack " << meshbuffer << "\n";
        //printMeshBuffer((float*)meshbuffer);

        SDR__StartBatch(*(DWORD*)(*Renderer + 0x40), 0, 4, 0);
        *(int*)(*(int*)(*Renderer + 0x40) + 0xf4) = -1;
        SDR__SetTexture(*(DWORD*)(*Renderer + 0x40), 0, *HEADLIGHTS_ptr);

        for (int i = 0; i < headlightsTraffic_grid_length - 1; i++) {
            for (int j = 0; j < headlightsTraffic_grid_width - 1; j++) {
                //position
                for (int k = 0; k < 8; k++) {
                    ((float*)meshbuffer)[k] = position[i][j * 4 + k];
                }
                for (int k = 0; k < 4; k++) {
                    ((float*)meshbuffer)[k + 8] = position[i + 1][j * 4 + k + 4];
                }
                for (int k = 0; k < 4; k++) {
                    ((float*)meshbuffer)[k + 12] = position[i + 1][j * 4 + k];
                }

                //UV
                for (int k = 0; k < 4; k++) {
                    ((float*)meshbuffer)[k + 16] = texcoord[i][j * 2 + k];
                }
                for (int k = 0; k < 2; k++) {
                    ((float*)meshbuffer)[k + 20] = texcoord[i + 1][j * 2 + k + 2];
                }
                for (int k = 0; k < 2; k++) {
                    ((float*)meshbuffer)[k + 22] = texcoord[i + 1][j * 2 + k];
                }

                ((float*)meshbuffer)[24] = 1.0;

                //attempt at having depth dependent intensity

                float mixDmg = (alphaL * (j) / headlightsTraffic_grid_width + alphaR * (1.0 - 1.0 * (j) / headlightsTraffic_grid_width));
                float mixDmgp1 = (alphaL * (j + 1) / headlightsTraffic_grid_width + alphaR * (1.0 - 1.0 * (j + 1) / headlightsTraffic_grid_width));
                float falloff = (1.0 - (1.0 * (i + 1) / headlightsTraffic_grid_length));
                float falloffp1 = (1.0 - (1.0 * (i + 2) / headlightsTraffic_grid_length));

                ((int*)meshbuffer)[32] = (char)((int)(150.0 * mixDmg * mixDmg * falloff * falloff)) << 0x18 | color; //ABGR
                ((int*)meshbuffer)[33] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloff * falloff)) << 0x18 | color;
                ((int*)meshbuffer)[34] = (char)((int)(150.0 * mixDmgp1 * mixDmgp1 * falloffp1 * falloffp1)) << 0x18 | color;
                ((int*)meshbuffer)[35] = (char)((int)(150.0 * mixDmg * mixDmg * falloffp1 * falloffp1)) << 0x18 | color;

                SDR__AddPoly(*(DWORD*)(*Renderer + 0x40), 0, (float*)meshbuffer, matrix_6);
            }
        }
        SDR__EndBatch(*(DWORD*)(*Renderer + 0x40));

    }

    float __stdcall ReplaceCarShadow(DWORD CarRenderInfo, bVector4* param_3, float* carPositionMatrix, float* matrix_5, float* mat_6, int param_7) {
        if (*GameFlow != 6) return 1.0f;
        if (Settings::ProjectionCutoff && (*((float*)((int)theSunAndSky + Settings::ProjectionCutoffField)) > Settings::ProjectionCutoffThresold)) return 1.0f;
        if (*HEADLIGHTS_ptr == 0) return 1.0f;

        //field CarRenderInfo + 43c has other data -> towards pvehicle ?
        switch (*(int*)(*(int*)(CarRenderInfo + 0x43c) + RideInfoOff_VehType)) { //0 player 1 ? 2 racer 3 cop 4 traffic 5 heli 6 ? 7 ?
        case 0: //player
        case 2: //racer
            //cout << "CarRenderInfo@" << (DWORD*)CarRenderInfo << "\n";
            //cout << "Lights combination (CRI + 0x5308) @ " << ((int*)(CarRenderInfo + 0x5308)) << " = " << (int*)*((int*)(CarRenderInfo + 0x5308)) << "\n";
            //cout << "Class???@" << (int*)*((int*)(CarRenderInfo + 0x43c)) << " (CRI + 43c) \n";
            //cout << "Class??? +1D00 " << (int*)*(int*)(*((int*)(CarRenderInfo + 0x43c)) + 0x1D00) << " \n";

            makeHeadlightsRacer(CarRenderInfo, carPositionMatrix, mat_6);
            break;
        case 3: //cop
        case 4: //traffic
            // avoid drawing headlights in front of truck trailers
            if (*(float*)(CarRenderInfo + 0x370) < -6.0) return 1.0f;

            makeHeadlightsTraffic(CarRenderInfo, carPositionMatrix, mat_6);
            break;
        }
        return 1.0f;
    }

    void* replaceCarshadowDetourReturn;
    __declspec(naked) void replaceCarshadowDetour() {
        __asm {

            //comparison stuff
            MOVSS      XMM0, dword ptr[ESI + 0x5840]
            COMISS     XMM0, zero
            JBE        LAB_008494fd

            MOV        ECX, EBX
            CALL       FUN_00520d70
            TEST       AL, AL
            JNZ        LAB_008494fd

            CMP        dword ptr[EBX + 0x4], 0x1c
            JZ         LAB_008494fd

            //\/ one of these checks that the car is not a heli
            MOV        EDX, dword ptr[ESI + 0x43c]
            MOV        EAX, dword ptr[EDX + 0xe44]
            CMP        EAX, 0x7
            JZ         LAB_008494fd

            CMP        EAX, 0x5
            JZ         LAB_008494fd
            //^ one of these checks that the car is not a heli

            CMP        dword ptr[EBP + 0x34], 0x0   //param_12
            JNZ        LAB_008494fd




            MOV        EAX, dword ptr[EBP + 0x24]   //param_8
            FLD        dword ptr[EBP + 0x38]          //param_13      [EBP+38] onto the FPU stack ST(0)
            MOV        ECX, dword ptr[ESI + 0x4f1c]
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]    // = > local_1d0
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]    // = > matrix_210
            PUSH       EAX
            PUSH       ECX
            FSTP       dword ptr[ESP]       // = > local_2a4        stores ST(0) to [ESP] ???
            LEA        ECX, [ESP + 0x64]    // = > vector4_240
            PUSH       ECX
            PUSH       EBX
            MOV        ECX, ESI
            CALL       DrawAmbientShadow
            FSTP       ST(0)


            //injected function
            //inject after carshadow for better render sort
            MOV        EAX, dword ptr[EBP + 0x24]   //param_8
            //FLD        dword ptr[EBP + 0x38]          //param_13      [EBP+38] onto the FPU stack ST(0)
            MOV        ECX, dword ptr[ESI + 0x4f1c]
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]    // = > local_1d0
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]    // = > matrix_210
            PUSH       EAX
            PUSH       ECX
            //FSTP       dword ptr[ESP]       // = > local_2a4        stores ST(0) to [ESP] ???
            //LEA        ECX, [ESP + 0x64]    // = > vector4_240
            //PUSH       ECX
            //PUSH       EBX
            PUSH       ESI                  // replaced from MOV ECX, ESI -> thiscall to stdcall
            CALL       ReplaceCarShadow          //float10 * 1_drawCarShadow_008254
            FSTP       ST(0)




        LAB_008494fd:
            jmp replaceCarshadowDetourReturn;

        }
    }
    __declspec(naked) void replaceCarshadowDetour_10() {
        __asm {

            MOV        ECX, EBX
            CALL       FUN_00538df0
            TEST       AL, AL
            JNZ        LAB_00878b19

            MOV        ECX, EBX
            CALL       FUN_00520d70 //FUN_00538e10
            TEST       AL, AL
            JNZ        LAB_00878b19

            MOV        EDX, dword ptr[ESI + 0x43c]
            MOV        EAX, dword ptr[EDX + 0xad0]
            CMP        EAX, 0x7
            JZ         LAB_00878b19

            CMP        EAX, 0x5
            JZ         LAB_00878b19

            CMP        dword ptr[EBP + 0x34], 0x0
            JNZ        LAB_00878b19

            MOV        EAX, dword ptr[EBP + 0x24]
            FLD        dword ptr[EBP + 0x38]
            MOV        ECX, dword ptr[ESI + 0x4f0c]
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]
            PUSH       EAX
            PUSH       ECX
            FSTP       dword ptr[ESP]
            LEA        ECX, [ESP + 0x64]
            PUSH       ECX
            PUSH       EBX
            MOV        ECX, ESI
            CALL       DrawAmbientShadow
            FSTP       ST(0)


            //injected function
            //inject after carshadow for better render sort
            MOV        EAX, dword ptr[EBP + 0x24]   //param_8
            //FLD        dword ptr[EBP + 0x38]          //param_13      [EBP+38] onto the FPU stack ST(0)
            MOV        ECX, dword ptr[ESI + 0x4f0c] //4f1c in 1.18
            PUSH       EAX
            PUSH       ECX
            LEA        EDX, [ESP + 0xc8]    // = > local_1d0
            PUSH       EDX
            LEA        EAX, [ESP + 0x8c]    // = > matrix_210
            PUSH       EAX
            PUSH       ECX
            //FSTP       dword ptr[ESP]       // = > local_2a4        stores ST(0) to [ESP] ???
            //LEA        ECX, [ESP + 0x64]    // = > vector4_240
            //PUSH       ECX
            //PUSH       EBX
            PUSH       ESI                  // replaced from MOV ECX, ESI -> thiscall to stdcall
            CALL       ReplaceCarShadow          //float10 * 1_drawCarShadow_008254
            FSTP       ST(0)




        LAB_00878b19:
            jmp replaceCarshadowDetourReturn;

        }
    }

    __declspec(naked) void getRawFlaresDetour() {
        __asm {

            mov ecx, dword ptr[esi + 0x84]; //PVehicle
            mov eax, dword ptr[ecx + 0x9c]; //light flares
            mov dword ptr[edi + 0xe0], eax;

            pop edi;
            pop esi;
            add esp, 0x1c;
            ret 0x4;
        }
    }
    __declspec(naked) void getRawFlaresDetour_10() {
        __asm {

            mov ecx, dword ptr[esi + 0x84]; //PVehicle
            mov eax, dword ptr[ecx + 0x9c]; //light flares
            mov dword ptr[edi + 0xe0], eax;

            pop edi;
            pop esi;
            add esp, 0x20;
            ret 0x4;
        }
    }

    void* setEmissiveLightsDetourReturn;
    __declspec(naked) void setEmissiveLightsDetour() {
        __asm {
            MOV     EAX, dword ptr[EBP + 0x18];

            TEST    dword ptr[EAX + 0x40], 0x40000000; // material flag for HEADLIGHTREFLECTOR
            JZ      notHeadlightReflector;

            TEST    EBX, 0x100000; //areHeadlightsOn (new)
            JNZ     lightOn;
            JMP     setEmissiveLightsDetourReturn;


        notHeadlightReflector:
            TEST    EBX, 0x200000; //areBrakelightsOn
            JNZ     lightOn;
            JMP     setEmissiveLightsDetourReturn;


        lightOn:                               // XREF[1]:     007612ed(j)
            MOVSS   dword ptr[ESI + 0xc], XMM1; // = > DAT_3f800000, emissive light on

            JMP     setEmissiveLightsDetourReturn;
        }
    }

    void __fastcall lightsUpdateReplacement(DWORD CarRenderInfo) {

        void* pvVar3;
        int texIDBrakeL;
        int texIDBrakeR;
        int texIDBGlassL;
        int texIDBGlassR;
        int texIDBrakeC;
        int buffer[2];

        DWORD* electricsState = (DWORD*)(CarRenderInfo + CRIoff_mOnLights);
        DWORD* lightsDamage = (DWORD*)(CarRenderInfo + CRIoff_mBrokenLights);
        DWORD* lightsGlassDamage = (DWORD*)(CarRenderInfo + CRIoff_mCrackedLights);
        DWORD* headlightTextures = (DWORD*)(CarRenderInfo + CRIoff_headlightTextures); //within UsedCarTextureInfo
        DWORD* headlightGlassTextures = (DWORD*)(CarRenderInfo + CRIoff_headlightGlassTextures);
        DWORD* brakelightTextures = (DWORD*)(CarRenderInfo + CRIoff_brakelightTextures);
        DWORD* brakelightGlassTextures = (DWORD*)(CarRenderInfo + CRIoff_brakelightGlassTextures);
        void* this_00 = (void*)(CarRenderInfo + CRIoff_LightsReplacementTextureTable);

        //get headlight texture index
        //can't be switched to zero on vanilla cars because they lack headlight off textures
        int texIDHeadL = 1;
        int texIDHeadR = 1;
        int texIDHGlassL = 1;
        int texIDHGlassR = 1;

        if ((*lightsGlassDamage & 0x00000001)) texIDHGlassL = 2;
        if ((*lightsDamage & 0x00000001)) { texIDHeadL = 2; texIDHGlassL = 2; }
        if ((*lightsGlassDamage & 0x00000002)) texIDHGlassR = 2;
        if ((*lightsDamage & 0x00000002)) { texIDHeadR = 2; texIDHGlassR = 2; }

        //removed unnecessary bit for headlights

        //get headlights textures
        int texHeadL = headlightTextures[texIDHeadL];
        int texHeadR = headlightTextures[texIDHeadR];
        int texHGlassL = headlightGlassTextures[texIDHGlassL];
        int texHGlassR = headlightGlassTextures[texIDHGlassR];

        //toggle headlight damage textures
        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 1);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHeadL); //headlights 

        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 2);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHeadR); //headlights (only one necessary)

        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 3);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHGlassL); //headlight glass

        pvVar3 = eReplacementTextures__constructor(this_00, 0, buffer, 4);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, texHGlassR); //headlight glass (only one necessary)

        /* checks lights damage */
        texIDBrakeL = 0;
        texIDBrakeR = 0;
        texIDBrakeC = 0;
        texIDBGlassL = 0;
        texIDBGlassR = 0;

        if (!(*lightsDamage & 0x00000008)) { //left brakelight
            if ((*electricsState & 0x00000008)) {
                texIDBrakeL = 1;
                texIDBGlassL = 1;
                if ((*lightsGlassDamage & 0x00000008)) {
                    texIDBGlassL = 2;
                }
            }
        }
        else {
            texIDBrakeL = 2;
            texIDBGlassL = 2;
        }

        if (!(*lightsDamage & 0x00000010)) { //right brakelight
            if ((*electricsState & 0x00000010)) {
                texIDBrakeR = 1;
                texIDBGlassR = 1;
                if ((*lightsGlassDamage & 0x00000010)) {
                    texIDBGlassR = 2;
                }
            }
        }
        else {
            texIDBrakeR = 2;
            texIDBGlassR = 2;
        }

        if (!(*lightsDamage & 0x00000020)) { //center brakelight
            if ((*electricsState & 0x00000020)) {
                texIDBrakeC = 1;
            }
        }
        else {
            texIDBrakeC = 2;
        }

        //removed unnecessary bit for brakelights

        if (*(int*)(GetCarTypeInfoFromHash(**(DWORD**)(CarRenderInfo + 0x43c)) + 0xf4) != 2) {

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 5);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[(texIDBrakeL == 2) ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 6);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[(texIDBrakeR == 2) ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 7);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[(texIDBrakeC == 2) ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 8);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassL == 2 ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 9);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassR == 2 ? 2 : 0]);

            pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 10);
            eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBrakeC == 2 ? 2 : 0]);


            //manage headlights
            if ((*electricsState & 0x00000003)) { //&& !(lightsDamage & 0x00000003)
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) |= 0x100000;
            }
            else {
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) &= ~0x100000;
            }
            //if (((texIDHeadL != 2) && (texIDHeadR != 2)) && ((texIDHGlassL != 0 && (texIDHGlassR != 0))) && (*electricsState & 0x00000003)) { //shuts off the whole car when headlights broken, not the wanted effect !
            //    *(int*)(CarRenderInfo + 0x54c8) |= 0x100000;
            //}
            //else {
            //    *(int*)(CarRenderInfo + 0x54c8) &= ~0x100000;
            //}

            //manage brakelights
            if (((texIDBrakeL != 2) && (texIDBrakeR != 2)) && ((texIDBGlassL != 0 && (texIDBGlassR != 0)))) {
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) |= 0x200000;
            }
            else {
                *(int*)(CarRenderInfo + CRIoff_mSelfIlluminationFlags) &= ~0x200000;
            }
            return;
        }

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 5);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeL]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 6);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeR]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 7);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeC]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 8);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassL]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 9);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightGlassTextures[texIDBGlassR]);

        pvVar3 = (void*)eReplacementTextures__constructor(this_00, 0, buffer, 10);
        eReplacementTextures__EntryHandle__SetNewNameHash(pvVar3, 0, brakelightTextures[texIDBrakeC]);

        return;
    }

    void __fastcall headlightsReplacement(DWORD this_, DWORD unused, DWORD arg1) {

        DWORD* PVehicle = (DWORD*)(this_ + 0xb8);

        if (*(DWORD*)(this_ + 0x860) == 9999) {
            *(char*)(this_ + 0x863) = 1; //auto mode
            PVehicle[0x27] = PVehicle[0x27] | 0x00000007;

            std::uniform_int_distribution<> headlightSensitivity(-INT16_MIN, INT16_MAX);
            *(int16_t*)(this_ + 0x860) = headlightSensitivity(gen);
        }

        if (*((float*)((int)theSunAndSky + Settings::AutomaticHeadlightsField)) > (Settings::AutomaticHeadlightsThresold * (1.0 + *(int16_t*)(this_ + 0x860) / 327670.0))) {
            //auto turn off
            if (*(char*)(this_ + 0x863) == 1) {
                PVehicle[0x27] = PVehicle[0x27] & ~0x00000007;
                *(char*)(this_ + 0x863) = 0;
            }
            //headlights forced on, do not turn off, set them back to auto if the player manually turns them off
            if (*(char*)(this_ + 0x863) == 2) {
                *(char*)(this_ + 0x863) = 0;
            }
        }
        else {
            if (*(char*)(this_ + 0x863) == 0) {
                //headlights already on, save that they're forced
                if (PVehicle[0x27] & 0x00000007) {
                    *(char*)(this_ + 0x863) = 2;
                }
                //auto turn on
                else {
                    PVehicle[0x27] = PVehicle[0x27] | 0x00000007;
                    *(char*)(this_ + 0x863) = 1;
                }
            }
        }
    }


}