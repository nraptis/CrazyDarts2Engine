//
//  LevelFormationTracerTracer.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/31/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelFormationTracer.hpp"
#include "LevelFormationNode.hpp"
#include "os_core_graphics.h"
#include "FPolyPath.hpp"
#include "LevelPath.hpp"
#include "Game.hpp"
#include "core_includes.h"

static FPointList cDumpList;
static FPolyPath cPolyPath;
static FPointList cSegmentList;


LevelFormationTracer::LevelFormationTracer(LevelFormation *pFormation) {
    
    SetSpeedClass(SPEED_CLASS_MEDIUM);
    
    mFormation = pFormation;
    mPathIndex = 0;
    
    mSpawnPathOffset = 0;
    
    mSpecialType = TRACER_SPECIAL_TYPE_NONE;
    
    mSpecialX = 0.0f;
    mSpecialY = 0.0f;
    
    mSpecialRadius = 256.0f;
    mSpecialWidth = 256.0f;
    mSpecialHeight = 256.0f;
    
    mSection = NULL;
    
    mSpecialCornerRadius = 60.0f;
    
    mDidOriginateOnWave = false;
    mDidOriginateAsPermanent = false;
    
    mPlayOnEnter = false;
    mPlayOnExit = false;
    
    mReverse = false;
    
    mStopped = false;
    
    mKillTimer = 8;
}

LevelFormationTracer::~LevelFormationTracer() {
    
    FreeList(LevelFormationTracerNode, mTracerNodeList);
    FreeList(LevelFormationTracerNode, mTracerNodeKillList);
    
    FreeList(LevelFormationNode, mSpawnNodeList);
    FreeList(LevelFormationNode, mSpawnNodeKillList);
}

void LevelFormationTracer::Reset() {
    for (int i=0;i<mTracerNodeList.mCount;i++) {
        LevelFormationTracerNode *aNode = ((LevelFormationTracerNode *)mTracerNodeList.mData[i]);
        mTracerNodeKillList.Add(aNode);
    }
    mTracerNodeList.RemoveAll();
    
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->Reset();
        mSpawnNodeKillList.Add(aNode);
    }
    mSpawnNodeList.RemoveAll();
    
    mPath.RemoveAll();
    mDist.RemoveAll();
}

void LevelFormationTracer::DisposeObject(GameObject *pObject) {
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->DisposeObject(pObject);
    }
}

void LevelFormationTracer::DisposeAllObjects() {
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->DisposeAllObjects();
    }
}


bool LevelFormationTracer::ContainsObject(GameObject *pObject) {
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        if (aNode->ContainsObject(pObject) == true) {
            return true;
        }
    }
    return true;
}

void LevelFormationTracer::RefreshSection(LevelSection *pSection) {
    mSection = pSection;
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->RefreshSection(pSection);
    }
}

