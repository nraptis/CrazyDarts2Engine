//
//  LevelMotionControllerSliceBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/20/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelMotionControllerSliceBlueprint.hpp"
#include "LevelWave.hpp"
#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

LevelMotionControllerSliceBlueprint::LevelMotionControllerSliceBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_NONE;
    mSpeedClass = SPEED_CLASS_MEDIUM;
    mSpeedNegateAlways = false;
    mSpeedNegateRandomly = false;
    mRotatesObject = false;
    mRotatesCannons = false;
    mRotatesPlatforms = false;
    
    mKillTimer = 8;
}

LevelMotionControllerSliceBlueprint::~LevelMotionControllerSliceBlueprint() {
    
}

void LevelMotionControllerSliceBlueprint::Build(LevelMotionControllerSlice *pSlice) {
    if (pSlice == NULL) { return; }
    pSlice->mSpeedClass = mSpeedClass;
    pSlice->mSpeedNegateAlways = mSpeedNegateAlways;
    pSlice->mSpeedNegateRandomly = mSpeedNegateRandomly;
    pSlice->mRotatesObject = mRotatesObject;
    pSlice->mRotatesCannons = mRotatesCannons;
    pSlice->mRotatesPlatforms = mRotatesPlatforms;
}

FJSONNode *LevelMotionControllerSliceBlueprint::Save() {
    FJSONNode *aExport = new FJSONNode();
    if (mType != LEVEL_MOTION_SLICE_TYPE_NONE) { aExport->AddDictionaryInt("type", mType); }
    if (mSpeedClass != SPEED_CLASS_MEDIUM) { aExport->AddDictionaryInt("speed_class", mSpeedClass); }
    if (mSpeedNegateAlways == true) { aExport->AddDictionaryBool("negate", mSpeedNegateAlways); }
    if (mSpeedNegateRandomly == true) { aExport->AddDictionaryBool("negate_rand", mSpeedNegateRandomly); }
    if (mRotatesObject == true) { aExport->AddDictionaryBool("rotates_obj", mRotatesObject); }
    if (mRotatesCannons == true) { aExport->AddDictionaryBool("rotates_cannons", mRotatesCannons); }
    if (mRotatesPlatforms == true) { aExport->AddDictionaryBool("rotates_platforms", mRotatesPlatforms); }
    
    return aExport;
}

void LevelMotionControllerSliceBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    mSpeedClass = pNode->GetInt("speed_class", SPEED_CLASS_MEDIUM);
    mSpeedNegateAlways = pNode->GetBool("negate", false);
    mSpeedNegateRandomly = pNode->GetBool("negate_rand", false);
    mRotatesObject = pNode->GetBool("rotates_obj", false);
    mRotatesCannons = pNode->GetBool("rotates_cannons", false);
    mRotatesPlatforms = pNode->GetBool("rotates_platforms", false);
}



LevelMotionControllerSliceNegateBlueprint::LevelMotionControllerSliceNegateBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_NEGATE;
    mNegateHAlways = true;
    mNegateHRandomly = false;
    mNegateVAlways = false;
    mNegateVRandomly = false;
}

LevelMotionControllerSliceNegateBlueprint::~LevelMotionControllerSliceNegateBlueprint() {
    
}

void LevelMotionControllerSliceNegateBlueprint::Build(LevelMotionControllerSliceNegate *pSlice) {
    if (pSlice == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    pSlice->mNegateHAlways = mNegateHAlways;
    pSlice->mNegateHRandomly = mNegateHRandomly;
    pSlice->mNegateVAlways = mNegateVAlways;
    pSlice->mNegateVRandomly = mNegateVRandomly;
}

FJSONNode *LevelMotionControllerSliceNegateBlueprint::Save() {
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    if (mNegateHAlways == false) { aExport->AddDictionaryBool("negate_h", mNegateHAlways); }
    if (mNegateHRandomly == true) { aExport->AddDictionaryBool("negate_h_rand", mNegateHRandomly); }
    if (mNegateVAlways == true) { aExport->AddDictionaryBool("negate_v", mNegateVAlways); }
    if (mNegateVRandomly == true) { aExport->AddDictionaryBool("negate_v_rand", mNegateVRandomly); }
    return aExport;
}

void LevelMotionControllerSliceNegateBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    mNegateHAlways = pNode->GetBool("negate_h", true);
    mNegateHRandomly = pNode->GetBool("negate_h_rand", false);
    mNegateVAlways = pNode->GetBool("negate_v", false);
    mNegateVRandomly = pNode->GetBool("negate_v_rand", false);
}




LevelMotionControllerSliceFixedModBlueprint::LevelMotionControllerSliceFixedModBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_FIXED_MOD;
    mOffsetX = 0;
    mOffsetY = 0;
    mRotation = 0;
}

LevelMotionControllerSliceFixedModBlueprint::~LevelMotionControllerSliceFixedModBlueprint() {
    
}

