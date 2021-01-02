//
//  LevelFormationConfigurationBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/8/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelFormationConfigurationBlueprint.hpp"
#include "core_includes.h"
#include "LevelPath.hpp"
#include "LevelFormationConfiguration.hpp"

LevelFormationConfigurationBlueprint::LevelFormationConfigurationBlueprint() {
    mTracerSpeedGlobal = SPEED_CLASS_DEFAULT;
    mTracerSpeed1 = SPEED_CLASS_DEFAULT;
    mTracerSpeed2 = SPEED_CLASS_DEFAULT;
    mTracerSpeed3 = SPEED_CLASS_DEFAULT;
    mTracerSpeed4 = SPEED_CLASS_DEFAULT;
    mTracerSpeed5 = SPEED_CLASS_DEFAULT;
    mTracerSpeed6 = SPEED_CLASS_DEFAULT;
    mTracerSpeed7 = SPEED_CLASS_DEFAULT;
    mTracerSpeed8 = SPEED_CLASS_DEFAULT;
    
    mTracerReverseGlobal = false;
    mTracerReverse1 = false;
    mTracerReverse2 = false;
    mTracerReverse3 = false;
    mTracerReverse4 = false;
    mTracerReverse5 = false;
    mTracerReverse6 = false;
    mTracerReverse7 = false;
    mTracerReverse8 = false;
    
    mTracerPlayOnEnterGlobal = false;
    mTracerPlayOnExitGlobal = false;
    
    mTracerOffsetGlobal = 0;
    mTracerOffset1 = 0;
    mTracerOffset2 = 0;
    mTracerOffset3 = 0;
    mTracerOffset4 = 0;
    mTracerOffset5 = 0;
    mTracerOffset6 = 0;
    mTracerOffset7 = 0;
    mTracerOffset8 = 0;
    
    
}

LevelFormationConfigurationBlueprint::~LevelFormationConfigurationBlueprint() {
    
}

void LevelFormationConfigurationBlueprint::Reset() {
    
}

bool LevelFormationConfigurationBlueprint::ShouldSave() {
    
    if (mTracerSpeedGlobal != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerOffsetGlobal != 0) { return true; }
    if (mTracerReverseGlobal == true) { return true; }
    
    if (mTracerSpeed1 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed2 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed3 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed4 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed5 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed6 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed7 != SPEED_CLASS_DEFAULT) { return true; }
    if (mTracerSpeed8 != SPEED_CLASS_DEFAULT) { return true; }
    
    if (mTracerOffset1 != 0) { return true; }
    if (mTracerOffset2 != 0) { return true; }
    if (mTracerOffset3 != 0) { return true; }
    if (mTracerOffset4 != 0) { return true; }
    if (mTracerOffset5 != 0) { return true; }
    if (mTracerOffset6 != 0) { return true; }
    if (mTracerOffset7 != 0) { return true; }
    if (mTracerOffset8 != 0) { return true; }
    
    if (mTracerReverse1 != false) { return true; }
    if (mTracerReverse2 != false) { return true; }
    if (mTracerReverse3 != false) { return true; }
    if (mTracerReverse4 != false) { return true; }
    if (mTracerReverse5 != false) { return true; }
    if (mTracerReverse6 != false) { return true; }
    if (mTracerReverse7 != false) { return true; }
    if (mTracerReverse8 != false) { return true; }
    
    if (mTracerPlayOnEnterGlobal == true) { return true; }
    if (mTracerPlayOnExitGlobal == true) { return true; }
    
    return false;
}

