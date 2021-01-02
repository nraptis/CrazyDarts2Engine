//
//  LevelWaveBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/18/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelWaveBlueprint.hpp"
#include "LevelWave.hpp"
#include "FormationCollection.hpp"

#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

LevelWaveBlueprint::LevelWaveBlueprint() {
    mPath.mWave = this;
    
    for (int i=0;i<WAVE_MAX_SPAWN_COUNT;i++) {
        mSpawn[i].Reset();
    }
    
    mSelectedSpawnIndex = 0;
    
    mSpawnCount = 1;
    mSpawnSpacing = 120;
    
    mMaxSpawnWidth = 50;
    mMaxSpawnHeight = 50;
    
    //mCreationType = WAVE_CREATION_TYPE_SCREEN_CLEAR_IGNORE_PERMS;
    
    mCreationRequiresPrevWaveStart = false;
    mCreationRequiresPrevWaveComplete = false;
    mCreationRequiresScreenWavesClear = false;
    
    mLoopCount = 0;
    
    mCreationDelay = 0;
    
    mKillTimer = 8;
}

LevelWaveBlueprint::~LevelWaveBlueprint() {
    
}

void LevelWaveBlueprint::Reset() {
    mPath.Reset();
    for (int i=0;i<WAVE_MAX_SPAWN_COUNT;i++) {
        mSpawn[i].Reset();
    }
    mSpawnCount = 1;
}

//mPath
void LevelWaveBlueprint::Update() {
    mPath.Update();
    
    if (mSpawnCount < 1) mSpawnCount = 1;
    if (mSpawnCount > WAVE_MAX_SPAWN_COUNT) mSpawnCount = WAVE_MAX_SPAWN_COUNT;
    
    if (mSelectedSpawnIndex < 0) { mSelectedSpawnIndex = 0; }
    if (mSelectedSpawnIndex >= mSpawnCount) { mSelectedSpawnIndex = mSpawnCount - 1; }

    
    
}

void LevelWaveBlueprint::Draw(bool pSelected) {
    mPath.Draw(pSelected);
    
    /*
    if (pSelected) {
        Graphics::PipelineStateSetShape2DAlphaBlending();
        Graphics::SetColor(0.8f, 0.45f, 0.35f, 0.85f);
        
        if (mPath.mNodeList.mCount > 0) {
            
            LevelPathNodeBlueprint *aFirst = (LevelPathNodeBlueprint *)mPath.mNodeList.First();
            
            float aX = aFirst->mEditorX;
            float aY = aFirst->mEditorY;
            
            float aWidth2 = ((float)mMaxSpawnWidth) / 2.0f;
            float aWidth = ((float)mMaxSpawnWidth);
            
            float aHeight2 = ((float)mMaxSpawnHeight) / 2.0f;
            float aHeight = ((float)mMaxSpawnHeight);
            
            Graphics::OutlineRect(aX - aWidth2, aY - aHeight2, aWidth, aHeight, 2.0f);
            
            if (mPath.mNodeList.mCount > 1) {
                
                LevelPathNodeBlueprint *aLast = (LevelPathNodeBlueprint *)mPath.mNodeList.Last();
                
                aX = aLast->mEditorX;
                aY = aLast->mEditorY;
                
                Graphics::SetColor(0.35f, 0.85f, 0.35f, 0.25f);
                Graphics::OutlineRect(aX - aWidth2, aY - aHeight2, aWidth, aHeight, 2.0f);
            }
        }
    }
    */
    
}

int LevelWaveBlueprint::CountProgressObjects() {
    int aResult = 0;
    for (int i=0;i<mSpawnCount;i++) {
        aResult += mSpawn[i].CountProgressObjects();
    }
    return aResult;
}

int LevelWaveBlueprint::CountObjectsOfType(int pGameObjectType) {
    int aResult = 0;
    for (int i=0;i<mSpawnCount;i++) {
        aResult += mSpawn[i].CountObjectsOfType(pGameObjectType);
    }
    return aResult;
}

void LevelWaveBlueprint::ApplyEditorConstraints() {
    mPath.ApplyEditorConstraints();
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
    
}

