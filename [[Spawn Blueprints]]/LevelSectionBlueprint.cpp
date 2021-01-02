//
//  LevelSectionBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/18/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelSectionBlueprint.hpp"
#include "LevelSection.hpp"
#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

LevelSectionBlueprint::LevelSectionBlueprint() {
    mCurrentWave = NULL;
    mCurrentPerm = NULL;
    
    mKeepAliveTimer = 0;
    mForceKillTimer = 0;
    
    mRequiredFlyInType = SECTION_FLY_NONE;
    mRequiredFlyOutType = SECTION_FLY_NONE;
    
    mBlockFreeLives = false;
    
    mKnockDownPermsOnClear = false;
    
    mProgressCount = 0;
}

LevelSectionBlueprint::~LevelSectionBlueprint() {
    FreeList(LevelWaveBlueprint, mWaveList);
    FreeList(LevelWaveBlueprint, mKillWaveList);
    
    FreeList(LevelSectionPermBlueprint, mPermList);
    FreeList(LevelSectionPermBlueprint, mKillPermList);
}

void LevelSectionBlueprint::Reset() {
    
    mLinkData.Reset();
    
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
        mKillWaveList.Add(aWave);
    }
    mWaveList.RemoveAll();
    mCurrentWave = NULL;
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList[i];
        mKillPermList.Add(aPerm);
    }
    mPermList.RemoveAll();
    mCurrentPerm = NULL;
}

void LevelSectionBlueprint::Update() {
    
    
    EnumList(LevelWaveBlueprint, aWave, mWaveList) {
        aWave->Update();
    }
    
    EnumList(LevelSectionPermBlueprint, aPerm, mPermList) {
        aPerm->Update();
    }
    
    
    
    
    EnumList(LevelWaveBlueprint, aWave, mKillWaveList) {
        if (mCurrentWave == aWave) { mCurrentWave = NULL; }
        aWave->mKillTimer--;
        if (aWave->mKillTimer <= 0) { mDeleteWaveList.Add(aWave); }
    }
    EnumList(LevelWaveBlueprint, aWave, mDeleteWaveList) {
        mKillWaveList.Remove(aWave);
        delete aWave;
    }
    mDeleteWaveList.RemoveAll();
    
    
    
    
    EnumList(LevelSectionPermBlueprint, aPerm, mKillPermList) {
        if (mCurrentPerm == aPerm) { mCurrentPerm = NULL; }
        aPerm->mKillTimer--;
        if (aPerm->mKillTimer <= 0) { mDeletePermList.Add(aPerm); }
    }
    EnumList(LevelSectionPermBlueprint, aPerm, mDeletePermList) {
        mKillPermList.Remove(aPerm);
        delete aPerm;
    }
    mDeletePermList.RemoveAll();
    
}

void LevelSectionBlueprint::Draw() {
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
        aWave->Draw(aWave == mCurrentWave);
    }
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList[i];
        aPerm->Draw(aPerm == mCurrentPerm);
    }
}

int LevelSectionBlueprint::CountProgressObjects() {
    
    int aResult = 0;
    
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
        aResult += aWave->CountProgressObjects();
    }
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList.mData[i];
        aResult += aPerm->CountProgressObjects();
    }
    return aResult;
}


int LevelSectionBlueprint::CountObjectsOfType(int pGameObjectType) {
    int aResult = 0;
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
        aResult += aWave->CountObjectsOfType(pGameObjectType);
    }
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList.mData[i];
        aResult += aPerm->CountObjectsOfType(pGameObjectType);
    }
    return aResult;
}



void LevelSectionBlueprint::WaveAdd() {
    mCurrentWave = new LevelWaveBlueprint();
    mWaveList.Add(mCurrentWave);
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
}

void LevelSectionBlueprint::WaveInsert() {
    
    int aIndex = mWaveList.Find(mCurrentWave);
    if (aIndex == -1) {
        aIndex = mWaveList.mCount;
    } else {
        aIndex += 1;
    }
    
    mCurrentWave = new LevelWaveBlueprint();
    mWaveList.Insert(mCurrentWave, aIndex);
    if (mCurrentWave != NULL) {
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
    }
}

void LevelSectionBlueprint::WaveRemove() {
    if (mCurrentWave != NULL) {
        mWaveList.Remove(mCurrentWave);
        mKillWaveList.Add(mCurrentWave);
        mCurrentWave = NULL;
        WaveSelectPrev();
    }
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
    
}