FJSONNode *LevelFormationConfigurationBlueprint::Save() {
    FJSONNode *aExport = new FJSONNode();
    aExport->mNodeType = JSON_NODE_TYPE_DICTIONARY;
    
    if (mTracerOffsetGlobal != 0) {
        aExport->AddDictionaryInt("tracer_offset_global", mTracerOffsetGlobal);
    } else {
        if (mTracerSpeed1 != 0) {
            aExport->AddDictionaryInt("tracer_offset_1", mTracerOffset1);
        }
        if (mTracerSpeed2 != 0) {
            aExport->AddDictionaryInt("tracer_offset_2", mTracerOffset2);
        }
        if (mTracerSpeed3 != 0) {
            aExport->AddDictionaryInt("tracer_offset_3", mTracerOffset3);
        }
        if (mTracerSpeed4 != 0) {
            aExport->AddDictionaryInt("tracer_offset_4", mTracerOffset4);
        }
        if (mTracerSpeed5 != 0) {
            aExport->AddDictionaryInt("tracer_offset_5", mTracerOffset5);
        }
        if (mTracerSpeed6 != 0) {
            aExport->AddDictionaryInt("tracer_offset_6", mTracerOffset6);
        }
        if (mTracerSpeed7 != 0) {
            aExport->AddDictionaryInt("tracer_offset_7", mTracerOffset7);
        }
        if (mTracerSpeed8 != 0) {
            aExport->AddDictionaryInt("tracer_offset_8", mTracerOffset8);
        }
    }
    
    
    if (mTracerSpeedGlobal != SPEED_CLASS_DEFAULT) {
        aExport->AddDictionaryInt("tracer_speed_class_global", mTracerSpeedGlobal);
    } else {
        if (mTracerSpeed1 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_1", mTracerSpeed1);
        }
        if (mTracerSpeed2 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_2", mTracerSpeed2);
        }
        if (mTracerSpeed3 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_3", mTracerSpeed3);
        }
        if (mTracerSpeed4 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_4", mTracerSpeed4);
        }
        if (mTracerSpeed5 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_5", mTracerSpeed5);
        }
        if (mTracerSpeed6 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_6", mTracerSpeed6);
        }
        if (mTracerSpeed7 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_7", mTracerSpeed7);
        }
        if (mTracerSpeed8 != SPEED_CLASS_DEFAULT) {
            aExport->AddDictionaryInt("tracer_speed_class_8", mTracerSpeed8);
        }
    }
    
    if (mTracerReverseGlobal == true) {
        aExport->AddDictionaryBool("tracer_reverse_global", mTracerReverseGlobal);
    } else {
        if (mTracerReverse1 == true) { aExport->AddDictionaryBool("tracer_reverse_1", mTracerReverse1); }
        if (mTracerReverse2 == true) { aExport->AddDictionaryBool("tracer_reverse_2", mTracerReverse2); }
        if (mTracerReverse3 == true) { aExport->AddDictionaryBool("tracer_reverse_3", mTracerReverse3); }
        if (mTracerReverse4 == true) { aExport->AddDictionaryBool("tracer_reverse_4", mTracerReverse4); }
        if (mTracerReverse5 == true) { aExport->AddDictionaryBool("tracer_reverse_5", mTracerReverse5); }
        if (mTracerReverse6 == true) { aExport->AddDictionaryBool("tracer_reverse_6", mTracerReverse6); }
        if (mTracerReverse7 == true) { aExport->AddDictionaryBool("tracer_reverse_7", mTracerReverse7); }
        if (mTracerReverse8 == true) { aExport->AddDictionaryBool("tracer_reverse_8", mTracerReverse8); }
    }
    
    if (mTracerPlayOnEnterGlobal == true) {
        aExport->AddDictionaryBool("tracer_play_on_enter_global", mTracerPlayOnEnterGlobal);
    }
    
    if (mTracerPlayOnExitGlobal == true) {
        aExport->AddDictionaryBool("tracer_play_on_exit_global", mTracerPlayOnExitGlobal);
    }
    
    return aExport;
}

