//
//  LevelSection.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/14/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelSection.hpp"
#include "Game.hpp"
#include "LevelSectionBlueprint.hpp"

LevelSection::LevelSection() {
    
    mEditorPreviewMode = false;
    
    mCandidateWave = NULL;
    mCandidateWaveDelay = 0;
    mCandidateWaveIndex = 0;
    
    mLoadError = false;
    
    mIsComplete = false;
    
    mDidFirstUpdate = false;
    
    mKnockDownPermsOnClear = false;
    mBlockFreeLives = false;
    
    mAllWavesComplete = false;
    mAllPermsComplete = false;
    
    mDidSpawn = false;
    
    mPauseExit = false;
    mPauseExitCompletionDelayTimer = 0;
    
    mClockCountingDown = false;
    
    
    mKeepAliveTimer = 0;
    mForceKillTimer = 0;
    
    mProgressCount = 0;
    
    mStartWaveIndex = 0;
    
    mIndex = -1;
    
    mX = 0.0f;
    mY = 0.0f;
    
    mSpawnObjectIndex = 0;
    
    mRequiredFlyInType = SECTION_FLY_NONE;
    mRequiredFlyOutType = SECTION_FLY_NONE;
    
    mFlyInType = SECTION_FLY_NONE;
    mFlyInTimer = 0;
    mFlyInTime = FLY_TIME_SIDES;
    mFlyInDelay = 0;
    
    mFlyOutType = SECTION_FLY_NONE;
    mFlyOutTimer = 0;
    mFlyOutTime = FLY_TIME_SIDES;
    
}

LevelSection::~LevelSection() {
    mActiveWaveList.RemoveAll();
    
    FreeList(LevelWave, mWaveList);
    FreeList(LevelWave, mKillWaveList);
    
    FreeList(LevelSectionPerm, mPermList);
    FreeList(LevelSectionPerm, mKillPermList);
}

void LevelSection::Reset() {
    
    mSpawnObjectIndex = 0;
    
    mActiveWaveList.RemoveAll();
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->Reset();
        mKillWaveList.Add(aWave);
    }
    mWaveList.RemoveAll();
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->Reset();
        mKillPermList.Add(aPerm);
    }
    mPermList.RemoveAll();
    
    mCandidateWaveIndex = mStartWaveIndex;
    mCandidateWave = NULL;
    mIsComplete = false;
    mAllWavesComplete = false;
    mAllPermsComplete = false;
    mDidSpawn = false;
    mClockCountingDown = false;
    mCandidateWaveDelay = 0;
    mProgressCount = 0;
    
}

void LevelSection::Spawn() {
    
    EnumList(LevelWave, aWave, mWaveList) {
        
        if (aWave->mPath.mDidFinalize == false) {
            aWave->mPath.Finalize();
        }
        
        aWave->RefreshSection(this);
        aWave->RefreshClockTime();
    }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->RefreshSection(this);
    }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->Prepare();
    }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->Spawn();
    }
    
    mDidSpawn = true;
    
    Update();
}

