//
//  LevelSectionPerm.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/5/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelSectionPerm.hpp"
#include "LevelSection.hpp"
#include "Game.hpp"
#include "Balloon.hpp"
#include "FormationCollection.hpp"

LevelSectionPerm::LevelSectionPerm(LevelSection *pSection) {
    mSection = pSection;
    
    mDidSpawn = false;
    
    mObject = NULL;
    mFormation = NULL;
    
    mObjectType = GAME_OBJECT_TYPE_BALLOON;
    
    mObjectX = 0.0f;
    mObjectY = 0.0f;
    mObjectRotation = 0.0f;
    
    mObjectID = -1;
    
    mSpawnEqualSpacing = true;
    mSpawnSpacing = 120;
    mSpawnOffset = 0;
    
    mIsPlayingEnter = false;
    mIsPlayingExit = false;
    
    mDisableDeadObjectMarkers = false;
    
    mPauseExit = false;
    
    mShortCircuit = false;
    mShortCircuitExternal = false;
    
    mDidShortCircuit = false;
    mDidKnockDown = false;
    
    mBaseX = 0.0f;
    mBaseY = 0.0f;
    
    mX = 0.0f;
    mY = 0.0f;
    
    mKillTimer = 8;
}

LevelSectionPerm::~LevelSectionPerm() {
    
    DisposeAllObjects();
    
    if (mFormation != NULL) {
        delete mFormation;
        mFormation = NULL;
    }
    
    FreeList(LevelPermSpawn, mSpawnList);
    FreeList(LevelPermSpawn, mSpawnKillList);
}

void LevelSectionPerm::Reset() {
    
    
    //mFormationID.Clear();
    //mObjectType = GAME_OBJECT_TYPE_BALLOON;
    
    mDidSpawn = false;
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->Reset();
        mSpawnKillList.Add(aSpawn);
    }
    mSpawnList.RemoveAll();
    
    mMotionController.Reset();
    mFormationConfiguration.Reset();
    
    mPath.Reset();
    
    if (mObject != NULL) {
        GameObject *aObject = mObject;
        mObject = NULL;
        
        if (gGame != NULL) {
            gGame->DisposeObject(aObject);
        }
    }
}

void LevelSectionPerm::Prepare() {
    
    mPath.Finalize();
}

void LevelSectionPerm::Spawn() {
    
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
    
    for (int i=0;i<mSpawnList.mCount;i++) {
        LevelPermSpawn *aSpawn = ((LevelPermSpawn *)mSpawnList.mData[i]);
        aSpawn->mSection = mSection;
    }
    
    //Either we are using the path or no...
    
    if (mPath.mNodeList.mCount > 0 &&
        mPath.mDidFailFinalize == false &&
        mPath.mDidFinalize == true &&
        mSpawnList.mCount > 0 &&
        mPath.mPath.mCount > 1) { //Are we super sure that we can do this?
        
        //One case, we are evenly spawned..
        
        if (mSpawnEqualSpacing) {
            for (int i=0;i<mSpawnList.mCount;i++) {
                LevelPermSpawn *aSpawn = ((LevelPermSpawn *)mSpawnList.mData[i]);
                float aPercent = ((float)i) / ((float)mSpawnList.mCount);
                float aPathPos = aPercent * ((float)mPath.mPath.mCount);
                aSpawn->mPathIndex = (int)(round(aPathPos));
            }
        } else {
            
            int aSpawnLoc = 0;
            float aSpawnDist = 0.0f;
            for (int i=0;i<mSpawnList.mCount;i++) {
                LevelPermSpawn *aSpawn = ((LevelPermSpawn *)mSpawnList.mData[i]);
                aSpawnDist += (mSpawnSpacing + aSpawn->mSpacingOffset);
                while (aSpawnLoc < mPath.mDist.mCount) {
                    if (mPath.mDist.mData[aSpawnLoc] >= aSpawnDist) {
                        aSpawnLoc -= 1;
                        break;
                    } else {
                        aSpawnLoc += 1;
                    }
                }
                
                if (aSpawnLoc >= mPath.mDist.mCount) { aSpawnLoc = mPath.mDist.mCount - 1; }
                if (aSpawnLoc < 0) { aSpawnLoc = 0; }
                
                aSpawn->mPathIndex = aSpawnLoc;
            }
        }
        
        int aPathCount = mPath.mPath.mCount;
        if (aPathCount > 1) {
            for (int i=0;i<mSpawnList.mCount;i++) {
                LevelPermSpawn *aSpawn = ((LevelPermSpawn *)mSpawnList.mData[i]);
                aSpawn->mPathIndex = (aSpawn->mPathIndex + mSpawnOffset) % aPathCount;
                if (aSpawn->mPathIndex < 0) {
                    aSpawn->mPathIndex += aPathCount;
                }
            }
        }
        
        EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
            aSpawn->Spawn();
        }
        
    } else {
        
        //
        // We are either an object or formation, free floating...
        //
        
        if (mFormationID.mLength > 0) {
            mFormation = gFormationCollection.GetFormation(mFormationID.c());
            if (mFormation != NULL) {
                mFormation->mSection = mSection;
                mFormation->mDidOriginateOnWave = false;
                mFormation->mDidOriginateAsPermanent = true;
                mFormation->mX = mBaseX;
                mFormation->mY = mBaseY;
                mFormation->Spawn(&mMotionController, &mFormationConfiguration);
                mFormation->ApplyStyleController(&mStyleController);
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
                
                mObject->mDidOriginateOnWave = false;
                mObject->mDidOriginateAsPermanent = true;
                mStyleController.ApplyToObject(mObject);
            }
        }
    }
    
    mDidSpawn = true;
    
    PositionObject();
    
    if (mFormation != NULL) {
        mFormation->ApplyStyleController(&mStyleController);
    }
    
    if (mObject != NULL) {
        mObject->mSection = mSection;
        mStyleController.ApplyToObject(mObject);
    }
}

