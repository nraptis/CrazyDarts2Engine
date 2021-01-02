//
//  LevelPath.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/15/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelPath.hpp"
#include "os_core_graphics.h"
#include "FSpline.hpp"
#include "FPolyPath.hpp"
#include "core_includes.h"

static FPointList cPointList;
static FPointList cDumpList;
static FPolyPath cPolyPath;
static FPointList cSegmentList;

LevelPathNode::LevelPathNode() {
    mX = 0.0f;
    mY = 0.0f;
    mType = PATH_NODE_INVALID;
    mWaitTimer = 0;
    mDecelDistance = 0;
    mKillTimer = 8;
}

LevelPathNode::~LevelPathNode() {
    
}

LevelPath::LevelPath() {
    mSpeed = 3.0f;
    mDidFinalize = false;
    mDidFailFinalize = false;
    
    mSmooth = false;
    mClosed = false;
    
    mTempDist = 0.0f;
    
    mStartWaitIndex = -1;
    
    mStartConstraintTypeX = X_CONSTRAINT_NONE;
    mStartConstraintTypeY = Y_CONSTRAINT_NONE;
    
    mEndConstraintTypeX = X_CONSTRAINT_NONE;
    mEndConstraintTypeY = Y_CONSTRAINT_NONE;
    
    
    mDrawOffsetX = 0.0f;
    mDrawOffsetY = 0.0f;
    
    
    mContainsEnterAndExit = false;
    
    mClockTime = 0;
    
    mEnterIndex = -1;
    mExitIndex = -1;

    mTempX = 0.0f;
    mTempY = 0.0f;
    mTempDist = 0.0f;
}

LevelPath::~LevelPath() {
    FreeList(LevelPathNode, mNodeList);
    FreeList(LevelPathNode, mKillList);
}


void LevelPath::Update() {
    EnumList(LevelPathNode, aNode, mKillList) {
        aNode->mKillTimer--;
        if (aNode->mKillTimer <= 0) { mDeleteList.Add(aNode); }
    }
    EnumList(LevelPathNode, aNode, mDeleteList) {
        mKillList.Remove(aNode);
        delete aNode;
    }
    mDeleteList.RemoveAll();
}

void LevelPath::Add(int pType, float pX, float pY, int pAccel, int pDecel, int pWait) {
    LevelPathNode *aNode = new LevelPathNode();
    aNode->mX = pX;
    aNode->mY = pY;
    aNode->mType = pType;
    aNode->mAccelDistance = pAccel;
    aNode->mDecelDistance = pDecel;
    aNode->mWaitTimer = pWait;
    mNodeList.Add(aNode);
    mDidFailFinalize = false;
    mDidFinalize = false;
    mClockTime += pWait;
}

void LevelPath::AddMove(float pX, float pY, int pAccel, int pDecel, int pWait) {
    Add(PATH_NODE_NORMAL, pX, pY, pAccel, pDecel, pWait);
}

void LevelPath::Shift(float pShiftX, float pShiftY) {
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelPathNode *aNode = ((LevelPathNode *)mNodeList.mData[i]);
        aNode->mX += pShiftX;
        aNode->mY += pShiftY;
    }
}

void LevelPath::Reset() {
    mDidFinalize = false;
    mDidFailFinalize = false;
    mClockTime = 0;
    mStartWaitIndex = -1;
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelPathNode *aNode = ((LevelPathNode *)mNodeList.mData[i]);
        mKillList.Add(aNode);
    }
    mNodeList.RemoveAll();
    mPath.RemoveAll();
    mDist.RemoveAll();
    mWait.RemoveAll();
}