void LevelSection::Update() {
    
    bool aBlockWaves = false;
    
    //IsAnyObjectBeingKnockedDown()
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->mIsPlayingEnter = false;
        aPerm->mIsPlayingExit = false;
    }
    
    if (mFlyInType != SECTION_FLY_NONE) {
        if (mFlyInTimer >= mFlyInTime) {
            
        } else {
            aBlockWaves = true;
        }
    }
    
    UpdateFlyIn();
    UpdateFlyOut();
    
    if (mFlyInType != SECTION_FLY_NONE) {
        EnumList(LevelSectionPerm, aPerm, mPermList) {
            aPerm->mIsPlayingEnter = true;
        }
    }
    
    if (mFlyOutType != SECTION_FLY_NONE) {
        EnumList(LevelSectionPerm, aPerm, mPermList) {
            aPerm->mIsPlayingExit = true;
        }
    }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->Update();
    }
    
    if (mFlyInDelay > 0) {
        --mFlyInDelay;
        return;
    }
    
    mAllWavesComplete = true;
    mAllPermsComplete = true;
    
    if (mDidSpawn == false) {
        mAllWavesComplete = false;
        mAllPermsComplete = false;
        mIsComplete = false;
    }
    
    if (mCandidateWaveIndex < mWaveList.mCount) {
        mAllWavesComplete = false;
    }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        
        if (mEditorPreviewMode == true) {
            mAllPermsComplete = false;
        } else {
            if (aPerm->IsClearForSectionCompletion() == false) {
                mAllPermsComplete = false;
            }
        }
    }
    
    if (mCandidateWave == NULL && aBlockWaves == false) {
        mCandidateWave = (LevelWave *)mWaveList.Fetch(mCandidateWaveIndex);
        if (mCandidateWave != NULL) {
            if (mCandidateWaveIndex > mStartWaveIndex) {
                mCandidateWaveDelay = mCandidateWave->mCreationDelay;
            } else {
                mCandidateWaveDelay = 0;
            }
            ++mCandidateWaveIndex;
        }
    }
    
    if (mCandidateWave != NULL) {
        
        bool aCanSpawnWave = true;
        
        if (mCandidateWave->mCreationRequiresPrevWaveStart == true) {
            //Always yes...
        }
        
        bool aAllComplete = true;
        EnumList(LevelWave, aWave, mActiveWaveList) {
            if (aWave->mIsComplete == false) {
                aAllComplete = false;
            }
        }
        
        if (mCandidateWave->mCreationRequiresPrevWaveComplete == true) {
            if (aAllComplete == false) {
                aCanSpawnWave = false;
            }
        }
        
        if (mCandidateWave->mCreationRequiresScreenWavesClear == true) {
            if (gGame != NULL) {
                
                if (mEditorPreviewMode == true) {
                    
                    if (mActiveWaveList.mCount > 0) {
                        aCanSpawnWave = false;
                    }
                    
                } else {
                    if (gGame->IsScreenClearForSpawn(false) == false) {
                        aCanSpawnWave = false;
                    }
                }
            }
        }
        
        if (mDidFirstUpdate == false) {
            aCanSpawnWave = false;
        }
        
        if ((mFlyInType != SECTION_FLY_NONE) || (mFlyOutType != SECTION_FLY_NONE)) {
            aCanSpawnWave = false;
        }
        
        if (aCanSpawnWave) {
            if (mCandidateWaveDelay > 0) {
                mCandidateWaveDelay--;
            } else {
                
                mActiveWaveList.Add(mCandidateWave);
                mCandidateWave->mEditorPreviewMode = mEditorPreviewMode;
                mCandidateWave->Prepare();
                mCandidateWave = NULL;
                
                bool aCheckAhead = true;
                while (aCheckAhead == true) {
                    aCheckAhead = false;
                    
                    LevelWave *aCheckWave = (LevelWave *)mWaveList.Fetch(mCandidateWaveIndex);
                    if (aCheckWave != NULL && aCheckWave->mCreationDelay == 0) {
                        
                        //All the conditions to insta-spawn these preceeding waves...
                        
                        //mCreationRequiresPrevWaveStart; (Don't Care)
                        //mCreationRequiresPrevWaveComplete; (Can't Do It)
                        //mCreationRequiresScreenWavesClear; (Can't Do It)
                        //mCreationRequiresScreenPermsClear; (Can't Do It)
                        if (   aCheckWave->mCreationRequiresPrevWaveComplete == false
                            && aCheckWave->mCreationRequiresScreenWavesClear == false) {
                            
                            mActiveWaveList.Add(aCheckWave);
                            aCheckWave->mEditorPreviewMode = mEditorPreviewMode;
                            aCheckWave->Prepare();
                            aCheckAhead = true;
                            ++mCandidateWaveIndex;
                        }
                    }
                }
            }
        }
    }
    
    EnumList(LevelWave, aWave, mActiveWaveList) {
        if (aWave->mIsComplete == false) {
            mAllWavesComplete = false;
        }
    }
    
    if (mCandidateWave != NULL) {
        mAllWavesComplete = false;
    }
    
    if ((mAllWavesComplete == true) && (mAllPermsComplete == true)) {
        
        //Essentially, what we want to do here is CHECK if the SECTION ITSELF should have a pause-exit delay...
        if ((mPauseExit == false) && (mIsComplete == false)) {
            mPauseExit = ShouldPauseExit();
            if (mPauseExit == true) {
                BeginPauseExit();
            }
        }
        
        if (mPauseExit == false) {
            mIsComplete = true;
        }
    }
    
    if ((mPauseExit == true) && (mIsComplete == false)) {
        if ((mPauseExit == true) && (mPauseExitCompletionDelayTimer == 0)) {
            if (IsPauseExitComplete()) {
                mPauseExitCompletionDelayTimer = 24;
            }
        }
        
        if (mPauseExitCompletionDelayTimer > 0) {
            mPauseExitCompletionDelayTimer--;
            if (mPauseExitCompletionDelayTimer <= 0) {
                mPauseExitCompletionDelayTimer = 0;
                mIsComplete = true;
            }
        }
    }
    
    EnumList(LevelWave, aWave, mActiveWaveList) {
        aWave->Update();
        if (aWave->mIsComplete) {
            mRemoveActiveWaveList.Add(aWave);
        }
    }
    EnumList(LevelWave, aWave, mRemoveActiveWaveList) {
        mActiveWaveList.Remove(aWave);
    }
    mRemoveActiveWaveList.RemoveAll();
    
    EnumList(LevelWave, aWave, mKillWaveList) {
        if (mCandidateWave == aWave) { mCandidateWave = NULL; }
        aWave->mKillTimer--;
        if (aWave->mKillTimer <= 0) { mDeleteWaveList.Add(aWave); }
    }
    EnumList(LevelWave, aWave, mDeleteWaveList) {
        mKillWaveList.Remove(aWave);
        delete aWave;
    }
    mDeleteWaveList.RemoveAll();
    
    mDidFirstUpdate = true;
}