static FList cPermShortCircuitHandoffList;
void LevelSectionPerm::Update() {
    
    //////
    //
    EnumList(LevelPermSpawn, aSpawn, mSpawnKillList) {
        aSpawn->mKillTimer--;
        if (aSpawn->mKillTimer <= 0) { mSpawnDeleteList.Add(aSpawn); }
    }
    EnumList(LevelPermSpawn, aSpawn, mSpawnDeleteList) {
        mSpawnKillList.Remove(aSpawn);
        delete aSpawn;
    }
    mSpawnDeleteList.RemoveAll();
    //
    //////
    
    
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->mIsPlayingEnter = mIsPlayingEnter;
        aSpawn->mIsPlayingExit = mIsPlayingExit;
        aSpawn->Update();
    }
    
    
    if (mObject != NULL && mObject->mKill != 0) {
        mObject = NULL;
    }
    
    
    mPath.Update();
    
    //if (mPath.mDidFinalize == true && mPath.mDidFailFinalize == false && mPath.mPath.mCount > 0) {
    //}
    
    mMotionController.mSkipUpdate = false;
    if (mIsPlayingEnter && (mMotionController.mPlayOnEnter == false)) { mMotionController.mSkipUpdate = true; }
    if (mIsPlayingExit && (mMotionController.mPlayOnExit == false)) { mMotionController.mSkipUpdate = true; }
    mMotionController.Update();
    
    PositionObject();
    
    if (mFormation != NULL) {
        mFormation->mIsPlayingEnter = mIsPlayingEnter;
        mFormation->mIsPlayingExit = mIsPlayingExit;
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
        
        EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
            if (aSpawn->IsClearForSectionCompletion() == false) {
                aTripShortCircuit = false;
            }
        }
        
        if (aTripShortCircuit == true) {
            if ((mDidKnockDown == false) && (mPauseExit == false) && (gGame != NULL)) {
                mDidKnockDown = true;
                mDidShortCircuit = true;
                
                cPermShortCircuitHandoffList.RemoveAll();
                HandOffAllGameObjects(&cPermShortCircuitHandoffList);
                
                EnumList(GameObject, aObject, cPermShortCircuitHandoffList) {
                    gGame->KnockDown(aObject);
                }
            }
        }
    }
}

void LevelSectionPerm::Draw() {
    
    mPath.Draw();
    
}

void LevelSectionPerm::DisposeObject(GameObject *pObject) {
    
    if ((mObject == pObject) && (pObject != NULL)) {
        mObject = NULL;
    }
    
    if (mFormation != NULL) {
        mFormation->DisposeObject(pObject);
    }
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->DisposeObject(pObject);
    }
}