void LevelPath::Finalize() {
    mPath.RemoveAll();
    mDist.RemoveAll();
    mWait.RemoveAll();
    mPlayEnter.RemoveAll();
    mPlayExit.RemoveAll();
    
    mDidFailFinalize = false;
    mStartWaitIndex = -1;
    
    if (mSpeed < 0.4f) {
        mSpeed = 0.4f;
    }
    
    if (mNodeList.mCount < 2) {
        mDidFailFinalize = true;
        return;
    }
    
    mContainsEnterAndExit = false;
    mEnterIndex = -1;
    mExitIndex = -1;
    
    if (mClosed == false) {
        for (int i=0;(i<mNodeList.mCount) && (mEnterIndex == -1);i++) {
            LevelPathNode *aNode = ((LevelPathNode *)mNodeList.mData[i]);
            if (aNode->mWaitTimer > 0) {
                mEnterIndex = i;
            }
        }
        for (int i=(mNodeList.mCount - 1);(i>=0) && (mExitIndex == -1);i--) {
            LevelPathNode *aNode = ((LevelPathNode *)mNodeList.mData[i]);
            if (aNode->mWaitTimer > 0) {
                mExitIndex = i;
            }
        }
    }
    
    if (mEnterIndex != -1) { mContainsEnterAndExit = true; }
    
    
    LevelPathNode *aFirstNode = ((LevelPathNode *)mNodeList.First());
    float aStartX = aFirstNode->mX;
    float aStartY = aFirstNode->mY;
    
    mTempX = aStartX;
    mTempY = aStartY;
    mTempDist = 0.0f;
    
    int aIndex = 0;
    
    mDidFinalize = true;
    
    int aMax = mNodeList.mCount;
    if (mClosed) {
        aMax += 1;
    }
    
    while (aIndex < aMax) {
        LevelPathNode *aNode = ((LevelPathNode *)mNodeList.FetchCircular(aIndex));
        if (aIndex > 0) {
            if ((aNode->mWaitTimer > 0) || (aIndex == (aMax - 1))) {
                LevelPath::AddSegmentBacktrackingFrom(aIndex);
            }
        }
        if (aNode->mWaitTimer > 0) {
            if (mWait.mCount > 0) {
                mWait.mData[mWait.mCount - 1] = aNode->mWaitTimer;
                
                mPlayEnter.mData[mPlayEnter.mCount - 1] = 0;
                mPlayExit.mData[mPlayExit.mCount - 1] = 0;
            }
        }
        
        ++aIndex;
    }
    
    mStartWaitIndex = -1;
    
    if (mWait.mCount > 0) {
        if (aFirstNode->mWaitTimer > 0) {
            mWait.mData[0] = aFirstNode->mWaitTimer;
            if (mClosed) {
                //We are already factoring in the wait for 0...
                mWait.mData[mWait.mCount - 1] = 0;
            }
        }
        
        for (int i=0;(i < mWait.mCount) && (mStartWaitIndex == -1);i++) {
            if (mWait.mData[i] != 0) {
                mStartWaitIndex = i;
            }
        }
    }
}

