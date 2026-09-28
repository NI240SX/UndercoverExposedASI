#pragma once
#include <sys/types.h>
#include "nfs_structs.h"
#include "MiscGameFuncs.h"
#include <map>
#include <ctime>
#include <fstream>
#include <chrono>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_internal.h"
#include "imgui/implot.h"
//GetLocalizedString (EALocalizationManager something)
//

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

constexpr auto CollectionName = 0x9CA1C8F9;

namespace AttribEditor {


    std::ofstream modscript;
    bool modscriptInitialized = false;

    std::map<int, const char*> AttribTypes;
    std::map<int, const char*> AttribClasses;
    std::map<int, const char*> AttribCollections;
    std::map<int, const char*> AttribFields;
    std::map<int, const char*> BinHashes;

    bool GCollectionKeysInitialized = false;
    std::map<int, const char*> GCollectionKeys;

    void initializeModscript() {
        modscript.open("generated.nfsms", std::ios_base::app | std::ios_base::out);

        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto time_buf = new char[26];
        ctime_s(time_buf, 26, &time);

        modscript << "\n#Generated script during session from " << time_buf << "#This script may not work as is!\n\n";




        //trollingly creating an enum
        //if (Settings::tests) {
        //    for (auto& t : AttribTypes) {
        //        int i = 0;
        //        char* next = (char*)t.second;
        //        while (t.second[i] != 0) {
        //            if (t.second[i] == ':') next = (char*)&t.second[i + 1];
        //            i++;
        //        }
        //        ucetesting::modscript << next << " = 0x" << (DWORD*)t.first << ",\n";
        //        ucetesting::modscript.flush();

        //    }
        //}






        modscriptInitialized = true;
    }

    void ReadHashes(const char* filename, std::map<int, const char*>& map)
    {

        std::ifstream inputFile(filename);
        std::string line;
        try {
            while (std::getline(inputFile, line)) {
                if (line.empty()) continue;
                if (line[0] == '#') continue;
                auto arr = new char[line.size() + 1];
                strncpy_s(arr, line.size() + 1, line.c_str(), line.size());
                int hash = getVltHash(arr);
                map[hash] = arr;
            }
        }
        catch (std::exception& e) {
            std::cout << "Error reading " << filename << ": " << e.what() << std::endl;
        }
        inputFile.close();
    }

    void InitHashMaps() {
        //Fill in hash maps
                    //addAttribType("EA::Reflection::Int64");
            //addAttribType("EA::Reflection::Int32");
            //addAttribType("EA::Reflection::Int16");
            //addAttribType("EA::Reflection::Int8");
            //addAttribType("EA::Reflection::UInt64");
            //addAttribType("EA::Reflection::UInt32");
            //addAttribType("EA::Reflection::UInt16");
            //addAttribType("EA::Reflection::UInt8");
            //addAttribType("EA::Reflection::Char");
            //addAttribType("EA::Reflection::Bool");
            //addAttribType("EA::Reflection::Float");
            //addAttribType("EA::Reflection::Double");
            //addAttribType("EA::Reflection::Text");
            //addAttribType("EA::Reflection::Reference");
            //addAttribType("EA::Reflection::Type");
            //addAttribType("EA::Reflection::KeyValueAttrib");
            //addAttribType("Attrib::Blob");
            //addAttribType("Attrib::RefSpec");
            //addAttribType("Attrib::Definition");
            //addAttribType("Attrib::Types::Symbol");
            //addAttribType("Attrib::Types::Colour");
            //addAttribType("Attrib::Types::FloatColour");
            //addAttribType("Attrib::Types::ShaderColour");
            //addAttribType("Attrib::Types::Matrix");
            //addAttribType("Attrib::Types::Quaternion");
            //addAttribType("Attrib::Types::Vector4");
            //addAttribType("Attrib::Types::Vector3");
            //addAttribType("Attrib::Types::Vector2");

        ReadHashes("AttribTypes.txt", AttribTypes);
        ReadHashes("AttribClasses.txt", AttribClasses);
        ReadHashes("AttribCollections.txt", AttribCollections);
		ReadHashes("AttribFields.txt", AttribFields);
        ReadHashes("BinHashes.txt", BinHashes);
    }

    void InitGCollectionKeys() {
        auto gameplay = Attrib::Database::GetClass(*Attrib::DatabasePrivate, 0, getVltHash("gameplay"));
        if (gameplay == NULL) return;

        for (int i = 0; i < gameplay->mPrivates->mCollections.__inherit_0.mTableSize; i++) {
            auto chi = Attrib::FindCollection(gameplay->mKey, gameplay->mPrivates->mCollections.__inherit_0.mTable[i].mKey, NULL);
            if (chi != NULL) {
                GCollectionKeys[chi->mKey] = *(char**)Attrib::Collection::GetData(chi, 0, CollectionName, 0); //okayish for display only
            }
        }
    }


	static enum ModscriptHeaderType {
        update_field,
        update_collection
	};

	static ModscriptHeaderType headerType = update_field;
    static char* CurrentClassName;
    static char* CurrentCollectionName;
    static char* CurrentAttributeName;

    void WriteModscriptHeader(int arrayIndex, char* arrayIndexBuf) {
        switch (headerType) {
		case update_field:
			modscript << "update_field ";
			break;
		case update_collection:
			modscript << "update_collection ";
			break;
        }
        modscript << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << (arrayIndex == -1 ? " " : arrayIndexBuf);
    }

    static const float one_f = 1.0f;
    static const float ten_f = 10.0f;

    static const int one_i = 1;
    static const int ten_i = 10;
    static const int u8_max = 255;


    static const ImVec4 colorNotExisting{
        0.6f, 0.2f, 0.2f, 1.0f
    };
    static const ImVec4 colorNotSupported{
        0.6f, 0.5f, 0.2f, 1.0f
    };



    const char* GetCollectionName(DWORD classKey, DWORD collectionKey) {
        if (AttribCollections[collectionKey] != NULL) {
            return AttribCollections[collectionKey];
        }
        Attrib::Collection* chi = Attrib::FindCollection(classKey, collectionKey, NULL);
        if (chi != NULL) {
            int NameDefOffset = -1;
            char** colName;
            for (int i = 0; i < chi->mClass->mPrivates->mNumDefinitions; i++) {
                if (chi->mClass->mPrivates->mDefinitions[i].mKey == CollectionName && chi->mClass->mPrivates->mDefinitions[i].mFlags & Attrib::Definition::Flags::kInLayout) {
                    NameDefOffset = chi->mClass->mPrivates->mDefinitions[i].mOffset;
                    break;
                }
            }
            if (NameDefOffset != -1) colName = (char**)(chi->mLayout + NameDefOffset);
            else (colName = (char**)Attrib::Collection::GetData(chi, 0, CollectionName, 0));

            if (colName && *colName && **colName) {
                return *colName;
            }
    //        {
    //            char buf[10];
				//snprintf(buf, sizeof(buf), "0x%08x", collectionKey);
				//return buf;
    //        }
        }
        else return "(collection not found)";
        return NULL;
    }

