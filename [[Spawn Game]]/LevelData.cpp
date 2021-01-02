//
//  LevelData.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/10/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelData.hpp"
#include "Game.hpp"

LevelData::LevelData() {
    mEditorPreviewMode = false;
    mCurrentSectionIndex = 0;
    mDelayedDispose = 0;
    mInitialDelay = 60;
    mCurrentSection = NULL;
    mPreviousSection = NULL;
    mIsComplete = false;
    mIsHalted = false;
    
    
    mClockMode = Uninitialized;
    mClockTime = 0;
    mClockTicks = 0;
    
    mClockFlyingInTicksRemaining = 0;
    
}

LevelData::~LevelData() {
    DisposeAllObjects();
    FreeList(LevelSection, mSectionList);
}

void LevelData::Update() {
    
    if (mInitialDelay > 0) {
        --mInitialDelay;
        return;
    }
    
    if ((mPreviousSection != NULL) && (mIsHalted == false)) {
        mPreviousSection->Update();
        if (mPreviousSection->mFlyOutType == SECTION_FLY_NONE) {
            DisposeSection(mPreviousSection);
            mPreviousSection = NULL;
        }
    }
    
    if ((mCurrentSection != NULL) && (mIsHalted == false)) {
        mCurrentSection->Update();
        
        bool aKeepAlive = false;
        bool aForceKill = false;
        
        if (mCurrentSection->mFlyInType == SECTION_FLY_NONE &&
            mCurrentSection->mFlyOutType == SECTION_FLY_NONE) {
            
            if (mCurrentSection->mForceKillTimer > 0) {
                mCurrentSection->mForceKillTimer--;
                if (mCurrentSection->mForceKillTimer <= 0) {
                    aForceKill = true;
                }
            }
            
            if (mCurrentSection->mKeepAliveTimer > 0) {
                mCurrentSection->mKeepAliveTimer--;
                if (mCurrentSection->mKeepAliveTimer > 0) {
                    aKeepAlive = true;
                }
            }
        } else {
            if (mCurrentSection->mKeepAliveTimer > 0) {
                aKeepAlive = true;
            }
        }
        
        if (mCurrentSection->mFlyInType != SECTION_FLY_NONE ||
            mCurrentSection->mFlyOutType != SECTION_FLY_NONE) {
            aKeepAlive = true;
        }
        
        if (aKeepAlive == false) {
            
            bool aExecuteFlyOut = false;
            
            if (mDelayedDispose > 0) {
                mDelayedDispose--;
                if (mDelayedDispose <= 0) {
                    mDelayedDispose = 0;
                    aExecuteFlyOut = true;
                }
            } else {
                if (((mCurrentSection->mIsComplete == true) || (aForceKill == true))) {
                    
                    DisposeSection(mPreviousSection);
                    mPreviousSection = NULL;
                    
                    if (mCurrentSection->HasAnyObjects()) {
                        if (aForceKill == true) {
                            aExecuteFlyOut = true;
                        } else {
                            mDelayedDispose = 5;
                        }
                    } else {
                        DisposeSection(mCurrentSection);
                        mCurrentSectionIndex++;
                        mCurrentSection = NULL;
                        
                        /*
#ifdef CALIBRATION_MODE
                        if (mCurrentSectionIndex < mSectionList.mCount) {
                            gGame->mTimeWhenSectionStarted = gGame->mTime;
                            gGame->SaveState();
                        }
#endif
                         */
                        
                    }
                }
            }
            
            if (aExecuteFlyOut == true) {
                DisposeSection(mPreviousSection);
                mPreviousSection = mCurrentSection;
                mCurrentSection = NULL;
                mCurrentSectionIndex++;
                
                /*
#ifdef CALIBRATION_MODE
                if (mCurrentSectionIndex < mSectionList.mCount) {
                    gGame->mTimeWhenSectionStarted = gGame->mTime;
                    gGame->SaveState();
                }
#endif
                 */
                
                if ((mPreviousSection != NULL) && (mPreviousSection->mKnockDownPermsOnClear == true) && mPreviousSection->IsClearOfAllPermanentProgressObjects() ) {
                    
                    KnockDownAllObjectsFromSection(mPreviousSection);
                    DisposeSection(mPreviousSection);
                    mPreviousSection = NULL;
                    
                    
                } else {
                    FlyOutPreviousSection();
                }
            }
        }
    }
    
    //Logic to start-up the current section... Now we can check if there was a forced fly out type on the previous section..?
    
    if ((mCurrentSection == NULL) && (mIsHalted == false)) {
        if (mCurrentSectionIndex < mSectionList.mCount) {
            
            mCurrentSection = (LevelSection *)(mSectionList[mCurrentSectionIndex]);
            mCurrentSection->Spawn();
            
            FlyInCurrentSection();
            
            mClockMode = FlyingIn;
            mClockTime = mCurrentSection->GetClockTicks();
            mClockTicks = mClockTime;
            
            //Log("*** Spawning Level Section: [%s]\n", mCurrentSection->mName.c());
            
        } else {
            mClockMode = AllSectionsCompleted;
            mClockTime = 0;
            mClockTicks = 0;
        }
    }
    
    if (mCurrentSection != NULL) {
        mClockTicks = mCurrentSection->GetClockTicks();
        if (mClockTicks < mClockTime) {
            if (mClockTicks <= 0) {
                if (mCurrentSectionIndex < (mSectionList.mCount - 1)) {
                    mClockMode = FlyingOut;
                } else {
                    mClockMode = AllSectionsCompleted;
                }
            } else {
                mClockMode = CountingDown;
            }
        } else {
            mClockMode = FlyingIn;
        }
    }
    
    if (mCurrentSectionIndex >= mSectionList.mCount) {
        if (mCurrentSection == NULL) {
            if (mPreviousSection == NULL) {
                mIsComplete = true;
            }
        }
    }
    
    if (mClockMode == FlyingIn) {
        if (mCurrentSection != NULL) {
            mClockFlyingInTicksRemaining = mCurrentSection->GetClockFlyingInTicksRemaining();
        } else {
            mClockFlyingInTicksRemaining = 0;
        }
        
    } else {
        mClockFlyingInTicksRemaining = 0;
    }
    
}

