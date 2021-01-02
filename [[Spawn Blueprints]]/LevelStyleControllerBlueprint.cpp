//
//  LevelStyleControllerBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/19/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelStyleControllerBlueprint.hpp"
#include "os_core_graphics.h"
#include "core_includes.h"

#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

LevelStyleControllerBlueprint::LevelStyleControllerBlueprint() {
    mDisableThreads = false;
    mEnableBobbleMode = false;
    mRandomizeSpin = false;
    mAllRed = false;
    mAllGreen = false;
    mAllBlue = false;
}

LevelStyleControllerBlueprint::~LevelStyleControllerBlueprint() {
    
}

void LevelStyleControllerBlueprint::Update() {
    
}

void LevelStyleControllerBlueprint::Reset() {
    mDisableThreads = false;
    mEnableBobbleMode = false;
    mRandomizeSpin = false;
    mAllRed = false;
    mAllGreen = false;
    mAllBlue = false;
    
    
}

bool LevelStyleControllerBlueprint::ShouldSave() {
    
    if (mDisableThreads == true) { return true; }
    if (mEnableBobbleMode == true) { return true; }
    if (mEnableBobbleMode == true) { return true; }
    if (mAllRed == true) { return true; }
    if (mAllGreen == true) { return true; }
    if (mAllBlue == true) { return true; }
    
    
    return false;
}

void LevelStyleControllerBlueprint::Build(LevelStyleController *pStyleController) {
    
    if (pStyleController == NULL) { return; }
    
    pStyleController->Reset();
    
    pStyleController->mDisableThreads = mDisableThreads;
    pStyleController->mEnableBobbleMode = mEnableBobbleMode;
    pStyleController->mRandomizeSpin = mRandomizeSpin;
    
    pStyleController->mAllRed = mAllRed;
    pStyleController->mAllGreen = mAllGreen;
    pStyleController->mAllBlue = mAllBlue;
}

FJSONNode *LevelStyleControllerBlueprint::Save() {
    
    FJSONNode *aExport = new FJSONNode();
    aExport->mNodeType = JSON_NODE_TYPE_DICTIONARY;
    
    if (mDisableThreads != false) { aExport->AddDictionaryBool("disable_threads", mDisableThreads); }
    if (mEnableBobbleMode != false) { aExport->AddDictionaryBool("enable_bobble", mEnableBobbleMode); }
    if (mRandomizeSpin != false) { aExport->AddDictionaryBool("randomize_spin", mRandomizeSpin); }
    
    if (mAllRed != false) { aExport->AddDictionaryBool("all_red", mAllRed); }
    if (mAllGreen != false) { aExport->AddDictionaryBool("all_green", mAllGreen); }
    if (mAllBlue != false) { aExport->AddDictionaryBool("all_blue", mAllBlue); }
    
    
    return aExport;
}

void LevelStyleControllerBlueprint::Load(FJSONNode *pNode) {
    Reset();
    if (pNode == NULL) { return; }
    
    mDisableThreads = pNode->GetBool("disable_threads", false);
    mEnableBobbleMode = pNode->GetBool("enable_bobble", false);
    mRandomizeSpin = pNode->GetBool("randomize_spin", false);
    
    mAllRed = pNode->GetBool("all_red", false);
    mAllGreen = pNode->GetBool("all_green", false);
    mAllBlue = pNode->GetBool("all_blue", false);
}