void LevelSectionBlueprint::WaveSelectNext() {
    if ((mWaveList.mCount) > 0 && (mCurrentWave != NULL)) {
        int aIndex = mWaveList.Find(mCurrentWave) + 1;
        if (aIndex >= mWaveList.mCount) aIndex = 0;
        mCurrentWave = (LevelWaveBlueprint *)mWaveList.Fetch(aIndex);
    } else {
        mCurrentWave = (LevelWaveBlueprint *)mWaveList.First();
    }
    if (mCurrentWave != NULL) {
        
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
    }
}

void LevelSectionBlueprint::WaveSelectPrev() {
    if ((mWaveList.mCount) > 0 && (mCurrentWave != NULL)) {
        int aIndex = mWaveList.Find(mCurrentWave) - 1;
        if (aIndex < 0) aIndex = mWaveList.mCount - 1;
        mCurrentWave = (LevelWaveBlueprint *)mWaveList.Fetch(aIndex);
    } else {
        mCurrentWave = (LevelWaveBlueprint *)mWaveList.Last();
    }
    if (mCurrentWave != NULL) {
        
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
        
    }
}

void LevelSectionBlueprint::WaveDeselect() {
    mCurrentWave = NULL;
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
    
}

void LevelSectionBlueprint::WaveMoveUp() {
    mWaveList.MoveObjectDown(mCurrentWave);
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
    
}

void LevelSectionBlueprint::WaveMoveDown() {
    mWaveList.MoveObjectUp(mCurrentWave);
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
}

void LevelSectionBlueprint::WaveSelect(int pIndex) {
    mCurrentWave = (LevelWaveBlueprint *)mWaveList.Fetch(pIndex);
    if (mCurrentWave != NULL) {
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
    }
}

int LevelSectionBlueprint::WaveCount(int pIndex) {
    LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList.Fetch(pIndex);
    if (aWave != NULL) { return aWave->mPath.mNodeList.mCount; }
    return 0;
}


void LevelSectionBlueprint::PermAdd(float pX, float pY) {
    mCurrentPerm = new LevelSectionPermBlueprint();
    mCurrentPerm->mEditorX = pX;
    mCurrentPerm->mEditorY = pY;
    
    #ifdef EDITOR_MODE
    if (gPermEditor != NULL) {
        
        if (gPermEditor->mGrid.mGridEnabled) {
            float aX = pX;
            float aY = pY;
            gPermEditor->mGrid.GridSnap(&aX, &aY);
            mCurrentPerm->mEditorX = aX;
            mCurrentPerm->mEditorY = aY;
            mCurrentPerm->mConstraint.mTypeX = X_CONSTRAINT_NONE;
            mCurrentPerm->mConstraint.mTypeY = Y_CONSTRAINT_NONE;
        } else {
            if (gPermEditor->mSnapsEnabled == true) {
                mCurrentPerm->mConstraint.mTypeX = gEditor->ClosestXConstraint(pX);
                mCurrentPerm->mConstraint.mTypeY = gEditor->ClosestYConstraint(pY);
            }
        }
    }
    #endif
    
    mPermList.Add(mCurrentPerm);
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
    mCurrentPerm->ApplyEditorConstraints();
}

void LevelSectionBlueprint::PermRemove() {
    if (mCurrentPerm != NULL) {
        mPermList.Remove(mCurrentPerm);
        mKillPermList.Add(mCurrentPerm);
        mCurrentPerm = NULL;
        PermSelectPrev();
    }
}

void LevelSectionBlueprint::PermSelectNext() {
    if ((mWaveList.mCount) > 0 && (mCurrentPerm != NULL)) {
        int aIndex = mPermList.Find(mCurrentPerm) + 1;
        if (aIndex >= mPermList.mCount) aIndex = 0;
        mCurrentPerm = (LevelSectionPermBlueprint *)mPermList.Fetch(aIndex);
    } else {
        mCurrentPerm = (LevelSectionPermBlueprint *)mPermList.First();
    }
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
}

void LevelSectionBlueprint::PermSelectPrev() {
    if ((mPermList.mCount) > 0 && (mCurrentPerm != NULL)) {
        int aIndex = mPermList.Find(mCurrentPerm) - 1;
        if (aIndex < 0) aIndex = mWaveList.mCount - 1;
        mCurrentPerm = (LevelSectionPermBlueprint *)mPermList.Fetch(aIndex);
    } else {
        mCurrentPerm = (LevelSectionPermBlueprint *)mPermList.Last();
    }
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
}

void LevelSectionBlueprint::PermDeselect() {
    mCurrentPerm = NULL;
}

void LevelSectionBlueprint::PermMoveUp() {
    mPermList.MoveObjectDown(mCurrentPerm);
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
}

void LevelSectionBlueprint::PermMoveDown() {
    mPermList.MoveObjectUp(mCurrentPerm);
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
}