void LevelWaveBlueprint::FindLargestSpawnSize() {
    
    mMaxSpawnWidth = 50;
    mMaxSpawnHeight = 50;
    
    float aMinX = 0.0f;
    float aMinY = 0.0f;
    float aMaxX = 0.0f;
    float aMaxY = 0.0f;
    
    for (int i=0;i<mSpawnCount;i++) {
        
        LevelFormation *aFormation = gFormationCollection.GetFormation(mSpawn[i].mFormationID.c());
        if (aFormation != NULL) {
            
            aFormation->Spawn(NULL, NULL);
            
            float aTop, aRight, aBottom, aLeft;

            aFormation->GetExtremeties(aTop, aRight, aBottom, aLeft);
            
            if (aTop < aMinY) { aMinY = aTop; }
            if (aBottom > aMaxY) { aMaxY = aBottom; }
            
            if (aLeft < aMinX) { aMinX = aLeft; }
            if (aRight > aMaxX) { aMaxX = aRight; }
            
            aFormation->Reset();
            delete aFormation;
        }
    }
    
    aMinX = fabsf(aMinX);
    aMinY = fabsf(aMinY);
    aMaxX = fabsf(aMaxX);
    aMaxY = fabsf(aMaxY);

    float aRadiusX = 0.0f;
    float aRadiusY = 0.0f;
    
    if (aMinX > aRadiusX) { aRadiusX = aMinX; }
    if (aMaxX > aRadiusX) { aRadiusX = aMaxX; }
    
    if (aMinY > aRadiusY) { aRadiusY = aMinY; }
    if (aMaxY > aRadiusY) { aRadiusY = aMaxY; }
    
    mMaxSpawnWidth = (int)(round(aRadiusX)) + 50;
    mMaxSpawnHeight = (int)(round(aRadiusY)) + 50;
    
}

void LevelWaveBlueprint::ShiftSpawnFromIndex(int pIndex) {
    
    if (mSpawnCount >= WAVE_MAX_SPAWN_COUNT) { return; }
    if (pIndex >= (mSpawnCount)) { return; }
    if (pIndex < 0) { pIndex = 0; }
    
    //if (mSpawnCount >= WAVE_MAX_SPAWN_COUNT) { return; }
    
    for (int i=mSpawnCount;i>pIndex;i--) {
        FJSONNode *aSave = mSpawn[i-1].Save();
        mSpawn[i].Load(aSave);
        delete aSave;
    }
    
    mSpawnCount++;
}

void LevelWaveBlueprint::DeleteSpawnFromIndex(int pIndex) {
    
    if (mSpawnCount >= WAVE_MAX_SPAWN_COUNT) { return; }
    if (mSpawnCount <= 1) { return; }
    if (pIndex >= (mSpawnCount)) { return; }
    if (pIndex < 0) { pIndex = 0; }
    
    for (int i=pIndex+1;i<mSpawnCount;i++) {
        FJSONNode *aSave = mSpawn[i].Save();
        mSpawn[i-1].Load(aSave);
        delete aSave;
    }
    
    mSpawnCount--;
}

void LevelWaveBlueprint::FlipH() {
    mPath.FlipH();
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
}

void LevelWaveBlueprint::FlipV() {
    mPath.FlipV();
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->RefreshPlayback();
    }
#endif
}


//void LevelWaveBlueprint::Build() {
 
//#ifdef EDITOR_MODE
//    if (gEditor) {
        
//        gEditor->RefreshPlayback();
        //LevelWave *aWave = ((LevelWave *)gEditor->mEditorSection.mWaveList.Fetch(gEditor->WaveIndex()));
        //Build(aWave);
        
//    }
//#endif
//}

void LevelWaveBlueprint::Build(LevelWave *pWave) {
    
    if (pWave == NULL) { return; }
    
    pWave->Reset();
    
    mPath.mMaxSpawnWidth = mMaxSpawnWidth;
    mPath.mMaxSpawnHeight = mMaxSpawnHeight;
    
    mPath.Build(&pWave->mPath);
    
    //pWave->mCreationType = mCreationType;
    pWave->mCreationRequiresPrevWaveStart = mCreationRequiresPrevWaveStart;
    pWave->mCreationRequiresPrevWaveComplete = mCreationRequiresPrevWaveComplete;
    pWave->mCreationRequiresScreenWavesClear = mCreationRequiresScreenWavesClear;
    pWave->mCreationDelay = mCreationDelay;
    
    pWave->mLoopCount = mLoopCount;
    
    if (mSpawnCount < 1) mSpawnCount = 1;
    if (mSpawnCount > WAVE_MAX_SPAWN_COUNT) mSpawnCount = WAVE_MAX_SPAWN_COUNT;
    
    pWave->mSpawnSpacing = (float)mSpawnSpacing;
    
    for (int i=0;i<mSpawnCount;i++) {
        LevelWaveSpawn *aSpawn = new LevelWaveSpawn(pWave, &pWave->mPath);
        
        aSpawn->mSection = pWave->mSection;
        
        aSpawn->mOffsetSpawnDistance = mSpawn[i].mSpawnSpacingOffset;
        aSpawn->mFormationID = mSpawn[i].mFormationID.c();
        aSpawn->mObjectType = mSpawn[i].mObjectType;
        aSpawn->mShortCircuit = mSpawn[i].mShortCircuit;
        aSpawn->mShortCircuitExternal = mSpawn[i].mShortCircuitExternal;
        
        if (mSpawn[i].mMotionController.IsEmpty() == false) {
            mSpawn[i].mMotionController.Build(&(aSpawn->mMotionController));
        }
        
        mSpawn[i].mStyleController.Build(&(aSpawn->mStyleController));
        
        if (mSpawn[i].mFormationID.mLength > 0) {
            mSpawn[i].mFormationConfiguration.Build(&(aSpawn->mFormationConfiguration));
        }
        pWave->mSpawnList.Add(aSpawn);
    }
}