void LevelData::FlyOutPreviousSection() {
    
    if (mPreviousSection == NULL) {
        return;
    }
    
    if (mPreviousSection->mRequiredFlyOutType == SECTION_FLY_LEFT) {
        mPreviousSection->FlyOut(SECTION_FLY_LEFT);
    } else if (mPreviousSection->mRequiredFlyOutType == SECTION_FLY_TOP) {
        mPreviousSection->FlyOut(SECTION_FLY_TOP);
    } else if (mPreviousSection->mRequiredFlyOutType == SECTION_FLY_RIGHT) {
        mPreviousSection->FlyOut(SECTION_FLY_RIGHT);
    } else {
        if (gRand.GetBool()) {
            mPreviousSection->FlyOut(SECTION_FLY_LEFT);
        } else {
            mPreviousSection->FlyOut(SECTION_FLY_RIGHT);
        }
    }
}

void LevelData::FlyInCurrentSection() {
    
    if (mCurrentSection == NULL) {
        return;
    }
    
    int aFlyInDelayTimer = 0;
   
    
    if (mPreviousSection != NULL) {
        
        bool aRequiresDelay = false;
        
        if (mCurrentSection->mRequiredFlyInType == SECTION_FLY_TOP) {
            aRequiresDelay = true;
        }
        
        if (mPreviousSection->mFlyOutType == SECTION_FLY_TOP) {
            aRequiresDelay = true;
        }
        
        if ((mPreviousSection->mFlyOutType == SECTION_FLY_LEFT) && (mCurrentSection->mRequiredFlyInType == SECTION_FLY_LEFT)) {
            aRequiresDelay = true;
        }
        
        if ((mPreviousSection->mFlyOutType == SECTION_FLY_RIGHT) && (mCurrentSection->mRequiredFlyInType == SECTION_FLY_RIGHT)) {
            aRequiresDelay = true;
        }
        
        if (mPreviousSection->HasAnyObjects() == false) {
            aRequiresDelay = false;
        }
        
        if (aRequiresDelay == true) {
            aFlyInDelayTimer = mPreviousSection->mFlyOutTime;
        }
    }
    
    int aFlyInType = SECTION_FLY_NONE;
    
    if (mCurrentSection->mRequiredFlyInType == SECTION_FLY_TOP) {
        aFlyInType = SECTION_FLY_TOP;
    } else if (mCurrentSection->mRequiredFlyInType == SECTION_FLY_LEFT) {
        aFlyInType = SECTION_FLY_LEFT;
    } else if (mCurrentSection->mRequiredFlyInType == SECTION_FLY_RIGHT) {
        aFlyInType = SECTION_FLY_RIGHT;
    } else {
    
        //mCurrentSection->mRequiredFlyInType == SECTION_FLY_NONE
        
        bool aIsFirstSection = false;
        if (mCurrentSectionIndex == 0) {
            aIsFirstSection = true;
        }
        
        if (aIsFirstSection == true) {
            //Test: For first section, we do not fly in...
            return;
        }
        
        //If we don't have permanents, we default to the
        if (mCurrentSection->HasAnyPermanents()) {
            
            if (gRand.GetBool()) {
                aFlyInType = SECTION_FLY_RIGHT;
            } else {
                aFlyInType = SECTION_FLY_LEFT;
            }
            
            if (mPreviousSection != NULL) {
                if (mPreviousSection->mFlyOutType == SECTION_FLY_LEFT) {
                    aFlyInType = SECTION_FLY_RIGHT;
                }
                if (mPreviousSection->mFlyOutType == SECTION_FLY_RIGHT) {
                    aFlyInType = SECTION_FLY_LEFT;
                }
            }
        } else {
            aFlyInType = SECTION_FLY_NONE;
        }
    }
    
    if (true) {
        
        FString aTypeName;
        
        if (aFlyInType == SECTION_FLY_LEFT) {
            aTypeName = "LEFT";
        } else if (aFlyInType == SECTION_FLY_RIGHT) {
            aTypeName = "RIGHT";
        } else if (aFlyInType == SECTION_FLY_TOP) {
            aTypeName = "TOP";
        } else {
            aTypeName = "NONE";
        }
        
        //Log("Fly In: [%s] Del_Ticks: %d\n", aTypeName.c(), aFlyInDelayTimer);
        
    }
    
    mCurrentSection->FlyIn(aFlyInType, aFlyInDelayTimer);
    
}