void LevelFormationTracer::Spawn() {
    
    //In-case we have pre-spawned... Should not have..!
    
    EnumList(LevelFormationTracerNode, aNode, mTracerNodeList) {
        aNode->mSection = mSection;
        aNode->SetUp();
    }
    
    cDumpList.Reset();
    
    if (mSpecialType == TRACER_SPECIAL_TYPE_NONE) {
        for (int i=0;i<mTracerNodeList.mCount;i++) {
            LevelFormationTracerNode *aNode = ((LevelFormationTracerNode *)mTracerNodeList.mData[i]);
            cDumpList.Add(aNode->mX, aNode->mY);
        }
    } else {
        
        float aLeft = gGame->mGameAreaLeft;
        float aRight = gGame->mGameAreaRight;
        float aTop = gGame->mGameAreaTop;
        float aBottom = gGame->mGameAreaBottom;
        float aWidth = aRight - aLeft;
        float aHeight = aBottom - aTop;
        float aCenterX = (mSpecialX / 100.0f) * aWidth;
        float aCenterY = (mSpecialY / 100.0f) * aHeight;
        
        if (mSpecialType == TRACER_SPECIAL_TYPE_CIRCLE) { AddDumpPointsCircle(aCenterX, aCenterY); }
        if (mSpecialType == TRACER_SPECIAL_TYPE_ROUNDED_RECT) { AddDumpPointsRoundedRect(aCenterX, aCenterY); }
    }
    
    
    if (cDumpList.mCount <= 1) {
        Log("Fatal Error, Point Count[%d]\n", cDumpList.mCount);
        return;
    }
    
    cPolyPath.mClosed = true;
    cPolyPath.RemoveAll();
    cPolyPath.Add(&cDumpList);
    cPolyPath.Generate();
    
    //IMPORTANT: Tracers always loop and thus "closed" !!!
    
    float aSpeed = mSpeed;
    if (aSpeed > 10.0f) { aSpeed = 10.0f; }
    if (aSpeed < 0.125f) { aSpeed = 0.125f; }
    
    cSegmentList.RemoveAll();
    cSegmentList.Add(cDumpList.mX[0], cDumpList.mY[0]);
    mDist.Add(0.0f);
    
    float aCurrentDist = 0.00f;
    while (aCurrentDist < cPolyPath.mLength) {
        aCurrentDist += aSpeed;
        float aInterpX = 0.0f; float aInterpY = 0.0f;
        cPolyPath.GetWithDist(aCurrentDist, aInterpX, aInterpY);
        cSegmentList.Add(aInterpX, aInterpY);
        mDist.Add(aCurrentDist);
    }
    
    //IMPORTANT: Loop back to start...
    float aLastX = cDumpList.mX[0];
    float aLastY = cDumpList.mY[0];
    float aSegmentLastX = 0.0f;
    float aSegmentLastY = 0.0f;
    if (cSegmentList.mCount > 0) {
        aSegmentLastX = cSegmentList.mX[cSegmentList.mCount - 1];
        aSegmentLastY = cSegmentList.mY[cSegmentList.mCount - 1];
    }
    
    //If we land too close to the start, remove the point.
    float aDistanceToEnd = DistanceSquared(aLastX, aLastY, aSegmentLastX, aSegmentLastY);
    if (aDistanceToEnd <= ((aSpeed * aSpeed) * 0.5f)) {
        cSegmentList.mCount -= 1;
        mDist.mCount -= 1;
    }
    
    
    if (cSegmentList.mCount <= 2) { return; }
    
    mPath.Add(&cSegmentList);
    
    
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    
    int aPathCount = mPath.mCount;
    
    int aSpawnIndex = 0;
    EnumList(LevelFormationNode, aFormationNode, mSpawnNodeList) {
        aFormationNode->mTracer = this;
        aFormationNode->mFormation = mFormation;
        
        aFormationNode->mDidOriginateOnWave = mDidOriginateOnWave;
        aFormationNode->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        
        float aPercent = ((float)aSpawnIndex) / ((float)mSpawnNodeList.mCount);
        aFormationNode->mPathIndexOffset = (int)round(aPercent * ((float)aPathCount));
        
        if (aPathCount > 0) {
            
            aFormationNode->mPathIndexOffset = (aFormationNode->mPathIndexOffset + mSpawnPathOffset) % aPathCount;
            if (aFormationNode->mPathIndexOffset < 0) {
                aFormationNode->mPathIndexOffset += aPathCount;
            }
        }
        
        
        aFormationNode->Spawn();
        ++aSpawnIndex;
    }
    
}

