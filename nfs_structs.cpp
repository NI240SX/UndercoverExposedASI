#include "nfs_structs.h"
#include <stdio.h>

//declare functions from nfs_structs.h
Attrib::Definition* (__cdecl* Attrib::Class::GetDefinition)(Attrib::Definition* defStart, Attrib::Definition* defEnd, Attrib::Definition* match);

void(__fastcall* Attrib::Instance::constructor)(Attrib::InstanceBase* this_, DWORD edx, Attrib::Collection* node, DWORD stuff);
void(__fastcall* Attrib::Instance::destructor)(Attrib::InstanceBase* this_);
void(__fastcall* Attrib::Instance::Get)(Attrib::InstanceBase* this_, DWORD unaff, Attrib::Attribute*, DWORD);
void* (__fastcall* Attrib::Instance::GetAttributePointer)(Attrib::InstanceBase* this_, DWORD edx, DWORD key, DWORD stuff);
DWORD(__fastcall* Attrib::Instance::GetCollectionKey)(Attrib::Instance* this_);
DWORD* (__fastcall* Attrib::Instance::GetRaceTracks)(Attrib::InstanceBase*, DWORD edx, DWORD);
bool(__fastcall* Attrib::Instance::Contains)(Attrib::InstanceBase* this_, DWORD edx, DWORD key);

void(__fastcall* Attrib::Gen::frontend::constructor)(Attrib::Gen::frontend* this_, DWORD unaff, Attrib::RefSpec*, DWORD);
void(__fastcall* Attrib::Gen::pvehicle::constructor)(Attrib::Gen::pvehicle* this_, DWORD unaff, DWORD, DWORD);
void(__fastcall* Attrib::Gen::zone_visual_style::constructor)(Attrib::Gen::zone_visual_style* this_, DWORD edx, DWORD node, DWORD stuff);
void(__fastcall* Attrib::Gen::presetride::constructor)(Attrib::Gen::presetride* this_, DWORD unaff, DWORD, DWORD);
void(__fastcall* Attrib::Gen::car_list::constructor)(Attrib::Gen::car_list* this_, DWORD unaff, DWORD, DWORD);

Attrib::Attribute* (__fastcall* Attrib::Collection::Get)(Attrib::Collection* this_, DWORD edx, Attrib::Attribute* storage, Attrib::Instance* inst, DWORD key);
int(__fastcall* Attrib::Collection::GetData)(Attrib::Collection* this_, DWORD edx, DWORD key, DWORD index); //actually void* but well
Attrib::Node* (__fastcall* Attrib::Collection::GetNode)(Attrib::Collection* this_, DWORD edx, DWORD key, DWORD* keyptr);
bool(__fastcall* Attrib::Collection::Contains)(Attrib::Collection* this_, DWORD edx, DWORD key);
bool(__fastcall* Attrib::Collection::AddAttribute)(Attrib::Collection* this_, DWORD edx, DWORD key, DWORD arraySize);
bool(__fastcall* Attrib::Collection::RemoveAttribute)(Attrib::Collection* this_, DWORD edx, DWORD key);

void* (__fastcall* Attrib::Attribute::GetInternalPointer)(Attrib::Attribute* this_, DWORD unaff, DWORD);
bool(__fastcall* Attrib::Attribute::SetLength)(Attrib::Attribute* this_, DWORD edx, DWORD length);
int(__fastcall* Attrib::Attribute::GetLength)(Attrib::Attribute* this_);

void(__fastcall* Attrib::RefSpec::constructor)(Attrib::RefSpec* this_, DWORD unaff, Attrib::RefSpec*);
void(__fastcall* Attrib::RefSpec::Clean)(Attrib::RefSpec* this_);
//Attrib::Class* (__fastcall* Attrib::Database::GetClass)(DWORD Database, DWORD edx, DWORD classKey);

void* (__cdecl* Attrib::DefaultDataArea)(DWORD length);
Attrib::Collection* (__cdecl* Attrib::FindCollection)(DWORD classKey, DWORD collectionKey, char* collectionName);
Attrib::Class* (__fastcall* Attrib::Database::GetClass)(DWORD Database, DWORD edx, DWORD classKey);
bool(__fastcall* Attrib::AddAttribute)(Attrib::Collection* this_, DWORD edx, DWORD key, DWORD arraySizeMaybe);

DWORD* Attrib::DatabasePrivate;