    void EditRefSpec(int data, int arrayIndex, char  arrayIndexBuf[8], Attrib::Collection** nextCollection, const char* classText, const char* collectionText, bool displayVltedNames = true)
    {
        DWORD* classKey = &((DWORD*)data)[0];
        DWORD* collectionKey = &((DWORD*)data)[1];
        DWORD* collectionPtr = &((DWORD*)data)[2];

        ImGui::SetNextItemWidth(-FLT_MIN);
        float width = ImGui::CalcItemWidth();
        ImGui::SetNextItemWidth(width / 3);
        ImGui::Text(displayVltedNames ? classText : "Class"); ImGui::SameLine(width / 3);
        ImGui::SetNextItemWidth(width / 3);
        ImGui::Text(displayVltedNames ? collectionText : "Collection");
        //static int lastAttribute = 0;
        static char classKeyBuf[64];
        static char collectionKeyBuf[100];
        //if (lastAttribute != def->mKey) {
        if (AttribClasses[*classKey] != NULL) snprintf(classKeyBuf, sizeof(classKeyBuf), "%s", AttribClasses[*classKey]);
        else snprintf(classKeyBuf, sizeof(classKeyBuf), "0x%08x", *classKey);
        if (GetCollectionName(*classKey, *collectionKey)) snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "%s", GetCollectionName(*classKey, *collectionKey));
        else snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "0x%08x", *collectionKey);
        //lastAttribute = def->mKey;
        //}

        ImGui::PushID(1);
        ImGui::SetNextItemWidth(width / 3);
        if (ImGui::InputText("", classKeyBuf, sizeof(classKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (classKeyBuf[0] == '0' && classKeyBuf[1] == 'x') *classKey = strtoul(classKeyBuf, NULL, 16);
            else *classKey = getVltHash(classKeyBuf);
            *collectionPtr = (DWORD)Attrib::FindCollection(*classKey, *collectionKey, 0);
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            modscript << classText << " " << classKeyBuf << "\n";
        }
        ImGui::PopID();
        ImGui::PushID(2);
        //ImGui::SameLine(width/3);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(width / 3);
        if (ImGui::InputText("", collectionKeyBuf, sizeof(collectionKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (collectionKeyBuf[0] == '0' && collectionKeyBuf[1] == 'x') *collectionKey = strtoul(collectionKeyBuf, NULL, 16);
            else *collectionKey = getVltHash(collectionKeyBuf);
            *collectionPtr = (DWORD)Attrib::FindCollection(*classKey, *collectionKey, 0);
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            modscript << collectionText << " " << collectionKeyBuf << "\n";
        }
        ImGui::PopID();
        //ImGui::SameLine(2*width/3);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(width / 3);
        if (ImGui::Button("Go to collection")) {
            *nextCollection = Attrib::FindCollection(*classKey, *collectionKey, 0);
        }
    }

    void EditFloatN(int data, int arrayIndex, char  arrayIndexBuf[8], int count, const char* n...)
    {
        const char** names = &n;
        bool wronglyNamed = false;
        if (names[0] == NULL) {
            names = &names[1];
            wronglyNamed = true;
        }

        float width = ImGui::CalcItemWidth();
        for (int i = 1; i < count; i++) {
            ImGui::Text(names[i - 1]); ImGui::SameLine(width * i / count);
        }
        ImGui::Text(names[count - 1]);
        ImGui::SetNextItemWidth(width);
        //int result = ImGui::InputScalarN("", ImGuiDataType_Float, (float*)(data), count, (void*)(&one_f), (void*)(&ten_f), "%.3f", ImGuiInputTextFlags_AutoSelectAll);
        //for (int i = 0; i < count; i++) {
        //    if (result & (1 << i)) {
        //        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
        //        modscript << names[i] << " " << ((float*)data)[i] << "\n";
        //    }
        //}

        ImGuiContext& g = *GImGui;
        ImGui::PushMultiItemsWidths(count, width);
        for (int i = 0; i < count; i++)
        {
            ImGui::PushID(i);
            if (i > 0) ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
            
            //if (ImGui::InputScalar("", ImGuiDataType_Float, (void*)(data + 4 * i), (void*)(&one_f), (void*)(&ten_f), "%.3f", ImGuiInputTextFlags_AutoSelectAll)) {
            ImGui::DragFloat("", (float*)(data + 4 * i), 0.005f);
		    if (ImGui::IsItemDeactivatedAfterEdit()) {
				WriteModscriptHeader(arrayIndex, arrayIndexBuf);
				modscript << names[(wronglyNamed? count : 0) + i] << " " << ((float*)data)[i] << "\n";
            }
            ImGui::PopID();
            ImGui::PopItemWidth();
        }





    }
    void EditFloatN(void* data, int arrayIndex, char  arrayIndexBuf[8], int count, const char* n...) {
        EditFloatN((int)data, arrayIndex, arrayIndexBuf, count, n);
    }

    void EditBStringHash(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool doVltScript = true, bool displayVltedNames = true)
    {
        char stringBuf[100];
		if (BinHashes[*(int*)data] != NULL) snprintf(stringBuf, sizeof(stringBuf), "%s", BinHashes[*(int*)data]);
		else snprintf(stringBuf, sizeof(stringBuf), "0x%08x", *(int*)data);

        float width = ImGui::CalcItemWidth();

        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::SetNextItemWidth(width / 5);
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
                ImGui::SameLine(width / 5);
            }
            ImGui::PushID(dataFieldName);
        }
        ImGui::SetNextItemWidth(2 * width / 5);
        if (ImGui::InputText("", stringBuf, 99, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (stringBuf[0] == '0' && stringBuf[1] == 'x') *(int*)data = strtoul(stringBuf, NULL, 16);
            else *(int*)data = getBinHash(stringBuf);
            if (doVltScript) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                if (dataFieldName != NULL) modscript << dataFieldName << " ";
                modscript << "0x" << (DWORD*)(*(int*)data) << "\n"; //force hash for vlted, else this stupid ass gonn use vlt hashing
            }
        }
        auto localized = getLocalizedString((*(int*)data));
        if (localized != NULL) {
            ImGui::SameLine(3 * width / 5);
            ImGui::Text(localized);
        }

        if (dataFieldName != NULL) ImGui::PopID();
    }

    template<typename T>
    ImGuiDataType_ GetImGuiDataTypeFromType() {
        if (std::is_same<T, UINT8>::value) return ImGuiDataType_U8;
        if (std::is_same<T, INT8>::value) return ImGuiDataType_S8;
		if (std::is_same<T, UINT16>::value) return ImGuiDataType_U16;
		if (std::is_same<T, INT16>::value) return ImGuiDataType_S16;
		if (std::is_same<T, UINT32>::value) return ImGuiDataType_U32;
		if (std::is_same<T, INT32>::value) return ImGuiDataType_S32;
		if (std::is_same<T, UINT64>::value) return ImGuiDataType_U64;
		if (std::is_same<T, INT64>::value) return ImGuiDataType_S64;
		if (std::is_same<T, float>::value) return ImGuiDataType_Float;
		if (std::is_same<T, double>::value) return ImGuiDataType_Double;
		if (std::is_same<T, char>::value) return ImGuiDataType_S8;
		if (std::is_same<T, bool>::value) return ImGuiDataType_Bool;
		if (std::is_same<T, char*>::value) return ImGuiDataType_String;
    }

    template<typename T>
    void EditInt(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }

        //if (ImGui::InputScalar("", GetImGuiDataTypeFromType<T>(), (T*)data, (const void*)1, (const void*)10, "%d", ImGuiInputTextFlags_AutoSelectAll)) {
		ImGui::DragScalar("", GetImGuiDataTypeFromType<T>(), (T*)data, 1.0f);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
			if (std::is_same<T,UINT8>::value || std::is_same<T, INT8>::value) modscript << (int)(*(T*)data) << "\n";
			else modscript << *(T*)data << "\n";
        }
        if (dataFieldName != NULL) ImGui::PopID();
    }

	void EditInt(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true) {
		EditInt<INT32>(data, arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
	}
	void EditByte(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true) {
		EditInt<UINT8>(data, arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
	}


    void EditFloat(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }
        //if (ImGui::InputFloat("", (float*)data)) {
		ImGui::DragFloat("", (float*)data, 0.005f);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *(float*)data << "\n";
        }
        if (dataFieldName != NULL) ImGui::PopID();
    }

    void EditFloatHex(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }
        //if (ImGui::InputFloat("", (float*)data)) {
		ImGui::DragFloat("", (float*)data, 0.005f);
        if(ImGui::IsItemDeactivatedAfterEdit()){
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << "0x" << (*(DWORD**)data) << "\n";
        }
        if (dataFieldName != NULL) ImGui::PopID();
    }

    //void EditByte(int data, char* className, char* collectionName, char* attributeName, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    //{
    //    if (dataFieldName != NULL) {
    //        if (displayVltedNames) {
    //            ImGui::Text(dataFieldName);
    //            ImGui::SameLine();
    //        }
    //        ImGui::PushID(dataFieldName);
    //    }
    //    if (ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U8, (UINT8*)data, (const void*)1, (const void*)10, "%d", ImGuiInputTextFlags_AutoSelectAll)) {
    //        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
    //        if (dataFieldName != NULL) modscript << dataFieldName << " ";
    //        modscript << (int)(*(UINT8*)data) << "\n";
    //    }
    //    if (dataFieldName != NULL) ImGui::PopID();
    //}

    void EditText(char** stringVal, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL)
    {
        char stringBuf[100];

        //static int lastAttribute = 0;
        if (stringVal != NULL && *stringVal != NULL) { //&& lastAttribute != def->mKey
            snprintf(stringBuf, sizeof(stringBuf), *stringVal);
            //lastAttribute = def->mKey;
        }

        if (ImGui::InputText("", stringBuf, 99, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
            int l = 0;
            while (stringBuf[l] != 0) l++;
            *stringVal = new char[l + 1];
            memcpy_s(*stringVal, l + 1, stringBuf, l + 1);
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *stringVal << "\n";
        }
    }

    void EnumCombo(int data, const char* items_separated_by_zeroes, int arrayIndex, char arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }

        float width = ImGui::CalcItemWidth();
        ImGui::SetNextItemWidth(2 * width / 3);
        if (ImGui::Combo("##enum", (int*)data, items_separated_by_zeroes)) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *(UINT32*)data << "\n";
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (ImGui::InputInt("##enumManual", (int*)data)) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *(UINT32*)data << "\n";
        }

        if (dataFieldName != NULL) ImGui::PopID();
    }

    void EnumFlags(int data, const char* items_separated_by_zeroes, int arrayIndex, char arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }

        float width = ImGui::CalcItemWidth();
        ImGui::SetNextItemWidth(2 * width / 3);
        bool showFlags = ImGui::TreeNodeEx("Flags", ImGuiTreeNodeFlags_NoTreePushOnOpen);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (ImGui::InputInt("##enumManual", (int*)data)) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *(UINT32*)data << "\n";
        }

        if (showFlags) {
            ImGui::TreePush("Flags");
            int items_count = 0;
            const char* p = items_separated_by_zeroes;
            int idx = 0;

            {
                bool cur = (*(int*)data) & 1 << idx;
                if (ImGui::Checkbox(p, &cur)) {
                    if (cur) {
                        (*(int*)data) |= 1 << idx;
                    }
                    else {
                        (*(int*)data) &= ~(1 << idx);
                    }
                    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                    if (dataFieldName != NULL) modscript << dataFieldName << " ";
                    modscript << *(UINT32*)data << "\n";
                }
                p += ImStrlen(p) + 1;

            }

            while (*p) {
                bool cur = (*(int*)data) & 1 << idx;
                if (ImGui::Checkbox(p, &cur)) {
                    if (cur) {
                        (*(int*)data) |= 1 << idx;
                    }
                    else {
                        (*(int*)data) &= ~(1 << idx);
                    }
                    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                    if (dataFieldName != NULL) modscript << dataFieldName << " ";
                    modscript << *(UINT32*)data << "\n";
                }
                p += ImStrlen(p) + 1;
                idx++;
            }

            ImGui::TreePop();
        }

        if (dataFieldName != NULL) ImGui::PopID();
    }

    void EnumCombo(int data, const char** items, int count, int arrayIndex, char arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true) {
        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }

        float width = ImGui::CalcItemWidth();
        ImGui::SetNextItemWidth(2 * width / 3);
        if (ImGui::Combo("##enum", (int*)data, items, count, count)) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *(UINT32*)data << "\n";
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (ImGui::InputInt("##enumManual", (int*)data)) {
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            if (dataFieldName != NULL) modscript << dataFieldName << " ";
            modscript << *(UINT32*)data << "\n";
        }

        if (dataFieldName != NULL) ImGui::PopID();
    }

    void EditePaintSlot(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        EnumCombo(data, "CUSTOM_PAINT_SLOT_BODY\0CUSTOM PAINT_SLOT_RIMS_FRONT\0CUSTOM PAINT_SLOT_RIMS_REAR\0CUSTOM_PAINT_SLOT_ROLLCAGE\0CUSTOM PAINT_SLOT_BUMPER_FRONT\0CUSTOM PAINT_SLOT_BUMPER_REAR\0CUSTOM PAINT_SLOT_SIDESKIRT\0CUSTOM_PAINT_SLOT_HOOD\0CUSTOM PAINT_SLOT_MIRROR\0CUSTOM PAINT_SLOT_SPOILER\0CUSTOM_PAINT_SLOT_WINDOW TINT\0CUSTOM PAINT_SLOT ROOFSCOOP\0MAX_CUSTOM_PAINT_SLOTS\0INVALID_PAINT_SLOT",
            arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
    }

    void EditePartMaterialFlags(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        EnumCombo(data, "PARTMAT_NONE\0PARTMAT_CARBON\0PARTMAT_CHROME\0PARTMAT_MAGSILVER\0PARTMAT_GLASS\0PARTMAT_GUNMETAL\0PARTMAT_UNUSED3\0PARTMAT_UNUSED4\0PARTMAT_UNUSED5\0NUM_PART_MAT",
            arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
    }

    void EditAutosculptRegionID(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        EnumCombo(data, "RegionFrontBumper\0RegionRearBumper\0RegionSkirtLeft\0RegionSkirtRight\0RegionRimsFront\0RegionRimsRear\0RegionDoorLeft\0RegionDoorRight\0RegionDoorRearLeft\0RegionDoorRearRight\0RegionFenderFrontLeft\0RegionFenderFrontRight\0RegionBody\0RegionHoods\0RegionSpoilers\0RegionRoofScoops\0RegionRoofHeight\0RegionExhaust\0RegionFrontWindow\0RegionRearWindow\0RegionFrontLeftWindow\0RegionFrontRightWindow\0RegionRearLeftWindow\0RegionRearRightWindow\0RegionLeftDoor\0RegionRightDoor\0RegionInterior\0RegionDoorline\0RegionWindTunnelType1\0RegionWindTunnelType2\0RegionWindTunnelType3\0RegionWindTunnelType4\0RegionWindTunnelType5\0RegionWindTunnelType6\0RegionWindTunnelType7\0RegionWindTunnelType8\0RegionWindTunnelType9\0RegionWindTunnelType10\0RegionRearBumperExtra\0RegionFrontBumperExtra\0RegionNum\0RegionInvalid",
            arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
    }


    void EditCAR_SLOT_ID(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        EnumCombo(data, "CARSLOTID_BADGING_BUMPER_SET_FRONT\0CARSLOTID_BADGING_BUMPER_SET_REAR\0CARSLOTID_BADGING_FENDER_FRONT_LEFT\0CARSLOTID_BADGING_FENDER_FRONT_RIGHT\0CARSLOTID_BADGING_FENDER_REAR_LEFT\0CARSLOTID_BADGING_FENDER_REAR_RIGHT\0CARSLOTID_BADGING_TRUNK\0CARSLOTID_BASE\0CARSLOTID_BODY\0CARSLOTID_BODY_DOORLINE\0CARSLOTID_BRAKE_CALIPER_FRONT\0CARSLOTID_BRAKE_CALIPER_REAR\0CARSLOTID_BRAKE_ROTOR_FRONT\0CARSLOTID_BRAKE_ROTOR_REAR\0CARSLOTID_BRAKELIGHT_GLASS_LEFT\0CARSLOTID_BRAKELIGHT_GLASS_RIGHT\0CARSLOTID_BRAKELIGHT_LEFT\0CARSLOTID_BRAKELIGHT_RIGHT\0CARSLOTID_BUMPER_FRONT\0CARSLOTID_BUMPER_FRONT_EXTRA\0CARSLOTID_BUMPER_REAR\0CARSLOTID_BUMPER_REAR_EXTRA\0CARSLOTID_CHASSIS\0CARSLOTID_DAMAGE_BODY\0CARSLOTID_DAMAGE_BRAKELIGHT_LEFT\0CARSLOTID_DAMAGE_BRAKELIGHT_RIGHT\0CARSLOTID_DAMAGE_BUMPER_FRONT\0CARSLOTID_DAMAGE_BUMPER_REAR\0CARSLOTID_DAMAGE_DOOR_LEFT\0CARSLOTID_DAMAGE_DOOR_REAR_LEFT\0CARSLOTID_DAMAGE_DOOR_REAR_RIGHT\0CARSLOTID_DAMAGE_DOOR_RIGHT\0CARSLOTID_DAMAGE_FENDER_FRONT_LEFT\0CARSLOTID_DAMAGE_FENDER_FRONT_RIGHT\0CARSLOTID_DAMAGE_HEADLIGHT_LEFT\0CARSLOTID_DAMAGE_HEADLIGHT_RIGHT\0CARSLOTID_DAMAGE_HOOD\0CARSLOTID_DAMAGE_SIDEMIRROR_LEFT\0CARSLOTID_DAMAGE_SIDEMIRROR_RIGHT\0CARSLOTID_DAMAGE_TRUNK\0CARSLOTID_DAMAGE_TRUNK911\0CARSLOTID_DAMAGE_WIDEBODY\0CARSLOTID_DAMAGE_WIDEBODY_BUMPER_FRONT\0CARSLOTID_DAMAGE_WIDEBODY_BUMPER_REAR\0CARSLOTID_DAMAGE_WIDEBODY_DOOR_LEFT\0CARSLOTID_DAMAGE_WIDEBODY_DOOR_REAR_LEFT\0CARSLOTID_DAMAGE_WIDEBODY_DOOR_REAR_RIGHT\0CARSLOTID_DAMAGE_WIDEBODY_DOOR_RIGHT\0CARSLOTID_DAMAGE_WIDEBODY_FENDER_FRONT_LEFT\0CARSLOTID_DAMAGE_WIDEBODY_FENDER_FRONT_RIGHT\0CARSLOTID_DAMAGE_WINDOW_FRONT\0CARSLOTID_DAMAGE_WINDOW_FRONT_LEFT\0CARSLOTID_DAMAGE_WINDOW_FRONT_RIGHT\0CARSLOTID_DAMAGE_WINDOW_REAR\0CARSLOTID_DAMAGE_WINDOW_REAR_LEFT\0CARSLOTID_DAMAGE_WINDOW_REAR_RIGHT\0CARSLOTID_DECAL_FRONT_WINDOW\0CARSLOTID_DECAL_REAR_WINDOW\0CARSLOTID_DOOR_LEFT\0CARSLOTID_DOOR_REAR_LEFT\0CARSLOTID_DOOR_REAR_RIGHT\0CARSLOTID_DOOR_RIGHT\0CARSLOTID_DOORHANDLE_FRONT_LEFT\0CARSLOTID_DOORHANDLE_FRONT_RIGHT\0CARSLOTID_DOORHANDLE_REAR_LEFT\0CARSLOTID_DOORHANDLE_REAR_RIGHT\0CARSLOTID_DRIVER\0CARSLOTID_ENGINE\0CARSLOTID_EXHAUST\0CARSLOTID_EXHAUST_TIPS_CENTER\0CARSLOTID_EXHAUST_TIPS_LEFT\0CARSLOTID_EXHAUST_TIPS_RIGHT\0CARSLOTID_FENDER_FRONT_LEFT\0CARSLOTID_FENDER_FRONT_RIGHT\0CARSLOTID_HEADLIGHT_GLASS_LEFT\0CARSLOTID_HEADLIGHT_GLASS_RIGHT\0CARSLOTID_HEADLIGHT_LEFT\0CARSLOTID_HEADLIGHT_RIGHT\0CARSLOTID_HOOD\0CARSLOTID_LICENSE_PLATE\0CARSLOTID_LIGHTBAR\0CARSLOTID_ROLL_CAGE\0CARSLOTID_ROOF\0CARSLOTID_ROOF_INSIDE\0CARSLOTID_ROOFSCOOP\0CARSLOTID_SEAT_LEFT\0CARSLOTID_SEAT_RIGHT\0CARSLOTID_SIDEMIRROR_LEFT\0CARSLOTID_SIDEMIRROR_RIGHT\0CARSLOTID_SKIRT_LEFT\0CARSLOTID_SKIRT_RIGHT\0CARSLOTID_SPOILER\0CARSLOTID_STEERINGWHEEL\0CARSLOTID_TRUNK\0CARSLOTID_UNIVERSAL_SPOILER_BASE\0CARSLOTID_WHEEL_FRONT\0CARSLOTID_WHEEL_REAR\0CARSLOTID_WINDOW_FRONT\0CARSLOTID_WINDOW_FRONT_LEFT\0CARSLOTID_WINDOW_FRONT_RIGHT\0CARSLOTID_WINDOW_REAR\0CARSLOTID_WINDOW_REAR_LEFT\0CARSLOTID_WINDOW_REAR_RIGHT\0CARSLOTID_BRAKE_PACKAGE\0CARSLOTID_DRIVETRAIN_PACKAGE\0CARSLOTID_ENGINE_PACKAGE\0CARSLOTID_FORCED_INDUCTION_PACKAGE\0CARSLOTID_INTERIOR\0CARSLOTID_LIVERY\0CARSLOTID_NITROUS_PACKAGE\0CARSLOTID_SUSPENSION_PACKAGE\0CARSLOTID_TIRE_FRONT\0CARSLOTID_TIRE_PACKAGE\0CARSLOTID_TIRE_REAR\0CARSLOTID_VECTORVINYL",
            arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
    }
    void EditCAR_PART_ID(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        EnumCombo(data, "CARPARTID_BADGING_BUMPER_SET_FRONT\0CARPARTID_BADGING_BUMPER_SET_REAR\0CARPARTID_BADGING_FENDER_FRONT_LEFT\0CARPARTID_BADGING_FENDER_FRONT_RIGHT\0CARPARTID_BADGING_FENDER_REAR_LEFT\0CARPARTID_BADGING_FENDER_REAR_RIGHT\0CARPARTID_BADGING_TRUNK\0CARPARTID_BASE\0CARPARTID_BODY\0CARPARTID_BODY_DOORLINE\0CARPARTID_BRAKE\0CARPARTID_BRAKELIGHT_GLASS_LEFT\0CARPARTID_BRAKELIGHT_GLASS_RIGHT\0CARPARTID_BRAKELIGHT_LEFT\0CARPARTID_BRAKELIGHT_RIGHT\0CARPARTID_BRAKEROTOR\0CARPARTID_BUMPER_FRONT\0CARPARTID_BUMPER_FRONT_EXTRA\0CARPARTID_BUMPER_REAR\0CARPARTID_BUMPER_REAR_EXTRA\0CARPARTID_CHASSIS\0CARPARTID_DAMAGE_BODY\0CARPARTID_DAMAGE_BRAKELIGHT_LEFT\0CARPARTID_DAMAGE_BRAKELIGHT_RIGHT\0CARPARTID_DAMAGE_BUMPER_FRONT\0CARPARTID_DAMAGE_BUMPER_REAR\0CARPARTID_DAMAGE_DOOR_LEFT\0CARPARTID_DAMAGE_DOOR_REAR_LEFT\0CARPARTID_DAMAGE_DOOR_REAR_RIGHT\0CARPARTID_DAMAGE_DOOR_RIGHT\0CARPARTID_DAMAGE_FENDER_FRONT_LEFT\0CARPARTID_DAMAGE_FENDER_FRONT_RIGHT\0CARPARTID_DAMAGE_HEADLIGHT_LEFT\0CARPARTID_DAMAGE_HEADLIGHT_RIGHT\0CARPARTID_DAMAGE_HOOD\0CARPARTID_DAMAGE_SIDEMIRROR_LEFT\0CARPARTID_DAMAGE_SIDEMIRROR_RIGHT\0CARPARTID_DAMAGE_TRUNK\0CARPARTID_DAMAGE_WINDOW_FRONT\0CARPARTID_DAMAGE_WINDOW_FRONT_LEFT\0CARPARTID_DAMAGE_WINDOW_FRONT_RIGHT\0CARPARTID_DAMAGE_WINDOW_REAR\0CARPARTID_DAMAGE_WINDOW_REAR_LEFT\0CARPARTID_DAMAGE_WINDOW_REAR_RIGHT\0CARPARTID_DECAL_FRONT_WINDOW\0CARPARTID_DECAL_REAR_WINDOW\0CARPARTID_DOOR_LEFT\0CARPARTID_DOOR_REAR_LEFT\0CARPARTID_DOOR_REAR_RIGHT\0CARPARTID_DOOR_RIGHT\0CARPARTID_DOORHANDLE_FRONT_LEFT\0CARPARTID_DOORHANDLE_FRONT_RIGHT\0CARPARTID_DOORHANDLE_REAR_LEFT\0CARPARTID_DOORHANDLE_REAR_RIGHT\0CARPARTID_DRIVER\0CARPARTID_ENGINE\0CARPARTID_EXHAUST\0CARPARTID_EXHAUST_TIPS_CENTER\0CARPARTID_EXHAUST_TIPS_LEFT\0CARPARTID_EXHAUST_TIPS_RIGHT\0CARPARTID_FENDER_FRONT_LEFT\0CARPARTID_FENDER_FRONT_RIGHT\0CARPARTID_HEADLIGHT_GLASS_LEFT\0CARPARTID_HEADLIGHT_GLASS_RIGHT\0CARPARTID_HEADLIGHT_LEFT\0CARPARTID_HEADLIGHT_RIGHT\0CARPARTID_HOOD\0CARPARTID_LIGHTBAR\0CARPARTID_PLATES\0CARPARTID_ROLL_CAGE\0CARPARTID_ROOF\0CARPARTID_ROOF_INSIDE\0CARPARTID_ROOFSCOOP\0CARPARTID_SEAT\0CARPARTID_SIDEMIRROR_LEFT\0CARPARTID_SIDEMIRROR_RIGHT\0CARPARTID_SKIRT_LEFT\0CARPARTID_SKIRT_RIGHT\0CARPARTID_SPOILER\0CARPARTID_STEERINGWHEEL\0CARPARTID_TRUNK\0CARPARTID_UNIVERSAL_SPOILER_BASE\0CARPARTID_WHEEL\0CARPARTID_WHEEL_REAR\0CARPARTID_WINDOW_FRONT\0CARPARTID_WINDOW_FRONT_LEFT\0CARPARTID_WINDOW_FRONT_RIGHT\0CARPARTID_WINDOW_REAR\0CARPARTID_WINDOW_REAR_LEFT\0CARPARTID_WINDOW_REAR_RIGHT\0CARPARTID_BRAKE_PACKAGE\0CARPARTID_DRIVETRAIN_PACKAGE\0CARPARTID_ENGINE_PACKAGE\0CARPARTID_FORCED_INDUCTION_PACKAGE\0CARPARTID_INTERIOR\0CARPARTID_LIVERY\0CARPARTID_NITROUS_PACKAGE\0CARPARTID_SUSPENSION_PACKAGE\0CARPARTID_TIRE\0CARPARTID_TIRE_PACKAGE\0CARPARTID_VECTORVINYL\0CARPARTID_NUM",
            arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
    }

    void EditTireCondition(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        EnumCombo(data, "kGrip\0kWet\0kFlat\0kDrift\0kDrag",
            arrayIndex, arrayIndexBuf, dataFieldName, displayVltedNames);
    }

    constexpr auto car_paint_swatch = 0x8C3CC099;
    constexpr auto swatch = 0x4256700E;

    void PreviewSwatchPacked(int swatchPointer) {
        auto col = Attrib::FindCollection(car_paint_swatch, swatch, NULL);
        if (col == NULL) return;
        struct CarPaintSwatch* cps = (struct CarPaintSwatch*)Attrib::Collection::GetData(col, 0, swatch, *(UINT8*)(swatchPointer));

        float rgb[3]; //BGR
        if (cps != NULL) {
            rgb[0] = 1.0f / 255.0f * ((cps->rgb >> 16) & 0xFF);
            rgb[1] = 1.0f / 255.0f * ((cps->rgb >> 8) & 0xFF);
            rgb[2] = 1.0f / 255.0f * ((cps->rgb >> 0) & 0xFF);
        }

        float hsv[3];
        ImGui::ColorConvertRGBtoHSV(rgb[0], rgb[1], rgb[2], hsv[0], hsv[1], hsv[2]);
        hsv[1] = 1.0f / 255.0f * *(UINT8*)(swatchPointer + 1);
        hsv[2] = 1.0f / 255.0f * *(UINT8*)(swatchPointer + 2);
        ImGui::ColorEdit3("", hsv, ImGuiColorEditFlags_InputHSV);
    }

    void PreviewSwatch(int swatchPointer) {
        auto col = Attrib::FindCollection(car_paint_swatch, swatch, NULL);
        if (col == NULL) return;
        struct CarPaintSwatch* cps = (struct CarPaintSwatch*)Attrib::Collection::GetData(col, 0, swatch, *(UINT8*)(swatchPointer));

        float rgb[3]; //BGR
        if (cps != NULL) {
            rgb[0] = 1.0f / 255.0f * ((cps->rgb >> 16) & 0xFF);
            rgb[1] = 1.0f / 255.0f * ((cps->rgb >> 8) & 0xFF);
            rgb[2] = 1.0f / 255.0f * ((cps->rgb >> 0) & 0xFF);
        }

        float hsv[3];
        ImGui::ColorConvertRGBtoHSV(rgb[0], rgb[1], rgb[2], hsv[0], hsv[1], hsv[2]);
        hsv[1] = *(float*)(swatchPointer + 4);
        hsv[2] = *(float*)(swatchPointer + 8);
        ImGui::ColorEdit3("", hsv, ImGuiColorEditFlags_InputHSV);
    }

    void EditColorPacked(const char* colorText, int data, float totalWidth, bool& edited) {
        bool doColor = ImGui::TreeNodeEx(colorText, ImGuiTreeNodeFlags_NoTreePushOnOpen);
        ImGui::PushID(colorText);
        ImGui::SameLine(); PreviewSwatchPacked(data);
        ImGui::PopID();
        if (doColor) {
            ImGui::TreePush(colorText);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Swatch"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::InputScalar("##swatch", ImGuiDataType_S8, (UINT8*)(data), &one_i, &ten_i)) {
                //TODO clamp to max swatch
                edited = true;
            }

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Saturation"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::SliderScalar("##sat", ImGuiDataType_U8, (UINT8*)(data + 1), &zero, &u8_max)) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Brightness"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::SliderScalar("##bright", ImGuiDataType_U8, (UINT8*)(data + 2), &zero, &u8_max)) edited = true;

            ImGui::TreePop();
        }
    }

    bool HideHeritedAttributes = false;
    bool HideEmptyAttributes = false;

    void PreviewCurve(struct Curve* curve) {
        if (ImGui::TreeNode("Curve preview")) {
            //ImGui::Text("better than vlted huh");

            float* xval = new float[curve->X.mLength];
            float* yval = new float[curve->Y.mLength];

            for (int i = 0; i < curve->X.mLength; i++) {
                xval[i] = (curve->xMin + (curve->xMax - curve->xMin) * curve->X.mArray[i]);// *curve->graphScale; //NOT SURE ABOUT GRAPH SCALE
            }
            for (int i = 0; i < curve->Y.mLength; i++) {
                yval[i] = (curve->yMin + (curve->yMax - curve->yMin) * curve->Y.mArray[i]);// *curve->graphScale;
            }


            if (ImPlot::BeginPlot("##GraphPreview")) {
				ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine(CurrentAttributeName, xval, yval, IM_MIN(curve->X.mLength, curve->Y.mLength));
                //ImPlot::PlotScatter("Data 2", xs2, ys2, 50, {
                //    ImPlotProp_Marker, ImPlotMarker_Square,
                //    ImPlotProp_MarkerSize, 6,
                //    ImPlotProp_LineColor, GetColormapColor(1),
                //    ImPlotProp_FillColor, GetColormapColor(1),
                //    ImPlotProp_FillAlpha, 0.25f
                //    });
                ImPlot::EndPlot();
            }

            delete[] xval;
            delete[] yval;
            ImGui::TreePop();
        }
    }

    void EditGraphData(struct Curve* curve, int arrayIndex, char  arrayIndexBuf[8], const char* prefix = "", const char* X = "XValues", const char* Y = "YValues", const char* Y2 = "Y2Values")
    {
        if (ImGui::TreeNode("Graph data")) {
            headerType = update_collection; //why vlted
            ImGui::SetNextItemWidth(-FLT_MIN);
            float width = ImGui::CalcItemWidth() / 3;
            for (int i = 0; i < curve->X.mLength || i < curve->Y.mLength || i < curve->Y2.mLength; i++) {
                ImGui::PushID(i);
                ImGui::Text("");
                if (i < curve->X.mLength) {
                    ImGui::PushID(1);
                    ImGui::SameLine(0);
                    ImGui::Text("X[%d]", i);
                    //ImGui::SameLine(20.0f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(width - 20.0f);
                    ImGui::DragFloat("", &curve->X.mArray[i], 0.005f);
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                        modscript << prefix << X << " " << i << " Value " << curve->X.mArray[i] << "\n";
                    }
                    ImGui::PopID();
                }
                if (i < curve->Y.mLength) {
                    ImGui::PushID(2);
                    ImGui::SameLine(width);
                    ImGui::Text("Y[%d]", i);
                    //ImGui::SameLine(width + 20.0f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(width - 20.0f);
                    ImGui::DragFloat("", &curve->Y.mArray[i], 0.005f);
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                        modscript << prefix << Y << " " << i << " Value " << curve->Y.mArray[i] << "\n";
                    }
                    ImGui::PopID();
                }
                if (i < curve->Y2.mLength) {
                    ImGui::PushID(3);
                    ImGui::SameLine(2 * width);
                    ImGui::Text("Y2[%d]", i);
                    //ImGui::SameLine(2* width + 20.0f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(width - 20.0f);
                    ImGui::DragFloat("", &curve->Y2.mArray[i], 0.005f);
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                        modscript << prefix << Y2 << " " << i << " Value " << curve->Y2.mArray[i] << "\n";
                    }
                    ImGui::PopID();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
            headerType = update_field; //why vlted
        }
    }

    void EditColourF4(int data, int arrayIndex, char  arrayIndexBuf[8], bool isVector4 = false)
    {
        ImGui::ColorEdit4("", (float*)data, ImGuiColorEditFlags_AlphaBar);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            if (isVector4) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "X " << ((float*)data)[0] << "\n";
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Y " << ((float*)data)[1] << "\n";
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Z " << ((float*)data)[2] << "\n";
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "W " << ((float*)data)[3] << "\n";
            }
            else {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "R " << ((float*)data)[0] << "\n";
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "G " << ((float*)data)[1] << "\n";
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "B " << ((float*)data)[2] << "\n";
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "A " << ((float*)data)[3] << "\n";
            }
        }
    }

    void CorrectedText(float totalWidth, const char* text)
    {
        ImGui::SetNextItemWidth(totalWidth / 2);
        ImGui::Text(text); ImGui::SameLine(totalWidth / 2);
        ImGui::SetNextItemWidth(totalWidth / 2);
    }

    void EditStringKey(int data, int arrayIndex, char  arrayIndexBuf[8], const char* dataFieldName = NULL, bool displayVltedNames = true)
    {
        char stringBuf[100];

        DWORD* stringKey = &((DWORD*)data)[0];
        char** stringVal = (char**)&((DWORD*)data)[1];

        if (dataFieldName != NULL) {
            if (displayVltedNames) {
                ImGui::Text(dataFieldName);
                ImGui::SameLine();
            }
            ImGui::PushID(dataFieldName);
        }


        //static int lastAttribute = 0;
        if (stringVal != NULL && *stringVal != NULL) { //&& lastAttribute != def->mKey
            snprintf(stringBuf, sizeof(stringBuf), *stringVal);
            //lastAttribute = def->mKey;
        }

        auto totalWidth = ImGui::CalcItemWidth();
        ImGui::SetNextItemWidth(totalWidth * 2.0f / 3.0f);
        if (ImGui::InputText("", stringBuf, 99, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
            //TODO MEMORY LEAK
            int l = 0;
            while (stringBuf[l] != 0) l++;
            *stringVal = new char[l + 1];
            memcpy_s(*stringVal, l + 1, stringBuf, l + 1);
            *stringKey = getVltHash(*stringVal);
            WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            modscript << *stringVal << "\n";
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(totalWidth / 3.0f);
        ImGui::Text("0x%08x", *stringKey); // - Text attribute not supported yet

        if (dataFieldName != NULL) ImGui::PopID();
    }

    void EditAttrib(int data, Attrib::Definition* def, Attrib::Collection* currentCollection, Attrib::Collection** nextCollection, int arrayIndex, bool notAnArray = true) {
        //Array index -1 = not an array
        //Array index -2 = array header

        if (HideEmptyAttributes && data == NULL) return;

        bool supported = true;
        bool isInCurrentCollection = def->mFlags & Attrib::Definition::Flags::kInLayout ? true : Attrib::Collection::Contains(currentCollection, 0, def->mKey);
        if (!isInCurrentCollection) {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, colorNotExisting);
        }

        if (notAnArray) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
        }
        ImGui::AlignTextToFramePadding();

        char arrayIndexBuf[8];
        snprintf(arrayIndexBuf, sizeof(arrayIndexBuf), "[%d] ", arrayIndex);

        if (notAnArray) ImGui::TextUnformatted((arrayIndex < 0 ? CurrentAttributeName : arrayIndexBuf));
        ImGui::TableNextColumn();


        ImGui::SetNextItemWidth(-FLT_MIN);

        if (data == NULL) {
            if (!isInCurrentCollection) {
                //ImGui::SameLine(); ImGui::Text("NOT IN CURRENT COLLECTION");
                ImGui::PopStyleColor();
            }

            ImGui::TableNextColumn();
            //ImGui::SameLine();
            if (def->mFlags & Attrib::Definition::Flags::kArray) {
                ImGui::Text("Create local, size=");
                static int count = 1;
                ImGui::SameLine();
                ImGui::SetNextItemWidth(20.0f);
                if (ImGui::InputInt("##arraysize", &count, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    Attrib::Collection::AddAttribute(currentCollection, 0, def->mKey, count);
                    modscript << "add_field " << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << " " << count << "\n";
                }
            }
            else
                if (ImGui::Button("Create")) {
                    Attrib::Collection::AddAttribute(currentCollection, 0, def->mKey, 0);
                    modscript << "add_field " << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << "\n";
                }

            ImGui::SameLine();
            if (arrayIndex == -2) ImGui::Text("(array of %s)", AttribTypes[def->mType]);
            else ImGui::Text("(%s)", AttribTypes[def->mType]);

            return;
        }

        Attrib::Instance instance;
        Attrib::Attribute attrib;

        headerType = update_field;
        if (arrayIndex == -2) {
            //RESIZING STUFF
            // 


                //this will require an Instance apparently, go figure why it isn't needed in most other cases
            Attrib::Instance::constructor(&instance, 0, currentCollection, 0);
            Attrib::Collection::Get(currentCollection, 0, &attrib, &instance, def->mKey);

            //ImGui::Text("Resize, size=");
            //static int count = Attrib::Attribute::GetLength(&attrib);
            //ImGui::SameLine();
            //ImGui::PushItemWidth(20.0f);
            //if (ImGui::InputInt("##arrayresize", &count, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue)) {
            //    Attrib::Attribute::SetLength(&attrib, 0, count); //doesnt work ???
            //    //modscript << "resize_field " << className << " " << collectionName << " " << attributeName << " " << count << "\n";
            //}
            //ImGui::PopItemWidth();


            DWORD key = def->mKey;
            ImGui::Text("Resize, size=");
            //int count = ((Attrib::Array*)Attrib::Collection::GetNode(currentCollection, 0, key, &key))->mSize;
            int count = Attrib::Attribute::GetLength(&attrib);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(20.0f);
            if (ImGui::InputInt("##arrayresize", &count, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue)) {
                char* oldData = new char[count * def->mSize];
                for (int i = 0; i < count; i++) {
                    auto src = (void*)Attrib::Collection::GetData(currentCollection, 0, def->mKey, i);
                    if (src != NULL) memcpy(oldData + def->mSize * i, src, def->mSize);
                }

                Attrib::Collection::RemoveAttribute(currentCollection, 0, def->mKey);
                Attrib::Collection::AddAttribute(currentCollection, 0, def->mKey, count);

                for (int i = 0; i < count; i++) {
                    auto dst = (void*)Attrib::Collection::GetData(currentCollection, 0, def->mKey, i);
                    if (dst != NULL) memcpy(dst, oldData + def->mSize * i, def->mSize);
                }
                delete[] oldData;

                modscript << "resize_field " << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << " " << count << "\n";
            }



            //use Attrib::Collection::CopyAttribute to initialize new attribute from parent ?
            //use Attrib::Collection::RemoveAttribute to remove attribute


        }
        else switch (def->mType) {
        case AttribTypes::Int64:
            EditInt<INT64>(data, arrayIndex, arrayIndexBuf);
            break;
        case AttribTypes::Int32:
            EditInt<INT32>(data, arrayIndex, arrayIndexBuf);
            break;
        case AttribTypes::Int16:
            EditInt<INT16>(data, arrayIndex, arrayIndexBuf);
            break;
        case AttribTypes::Int8:
            EditInt<INT8>(data, arrayIndex, arrayIndexBuf);
            break;

        case AttribTypes::UInt64:
            EditInt<UINT64>(data, arrayIndex, arrayIndexBuf);
            break;

        case AttribTypes::UInt32:
            if (currentCollection->mClass->mKey == getVltHash("emitterdata")) {
				float color[4] = {(float)((UINT8*)data)[3]/255.0f, (float)((UINT8*)data)[2]/255.0f, (float)((UINT8*)data)[1]/255.0f, (float)((UINT8*)data)[0]/255.0f};
                int result = ImGui::ColorEdit4("", color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8);
				if (result) {
					((UINT8*)data)[0] = (UINT8)(color[3] * 255.0f);
					((UINT8*)data)[1] = (UINT8)(color[2] * 255.0f);
					((UINT8*)data)[2] = (UINT8)(color[1] * 255.0f);
					((UINT8*)data)[3] = (UINT8)(color[0] * 255.0f);
                    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                    modscript << *(UINT32*)data << "\n";
                }                
                break;
            }
            EditInt<UINT32>(data, arrayIndex, arrayIndexBuf);
            break;

        case AttribTypes::UInt16:
            EditInt<UINT16>(data, arrayIndex, arrayIndexBuf);
            break;

        case AttribTypes::UInt8:
            EditInt<UINT8>(data, arrayIndex, arrayIndexBuf);
            //if (ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U8, (UINT8*)data, (const void*)1, (const void*)10, "%d", ImGuiInputTextFlags_AutoSelectAll)) {
            //    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            //    modscript << (int)(*(UINT8*)data) << "\n";
            //}
            break;

        case AttribTypes::Bool:
            if (ImGui::Checkbox("", (bool*)data)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << (*(bool*)data ? "True" : "False") << "\n";
            }
            break;

        case AttribTypes::Char:
            EditInt<UINT8>(data, arrayIndex, arrayIndexBuf);
            //if (ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U8, (char*)data, (const void*)1, (const void*)10, "%d", ImGuiInputTextFlags_AutoSelectAll)) {
            //    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            //    modscript << *(char*)data << "\n";
            //}
            break;

        case AttribTypes::Float:
            //if (ImGui::InputFloat("", (float*)data, 0.01f, 1.0f, "%.3f", ImGuiInputTextFlags_AutoSelectAll)) {
			ImGui::DragFloat("", (float*)data, 0.005f);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << *(float*)data << "\n";
            }
            break;

        case AttribTypes::Double:
            //if (ImGui::InputDouble("", (double*)data, 0.01f, 1.0f, "%.6", ImGuiInputTextFlags_AutoSelectAll)) {
			ImGui::DragScalar("", ImGuiDataType_Double, (double*)data, 0.005f);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << *(double*)data << "\n";
            }
            break;


        case AttribTypes::Text:
        {
            char** stringVal = (char**)data;
            EditText(stringVal, arrayIndex, arrayIndexBuf);

        }
        //ImGui::Text("%s - Text attribute not supported yet", *(char**)data);
        break;

        case AttribTypes::StringKey:
        case AttribTypes::StringKey_:
        {
            EditStringKey(data, arrayIndex, arrayIndexBuf);
        }
        break;

        case GCollectionKey: {
            DWORD key = *(DWORD*)data;

            if (!GCollectionKeysInitialized) InitGCollectionKeys();

            char stringBuf[100];
            if (GCollectionKeys[key]) snprintf(stringBuf, sizeof(stringBuf), GCollectionKeys[key]);
            else snprintf(stringBuf, sizeof(stringBuf), "0x%08x", key);


            float width = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(2* width / 3);
            if (ImGui::InputText("", stringBuf, sizeof(stringBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (stringBuf[0] == '0' && stringBuf[1] == 'x') *(DWORD*)data = strtoul(stringBuf, NULL, 16);
                else *(DWORD*)data = getVltHash(stringBuf);
                
                auto arr = new char[strlen(stringBuf) + 1];
                strncpy_s(arr, strlen(stringBuf) + 1, stringBuf, sizeof(stringBuf));
                GCollectionKeys[*(DWORD*)data] = arr;

                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << stringBuf << "\n";
            }
            ImGui::SetNextItemWidth(width / 3);
            ImGui::SameLine();
            ImGui::PushID(1);
            if (ImGui::Button("Go to collection")) {
                *nextCollection = Attrib::FindCollection(getVltHash("gameplay"), key, 0);
            }
            ImGui::PopID();

        } break;

        case AttribTypes::type_bStringHash:
        {
            EditBStringHash(data, arrayIndex, arrayIndexBuf);
        }
        break;






        case AttribTypes::FloatColour:
        {
            EditColourF4(data, arrayIndex, arrayIndexBuf);
        }
        break;

        case AttribTypes::Vector4:
        {
            if (currentCollection->mClass->mKey == getVltHash("zone_visual_style")) {
                EditColourF4(data, arrayIndex, arrayIndexBuf, true);
                break;
            }
			EditFloatN(data, arrayIndex, arrayIndexBuf, 4, "X", "Y", "Z", "W");
            /*int result = ImGui::InputScalarN("", ImGuiDataType_Float, (float*)data, 4, (void*)(&one_f), (void*)(&ten_f), "%.3f", ImGuiInputTextFlags_AutoSelectAll);
            if (result & (1 << 0)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "X " << ((float*)data)[0] << "\n";
            }
            if (result & (1 << 1)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Y " << ((float*)data)[1] << "\n";
            }
            if (result & (1 << 2)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Z " << ((float*)data)[2] << "\n";
            }
            if (result & (1 << 3)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "W " << ((float*)data)[3] << "\n";
            } */
        }
        break;

        case AttribTypes::Vector3:
        {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 3, "X", "Y", "Z");
            /*int result = ImGui::InputScalarN("", ImGuiDataType_Float, (float*)data, 3, (void*)(&one_f), (void*)(&ten_f), "%.3f", ImGuiInputTextFlags_AutoSelectAll);
            if (result & (1 << 0)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "X " << ((float*)data)[0] << "\n";
            }
            if (result & (1 << 1)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Y " << ((float*)data)[1] << "\n";
            }
            if (result & (1 << 2)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Z " << ((float*)data)[2] << "\n";
            }*/
        }
        break;

        case AttribTypes::Vector2:
        {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "X", "Y");
/*            int result = ImGui::InputScalarN("", ImGuiDataType_Float, (float*)data, 2, (void*)(&one_f), (void*)(&ten_f), "%.3f", ImGuiInputTextFlags_AutoSelectAll);
            if (result & (1 << 0)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "X " << ((float*)data)[0] << "\n";
            }
            if (result & (1 << 1)) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "Y " << ((float*)data)[1] << "\n";
            }*/
        }
        break;

        case AttribTypes::Matrix:
        {
            for (int i = 0; i < 4; i++) {
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(i);
                int result = ImGui::InputScalarN("", ImGuiDataType_Float, ((float*)data) + 4 * i, 4, (void*)(&one_f), (void*)(&ten_f), "%.3f", ImGuiInputTextFlags_AutoSelectAll);
                for (int j = 0; j < 4; j++)
                    if (result & (1 << j)) {
                        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                        modscript << "M[" << (i + 1) << "," << (j + 1) << "] " << ((float*)data)[0] << "\n";
                    }
                ImGui::PopID();
            }
        }
        break;


        case AttribTypes::RefSpec:
        {
            //class key
            //collection key
            //collection ptr
            EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "Class", "Collection");
        }
        break;

        case AttribTypes::EffectLinkageRecord:
        {
            //3 RefSpec, Surface/Effects/Audio
            //4 floats, MinSpeed/MaxSpeed, SFXMinSpeed, SFXMaxSpeed
            ImGui::PushID(1);
            EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "SurfaceClassKey", "SurfaceCollectionKey");
            ImGui::PopID();
            ImGui::PushID(2);
            EditRefSpec(data + 12, arrayIndex, arrayIndexBuf, nextCollection, "EffectClassKey", "EffectCollectionKey");
            ImGui::PopID();
            ImGui::PushID(3);
            EditRefSpec(data + 24, arrayIndex, arrayIndexBuf, nextCollection, "AudioClassKey", "AudioCollectionKey");
            ImGui::PopID();
            ImGui::PushID(4);
            EditFloatN(data + 36, arrayIndex, arrayIndexBuf, 2, "MinSpeed", "MaxSpeed");
            ImGui::PopID();
            ImGui::PushID(5);
            EditFloatN(data + 44, arrayIndex, arrayIndexBuf, 2, "SFXMinSpeed", "SFXMaxSpeed");
            ImGui::PopID();

        }
        break;

        case TireEffectRecord: {
            EditTireCondition(data, arrayIndex, arrayIndexBuf, "TireCondition");
            EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "EmitterClass", "EmitterCollection");
            EditRefSpec(data + 16, arrayIndex, arrayIndexBuf, nextCollection, "EmitterLowLodClass", "EmitterLowLodCollection");
            EditFloatN(data + 28, arrayIndex, arrayIndexBuf, 2, "MinSpeed", "MaxSpeed");

        } break;

        case TireTimeEffectRecord: {
            EditTireCondition(data, arrayIndex, arrayIndexBuf, "TireCondition");
            EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "EmitterClass", "EmitterCollection");
            EditRefSpec(data + 16, arrayIndex, arrayIndexBuf, nextCollection, "EmitterLowLodClass", "EmitterLowLodCollection");
            EditFloatN(data + 28, arrayIndex, arrayIndexBuf, 2, "MinTime", "MaxTime");
        } break;

        case RoadNoiseRecord: {
            ImGui::PushID(1);
            EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "Frequency", "Amplitude");
            ImGui::PopID();
            EditFloatN(data + 8, arrayIndex, arrayIndexBuf, 2, "MinSpeed", "MaxSpeed");
        } break;

        case AttribTypes::CarBodyMotion:
        {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 3, "DegPerG", "MaxG", "DegPerSec");
        }
        break;

        case AttribTypes::AxlePair:
        {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "Front", "Rear");
        }
        break;

        //incomplete, no array resize
        case AttribTypes::Curve:
        {
            auto curve = (struct Curve*)data;
            ImGui::PushID(1);
            EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "MinX", "MaxX");
            ImGui::PopID();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushID(2);
            EditFloatN(data + 8, arrayIndex, arrayIndexBuf, 2, "MinY", "MaxY");
            ImGui::PopID();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushID(3);
            EditFloatN(data + 16, arrayIndex, arrayIndexBuf, 1, "GraphScale");
            ImGui::PopID();
            EditGraphData(curve, arrayIndex, arrayIndexBuf);
            PreviewCurve(curve);
        }
        break;

        case AttribTypes::AxlePairCurve:
        {
            auto curve = (struct Curve*)data;

            //this is stupid, doing it that way only because of vlted, all my homies hate vlted
            if (ImGui::TreeNodeEx("Front", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushID(1);
                EditFloatN(&curve->xMin, arrayIndex, arrayIndexBuf, 2, "Front_MinX", "Front_MaxX");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(2);
                EditFloatN(&curve->yMin, arrayIndex, arrayIndexBuf, 2, "Front_MinY", "Front_MaxY");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(3);
                EditFloatN(&curve->graphScale, arrayIndex, arrayIndexBuf, 1, "Front_GraphScale");
                ImGui::PopID();
                EditGraphData(curve, arrayIndex, arrayIndexBuf, "Front_");
                PreviewCurve(curve);
                ImGui::TreePop();
            }

            curve = &curve[1]; //switch to rear

            if (ImGui::TreeNodeEx("Rear", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushID(1);
                EditFloatN(&curve->xMin, arrayIndex, arrayIndexBuf, 2, "Rear_MinX", "Rear_MaxX");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(2);
                EditFloatN(&curve->yMin, arrayIndex, arrayIndexBuf, 2, "Rear_MinY", "Rear_MaxY");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(3);
                EditFloatN(&curve->graphScale , arrayIndex, arrayIndexBuf, 1, "Rear_GraphScale");
                ImGui::PopID();
                EditGraphData(curve, arrayIndex, arrayIndexBuf, "Rear_");
                PreviewCurve(curve);




                ImGui::TreePop();
            }
        }
        break;


        case AttribTypes::GlueCurve:
        {
            auto curve = (struct Curve*)data;

            //this is stupid, doing it that way only because of vlted, all my homies hate vlted
            if (ImGui::TreeNodeEx("Easy", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushID(1);
                EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "Easy_MinX", "Easy_MaxX");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(2);
                EditFloatN(data + 8, arrayIndex, arrayIndexBuf, 2, "Easy_MinY", "Easy_MaxY");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(3);
                EditFloatN(data + 16, arrayIndex, arrayIndexBuf, 1, "Easy_GraphScale");
                ImGui::PopID();
                EditGraphData(curve, arrayIndex, arrayIndexBuf, "Easy_");
                PreviewCurve(curve);
                ImGui::TreePop();
            }

            curve = &curve[1]; //switch to Hard

            if (ImGui::TreeNodeEx("Hard", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushID(1);
                EditFloatN(&curve->xMin, arrayIndex, arrayIndexBuf, 2, "Hard_MinX", "Hard_MaxX");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(2);
                EditFloatN(&curve->yMin, arrayIndex, arrayIndexBuf, 2, "Hard_MinY", "Hard_MaxY");
                ImGui::PopID();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(3);
                EditFloatN(&curve->graphScale, arrayIndex, arrayIndexBuf, 1, "Hard_GraphScale");
                ImGui::PopID();
                EditGraphData(curve, arrayIndex, arrayIndexBuf, "Hard_");
                PreviewCurve(curve);
                ImGui::TreePop();
            }
        }
        break;

        case AttribTypes::FEPartData:
        {
            auto partData = (struct FEPartData*)data;
            EditBStringHash((int)&partData->HAL_ID, arrayIndex, arrayIndexBuf, "HAL_ID");
            EditBStringHash((int)&partData->CF_HAL_ID, arrayIndex, arrayIndexBuf, "CF_HAL_ID");
            EditInt((int)&partData->price, arrayIndex, arrayIndexBuf, "Price");
            EditByte((int)&partData->showroom_unlock, arrayIndex, arrayIndexBuf, "ShowroomUnlock");
            EditInt((int)&partData->tier1_price, arrayIndex, arrayIndexBuf, "Tier1Price");
            EditInt((int)&partData->tier2_price, arrayIndex, arrayIndexBuf, "Tier2Price");
            EditInt((int)&partData->tier3_price, arrayIndex, arrayIndexBuf, "Tier3Price");
            EditInt((int)&partData->tier4_price, arrayIndex, arrayIndexBuf, "Tier4Price");
            EditByte((int)&partData->tier, arrayIndex, arrayIndexBuf, "Tier");
            EditByte((int)&partData->tier_1_showroom_unlock, arrayIndex, arrayIndexBuf, "Tier1ShowroomUnlock");
            EditByte((int)&partData->tier_2_showroom_unlock, arrayIndex, arrayIndexBuf, "Tier2ShowroomUnlock");
            EditByte((int)&partData->tier_3_showroom_unlock, arrayIndex, arrayIndexBuf, "Tier3ShowroomUnlock");
            EditByte((int)&partData->tier_4_showroom_unlock, arrayIndex, arrayIndexBuf, "Tier4ShowroomUnlock");
            EditBStringHash((int)&partData->BrandHALId, arrayIndex, arrayIndexBuf, "BrandHALId");
            EditBStringHash((int)&partData->LogoTextureId, arrayIndex, arrayIndexBuf, "LogoTextureId");
            EditBStringHash((int)&partData->DetailHash, arrayIndex, arrayIndexBuf, "DetailHash");
            if (partData->PartDetails != NULL && ImGui::TreeNode("PartDetails")) {
                EditBStringHash((int)&partData->PartDetails->Logo, arrayIndex, arrayIndexBuf, "Logo", false); //not in vlt script
                EditBStringHash((int)&partData->PartDetails->Logo, arrayIndex, arrayIndexBuf, "Name", false);
                ImGui::TreePop();
            }
            if (partData->OfferID != NULL) ImGui::Text((const char*)partData->OfferID);
            EditByte((int)&partData->IsOnlineLockable, arrayIndex, arrayIndexBuf, "IsOnlineLockable");
        }
        break;

        case AttribTypes::CCarSlotEntry:
        {
            DWORD dynamicArray = ((DWORD*)data)[0];
            DWORD arraySize = ((DWORD*)data)[1];
            char** slotName = &((char**)data)[2];

            EditText(slotName, arrayIndex, arrayIndexBuf, "Type"); //no vlted that's NOT a type...
            if (ImGui::TreeNode("Parts")) {
                for (int i = 0; i < arraySize; i++) {
                    ImGui::PushID(i);
                    EditRefSpec(dynamicArray + 12 * i, arrayIndex, arrayIndexBuf, nextCollection, "Group", "Name", false); //that's NOT group and name either
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }

        }
        break;

        case AttribTypes::CCarKitSlotEntry:
        {
            auto totalWidth = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("SlotName"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);

            EditText((char**)(data + 12), arrayIndex, arrayIndexBuf, "Type"); //no vlted that's NOT a type...
            if (ImGui::TreeNodeEx("Part", ImGuiTreeNodeFlags_DefaultOpen)) {
                EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "Group", "Name", false); //that's NOT group and name either
                ImGui::TreePop();
            }
        }
        break;

        case CCarDamageLevels:
        {
            ImGui::PushID(1);
            EditFloatN(data, arrayIndex, arrayIndexBuf, 4, "Speed0", "Speed1", "Speed2", "Influence");
            ImGui::PopID();
            ImGui::PushID(2);
            ImGui::SetNextItemWidth(-FLT_MIN);
            EditFloatN(data + 16, arrayIndex, arrayIndexBuf, 4, "LightCrack", "LightShatter", "WindowCrack", "WindowShatter");
            ImGui::PopID();
        }
        break;

        case CCarDamageThreshold:
        {
            ImGui::PushID(1);
            EditFloatN(data, arrayIndex, arrayIndexBuf, 4, "Threshold0", "Threshold1", "Threshold2", "Threshold3");
            ImGui::PopID();
            ImGui::PushID(2);
            ImGui::SetNextItemWidth(-FLT_MIN);
            EditFloatN(data + 16, arrayIndex, arrayIndexBuf, 4, "DeltaThreshold0", "DeltaThreshold1", "DeltaThreshold2", "DeltaThreshold3");
            ImGui::PopID();
        }
        break;

        case CCarDamageEntry:
        {
            auto totalWidth = ImGui::CalcItemWidth();
            //EditInt(data, arrayIndex, arrayIndexBuf, "PartID");
            EditCAR_PART_ID(data, arrayIndex, arrayIndexBuf, "PartID");
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("AttachPart"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditText((char**)(data + 4), arrayIndex, arrayIndexBuf, "AttachPart");
            ImGui::PushID(1);
            EditRefSpec(data + 8, arrayIndex, arrayIndexBuf, nextCollection, "MaterialClassKey", "MaterialCollectionKey");
            ImGui::PopID();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("SmackableCollisionName"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditText((char**)(data + 20), arrayIndex, arrayIndexBuf, "SmackableCollisionName");
            ImGui::PushID(2);
            EditRefSpec(data + 24, arrayIndex, arrayIndexBuf, nextCollection, "SmackableCollisionAttributeClassKey", "SmackableCollisionAttributeCollectionKey");
            ImGui::PopID();
        }
        break;

        case CCarDamageMarkerEntry:
        {
            auto totalWidth = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("MarkerName"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditText((char**)(data + 0), arrayIndex, arrayIndexBuf, "Name"); //vlted name, proper name
            //EditInt(data + 4, arrayIndex, arrayIndexBuf, "PartID");
            EditCAR_PART_ID(data + 4, arrayIndex, arrayIndexBuf, "PartID");
            //EditInt(data+8, arrayIndex, arrayIndexBuf, "SlotID");
            EditCAR_SLOT_ID(data + 8, arrayIndex, arrayIndexBuf, "SlotID");
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("AttachPart"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditText((char**)(data + 12), arrayIndex, arrayIndexBuf, "4"); //vlted name, proper name
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("SmackableCollisionName"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditText((char**)(data + 16), arrayIndex, arrayIndexBuf, "5"); //vlted name, proper name
            ImGui::PushID(1);
            EditRefSpec(data + 8, arrayIndex, arrayIndexBuf, nextCollection, "6", "7", false);
            ImGui::PopID();
        }
        break;

        case PresetRidePart:
        {
            //EditInt(data, arrayIndex, arrayIndexBuf, "SlotID");
            EditCAR_SLOT_ID(data, arrayIndex, arrayIndexBuf, "SlotID");
            EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "PartClassKey", "PartCollectionKey");
            EditInt(data + 16, arrayIndex, arrayIndexBuf, "KitNumber");
        }
        break;

        //TODO bit wonky
        case PresetRideAutosculpt:
        {
            //EditInt(data, arrayIndex, arrayIndexBuf, "RegionID");
            EditAutosculptRegionID(data, arrayIndex, arrayIndexBuf, "RegionID");
            char buf[10];
            for (int i = 0; i < 11; i++) {
                snprintf(buf, sizeof(buf), "Zone%d", i + 1);
                ImGui::Text("Zone %d", i); ImGui::SameLine();
                EditFloat(data + 4 + 4 * i, arrayIndex, arrayIndexBuf, buf, false);
            }
        }
        break;

        case PresetRidePaint:
        {
            auto totalWidth = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("SlotID"); ImGui::SameLine(totalWidth / 2);
            //ImGui::SetNextItemWidth(totalWidth / 2);
            //EditInt(data, arrayIndex, arrayIndexBuf, "1", false);
            EditePaintSlot(data, arrayIndex, arrayIndexBuf, "1", false);

            EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "2", "3", false);

            PreviewSwatch(data+16);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Swatch"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditByte(data + 16, arrayIndex, arrayIndexBuf, "5", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Saturation"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditFloatHex(data + 20, arrayIndex, arrayIndexBuf, "6", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Variance"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditFloat(data + 24, arrayIndex, arrayIndexBuf, "7", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Flags"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            //EditInt(data + 28, arrayIndex, arrayIndexBuf, "8", false);
            EditePartMaterialFlags(data + 28, arrayIndex, arrayIndexBuf, "8", false);

        }
        break;

        case PresetRideRandomPaint: {
            auto totalWidth = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("SlotID"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditePaintSlot(data, arrayIndex, arrayIndexBuf, "SlotID", false);
            //EditInt(data, arrayIndex, arrayIndexBuf, "SlotID", false);

            EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "GroupClassKey", "GroupCollectionKey", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Swatch"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditByte(data + 16, arrayIndex, arrayIndexBuf, "5", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Saturation"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditFloat(data + 20, arrayIndex, arrayIndexBuf, "6", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Variance"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditFloat(data + 24, arrayIndex, arrayIndexBuf, "7", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Flags"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            //EditInt(data + 28, arrayIndex, arrayIndexBuf, "8", false);
            EditePartMaterialFlags(data + 28, arrayIndex, arrayIndexBuf, "8", false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Chance"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditFloat(data + 32, arrayIndex, arrayIndexBuf, "Chance", false);
        }
                                  break;

                                  //VinylsAttrib::VinylLayer
        case VinylLayer_: {
            bool edited = false; //vlted doesn't read these properly so need to do things differently here

            auto totalWidth = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("PartNameHash"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditBStringHash(data, arrayIndex, arrayIndexBuf, NULL, false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Mirrored"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::Checkbox("##mirror", (bool*)(data + 4))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("TranslationX"); ImGui::SameLine(totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##tx", ImGuiDataType_S16, (INT16*)(data + 6))) edited = true;
            ImGui::SameLine(2 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("TranslationY"); ImGui::SameLine(3 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##ty", ImGuiDataType_S16, (INT16*)(data + 8))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Rotation"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::InputScalar("##rot", ImGuiDataType_S8, (INT8*)(data + 10))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("ScaleX"); ImGui::SameLine(totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##sx", ImGuiDataType_S8, (INT8*)(data + 11))) edited = true;
            ImGui::SameLine(2 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("ScaleY"); ImGui::SameLine(3 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##sy", ImGuiDataType_S8, (INT8*)(data + 12))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("ProportionalScale"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::Checkbox("##proportional", (bool*)(data + 13))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Shear"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::InputScalar("##shear", ImGuiDataType_S8, (INT8*)(data + 14))) edited = true;

            EditColorPacked("Color0", data + 16, totalWidth, edited);
            EditColorPacked("Color1", data + 19, totalWidth, edited);
            EditColorPacked("Color2", data + 22, totalWidth, edited);
            EditColorPacked("Color3", data + 25, totalWidth, edited);

            if (edited)  for (int i = 0; i < 7; i++) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << (i + 1) << " 0x";
                modscript << ((DWORD**)data)[i] << "\n";
            }

        } break;

        case DecalLayer: //vlted is unable to edit this at all
        case DecalLayer_: {
            bool edited = false;

            auto totalWidth = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("PartNameHash"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            EditBStringHash(data, arrayIndex, arrayIndexBuf, NULL, false);

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Mirrored"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::Checkbox("##mirror", (bool*)(data + 4))) edited = true;

            //VinylsAttrib::VinylTransform
            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("TranslationX"); ImGui::SameLine(totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##tx", ImGuiDataType_S16, (INT16*)(data + 6))) edited = true;
            ImGui::SameLine(2 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("TranslationY"); ImGui::SameLine(3 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##ty", ImGuiDataType_S16, (INT16*)(data + 8))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Rotation"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::InputScalar("##rot", ImGuiDataType_S8, (INT8*)(data + 10))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("ScaleX"); ImGui::SameLine(totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##sx", ImGuiDataType_S8, (INT8*)(data + 11))) edited = true;
            ImGui::SameLine(2 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            ImGui::Text("ScaleY"); ImGui::SameLine(3 * totalWidth / 4);
            ImGui::SetNextItemWidth(totalWidth / 4);
            if (ImGui::InputScalar("##sy", ImGuiDataType_S8, (INT8*)(data + 12))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("ProportionalScale"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::Checkbox("##proportional", (bool*)(data + 13))) edited = true;

            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Shear"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::InputScalar("##shear", ImGuiDataType_S8, (INT8*)(data + 14))) edited = true;

            //VinylsAttrib::VinylColor
            EditColorPacked("Color", data + 16, totalWidth, edited);

            //probably does not work!
            //if (edited)  for (int i = 0; i < 5; i++) {
            //    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
            //    modscript << (i + 1) << " 0x";
            //    modscript << ((DWORD**)data)[i] << "\n";
            //}

        } break;

        case CarPaintSwatch: {
            struct CarPaintSwatch* cps = (struct CarPaintSwatch*)data;
                
            auto totalWidth = ImGui::CalcItemWidth();

            float rgb[3]; //BGR
            rgb[0] = 1.0f / 255.0f * ((cps->rgb >> 16) & 0xFF);
            rgb[1] = 1.0f / 255.0f * ((cps->rgb >> 8) & 0xFF);
            rgb[2] = 1.0f / 255.0f * ((cps->rgb >> 0) & 0xFF);


            ImGui::SetNextItemWidth(totalWidth / 2);
            ImGui::Text("Color"); ImGui::SameLine(totalWidth / 2);
            ImGui::SetNextItemWidth(totalWidth / 2);
            if (ImGui::ColorEdit3("##color", rgb)) {
                cps->rgb = (
                    0xFF << 24
                    | ((UINT)IM_F32_TO_INT8_SAT(rgb[0])) << 16
                    | ((UINT)IM_F32_TO_INT8_SAT(rgb[1])) << 8
                    | ((UINT)IM_F32_TO_INT8_SAT(rgb[2])) << 0
                    );

                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "RGB 0x";
                modscript << (DWORD*)cps->rgb << "\n";
            }

            EnumCombo((int)& cps->material_a, "ePaintMaterialWhite\0ePaintMaterialSilver\0ePaintMaterialBlack\0ePaintMaterialRed\0ePaintMaterialViolet\0ePaintMaterialIndigo\0ePaintMaterialBlue\0ePaintMaterialGreen\0ePaintMaterialYellow\0ePaintMaterialOrange",
                arrayIndex, arrayIndexBuf, "MaterialA");
            EnumCombo((int)&cps->material_b, "ePaintMaterialWhite\0ePaintMaterialSilver\0ePaintMaterialBlack\0ePaintMaterialRed\0ePaintMaterialViolet\0ePaintMaterialIndigo\0ePaintMaterialBlue\0ePaintMaterialGreen\0ePaintMaterialYellow\0ePaintMaterialOrange",
                arrayIndex, arrayIndexBuf, "MaterialB");
            EditFloat((int)&cps->blend, arrayIndex, arrayIndexBuf, "Blend");
            EnumFlags((int)&cps->speechColour, "Invalid_Type_car_color\0Type_car_color_Red\0Type_car_color_Black\0Type_car_color_White\0Type_car_color_Blue\0Type_car_color_Green\0Type_car_color_Orange\0Type_car_color_Silver\0Type_car_color_Gold\0Type_car_color_Purple\0Type_car_color_Brown\0Type_car_color_Yellow\0Type_car_color_Pink\0Type_car_color_Beige\0Type_car_color_No_colour",
                arrayIndex, arrayIndexBuf, "SpeechColour");

        } break;

        case UpgradeSpecs: {
            EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "Class", "Collection");
            EditByte(data + 12, arrayIndex, arrayIndexBuf, "Level");
        } break;
        
        case ParticleTextureRecord: {
			EditBStringHash(data, arrayIndex, arrayIndexBuf, "TextureType");
            EditInt(data + 4, arrayIndex, arrayIndexBuf, "Index");
        }break;

        case ParticleAnimationInfo: {
			EnumCombo(data, "ANIMATE_PARTICLE_NONE\0ANIMATE_PARTICLE_2x2\0ANIMATE_PARTICLE_4x4\0ANIMATE_PARTICLE_8x8\0ANIMATE_PARTICLE_16x16", arrayIndex, arrayIndexBuf, "AnimType");
            EditByte(data + 4, arrayIndex, arrayIndexBuf, "FPS");
            EditByte(data + 5, arrayIndex, arrayIndexBuf, "RandomStartFrame");
        }break;

        case EmittedGeometryRecord: {
            auto totalWidth = ImGui::CalcItemWidth();
            CorrectedText(totalWidth, "Enum");
			EditBStringHash(data, arrayIndex, arrayIndexBuf, "1", true, false);

            CorrectedText(totalWidth, "Index");
            EditInt(data + 4, arrayIndex, arrayIndexBuf, "2", false);
        }break;

        case FEVinylGroupInfo: {
            auto totalWidth = ImGui::CalcItemWidth();

            CorrectedText(totalWidth, "group_number");
            EditByte(data, arrayIndex, arrayIndexBuf, "Index", false);

            CorrectedText(totalWidth, "HAL_ID");
			EditBStringHash(data + 4, arrayIndex, arrayIndexBuf, "1", true, false);

            bool edited = false; //vlted doesn't read these properly so need to do things differently here
            CorrectedText(totalWidth, "sort_order");
            if (ImGui::InputScalar("##sort_order", ImGuiDataType_S8, (INT8*)(data + 8))) edited = true;
            CorrectedText(totalWidth, "combine_group");
            if (ImGui::Checkbox("##combine_group", (bool*)(data + 9))) edited = true;
            CorrectedText(totalWidth, "combine_group_number");
            if (ImGui::InputScalar("##combine_group_number", ImGuiDataType_S8, (INT8*)(data + 10))) edited = true;
            CorrectedText(totalWidth, "black_and_white_only");
            if (ImGui::Checkbox("##black_and_white_only", (bool*)(data + 11))) edited = true;
            CorrectedText(totalWidth, "not_user_selectable");
            if (ImGui::Checkbox("##not_user_selectable", (bool*)(data + 12))) edited = true;
            CorrectedText(totalWidth, "non_deformable");
            if (ImGui::Checkbox("##non_deformable", (bool*)(data + 13))) edited = true;
            CorrectedText(totalWidth, "collectors_edition");
            if (ImGui::Checkbox("##collectors_edition", (bool*)(data + 14))) edited = true;
            CorrectedText(totalWidth, "showroom_unlock");
            if (ImGui::InputScalar("##showroom_unlock", ImGuiDataType_S8, (INT8*)(data + 15))) edited = true;

            if (edited)  for (int i = 2; i < 4; i++) {
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << (i + 1) << " 0x";
                modscript << ((DWORD**)data)[i] << "\n";
            }

            CorrectedText(totalWidth, "price");
            EditInt(data, arrayIndex, arrayIndexBuf, "5", false);
        } break;

        case FEPartCamera: {
			EditText((char**)(data), arrayIndex, arrayIndexBuf, "SlotName");
            EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "CameraClassKey", "CameraCollectionKey");
            EditRefSpec(data + 16, arrayIndex, arrayIndexBuf, nextCollection, "Camera_4_3ClassKey", "Camera_4_3CollectionKey");
        } break;

        case AtttributeRefSpec: {
            DWORD* classKey = &((DWORD*)data)[0];
            DWORD* definitionKey = &((DWORD*)data)[1];

            ImGui::SetNextItemWidth(-FLT_MIN);
            float width = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(width / 2);
            ImGui::Text("ClassKey"); ImGui::SameLine(width / 2);
            ImGui::SetNextItemWidth(width / 2);
            ImGui::Text("DefinitionKey");
            static char classKeyBuf[64];
            static char collectionKeyBuf[100];
            //if (lastAttribute != def->mKey) {
            if (AttribClasses[*classKey] != NULL) snprintf(classKeyBuf, sizeof(classKeyBuf), "%s", AttribClasses[*classKey]);
            else snprintf(classKeyBuf, sizeof(classKeyBuf), "0x%08x", *classKey);
            if (AttribFields[*definitionKey]) snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "%s", AttribFields[*definitionKey]);
            else snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "0x%08x", *definitionKey);
            //lastAttribute = def->mKey;
            //}

            ImGui::PushID(1);
            ImGui::SetNextItemWidth(width / 3);
            if (ImGui::InputText("", classKeyBuf, sizeof(classKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (classKeyBuf[0] == '0' && classKeyBuf[1] == 'x') *classKey = strtoul(classKeyBuf, NULL, 16);
                else *classKey = getVltHash(classKeyBuf);
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "ClassKey" << " " << classKeyBuf << "\n";
            }
            ImGui::PopID();
            ImGui::PushID(2);
            //ImGui::SameLine(width/3);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(width / 3);
            if (ImGui::InputText("", collectionKeyBuf, sizeof(collectionKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (collectionKeyBuf[0] == '0' && collectionKeyBuf[1] == 'x') *definitionKey = strtoul(collectionKeyBuf, NULL, 16);
                else *definitionKey = getVltHash(collectionKeyBuf);
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "DefinitionKey" << " " << collectionKeyBuf << "\n";
            }
            ImGui::PopID();
        } break;

        case AirSupport: {
            EnumCombo(data, "HI_PATROL\0PURSUIT\0SKID_HIT\0SPIKE_DROP", 
                arrayIndex, arrayIndexBuf, "HeliStrategy");
            EditInt(data + 4, arrayIndex, arrayIndexBuf, "Chance");
            EditFloat(data + 8, arrayIndex, arrayIndexBuf, "Duration");
        } break;

        case HeavySupport: {
            EnumCombo(data, "NONE\0E_BRAKE\0COORDINATED_E_BRAKE\0RAM\0HEAVY_ROADBLOCK",
                arrayIndex, arrayIndexBuf, "HeavyStrategy");
            EditInt(data + 4, arrayIndex, arrayIndexBuf, "Chance");
            EditFloat(data + 8, arrayIndex, arrayIndexBuf, "Duration");
            EditInt(data + 12, arrayIndex, arrayIndexBuf, "ChanceBigSUV");
        } break;

        case LeaderSupport: {
            EnumCombo(data, "NONE\0E_BRAKE\0COORDINATED_E_BRAKE\0RAM\0HEAVY_ROADBLOCK\0CROSS_FOLLOW\0CROSS_BRAKE\0CROSS_PLUS_V_BLOCK", //first ones used as padding...
                arrayIndex, arrayIndexBuf, "LeaderStrategy");
            EditInt(data + 4, arrayIndex, arrayIndexBuf, "Chance");
            EditFloat(data + 8, arrayIndex, arrayIndexBuf, "Duration");
            EditInt(data + 12, arrayIndex, arrayIndexBuf, "PriorityChance");
            EditFloat(data + 16, arrayIndex, arrayIndexBuf, "PriorityTime");
        } break;

        case FEHintsData: {
            EditBStringHash(data, arrayIndex, arrayIndexBuf, "Subject");
            EditBStringHash(data+4, arrayIndex, arrayIndexBuf, "Text");
            EditText((char**)(data + 8), arrayIndex, arrayIndexBuf, "Picture");
        } break;

        case AICollisionReactionRecord: {
            EditInt(data, arrayIndex, arrayIndexBuf, "Goal");
			EditRefSpec(data + 4, arrayIndex, arrayIndexBuf, nextCollection, "ClassKey", "DefinitionKey", false); //thats not a fucking definition key
        } break;

        case RwacSampleBankAsset: {
            auto totalWidth = ImGui::CalcItemWidth();

            CorrectedText(totalWidth, "Bank");
            EditInt(data, arrayIndex, arrayIndexBuf, "1", false);
            CorrectedText(totalWidth, "Asset");
            EditInt(data+4, arrayIndex, arrayIndexBuf, "2", false);
        } break;

        case FETuningSlider: {
            EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "Group", "Name", false);
            auto totalWidth = ImGui::CalcItemWidth();

            CorrectedText(totalWidth, "TitleHash");
            EditBStringHash(data + 12, arrayIndex, arrayIndexBuf, "1", true, false);
            CorrectedText(totalWidth, "LeftHash");
            EditBStringHash(data + 16, arrayIndex, arrayIndexBuf, "2", true, false);
            CorrectedText(totalWidth, "RightHash");
            EditBStringHash(data + 20, arrayIndex, arrayIndexBuf, "3", true, false);
            CorrectedText(totalWidth, "HelpHash");
            EditBStringHash(data + 24, arrayIndex, arrayIndexBuf, "4", true, false);
        } break;

        case RoadblockSetup: {
            EditFloat(data, arrayIndex, arrayIndexBuf, "MinimumWidthRequired");
            EditInt(data + 4, arrayIndex, arrayIndexBuf, "RequiredVehicles");
            EditFloat(data + 8, arrayIndex, arrayIndexBuf, "MinimumThreatLevel");
			EditFloat(data + 12, arrayIndex, arrayIndexBuf, "MaximumThreatLevel");
            for (int i = 0; i < 6; i++) {
                if (ImGui::TreeNode(&i, "Roadblock Element #%d", i + 1)) {
                    char buf[32];
                    sprintf_s(buf, sizeof(buf), "ElementType%d", i + 1);
					EnumCombo(data + 16 + i * 16, "kNone\0kCar\0kBarrier\0kSpikeStrip",
						arrayIndex, arrayIndexBuf, buf);
                    sprintf_s(buf, sizeof(buf), "OffsetX%d", i + 1);
                    EditFloat(data + 20 + i * 16, arrayIndex, arrayIndexBuf, buf);
                    sprintf_s(buf, sizeof(buf), "OffsetZ%d", i + 1);
                    EditFloat(data + 24 + i * 16, arrayIndex, arrayIndexBuf, buf);
                    sprintf_s(buf, sizeof(buf), "Angle%d", i + 1);
                    EditFloat(data + 28 + i * 16, arrayIndex, arrayIndexBuf, buf);

                    ImGui::TreePop();
                }
            }
        } break;
                         
        case PhysicsUpgrade: {
            DWORD* classKey = &((DWORD*)data)[0];
            DWORD* definitionKey = &((DWORD*)data)[1];

            ImGui::SetNextItemWidth(-FLT_MIN);
            float width = ImGui::CalcItemWidth();
            ImGui::SetNextItemWidth(width / 3);
            ImGui::Text("ClassKey"); ImGui::SameLine(width / 3);
            ImGui::SetNextItemWidth(width / 3);
            ImGui::Text("DefinitionKey");
            static char classKeyBuf[64];
            static char collectionKeyBuf[100];
            //if (lastAttribute != def->mKey) {
            if (AttribClasses[*classKey] != NULL) snprintf(classKeyBuf, sizeof(classKeyBuf), "%s", AttribClasses[*classKey]);
            else snprintf(classKeyBuf, sizeof(classKeyBuf), "0x%08x", *classKey);
            if (AttribFields[*definitionKey]) snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "%s", AttribFields[*definitionKey]);
            else snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "0x%08x", *definitionKey);
            //lastAttribute = def->mKey;
            //}

            ImGui::PushID(1);
            ImGui::SetNextItemWidth(width / 3);
            if (ImGui::InputText("", classKeyBuf, sizeof(classKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (classKeyBuf[0] == '0' && classKeyBuf[1] == 'x') *classKey = strtoul(classKeyBuf, NULL, 16);
                else *classKey = getVltHash(classKeyBuf);
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "ClassKey" << " " << classKeyBuf << "\n";
            }
            ImGui::PopID();
            ImGui::PushID(2);
            //ImGui::SameLine(width/3);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(width / 3);
            if (ImGui::InputText("", collectionKeyBuf, sizeof(collectionKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (collectionKeyBuf[0] == '0' && collectionKeyBuf[1] == 'x') *definitionKey = strtoul(collectionKeyBuf, NULL, 16);
                else *definitionKey = getVltHash(collectionKeyBuf);
                WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                modscript << "DefinitionKey" << " " << collectionKeyBuf << "\n";
            }
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::SetNextItemWidth(width / 3);
            EditFloat(data + 8, arrayIndex, arrayIndexBuf, "BlendingPower");
        } break;

        case TrafficPatternRecord: {
            auto totalWidth = ImGui::CalcItemWidth();
            EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "Group", "Name", false);
            CorrectedText(totalWidth, "Rate");
            EditFloat(data + 12, arrayIndex, arrayIndexBuf, "Density", false);
            CorrectedText(totalWidth, "MaxInstances");
            EditInt(data + 16, arrayIndex, arrayIndexBuf, "5", false);
            CorrectedText(totalWidth, "Percent");
            EditInt(data + 20, arrayIndex, arrayIndexBuf, "6", false);
        } break;

        case PhysicsTuningDescription: {
            auto totalWidth = ImGui::CalcItemWidth();
            EditRefSpec(data, arrayIndex, arrayIndexBuf, nextCollection, "1", "2", false);
            CorrectedText(totalWidth, "Increase");
            EditByte(data + 12, arrayIndex, arrayIndexBuf, "4", false);
        } break;

        case CollisionReactionRecord: {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 4, "Elasticity", "RollHeight", "WeightBias", "MassScale");
            //EditFloat(data + 4, arrayIndex, arrayIndexBuf, "RollHeight");
            //EditFloat(data + 8, arrayIndex, arrayIndexBuf, "WeightBias");
            //EditFloat(data + 12, arrayIndex, arrayIndexBuf, "MassScale");
        } break;

        case ControllerDataRecord: {
            EditStringKey(data, arrayIndex, arrayIndexBuf, "DeviceName");
			EnumCombo(data + 8, "kUpdate\0kPress\0kRelease\0kAnalogPress\0kAnalogRelease\0kCenterControl",
                arrayIndex, arrayIndexBuf, "UpdateType");
            EditFloatN(data + 12, arrayIndex, arrayIndexBuf, 2, "LowerDZ", "UpperDZ");
        } break;

        case EngineLFOParams: {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 5, "FrequencyStart", "FrequencyEnd", "RpmAmplitude", "VolumeAmplitude", "Lifespan");
        } break;

        case HighwayPatternCarData: {
            EditInt(data, arrayIndex, arrayIndexBuf, "Row");
            EditInt(data+4, arrayIndex, arrayIndexBuf, "Lane");
			EditRefSpec(data + 8, arrayIndex, arrayIndexBuf, nextCollection, "VehicleClassKey", "VehicleCollectionKey");
            EnumCombo(data + 20, "LANECHG_NONE\0LANECHG_LEFT\0LANECHG_RIGHT",
                arrayIndex, arrayIndexBuf, "Change");
            auto totalWidth = ImGui::CalcItemWidth();
            CorrectedText(totalWidth, "ChangeDist");
            EditFloatHex(data + 24, arrayIndex, arrayIndexBuf, "Unk", false);
                
        } break;

        case JittererParams_:
        case JittererParams: {
			EditFloatN(data, arrayIndex, arrayIndexBuf, 4, "MidPoint", "Range", "MaxDelta", "Multiplier"); //vlted has it wrong (array of shorts), i cba
        } break;

        case PerformanceWeightWithDirection: {
			EnumCombo(data, "PERFORMANCE_SIMPLE_TIME_0_TO_100_MPH\0PERFORMANCE_SIMPLE_TIME_0_TO_60_MPH\0PERFORMANCE_SIMPLE_TIME_70_TO_150_MPH\0PERFORMANCE_SIMPLE_TIME_100_TO_0_MPH\0PERFORMANCE_SIMPLE_TIME_60_TO_0_MPH\0PERFORMANCE_SIMPLE_DISTANCE_60_TO_0_MPH\0PERFORMANCE_SIMPLE_DISTANCE_100_TO_0_MPH\0PERFORMANCE_SIMPLE_TIME_QUARTER_MILE\0PERFORMANCE_SIMPLE_SPEED_QUARTER_MILE_MPH\0PERFORMANCE_SIMPLE_TIME_60_FEET\0PERFORMANCE_SIMPLE_TOPSPEED\0PERFORMANCE_SIMPLE_MAX_GS_30_MPH\0PERFORMANCE_SIMPLE_MAX_GS_60_MPH\0PERFORMANCE_SIMPLE_MAX_GS_100_MPH\0PERFORMANCE_SIMPLE_TIME_CORNER_200M_40M_300M\0PERFORMANCE_SIMPLE_DOWNFORCE\0PERFORMANCE_SIMPLE_DRAG\0PERFORMANCE_SIMPLE_FRONT_TOE_30_MPH\0PERFORMANCE_SIMPLE_FRONT_TOE_60_MPH\0PERFORMANCE_SIMPLE_FRONT_TOE_100_MPH\0PERFORMANCE_SIMPLE_CASTER\0PERFORMANCE_SIMPLE_FRONT_WEIGHT_BIAS\0PERFORMANCE_SIMPLE_EBRAKE\0PERFORMANCE_SIMPLE_STEERING_ALIGNMENT_TORQUE\0PERFORMANCE_SIMPLE_AERO_CG\0PERFORMANCE_SIMPLE_ROLL_CENTER\0PERFORMANCE_SIMPLE_SPRING_STIFFNESS\0PERFORMANCE_SIMPLE_SWAYBAR_STIFFNESS\0PERFORMANCE_SIMPLE_SHOCK_DIGRESSION\0PERFORMANCE_SIMPLE_SHOCK_VALVING\0PERFORMANCE_SIMPLE_HORSEPOWER\0PERFORMANCE_SIMPLE_HORSEPOWERRPM\0PERFORMANCE_SIMPLE_PEAKTORQUE\0PERFORMANCE_SIMPLE_PEAKTORQUERPM\0PERFORMANCE_SIMPLE_WEIGHT\0PERFORMANCE_SIMPLE_WEIGHT_TO_POWER_RATIO\0PERFORMANCE_COMPLEX_GRIP\0PERFORMANCE_COMPLEX_TOPSPEED\0PERFORMANCE_COMPLEX_ACCELERATION\0PERFORMANCE_COMPLEX_HANDLING\0NUMBER_PERFORMANCE",
				arrayIndex, arrayIndexBuf, "PerformanceType");
            EditByte(data + 4, arrayIndex, arrayIndexBuf, "Inverse");
			EditFloat(data + 8, arrayIndex, arrayIndexBuf, "Percentage");
        } break;

        case FEMsgToSoundTrigger: {
            auto totalWidth = ImGui::CalcItemWidth();
            CorrectedText(totalWidth, "FEngMsg");
            EditBStringHash(data, arrayIndex, arrayIndexBuf, "1", false);
            CorrectedText(totalWidth, "SoundTrigger");
            EnumCombo(data + 4, "none\0splice\0hud_countdown_3\0hud_countdown_2\0hud_countdown_1\0hud_countdown_go\0hud_getmoney_opp\0hud_getmoney_team\0hud_heat_up\0hud_lap_counter\0hud_losemoney_team\0hud_score_opp\0hud_score_team\0hud_split_time\0cars_accept\0cars_accept_screen\0cars_accept_screen_conc\0cars_appear\0cars_back\0cars_back_screen\0cars_bottom_row\0cars_buy_car\0cars_context_close\0cars_context_open\0cars_datasheet_appear\0cars_datasheet_disappear\0cars_disappear\0cars_down\0cars_cancel\0cars_delete\0cars_failure\0cars_finish\0cars_left\0cars_limit_reached\0cars_locked\0cars_more_info\0cars_more_info_scr\0cars_more_info_scr_cnc\0cars_move_down\0cars_move_up\0cars_no\0cars_no_cash\0cars_ok\0cars_option_left\0cars_option_right\0cars_page_next\0cars_page_prev\0cars_photo_cycle_camera\0cars_photo_snapshot\0cars_purchase\0cars_receive\0cars_remote_change\0cars_revert\0cars_right\0cars_select\0cars_select_car\0cars_select_screen\0cars_success\0cars_send\0cars_sub_enter\0cars_sub_exit\0cars_tform_next\0cars_tform_prev\0cars_top_row\0cars_tune_color\0cars_yes\0cars_up\0main_accept\0main_accept_screen\0main_accept_scrinst\0main_appear\0main_back\0main_back_screen\0main_blank_appear\0main_busted\0main_busted_x\0main_busted_yellow_bar\0main_cancel\0main_context_close\0main_context_open\0main_datasheet_appear\0main_datasheet_disappear\0main_disappear\0main_dominated\0main_down\0main_driver_skills_tally\0main_evaded\0main_failure\0main_impounded\0main_left\0main_level_up\0main_level_up_continue\0main_limit_reached\0main_mission_failed\0main_mission_success\0main_more_info\0main_more_info_scr\0main_more_info_scr_cnc\0main_no\0main_ok\0main_option_left\0main_option_right\0main_page_next\0main_page_prev\0main_photo_cycle_camera\0main_photo_snapshot\0main_receive\0main_right\0main_select\0main_send\0main_start_game\0main_sub_enter\0main_sub_exit\0main_success\0main_text_appear\0main_up\0main_yes\0main_zoom_in_start\0main_zoom_out_start\0map_district_away\0map_enter_from_back\0map_enter_world\0map_event_over\0map_district_over\0map_datasheet_appear\0map_datasheet_disappear\0map_zoom_in_start\0map_zoom_out_start",
                arrayIndex, arrayIndexBuf, "2", false);
        } break;

        case MovieVolume: {
            auto totalWidth = ImGui::CalcItemWidth();
            CorrectedText(totalWidth, "Name");
            EditBStringHash(data, arrayIndex, arrayIndexBuf, "1", false);
            CorrectedText(totalWidth, "Volume");
            EditByte(data + 4, arrayIndex, arrayIndexBuf, "2", false);
        } break;

        case DamageScaleRecord: {
			EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "VisualScale", "HitPointScale");
        } break;

        case Control_:
        case Control: { //DamageFilter::Control
            EditInt(data, arrayIndex, arrayIndexBuf, "Allow");
            EditInt(data+4, arrayIndex, arrayIndexBuf, "Reject");
            EditFloat(data + 8, arrayIndex, arrayIndexBuf, "MaxCausalityTime");
        } break;

        case CameraReactionRecord: {
            EnumCombo(data, "CameraReaction_Base\0CameraReaction_SpeedTrim\0CameraReaction_Speed\0CameraReaction_AccelFwd\0CameraReaction_AccelSide",
                arrayIndex, arrayIndexBuf, "ReactionType");
            EditFloatN(data + 4, arrayIndex, arrayIndexBuf, 3, "InputMin", "ValueMin1", "ValueMin2");
            EditFloatN(data + 16, arrayIndex, arrayIndexBuf, 3, "InputMax", "ValueMax1", "ValueMax2");
        } break;

        case CameraSpeedReactionRecord: {
            EditFloatN(data, arrayIndex, arrayIndexBuf, 2, "SpeedMin", "ValueMin");
            EditFloatN(data + 8, arrayIndex, arrayIndexBuf, 2, "SpeedMax", "ValueMax");
        } break;

        case CameraCurveReactionRecord: {
            auto totalWidth = ImGui::CalcItemWidth();
            CorrectedText(totalWidth, "ReactionType");
            EnumCombo(data, "CameraReaction_Base\0CameraReaction_SpeedTrim\0CameraReaction_Speed\0CameraReaction_AccelFwd\0CameraReaction_AccelSide",
                arrayIndex, arrayIndexBuf, "1", false);
            
            auto curve = (struct Curve*)(data+4);
            ImGui::PushID(1);
            EditFloatN(&curve->xMin, arrayIndex, arrayIndexBuf, 2, NULL, "Front_MinX", "Front_MaxX", "2", "3"); //NULL signal for wrongly named shi
            ImGui::PopID();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushID(2);
            EditFloatN(&curve->yMin, arrayIndex, arrayIndexBuf, 2, NULL, "Front_MinY", "Front_MaxY", "4", "5");
            ImGui::PopID();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushID(3);
            EditFloatN(&curve->graphScale, arrayIndex, arrayIndexBuf, 1, NULL, "Front_GraphScale", "6");
            ImGui::PopID();
            EditGraphData(curve, arrayIndex, arrayIndexBuf, "", "7", "9", "11");
            PreviewCurve(curve);

        } break;

                         //enums
#if 1
        case ePresetCarFilter: {
            const char* items[] = { "PRESET_CAR_FILTER_BONUS", "PRESET_CAR_FILTER_CUSTOM", "PRESET_CAR_FILTER_AI", "PRESET_CAR_FILTER_DEBUG" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case eDRIVE_BY_TYPE: {
            const char* items[] = { "DRIVE_BY_UNKNOWN", "DRIVE_BY_TREE", "DRIVE_BY_LAMPPOST", "DRIVE_BY_SMOKABLE", "DRIVE_BY_TUNNEL_IN", "DRIVE_BY_TUNNEL_OUT",
                "DRIVE_BY_OVERPASS_IN", "DRIVE_BY_OVERPASS_OUT", "DRIVE_BY_AI_CAR", "DRIVE_BY_TRAFFIC", "DRIVE_BY_BRIDGE", "DRIVE_BY_PRE_COL", "DRIVE_BY_CAMERA_BY",
            "DRIVE_BY_POLE_SKINNY", "DRIVE_BY_SUPPORT_OVERPASS", "DRIVE_BY_BILLBOARD", "DRIVE_BY_STRUCTURE", "DRIVE_BY_STAGING", "DRIVE_BY_TUNNEL_FAN", "DRIVE_BY_LIGHTBULB" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case CubeType: {
            const char* items[] = { "FILTER0", "TRANSFORMERS", "GONE_IN_60_SECONDS","BOYS_IN_THE_HOOD","INTHEZONE","COPINTRO","LEVELUP","WHITEOVERLAY","COPAPPROACHING","FE" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case KIT_TYPE: {
            const char* items[] = { "KIT_TYPE_BASE", "KIT_TYPE_STOCK", "KIT_TYPE_AUTOSCULPT", "KIT_TYPE_WIDEBODY" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case eInductionType_: {
            const char* items[] = { "INDUCTION_NONE", "INDUCTION_TURBO_CHARGER", "INDUCTION_ROOTS_BLOWER", "INDUCTION_CENTRIFUGAL_BLOWER", "INDUCTION_TWINSCREW_BLOWER" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case EmitterPostProcessingEffect: {
            const char* items[] = { "EmitterPostProcessingEffect_NONE", "EmitterPostProcessingEffect_SMOKE" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case eSNDCTLSTATE: {
            const char* items[] = { "SNDSTATE_OFF", "SNDSTATE_PAUSE", "SNDSTATE_INGAME", "SNDSTATE_FE", "SNDSTATE_FE_UPSCREEN","SNDSTATE_FE_SMS_MESSAGE",
                "SNDSTATE_NIS_STORY","SNDSTATE_NIS_INTRO", "SNDSTATE_NIS_321","SNDSTATE_NIS_BLK","SNDSTATE_NIS_ARREST","SNDSTATE_FMV",
                "SNDSTATE_STOP_MUSIC","SNDSTATE_GAMESTARTRACE","SNDSTATE_MINILOAD","SNDSTATE_FADEOUT","SNDSTATE_ERROR","SNDSTATE_SYSTEM_HUD","SNDSTATE_NIS_CAM",
            "SNDSTATE_POSTRACESCREEN", "SNDSTATE_LOADTRANSITION", "SNDSTATE_GENERICDIALOG", "SNDSTATE_NISLOADING", "SNDSTATE_PRERACESCREEN" };
            EnumCombo(data, items, IM_COUNTOF(items), arrayIndex, arrayIndexBuf);
        } break;

        case eLiveryID: {
            EnumCombo(data, "LIVERYID_COP_CAR_MID_05_01\0LIVERYID_COP_CAR_MUS\0LIVERYID_COP_CAR_GTR\0LIVERYID_COP_CAR_POR_911\0LIVERYID_COP_SUV\0LIVERYID_BOSS_CARMEN\0LIVERYID_BOSS_CHASE\0LIVERYID_BOSS_ISF\0LIVERYID_BOSS_NICKEL\0LIVERYID_BOSS_ROSE\0LIVERYID_BOSS_ZACK\0LIVERYID_BOSS_INTRO\0LIVERYID_MEGANE\0LIVERYID_BOSS_GMAC\0LIVERYID_TERLINGUA\0LIVERYID_BOSS_HECTOR\0LIVERYID_DCAST_GT2\0LIVERYID_DCAST_R8\0LIVERYID_DCAST CAMARO\0LIVERYID_DCAST_CHARGER\0LIVERYID_DCAST_VIPER\0LIVERYID_DCAST MUSTANG\0LIVERYID_DCAST_MURCIELAGO\0LIVERYID_DCAST ISF\0LIVERYID_DCAST_MPS3\0LIVERYID_DCAST_GTO\0LIVERYID_DCAST_GTR\0LIVERYID_DCAST 911 TURBO\0LIVERYID_DCAST_GOLF\0LIVERYID_DCAST CAMARO2\0LIVERYID_BOSS_ROSES",
                arrayIndex, arrayIndexBuf);
        } break;

        case VinylFinish: {
            EnumCombo(data, "VinylFinishMatte\0VinylFinishGloss\0VinylFinishChrome",
                arrayIndex, arrayIndexBuf);
        } break;

        case COLLISIONSFX_LAYERS: {
            EnumCombo(data, "unaff\0LAYER_CORE\0LAYER_ZONE\0LAYER_SURFACE\0LAYER_OBJECT\0LAYER_SWEETENERS\0LAYER_SCRAPE",
                arrayIndex, arrayIndexBuf);
        } break;

        case ePaintSlot: {
            EditePaintSlot(data, arrayIndex, arrayIndexBuf);
        } break;

        case Splicer_Collision_Core: {
            EnumCombo(data, "Cor_Side_Lit_01\0Cor_Side_Lit_02\0Cor_Side_Lit_03\0Cor_Side_Lit_04\0Cor_Side_Lit_05\0Cor_Side_Med_01\0Cor_Side_Med_02\0Cor_Side_Med_03\0Cor_Side_Med_04\0Cor_Side_Med_05\0Cor_Side_Hard_01\0Cor_Side_Hard_02\0Cor_Side_Hard_03\0Cor_Side_Hard_04\0Cor_Side_Hard_05\0Cor_Side_Fatal_01\0Cor_Side_Fatal_02\0Cor_Side_Fatal_03\0Cor_Side_Fatal_04\0Cor_Side_Fatal_05\0Cor_Frnt_Lit_01\0Cor_Frnt_Lit_02\0Cor_Frnt_Lit_03\0Cor_Frnt_Lit_04\0Cor_Frnt_Lit_05\0Cor_Frnt_Med_01\0Cor_Frnt_Med_02\0Cor_Frnt_Med_03\0Cor_Frnt_Med_04\0Cor_Frnt_Med_05\0Cor_Frnt_Hard_01\0Cor_Frnt_Hard_02\0Cor_Frnt_Hard_03\0Cor_Frnt_Hard_04\0Cor_Frnt_Hard_05\0Cor_Frnt_Fatal_01\0Cor_Frnt_Fatal_02\0Cor_Frnt_Fatal_03\0Cor_Frnt_Fatal_04\0Cor_Frnt_Fatal_05\0Cor_Bot_Lit_01\0Cor_Bot_Lit_02\0Cor_Bot_Lit_03\0Cor_Bot_Lit_04\0Cor_Bot_Lit_05\0Cor_Bot_Med_01\0Cor_Bot_Med_02\0Cor_Bot_Med_03\0Cor_Bot_Hard_01\0Cor_Bot_Hard_02\0Cor_Bot_Hard_03",
                arrayIndex, arrayIndexBuf);
        } break;

        case Splicer_Collision_Object: {
            const char* items[] = { "none", "bbq_00", "bbq_00_v00", "bbq_00_v01", "bbq_00_v02", "bbq_01", "bbq_01_v00", "bbq_01_v01", "bbq_01_v02", "bus_sign_00", "bus_sign_00_v00", "bus_sign_00_v01", "bus_sign_00_v02", "bus_sign_01", "bus_sign_01_v00", "bus_sign_01_v01", "bus_sign_01_v02", "busstop_00", "busstop_00_v00", "busstop_00_v01", "busstop_00_v02", "busstop_01", "busstop_01_v00", "busstop_01_v01", "busstop_01_v02", "busstop_02", "busstop_02_v00", "busstop_02_v01", "busstop_02_v02", "car_bumper_00", "car_bumper_00_v00", "car_bumper_00_v01", "car_bumper_00_v02", "car_bumper_01", "car_bumper_01_v00", "car_bumper_01_v01", "car_bumper_01_v02", "car_coplights_00", "car_coplights_00_v00", "car_coplights_00_v01", "car_coplights_00_v02", "car_coplights_01", "car_coplights_01_v00", "car_coplights_01_v01", "car_coplights_01_v02", "car_door_00", "car_door_00_v00", "car_door_00_v01", "car_door_00_v02", "car_door_01", "car_door_01_v00", "car_door_01_v01", "car_door_01_v02", "cardboard_00", "cardboard_00_v00", "cardboard_00_v01", "cardboard_00_v02", "cardboard_01", "cardboard_01_v00", "cardboard_01_v01", "cardboard_01_v02", "cardboard_02", "cardboard_02_v00", "cardboard_02_v01", "cardboard_02_v02", "casinobreak_00", "casinobreak_00_v00", "casinobreak_00_v01", "casinobreak_00_v02", "casinobreak_01", "casinobreak_01_v00", "casinobreak_01_v01", "casinobreak_01_v02", "chain_link_00", "chain_link_00_v00", "chain_link_00_v01", "chain_link_00_v02", "chain_link_01", "chain_link_01_v00", "chain_link_01_v01", "chain_link_01_v02", "chain_link_02", "chain_link_02_v00", "chain_link_02_v01", "chain_link_02_v02", "conc_barricade_00", "conc_barricade_00_v00", "conc_barricade_00_v01", "conc_barricade_00_v02", "conc_barricade_01", "conc_barricade_01_v00", "conc_barricade_01_v01", "conc_barricade_01_v02", "construction_sign_detach_00", "construction_sign_detach_00_v00", "construction_sign_detach_00_v01", "construction_sign_detach_00_v02", "construction_sign_detach_01", "construction_sign_detach_01_v00", "construction_sign_detach_01_v01", "construction_sign_detach_01_v02", "construction_sign_detach_02", "construction_sign_detach_02_v00", "construction_sign_detach_02_v01", "construction_sign_detach_02_v02", "construction_sign_sml_00", "construction_sign_sml_00_v00", "construction_sign_sml_00_v01", "construction_sign_sml_00_v02", "construction_sign_sml_01", "construction_sign_sml_01_v00", "construction_sign_sml_01_v01", "construction_sign_sml_01_v02", "construction_sign_sml_02", "construction_sign_sml_02_v00", "construction_sign_sml_02_v01", "construction_sign_sml_02_v02", "construction_sign_scrn_00", "construction_sign_scrn_00_v00", "construction_sign_scrn_00_v01", "construction_sign_scrn_00_v02", "construction_sign_scrn_01", "construction_sign_scrn_01_v00", "construction_sign_scrn_01_v01", "construction_sign_scrn_01_v02", "construction_sign_scrn_02", "construction_sign_scrn_02_v00", "construction_sign_scrn_02_v01", "construction_sign_scrn_02_v02", "control_panel_stairs_00", "control_panel_stairs_00_v00", "control_panel_stairs_00_v01", "control_panel_stairs_00_v02", "control_panel_stairs_01", "control_panel_stairs_01_v00", "control_panel_stairs_01_v01", "control_panel_stairs_01_v02", "control_panel_stairs_02", "control_panel_stairs_02_v00", "control_panel_stairs_02_v01", "control_panel_stairs_02_v02", "device_box_large_00", "device_box_large_00_v00", "device_box_large_00_v01", "device_box_large_00_v02", "device_box_large_01", "device_box_large_01_v00", "device_box_large_01_v01", "device_box_large_01_v02", "device_box_large_02", "device_box_large_02_v00", "device_box_large_02_v01", "device_box_large_02_v02", "field_marker_00", "field_marker_00_v00", "field_marker_00_v01", "field_marker_00_v02", "field_marker_01", "field_marker_01_v00", "field_marker_01_v01", "field_marker_01_v02", "field_pylon_00", "field_pylon_00_v00", "field_pylon_00_v01", "field_pylon_00_v02", "field_pylon_01", "field_pylon_01_v00", "field_pylon_01_v01", "field_pylon_01_v02", "fire_box_00", "fire_box_00_v00", "fire_box_00_v01", "fire_box_00_v02", "fire_box_01", "fire_box_01_v00", "fire_box_01_v01", "fire_box_01_v02", "fire_box_02", "fire_box_02_v00", "fire_box_02_v01", "fire_box_02_v02", "furniture_cafe_chair_00", "furniture_cafe_chair_00_v00", "furniture_cafe_chair_00_v01", "furniture_cafe_chair_00_v02", "furniture_cafe_chair_01", "furniture_cafe_chair_01_v00", "furniture_cafe_chair_01_v01", "furniture_cafe_chair_01_v02", "furniture_cafe_chair_02", "furniture_cafe_chair_02_v00", "furniture_cafe_chair_02_v01", "furniture_cafe_chair_02_v02", "furniture_cafe_table_00", "furniture_cafe_table_00_v00", "furniture_cafe_table_00_v01", "furniture_cafe_table_00_v02", "furniture_cafe_table_01", "furniture_cafe_table_01_v00", "furniture_cafe_table_01_v01", "furniture_cafe_table_01_v02", "furniture_cafe_table_02", "furniture_cafe_table_02_v00", "furniture_cafe_table_02_v01", "furniture_cafe_table_02_v02", "furniture_cafe_umbrella_00", "furniture_cafe_umbrella_00_v00", "furniture_cafe_umbrella_00_v01", "furniture_cafe_umbrella_00_v02", "furniture_cafe_umbrella_01", "furniture_cafe_umbrella_01_v00", "furniture_cafe_umbrella_01_v01", "furniture_cafe_umbrella_01_v02", "furniture_cafe_umbrella_02", "furniture_cafe_umbrella_02_v00", "furniture_cafe_umbrella_02_v01", "furniture_cafe_umbrella_02_v02", "furniture_couch_00", "furniture_couch_00_v00", "furniture_couch_00_v01", "furniture_couch_00_v02", "furniture_couch_01", "furniture_couch_01_v00", "furniture_couch_01_v01", "furniture_couch_01_v02", "furniture_couch_02", "furniture_couch_02_v00", "furniture_couch_02_v01", "furniture_couch_02_v02", "furniture_cushion_00", "furniture_cushion_00_v00", "furniture_cushion_00_v01", "furniture_cushion_00_v02", "furniture_cushion_01", "furniture_cushion_01_v00", "furniture_cushion_01_v01", "furniture_cushion_01_v02", "furniture_cushion_02", "furniture_cushion_02_v00", "furniture_cushion_02_v01", "furniture_cushion_02_v02", "furniture_lawn_chair_00", "furniture_lawn_chair_00_v00", "furniture_lawn_chair_00_v01", "furniture_lawn_chair_00_v02", "furniture_lawn_chair_01", "furniture_lawn_chair_01_v00", "furniture_lawn_chair_01_v01", "furniture_lawn_chair_01_v02", "furniture_mattress_00", "furniture_mattress_00_v00", "furniture_mattress_00_v01", "furniture_mattress_00_v02", "furniture_mattress_01", "furniture_mattress_01_v00", "furniture_mattress_01_v01", "furniture_mattress_01_v02", "furniture_mattress_02", "furniture_mattress_02_v00", "furniture_mattress_02_v01", "furniture_mattress_02_v02", "garbage_domestic_00", "garbage_domestic_00_v00", "garbage_domestic_00_v01", "garbage_domestic_00_v02", "garbage_domestic_01", "garbage_domestic_01_v00", "garbage_domestic_01_v01", "garbage_domestic_01_v02", "garbage_domestic_02", "garbage_domestic_02_v00", "garbage_domestic_02_v01", "garbage_domestic_02_v02", "garbage_park_00", "garbage_park_00_v00", "garbage_park_00_v01", "garbage_park_00_v02", "garbage_park_01", "garbage_park_01_v00", "garbage_park_01_v01", "garbage_park_01_v02", "garbage_park_02", "garbage_park_02_v00", "garbage_park_02_v01", "garbage_park_02_v02", "goal_post_00", "goal_post_00_v00", "goal_post_00_v01", "goal_post_00_v02", "goal_post_01", "goal_post_01_v00", "goal_post_01_v01", "goal_post_01_v02", "goal_post_02", "goal_post_02_v00", "goal_post_02_v01", "goal_post_02_v02", "hydrant_00", "hydrant_00_v00", "hydrant_00_v01", "hydrant_00_v02", "hydrant_01", "hydrant_01_v00", "hydrant_01_v01", "hydrant_01_v02", "light_plastic_barrel_00", "light_plastic_barrel_00_v00", "light_plastic_barrel_00_v01", "light_plastic_barrel_00_v02", "light_plastic_barrel_01", "light_plastic_barrel_01_v00", "light_plastic_barrel_01_v01", "light_plastic_barrel_01_v02", "light_plastic_barrel_02", "light_plastic_barrel_02_v00", "light_plastic_barrel_02_v01", "light_plastic_barrel_02_v02", "light_wood_00", "light_wood_00_v00", "light_wood_00_v01", "light_wood_00_v02", "mailbox_00", "mailbox_00_v00", "mailbox_00_v01", "mailbox_00_v02", "mailbox_01", "mailbox_01_v00", "mailbox_01_v01", "mailbox_01_v02", "mansion_gate_00", "mansion_gate_00_v00", "mansion_gate_00_v01", "mansion_gate_00_v02", "mansion_gate_01", "mansion_gate_01_v00", "mansion_gate_01_v01", "mansion_gate_01_v02", "metal_bar_large_00", "metal_bar_large_00_v00", "metal_bar_large_00_v01", "metal_bar_large_00_v02", "metal_bar_large_01", "metal_bar_large_01_v00", "metal_bar_large_01_v01", "metal_bar_large_01_v02", "metal_bar_large_02", "metal_bar_large_02_v00", "metal_bar_large_02_v01", "metal_bar_large_02_v02", "metal_bar_medium_00", "metal_bar_medium_00_v00", "metal_bar_medium_00_v01", "metal_bar_medium_00_v02", "metal_bar_medium_01", "metal_bar_medium_01_v00", "metal_bar_medium_01_v01", "metal_bar_medium_01_v02", "metal_bar_small_00", "metal_bar_small_00_v00", "metal_bar_small_00_v01", "metal_bar_small_00_v02", "metal_bar_small_01", "metal_bar_small_01_v00", "metal_bar_small_01_v01", "metal_bar_small_01_v02", "metal_bar_piece_large_00", "metal_bar_piece_large_00_v00", "metal_bar_piece_large_00_v01", "metal_bar_piece_large_00_v02", "metal_bar_piece_large_01", "metal_bar_piece_large_01_v00", "metal_bar_piece_large_01_v01", "metal_bar_piece_large_01_v02", "metal_bar_piece_large_02", "metal_bar_piece_large_02_v00", "metal_bar_piece_large_02_v01", "metal_bar_piece_large_02_v02", "metal_barrel_00", "metal_barrel_00_v00", "metal_barrel_00_v01", "metal_barrel_00_v02", "metal_barrel_01", "metal_barrel_01_v00", "metal_barrel_01_v01", "metal_barrel_01_v02", "metal_barrel_02", "metal_barrel_02_v00", "metal_barrel_02_v01", "metal_barrel_02_v02", "metal_barrel_oil_00", "metal_barrel_oil_00_v00", "metal_barrel_oil_00_v01", "metal_barrel_oil_00_v02", "metal_barrel_oil_01", "metal_barrel_oil_01_v00", "metal_barrel_oil_01_v01", "metal_barrel_oil_01_v02", "metal_barrel_oil_02", "metal_barrel_oil_02_v00", "metal_barrel_oil_02_v01", "metal_barrel_oil_02_v02", "metal_beam_00", "metal_beam_00_v00", "metal_beam_00_v01", "metal_beam_00_v02", "metal_beam_01", "metal_beam_01_v00", "metal_beam_01_v01", "metal_beam_01_v02", "metal_box_large_00", "metal_box_large_00_v00", "metal_box_large_00_v01", "metal_box_large_00_v02", "metal_box_large_01", "metal_box_large_01_v00", "metal_box_large_01_v01", "metal_box_large_01_v02", "metal_box_large_02", "metal_box_large_02_v00", "metal_box_large_02_v01", "metal_box_large_02_v02", "metal_cooler_00", "metal_cooler_00_v00", "metal_cooler_00_v01", "metal_cooler_00_v02", "metal_cooler_01", "metal_cooler_01_v00", "metal_cooler_01_v01", "metal_cooler_01_v02", "metal_crate_00", "metal_crate_00_v00", "metal_crate_00_v01", "metal_crate_00_v02", "metal_crate_01", "metal_crate_01_v00", "metal_crate_01_v01", "metal_crate_01_v02", "metal_dumpster_00", "metal_dumpster_00_v00", "metal_dumpster_00_v01", "metal_dumpster_00_v02", "metal_dumpster_01", "metal_dumpster_01_v00", "metal_dumpster_01_v01", "metal_dumpster_01_v02", "metal_dumpster_02", "metal_dumpster_02_v00", "metal_dumpster_02_v01", "metal_dumpster_02_v02", "metal_frame_large_00", "metal_frame_large_00_v00", "metal_frame_large_00_v01", "metal_frame_large_00_v02", "metal_frame_large_01", "metal_frame_large_01_v00", "metal_frame_large_01_v01", "metal_frame_large_01_v02", "metal_frame_large_02", "metal_frame_large_02_v00", "metal_frame_large_02_v01", "metal_frame_large_02_v02", "metal_frame_medium_00", "metal_frame_medium_00_v00", "metal_frame_medium_00_v01", "metal_frame_medium_00_v02", "metal_frame_medium_01", "metal_frame_medium_01_v00", "metal_frame_medium_01_v01", "metal_frame_medium_01_v02", "metal_frame_medium_02", "metal_frame_medium_02_v00", "metal_frame_medium_02_v01", "metal_frame_medium_02_v02", "metal_frame_small_00", "metal_frame_small_00_v00", "metal_frame_small_00_v01", "metal_frame_small_00_v02", "metal_frame_small_01", "metal_frame_small_01_v00", "metal_frame_small_01_v01", "metal_frame_small_01_v02", "metal_frame_small_02", "metal_frame_small_02_v00", "metal_frame_small_02_v01", "metal_frame_small_02_v02", "metal_panel_large_00", "metal_panel_large_00_v00", "metal_panel_large_00_v01", "metal_panel_large_00_v02", "metal_panel_large_01", "metal_panel_large_01_v00", "metal_panel_large_01_v01", "metal_panel_large_01_v02", "metal_panel_large_02", "metal_panel_large_02_v00", "metal_panel_large_02_v01", "metal_panel_large_02_v02", "metal_panel_medium_00", "metal_panel_medium_00_v00", "metal_panel_medium_00_v01", "metal_panel_medium_00_v02", "metal_panel_medium_01", "metal_panel_medium_01_v00", "metal_panel_medium_01_v01", "metal_panel_medium_01_v02", "metal_panel_medium_02", "metal_panel_medium_02_v00", "metal_panel_medium_02_v01", "metal_panel_medium_02_v02", "metal_panel_small_00", "metal_panel_small_00_v00", "metal_panel_small_00_v01", "metal_panel_small_00_v02", "metal_panel_small_01", "metal_panel_small_01_v00", "metal_panel_small_01_v01", "metal_panel_small_01_v02", "metal_panel_small_02", "metal_panel_small_02_v00", "metal_panel_small_02_v01", "metal_panel_small_02_v02", "metal_pipe_large_00", "metal_pipe_large_00_v00", "metal_pipe_large_00_v01", "metal_pipe_large_00_v02", "metal_pipe_large_01", "metal_pipe_large_01_v00", "metal_pipe_large_01_v01", "metal_pipe_large_01_v02", "metal_pipe_large_02", "metal_pipe_large_02_v00", "metal_pipe_large_02_v01", "metal_pipe_large_02_v02", "metal_pipe_medium_00", "metal_pipe_medium_00_v00", "metal_pipe_medium_00_v01", "metal_pipe_medium_00_v02", "metal_pipe_medium_01", "metal_pipe_medium_01_v00", "metal_pipe_medium_01_v01", "metal_pipe_medium_01_v02", "metal_pipe_medium_02", "metal_pipe_medium_02_v00", "metal_pipe_medium_02_v01", "metal_pipe_medium_02_v02", "metal_pipepiece_large_00", "metal_pipepiece_large_00_v00", "metal_pipepiece_large_00_v01", "metal_pipepiece_large_00_v02", "metal_pipepiece_large_01", "metal_pipepiece_large_01_v00", "metal_pipepiece_large_01_v01", "metal_pipepiece_large_01_v02", "metal_pipepiece_large_02", "metal_pipepiece_large_02_v00", "metal_pipepiece_large_02_v01", "metal_pipepiece_large_02_v02", "metal_slab_large_00", "metal_slab_large_00_v00", "metal_slab_large_00_v01", "metal_slab_large_00_v02", "metal_slab_large_01", "metal_slab_large_01_v00", "metal_slab_large_01_v01", "metal_slab_large_01_v02", "metal_slab_large_02", "metal_slab_large_02_v00", "metal_slab_large_02_v01", "metal_slab_large_02_v02", "milk_crate_00", "milk_crate_00_v00", "milk_crate_00_v01", "milk_crate_00_v02", "milk_crate_01", "milk_crate_01_v00", "milk_crate_01_v01", "milk_crate_01_v02", "milk_crate_02", "milk_crate_02_v00", "milk_crate_02_v01", "milk_crate_02_v02", "newspaper_box_00", "newspaper_box_00_v00", "newspaper_box_00_v01", "newspaper_box_00_v02", "newspaper_box_01", "newspaper_box_01_v00", "newspaper_box_01_v01", "newspaper_box_01_v02", "newspaper_box_02", "newspaper_box_02_v00", "newspaper_box_02_v01", "newspaper_box_02_v02", "parking_arm_00", "parking_arm_00_v00", "parking_arm_00_v01", "parking_arm_00_v02", "parking_arm_01", "parking_arm_01_v00", "parking_arm_01_v01", "parking_arm_01_v02", "parking_arm_02", "parking_arm_02_v00", "parking_arm_02_v01", "parking_arm_02_v02", "parking_meter_00", "parking_meter_00_v00", "parking_meter_00_v01", "parking_meter_00_v02", "parking_meter_01", "parking_meter_01_v00", "parking_meter_01_v01", "parking_meter_01_v02", "parking_meter_02", "parking_meter_02_v00", "parking_meter_02_v01", "parking_meter_02_v02", "pipe_stand_00", "pipe_stand_00_v00", "pipe_stand_00_v01", "pipe_stand_00_v02", "pipe_stand_01", "pipe_stand_01_v00", "pipe_stand_01_v01", "pipe_stand_01_v02", "plastic_barricade_00", "plastic_barricade_00_v00", "plastic_barricade_00_v01", "plastic_barricade_00_v02", "plastic_barricade_01", "plastic_barricade_01_v00", "plastic_barricade_01_v01", "plastic_barricade_01_v02", "plastic_barricade_02", "plastic_barricade_02_v00", "plastic_barricade_02_v01", "plastic_barricade_02_v02", "portapotty_00", "portapotty_00_v00", "portapotty_00_v01", "portapotty_00_v02", "portapotty_01", "portapotty_01_v00", "portapotty_01_v01", "portapotty_01_v02", "portapotty_02", "portapotty_02_v00", "portapotty_02_v01", "portapotty_02_v02", "powerlinestand_00", "powerlinestand_00_v00", "powerlinestand_00_v01", "powerlinestand_00_v02", "powerlinestand_01", "powerlinestand_01_v00", "powerlinestand_01_v01", "powerlinestand_01_v02", "prop_car_heavy_00", "prop_car_heavy_00_v00", "prop_car_heavy_00_v01", "prop_car_heavy_00_v02", "prop_car_heavy_01", "prop_car_heavy_01_v00", "prop_car_heavy_01_v01", "prop_car_heavy_01_v02", "prop_car_heavy_02", "prop_car_heavy_02_v00", "prop_car_heavy_02_v01", "prop_car_heavy_02_v02", "road_cone_00", "road_cone_00_v00", "road_cone_00_v01", "road_cone_00_v02", "road_cone_01", "road_cone_01_v00", "road_cone_01_v01", "road_cone_01_v02", "road_cone_02", "road_cone_02_v00", "road_cone_02_v01", "road_cone_02_v02", "road_pylon_00", "road_pylon_00_v00", "road_pylon_00_v01", "road_pylon_00_v02", "road_pylon_01", "road_pylon_01_v00", "road_pylon_01_v01", "road_pylon_01_v02", "road_pylon_02", "road_pylon_02_v00", "road_pylon_02_v01", "road_pylon_02_v02", "sandwich_board_00", "sandwich_board_00_v00", "sandwich_board_00_v01", "sandwich_board_00_v02", "sandwich_board_01", "sandwich_board_01_v00", "sandwich_board_01_v01", "sandwich_board_01_v02", "sandwich_board_02", "sandwich_board_02_v00", "sandwich_board_02_v01", "sandwich_board_02_v02", "sawhorse_00", "sawhorse_00_v00", "sawhorse_00_v01", "sawhorse_00_v02", "sawhorse_01", "sawhorse_01_v00", "sawhorse_01_v01", "sawhorse_01_v02", "scaffold_light_00", "scaffold_light_00_v00", "scaffold_light_00_v01", "scaffold_light_00_v02", "scaffold_light_01", "scaffold_light_01_v00", "scaffold_light_01_v01", "scaffold_light_01_v02", "shipping_container_00", "shipping_container_00_v00", "shipping_container_00_v01", "shipping_container_00_v02", "shipping_container_01", "shipping_container_01_v00", "shipping_container_01_v01", "shipping_container_01_v02", "shipping_container_02", "shipping_container_02_v00", "shipping_container_02_v01", "shipping_container_02_v02", "shopping_cart_00", "shopping_cart_00_v00", "shopping_cart_00_v01", "shopping_cart_00_v02", "shopping_cart_01", "shopping_cart_01_v00", "shopping_cart_01_v01", "shopping_cart_01_v02", "small_gate_00", "small_gate_00_v00", "small_gate_00_v01", "small_gate_00_v02", "small_gate_01", "small_gate_01_v00", "small_gate_01_v01", "small_gate_01_v02", "small_propanetank_00", "small_propanetank_00_v00", "small_propanetank_00_v01", "small_propanetank_00_v02", "small_propanetank_01", "small_propanetank_01_v00", "small_propanetank_01_v01", "small_propanetank_01_v02", "street_sign_00", "street_sign_00_v00", "street_sign_00_v01", "street_sign_00_v02", "street_sign_01", "street_sign_01_v00", "street_sign_01_v01", "street_sign_01_v02", "ticket_booth_00", "ticket_booth_00_v00", "ticket_booth_00_v01", "ticket_booth_00_v02", "ticket_booth_01", "ticket_booth_01_v00", "ticket_booth_01_v01", "ticket_booth_01_v02", "ticket_booth_02", "ticket_booth_02_v00", "ticket_booth_02_v01", "ticket_booth_02_v02", "traffic_signal_00", "traffic_signal_00_v00", "traffic_signal_00_v01", "traffic_signal_00_v02", "traffic_signal_01", "traffic_signal_01_v00", "traffic_signal_01_v01", "traffic_signal_01_v02", "trash_bag_00", "trash_bag_00_v00", "trash_bag_00_v01", "trash_bag_00_v02", "trash_bag_01", "trash_bag_01_v00", "trash_bag_01_v01", "trash_bag_01_v02", "water_barrier_00", "water_barrier_00_v00", "water_barrier_00_v01", "water_barrier_00_v02", "water_barrier_01", "water_barrier_01_v00", "water_barrier_01_v01", "water_barrier_01_v02", "wooden_bench_00", "wooden_bench_00_v00", "wooden_bench_00_v01", "wooden_bench_00_v02", "wooden_bench_01", "wooden_bench_01_v00", "wooden_bench_01_v01", "wooden_bench_01_v02", "wooden_board_medium_00", "wooden_board_medium_00_v00", "wooden_board_medium_00_v01", "wooden_board_medium_00_v02", "wooden_board_medium_01", "wooden_board_medium_01_v00", "wooden_board_medium_01_v01", "wooden_board_medium_01_v02", "wooden_board_medium_02", "wooden_board_medium_02_v00", "wooden_board_medium_02_v01", "wooden_board_medium_02_v02", "wooden_board_small_00", "wooden_board_small_00_v00", "wooden_board_small_00_v01", "wooden_board_small_00_v02", "wooden_board_small_01", "wooden_board_small_01_v00", "wooden_board_small_01_v01", "wooden_board_small_01_v02", "wooden_board_small_02", "wooden_board_small_02_v00", "wooden_board_small_02_v01", "wooden_board_small_02_v02", "wooden_crate_00", "wooden_crate_00_v00", "wooden_crate_00_v01", "wooden_crate_00_v02", "wooden_crate_01", "wooden_crate_01_v00", "wooden_crate_01_v01", "wooden_crate_01_v02", "wooden_fence_00", "wooden_fence_00_v00", "wooden_fence_00_v01", "wooden_fence_00_v02", "wooden_fence_01", "wooden_fence_01_v00", "wooden_fence_01_v01", "wooden_fence_01_v02", "wooden_pallet_00", "wooden_pallet_00_v00", "wooden_pallet_00_v01", "wooden_pallet_00_v02", "wooden_pallet_01", "wooden_pallet_01_v00", "wooden_pallet_01_v01", "wooden_pallet_01_v02", "wooden_panel_large_00", "wooden_panel_large_00_v00", "wooden_panel_large_00_v01", "wooden_panel_large_00_v02", "wooden_panel_large_01", "wooden_panel_large_01_v00", "wooden_panel_large_01_v01", "wooden_panel_large_01_v02", "wooden_panel_large_02", "wooden_panel_large_02_v00", "wooden_panel_large_02_v01", "wooden_panel_large_02_v02", "wooden_table_00", "wooden_table_00_v00", "wooden_table_00_v01", "wooden_table_00_v02", "wooden_table_01", "wooden_table_01_v00", "wooden_table_01_v01", "wooden_table_01_v02", "wooden_walkway_00", "wooden_walkway_00_v00", "wooden_walkway_00_v01", "wooden_walkway_00_v02", "wooden_walkway_01", "wooden_walkway_01_v00", "wooden_walkway_01_v01", "wooden_walkway_01_v02" };
            EnumCombo(data, items, IM_COUNTOF(items),
                arrayIndex, arrayIndexBuf);
        } break;

        case Splicer_Collision_Surface: {
            EnumCombo(data, "srf_car_front_lit_01\0srf_car_front_lit_02\0srf_car_front_lit_03\0srf_car_front_med_01\0srf_car_front_med_02\0srf_car_front_med_03\0srf_car_front_hrd_01\0srf_car_front_hrd_02\0srf_car_front_hrd_03\0srf_car_front_hrd_04\0srf_car_front_hrd_05\0srf_conc_front_lit_01\0srf_conc_front_lit_02\0srf_conc_front_lit_03\0srf_conc_front_med_01\0srf_conc_front_med_02\0srf_conc_front_med_03\0srf_conc_front_hrd_01\0srf_conc_front_hrd_02\0srf_conc_front_hrd_03\0srf_asphalt_front_lit_01\0srf_asphalt_front_lit_02\0srf_asphalt_front_lit_03\0srf_asphalt_front_med_01\0srf_asphalt_front_med_02\0srf_asphalt_front_med_03\0srf_asphalt_front_hrd_01\0srf_asphalt_front_hrd_02\0srf_asphalt_front_hrd_03\0srf_dirt_lit_01\0srf_dirt_lit_02\0srf_dirt_lit_03\0srf_dirt_med_01\0srf_dirt_med_02\0srf_dirt_med_03\0srf_dirt_hrd_01\0srf_dirt_hrd_02\0srf_dirt_hrd_03\0srf_grss_lit_01\0srf_grss_lit_02\0srf_grss_lit_03\0srf_grss_med_01\0srf_grss_med_02\0srf_grss_med_03\0srf_grss_hard_01\0srf_grss_hard_02\0srf_grss_hard_03\0srf_grvl_lit_01\0srf_grvl_lit_02\0srf_grvl_lit_03\0srf_grvl_med_01\0srf_grvl_med_02\0srf_grvl_med_03\0srf_grvl_hrd_01\0srf_grvl_hrd_02\0srf_grvl_hrd_03\0srf_rubber_lit_01\0srf_rubber_lit_02\0srf_rubber_lit_03\0srf_rubber_med_01\0srf_rubber_med_02\0srf_rubber_med_03\0srf_rubber_hrd_01\0srf_rubber_hrd_02\0srf_rubber_hrd_03\0srf_metal_lit_01\0srf_metal_lit_02\0srf_metal_lit_03\0srf_metal_med_01\0srf_metal_med_02\0srf_metal_med_03\0srf_metal_hrd_01\0srf_metal_hrd_02\0srf_metal_hrd_03",
                arrayIndex, arrayIndexBuf);
        } break;

        case ePaintUsage: {
            EnumCombo(data, "eBodyPaint\0eVinylPaint\0eRimPaint\0eWindowTint",
                arrayIndex, arrayIndexBuf);
        } break;

        case eSkyStyle: {
            EnumCombo(data, "SKY_STYLE_01\0SKY_STYLE_02\0SKY_STYLE_03\0SKY_STYLE_04\0SKY_STYLE_05\0SKY_STYLE_06\0SKY_STYLE_07\0SKY_STYLE_08\0SKY_STYLE_09\0SKY_STYLE_10\0SKY_STYLE_11\0SKY_STYLE_12\0SKY_STYLE_13\0SKY_STYLE_14\0SKY_STYLE_15\0SKY_STYLE_NUM",
                arrayIndex, arrayIndexBuf);
        } break;

        case eDecalType: {
            EnumCombo(data, "DECAL_TYPE_SKID_CONCRETE\0DECAL_TYPE_SKID_DIRT\0DECAL_TYPE_SKID_GRASS\0DECAL_TYPE_SKID_SAND\0DECAL_TYPE_SCRAPE_METAL\0DECAL_TYPE_SCRAPE_PLASTIC\0DECAL_TYPE_SCRAPE_WOOD\0DECAL_TYPE_SCRAPE_CONCRETE",
                arrayIndex, arrayIndexBuf);
        } break;

        case SURFACE_SFX: {
            EnumCombo(data, "SURFACE_SFX_NONE\0SURFACE_SFX_LIGHT_CRACK\0unaff\0SURFACE_SFX_TAR_STRIP\0unaff\0SURFACE_SFX_ROADSIDE_PATCH\0SURFACE_SFX_HEAVY_PATCH\0SURFACE_SFX_DIRT\0MAX_SURFACE_SFX",
                arrayIndex, arrayIndexBuf);
        } break;

        case SCRAPE_SURFACES: {
            EnumCombo(data, "SCRAPE_NONE\0SCRAPE_CONCRETE\0SCRAPE_METAL\0SCRAPE_RUBBER\0SCRAPE_OFFROAD",
                arrayIndex, arrayIndexBuf);
        } break;

        case SurfaceEffectType: {
            EnumCombo(data, "kSine\0kSquare\0kTriangle",
                arrayIndex, arrayIndexBuf);
        } break;

        case ROLLFX_LOOP: {
            EnumCombo(data, "ROLLFX_NONE\0ROLLFX_ASPHALT\0ROLLFX_CONCRETE\0ROLLFX_GRASS\0ROLLFX_SAND\0ROLLFX_GRAVEL\0ROLLFX_DIRT\0ROLLFX_SIDEWALK\0ROLLFX_COBBLESTONE\0ROLLFX_BUMPY_HIGHWAY\0ROLLFX_METAL\0ROLLFX_WET_ASPHALT\0ROLLFX_SPIKESTRIP\0MAX_NUM_ROLLFX",
                arrayIndex, arrayIndexBuf);
        } break;

        case PartType: {
            EnumCombo(data, "PartType_CarSpecific\0PartType_Universal\0PartType_UniversalAcrossKits\0PartType_Vinyl\0PartType_Windtunnel\0PartType_Damage\0PartType_DamageSD",
                arrayIndex, arrayIndexBuf);
        } break;

        case eBustedLevel: {
            EnumCombo(data, "BUST_LEVEL_CIVIC\0BUST_LEVEL_STATE\0BUST_LEVEL_SUPER_STATE\0BUST_LEVEL_FED\0BUST_LEVEL_SUV\0BUST_LEVEL_INVALID",
                arrayIndex, arrayIndexBuf);
        } break;

        case eEndTransitionType_: {
            EnumCombo(data, "END_CAMERA_BLEND\0END_WHITE_FLASH\0END_BLEND_AND_WHITE_FLASH",
                arrayIndex, arrayIndexBuf);
        } break;

        case eFinishOutcome_: {
            EnumCombo(data, "FINISH_WIN\0FINISH_LOSE\0FINISH_INVALID",
                arrayIndex, arrayIndexBuf);
        } break;

        case eOnlineStatus_: {
            EnumCombo(data, "ONLINE_ONLY\0ONLINE_DISABLED\0ONLINE_DOES_NOT_MATTER",
                arrayIndex, arrayIndexBuf);
        } break;

        case ePlayerUnderAIControl_: {
            EnumCombo(data, "AI_CONTROL_NEVER\0AI_CONTROL_WITH_COLLISIONS\0AI_CONTROL_NO_COLLISIONS",
                arrayIndex, arrayIndexBuf);
        } break;

        case eRaceType: {
            EnumCombo(data, "RACE_TYPE_SHORT_FOOTPRINT\0RACE_TYPE_MEDIUM_FOOTPRINT\0RACE_TYPE_LONG_FOOTPRINT\0RACE_TYPE_CHECKPOINT\0RACE_TYPE_HIGHWAY_BATTLE\0RACE_TYPE_OUTRUN\0RACE_TYPE_WANTED_EVENT\0RACE_TYPE_COPS\0RACE_TYPE_ROBBERS\0RACE_TYPE_INVALID",
                arrayIndex, arrayIndexBuf);
        } break;

        case eSceneRoot_: {
            EnumCombo(data, "ROOT_WORLD_ORIGIN\0ROOT_PLAYER_CAR_POS\0ROOT_PLAYER_CAR_POS_AT_GROUND_HEIGHT\0ROOT_PLAYER_CAR_POS_ORIENTED_TO_GROUND\0ROOT_PLAYER_CAMERA_POS\0ROOT_PLAYER_CAMERA_POS_AT_GROUND_HEIGHT\0ROOT_PLAYER_CAMERA_POS_ORIENTED_TO_GROUND\0ROOT_BIG_BANG_MARKER\0ROOT_BIG_BANG_MARKER_AT_GROUND_HEIGHT\0ROOT_BIG_BANG_MARKER_ORIENTED_TO_GROUND\0ROOT_TRACK_MARKER\0ROOT_TRACK_MARKER_AT_GROUND_HEIGHT\0ROOT_TRACK_MARKER_ORIENTED_TO_GROUND",
                arrayIndex, arrayIndexBuf);
        } break;

        case eVehicleCategory: {
            EnumCombo(data, "VC_PLAYER\0VC_RACER\0VC_COP\0VC_TRAFFIC\0VC_FORCE_PRESET_PLAYER\0VC_FORCE_PRESET_RACER\0VC_FORCE_PRESET_COP\0VC_FORCE_PRESET_TRAFFIC\0VC_COP_BY_THREAT_LEVEL",
                arrayIndex, arrayIndexBuf);
        } break;

        case ePerformanceType: {
            EnumCombo(data, "PERFORMANCE_SIMPLE_TIME_0_TO_100_MPH\0PERFORMANCE_SIMPLE_TIME_0_TO_60_MPH\0PERFORMANCE_SIMPLE_TIME_70_TO_150_MPH\0PERFORMANCE_SIMPLE_TIME_100_TO_0_MPH\0PERFORMANCE_SIMPLE_TIME_60_TO_0_MPH\0PERFORMANCE_SIMPLE_DISTANCE_60_TO_0_MPH\0PERFORMANCE_SIMPLE_DISTANCE_100_TO_0_MPH\0PERFORMANCE_SIMPLE_TIME_QUARTER_MILE\0PERFORMANCE_SIMPLE_SPEED_QUARTER_MILE_MPH\0PERFORMANCE_SIMPLE_TIME_60_FEET\0PERFORMANCE_SIMPLE_TOPSPEED\0PERFORMANCE_SIMPLE_MAX_GS_30_MPH\0PERFORMANCE_SIMPLE_MAX_GS_60_MPH\0PERFORMANCE_SIMPLE_MAX_GS_100_MPH\0PERFORMANCE_SIMPLE_TIME_CORNER_200M_40M_300M\0PERFORMANCE_SIMPLE_DOWNFORCE\0PERFORMANCE_SIMPLE_DRAG\0PERFORMANCE_SIMPLE_FRONT_TOE_30_MPH\0PERFORMANCE_SIMPLE_FRONT_TOE_60_MPH\0PERFORMANCE_SIMPLE_FRONT_TOE_100_MPH\0PERFORMANCE_SIMPLE_CASTER\0PERFORMANCE_SIMPLE_FRONT_WEIGHT_BIAS\0PERFORMANCE_SIMPLE_EBRAKE\0PERFORMANCE_SIMPLE_STEERING_ALIGNMENT_TORQUE\0PERFORMANCE_SIMPLE_AERO_CG\0PERFORMANCE_SIMPLE_ROLL_CENTER\0PERFORMANCE_SIMPLE_SPRING_STIFFNESS\0PERFORMANCE_SIMPLE_SWAYBAR_STIFFNESS\0PERFORMANCE_SIMPLE_SHOCK_DIGRESSION\0PERFORMANCE_SIMPLE_SHOCK_VALVING\0PERFORMANCE_SIMPLE_HORSEPOWER\0PERFORMANCE_SIMPLE_HORSEPOWERRPM\0PERFORMANCE_SIMPLE_PEAKTORQUE\0PERFORMANCE_SIMPLE_PEAKTORQUERPM\0PERFORMANCE_SIMPLE_WEIGHT\0PERFORMANCE_SIMPLE_WEIGHT_TO_POWER_RATIO\0PERFORMANCE_COMPLEX_GRIP\0PERFORMANCE_COMPLEX_TOPSPEED\0PERFORMANCE_COMPLEX_ACCELERATION\0PERFORMANCE_COMPLEX_HANDLING",
                arrayIndex, arrayIndexBuf);
        } break;

        case CarClassType: {
            EnumCombo(data, "CarClassType_Racing\0CarClassType_Cop\0CarClassType_Traffic",
                arrayIndex, arrayIndexBuf);
        } break;

        case eSFXOBJ_MOMENT_MIXOUT: {
            EnumCombo(data, "eAZI_MOMENT_3DPOS\0eVOL_MOMENT_PURSUIT_BREAKER\0eFLT_MOMENT_LOWPASSROLLOFF\0eVOL_MOMENT_NIS\0eVOL_MOMENT_CRASH_SWEETNER\0eVOL_MOMENT_SPEED_BREAKER\0eVOL_MOMENT_SPOT_AMBIENCE\0eVOL_MOMENT_MISC_FX\0eVOL_MOMENT_LOADING_MUSIC\0eVOL_MOMENT_DRIVER_JOB\0eVOL_MOMENT_RACE_INTRO\0eVOL_MOMENT_CAUSEWAY\0eVOL_MOMENT_ONLINE\0eVOL_MOMENT_ARRESTS\0eVOL_MOMENT_RADAR\0eVOL_MOMENT_RADAR_INIT\0eVOL_MOMENT_REGIONAL_AMBIENCE\0eVOL_MOMENT_PURSUIT_BREAKER_NIS\0eVOL_MOMENT_TAKEDOWN_CAM",
                arrayIndex, arrayIndexBuf);
        } break;

        case eSFXOBJ_MOMENT_MIXINPUT: {
            EnumCombo(data, "eTRG_MOMENT_SPEED_BREAKER\0eTRG_MOMENT_PURSUIT_BREAKER\0eTRG_MOMENT_NIS\0eTRG_MOMENT_CRASH_SWEETNER\0eTRG_MOMENT_LOADING_MUSIC\0eTRG_MOMENT_MUTE_AI\0eTRG_MOMENT_MUTE_PLAYER\0eTRG_MOMENT_SUDDEN_DEATH_CAM\0eTRG_MOMENT_ROAD_BLOCK_CAM\0eTRG_MOMENT_WIN_OR_LOSE\0eTRG_MOMENT_PURSUIT_BREAKER_NIS\0eTRG_MOMENT_TAKEDOWN_CAM",
                arrayIndex, arrayIndexBuf);
        } break;

        case EMomentChokeGroup:
        case EMomentChokeGroup_: {
            EnumCombo(data, "eMOMENT_CHOKE_NONE\0eMOMENT_CHOKE_PURSUIT_BREAKER\0eMOMENT_CHOKE_SPEED_BREAKER\0eMOMENT_CHOKE_CRASH_SWEETENER\0eMOMENT_CHOKE_NIS\0eMOMENT_CHOKE_SPOT_AMBIENCE\0eMOMENT_CHOKE_MISC_FX\0eMOMENT_CHOKE_JUMP_CAM\0eMOMENT_CHOKE_LOADING_MUSIC",
                arrayIndex, arrayIndexBuf);
        } break;

        case EMomentPanning:
        case EMomentPanning_: {
            EnumCombo(data, "eMOMENT_PANNING_NONE\0eMOMENT_PANNING_POINTSOURCE",
                arrayIndex, arrayIndexBuf);
        } break;

        case Tier: {
            EnumCombo(data, "kRaceTier_None\0kRaceTier_1\0kRaceTier_2\0kRaceTier_3\0kRaceTier_4\0kRaceTier_NumTiers",
                arrayIndex, arrayIndexBuf);
        } break;

        case RaceCarClass:
        case RaceCarClass_: {
            EnumCombo(data, "kRaceCar_ClassNone\0kRaceCar_ClassExotic\0kRaceCar_ClassMuscle\0kRaceCar_ClassTuner\0kRaceCar_NumClasses",
                arrayIndex, arrayIndexBuf);
        } break;

        case EAIBreakerType: {
            EnumCombo(data, "BT_NONE\0BT_REAR\0BT_SIDE",
                arrayIndex, arrayIndexBuf);
        } break;

        case SPLICE_DAMAGE: {
            EnumCombo(data, "dmg_hood_lit_01\0dmg_hood_lit_02\0dmg_hood_lit_03\0dmg_hood_lit_04\0dmg_hood_lit_05\0dmg_hood_lit_06\0dmg_hood_med_01\0dmg_hood_med_02\0dmg_hood_med_03\0dmg_hood_hrd_01\0dmg_hood_hrd_02\0dmg_hood_hrd_03\0dmg_bumper_lit_01\0dmg_bumper_lit_02\0dmg_bumper_lit_03\0dmg_bumper_med_01\0dmg_bumper_med_02\0dmg_bumper_med_03\0dmg_bumper_hrd_01\0dmg_bumper_hrd_02\0dmg_bumper_hrd_03\0dmg_panel_lit_01\0dmg_panel_lit_02\0dmg_panel_lit_03\0dmg_panel_med_01\0dmg_panel_med_02\0dmg_panel_med_03\0dmg_panel_hrd_01\0dmg_panel_hrd_02\0dmg_panel_hrd_03\0dmg_roof_lit_01\0dmg_roof_lit_02\0dmg_roof_lit_03\0dmg_roof_med_01\0dmg_roof_med_02\0dmg_roof_med_03\0dmg_roof_hrd_01\0dmg_roof_hrd_02\0dmg_roof_hrd_03\0dmg_door_lit_01\0dmg_door_lit_02\0dmg_door_lit_03\0dmg_door_med_01\0dmg_door_med_02\0dmg_door_med_03\0dmg_door_hrd_01\0dmg_door_hrd_02\0dmg_door_hrd_03\0dmg_frntwind_lit_01\0dmg_frntwind_lit_02\0dmg_frntwind_lit_03\0dmg_frntwind_hrd_01\0dmg_frntwind_hrd_02\0dmg_frntwind_hrd_03\0dmg_sdwind_lit_01\0dmg_sdwind_lit_02\0dmg_sdwind_lit_03\0dmg_sdwind_01\0dmg_sdwind_02\0dmg_sdwind_03\0dmg_sdwind_04\0dmg_sdwind_05\0dmg_hdlight_crack_01\0dmg_hdlight_crack_02\0dmg_hdlight_crack_03\0dmg_hdlight_crack_04\0dmg_hdlight_smash_01\0dmg_hdlight_smash_02\0dmg_hdlight_smash_03\0dmg_hdlight_smash_04\0dmg_hdlight_smash_05",
                arrayIndex, arrayIndexBuf);
        } break;

        case CAR_SLOT_ID: {
            EditCAR_SLOT_ID(data, arrayIndex, arrayIndexBuf);
        } break;

        case CAR_PART_ID: {
            EditCAR_PART_ID(data, arrayIndex, arrayIndexBuf);
        } break;

        case EffectParticleConstraint: {
            EnumCombo(data, "CONSTRAIN_PARTICLE_NONE\0CONSTRAIN_PARTICLE_XY_AXIS\0CONSTRAIN_PARTICLE_XZ_AXIS\0CONSTRAIN_PARTICLE_YZ_AXIS\0CONSTRAIN_PARTICLE_SPRAY_SIDEWAYS\0CONSTRAIN_PARTICLE_SPRAY_HEADON",
                arrayIndex, arrayIndexBuf);
        }break;

        case EmitterShaderTechnique: {
            EnumCombo(data, "EmitterShaderTechnique_DEFAULT\0EmitterShaderTechnique_WATERSPLASH", arrayIndex, arrayIndexBuf);
        } break;

                        //CSys types, flag type structure
        case Type_pursuit_breaker_type: {
            EnumFlags(data, "Invalid_Type_pursuit_breaker_type\0Type_pursuit_breaker_type_generic\0Type_pursuit_breaker_type_lumbermill\0Type_pursuit_breaker_type_reservior\0Type_pursuit_breaker_type_metal_refinery\0Type_pursuit_breaker_type_quarry\0Type_pursuit_breaker_type_dam\0Type_pursuit_breaker_type_freight_yard\0Type_pursuit_breaker_type_monorail\0Type_pursuit_breaker_type_spillway\0Type_pursuit_breaker_type_construction_site_causeway\0Type_pursuit_breaker_type_rock_wall\0Type_pursuit_breaker_type_gas_station\0Type_pursuit_breaker_type_electrical_tower\0Type_pursuit_breaker_type_crane\0Type_pursuit_breaker_type_interstate_sign\0Type_pursuit_breaker_type_building\0Type_pursuit_breaker_type_heavy_machinery\0Type_pursuit_breaker_type_scaffolding\0Type_pursuit_breaker_type_semi_truck",
                arrayIndex, arrayIndexBuf);
        } break;

        case Type_car_type: {
            EnumFlags(data, "Invalid_Type_car_type\0Type_car_type_McLaren\0Type_car_type_Porsche\0Type_car_type_FordGT\0Type_car_type_Viper\0Type_car_type_AstonMartin\0Type_car_type_Corvette\0Type_car_type_BMW\0Type_car_type_Mercedes\0Type_car_type_Audi\0Type_car_type_Mitsubishi\0Type_car_type_Mustang\0Type_car_type_Challenger\0Type_car_type_Camaro\0Type_car_type_GTO\0Type_car_type_Nissan\0Type_car_type_Mazda\0Type_car_type_Lexus\0Type_car_type_Veyron\0Type_car_type_Volkswagen\0Type_car_type_CCX\0Type_car_type_Charger\0Type_car_type_Pagani\0Type_car_type_Lamborghini\0Type_car_type_Shelby\0Type_car_type_Chevelle\0Type_car_type_SportsCar\0Type_car_type_Supra\0Type_car_type_Cadillac\0Type_car_type_Cuda\0Type_car_type_Escort\0Type_car_type_Focus",
                arrayIndex, arrayIndexBuf);
        } break;

        case Type_vo_linenumber: {
            EnumFlags(data, "Invalid_Type_vo_linenumber\0Type_vo_linenumber_Line01\0Type_vo_linenumber_Line02\0Type_vo_linenumber_Line03\0Type_vo_linenumber_Line04\0Type_vo_linenumber_Line05\0Type_vo_linenumber_Line06\0Type_vo_linenumber_Line07\0Type_vo_linenumber_Line08\0Type_vo_linenumber_Line09\0Type_vo_linenumber_Line10\0Type_vo_linenumber_Line11\0Type_vo_linenumber_Line12\0Type_vo_linenumber_Line13\0Type_vo_linenumber_Line14\0Type_vo_linenumber_Line15\0Type_vo_linenumber_Line16\0Type_vo_linenumber_Line17\0Type_vo_linenumber_Line18\0Type_vo_linenumber_Line19\0Type_vo_linenumber_Line20\0Type_vo_linenumber_Line21\0Type_vo_linenumber_Line22\0Type_vo_linenumber_Line23\0Type_vo_linenumber_Line24\0Type_vo_linenumber_Line25\0Type_vo_linenumber_Line26\0Type_vo_linenumber_Line27\0Type_vo_linenumber_Line28\0Type_vo_linenumber_Line29\0Type_vo_linenumber_Line30\0Type_vo_linenumber_Line31",
                arrayIndex, arrayIndexBuf);
        } break;
#endif

        default:
            if (AttribTypes[def->mType] && AttribTypes[def->mType][11] == ':' && AttribTypes[def->mType][20] == 'f' && AttribTypes[def->mType][25] == '_'
                //&& strncmp("Attrib::Gen::ClassRefSpec_", AttribTypes[def->mType], 26) == 0
                ) {
                //dirty check for Attrib::Gen::ClassRefSpec_
                const char* RefSpecClassName = AttribTypes[def->mType] + 26;

                DWORD* collectionKey = &((DWORD*)data)[0];
                DWORD* collectionPtr = &((DWORD*)data)[1];

                ImGui::SetNextItemWidth(-FLT_MIN);
                float width = ImGui::CalcItemWidth();
                ImGui::SetNextItemWidth(width / 3);
                ImGui::Text("Class"); ImGui::SameLine(width / 3);
                ImGui::SetNextItemWidth(width / 3);
                ImGui::Text("Collection");
                static char collectionKeyBuf[100];

                if (GetCollectionName(getVltHash(RefSpecClassName), *collectionKey)) snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "%s", GetCollectionName(getVltHash(RefSpecClassName), *collectionKey));
                else snprintf(collectionKeyBuf, sizeof(collectionKeyBuf), "0x%08x", *collectionKey);

                ImGui::SetNextItemWidth(width / 3);
                ImGui::Text(RefSpecClassName);
                ImGui::PushID(2);
                ImGui::SameLine(width / 3);
                ImGui::SetNextItemWidth(width / 3);
                if (ImGui::InputText("", collectionKeyBuf, sizeof(collectionKeyBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (collectionKeyBuf[0] == '0' && collectionKeyBuf[1] == 'x') *collectionKey = strtoul(collectionKeyBuf, NULL, 16);
                    else *collectionKey = getVltHash(collectionKeyBuf);
                    *collectionPtr = (DWORD)Attrib::FindCollection(getVltHash(RefSpecClassName), *collectionKey, 0);
                    WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                    modscript << "Collection" << " " << collectionKeyBuf << "\n";
                }
                ImGui::PopID();
                ImGui::SetNextItemWidth(width / 3);
                ImGui::SameLine(2 * width / 3);
                if (ImGui::Button("Go to collection")) {
                    *nextCollection = Attrib::FindCollection(getVltHash(RefSpecClassName), *collectionKey, 0);
                }

            }
            else {
                supported = false;

                if (isInCurrentCollection) ImGui::PushStyleColor(ImGuiCol_FrameBg, colorNotSupported);
                if (def->mSize == 4) {
                    if (ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U32, (UINT32*)data, (const void*)1, (const void*)10, "%d", ImGuiInputTextFlags_AutoSelectAll)) {
                        WriteModscriptHeader(arrayIndex, arrayIndexBuf);
                        modscript << *(UINT32*)data << "\n";
                    }
                }
                else {
                    ImGui::Text("not supported yet - length %d pointer %08X", def->mSize, data);
                }
            }
        }
        //if (arrayIndex == -2 && !supported) ImGui::Text("not supported yet - length %d pointer %08X", def->mSize, data); //not sure how to show this easily...whatever
        if (!supported && isInCurrentCollection) ImGui::PopStyleColor();


        ImGui::TableNextColumn();

        if (!isInCurrentCollection) {
            //ImGui::SameLine();
            if (arrayIndex == -2) {
                DWORD key = def->mKey;
                ImGui::Text("Make local, size=");
                //int count = ((Attrib::Array*)Attrib::Collection::GetNode(currentCollection, 0, key, &key))->mSize;
                int count = Attrib::Attribute::GetLength(&attrib);
                ImGui::SameLine();
                ImGui::PushItemWidth(20.0f);
                if (ImGui::InputInt("##arraysize", &count, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue)) {

                    char* oldData = new char[count * def->mSize];
                    for (int i = 0; i < count; i++) {
                        auto src = (void*)Attrib::Collection::GetData(currentCollection, 0, def->mKey, i);
                        if (src != NULL) memcpy(oldData + def->mSize * i, src, def->mSize);
                    }

                    Attrib::Collection::AddAttribute(currentCollection, 0, def->mKey, count);
                    for (int i = 0; i < count; i++) {
                        auto dst = (void*)Attrib::Collection::GetData(currentCollection, 0, def->mKey, i);
                        if (dst != NULL) memcpy(dst, oldData + def->mSize * i, def->mSize);
                    }
                    delete[] oldData;
                    modscript << "add_field " << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << " " << count << "\n";
                }
                ImGui::PopItemWidth();
            }
            else
                if (!(def->mFlags & Attrib::Definition::Flags::kArray) && ImGui::Button("Make local")) {
                    void* dataptr_old = (void*)Attrib::Collection::GetData(currentCollection, 0, def->mKey, 0);
                    Attrib::Collection::AddAttribute(currentCollection, 0, def->mKey, 0);
                    auto dst = (void*)Attrib::Collection::GetData(currentCollection, 0, def->mKey, 0);
                    if (dst != NULL && dataptr_old != NULL) memcpy(dst, dataptr_old, def->mSize);
                    modscript << "add_field " << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << "\n";
                }
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }
        else if (!(def->mFlags & Attrib::Definition::Flags::kInLayout)) {
            if (ImGui::Button("Remove")) {
                Attrib::Collection::RemoveAttribute(currentCollection, 0, def->mKey);
                modscript << "delete_field " << CurrentClassName << " " << CurrentCollectionName << " " << CurrentAttributeName << "\n";
            }
            ImGui::SameLine();
        }


        //ImGui::SameLine();

        if (arrayIndex == -2) {
            if (AttribTypes[def->mType]) ImGui::Text("(array of %s)", AttribTypes[def->mType]);
            else ImGui::Text("(array of %08X)", def->mType);
            Attrib::Instance::destructor(&instance);
        }
        else {
            if (AttribTypes[def->mType]) ImGui::Text("(%s)", AttribTypes[def->mType]);
            else ImGui::Text("(%08X)", def->mType);
        }


        if (supported) modscript.flush();
    }




    void ShowEditAttribValue(Attrib::Collection* currentCollection, Attrib::Definition* def, Attrib::Collection** nextCollection) {
        if (currentCollection == NULL || def == NULL) return;

        DWORD key = def->mKey;


        if (HideHeritedAttributes && !(def->mFlags & Attrib::Definition::Flags::kInLayout) && !Attrib::Collection::Contains(currentCollection, 0, def->mKey)) {
            return;
        }

        //Attrib::Node* node = Attrib::Collection::GetNode(col, 0, key, &key);

        char attributeName[80];
        if (AttribFields[def->mKey] != NULL) {
            snprintf(attributeName, sizeof(attributeName), "%s", AttribFields[def->mKey]);
        }
        else {
            snprintf(attributeName, sizeof(attributeName), "0x%08x", def->mKey);
        }
		CurrentAttributeName = attributeName;

        //replicate what Attrib::Collection::GetData does - TODO? broken

        //TODO BROKEN
//        if (node != NULL) {
            //if (!(node->mFlags & Attrib::Node::Flags::NotAnArray)) {
//                if (node->mFlags & Attrib::Node::Flags::DirectPointer) {
//                    EditAttrib(node->mPtr, def);
            //	}
//                else if (node->mFlags & Attrib::Node::Flags::InLayout) {
//                    EditAttrib(col->mLayout + node->mPtr, def);
//                }
//                else if (node->mFlags & Attrib::Node::Flags::Static) {
//                    EditAttrib(col->mClass->mPrivates->mStaticData + node->mPtr, def);
//                }
//                else {
//                    EditAttrib(node->mPtr, def);
//                }
            //}
            //else {
//                //array
//                Attrib::Array* arr;
//                if (!(node->mFlags & Attrib::Node::Flags::InLayout)) {
//                    if (!(node->mFlags & Attrib::Node::Flags::Static)) {
//                        arr = (Attrib::Array*)node->mPtr;
            //		}
            //		else {
            //			arr = (Attrib::Array*)(col->mClass->mPrivates->mStaticData + node->mPtr);
            //		}
//                }
//                else {
            //		arr = (Attrib::Array*)(col->mLayout + node->mPtr);
//                }
//                if (ImGui::TreeNode(attributeName)) {
//                    for (int i = 0; i < arr->mCount; i++) {
//                        //EditAttrib((int)((arr + 1) + (arr->mEncodedTypeAndPad >> 0xc & 8) + (arr->mSize * i)), def, i);
//                        EditAttrib(Attrib::Collection::GetData(col, 0, def->mKey, i), def, i);
//                    }
//                    ImGui::TreePop();
//                }
            //}
//        }

        ImGui::PushID(def->mKey);

        if (def->mFlags & Attrib::Definition::Flags::kArray) {
            Attrib::Node* node = Attrib::Collection::GetNode(currentCollection, 0, key, &key);

            if (node != NULL) {
                Attrib::Array* arr;
                if (!(node->mFlags & Attrib::Node::Flags::InLayout)) {
                    if (!(node->mFlags & Attrib::Node::Flags::Static)) {
                        arr = (Attrib::Array*)node->mPtr;
                    }
                    else {
                        arr = (Attrib::Array*)(currentCollection->mClass->mPrivates->mStaticData + node->mPtr);
                    }
                }
                else {
                    arr = (Attrib::Array*)(currentCollection->mLayout + node->mPtr);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();

                bool treeNode = ImGui::TreeNode(attributeName);
                EditAttrib(Attrib::Collection::GetData(currentCollection, 0, def->mKey, 0), def, currentCollection, nextCollection, -2, false); //use this to display array info header
                if (arr != NULL && treeNode) {
                    for (int i = 0; i < arr->mCount; i++) {
                        ImGui::PushID(i);
                        //EditAttrib((int)((arr + 1) + (arr->mEncodedTypeAndPad >> 0xc & 8) + (arr->mSize * i)), def, i);
                        EditAttrib(Attrib::Collection::GetData(currentCollection, 0, def->mKey, i), def, currentCollection, nextCollection, i);
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
            }
            else {
                EditAttrib(NULL, def, currentCollection, nextCollection, -2);
            }


        }
        else {
            if (def->mFlags & Attrib::Definition::Flags::kInLayout) {
                //in layout, fix for frontend and emitterdata
                EditAttrib(currentCollection->mLayout + def->mOffset, def, currentCollection, nextCollection, -1);
            }
            else {
                //collection data
                EditAttrib(Attrib::Collection::GetData(currentCollection, 0, def->mKey, 0), def, currentCollection, nextCollection, -1); //GetData works for anything that's cool
            }
        }
        ImGui::PopID();
    }

constexpr auto PrevNextStackSize = 32;

    static Attrib::Collection* PrevNextStack[PrevNextStackSize];
    static UINT PrevNextStackPos = 0;

    void updatePrevNextStack(Attrib::Collection* col) {
        //if (!doAttribEditor) return;
        PrevNextStack[PrevNextStackPos] = col;
        PrevNextStackPos++;
        PrevNextStackPos %= PrevNextStackSize;
        PrevNextStack[PrevNextStackPos] = NULL;
    }

    void DisplayChildrenRecursive(Attrib::Class* clazz, Attrib::Collection* parent, Attrib::Collection** selected) {

        int NameDefOffset = -1;

        //slower method for cases where CollectionName exists but attrib cannot find it for some fucking reason
        for (int i = 0; i < clazz->mPrivates->mNumDefinitions; i++) {
            if (clazz->mPrivates->mDefinitions[i].mKey == CollectionName && clazz->mPrivates->mDefinitions[i].mFlags & Attrib::Definition::Flags::kInLayout) {
                NameDefOffset = clazz->mPrivates->mDefinitions[i].mOffset;
                break;
            }
        }

        for (int i = 0; i < clazz->mPrivates->mCollections.__inherit_0.mTableSize; i++) {
            auto chi = Attrib::FindCollection(clazz->mKey, clazz->mPrivates->mCollections.__inherit_0.mTable[i].mKey, NULL);
            if (chi != NULL && chi->mParent == parent) {
                char buf[64];
                char** colName;

                if (NameDefOffset != -1) colName = (char**)(chi->mLayout + NameDefOffset);
                else (colName = (char**)Attrib::Collection::GetData(chi, 0, CollectionName, 0));

                if (colName && *colName && **colName) {
                    snprintf(buf, sizeof(buf), *colName);
                }
                else if (AttribCollections[chi->mKey] != NULL) {
                    snprintf(buf, sizeof(buf), "%s", AttribCollections[chi->mKey]);
                }
                else {
                    snprintf(buf, sizeof(buf), "0x%08x", chi->mKey);
                }


                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_None;
                tree_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick; // Standard opening mode as we are likely to want to add selection afterwards
                tree_flags |= ImGuiTreeNodeFlags_NavLeftJumpsToParent;  // Left arrow support
                tree_flags |= ImGuiTreeNodeFlags_SpanFullWidth;         // Span full width for easier mouse reach
                tree_flags |= ImGuiTreeNodeFlags_DrawLinesToNodes;      // Always draw hierarchy outlines
                if (chi == *selected)
                    tree_flags |= ImGuiTreeNodeFlags_Selected;          // Draw selection highlight
                //test for children
                bool isLeaf = true;
                for (int i = 0; i < clazz->mPrivates->mCollections.__inherit_0.mTableSize; i++) {
                    auto chichi = Attrib::FindCollection(clazz->mKey, clazz->mPrivates->mCollections.__inherit_0.mTable[i].mKey, NULL);
                    if (chichi != NULL && chichi->mParent == chi) {
                        isLeaf = false;
                        break;
                    }
                }
                if (isLeaf) {
                    tree_flags |= ImGuiTreeNodeFlags_Leaf;
                    //tree_flags |= ImGuiTreeNodeFlags_Bullet;
                }

                //if (node->Childs.Size == 0)
                //    tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen; // Use _NoTreePushOnOpen + set is_open=false to avoid unnecessarily push/pop on leaves.
                //if (node->DataMyBool == false)
                //    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                int id = chi->mKey + clazz->mKey;
                ImGui::SetNextItemStorageID((ImGuiID)id);        // Use node->UID as storage id
                if (ImGui::TreeNodeEx((void*)(intptr_t)id, tree_flags, buf)) {
                    DisplayChildrenRecursive(clazz, chi, selected);
                    ImGui::TreePop();
                }
                //if (ImGui::TreeNode(buf)) {
                //    DisplayChildrenRecursive(clazz, chi, selected);
                //    ImGui::TreePop();
                //}

                //if (node->Childs.Size == 0)
                //    is_open = false;
                //if (node->DataMyBool == false)
                //    ImGui::PopStyleColor();
                if (ImGui::IsItemFocused()) {
                    updatePrevNextStack(*selected);
                    *selected = chi;
                    //ImGui::TableNextRow();
                    //ImGui::TableNextColumn();
                    ImGui::SameLine(); ImGui::TextUnformatted(" "); //ugly selection fix                        
                }

            }
        }

    }

    void EditCollection(Attrib::Collection*& col)
    {
        if (col != NULL) {
            if (!modscriptInitialized) initializeModscript();

            static char classBuf[64], collBuf[100];

            if (AttribClasses[col->mClass->mKey] != NULL) snprintf(classBuf, sizeof(classBuf), "%s", AttribClasses[col->mClass->mKey]);
            else snprintf(classBuf, sizeof(classBuf), "0x%08x", col->mClass->mKey);
			CurrentClassName = classBuf;

            //if (AttribCollections[col->mKey] != NULL) snprintf(collBuf, sizeof(collBuf), "%s", AttribCollections[col->mKey]);
            //else snprintf(collBuf, sizeof(collBuf), "0x%08x", col->mKey);

            char** colName;
            if ((colName = (char**)Attrib::Collection::GetData(col, 0, CollectionName, 0)) && *colName && **colName) {
                snprintf(collBuf, sizeof(collBuf), *colName);
            }
            else if (AttribCollections[col->mKey] != NULL) snprintf(collBuf, sizeof(collBuf), "%s", AttribCollections[col->mKey]);
            else snprintf(collBuf, sizeof(collBuf), "0x%08x", col->mKey);
			CurrentCollectionName = collBuf;


            if (col->mClass->mPrivates->mNumDefinitions > 0) {
                Attrib::Definition* defs = col->mClass->mPrivates->mDefinitions;
                int uid = col->mKey + col->mClass->mKey;

                Attrib::Collection* nextCollection = NULL;

                if (ImGui::BeginTable("##properties", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersH)) {

                    // Push object ID after we entered the table, so table is shared for all objects
                    //ImGui::PushID(uid + 1);
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f); // Default twice larger
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);

                    ImGui::TableNextRow(); //TODO
                    //ImGui::PushID(field_desc.Name);
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    if (ImGui::TreeNodeEx("Static", ImGuiTreeNodeFlags_DefaultOpen)) {
                        for (int i = 0; i < col->mClass->mPrivates->mNumDefinitions; i++) {
                            if (defs[i].mFlags & Attrib::Definition::Flags::kIsStatic) {
                                ShowEditAttribValue(col, &defs[i], &nextCollection);
                            }
                        }
                        ImGui::TreePop();
                    }

                    ImGui::TableNextRow(); //TODO
                    //ImGui::PushID(field_desc.Name);
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    if (ImGui::TreeNodeEx("Layout", ImGuiTreeNodeFlags_DefaultOpen)) {
                        for (int i = 0; i < col->mClass->mPrivates->mNumDefinitions; i++) {
                            if (defs[i].mFlags & Attrib::Definition::Flags::kInLayout) {
                                ShowEditAttribValue(col, &defs[i], &nextCollection);
                            }
                        }
                        ImGui::TreePop();
                    }

                    ImGui::TableNextRow(); //TODO
                    //ImGui::PushID(field_desc.Name);
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    if (ImGui::TreeNodeEx("Collection", ImGuiTreeNodeFlags_DefaultOpen)) {
                        for (int i = 0; i < col->mClass->mPrivates->mNumDefinitions; i++) {
                            if (!(defs[i].mFlags & Attrib::Definition::Flags::kInLayout) && !(defs[i].mFlags & Attrib::Definition::Flags::kIsStatic)) {
                                ShowEditAttribValue(col, &defs[i], &nextCollection);
                            }
                        }
                        ImGui::TreePop();
                    }

                    //ImGui::PopID();
                    ImGui::EndTable();
                }

                if (nextCollection != NULL) {
                    updatePrevNextStack(col);
                    col = nextCollection;
                }
            }

        }
    }

    void ShowAttribEditor(bool* doAttribEditor) {
        static Attrib::Collection* col = NULL; //selected
        static char FilterBuf[100];

        char windowTitle[128];
        if (col != NULL) {
            if (AttribClasses[col->mClass->mKey]) {
				if (GetCollectionName(col->mClass->mKey, col->mKey)) snprintf(windowTitle, sizeof(windowTitle), "Attrib editor - %s - %s###Attrib editor", AttribClasses[col->mClass->mKey], GetCollectionName(col->mClass->mKey, col->mKey));
				else snprintf(windowTitle, sizeof(windowTitle), "Attrib editor - %s - 0x%08x###Attrib editor", AttribClasses[col->mClass->mKey], col->mKey);
            }
            else {
                if (GetCollectionName(col->mClass->mKey, col->mKey)) snprintf(windowTitle, sizeof(windowTitle), "Attrib editor - 0x%08x - %s###Attrib editor", col->mClass->mKey, GetCollectionName(col->mClass->mKey, col->mKey));
                else snprintf(windowTitle, sizeof(windowTitle), "Attrib editor - 0x%08x - 0x%08x###Attrib editor", col->mClass->mKey, col->mKey);
            }
        }
		else snprintf(windowTitle, sizeof(windowTitle), "Attrib editor###Attrib editor");

        if (ImGui::Begin(windowTitle, doAttribEditor)) {
            Attrib::ClassTable__Node* classTable = *(Attrib::ClassTable__Node**)(*(DWORD*)(*Attrib::DatabasePrivate + 0x4) + 0x8);
            auto classTableCount = (UINT16*)(*(DWORD*)(*Attrib::DatabasePrivate + 0x4) + 0x8 + 0x4); //0x4 TableSize, 0x6 NumEntries


            //ImGui::Text("class table at %08X", classTable);
            //ImGui::Text("class table count at %08X", classTableCount);
            //if (classTableCount != NULL) ImGui::Text("class table count %d", *classTableCount);

            // Left side: draw tree
            // - Currently using a table to benefit from RowBg feature
            // - Our tree node are all of equal height, facilitating the use of a clipper.
            if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
            {
                ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
                //ImGui::Checkbox("Use Clipper", &UseClipper);
                //ImGui::SameLine();
                //ImGui::Text("(%d root nodes)", root_node->Childs.Size);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F, ImGuiInputFlags_Tooltip);
                ImGui::InputTextWithHint("##Filter", "class name", FilterBuf, IM_COUNTOF(FilterBuf), ImGuiInputTextFlags_EscapeClearsAll);
                //if ()
                    //Filter.Build();
                ImGui::PopItemFlag();

                /* { //broken, i'm not debugging that shit rn
                    bool canDoPrev = PrevNextStack[(PrevNextStackPos - 1) % PrevNextStackSize] != NULL;
                    bool canDoNext = PrevNextStack[(PrevNextStackPos + 1) % PrevNextStackSize] != NULL;
                    if (!canDoPrev) ImGui::BeginDisabled();
                    float width = ImGui::CalcItemWidth();
                    ImGui::SetNextItemWidth(width / 2);
                    if (ImGui::Button("Previous") && canDoPrev) {
                        PrevNextStackPos = (PrevNextStackPos - 1) % PrevNextStackSize;
                        col = PrevNextStack[PrevNextStackPos];
                    }
                    if (!canDoPrev) ImGui::EndDisabled();

                    ImGui::SameLine();
                    if (!canDoNext) ImGui::BeginDisabled();
                    ImGui::SetNextItemWidth(width / 2);
                    if (ImGui::Button("Next") && canDoNext) {
                        PrevNextStackPos = (PrevNextStackPos + 1) % PrevNextStackSize;
                        col = PrevNextStack[PrevNextStackPos];
                    }
                    if (!canDoNext) ImGui::EndDisabled();

                    if (canDoPrev) ImGui::Text("Previous 0x%08x", PrevNextStack[(PrevNextStackPos - 1) % PrevNextStackSize]->mKey);
                    if (canDoNext) ImGui::Text("Next 0x%08x", PrevNextStack[(PrevNextStackPos + 1) % PrevNextStackSize]->mKey);
                }*/

                if (ImGui::BeginTable("##list", 1, ImGuiTableFlags_RowBg)) {
                    if (classTable != NULL) {
                        for (int i = 0; i < *classTableCount; i++) {
                            //char buf[100];
                            //sprintf_s(buf, 99, "class 0x%08x %s at %08X (node at %08X)", classTable[i].mKey, AttribClasses[classTable[i].mKey] != NULL ? AttribClasses[classTable[i].mKey] : "", & classTable[i].mPtr, & classTable[i]);

                            if (classTable[i].mPtr != NULL && classTable[i].mKey != 0) {
                                char buf[64];
                                if (AttribClasses[classTable[i].mKey] != NULL) {
                                    snprintf(buf, sizeof(buf), "%s", AttribClasses[classTable[i].mKey]);
                                }
                                else {
                                    snprintf(buf, sizeof(buf), "0x%08x", classTable[i].mKey);
                                }

                                if (FilterBuf[0] != 0) {
                                    int length = 0;
                                    while (FilterBuf[length] != 0) length++;
                                    if (strncmp(FilterBuf, buf, length)) continue;
                                }


                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_None;
                                tree_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick; // Standard opening mode as we are likely to want to add selection afterwards
                                tree_flags |= ImGuiTreeNodeFlags_NavLeftJumpsToParent;  // Left arrow support
                                tree_flags |= ImGuiTreeNodeFlags_SpanFullWidth;         // Span full width for easier mouse reach
                                tree_flags |= ImGuiTreeNodeFlags_DrawLinesToNodes;      // Always draw hierarchy outlines
                                int id = classTable[i].mKey;
                                ImGui::SetNextItemStorageID((ImGuiID)id);        // Use node->UID as storage id
                                if (ImGui::TreeNodeEx((void*)(intptr_t)id, tree_flags, buf)) {
                                    DisplayChildrenRecursive(classTable[i].mPtr, NULL, &col);
                                    ImGui::TreePop();
                                }
                            }
                        }
                    }

                    //if (UseClipper)
                    //    DrawClippedTree(root_node);
                    //else
                    //    DrawTree(root_node);
                    ImGui::EndTable();
                }
            }
            ImGui::EndChild();

            // Right side: draw properties
            ImGui::SameLine();

            if (ImGui::BeginChild("##editor", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
            {

                ImGui::BeginGroup(); // Lock X position

                ImGui::Text("Warning: this may cause instabilities and unwanted changes!");

                ImGui::SameLine(); ImGui::Checkbox("Hide inherited attributes", &HideHeritedAttributes);
                ImGui::SameLine(); ImGui::Checkbox("Hide empty attributes", &HideEmptyAttributes);

                if (ImGui::TreeNode("Go to...")) {
                    static char gotoclassBuf[64], gotocollBuf[100];
                    static DWORD classKey, collKey;

                    if (ImGui::InputText("ClassKey", gotoclassBuf, 64, ImGuiInputTextFlags_AutoSelectAll)) {
                        if (gotoclassBuf[0] == '0' && gotoclassBuf[1] == 'x') classKey = strtoul(gotoclassBuf, NULL, 16);
                        else classKey = getVltHash(gotoclassBuf);
                        updatePrevNextStack(col);
                        col = Attrib::FindCollection(classKey, collKey, NULL);
                    }
                    if (ImGui::InputText("CollectionKey", gotocollBuf, 64, ImGuiInputTextFlags_AutoSelectAll)) {
                        if (gotoclassBuf[0] == '0' && gotoclassBuf[1] == 'x') collKey = strtoul(gotocollBuf, NULL, 16);
                        else collKey = getVltHash(gotocollBuf);
                        updatePrevNextStack(col);
                        col = Attrib::FindCollection(classKey, collKey, NULL);
                    }

                    ImGui::Separator();
                    ImGui::TreePop();
                }

                EditCollection(col);





                //if (ExampleTreeNode* node = SelectedNode)
                //{
                //    ImGui::Text("%s", node->Name);
                //    ImGui::TextDisabled("UID: 0x%08X", node->UID);
                //    ImGui::Separator();
                //    if (ImGui::BeginTable("##properties", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
                //    {
                //        // Push object ID after we entered the table, so table is shared for all objects
                //        ImGui::PushID((int)node->UID);
                //        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                //        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f); // Default twice larger
                //        if (node->HasData)
                //        {
                //            // In a typical application, the structure description would be derived from a data-driven system.
                //            // - We try to mimic this with our ExampleMemberInfo structure and the ExampleTreeNodeMemberInfos[] array.
                //            // - Limits and some details are hard-coded to simplify the demo.
                //            for (const ExampleMemberInfo& field_desc : ExampleTreeNodeMemberInfos)
                //            {
                //                ImGui::TableNextRow();
                //                ImGui::PushID(field_desc.Name);
                //                ImGui::TableNextColumn();
                //                ImGui::AlignTextToFramePadding();
                //                ImGui::TextUnformatted(field_desc.Name);
                //                ImGui::TableNextColumn();
                //                void* field_ptr = (void*)(((unsigned char*)node) + field_desc.Offset);
                //                switch (field_desc.DataType)
                //                {
                //                case ImGuiDataType_Bool:
                //                {
                //                    IM_ASSERT(field_desc.DataCount == 1);
                //                    ImGui::Checkbox("##Editor", (bool*)field_ptr);
                //                    break;
                //                }
                //                case ImGuiDataType_S32:
                //                {
                //                    int v_min = INT_MIN, v_max = INT_MAX;
                //                    ImGui::SetNextItemWidth(-FLT_MIN);
                //                    ImGui::DragScalarN("##Editor", field_desc.DataType, field_ptr, field_desc.DataCount, 1.0f, &v_min, &v_max);
                //                    break;
                //                }
                //                case ImGuiDataType_Float:
                //                {
                //                    float v_min = 0.0f, v_max = 1.0f;
                //                    ImGui::SetNextItemWidth(-FLT_MIN);
                //                    ImGui::SliderScalarN("##Editor", field_desc.DataType, field_ptr, field_desc.DataCount, &v_min, &v_max);
                //                    break;
                //                }
                //                case ImGuiDataType_String:
                //                {
                //                    ImGui::InputText("##Editor", reinterpret_cast<char*>(field_ptr), 28);
                //                    break;
                //                }
                //                }
                //                ImGui::PopID();
                //            }
                //        }
                //        ImGui::PopID();
                //        ImGui::EndTable();
                //    }
                //}
                ImGui::EndGroup();

            }
            ImGui::EndChild();













        }
        ImGui::End();
    }

}
