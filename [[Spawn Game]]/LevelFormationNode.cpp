//
//  LevelFormationNodeFormationNode.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/31/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//



#include "LevelFormationNode.hpp"
#include "LevelFormationTracer.hpp"
#include "LevelFormation.hpp"
#include "LevelPath.hpp"
#include "LevelWave.hpp"
#include "Game.hpp"

#include "Balloon.hpp"
#include "BrickHead.hpp"
#include "Turtle.hpp"
#include "Bomb.hpp"
#include "PrizeBox.hpp"
#include "Star.hpp"
#include "FreeLife.hpp"
#include "Cannon.hpp"
#include "PlatformSmall.hpp"
#include "PlatformMedium.hpp"
#include "PlatformLarge.hpp"
#include "PlatformExtraLarge.hpp"
#include "Blimp.hpp"

LevelFormationNode::LevelFormationNode() {
    
    mFormation = NULL;
    
    mObjectID = -1;
    
    mBaseX = 0.0f;
    mBaseY = 0.0f;
    
    mObjectX = 0.0f;
    mObjectY = 0.0f;
    mObjectRotation = 0.0f;
    
    mObjectRotate = false;
    mObjectRotationOffset = 0.0f;
    
    
    mObject = NULL;
    mTracer = NULL;
    mSection = NULL;
    
    mDidOriginateOnWave = false;
    mDidOriginateAsPermanent = false;
    
    mObjectType = GAME_OBJECT_TYPE_BALLOON;
    
    mPathIndexOffset = 0;
    
    mKillTimer = 8;
}

LevelFormationNode::~LevelFormationNode() {
    if (mObject != NULL) {
        //mObject->mFormationNode = NULL;
        mObject = NULL;
    }
}

void LevelFormationNode::Spawn() {
    
    if (mObjectType == GAME_OBJECT_TYPE_BALLOON) {
        if (gGame->ShouldSpawnFreeLife()) {
            mObject = new FreeLife();
            gGame->mFreeLifeList.Add(mObject);
            gGame->NotifyDidSpawnFreeLife();
        } else {
            mObject = new Balloon();
            gGame->mBalloonList.Add(mObject);
            
        }
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_BRICKHEAD) {
        mObject = new BrickHead();
        gGame->mBrickHeadList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_TURTLE) {
        mObject = new Turtle();
        gGame->mTurtleList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_BOMB) {
        mObject = new Bomb();
        gGame->mBombList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL) {
        mObject = new PlatformSmall();
        gGame->mPlatformList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM) {
        mObject = new PlatformMedium();
        gGame->mPlatformList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE) {
        mObject = new PlatformLarge();
        gGame->mPlatformList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE) {
        mObject = new PlatformExtraLarge();
        gGame->mPlatformList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_BLIMP) {
        mObject = new Blimp();
        gGame->mBlimpList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_CANNON) {
        mObject = new Cannon();
        gGame->mCannonList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_STAR) {
        mObject = new Star();
        gGame->mStarList.Add(mObject);
    }
    
    if (mObjectType == GAME_OBJECT_TYPE_PRIZE_BOX) {
        mObject = new PrizeBox();
        gGame->mPrizeBoxList.Add(mObject);
    }
    
    if (mObject != NULL) {
        
        mObject->mSection = mSection;
        mObject->mFormationNode = this;
        
        if ((mObjectID == -1) && (mSection != NULL)) {
            mObjectID = mSection->GenerateObjectIndex();
        }
        
        if (mSection != NULL) {
            mObject->mSectionIndex = mSection->mIndex;
            mObject->mObjectIndex = mObjectID;
        } else {
            
        }
        
        gGame->RealizeObject(mObject);
        
        mObject->mDidOriginateOnWave = mDidOriginateOnWave;
        mObject->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        
        mObject->mTransform.mX = mBaseX;
        mObject->mTransform.mY = mBaseY;
        
        PositionObject();
    }
}

void LevelFormationNode::EditorPickerSpawn() {
    
}

void LevelFormationNode::Reset() {
    
    
    if (mObject != NULL) {
        GameObject *aObject = mObject;
        //aObject->mPermSpawn = NULL;
        //aObject->mWaveSpawn = NULL;
        
        
        mObject = NULL;
        if (gGame != NULL) {
            gGame->DisposeObject(aObject);
        }
    }
    
    mBaseX = 0.0f;
    mBaseY = 0.0f;
}