void LevelMotionControllerSliceFixedModBlueprint::Build(LevelMotionControllerSliceFixedMod *pSlice) {
    if (pSlice == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    pSlice->mOffsetX = ((float)mOffsetX);
    pSlice->mOffsetY = ((float)mOffsetY);
    pSlice->mRotation = ((float)mRotation);
}

FJSONNode *LevelMotionControllerSliceFixedModBlueprint::Save() {
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    if (mOffsetX != 0) { aExport->AddDictionaryInt("offset_x", mOffsetX); }
    if (mOffsetY != 0) { aExport->AddDictionaryInt("offset_y", mOffsetY); }
    if (mRotation != 0) { aExport->AddDictionaryInt("rotation", mRotation); }
    return aExport;
}

void LevelMotionControllerSliceFixedModBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    
    mOffsetX = pNode->GetInt("offset_x", 0);
    mOffsetY = pNode->GetInt("offset_y", 0);
    mRotation = pNode->GetInt("rotation", 0);
}






LevelMotionControllerSliceRotateBlueprint::LevelMotionControllerSliceRotateBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_ROTATE;
    mPivotOffsetX = 0;
    mPivotOffsetY = 0;
}

LevelMotionControllerSliceRotateBlueprint::~LevelMotionControllerSliceRotateBlueprint() {
    
}

void LevelMotionControllerSliceRotateBlueprint::Build(LevelMotionControllerSliceRotate *pSlice) {
    if (pSlice == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    pSlice->mPivotOffsetX = ((float)mPivotOffsetX);
    pSlice->mPivotOffsetY = ((float)mPivotOffsetY);
}

FJSONNode *LevelMotionControllerSliceRotateBlueprint::Save() {
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    if (mPivotOffsetX != 0) { aExport->AddDictionaryInt("pivot_offset_x", mPivotOffsetX); }
    if (mPivotOffsetY != 0) { aExport->AddDictionaryInt("pivot_offset_y", mPivotOffsetY); }
    return aExport;
}

void LevelMotionControllerSliceRotateBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    mPivotOffsetX = pNode->GetInt("pivot_offset_x", 0);
    mPivotOffsetY = pNode->GetInt("pivot_offset_y", 0);
}

void LevelMotionControllerSliceRotateBlueprint::ResetPivot() {
    mPivotOffsetX = 0;
    mPivotOffsetY = 0;
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
}





LevelMotionControllerSliceOscillateRotationBlueprint::LevelMotionControllerSliceOscillateRotationBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_OSCILLATE_ROTATION;
    mAngleSpan = 90;
    mAngleSpanOffsetStart = 0;
    mAngleSpanOffsetEnd = 0;
    mLinear = false;
    mLinearTimer = 90;
    mLinearTime = 180;
    mPause1 = 0;
    mPause2 = 0;
    mPauseStartType = 0;
}

LevelMotionControllerSliceOscillateRotationBlueprint::~LevelMotionControllerSliceOscillateRotationBlueprint() {
    
}

void LevelMotionControllerSliceOscillateRotationBlueprint::Build(LevelMotionControllerSliceOscillateRotation *pSlice) {
    if (pSlice == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    pSlice->mLinear = mLinear;
    pSlice->mLinearTimer = mLinearTimer;
    pSlice->mLinearTime = mLinearTime;
    pSlice->mAngleSpan = ((float)mAngleSpan);
    pSlice->mAngleSpanOffsetStart = ((float)mAngleSpanOffsetStart);
    pSlice->mAngleSpanOffsetEnd = ((float)mAngleSpanOffsetEnd);
    pSlice->mPause1 = mPause1;
    pSlice->mPause2 = mPause2;
    pSlice->mPauseStartType = mPauseStartType;
    pSlice->ApplyPauseStartType();
}

FJSONNode *LevelMotionControllerSliceOscillateRotationBlueprint::Save() {
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    if (mLinear == true) { aExport->AddDictionaryBool("linear", mLinear); }
    if (mLinearTimer != 90) { aExport->AddDictionaryInt("linear_tick", mLinearTimer); }
    if (mLinearTime != 180) { aExport->AddDictionaryInt("linear_time", mLinearTime); }
    if (mAngleSpan != 90) { aExport->AddDictionaryInt("angle_span", mAngleSpan); }
    if (mAngleSpanOffsetStart != 0) { aExport->AddDictionaryInt("angle_offset_start", mAngleSpanOffsetStart); }
    if (mAngleSpanOffsetEnd != 0) { aExport->AddDictionaryInt("angle_offset_end", mAngleSpanOffsetEnd); }
    if (mPause1 != 0) { aExport->AddDictionaryInt("pause_1", mPause1); }
    if (mPause2 != 0) { aExport->AddDictionaryInt("pause_2", mPause2); }
    if (mPauseStartType != 0) { aExport->AddDictionaryInt("pause_start_type", mPauseStartType); }
    
    return aExport;
}

void LevelMotionControllerSliceOscillateRotationBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    mLinear = pNode->GetBool("linear", false);
    mLinearTimer = pNode->GetInt("linear_tick", 90);
    mLinearTime  = pNode->GetInt("linear_time", 180);
    mAngleSpan = pNode->GetInt("angle_span", 90);
    mAngleSpanOffsetStart = pNode->GetInt("angle_offset_start", 0);
    mAngleSpanOffsetEnd = pNode->GetInt("angle_offset_end", 0);
    mPause1 = pNode->GetInt("pause_1", 0);
    mPause2 = pNode->GetInt("pause_2", 0);
    mPauseStartType = pNode->GetInt("pause_start_type", 0);
}








