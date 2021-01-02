//
//  LevelWaveSpawnSpawn.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/23/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelWaveSpawn.hpp"
#include "LevelPath.hpp"
#include "LevelWave.hpp"
#include "Game.hpp"
#include "Balloon.hpp"
#include "FormationCollection.hpp"

LevelWaveSpawn::LevelWaveSpawn(LevelWave *pWave, LevelPath *pPath) {
    
    mEditorPreviewMode = false;
    
    mWave = pWave;
    mPath = pPath;
    mPathIndex = 0;
    mObject = NULL;
    mFormation = NULL;
    mIsComplete = false;
    
    mClockTicks = 0;
    
    mPauseExit = false;
    mPauseExitCompletionDelayTimer = 0;
    
    mDidSpawn = false;
    mDidUpdateAfterSpawn = false;
    
    mShortCircuit = false;
    
    mDidShortCircuit = false;
    mDidKnockDown = false;
    
    mObjectX = 0.0f;
    mObjectY = 0.0f;
    mObjectRotation = 0.0f;
    
    mIsPlayingEnter = false;
    mIsPlayingExit = false;
    
    mOffsetSpawnDistance = 0.0f;
    mBaseX = 0.0f;
    mBaseY = 0.0f;
    mDistanceTraveled = 0.0f;
    
    mObjectID = -1;
    
    mWaitTimer = 0;
    
    mSection = NULL;
    
    mObjectType = GAME_OBJECT_TYPE_BALLOON;
    mKillTimer = 8;
    
    mCurrentWaitTime = 0;
    mCurrentWaitTick = 0;
    
}

LevelWaveSpawn::~LevelWaveSpawn() {
    
    //Log("Dealloc[LevelWaveSpawn:%X] Obj[%d]\n", this, mObject);
    
    if (mObject != NULL) {
        mObject->mWaveSpawn = NULL;
        mObject = NULL;
    }
    
    if (mFormation != NULL) {
        delete mFormation;
        mFormation = NULL;
    }
}

void LevelWaveSpawn::Spawn() {
    
    if (mObject != NULL) {
        GameObject *aObject = mObject;
        mObject = NULL;
        
        if (gGame != NULL) {
            gGame->DisposeObject(aObject);
        }
    }
    
    if (mFormation != NULL) {
        delete mFormation;
        mFormation = NULL;
    }
    
    mBaseX = 0.0f;
    mBaseY = 0.0f;
    mDistanceTraveled = 0.0f;
    mIsComplete = false;
    if (mPath->mPath.mCount > 0) {
        mBaseX = mPath->mPath.mX[0];
        mBaseY = mPath->mPath.mY[0];
    }
    
    if (mFormationID.mLength > 0) {
        mFormation = gFormationCollection.GetFormation(mFormationID.c());
        if (mFormation != NULL) {
            
            mFormation->mSection = mSection;
            mFormation->mDidOriginateAsPermanent = false;
            mFormation->mDidOriginateOnWave = true;
            
            mFormation->mX = mBaseX;
            mFormation->mY = mBaseY;
            mFormation->Spawn(&mMotionController, &mFormationConfiguration);
        }
    }
    
    if (mFormation == NULL) {
        
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
        
        if (mObjectType == GAME_OBJECT_TYPE_BOMB) {
            mObject = new Bomb();
            gGame->mBombList.Add(mObject);
        }
        
        if (mObjectType == GAME_OBJECT_TYPE_TURTLE) {
            mObject = new Turtle();
            gGame->mTurtleList.Add(mObject);
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
            
            if ((mObjectID == -1) && (mSection != NULL)) {
                mObjectID = mSection->GenerateObjectIndex();
            }
            
            if (mSection != NULL) {
                mObject->mSectionIndex = mSection->mIndex;
                mObject->mObjectIndex = mObjectID;
            } else {
                
#ifndef EDITOR_MODE
                Log("??\n");
#endif
                
            }
            
            gGame->RealizeObject(mObject);
            
            mObject->mDidOriginateAsPermanent = false;
            mObject->mDidOriginateOnWave = true;
            
            mObject->mTransform.mX = mBaseX;
            mObject->mTransform.mY = mBaseY;
        }
    }
    
    mDidSpawn = true;
    mDidUpdateAfterSpawn = false;
    
    if (mObject != NULL) {
        mObject->mSection = mSection;
        mObject->mWaveSpawn = this;
        mStyleController.ApplyToObject(mObject);
    }
    if (mFormation != NULL) {
        mFormation->SetWaveSpawn(this);
        mFormation->ApplyStyleController(&mStyleController);
    }
}