void LevelSection::Draw() {
    
}

void LevelSection::DisposeObject(GameObject *pObject) {
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->DisposeObject(pObject);
    }
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->DisposeObject(pObject);
    }
}

void LevelSection::AddWave(LevelWave *pLevelWave) {
    mWaveList.Add(pLevelWave);
}

void LevelSection::AddPerm(LevelSectionPerm *pPerm) {
    mPermList.Add(pPerm);
}

void LevelSection::FlyIn(int pType, int pDelay) {
    mFlyInType = pType;
    mFlyInTimer = 0;
    mFlyInDelay = pDelay;
    
    if (pType == SECTION_FLY_TOP) {
        mFlyInTime = FLY_TIME_TOP;
    } else {
        mFlyInTime = FLY_TIME_SIDES;
    }
    
    Update();Update();
}

void LevelSection::FlyOut(int pType) {
    mFlyOutType = pType;
    mFlyOutTimer = 0;
    
    if (pType == SECTION_FLY_TOP) {
        mFlyOutTime = FLY_TIME_TOP;
    } else {
        mFlyOutTime = FLY_TIME_SIDES;
    }
    
    Update();Update();
}

void LevelSection::UpdateFlyIn() {
    
    if (mFlyInType != SECTION_FLY_NONE) {
        if (mFlyInDelay <= 0) {
            mFlyInTimer += 1;
        }
        
        if (mFlyInTimer >= mFlyInTime) {
            mFlyInType = SECTION_FLY_NONE;
            mFlyInTimer = mFlyInTime;
            mX = 0.0f;
            mY = 0.0f;
        } else {
            
            float aStartX = 0.0f;
            float aStartY = 0.0f;
            
            float aEndX = 0.0f;
            float aEndY = 0.0f;
            
            if (mFlyInType == SECTION_FLY_LEFT) {
                aStartX = -(GAME_WIDTH + GAME_WIDTH / 4);
                aStartY = 0;
            }
            if (mFlyInType == SECTION_FLY_TOP) {
                aStartX = 0;
                aStartY = -(GAME_HEIGHT);
            }
            if (mFlyInType == SECTION_FLY_RIGHT) {
                aStartX = GAME_WIDTH + GAME_WIDTH / 4;
                aStartY = 0;
            }
            
            float aPercent = ((float)mFlyInTimer) / ((float)mFlyInTime);
            if (aPercent <= 0.0f) { aPercent = 0.0f; }
            if (aPercent >= 1.0f) { aPercent = 1.0f; }
            
            mX = aStartX + (aEndX - aStartX) * aPercent;
            mY = aStartY + (aEndY - aStartY) * aPercent;
        }
    }
}