LevelMotionControllerSliceOscillateVerticalBlueprint::LevelMotionControllerSliceOscillateVerticalBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_OSCILLATE_VERTICAL;
    mRadius = 32;
    mLinear = false;
    mLinearTimer = 90;
    mLinearTime = 180;
    mPause1 = 0;
    mPause2 = 0;
    mPauseStartType = 0;
}

LevelMotionControllerSliceOscillateVerticalBlueprint::~LevelMotionControllerSliceOscillateVerticalBlueprint() {
    
}

void LevelMotionControllerSliceOscillateVerticalBlueprint::Build(LevelMotionControllerSliceOscillateVertical *pSlice) {
    if (pSlice == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    pSlice->mLinear = mLinear;
    pSlice->mLinearTimer = mLinearTimer;
    pSlice->mLinearTime = mLinearTime;
    pSlice->mRadius = ((float)mRadius);
    pSlice->mPause1 = mPause1;
    pSlice->mPause2 = mPause2;
    pSlice->mPauseStartType = mPauseStartType;
    pSlice->ApplyPauseStartType();
}

FJSONNode *LevelMotionControllerSliceOscillateVerticalBlueprint::Save() {
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    if (mLinear == true) { aExport->AddDictionaryBool("linear", mLinear); }
    if (mLinearTimer != 90) { aExport->AddDictionaryInt("linear_tick", mLinearTimer); }
    if (mLinearTime != 180) { aExport->AddDictionaryInt("linear_time", mLinearTime); }
    if (mRadius != 32) { aExport->AddDictionaryInt("radius", mRadius); }
    if (mPause1 != 0) { aExport->AddDictionaryInt("pause_1", mPause1); }
    if (mPause2 != 0) { aExport->AddDictionaryInt("pause_2", mPause2); }
    if (mPauseStartType != 0) { aExport->AddDictionaryInt("pause_start_type", mPauseStartType); }
    return aExport;
}

void LevelMotionControllerSliceOscillateVerticalBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    mLinear = pNode->GetBool("linear", false);
    mLinearTimer = pNode->GetInt("linear_tick", 90);
    mLinearTime  = pNode->GetInt("linear_time", 180);
    mRadius = pNode->GetInt("radius", 32);
    mPause1 = pNode->GetInt("pause_1", 0);
    mPause2 = pNode->GetInt("pause_2", 0);
    mPauseStartType = pNode->GetInt("pause_start_type", 0);
}




LevelMotionControllerSliceOscillateHorizontalBlueprint::LevelMotionControllerSliceOscillateHorizontalBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_OSCILLATE_HORIZONTAL;
    mRadius = 32;
    mLinear = false;
    mLinearTimer = 90;
    mLinearTime = 180;
    mPause1 = 0;
    mPause2 = 0;
    mPauseStartType = 0;
}

LevelMotionControllerSliceOscillateHorizontalBlueprint::~LevelMotionControllerSliceOscillateHorizontalBlueprint() {
    
}

void LevelMotionControllerSliceOscillateHorizontalBlueprint::Build(LevelMotionControllerSliceOscillateHorizontal *pSlice) {
    if (pSlice == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    pSlice->mLinear = mLinear;
    pSlice->mLinearTimer = mLinearTimer;
    pSlice->mLinearTime = mLinearTime;
    pSlice->mRadius = ((float)mRadius);
    pSlice->mPause1 = mPause1;
    pSlice->mPause2 = mPause2;
    pSlice->mPauseStartType = mPauseStartType;
    pSlice->ApplyPauseStartType();
}

FJSONNode *LevelMotionControllerSliceOscillateHorizontalBlueprint::Save() {
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    if (mLinear == true) { aExport->AddDictionaryBool("linear", mLinear); }
    if (mLinearTimer != 90) { aExport->AddDictionaryInt("linear_tick", mLinearTimer); }
    if (mLinearTime != 180) { aExport->AddDictionaryInt("linear_time", mLinearTime); }
    if (mRadius != 32) { aExport->AddDictionaryInt("radius", mRadius); }
    if (mPause1 != 0) { aExport->AddDictionaryInt("pause_1", mPause1); }
    if (mPause2 != 0) { aExport->AddDictionaryInt("pause_2", mPause2); }
    if (mPauseStartType != 0) { aExport->AddDictionaryInt("pause_start_type", mPauseStartType); }
    return aExport;
}

void LevelMotionControllerSliceOscillateHorizontalBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    mLinear = pNode->GetBool("linear", false);
    mLinearTimer = pNode->GetInt("linear_tick", 90);
    mLinearTime  = pNode->GetInt("linear_time", 180);
    mRadius = pNode->GetInt("radius", 32);
    mPause1 = pNode->GetInt("pause_1", 0);
    mPause2 = pNode->GetInt("pause_2", 0);
    mPauseStartType = pNode->GetInt("pause_start_type", 0);
}