void LevelFormationTracer::EditorPickerSpawn() {
    EnumList(LevelFormationTracerNode, aNode, mTracerNodeList) {
        aNode->mSection = mSection;
        aNode->SetUp();
    }
    
    cDumpList.Reset();
    
    if (mSpecialType == TRACER_SPECIAL_TYPE_NONE) {
        for (int i=0;i<mTracerNodeList.mCount;i++) {
            LevelFormationTracerNode *aNode = ((LevelFormationTracerNode *)mTracerNodeList.mData[i]);
            cDumpList.Add(aNode->mX, aNode->mY);
        }
    } else {
        
        float aLeft = gGame->mGameAreaLeft;
        float aRight = gGame->mGameAreaRight;
        float aTop = gGame->mGameAreaTop;
        float aBottom = gGame->mGameAreaBottom;
        float aWidth = aRight - aLeft;
        float aHeight = aBottom - aTop;
        float aCenterX = (mSpecialX / 100.0f) * aWidth;
        float aCenterY = (mSpecialY / 100.0f) * aHeight;
        
        if (mSpecialType == TRACER_SPECIAL_TYPE_CIRCLE) { AddDumpPointsCircle(aCenterX, aCenterY); }
        if (mSpecialType == TRACER_SPECIAL_TYPE_ROUNDED_RECT) { AddDumpPointsRoundedRect(aCenterX, aCenterY); }
    }
    
    
    if (cDumpList.mCount <= 1) {
        Log("Fatal Error, Point Count[%d]\n", cDumpList.mCount);
        return;
    }
    
    cPolyPath.mClosed = true;
    cPolyPath.RemoveAll();
    cPolyPath.Add(&cDumpList);
    cPolyPath.Generate();
    
    //IMPORTANT: Tracers always loop and thus "closed" !!!
    
    float aSpeed = mSpeed;
    if (aSpeed > 10.0f) { aSpeed = 10.0f; }
    if (aSpeed < 0.125f) { aSpeed = 0.125f; }
    
    cSegmentList.RemoveAll();
    cSegmentList.Add(cDumpList.mX[0], cDumpList.mY[0]);
    mDist.Add(0.0f);
    
    float aCurrentDist = 0.00f;
    while (aCurrentDist < cPolyPath.mLength) {
        aCurrentDist += aSpeed;
        float aInterpX = 0.0f;
        float aInterpY = 0.0f;
        cPolyPath.GetWithDist(aCurrentDist, aInterpX, aInterpY);
        cSegmentList.Add(aInterpX, aInterpY);
        mDist.Add(aCurrentDist);
    }
    
    //IMPORTANT: Loop back to start...
    float aLastX = cDumpList.mX[0];
    float aLastY = cDumpList.mY[0];
    float aSegmentLastX = 0.0f;
    float aSegmentLastY = 0.0f;
    if (cSegmentList.mCount > 0) {
        aSegmentLastX = cSegmentList.mX[cSegmentList.mCount - 1];
        aSegmentLastY = cSegmentList.mY[cSegmentList.mCount - 1];
    }
    
    //If we land too close to the start, remove the point.
    float aDistanceToEnd = DistanceSquared(aLastX, aLastY, aSegmentLastX, aSegmentLastY);
    if (aDistanceToEnd <= ((aSpeed * aSpeed) * 0.5f)) {
        cSegmentList.mCount -= 1;
        mDist.mCount -= 1;
    }
    
    
    if (cSegmentList.mCount <= 2) { return; }
    
    mPath.Add(&cSegmentList);
    
    
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    
    int aPathCount = mPath.mCount;
    
    int aSpawnIndex = 0;
    EnumList(LevelFormationNode, aFormationNode, mSpawnNodeList) {
        aFormationNode->mTracer = this;
        aFormationNode->mFormation = mFormation;
        
        aFormationNode->mDidOriginateOnWave = mDidOriginateOnWave;
        aFormationNode->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        
        float aPercent = ((float)aSpawnIndex) / ((float)mSpawnNodeList.mCount);
        aFormationNode->mPathIndexOffset = (int)round(aPercent * ((float)aPathCount));
        
        if (aPathCount > 0) {
            
            aFormationNode->mPathIndexOffset = (aFormationNode->mPathIndexOffset + mSpawnPathOffset) % aPathCount;
            if (aFormationNode->mPathIndexOffset < 0) {
                aFormationNode->mPathIndexOffset += aPathCount;
            }
        }
        
        
        aFormationNode->EditorPickerSpawn();
        ++aSpawnIndex;
    }
}

void LevelFormationTracer::Update() {
    
    //mTracerPlayOnEnter = false;
    //mTracerPlayOnExit = false;
    
    if ((mPath.mCount > 2) && (mStopped == false)) {
        if (mReverse == true) {
            mPathIndex -= 1;
            if (mPathIndex < 0) {
                mPathIndex = mPath.mCount - 1;
            }
        } else {
            mPathIndex += 1;
            if (mPathIndex >= (mPath.mCount)) {
                mPathIndex = 0;
            }
        }
    }
    
    EnumList(LevelFormationTracerNode, aNode, mTracerNodeList) {
        aNode->Update();
    }
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->Update();
    }
    
    EnumList(LevelFormationTracerNode, aNode, mTracerNodeKillList) {
        aNode->mKillTimer--;
        if (aNode->mKillTimer <= 0) { mTracerNodeDeleteList.Add(aNode); }
    }
    
    EnumList(LevelFormationTracerNode, aNode, mTracerNodeDeleteList) {
        mTracerNodeKillList.Remove(aNode);
        delete aNode;
    }
    
    mTracerNodeDeleteList.RemoveAll();
}

void LevelFormationTracer::DrawDeadObjectMarkers() {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->DrawDeadObjectMarkers();
    }
}