void LevelSection::UpdateFlyOut() {
    if (mFlyOutType != SECTION_FLY_NONE) {
        
        float aStartX = 0.0f;
        float aStartY = 0.0f;
        
        float aEndX = 0.0f;
        float aEndY = 0.0f;
        
        if (mFlyOutType == SECTION_FLY_LEFT) {
            aEndX = -(GAME_WIDTH + GAME_WIDTH / 4);
            aEndY = 0;
        }
        if (mFlyOutType == SECTION_FLY_TOP) {
            aEndX = 0;
            aEndY = -(GAME_HEIGHT);
        }
        if (mFlyOutType == SECTION_FLY_RIGHT) {
            aEndX = GAME_WIDTH + GAME_WIDTH / 4;
            aEndY = 0;
        }
        
        mFlyOutTimer += 1;
        if (mFlyOutTimer >=  mFlyOutTime) {
            mFlyOutType = SECTION_FLY_NONE;
            mFlyOutTimer = mFlyOutType;
            mX = aEndX;
            mY = aEndY;
        } else {
            
            float aPercent = ((float)mFlyOutTimer) / ((float)mFlyOutTime);
            if (aPercent <= 0.0f) { aPercent = 0.0f; }
            if (aPercent >= 1.0f) { aPercent = 1.0f; }
            
            mX = aStartX + (aEndX - aStartX) * aPercent;
            mY = aStartY + (aEndY - aStartY) * aPercent;
        }
    }
}

void LevelSection::DrawDeadObjectMarkers() {
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->DrawDeadObjectMarkers();
    }
    
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->DrawDeadObjectMarkers();
    }
    
}

int LevelSection::GetClockTicks() {
    int aResult = 0;
    if (mDidSpawn == true) {
        EnumList(LevelWave, aWave, mWaveList) {
            EnumList(LevelWaveSpawn, aSpawn, aWave->mSpawnList) {
                if (aSpawn->mIsComplete == false) {
                    if (aSpawn->mClockTicks > aResult) {
                        aResult = aSpawn->mClockTicks;
                        //Log("aSpawn->mClockTicks = %d\n", aSpawn->mClockTicks);
                        
                    }
                }
            }
        }
        
        if (mForceKillTimer > aResult) {
            aResult = mForceKillTimer;
        }
    }
    
    return aResult;
}

int LevelSection::GetClockFlyingInTicksRemaining() {
    int aResult = 0;
    if (mDidSpawn == true) {
        if (mWaveList.mCount > 0) {
            EnumList(LevelWave, aWave, mWaveList) {
                
                
                
                int aStartWaitTime = aWave->mPath.mStartWaitIndex;
                EnumList(LevelWaveSpawn, aSpawn, aWave->mSpawnList) {
                    int aWait = (aStartWaitTime - aSpawn->mPathIndex);
                    if (aWait > aResult) {
                        aResult = aWait;
                    }
                }
            }
        } else {
            if (mFlyInType != SECTION_FLY_NONE) {
                aResult = (mFlyInTime - mFlyInTimer);
            }
        }
    }
    return aResult;
}

//Do we have any objects sitting on the board?
//Note: Be careful, this will assume that all spawning has taken place...
bool LevelSection::HasAnyObjects() {
    if (mIsComplete == false) { return true; }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        if (aPerm->HasAnyObjects() == true) {
            return true;
        }
    }
    
    return false;
}

bool LevelSection::HasAnyPermanents() {
    return (mPermList.mCount > 0);
}

bool LevelSection::HasAllPermanents() {
    return (mWaveList.mCount == 0) && (mPermList.mCount > 0);
}

bool LevelSection::HasAnyWaves() {
    return (mWaveList.mCount > 0);
}

bool LevelSection::HasAllWaves() {
    return (mWaveList.mCount > 0) && (mPermList.mCount == 0);
}

bool LevelSection::IsResting() {
    if (mDidSpawn == false) {
        return true;
    }
    if (mFlyInType != SECTION_FLY_NONE) {
        return true;
    }
    if (mFlyOutType != SECTION_FLY_NONE) {
        return true;
    }
    return false;
}