void LevelFormationNode::Update() {
    PositionObject();
}

void LevelFormationNode::Draw() {
    Graphics::PipelineStateSetShape2DAlphaBlending();
    Graphics::SetColor(0.25f, 0.65f, 0.85f, 0.85f);
    Graphics::DrawPoint(mX, mY, 4.0f);
    Graphics::SetColor();
}

void LevelFormationNode::DrawDeadObjectMarkers() {
    if ((mObject == NULL) && (gGame != NULL)) {
        gGame->DrawDeadObjectMarker(mObjectX, mObjectY);
    }
}

void LevelFormationNode::RefreshSection(LevelSection *pSection) {
    mSection = pSection;
}

void LevelFormationNode::DisposeObject(GameObject *pObject) {
    if (mObject != NULL && mObject == pObject) {
        //mObject->mPermSpawn = NULL;
        //mObject->mWaveSpawn = NULL;
        //mObject->mWave = NULL;
        mObject = NULL;
    }
}

void LevelFormationNode::DisposeAllObjects() {
    if (mObject != NULL) {
        //mObject->mPermSpawn = NULL;
        //mObject->mWaveSpawn = NULL;
        //mObject->mWave = NULL;
        mObject = NULL;
    }
}

bool LevelFormationNode::ContainsObject(GameObject *pObject) {
    if (mObject != NULL && mObject == pObject) {
        return true;
    }
    return false;
}

void LevelFormationNode::PositionObject() {
    
    
    //mTracer
    
    if (gGame != NULL) {
        
        if (mTracer != NULL && mTracer->mPath.mCount > 2) {
            
            
            int aPathIndex = mTracer->mPathIndex + mPathIndexOffset;
            
            int aCeil = (mTracer->mPath.mCount - 1);
            aPathIndex = aPathIndex % aCeil;
            
            if (aPathIndex >= 0 && aPathIndex < mTracer->mPath.mCount) {
                
                mX = mTracer->mPath.mX[aPathIndex];
                mY = mTracer->mPath.mY[aPathIndex];
                if (mFormation->mRotation != 0.0f) {
                    FVec2 aPoint;
                    aPoint.mX = mX; aPoint.mY = mY;
                    aPoint = PivotPoint(aPoint, mFormation->mRotation);
                    mX = aPoint.mX; mY = aPoint.mY;
                }
                
                mX += mFormation->mX;
                mY += mFormation->mY;
                
            } else {
                Log("Miss?\n");
            }
        } else if (mFormation != NULL) {
            
            float aLeft = gGame->mGameAreaLeft;
            float aRight = gGame->mGameAreaRight;
            float aTop = gGame->mGameAreaTop;
            float aBottom = gGame->mGameAreaBottom;
            
            float aWidth = aRight - aLeft;
            float aHeight = aBottom - aTop;
            
            mX = (mBaseX / 100.0f) * aWidth;
            mY = (mBaseY / 100.0f) * aHeight;
            
            if (mFormation->mRotation != 0.0f) {
                FVec2 aPoint;
                aPoint.mX = mX; aPoint.mY = mY;
                aPoint = PivotPoint(aPoint, mFormation->mRotation);
                mX = aPoint.mX; mY = aPoint.mY;
            }
            
            mX += mFormation->mX;
            mY += mFormation->mY;
        }
    }
    
    if (mObject != NULL) {
        mObject->mTransform.mX = mX;
        mObject->mTransform.mY = mY;
        
        if (mObjectRotate == true) {
            
            if (mFormation != NULL) {
                mObject->mTransform.mRotation = mFormation->mRotation + mObjectRotationOffset;
            }
        }
    }
}

LevelFormationNode *LevelFormationNode::Clone(LevelFormation *pFormation) {
    
    LevelFormationNode *aClone = new LevelFormationNode();
    
    aClone->mBaseX = mBaseX;
    aClone->mBaseY = mBaseY;
    aClone->mObjectType = mObjectType;
    
    aClone->mObjectRotate = mObjectRotate;
    aClone->mObjectRotationOffset = mObjectRotationOffset;
    
    return aClone;
}

void LevelFormationNode::EditorDisposeAll() {
    
    if (mObject != NULL) {
        GameObject *aObject = mObject;
        //aObject->mPermSpawn = NULL;
        //aObject->mWaveSpawn = NULL;
        
        
        mObject = NULL;
        if (gGame != NULL) {
            gGame->DisposeObject(aObject);
        }
    }
}