void LevelFormationTracer::AddDumpPointsCircle(float pX, float pY) {
    float aDirX = 0.0f;
    float aDirY = 0.0f;
    for (float aAngle=0.0f;aAngle<=360.0f;aAngle+=1.0f) {
        aDirX = Sin(aAngle);
        aDirY = -Cos(aAngle);
        cDumpList.Add(pX + aDirX * mSpecialRadius, pY + aDirY * mSpecialRadius);
    }
}

void LevelFormationTracer::AddDumpPointsRoundedRect(float pX, float pY) {
    
    float aSizeH = mSpecialWidth;
    float aSizeV = mSpecialHeight;
    
    float aLineLengthH = aSizeH - (mSpecialCornerRadius * 2.0f);
    float aLineLengthV = aSizeV - (mSpecialCornerRadius * 2.0f);
    
    float aDirX = 0.0f;
    float aDirY = 0.0f;
    
    float aStartX = pX - aLineLengthH / 2.0f;
    float aStartY = pY - aSizeV / 2.0f;
    float aEndX = pX + aLineLengthH / 2.0f;
    float aEndY = pY - aSizeV / 2.0f;
    
    cDumpList.Add(aStartX, aStartY);
    cDumpList.Add(aEndX, aEndY);
    
    float aCircleCenterX = aEndX;
    float aCircleCenterY = aEndY + mSpecialCornerRadius;
    
    for (float aAngle=0.0f + 2.0f;aAngle<=90.0f - 2.0f;aAngle+=2.0f) {
        aDirX = Sin(aAngle);
        aDirY = -Cos(aAngle);
        cDumpList.Add(aCircleCenterX + aDirX * mSpecialCornerRadius, aCircleCenterY + aDirY * mSpecialCornerRadius);
    }
    
    aStartX = pX + aSizeH / 2.0f;
    aStartY = pY - aLineLengthV / 2.0f;
    aEndX = pX + aSizeH / 2.0f;
    aEndY = pY + aLineLengthV / 2.0f;
    
    cDumpList.Add(aStartX, aStartY);
    cDumpList.Add(aEndX, aEndY);
    
    aCircleCenterX = aEndX - mSpecialCornerRadius;
    aCircleCenterY = aEndY;
    
    for (float aAngle=90.0f + 2.0f;aAngle<=180.0f - 2.0f;aAngle+=2.0f) {
        aDirX = Sin(aAngle);
        aDirY = -Cos(aAngle);
        cDumpList.Add(aCircleCenterX + aDirX * mSpecialCornerRadius, aCircleCenterY + aDirY * mSpecialCornerRadius);
    }
    
    aStartX = pX + aLineLengthH / 2.0f;
    aStartY = pY + aSizeV / 2.0f;
    aEndX = pX - aLineLengthH / 2.0f;
    aEndY = pY + aSizeV / 2.0f;
    
    cDumpList.Add(aStartX, aStartY);
    cDumpList.Add(aEndX, aEndY);
    
    
    aCircleCenterX = aEndX;
    aCircleCenterY = aEndY - mSpecialCornerRadius;
    
    for (float aAngle=180.0f + 2.0f;aAngle<=270.0f - 2.0f;aAngle+=2.0f) {
        aDirX = Sin(aAngle);
        aDirY = -Cos(aAngle);
        cDumpList.Add(aCircleCenterX + aDirX * mSpecialCornerRadius, aCircleCenterY + aDirY * mSpecialCornerRadius);
    }
    
    aStartX = pX - aSizeH / 2.0f;
    aStartY = pY + aLineLengthV / 2.0f;
    aEndX = pX - aSizeH / 2.0f;
    aEndY = pY - aLineLengthV / 2.0f;
    
    cDumpList.Add(aStartX, aStartY);
    cDumpList.Add(aEndX, aEndY);
    
    aCircleCenterX = aEndX + mSpecialCornerRadius;
    aCircleCenterY = aEndY;
    
    for (float aAngle=270.0f + 2.0f;aAngle<=360.0f - 2.0f;aAngle+=2.0f) {
        aDirX = Sin(aAngle);
        aDirY = -Cos(aAngle);
        cDumpList.Add(aCircleCenterX + aDirX * mSpecialCornerRadius, aCircleCenterY + aDirY * mSpecialCornerRadius);
    }
}