void LevelPath::AddSegmentBacktrackingFrom(int pIndex) {
    
    int aStartIndex = pIndex - 1;
    while (aStartIndex > 0) {
        LevelPathNode *aNode = ((LevelPathNode *)mNodeList.mData[aStartIndex]);
        if (aNode->mWaitTimer > 0) {
            break;
        } else {
            --aStartIndex;
        }
    }
    
    int aPlayEnter = 0;
    int aPlayExit = 0;
    
    if (mContainsEnterAndExit == true) {
        
        if (aStartIndex < mEnterIndex) { aPlayEnter = 1; }
        if (pIndex > mExitIndex) { aPlayExit = 1; }
        
    }
    
    
    LevelPathNode *aPrev = ((LevelPathNode *)mNodeList.mData[aStartIndex]);
    
    int aAccelDistance = 0;
    if (aPrev != NULL) {
        aAccelDistance = aPrev->mAccelDistance;
    }
    
    cPointList.Reset();
    
    cPointList.Add(aPrev->mX, aPrev->mY);
    
    mTempX = aPrev->mX;
    mTempY = aPrev->mY;
    
    float aTotalDist = 0.0f;
    
    int aDecelDistance = 0;
    
    for (int i=aStartIndex+1;i<=pIndex;i++) {
        LevelPathNode *aNode = ((LevelPathNode *)mNodeList.FetchCircular(i));
        
        float aDiffX = aNode->mX - aPrev->mX;
        float aDiffY = aNode->mY - aPrev->mY;
        
        float aDist = aDiffX * aDiffX + aDiffY * aDiffY;
        aTotalDist += aDist;
        
        if (aDist > 1.0f) {
            cPointList.Add(aNode->mX, aNode->mY);
        }
        
        aDecelDistance = aNode->mDecelDistance;
        aPrev = aNode;
    }
    
    if (aTotalDist <= 0) {
        Log("BAD PATH CHUNK [LEN = %f]\n", aTotalDist);
        return;
    }
    
    cDumpList.Reset();
    
    float aX = 0.0f;
    float aY = 0.0f;
    
    if (mSmooth == false || cPointList.mCount <= 2) {
        for (int i=0;i<cPointList.mCount;i++) {
            cDumpList.Add(cPointList.mX[i], cPointList.mY[i]);
        }
    } else {
        FSpline aSpline;
        
        if (mClosed == true && aStartIndex == 0 && pIndex == mNodeList.mCount) {
            for (int i=0;i<(cPointList.mCount-1);i++) {
                aSpline.Add(cPointList.mX[i], cPointList.mY[i]);
            }
            aSpline.Solve(false, true);
        } else {
            for (int i=0;i<cPointList.mCount;i++) {
                aSpline.Add(cPointList.mX[i], cPointList.mY[i]);
            }
            aSpline.Solve(false, false);
        }
        
        for (float aPos=0.0f;aPos<=aSpline.Max();aPos += 0.05f) {
            aSpline.Get(aPos, aX, aY);
            cDumpList.Add(aX, aY);
        }
        cDumpList.Add(aSpline.GetX(aSpline.Max()), aSpline.GetY(aSpline.Max()));
    }
    
    
    
    if (cDumpList.mCount <= 1) {
        Log("Fatal Error, Point Count[%d]\n", cDumpList.mCount);
        return;
    }
    
    cPolyPath.RemoveAll();
    cPolyPath.Add(&cDumpList);
    cPolyPath.Generate();
    
    bool aDecelerationEnabled = true;
    
    //float aDecelerationCutoffDistance = 118.0f;
    float aDecelerationCutoff = cPolyPath.mLength - ((float)aDecelDistance);
    float aDeceleration = 0.0f;
    
    bool aAccelerationEnabled = true;
    //float aAccelerationDistance = 60.0f;
    float aAccelerationSpeed = 0.0f;
    float aAcceleration = 0.0f;
    
    float aSpeed = mSpeed;
    if (aSpeed > 100.0f) { aSpeed = 100.0f; }
    if (aSpeed < 0.4f) { aSpeed = 0.4f; }
    
    if (cPolyPath.mLength < ((float)aDecelDistance) || aDecelDistance < 10) {
        aDecelerationEnabled = false;
    } else {
        aDeceleration = (aSpeed * aSpeed) / (2.0f * ((float)aDecelDistance));
    }
    
    if (cPolyPath.mLength < aAccelDistance || aAccelDistance < 10) {
        aAccelerationEnabled = false;
    } else {
        aAcceleration = (aSpeed * aSpeed) / (2.0f * ((float)aAccelDistance));
    }
    
    cSegmentList.RemoveAll();
    cSegmentList.Add(cDumpList.mX[0], cDumpList.mY[0]);
    mDist.Add(mTempDist);
    mWait.Add(0);
    mPlayEnter.Add(aPlayEnter);
    mPlayExit.Add(aPlayExit);
    
    
    float aCurrentDist = 0.00f;
    while (aSpeed > 0.05f && aCurrentDist < cPolyPath.mLength) {
        
        if (aDecelerationEnabled == true && aCurrentDist > aDecelerationCutoff) {
            aCurrentDist += aSpeed;
            aSpeed -= aDeceleration;
        } else if (aAccelerationEnabled == true && aCurrentDist < ((float)aAccelDistance)) {
            aAccelerationSpeed += aAcceleration;
            if (aAccelerationSpeed > aSpeed) {
                aAccelerationSpeed = aSpeed;
            }
            aCurrentDist += aAccelerationSpeed;
        } else {
            aCurrentDist += aSpeed;
        }
        
        float aInterpX = 0.0f;
        float aInterpY = 0.0f;
        cPolyPath.GetWithDist(aCurrentDist, aInterpX, aInterpY);
        cSegmentList.Add(aInterpX, aInterpY);
        mDist.Add(mTempDist + aCurrentDist);
        mWait.Add(0);
        mPlayEnter.Add(aPlayEnter);
        mPlayExit.Add(aPlayExit);
        
    }
    
    float aLastX = cDumpList.mX[cDumpList.mCount - 1];
    float aLastY = cDumpList.mY[cDumpList.mCount - 1];
    float aSegmentLastX = 0.0f;
    float aSegmentLastY = 0.0f;
    if (cSegmentList.mCount > 0) {
        aSegmentLastX = cSegmentList.mX[cSegmentList.mCount - 1];
        aSegmentLastY = cSegmentList.mY[cSegmentList.mCount - 1];
    }
    
    float aDistanceToEnd = DistanceSquared(aLastX, aLastY, aSegmentLastX, aSegmentLastY);
    
    if (aDistanceToEnd > 0.5f) {
        cSegmentList.Add(aLastX, aLastY);
        mDist.Add(aCurrentDist + cPolyPath.mLength);
        mWait.Add(0);
        mPlayEnter.Add(aPlayEnter);
        mPlayExit.Add(aPlayExit);
        
    } else {
        cSegmentList.mX[cSegmentList.mCount - 1] = aLastX;
        cSegmentList.mY[cSegmentList.mCount - 1] = aLastY;
        mDist.mData[mDist.mCount - 1] = mTempDist + cPolyPath.mLength;
    }
    
    mTempX = aLastX;
    mTempY = aLastY;
    
    mTempDist += cPolyPath.mLength;
    
    mPath.Add(&cSegmentList);
    
    cSegmentList.RemoveAll();
    cPolyPath.RemoveAll();
}