void LevelSectionBlueprint::PermSelect(int pIndex) {
    mCurrentPerm = (LevelSectionPermBlueprint *)mPermList.Fetch(pIndex);
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            gEditor->RefreshPlayback();
        }
    #endif
}

int LevelSectionBlueprint::PermSelectClosest(float pX, float pY) {
    int aResult = -1;
    float aBestDist = 80.0f * 80.0f;
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList[i];
        
        float aPathDist = 80.0f * 80.0f;
        aPerm->mPath.GetClosestIndex(pX, pY, aPathDist);
        if (aPathDist < aBestDist) {
            aResult = i;
            aBestDist = aPathDist;
        }
        
        float aDist = DistanceSquared(aPerm->mEditorX, aPerm->mEditorY, pX, pY);
        
        if (aDist < aBestDist) {
            aResult = i;
            aBestDist = aDist;
        }
    }
    
    if (aResult >= 0 && aResult < mPermList.mCount) {
        mCurrentPerm = (LevelSectionPermBlueprint *)mPermList.Fetch(aResult);
    } else {
        mCurrentPerm = NULL;
    }
    
    return aResult;
}

void LevelSectionBlueprint::PermRefreshPositions() {
#ifdef EDITOR_MODE
    float aLeft = gEditor->mGameAreaLeft;
    float aRight = gEditor->mGameAreaRight;
    float aTop = gEditor->mGameAreaTop;
    float aBottom = gEditor->mGameAreaBottom;
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList[i];
        aPerm->mPercentX = ((aPerm->mEditorX - aLeft) / (aRight - aLeft)) * 100.0f;
        aPerm->mPercentY = ((aPerm->mEditorY - aTop) / (aBottom - aTop)) * 100.0f;
    }
#endif
    
}

void LevelSectionBlueprint::Build() {
#ifdef EDITOR_MODE
    if (gEditor) {
        Build(&gEditor->mEditorSection);
    }
#endif
}

void LevelSectionBlueprint::Build(LevelSection *pSection) {
    
    if (pSection == NULL) { return; }
    
    pSection->Reset();
    
    pSection->mKeepAliveTimer = mKeepAliveTimer;
    pSection->mForceKillTimer = mForceKillTimer;
    pSection->mProgressCount = mProgressCount;
    
    pSection->mRequiredFlyInType = mRequiredFlyInType;
    pSection->mRequiredFlyOutType = mRequiredFlyOutType;
    pSection->mKnockDownPermsOnClear = mKnockDownPermsOnClear;
    pSection->mBlockFreeLives = mBlockFreeLives;
    
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWaveBlueprint = (LevelWaveBlueprint *)mWaveList[i];
        LevelWave *aWave = new LevelWave();
        aWave->mSection = pSection;
        aWaveBlueprint->Build(aWave);
        pSection->AddWave(aWave);
    }
    
    for (int i=0;i<mPermList.mCount;i++) {
        LevelSectionPermBlueprint *aPermBlueprint = (LevelSectionPermBlueprint *)mPermList.mData[i];
        LevelSectionPerm *aPerm = new LevelSectionPerm(pSection);
        aPerm->mSection = pSection;
        aPermBlueprint->Build(aPerm);
        pSection->AddPerm(aPerm);
    }
}