void LevelFormationTracer::SetSpeedClass(int pSpeedClass) {
    if (pSpeedClass == SPEED_CLASS_EXTRA_SLOW) {
        mSpeed = 0.25f;
    } else if (pSpeedClass == SPEED_CLASS_SLOW) {
        mSpeed = 0.40f;
    } else if (pSpeedClass == SPEED_CLASS_MEDIUM_SLOW) {
        mSpeed = 0.85f;
    } else if (pSpeedClass == SPEED_CLASS_MEDIUM_FAST) {
        mSpeed = 1.85f;
    } else if (pSpeedClass == SPEED_CLASS_FAST) {
        mSpeed = 2.5f;
    } else if (pSpeedClass == SPEED_CLASS_EXTRA_FAST) {
        mSpeed = 4.65f;
    } else if (pSpeedClass == SPEED_CLASS_INSANE) {
        mSpeed = 7.15f;
    } else { //"Default /
        mSpeed = 1.15f;
    }
}

LevelFormationTracer *LevelFormationTracer::Clone(LevelFormation *pFormation) {
    
    LevelFormationTracer *aClone = new LevelFormationTracer(pFormation);
    
    aClone->mSpeed = mSpeed;
    //aClone->mCount = mCount;
    
    
    aClone->mSpecialType = mSpecialType;
    
    aClone->mSpecialX = mSpecialX;
    aClone->mSpecialY = mSpecialY;
    
    aClone->mSpecialRadius = mSpecialRadius;
    aClone->mSpecialCornerRadius = mSpecialCornerRadius;
    
    aClone->mSpecialWidth = mSpecialWidth;
    aClone->mSpecialHeight = mSpecialHeight;
    
    aClone->mSpawnPathOffset = mSpawnPathOffset;
    
    EnumList(LevelFormationTracerNode, aNode, mTracerNodeList) {
        aClone->mTracerNodeList.Add(aNode->Clone(aClone));
    }
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aClone->mSpawnNodeList.Add(aNode->Clone(pFormation));
    }
    
    return aClone;
}

void LevelFormationTracer::HandOffAllGameObjects(FList *pList) {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL && aNode->mObject->mKill == 0 && pList != NULL) {
            pList->Add(aNode->mObject);
        }
        aNode->mObject = NULL;
    }
}

bool LevelFormationTracer::HasPauseExitType() {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (GameObject::IsPauseExitType(aNode->mObjectType) == true) {
            return true;
        }
    }
    return false;
}

void LevelFormationTracer::GetExtremeties(float &pTop, float &pRight, float &pBottom, float &pLeft) {
    if (mSpecialType == TRACER_SPECIAL_TYPE_NONE) {
        EnumList(LevelFormationTracerNode, aNode, mTracerNodeList) {
            if (aNode->mX < pLeft) { pLeft = aNode->mX; }
            if (aNode->mX > pRight) { pRight = aNode->mX; }
            if (aNode->mY < pTop) { pTop = aNode->mY; }
            if (aNode->mY > pBottom) { pBottom = aNode->mY; }
        }
    } else if (mSpecialType == TRACER_SPECIAL_TYPE_CIRCLE) {
        if (pTop > (-mSpecialRadius)) { pTop = -mSpecialRadius; }
        if (pRight < mSpecialRadius) { pRight = mSpecialRadius; }
        if (pBottom < mSpecialRadius) { pBottom = mSpecialRadius; }
        if (pLeft > (-mSpecialRadius)) { pLeft = -mSpecialRadius; }
    } else if (mSpecialType == TRACER_SPECIAL_TYPE_ROUNDED_RECT) {
        float aRadiusH = ((float)mSpecialWidth) / 2.0f;
        float aRadiusV = ((float)mSpecialHeight) / 2.0f;
        if (pTop > (-aRadiusV)) { pTop = -aRadiusV; }
        if (pRight < aRadiusH) { pRight = aRadiusH; }
        if (pBottom < aRadiusV) { pBottom = aRadiusV; }
        if (pLeft > (-aRadiusH)) { pLeft = -aRadiusH; }
    }
}

void LevelFormationTracer::EditorKillAllObjects() {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL) {
            
            GameObject *aObject = aNode->mObject;
            
            aObject->mPermSpawn = NULL;
            aObject->mWaveSpawn = NULL;
            aObject->mWave = NULL;
            aObject->mSection = NULL;
            
            aNode->mObject = NULL;
            
            aObject->Kill();
            //if (gGame != NULL) {
            //    gGame->DisposeObject(aObject);
            //}
        }
    }
}


void LevelFormationTracer::EditorDisposeAll() {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->EditorDisposeAll();
    }
}