FJSONNode *LevelWaveBlueprint::Save() {
    
    FJSONNode *aExport = new FJSONNode();
    
    if (mSpawnSpacing != 120) { aExport->AddDictionaryInt("spawn_spacing", mSpawnSpacing); }
    
    if (mCreationRequiresPrevWaveStart == true) { aExport->AddDictionaryBool("creation_prev_wave_start", true); }
    if (mCreationRequiresPrevWaveComplete == true) { aExport->AddDictionaryBool("creation_prev_wave_complete", true); }
    if (mCreationRequiresScreenWavesClear == true) { aExport->AddDictionaryBool("creation_screen_waves_clear", true); }
    
    if (mLoopCount != 0) { aExport->AddDictionaryInt("loop_count", mLoopCount); }
    
    if (mCreationDelay != 0) { aExport->AddDictionaryInt("creation_delay", mCreationDelay); }
    if (mMaxSpawnWidth != 50) { aExport->AddDictionaryInt("max_spawn_width", mMaxSpawnWidth); }
    if (mMaxSpawnHeight != 50) { aExport->AddDictionaryInt("max_spawn_height", mMaxSpawnHeight); }
    
    if (mPath.mNodeList.mCount > 0) { aExport->AddDictionary("path", mPath.Save()); }
    
    FJSONNode *aSpawnList = new FJSONNode();
    aSpawnList->mNodeType = JSON_NODE_TYPE_ARRAY;
    for (int i=0;i<mSpawnCount;i++) {
        aSpawnList->AddArray(mSpawn[i].Save());
    }
    aExport->AddDictionary("spawn", aSpawnList);
    
    return aExport;
}

void LevelWaveBlueprint::Load(FJSONNode *pNode) {
    Reset();
    if (pNode == NULL) { return; }
    
    mSpawnSpacing = pNode->GetInt("spawn_spacing", 90);
    //mCreationType = pNode->GetInt("creation_type", WAVE_CREATION_TYPE_SCREEN_CLEAR_IGNORE_PERMS);
    
    
    
    mCreationRequiresPrevWaveStart = pNode->GetBool("creation_prev_wave_start", false);
    mCreationRequiresPrevWaveComplete = pNode->GetBool("creation_prev_wave_complete", false);
    mCreationRequiresScreenWavesClear = pNode->GetBool("creation_screen_waves_clear", false);
    
    mLoopCount = pNode->GetInt("loop_count", 0);
    mCreationDelay = pNode->GetInt("creation_delay", 0);
    
    mMaxSpawnWidth = pNode->GetInt("max_spawn_width", 50);
    mMaxSpawnHeight = pNode->GetInt("max_spawn_height", 50);
    
    mSpawnCount = 0;
    FJSONNode *aSpawnList = pNode->GetArray("spawn");
    if (aSpawnList != NULL) {
        EnumJSONArray(aSpawnList, aSpawnNode) {
            mSpawn[mSpawnCount].Load(aSpawnNode);
            ++mSpawnCount;
        }
    }
    
    if (mSpawnCount <= 0) { mSpawnCount = 1; }
    if (mSpawnCount > WAVE_MAX_SPAWN_COUNT) { mSpawnCount = WAVE_MAX_SPAWN_COUNT; }
    
    FJSONNode *aPathNode = pNode->GetDictionary("path");
    mPath.Load(aPathNode);
}