void LevelFormationConfigurationBlueprint::Load(FJSONNode *pNode) {
    Reset();
    if (pNode == NULL) { return; }
    
    mTracerOffsetGlobal = pNode->GetInt("tracer_offset_global", 0);
    if (mTracerOffsetGlobal == 0) {
        mTracerOffset1 = pNode->GetInt("tracer_offset_1", 0);
        mTracerOffset2 = pNode->GetInt("tracer_offset_2", 0);
        mTracerOffset3 = pNode->GetInt("tracer_offset_3", 0);
        mTracerOffset4 = pNode->GetInt("tracer_offset_4", 0);
        mTracerOffset5 = pNode->GetInt("tracer_offset_5", 0);
        mTracerOffset6 = pNode->GetInt("tracer_offset_6", 0);
        mTracerOffset7 = pNode->GetInt("tracer_offset_7", 0);
        mTracerOffset8 = pNode->GetInt("tracer_offset_8", 0);
    }
    
    mTracerSpeedGlobal = pNode->GetInt("tracer_speed_class_global", SPEED_CLASS_DEFAULT);
    if (mTracerSpeedGlobal == SPEED_CLASS_DEFAULT) {
        mTracerSpeed1 = pNode->GetInt("tracer_speed_class_1", SPEED_CLASS_DEFAULT);
        mTracerSpeed2 = pNode->GetInt("tracer_speed_class_2", SPEED_CLASS_DEFAULT);
        mTracerSpeed3 = pNode->GetInt("tracer_speed_class_3", SPEED_CLASS_DEFAULT);
        mTracerSpeed4 = pNode->GetInt("tracer_speed_class_4", SPEED_CLASS_DEFAULT);
        mTracerSpeed5 = pNode->GetInt("tracer_speed_class_5", SPEED_CLASS_DEFAULT);
        mTracerSpeed6 = pNode->GetInt("tracer_speed_class_6", SPEED_CLASS_DEFAULT);
        mTracerSpeed7 = pNode->GetInt("tracer_speed_class_7", SPEED_CLASS_DEFAULT);
        mTracerSpeed8 = pNode->GetInt("tracer_speed_class_8", SPEED_CLASS_DEFAULT);
    }
    
    
    
    mTracerReverseGlobal = pNode->GetBool("tracer_reverse_global", false);
    if (mTracerReverseGlobal == false) {
        mTracerReverse1 = pNode->GetBool("tracer_reverse_1", false);
        mTracerReverse2 = pNode->GetBool("tracer_reverse_2", false);
        mTracerReverse3 = pNode->GetBool("tracer_reverse_3", false);
        mTracerReverse4 = pNode->GetBool("tracer_reverse_4", false);
        mTracerReverse5 = pNode->GetBool("tracer_reverse_5", false);
        mTracerReverse6 = pNode->GetBool("tracer_reverse_6", false);
        mTracerReverse7 = pNode->GetBool("tracer_reverse_7", false);
        mTracerReverse8 = pNode->GetBool("tracer_reverse_8", false);
    }
    
    mTracerPlayOnEnterGlobal = pNode->GetBool("tracer_play_on_enter_global", false);
    mTracerPlayOnExitGlobal = pNode->GetBool("tracer_play_on_exit_global", false);

    
    
    
    
    /*
    mSpawnSpacingOffset = pNode->GetInt("spacing_offset", 0);
    
    mFormationID = pNode->GetString("formation", mFormationID);
    
    
    if (mFormationID.mLength > 0) {
        //Possibility 1.) We have a formation...
    } else {
        //Possibility 2.) We have an object...
        mObjectType = pNode->GetInt("type", mObjectType);
    }
    
    FJSONNode *aMotionNode = pNode->GetDictionary("motion");
    mMotionController.Load(aMotionNode);
    */
    
}

void LevelFormationConfigurationBlueprint::Build(LevelFormationConfiguration *pConfiguration) {
    
    if (pConfiguration == NULL) { return; }
    
    pConfiguration->mTracerPlayOnEnterGlobal = mTracerPlayOnEnterGlobal;
    pConfiguration->mTracerPlayOnExitGlobal = mTracerPlayOnExitGlobal;
    
    pConfiguration->mTracerSpeedGlobal = mTracerSpeedGlobal;
    pConfiguration->mTracerSpeed1 = mTracerSpeed1;
    pConfiguration->mTracerSpeed2 = mTracerSpeed2;
    pConfiguration->mTracerSpeed3 = mTracerSpeed3;
    pConfiguration->mTracerSpeed4 = mTracerSpeed4;
    pConfiguration->mTracerSpeed5 = mTracerSpeed5;
    pConfiguration->mTracerSpeed6 = mTracerSpeed6;
    pConfiguration->mTracerSpeed7 = mTracerSpeed7;
    pConfiguration->mTracerSpeed8 = mTracerSpeed8;
    
    pConfiguration->mTracerOffsetGlobal = mTracerOffsetGlobal;
    pConfiguration->mTracerOffset1 = mTracerOffset1;
    pConfiguration->mTracerOffset2 = mTracerOffset2;
    pConfiguration->mTracerOffset3 = mTracerOffset3;
    pConfiguration->mTracerOffset4 = mTracerOffset4;
    pConfiguration->mTracerOffset5 = mTracerOffset5;
    pConfiguration->mTracerOffset6 = mTracerOffset6;
    pConfiguration->mTracerOffset7 = mTracerOffset7;
    pConfiguration->mTracerOffset8 = mTracerOffset8;
    
    pConfiguration->mTracerReverseGlobal = mTracerReverseGlobal;
    pConfiguration->mTracerReverse1 = mTracerReverse1;
    pConfiguration->mTracerReverse2 = mTracerReverse2;
    pConfiguration->mTracerReverse3 = mTracerReverse3;
    pConfiguration->mTracerReverse4 = mTracerReverse4;
    pConfiguration->mTracerReverse5 = mTracerReverse5;
    pConfiguration->mTracerReverse6 = mTracerReverse6;
    pConfiguration->mTracerReverse7 = mTracerReverse7;
    pConfiguration->mTracerReverse8 = mTracerReverse8;
    
}