void nfs_structs::Register(int GAME_VERSION) {
    switch (GAME_VERSION) {
    case UC_1_21:
        Attrib::Database::GetClass = reinterpret_cast <decltype(Attrib::Database::GetClass)>(0x0045e610);
        Attrib::Class::GetDefinition = (Attrib::Definition * (__cdecl*)(Attrib::Definition * defStart, Attrib::Definition * defEnd, Attrib::Definition * match))0x008e5d10;
        Attrib::Instance::constructor = reinterpret_cast<decltype(Attrib::Instance::constructor)>(0x0045cb40);
        Attrib::Instance::destructor = reinterpret_cast<decltype(Attrib::Instance::destructor)>(0x00462550);
        Attrib::Instance::Get = reinterpret_cast<decltype(Attrib::Instance::Get)>(0x004609a0);
        Attrib::Instance::GetAttributePointer = (void* (__fastcall*)(Attrib::InstanceBase * this_, DWORD edx, DWORD key, DWORD stuff))0x0045e7d0;
        Attrib::Instance::GetCollectionKey = reinterpret_cast<decltype(Attrib::Instance::GetCollectionKey)>(0x0045cbe0);
        Attrib::Instance::GetRaceTracks = reinterpret_cast<decltype(Attrib::Instance::GetRaceTracks)>(0x00463b70);
        Attrib::Instance::Contains = (bool(__fastcall*)(Attrib::InstanceBase * this_, DWORD edx, DWORD key))0x0045d840;
        //Attrib::AddAttribute = (bool(__fastcall*)(Attrib::Collection * this_, DWORD edx, DWORD key, DWORD arraySizeMaybe))0x0045fcd0;
        Attrib::Collection::Get = reinterpret_cast<decltype(Attrib::Collection::Get)>(0x0045fc60);
        Attrib::Collection::GetData = (int(__fastcall*)(Attrib::Collection * this_, DWORD edx, DWORD key, DWORD index))0x0045e4a0;
        Attrib::Collection::GetNode = reinterpret_cast<decltype(Attrib::Collection::GetNode)>(0x0045d510);
        Attrib::Collection::Contains = reinterpret_cast<decltype(Attrib::Collection::Contains)>(0x0045d5b0);
        Attrib::Collection::AddAttribute = reinterpret_cast<decltype(Attrib::Collection::AddAttribute)>(0x0045fcd0);
        Attrib::Collection::RemoveAttribute = reinterpret_cast<decltype(Attrib::Collection::RemoveAttribute)>(0x00461420);
        Attrib::Attribute::SetLength = reinterpret_cast<decltype(Attrib::Attribute::SetLength)>(0x00461aa0);
        Attrib::Attribute::GetLength = reinterpret_cast<decltype(Attrib::Attribute::GetLength)>(0x0045ea80);
        Attrib::Attribute::GetInternalPointer = reinterpret_cast<decltype(Attrib::Attribute::GetInternalPointer)>(0x0045eaa0);
        Attrib::RefSpec::constructor = reinterpret_cast<decltype(Attrib::RefSpec::constructor)>(0x0045ced0);
        Attrib::RefSpec::Clean = reinterpret_cast<decltype(Attrib::RefSpec::Clean)>(0x004628f0);
        Attrib::Gen::pvehicle::constructor = reinterpret_cast<decltype(Attrib::Gen::pvehicle::constructor)>(0x00451d40);
        Attrib::Gen::frontend::constructor = reinterpret_cast<decltype(Attrib::Gen::frontend::constructor)>(0x005019b0);
        Attrib::Gen::zone_visual_style::constructor = (void(__fastcall*)(Attrib::Gen::zone_visual_style * this_, DWORD edx, DWORD node, DWORD stuff))0x00822a40;
        Attrib::Gen::presetride::constructor = reinterpret_cast<decltype(Attrib::Gen::presetride::constructor)>(0x0040e760);
        Attrib::Gen::car_list::constructor = reinterpret_cast<decltype(Attrib::Gen::car_list::constructor)>(0x00506a80);
        Attrib::FindCollection = (Attrib::Collection * (__cdecl*)(DWORD clazz, DWORD node, char* nodeName))0x0045e840;
        Attrib::DefaultDataArea = (void* (__cdecl*)(DWORD length))0x0065abd0;


        Attrib::DatabasePrivate = (DWORD*)0x01293d90;
        break;
    case UC_1_0:
        Attrib::Database::GetClass = reinterpret_cast <decltype(Attrib::Database::GetClass)>(0x004647f0);
        Attrib::Class::GetDefinition = (Attrib::Definition * (__cdecl*)(Attrib::Definition * defStart, Attrib::Definition * defEnd, Attrib::Definition * match))0x009186f0;
        Attrib::Instance::constructor = reinterpret_cast<decltype(Attrib::Instance::constructor)>(0x00462c80);
        Attrib::Instance::destructor = reinterpret_cast<decltype(Attrib::Instance::destructor)>(0x00468820);
        Attrib::Instance::Get = reinterpret_cast<decltype(Attrib::Instance::Get)>(0x00466bb0);
        Attrib::Instance::GetAttributePointer = (void* (__fastcall*)(Attrib::InstanceBase * this_, DWORD edx, DWORD key, DWORD stuff))0x004649b0;
        Attrib::Instance::GetCollectionKey = reinterpret_cast<decltype(Attrib::Instance::GetCollectionKey)>(0x00462d20);
        Attrib::Instance::GetRaceTracks = reinterpret_cast<decltype(Attrib::Instance::GetRaceTracks)>(0x00469f30);
        Attrib::Instance::Contains = (bool(__fastcall*)(Attrib::InstanceBase * this_, DWORD edx, DWORD key))0x004639f0;
        Attrib::Collection::Get = reinterpret_cast<decltype(Attrib::Collection::Get)>(0x00465e70);
        Attrib::Collection::GetData = (int(__fastcall*)(Attrib::Collection * this_, DWORD edx, DWORD key, DWORD index))0x00464680;
        Attrib::Collection::GetNode = reinterpret_cast<decltype(Attrib::Collection::GetNode)>(0x004636d0);
        Attrib::Collection::Contains = reinterpret_cast<decltype(Attrib::Collection::Contains)>(0x00463770);
        Attrib::Collection::AddAttribute = reinterpret_cast<decltype(Attrib::Collection::AddAttribute)>(0x00465ee0);
        Attrib::Collection::RemoveAttribute = reinterpret_cast<decltype(Attrib::Collection::RemoveAttribute)>(0x00467600);
        Attrib::Attribute::SetLength = reinterpret_cast<decltype(Attrib::Attribute::SetLength)>(0x00467c90);
        Attrib::Attribute::GetLength = reinterpret_cast<decltype(Attrib::Attribute::GetLength)>(0x00464c60);
        Attrib::Attribute::GetInternalPointer = reinterpret_cast<decltype(Attrib::Attribute::GetInternalPointer)>(0x00464c80);
        Attrib::RefSpec::constructor = reinterpret_cast<decltype(Attrib::RefSpec::constructor)>(0x00463020);
        Attrib::RefSpec::Clean = reinterpret_cast<decltype(Attrib::RefSpec::Clean)>(0x00468bc0);
        Attrib::Gen::pvehicle::constructor = reinterpret_cast<decltype(Attrib::Gen::pvehicle::constructor)>(0x004574e0);
        Attrib::Gen::frontend::constructor = reinterpret_cast<decltype(Attrib::Gen::frontend::constructor)>(0x00466b20);
        Attrib::Gen::zone_visual_style::constructor = (void(__fastcall*)(Attrib::Gen::zone_visual_style * this_, DWORD edx, DWORD node, DWORD stuff))0x0084f1b0;
        Attrib::Gen::presetride::constructor = reinterpret_cast<decltype(Attrib::Gen::presetride::constructor)>(0x00410720);
        Attrib::Gen::car_list::constructor = reinterpret_cast<decltype(Attrib::Gen::car_list::constructor)>(0x0051cd90);
        Attrib::FindCollection = (Attrib::Collection * (__cdecl*)(DWORD clazz, DWORD node, char* nodeName))0x00464a20;
        Attrib::DefaultDataArea = (void* (__cdecl*)(DWORD length))0x00676490;

        Attrib::DatabasePrivate = (DWORD*)0x00d834d0;
        break;

    }
}

void Attrib::Gen::generic::constructor(Attrib::InstanceBase* this_, DWORD clazz, DWORD node) {
    auto inst = (Attrib::Instance*)this_;
    Attrib::Instance::constructor(inst, 0, Attrib::FindCollection(clazz, node, NULL), NULL);
    if (inst->mLayoutPtr == NULL) {
        inst->mLayoutPtr = Attrib::DefaultDataArea(0); //0 should be fineeeee
    }
}