void LevelPath::GenerateLoops(int pRepeatCount) {
    
    if (pRepeatCount <= 0) { return; }
    if (pRepeatCount > 25) { pRepeatCount = 25; }
    
    int aOriginalSize = mPath.mCount;
    
    if (aOriginalSize <= 0) {
        Log("GenerateLoops::Fatal Error: aOriginalSize <= 0\n");
        return;
    }
    
    if (mPath.mCount != mDist.mCount) {
        Log("GenerateLoops::Fatal Error: mPath.mCount != mDist.mCount\n");
        return;
    }
    
    if (mPath.mCount != mWait.mCount) {
        Log("GenerateLoops::Fatal Error: mPath.mCount != mWait.mCount\n");
        return;
    }
    
    if (mPath.mCount != mPlayEnter.mCount) {
        Log("GenerateLoops::Fatal Error: mPath.mCount != mPlayEnter.mCount\n");
        return;
    }
    
    if (mPath.mCount != mPlayExit.mCount) {
        Log("GenerateLoops::Fatal Error: mPath.mCount != mPlayExit.mCount\n");
        return;
    }
    
    
    for (int aRepeatIndex=0;aRepeatIndex<pRepeatCount;aRepeatIndex++) {
        
        for (int i=0;i<aOriginalSize;i++) {
            mPath.Add(mPath.mX[i], mPath.mY[i]);
        }
        
        for (int i=0;i<aOriginalSize;i++) {
            mDist.Add(mDist.mData[i]);
        }
        
        for (int i=0;i<aOriginalSize;i++) {
            mWait.Add(mWait.mData[i]);
        }
        
        for (int i=0;i<aOriginalSize;i++) {
            mPlayEnter.Add(mPlayEnter.mData[i]);
        }
        
        for (int i=0;i<aOriginalSize;i++) {
            mPlayExit.Add(mPlayExit.mData[i]);
        }
        
    }
    
    //FPointList                          mPath;
    //FFloatList                          mDist;
    //FIntList                            mWait;
    //FIntList                            mPlayEnter;
    //FIntList                            mPlayExit;
    
    
    
}


