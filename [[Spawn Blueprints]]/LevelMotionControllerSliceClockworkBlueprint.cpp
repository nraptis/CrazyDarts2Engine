//
//  LevelMotionControllerSliceClockworkBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/29/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelMotionControllerSliceClockworkBlueprint.hpp"

LevelMotionControllerSliceClockworkBlueprintNode::LevelMotionControllerSliceClockworkBlueprintNode() {
    
    mTime = 60;
    
    mOffsetX = 0;
    mOffsetY = 0;
    mRotation = 0;
    
    mPause = 0;
    
    mTranslationLinear = false;
    mRotationLinear = false;
}

LevelMotionControllerSliceClockworkBlueprintNode::~LevelMotionControllerSliceClockworkBlueprintNode() {
    
}

FJSONNode *LevelMotionControllerSliceClockworkBlueprintNode::Save() {
    FJSONNode *aExport = new FJSONNode();
    aExport->mNodeType = JSON_NODE_TYPE_DICTIONARY;
    if (mTime != 60) { aExport->AddDictionaryInt("time", mTime); }
    if (mOffsetX != 0) { aExport->AddDictionaryInt("x", mOffsetX); }
    if (mOffsetY != 0) { aExport->AddDictionaryInt("y", mOffsetY); }
    if (mRotation != 0) { aExport->AddDictionaryInt("rot", mRotation); }
    if (mPause != 0) { aExport->AddDictionaryInt("pause", mPause); }
    if (mTranslationLinear == true) { aExport->AddDictionaryBool("mot_linear", mTranslationLinear); }
    if (mRotationLinear == true) { aExport->AddDictionaryBool("rot_linear", mRotationLinear); }
    return aExport;
}

void LevelMotionControllerSliceClockworkBlueprintNode::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    mTime = pNode->GetInt("time", 60);
    mOffsetX = pNode->GetInt("x", 0);
    mOffsetY = pNode->GetInt("y", 0);
    mRotation = pNode->GetInt("rot", 0);
    mPause = pNode->GetInt("pause", 0);
    mTranslationLinear = pNode->GetBool("mot_linear", false);
    mRotationLinear = pNode->GetBool("rot_linear", false);
}

LevelMotionControllerSliceClockworkBlueprint::LevelMotionControllerSliceClockworkBlueprint() {
    mType = LEVEL_MOTION_SLICE_TYPE_CLOCKWORK;
    mNodeCount = 9;
    mStartNode = 0;
    mPauseStart = false;
    mClosed = false;
    mSelectedNodeIndex = 0;
    
    int aRotation = 0;
    for (int i=0;i<9;i++) {
        mNode[i].mRotation = aRotation;
        aRotation += 45;
    }
}

LevelMotionControllerSliceClockworkBlueprint::~LevelMotionControllerSliceClockworkBlueprint() {
    
}

void LevelMotionControllerSliceClockworkBlueprint::Build(LevelMotionControllerSliceClockwork *pSlice) {
    if (pSlice == NULL) { return; }
    
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    if (mStartNode < 0) { mStartNode = 0; }
    if (mStartNode >= mNodeCount) { mStartNode = 0; }
    
    LevelMotionControllerSliceBlueprint::Build(pSlice);
    
    pSlice->mClosed = mClosed;
    for (int i=0;i<mNodeCount;i++) {
        LevelMotionControllerSliceClockworkNode *aNode = new LevelMotionControllerSliceClockworkNode();
        
        aNode->mTime = mNode[i].mTime;
        aNode->mOffsetX = mNode[i].mOffsetX;
        aNode->mOffsetY = mNode[i].mOffsetY;
        aNode->mRotation = mNode[i].mRotation;
        aNode->mPause = mNode[i].mPause;
        
        aNode->mTranslationLinear = mNode[i].mTranslationLinear;
        aNode->mRotationLinear = mNode[i].mRotationLinear;
        
        pSlice->mNodeList.Add(aNode);
    }
    
    pSlice->ApplyStartingCondition(mStartNode, mPauseStart);
}

FJSONNode *LevelMotionControllerSliceClockworkBlueprint::Save() {
    
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    
    FJSONNode *aExport = LevelMotionControllerSliceBlueprint::Save();
    
    if (mStartNode != 0) { aExport->AddDictionaryInt("start_node", mStartNode); }
    if (mPauseStart == true) { aExport->AddDictionaryBool("pause_start", mPauseStart); }
    if (mClosed == true) { aExport->AddDictionaryBool("closed", mClosed); }
    
    
    FJSONNode *aNodeList = new FJSONNode();
    aNodeList->mNodeType = JSON_NODE_TYPE_ARRAY;
    for (int i=0;i<mNodeCount;i++) {
        aNodeList->AddArray(mNode[i].Save());
    }
    aExport->AddDictionary("node_list", aNodeList);
    
    return aExport;
}