void LevelWaveSpawn::DisposeObject(GameObject *pObject) {
    if (mObject != NULL && mObject == pObject) {
        mObject->mWaveSpawn = NULL;
        mObject->mWave = NULL;
        
        mObject = NULL;
    }
    if (mFormation != NULL) {
        mFormation->DisposeObject(pObject);
    }
}

void LevelWaveSpawn::DisposeAllObjects() {
    if (mObject != NULL) {
        mObject->mWaveSpawn = NULL;
        mObject->mWave = NULL;
        mObject = NULL;
    }
    if (mFormation != NULL) {
        mFormation->DisposeAllObjects();
    }
}

bool LevelWaveSpawn::ContainsObject(GameObject *pObject) {
    if (mObject != NULL && mObject == pObject) {
        return true;
    }
    if (mFormation != NULL) {
        if (mFormation->ContainsObject(pObject) == true) {
            return true;
        }
    }
    return false;
}

void LevelWaveSpawn::RefreshSection(LevelSection *pSection) {
    mSection = pSection;
    if (mFormation != NULL) {
        mFormation->RefreshSection(pSection);
    }
}

bool LevelWaveSpawn::DidStart() {
    if (mDidSpawn == true) {
        if (mDidUpdateAfterSpawn == true) {
            return true;
        }
    }
    return false;
}

bool LevelWaveSpawn::IsClear() {
    
    if (mDidSpawn == false) {
        return false;
    }
    
    if (mEditorPreviewMode == true) {
        return false;
    }
    
    
    if (mObject != NULL) {
        if (mObject->mKill == 0) {
            return false;
        }
    }
    
    if (mFormation != NULL) {
        if (mFormation->IsClear() == false) {
            return false;
        }
    }
    
    return true;
}

bool LevelWaveSpawn::IsClearForSectionCompletion() {
    if (mDidSpawn == false) { return false; }
    
    if (mEditorPreviewMode == true) { return false; }
    
    if (mObject != NULL) {
        if (mObject->IsRequiredToClearForSectionCompletion() == true) {
            return false;
        }
    }
    
    if (mFormation != NULL) {
        if (mFormation->IsClearForSectionCompletion() == false) {
            return false;
        }
    }
    
    return true;
}

void LevelWaveSpawn::Reset() {
    
    if (mObject != NULL) {
        GameObject *aObject = mObject;
        mObject = NULL;
        
        if (gGame != NULL) {
            gGame->DisposeObject(aObject);
        }
    }
    
    if (mFormation != NULL) {
        delete mFormation;
        mFormation = NULL;
    }
    
    mClockTicks = 0;
    
    mPathIndex = 0;
    mBaseX = 0.0f;
    mBaseY = 0.0f;
    mDistanceTraveled = 0.0f;
    
    mDidSpawn = false;
    mDidUpdateAfterSpawn = false;
    
    mDidShortCircuit = false;
    mDidKnockDown = false;
}