void LevelSection::HandOffAllGameObjects(FList *pList) {
    
    if (pList == NULL) { return; }
    
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->HandOffAllGameObjects(pList);
    }
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->HandOffAllGameObjects(pList);
    }
}

void LevelSection::HandOffAllPermanentGameObjects(FList *pList) {
    
    if (pList == NULL) { return; }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->HandOffAllGameObjects(pList);
    }
}

bool LevelSection::ShouldPauseExit() {
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        if (aPerm->ShouldPauseExit() == true) {
            return true;
        }
    }
    
    return false;
}

void LevelSection::BeginPauseExit() {
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->BeginPauseExit();
    }
}

bool LevelSection::IsPauseExitComplete() {
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        if (aPerm->IsPauseExitComplete() == false) {
            return false;
        }
    }
    
    return true;
}

bool LevelSection::IsClearForShortCircuitExternal() {
    if (mDidSpawn == true) {
        
        EnumList(LevelSectionPerm, aPerm, mPermList) {
            if (aPerm->IsClearForSectionCompletion() == false) {
                return false;
            }
        }
        
        for (int i=0;(i<mCandidateWaveIndex) && (i < mWaveList.mCount);i++) {
            LevelWave *aWave = (LevelWave *)(mWaveList.mData[i]);
            if (mCandidateWave != aWave) {
                if (aWave->IsClearForShortCircuitExternal() == false) {
                    return false;
                }
            }
        }
        
        if (mCandidateWave != NULL) {
            if ((mCandidateWave->mCreationRequiresPrevWaveComplete == true) || (mCandidateWave->mCreationRequiresScreenWavesClear == true)) {
                //This was will NOT spawn without clearing...
                //So we can ignore this wave...
                
            } else {
                return false;
            }
        }
        
        return true;
    }
    return false;
}

bool LevelSection::IsClearOfAllPermanentProgressObjects() {
    if (mDidSpawn == true) {
        EnumList(LevelSectionPerm, aPerm, mPermList) {
            if (aPerm->IsClearForSectionCompletion() == false) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void LevelSection::EditorDisposeAll() {
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->EditorDisposeAll();
    }
    
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->EditorDisposeAll();
    }
}

int LevelSection::GenerateObjectIndex() {
    int aResult = mSpawnObjectIndex;
    mSpawnObjectIndex += 1;
    return aResult;
}

void LevelSection::RefreshSection() {
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->RefreshSection(this);
    }
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->RefreshSection(this);
    }
}

bool LevelSection::ContainsObject(GameObject *pObject) {
    if (GetWaveForObject(pObject) != NULL) {
        return true;
    }
    if (GetPermForObject(pObject) != NULL) {
        return true;
    }
    return false;
}

void LevelSection::DisposeAllObjects() {
    EnumList(LevelWave, aWave, mWaveList) {
        aWave->DisposeAllObjects();
    }
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        aPerm->DisposeAllObjects();
    }
}

LevelWave *LevelSection::GetWaveForObject(GameObject *pObject) {
    LevelWave *aResult = NULL;
    EnumList(LevelWave, aWave, mWaveList) {
        if (aWave->ContainsObject(pObject) == true) {
            aResult = aWave;
        }
    }
    return aResult;
}

LevelSectionPerm *LevelSection::GetPermForObject(GameObject *pObject) {
    LevelSectionPerm *aResult = NULL;
    EnumList(LevelSectionPerm, aPerm, mPermList) {
        if (aPerm->ContainsObject(pObject) == true) {
            aResult = aPerm;
        }
    }
    return aResult;
}

void LevelSection::Load(const char *pFile) {
    
    mName = pFile;
    mName.RemovePath();
    mName.RemoveExtension();
    
    mLoadError = false;
    
    LevelSectionBlueprint aBlueprint;
    FJSON aJSON;
    aJSON.Load(pFile);
    
    if (aJSON.mRoot == NULL) {
        mLoadError = true;
        return;
    }
    
    aBlueprint.Load(aJSON.mRoot);
    aBlueprint.Build(this);
    
    RefreshSection();
    
}