void LevelMotionControllerSliceClockworkBlueprint::Load(FJSONNode *pNode) {
    if (pNode == NULL) { return; }
    LevelMotionControllerSliceBlueprint::Load(pNode);
    //mNodeCount = pNode->GetInt("node_count", 4);
    mStartNode = pNode->GetInt("start_node", 0);
    mPauseStart = pNode->GetBool("pause_start", false);
    mClosed = pNode->GetBool("closed", false);
    
    
    mNodeCount = 0;
    FJSONNode *aNodeList = pNode->GetArray("node_list");
    if (aNodeList != NULL) {
        EnumJSONArray(aNodeList, aNode) {
            mNode[mNodeCount].Load(aNode);
            ++mNodeCount;
        }
    }
    
    if (mSelectedNodeIndex >= mNodeCount) { mSelectedNodeIndex = mNodeCount - 1; }
    if (mSelectedNodeIndex < 0) { mSelectedNodeIndex = 0; }
    if (mStartNode < 0) { mStartNode = 0; }
    if (mStartNode >= mNodeCount) { mStartNode = 0; }
    
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetRotation(int pCount) {
    
    int aStep = 0;
    
    if (pCount == 2) { aStep = 180; }
    if (pCount == 3) { aStep = 120; }
    if (pCount == 4) { aStep = 90; }
    if (pCount == 5) { aStep = 72; }
    if (pCount == 6) { aStep = 60; }
    if (pCount == 8) { aStep = 45; }
    if (pCount == 10) { aStep = 36; }

    if (aStep != 0) {
        
        mNodeCount = (pCount + 1);
        int aRotation = 0;
        for (int i=0;i<mNodeCount;i++) {
            
            mNode[i].mRotation = aRotation;
            
            aRotation += aStep;
        }
    }
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetAllTime(int pTime) {
    
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    for (int i=0;i<MAX_CLOCKWORK_NODE_COUNT;i++) {
        mNode[i].mTime = pTime;
    }
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetAllPause(int pTime) {
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    for (int i=0;i<MAX_CLOCKWORK_NODE_COUNT;i++) {
        mNode[i].mPause = pTime;
    }
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetAllRotation(int pRotation) {
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    for (int i=0;i<MAX_CLOCKWORK_NODE_COUNT;i++) {
        mNode[i].mRotation = pRotation;
    }
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetSpin() {
    
    for (int i=0;i<MAX_CLOCKWORK_NODE_COUNT;i++) {
        mNode[i].mTime = 0;
        mNode[i].mPause = 0;
        mNode[i].mOffsetX = 0;
        mNode[i].mOffsetY = 0;
        mNode[i].mRotationLinear = false;
        mNode[i].mTranslationLinear = false;
    }
    
    mNodeCount = 2;
    
    mNode[0].mRotation = 0;
    mNode[0].mRotationLinear = true;
    mNode[0].mPause = 0;
    mNode[0].mTime = 800;
    
    mNode[1].mRotation = 360;
    mNode[1].mPause = 0;
    mNode[1].mTime = 0;
}


void LevelMotionControllerSliceClockworkBlueprint::QuickSetMoveX(const char *pInstructions) {
    
    FString aString = FString(pInstructions);
    FIntList aList;
    
    FString aNumberString = aString.GetNumber(0);
    if (aNumberString.mLength <= 0) { return; }
    
    aList.Add(aNumberString.ToInt());
    
    while (true) {
        aNumberString = aString.GetNextNumber();
        if (aNumberString.mLength <= 0) { break; }
        aList.Add(aNumberString.ToInt());
    }
    
    if (aList.mCount > mNodeCount) { mNodeCount = aList.mCount; }
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    
    for (int i=0;i<aList.mCount;i++) {
        mNode[i].mOffsetX = aList.mData[i];
    }
}


void LevelMotionControllerSliceClockworkBlueprint::QuickSetMoveY(const char *pInstructions) {
    FString aString = FString(pInstructions);
    FIntList aList;
    FString aNumberString = aString.GetNumber(0);
    if (aNumberString.mLength <= 0) { return; }
    aList.Add(aNumberString.ToInt());
    while (true) {
        aNumberString = aString.GetNextNumber();
        if (aNumberString.mLength <= 0) { break; }
        aList.Add(aNumberString.ToInt());
    }
    if (aList.mCount > mNodeCount) { mNodeCount = aList.mCount; }
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    for (int i=0;i<aList.mCount;i++) {
        mNode[i].mOffsetY = aList.mData[i];
    }
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetClearX() {
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    for (int i=0;i<MAX_CLOCKWORK_NODE_COUNT;i++) {
        mNode[i].mOffsetX = 0;
    }
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetClearY() {
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    for (int i=0;i<MAX_CLOCKWORK_NODE_COUNT;i++) {
        mNode[i].mOffsetY = 0;
    }
}


void LevelMotionControllerSliceClockworkBlueprint::QuickSetRotateBackForth(float pStartAngle, float pEndAngle, int pStepsIn, int pStepsOut) {
    
    if (pStepsIn > 5) { pStepsIn = 5; }
    if (pStepsIn < 0) { pStepsIn = 0; }
    
    if (pStepsOut > 5) { pStepsOut = 5; }
    if (pStepsOut < 0) { pStepsOut = 0; }
    
    mNodeCount = 3 + pStepsIn + pStepsOut;
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    
    
    int aIndex = 0;
    mNode[aIndex++].mRotation = pStartAngle;
    
    for (int i=0;i<pStepsIn;i++) {
        float aPercent = ((float)(i + 1)) / ((float)(pStepsIn + 1));
        float aAngle = pStartAngle + (pEndAngle - pStartAngle) * aPercent;
        mNode[aIndex++].mRotation = aAngle;
    }
    
    mNode[aIndex++].mRotation = pEndAngle;
    
    for (int i=0;i<pStepsOut;i++) {
        float aPercent = ((float)(i + 1)) / ((float)(pStepsOut + 1));
        float aAngle = pEndAngle + (pStartAngle - pEndAngle) * aPercent;
        mNode[aIndex++].mRotation = aAngle;
    }
    
    mNode[aIndex++].mRotation = pStartAngle;
}

void LevelMotionControllerSliceClockworkBlueprint::QuickSetRotateFullCircle(float pStartAngle, bool pReverse, int pSteps) {
    
    if (pSteps > 2) { pSteps = 2; }
    if (pSteps < 0) { pSteps = 0; }
    
    
    mNodeCount = 5 + pSteps + pSteps + pSteps + pSteps;
    if (mNodeCount < 2) { mNodeCount = 2; }
    if (mNodeCount > MAX_CLOCKWORK_NODE_COUNT) { mNodeCount = MAX_CLOCKWORK_NODE_COUNT; }
    
    
    float aAngle0 = pStartAngle;
    float aAngle1 = 0.0f;
    float aAngle2 = 0.0f;
    float aAngle3 = 0.0f;
    float aAngle4 = 0.0f;
    
    if (pReverse) {
        aAngle1 = pStartAngle - 90.0f;
        aAngle2 = pStartAngle - 180.0f;
        aAngle3 = pStartAngle - 270.0f;
        aAngle4 = pStartAngle - 360.0f;
    } else {
        aAngle1 = pStartAngle + 90.0f;
        aAngle2 = pStartAngle + 180.0f;
        aAngle3 = pStartAngle + 270.0f;
        aAngle4 = pStartAngle + 360.0f;
    }
    
    int aIndex = 0;
    
    mNode[aIndex++].mRotation = aAngle0;
    for (int i=0;i<pSteps;i++) {
        float aPercent = ((float)(i + 1)) / ((float)(pSteps + 1));
        mNode[aIndex++].mRotation = aAngle0 + (aAngle1 - aAngle0) * aPercent;
    }

    mNode[aIndex++].mRotation = aAngle1;
    for (int i=0;i<pSteps;i++) {
        float aPercent = ((float)(i + 1)) / ((float)(pSteps + 1));
        mNode[aIndex++].mRotation = aAngle1 + (aAngle2 - aAngle1) * aPercent;
    }
    
    mNode[aIndex++].mRotation = aAngle2;
    for (int i=0;i<pSteps;i++) {
        float aPercent = ((float)(i + 1)) / ((float)(pSteps + 1));
        mNode[aIndex++].mRotation = aAngle2 + (aAngle3 - aAngle2) * aPercent;
    }
    
    mNode[aIndex++].mRotation = aAngle3;
    for (int i=0;i<pSteps;i++) {
        float aPercent = ((float)(i + 1)) / ((float)(pSteps + 1));
        mNode[aIndex++].mRotation = aAngle3 + (aAngle4 - aAngle3) * aPercent;
    }
    
    mNode[aIndex++].mRotation = aAngle4;
}
