//
//  LevelWave.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/14/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelWave.hpp"
#include "Game.hpp"
#include "Balloon.hpp"

LevelWave::LevelWave() {
    
    mEditorPreviewMode = false;
    
    mIsComplete = false;
    
    //mPauseExitCompletionDelayTimer = 0;
    //mPauseExit = false;
    
    //mPauseShortCircuitSleeper = false;
    //mPauseShortCircuitSleeperTimer = 0;
    
    
    mCreationRequiresPrevWaveStart = false;
    mCreationRequiresPrevWaveComplete = false;
    mCreationRequiresScreenWavesClear = false;
    
    mSection = NULL;
    //mCreationRequiresScreenPermsClear = false;
    
    mDisableDeadObjectMarkers = false;
    
    mLoopCount = 0;
    
    mCreationDelay = 0;
    
    mSpawnIndex = 0;
    
    mSpawnSpacing = 120.0f;
    
    mKillTimer = 8;
}

LevelWave::~LevelWave() {
    
    DisposeAllObjects();
    
    FreeList(LevelWaveSpawn, mSpawnList);
    FreeList(LevelWaveSpawn, mSpawnKillList);
    FreeList(LevelWaveSpawn, mSpawnDeleteList);
}

void LevelWave::Reset() {
    mIsComplete = false;
    mPath.Reset();
    EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
        aSpawn->Reset();
        mSpawnKillList.Add(aSpawn);
    }
    mSpawnList.RemoveAll();
    mSpawnIndex = 0;
}

/*
void LevelWave::Restart() {
    mIsComplete = false;
    EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
        aSpawn->Restart();
    }
    mSpawnIndex = 0;
}
*/

void LevelWave::Prepare() {
    
    if (mPath.mDidFinalize == false) {
        mPath.Finalize();
    }
    
    //mClockTicks = 0;
    
    //if (mLoopCount > 1) {
    //    mPath.GenerateLoops(mLoopCount - 1);
    //}
    
}

void LevelWave::Update() {
    //
    EnumList(LevelWaveSpawn, aSpawn, mSpawnKillList) {
        aSpawn->mKillTimer--;
        if (aSpawn->mKillTimer <= 0) { mSpawnDeleteList.Add(aSpawn); }
    }
    EnumList(LevelWaveSpawn, aSpawn, mSpawnDeleteList) {
        mSpawnKillList.Remove(aSpawn);
        delete aSpawn;
    }
    mSpawnDeleteList.RemoveAll();
    //
    //////
    
    
    mPath.Update();
    
    if (mSpawnList.mCount <= 0) {
        mIsComplete = true;
        return;
    }
    
    
    for (int i=0;i<mSpawnIndex;i++) {
        LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
        aSpawn->mEditorPreviewMode = mEditorPreviewMode;
        
        if (aSpawn != NULL && aSpawn->mIsComplete == false) {
            if (aSpawn->mWaitTimer > 0) {
                
                if (aSpawn->mDidShortCircuit == true) {
                    aSpawn->mWaitTimer = 0;
                    if (aSpawn->mPathIndex < mPath.mPath.mCount) {
                        aSpawn->mPathIndex += 1;
                    }
                } else {
                    aSpawn->mWaitTimer--;
                    aSpawn->mClockTicks--;
                    if (aSpawn->mClockTicks < 0) { aSpawn->mClockTicks = 0; }
                    
                    if (aSpawn->mWaitTimer <= 0) {
                        aSpawn->mWaitTimer = 0;
                        if (aSpawn->mPathIndex < mPath.mPath.mCount) {
                            aSpawn->mPathIndex += 1;
                        }
                    }
                }
            } else {
                if (aSpawn->mPathIndex < mPath.mPath.mCount) {
                    aSpawn->mPathIndex += 1;
                }
            }
            
            bool aComplete = false;
            
            if (aSpawn->mPathIndex >= mPath.mPath.mCount) {
                if ((aSpawn->mWaitTimer <= 0) || (aSpawn->mDidShortCircuit == true)) {
                    aSpawn->mWaitTimer = 0;
                    aComplete = true;
                }
            } else {
                if (mPath.mWait.mData[aSpawn->mPathIndex] > 0 && aSpawn->mWaitTimer == 0) {
                    if (aSpawn->mDidShortCircuit == false) {
                        aSpawn->mWaitTimer = mPath.mWait.mData[aSpawn->mPathIndex];
                        aSpawn->mCurrentWaitTime = aSpawn->mWaitTimer;
                        aSpawn->mCurrentWaitTick = 0;
                        
                    }
                }
            }
            
            if (aSpawn->DidStart()) {
                if (mEditorPreviewMode == true) {
                    
                } else {
                    if (aSpawn->IsClear() == true) {
                        aComplete = true;
                    }
                }
            }
            
            if (aComplete) {
                aSpawn->mIsComplete = true;
                //HOOK: The wave is complete at this point...
                mRecentlyCompletedList.Add(aSpawn);
            }
        }
        
        if (aSpawn != NULL) {
            aSpawn->Update();
        }
    }
    
    if (mPath.mDidFinalize == true && mPath.mDidFailFinalize == false && mPath.mPath.mCount > 0) {
        if (mSpawnIndex >= 0 && mSpawnIndex < mSpawnList.mCount) {
            LevelWaveSpawn *aPrevSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(mSpawnIndex - 1);
            LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(mSpawnIndex);
            if (aSpawn !=  NULL) {
                bool aShouldSpawn = false;
                if (aPrevSpawn != NULL) {
                    
                    if (aPrevSpawn->mIsComplete) {
                        aShouldSpawn = true;
                    } else {
                        
                        if (aPrevSpawn->mDistanceTraveled > (mSpawnSpacing + aSpawn->mOffsetSpawnDistance)) {
                            aShouldSpawn = true;
                        }
                    }
                } else {
                    aShouldSpawn = true;
                }
                if (aShouldSpawn) {
                    if (aSpawn) {
                        aSpawn->mSection = mSection;
                        aSpawn->Spawn();
                        mSpawnIndex += 1;
                    }
                }
            }
        }
    }
    
    EnumList(LevelWaveSpawn, aSpawn, mRecentlyCompletedList) {
        HandleSpawnComplete(aSpawn);
    }
    mRecentlyCompletedList.RemoveAll();
    
    
    
    
    if (mSpawnIndex >= mSpawnList.mCount) {
        
        bool aComplete = true;
        
        EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
            if (aSpawn->mIsComplete == false) {
                aComplete = false;
            }
        }
        
        //if (aComplete == true) {
        //    mIsComplete = true;
        //}
        
        if (aComplete == true) {
            
            mIsComplete = true;
        }
    }
    
    
    if (mDisableDeadObjectMarkers == false) {
        if (IsClear() == true) {
            DisableDeadObjectMarkers();
        }
    }
    
}