static FList cWaveShortCircuitHandoffList;
void LevelWaveSpawn::Update() {
    
    if (mDidSpawn == true) {
        mDidUpdateAfterSpawn = true;
    }
    
    //aSpawn->mCurrentWaitTime = aSpawn->mWaitTimer;
    //aSpawn->mCurrentWaitTick = 0;
    
    if (mCurrentWaitTime > 0) {
        mCurrentWaitTick++;
        if (mCurrentWaitTick > mCurrentWaitTime) {
            mCurrentWaitTime = 0;
            mCurrentWaitTick = 0;
        }
    }
    
    if (mPathIndex >= 0 && mPathIndex < mPath->mPath.mCount) {
        mBaseX = mPath->mPath.mX[mPathIndex];
        mBaseY = mPath->mPath.mY[mPathIndex];
        mDistanceTraveled = mPath->mDist.mData[mPathIndex];
        mIsPlayingEnter = mPath->mPlayEnter.mData[mPathIndex];
        mIsPlayingExit = mPath->mPlayExit.mData[mPathIndex];
    } else {
        if (mPath->mPath.mCount > 0 && mPathIndex > 0) {
            mBaseX = mPath->mPath.mX[mPath->mPath.mCount - 1];
            mBaseY = mPath->mPath.mY[mPath->mPath.mCount - 1];
            mDistanceTraveled = mPath->mDist.mData[mPath->mDist.mCount - 1];
            mIsPlayingEnter = mPath->mPlayEnter.mData[mPath->mPlayEnter.mCount - 1];
            mIsPlayingExit = mPath->mPlayExit.mData[mPath->mPlayExit.mCount - 1];
        }
    }
    
    mMotionController.mSkipUpdate = false;
    if (mIsPlayingEnter && (mMotionController.mPlayOnEnter == false)) { mMotionController.mSkipUpdate = true; }
    if (mIsPlayingExit && (mMotionController.mPlayOnExit == false)) { mMotionController.mSkipUpdate = true; }
    mMotionController.Update();
    
    
    mObjectX = mBaseX;
    mObjectY = mBaseY;
    mObjectRotation = mMotionController.ApplyRotation();
    if (mObjectType == GAME_OBJECT_TYPE_CANNON) {
        mObjectRotation = mMotionController.ApplyRotationCannon();
    }
    if (mObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL ||
        mObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM ||
        mObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE ||
        mObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE) {
        mObjectRotation = mMotionController.ApplyRotationPlatform();
    }
    
    mMotionController.Apply(mBaseX, mBaseY, &mObjectX, &mObjectY);
    
    if (mObject != NULL) {
        mObject->mTransform.mX = mObjectX;
        mObject->mTransform.mY = mObjectY;
        mObject->mTransform.mRotation = mObjectRotation;
        
        if (mObject->mKill != 0) {
            mObject = NULL;
        }
    }
    
    if (mFormation != NULL) {
        
        mFormation->mIsPlayingEnter = mIsPlayingEnter;
        mFormation->mIsPlayingExit = mIsPlayingExit;
        
        mFormation->mX = mBaseX;
        mFormation->mY = mBaseY;
        mFormation->Update(&mMotionController);
    }
    
    if ((mShortCircuit == true) || (mShortCircuitExternal == true)) {
        
        bool aTripShortCircuit = false;
        
        if (mShortCircuit == true) {
            aTripShortCircuit = IsClearForSectionCompletion();
        }
        
        if (mShortCircuitExternal == true) {
            if (gGame != NULL) {
                aTripShortCircuit = gGame->IsClearForShortCircuitExternal();
            }
        }
        
        if (aTripShortCircuit == true) {
        
            if ((mDidKnockDown == false) && (mPauseExit == false) && (gGame != NULL)) {
                
                mDidKnockDown = true;
                mDidShortCircuit = true;
                
                cWaveShortCircuitHandoffList.RemoveAll();
                HandOffAllGameObjects(&cWaveShortCircuitHandoffList);
                
                EnumList(GameObject, aObject, cWaveShortCircuitHandoffList) {
                    gGame->KnockDown(aObject);
                }
            }
        }
    }
}

void LevelWaveSpawn::Draw() {
    
    if (mPathIndex >= 0 && mPathIndex < mPath->mPath.mCount) {
        
        float aX = mPath->mPath.mX[mPathIndex];
        float aY = mPath->mPath.mY[mPathIndex];
        
        Graphics::SetColor(1.0f, 0.25f, 0.25f, 0.65f);
        Graphics::DrawPoint(aX, aY, 5.0f);
    }
}

void LevelWaveSpawn::DrawDeadObjectMarkers() {
    
    if (mDidSpawn == true) {
        if (mFormation != NULL) {
            mFormation->DrawDeadObjectMarkers();
        } else {
            if ((gGame != NULL) && (mObject == NULL)) {
                gGame->DrawDeadObjectMarker(mObjectX, mObjectY);
            }
        }
    }
}

bool LevelWaveSpawn::ShouldPauseExit() {
    
    if (mMotionController.ShouldPauseExit() == true) {
        if (mFormation != NULL) {
            if (mFormation->HasPauseExitType() == true) {
                return true;
            }
        }
        if (mObject != NULL) {
            if (GameObject::IsPauseExitType(mObject->mGameObjectType) == true) {
                return true;
            }
        }
    }
    return false;
}

void LevelWaveSpawn::BeginPauseExit() {
    mMotionController.BeginPauseExit();
}

bool LevelWaveSpawn::IsPauseExitComplete() {
    if (HasAnyObjects() == true) {
        if (mMotionController.IsPauseExitComplete() == false) {
            return false;
        }
    }
    return true;
}

bool LevelWaveSpawn::HasAnyObjects() {
    
    if (mObject != NULL) {
        return true;
    }
    
    if (mFormation != NULL) {
        if (mFormation->HasAnyObjects() == true) {
            return true;
        }
    }
    
    return false;
}

void LevelWaveSpawn::HandOffAllGameObjects(FList *pList) {
    
    if (mObject != NULL && pList != NULL) {
        pList->Add(mObject);
    }
    mObject = NULL;
    
    if (mFormation != NULL) {
        mFormation->HandOffAllGameObjects(pList);
    }
}


void LevelWaveSpawn::EditorDisposeAll() {
    
    if (mFormation != NULL) {
        mFormation->EditorDisposeAll();
    }
    
    if (mObject != NULL) {
        GameObject *aObject = mObject;
        mObject = NULL;
        
        if (gGame != NULL) {
            gGame->DisposeObject(aObject);
        }
    }
}