void LevelPath::Draw() {
    
    if (mDidFinalize == false && mDidFailFinalize == false) {
        Finalize();
    }
    
    LevelPathNode *aPrev = NULL;
    LevelPathNode *aNode = NULL;
    
    //mDrawOffsetX = 0.0f;
    //mDrawOffsetY = 0.0f;
    
    
    Graphics::PipelineStateSetShape2DAlphaBlending();
    for (int i=0;i<mNodeList.mCount;i++) {
        aNode = ((LevelPathNode *)mNodeList.mData[i]);
        if (aPrev) {
            
            Graphics::SetColor(0.44f, 0.66f, 0.125f, 0.15f);
            Graphics::DrawLine(aPrev->mX + mDrawOffsetX, aPrev->mY + mDrawOffsetY, aNode->mX + mDrawOffsetX, aNode->mY + mDrawOffsetY, 2.5f);
            
            Graphics::SetColor(0.66f, 0.66f, 0.66f, 0.15f);
            Graphics::DrawLine(aPrev->mX + mDrawOffsetX, aPrev->mY + mDrawOffsetY, aNode->mX + mDrawOffsetX, aNode->mY + mDrawOffsetY, 1.5f);
        }
        
        aPrev = aNode;
    }
    
    for (int i=0;i<mNodeList.mCount;i++) {
        aNode = ((LevelPathNode *)mNodeList.mData[i]);
        
        if (aNode->mWaitTimer > 0) {
            Graphics::SetColor(0.125f, 0.25f, 0.88f, 0.5f);
            Graphics::OutlineRect(aNode->mX - 25.0f + mDrawOffsetX, aNode->mY - 25.0f + mDrawOffsetY, 50.0f, 50.0f, 3.0f);
        }
        
        if (aNode->mType == PATH_NODE_NORMAL) {
            Graphics::SetColor(0.125f, 0.88f, 0.125f, 0.6f);
            Graphics::OutlineRect(aNode->mX - 8.0f + mDrawOffsetX, aNode->mY - 8.0f + mDrawOffsetY, 16.0f, 16.0f, 3.0f);
        }
    }
    
    
    //mPath.DrawEdgesOpen();
    
    if (mPath.mCount > 0) {
        float aLastX = mPath.mX[0] + mDrawOffsetX, aLastY = mPath.mY[0] + mDrawOffsetY;
        float aX = 0.0f, aY = 0.0f;
        for (int i=1;i<mPath.mCount;i++) {
            
            float aRed = 1.0f;
            float aGreen = 1.0f;
            
            if (mPlayEnter.mData[i] != 0) { aRed = 0.25f; }
            if (mPlayExit.mData[i] != 0) { aGreen = 0.25f; }
            
            Graphics::SetColor(aRed, aGreen, 0.5f, 1.0f);
            
            
            aX = mPath.mX[i] + mDrawOffsetX;
            aY = mPath.mY[i] + mDrawOffsetY;
            Graphics::DrawLine(aLastX, aLastY, aX, aY, 1.0f);
            aLastX = aX;
            aLastY = aY;
        }
    }
}

void LevelPath::SetSpeedClass(int pSpeedClass) {
    if (pSpeedClass == SPEED_CLASS_EXTRA_SLOW) {
        mSpeed = 0.60f;
    } else if (pSpeedClass == SPEED_CLASS_SLOW) {
        mSpeed = 0.90f;
    } else if (pSpeedClass == SPEED_CLASS_MEDIUM_SLOW) {
        mSpeed = 1.40f;
    } else if (pSpeedClass == SPEED_CLASS_MEDIUM_FAST) {
        mSpeed = 1.85f;
    } else if (pSpeedClass == SPEED_CLASS_FAST) {
        mSpeed = 3.25f;
    } else if (pSpeedClass == SPEED_CLASS_EXTRA_FAST) {
        mSpeed = 4.5f;
    } else if (pSpeedClass == SPEED_CLASS_INSANE) {
        mSpeed = 6.0f;
    } else { //"Default /
        mSpeed = 2.25f;
    }
}