FJSONNode *LevelSectionBlueprint::Save() {
    FJSONNode *aExport = new FJSONNode();
    aExport->AddDictionaryBool("section", true);
    
    if (mKeepAliveTimer != 0) { aExport->AddDictionaryInt("keep_alive_timer", mKeepAliveTimer); }
    
    if (mForceKillTimer != 0) { aExport->AddDictionaryInt("force_kill_timer", mForceKillTimer); }
    
    if (mRequiredFlyInType != SECTION_FLY_NONE) { aExport->AddDictionaryInt("fly_in_type", mRequiredFlyInType); }
    if (mRequiredFlyOutType != SECTION_FLY_NONE) { aExport->AddDictionaryInt("fly_out_type", mRequiredFlyOutType); }
    
    if (mKnockDownPermsOnClear != false) { aExport->AddDictionaryBool("knock_down_perms", mKnockDownPermsOnClear); }
    if (mBlockFreeLives != false) { aExport->AddDictionaryBool("block_free_lives", mBlockFreeLives); }
    
    if (mLinkData.ShouldSave() == true) { aExport->AddDictionary("link_list", mLinkData.Save()); }
    
    mProgressCount = CountProgressObjects();
    aExport->AddDictionaryInt("progress", mProgressCount);

    // ==== begin size correction ====
    
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
        aWave->FindLargestSpawnSize();
    }
    
    bool aReloop = true;
    LevelWaveBlueprint *aWave1 = NULL;
    LevelWaveBlueprint *aWave2 = NULL;
    int aSize1 = 0;
    int aSize2 = 0;
    int aMaxSize = 0;
    while (aReloop == true) {
        aReloop = false;
        for (int i=0;i<4;i++) {
            if (i == 0) {
                aWave1 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_1_1));
                aWave2 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_1_2));
            } else if (i == 1) {
                aWave1 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_2_1));
                aWave2 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_2_2));
            } else if (i == 2) {
                aWave1 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_3_1));
                aWave2 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_3_2));
            } else {
                aWave1 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_4_1));
                aWave2 = (LevelWaveBlueprint *)(mWaveList.Fetch(mLinkData.mLink_4_2));
            }
            
            if ((aWave1 != NULL) && (aWave2 != NULL) && (aWave1 != aWave2)) {
                aSize1 = aWave1->mMaxSpawnWidth;
                aSize2 = aWave2->mMaxSpawnWidth;
                aMaxSize = max(aSize1, aSize2);
                if (aWave1->mMaxSpawnWidth != aSize1 || aWave2->mMaxSpawnWidth != aSize1) {
                    aReloop = true;
                    aWave1->mMaxSpawnWidth = aMaxSize;
                    aWave2->mMaxSpawnWidth = aMaxSize;
                }
                
                aSize1 = aWave1->mMaxSpawnHeight;
                aSize2 = aWave2->mMaxSpawnHeight;
                aMaxSize = max(aSize1, aSize2);
                if (aWave1->mMaxSpawnHeight != aSize1 || aWave2->mMaxSpawnHeight != aSize1) {
                    aReloop = true;
                    aWave1->mMaxSpawnHeight = aMaxSize;
                    aWave2->mMaxSpawnHeight = aMaxSize;
                }
            }
        }
    }
    
    //for (int i=0;i<mWaveList.mCount;i++) {
    //    LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
    //    Log("Wave[%d] Max [%d / %d]\n", i, aWave->mMaxSpawnWidth, aWave->mMaxSpawnHeight);
    //}
    
    // ==== end size correction ====
    
    
    FJSONNode *aWaveArray = new FJSONNode();
    aExport->AddDictionary("wave_list", aWaveArray);
    aWaveArray->mNodeType = JSON_NODE_TYPE_ARRAY;
    
    for (int i=0;i<mWaveList.mCount;i++) {
        LevelWaveBlueprint *aWave = (LevelWaveBlueprint *)mWaveList[i];
        aWaveArray->AddArray(aWave->Save());
    }
    
    if (mPermList.mCount > 0) {
        FJSONNode *aPermArray = new FJSONNode();
        aExport->AddDictionary("perm_list", aPermArray);
        aPermArray->mNodeType = JSON_NODE_TYPE_ARRAY;
        for (int i=0;i<mPermList.mCount;i++) {
            LevelSectionPermBlueprint *aPerm = (LevelSectionPermBlueprint *)mPermList.mData[i];
            aPermArray->AddArray(aPerm->Save());
        }
    }
    
    return aExport;
}

void LevelSectionBlueprint::Load(FJSONNode *pNode) {
    Reset();
    if (pNode == NULL) { return; }
    
    mKeepAliveTimer = pNode->GetInt("keep_alive_timer", 0);
    mForceKillTimer = pNode->GetInt("force_kill_timer", 0);
    mProgressCount = pNode->GetInt("progress", 0);
    
    mRequiredFlyInType = pNode->GetInt("fly_in_type", SECTION_FLY_NONE);
    mRequiredFlyOutType = pNode->GetInt("fly_out_type", SECTION_FLY_NONE);
    
    mKnockDownPermsOnClear = pNode->GetBool("knock_down_perms", false);
    mBlockFreeLives = pNode->GetBool("block_free_lives", false);
    
    FJSONNode *aLinkArray = pNode->GetArray("link_list");
    mLinkData.Reset();
    if (aLinkArray != NULL) {
        mLinkData.Load(aLinkArray);
    }
    
    FJSONNode *aWaveArray = pNode->GetArray("wave_list");
    if (aWaveArray != NULL) {
        EnumJSONArray(aWaveArray, aWaveNode) {
            LevelWaveBlueprint *aWave = new LevelWaveBlueprint();
            aWave->Load(aWaveNode);
            mWaveList.Add(aWave);
        }
    }
    
    FJSONNode *aPermArray = pNode->GetArray("perm_list");
    if (aPermArray != NULL) {
        EnumJSONArray(aPermArray, aPermNode) {
            LevelSectionPermBlueprint *aPerm = new LevelSectionPermBlueprint();
            aPerm->Load(aPermNode);
            mPermList.Add(aPerm);
        }
    }
}