void LevelData::DisposeObject(GameObject *pObject) {
    
    EnumList(LevelSection, aSection, mSectionList) {
        aSection->DisposeObject(pObject);
    }
    
    /*
    if (mPreviousSection != NULL) {
        mPreviousSection->DisposeObject(pObject);
    }
    
    if (mCurrentSection != NULL) {
        mCurrentSection->DisposeObject(pObject);
    }
    */
}

void LevelData::DisposeAllObjects() {
    
    EnumList(LevelSection, aSection, mSectionList) {
        aSection->DisposeAllObjects();
    }
    
    /*
    if (mPreviousSection != NULL) {
        mPreviousSection->DisposeAllObjects();
    }
    
    if (mCurrentSection != NULL) {
        mCurrentSection->DisposeAllObjects();
    }
    */
}

void LevelData::KnockDownAllObjectsFromSection(LevelSection *pLevelSection) {
    
    if (pLevelSection == NULL) { return; }
    
    //Log("*** Disposing Level Section: [%s]\n", pLevelSection->mName.c());
    
    if (pLevelSection == mCurrentSection) { mCurrentSection = NULL; }
    if (pLevelSection == mPreviousSection) { mPreviousSection = NULL; }
    
    FList aList;
    pLevelSection->HandOffAllGameObjects(&aList);
    for (int i=0;i<aList.mCount;i++) {
        GameObject *aObject = ((GameObject *)aList.mData[i]);
        if (gGame != NULL) {
            gGame->KnockDown(aObject);
            //gGame->FlyOffEscape(aObject);
        }
    }
    
    pLevelSection->Reset();
    
}

void LevelData::DisposeSection(LevelSection *pLevelSection) {
    
    if (pLevelSection == NULL) { return; }
    
    //Log("*** Disposing Level Section: [%s]\n", pLevelSection->mName.c());
    
    if (pLevelSection == mCurrentSection) { mCurrentSection = NULL; }
    if (pLevelSection == mPreviousSection) { mPreviousSection = NULL; }
    
    FList aList;
    pLevelSection->HandOffAllPermanentGameObjects(&aList);
    for (int i=0;i<aList.mCount;i++) {
        GameObject *aObject = ((GameObject *)aList.mData[i]);
        if (gGame != NULL) {
            gGame->FlyOffEscape(aObject);
        }
    }
    
    pLevelSection->Reset();
    
}

void LevelData::Print(const char *pName) {
    
}

void LevelData::AddSection(LevelSection *pLevelSection) {
    if (pLevelSection != NULL) {
        pLevelSection->mEditorPreviewMode = mEditorPreviewMode;
        mSectionList.Add(pLevelSection);
    }
}

int LevelData::GetProgressCount() {
    
    int aResult = 0;
    EnumList(LevelSection, aSection, mSectionList) {
        aResult += aSection->mProgressCount;
    }
    return aResult;
}


void LevelData::Halt() {
    mIsHalted = true;
}

void LevelData::DrawDeadObjectMarkers() {
    if (mPreviousSection != NULL) {
        mPreviousSection->DrawDeadObjectMarkers();
    }
    if (mCurrentSection != NULL) {
        mCurrentSection->DrawDeadObjectMarkers();
    }
}

LevelSection *LevelData::GetSectionForObject(GameObject *pObject) {
    LevelSection *aResult = NULL;
    
    if (pObject != NULL) {
        if (mCurrentSection != NULL) {
            if (mCurrentSection->ContainsObject(pObject)) {
                return mCurrentSection;
            }
        }
        
        if (mPreviousSection != NULL) {
            if (mPreviousSection->ContainsObject(pObject)) {
                return mPreviousSection;
            }
        }
        
    }
    
    return aResult;
}

LevelWave *LevelData::GetWaveForObject(GameObject *pObject) {
    LevelWave *aResult = NULL;
    LevelSection *aSection = GetSectionForObject(pObject);
    if ((aSection != NULL) && (pObject != NULL)) {
        aResult = aSection->GetWaveForObject(pObject);
    }
    return aResult;
}

LevelSectionPerm *LevelData::GetPermForObject(GameObject *pObject) {
    LevelSectionPerm *aResult = NULL;
    
    LevelSection *aSection = GetSectionForObject(pObject);
    if ((aSection != NULL) && (pObject != NULL)) {
        aResult = aSection->GetPermForObject(pObject);
    }
    
    return aResult;
}