void LevelSectionPerm::DisposeAllObjects() {
    
    if (mObject != NULL) {
        mObject = NULL;
    }
    
    if (mFormation != NULL) {
        mFormation->DisposeAllObjects();
    }
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->DisposeAllObjects();
    }
}

bool LevelSectionPerm::ContainsObject(GameObject *pObject) {
    
    if ((mObject == pObject) && (pObject != NULL)) {
        return true;
    }
    
    if (mFormation != NULL) {
        if (mFormation->ContainsObject(pObject)) {
            return true;
        }
    }
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        if (aSpawn->ContainsObject(pObject)) {
            return true;
        }
    }
    
    return false;
}

FVec2 LevelSectionPerm::ConvertLocalPointToGame(FVec2 pPos) {
    
    FVec2 aResult;
    
    float aX = mX;
    float aY = mY;
    aResult.mX = pPos.mX + aX;
    aResult.mY = pPos.mY + aY;
    
    return aResult;
}


void LevelSectionPerm::PositionObject() {
    
    mX = mBaseX;
    mY = mBaseY;
    
    if (mSection != NULL) {
        mX += mSection->mX;
        mY += mSection->mY;
    }
    
    mObjectX = mX;
    mObjectY = mY;
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
    
    mMotionController.Apply(mX, mY, &mObjectX, &mObjectY);
    if (mObject != NULL) {
        mObject->mTransform.mX = mObjectX;
        mObject->mTransform.mY = mObjectY;
        mObject->mTransform.mRotation = mObjectRotation;
    }
    
    if (mFormation != NULL) {
        mFormation->mX = mX;
        mFormation->mY = mY;
    }
    
}

bool LevelSectionPerm::IsClearForSectionCompletion() {
    if (mDidSpawn == false) { return false; }
    
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
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        if (aSpawn->IsClearForSectionCompletion() == false) {
            return false;
        }
    }
    
    return true;
}

bool LevelSectionPerm::HasAnyObjects() {
    
    if (mObject != NULL) {
        return true;
    }
    
    if (mFormation != NULL) {
        if (mFormation->HasAnyObjects() == true) {
            return true;
        }
    }
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        if (aSpawn->HasAnyObjects() == true) {
            return true;
        }
    }
    
    return false;
}

void LevelSectionPerm::HandOffAllGameObjects(FList *pList) {
    
    if (mObject != NULL && pList != NULL) {
        pList->Add(mObject);
    }
    mObject = NULL;
    
    if (mFormation != NULL) {
        mFormation->HandOffAllGameObjects(pList);
    }
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->HandOffAllGameObjects(pList);
    }
}

bool LevelSectionPerm::ShouldPauseExit() {

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

void LevelSectionPerm::RefreshSection(LevelSection *pSection) {
    mSection = pSection;
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->RefreshSection(pSection);
    }
    
    if (mFormation != NULL) {
        mFormation->RefreshSection(pSection);
    }
}

void LevelSectionPerm::BeginPauseExit() {
    mMotionController.BeginPauseExit();
}

bool LevelSectionPerm::IsPauseExitComplete() {
    if (HasAnyObjects() == true) {
        if (mMotionController.IsPauseExitComplete() == false) {
            return false;
        }
    }
    return true;
}

void LevelSectionPerm::DrawDeadObjectMarkers() {
    if ((mDidSpawn == true) && (mDisableDeadObjectMarkers == false)) {
        if (mSpawnList.mCount > 0) {
            EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
                aSpawn->DrawDeadObjectMarkers();
            }
        } else if (mFormation != NULL) {
            mFormation->DrawDeadObjectMarkers();
        } else {
            if ((mObject == NULL) && (gGame != NULL)) {
                gGame->DrawDeadObjectMarker(mObjectX, mObjectY);
            }
        }
    }
}

void LevelSectionPerm::DisableDeadObjectMarkers() {
    mDisableDeadObjectMarkers = true;
}

void LevelSectionPerm::EditorDisposeAll() {
    
    EnumList(LevelPermSpawn, aSpawn, mSpawnList) {
        aSpawn->EditorDisposeAll();
    }
    
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
