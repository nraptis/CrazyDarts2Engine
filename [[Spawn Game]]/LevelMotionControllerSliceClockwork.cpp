//
//  LevelMotionControllerSliceClockwork.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/29/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelMotionControllerSliceClockwork.hpp"
#include "GFXApp.hpp"

LevelMotionControllerSliceClockworkNode::LevelMotionControllerSliceClockworkNode() {
    mOffsetX = 0.0f;
    mOffsetY = 0.0f;
    mRotation = 0.0f;

    mPause = false;
    
    mTranslationLinear = false;
    mRotationLinear = false;
    
    mTime = 60;
}

LevelMotionControllerSliceClockworkNode::~LevelMotionControllerSliceClockworkNode() {
    
}

LevelMotionControllerSliceClockwork::LevelMotionControllerSliceClockwork() {
    
    mClosed = false;
    mPauseTimer = 0;
    mCurrentNodeTimer = 0;
    mCurrentNodeIndex = 0;
    
    mOffsetX = 0.0f;
    mOffsetY = 0.0f;
    mRotation = 0.0f;
    
    mNodeCurrent = NULL;
    mNodeNext = NULL;
}

LevelMotionControllerSliceClockwork::~LevelMotionControllerSliceClockwork() {
    
    FreeList(LevelMotionControllerSliceClockworkNode, mNodeList);
    
}

void LevelMotionControllerSliceClockwork::Update() {
    
    if (mNodeList.mCount < 2) {
        Log("Fatal Error: Update::LevelMotionControllerSliceClockwork < 2\n");
        return;
    }
    
    if (mPauseTimer > 0) {
        if (mPauseExit == true) {
            
        } else {
            mPauseTimer -= 1;
        }
        return;
    }
    
    mNodeCurrent = (LevelMotionControllerSliceClockworkNode *)mNodeList.Fetch(mCurrentNodeIndex);
    mNodeNext = (LevelMotionControllerSliceClockworkNode *)mNodeList.FetchCircular(mCurrentNodeIndex + 1);
    
    if (mNodeCurrent == NULL) {
        Log("Fatal Error: Update:: mNodeCurrent missing (%d) / (%d)\n", mCurrentNodeIndex, mNodeList.mCount);
        return;
    }
    
    if (mNodeNext == NULL) {
        Log("Fatal Error: Update:: mNodeCurrent missing (%d) / (%d)\n", mCurrentNodeIndex, mNodeList.mCount);
        return;
    }
    
    if (mNodeCurrent->mTime < 5) { mNodeCurrent->mTime = 60; }
    if (mNodeNext->mTime < 5) { mNodeNext->mTime = 60; }
    
    mCurrentNodeTimer++;
    if (mCurrentNodeTimer >= mNodeCurrent->mTime) {
        mPauseTimer = mNodeNext->mPause;
        AdvanceIndex();
        return;
    }
    
    
    float aPercent = ((float)mCurrentNodeTimer) / ((float)mNodeCurrent->mTime);
    
    float aPercentTranslation = aPercent;
    float aPercentRotation = aPercent;
    
    if (mNodeCurrent->mTranslationLinear == false) {
        aPercentTranslation = sinf(aPercentTranslation * PI_2);
    }
    
    if (mNodeCurrent->mRotationLinear == false) {
        aPercentRotation = sinf(aPercentRotation * PI_2);
    }
    
    float aStartX = mNodeCurrent->mOffsetX;
    float aStartY = mNodeCurrent->mOffsetY;
    float aStartRotation = mNodeCurrent->mRotation;
    
    
    float aEndX = mNodeNext->mOffsetX;
    float aEndY = mNodeNext->mOffsetY;
    float aEndRotation = mNodeNext->mRotation;
    
    mOffsetX = aStartX + (aEndX - aStartX) * aPercentTranslation;
    mOffsetY = aStartY + (aEndY - aStartY) * aPercentTranslation;
    mRotation = aStartRotation + (aEndRotation - aStartRotation) * aPercentRotation;
}

bool LevelMotionControllerSliceClockwork::ShouldPauseExit() {
    EnumList(LevelMotionControllerSliceClockworkNode, aNode, mNodeList) {
        if (aNode->mPause > 0) { return true; }
    }
    return false;
}

bool LevelMotionControllerSliceClockwork::IsPauseExitComplete() {
    if (ShouldPauseExit() == true) {
        return (mPauseTimer > 0);
    }
    return true;
}

void LevelMotionControllerSliceClockwork::AdvanceIndex() {
    
    mCurrentNodeTimer = 0;
    
    mCurrentNodeIndex += 1;
    int aLastIndex = mNodeList.mCount;
    if (mClosed == false) {
        aLastIndex -= 1;
    }
    
    if (mCurrentNodeIndex >= aLastIndex) {
        mCurrentNodeIndex = 0;
    }
    
    mNodeCurrent = (LevelMotionControllerSliceClockworkNode *)mNodeList.Fetch(mCurrentNodeIndex);
    
    if (mNodeCurrent == NULL) {
        Log("Fatal Error: AdvanceIndex:: mNodeCurrent missing (%d) / (%d)\n", mCurrentNodeIndex, mNodeList.mCount);
        mCurrentNodeIndex = 0;
        return;
    }

    mOffsetX = mNodeCurrent->mOffsetX;
    mOffsetY = mNodeCurrent->mOffsetY;
    mRotation = mNodeCurrent->mRotation;
    
}

void LevelMotionControllerSliceClockwork::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    //if ((pX != NULL) && (pY != NULL) && (mRotatesObject == false)) {
    if ((pX != NULL) && (pY != NULL)) {
        FVec2 aPos = FVec2(*pX, *pY);
        FVec2 aCenter = FVec2(pReferenceX, pReferenceY);
        aPos = PivotPoint(aPos, mRotation, aCenter);
        *pX = aPos.mX + mOffsetX;
        *pY = aPos.mY + mOffsetY;
    }
}

float LevelMotionControllerSliceClockwork::ApplyToObjectRotation() {
    return mRotation;
}

void LevelMotionControllerSliceClockwork::ApplyStartingCondition(int pStartNode, bool pPauseStart) {

    if (mNodeList.mCount < 2) {
        Log("Fatal Error: ApplyStartingCondition::LevelMotionControllerSliceClockwork < 2\n");
        return;
    }

    if (pStartNode < 0) { pStartNode = 0; }
    if (pStartNode >= mNodeList.mCount) { pStartNode = 0; }
    
    
    mCurrentNodeIndex = pStartNode;
    
    LevelMotionControllerSliceClockworkNode *aNode = (LevelMotionControllerSliceClockworkNode *)mNodeList.Fetch(mCurrentNodeIndex);
    
    mOffsetX = aNode->mOffsetX;
    mOffsetY = aNode->mOffsetY;
    mRotation = aNode->mRotation;
    
    if ((pPauseStart == true) && (aNode->mPause > 0)) {
        mPauseTimer = aNode->mPause;
    }
    
    
    /*
    
    if (mPauseStartType == 1) {
        mPauseTimer = mPause1;
        mLinearDir = -1;
        mLinearTimer = mLinearTime;
    }
    
    if (mPauseStartType == 2) {
        mPauseTimer = mPause2;
        mLinearDir = 1;
        mLinearTimer = 0;
    }
     
    */
    
}