void LevelWave::Draw() {
    for (int i=0;i<mSpawnIndex;i++) {
        LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
        if (aSpawn != NULL) {
            //aSpawn->Draw();
            
        }
    }
}

void LevelWave::DisableDeadObjectMarkers() {
    mDisableDeadObjectMarkers = true;
}

void LevelWave::DrawDeadObjectMarkers() {
    if (mDisableDeadObjectMarkers == false) {
        for (int i=0;i<mSpawnIndex;i++) {
            LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
            if (aSpawn != NULL) {
                aSpawn->DrawDeadObjectMarkers();
            }
        }
    }
}

void LevelWave::DisposeObject(GameObject *pObject) {
    for (int i=0;i<mSpawnIndex;i++) {
        LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
        aSpawn->DisposeObject(pObject);
    }
}

void LevelWave::DisposeAllObjects() {
    for (int i=0;i<mSpawnIndex;i++) {
        LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
        aSpawn->DisposeAllObjects();
    }
}

bool LevelWave::ContainsObject(GameObject *pObject) {
    for (int i=0;i<mSpawnIndex;i++) {
        LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
        if (aSpawn->ContainsObject(pObject) == true) {
            return true;
        }
    }
    return false;
}

void LevelWave::EditorDisposeAll() {
    EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
        aSpawn->EditorDisposeAll();
    }
}

void LevelWave::RefreshSection(LevelSection *pSection) {
    mSection = pSection;
    
    EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
        aSpawn->RefreshSection(pSection);
    }
}

void LevelWave::RefreshClockTime() {
    EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
        aSpawn->mClockTicks = mPath.mClockTime;
    }
}

bool LevelWave::IsClear() {
    if (mSpawnIndex >= mSpawnList.mCount) {
        EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
            if (aSpawn->IsClear() == false) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool LevelWave::IsResting(GameObject *pObject) {
    bool aResult = true;
    for (int i=0;i<mSpawnIndex;i++) {
        LevelWaveSpawn *aSpawn = (LevelWaveSpawn *)mSpawnList.Fetch(i);
        if (aSpawn->ContainsObject(pObject) == true) {
            
            if (aSpawn->mDidSpawn == true) {
                
                if (aSpawn->mWaitTimer == 0) {
                    //We are moving...
                    aResult = false;
                }
            }
            
        }
    }
    return aResult;
}

bool LevelWave::IsClearForShortCircuitExternal() {
    if (mSpawnIndex >= mSpawnList.mCount) {
        EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
            if (aSpawn->IsClearForSectionCompletion() == false) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void LevelWave::HandOffAllGameObjects(FList *pList) {
    if (pList == NULL) { return; }
    EnumList(LevelWaveSpawn, aSpawn, mSpawnList) {
        aSpawn->HandOffAllGameObjects(pList);
    }
}

void LevelWave::HandleSpawnComplete(LevelWaveSpawn *pSpawn) {
    
    FList aList;
    if (pSpawn) {
        pSpawn->HandOffAllGameObjects(&aList);
    }
    
    for (int i=0;i<aList.mCount;i++) {
        GameObject *aObject = ((GameObject *)aList.mData[i]);
        if (gGame != NULL) {
            gGame->FlyOffEscape(aObject);
        }
    }
}

