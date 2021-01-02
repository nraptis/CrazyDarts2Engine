//
//  LevelFormationBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/31/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelFormationBlueprint.hpp"
#include "os_core_graphics.h"
#include "FSpline.hpp"
#include "FPolyPath.hpp"
#include "core_includes.h"
#include "FormationCollection.hpp"
#include "FLine.hpp"
#include "LevelPath.hpp"
#include "Game.hpp"

#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif


LevelFormationNodeBlueprint::LevelFormationNodeBlueprint() {
    mEditorX = 0.0f;
    mEditorY = 0.0f;
    
    mPercentX = 0.0f;
    mPercentY = 0.0f;
    
    mGameX = 0.0f;
    mGameY = 0.0f;
    
    mObjectType = GAME_OBJECT_TYPE_BALLOON;
    
    mObjectRotate = false;
    mObjectRotationOffset = 0;
    
    mKillTimer = 8;
}

LevelFormationNodeBlueprint::~LevelFormationNodeBlueprint() {
    
}

FJSONNode *LevelFormationNodeBlueprint::Save(bool pFromTracer) {
    FJSONNode *aExport = new FJSONNode();
    aExport->mNodeType = JSON_NODE_TYPE_DICTIONARY;
    
#ifdef EDITOR_MODE
    
    //for tracer, we don't need the position...
    if (pFromTracer == false) {
        float aLeft = gEditor->mGameAreaLeft;
        float aRight = gEditor->mGameAreaRight;
        
        //100x percision
        float aX = (mEditorX - aLeft) / (aRight - aLeft) * 100.0f - 50.0f;
        aExport->AddDictionaryFloat("x", aX);
        
        
        float aTop = gEditor->mGameAreaTop;
        float aBottom = gEditor->mGameAreaBottom;
        //100x percision
        float aY = (mEditorY - aTop) / (aBottom - aTop) * 100.0f - 50.0f;
        aExport->AddDictionaryFloat("y", aY);
    }
    
    
    if (mObjectRotate == true) {
        aExport->AddDictionaryBool("rotate_object", mObjectRotate);
    }
    
    if (mObjectRotationOffset != 0) {
        aExport->AddDictionaryInt("rotate_object_offset", mObjectRotationOffset);
    }
    
    
    if (mObjectType != GAME_OBJECT_TYPE_BALLOON) {
        aExport->AddDictionaryInt("type", mObjectType);
    }
    
#endif
    
    return aExport;
}

void LevelFormationNodeBlueprint::Load(FJSONNode *pNode) {
    
    if (pNode == NULL) { return; }
    
    mPercentX = 0.0f;
    mPercentX = pNode->GetFloat("x", 0.0f);
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        float aLeft = gEditor->mGameAreaLeft;
        float aRight = gEditor->mGameAreaRight;
        mEditorX = aLeft + (aRight - aLeft) * ((mPercentX + 50.0f) / 100.0f);
    }
#endif
    
    mPercentY = 0.0f;
    mPercentY = pNode->GetFloat("y", 0.0f);
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        float aTop = gEditor->mGameAreaTop;
        float aBottom = gEditor->mGameAreaBottom;
        mEditorY = aTop + (aBottom - aTop) * ((mPercentY + 50.0f) / 100.0f);
    }
#endif
    
    mObjectType = pNode->GetInt("type", GAME_OBJECT_TYPE_BALLOON);
    
    mObjectRotate = pNode->GetBool("rotate_object", false);
    mObjectRotationOffset = pNode->GetInt("rotate_object_offset", 0);
}

LevelFormationBlueprint::LevelFormationBlueprint() {
    
    mRotation = 0.0f;
    
    mRotationEnabled = false;
    
    mRotationSpeedClass = SPEED_CLASS_MEDIUM;
    
    mRotationSpeedNegateAlways = false;
    mRotationSpeedNegateRandom = true;
    
    
    mSelectedNodeIndex = -1;
    mCurrentTracerIndex = 0;
    
}

LevelFormationBlueprint::~LevelFormationBlueprint() {
    FreeList(LevelFormationNodeBlueprint, mNodeList);
    FreeList(LevelFormationNodeBlueprint, mKillNodeList);
}

void LevelFormationBlueprint::Update() {
    
    
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        mTracer[i].Update();
    }
    
    
    RefreshNodePositions();
    
    EnumList(LevelFormationNodeBlueprint, aNode, mKillNodeList) {
        aNode->mKillTimer--;
        if (aNode->mKillTimer <= 0) { mDeleteNodeList.Add(aNode); }
    }
    EnumList(LevelFormationNodeBlueprint, aNode, mDeleteNodeList) {
        mKillNodeList.Remove(aNode);
        delete aNode;
    }
    mDeleteNodeList.RemoveAll();
    
    //
}

void LevelFormationBlueprint::Reset() {
    
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        mTracer[i].Reset();
    }
    
    EnumList(LevelFormationNodeBlueprint, aNode, mNodeList) {
        mKillNodeList.Add(aNode);
    }
    mNodeList.RemoveAll();
    
    
    mSelectedNodeIndex = -1;
    
#ifdef EDITOR_MODE
    if (gFormationEditor != NULL) {
        RefreshNodePositions();
    //    if (gFormationEditor != NULL) { gFormationEditor->Refresh(); }
        
    }
#endif
    
}

void LevelFormationBlueprint::Draw(bool pSelected) {
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
        if (pSelected) {
            if (i == mSelectedNodeIndex) {
                Graphics::SetColor(1.0f, 0.0f, 0.0f, 0.75f);
                Graphics::DrawPoint(aNode->mEditorX, aNode->mEditorY, 8.0f);
            } else {
                Graphics::SetColor(1.0f, 0.0f, 0.0f, 0.75f);
                Graphics::DrawPoint(aNode->mEditorX, aNode->mEditorY, 2.0f);
            }
        } else {
            Graphics::SetColor(0.65f, 0.65f, 0.65f, 0.35f);
            Graphics::DrawPoint(aNode->mEditorX, aNode->mEditorY, 4.0f);
        }
    }
    
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        mTracer[i].Draw(mCurrentTracerIndex == i);
    }
}

int LevelFormationBlueprint::CountProgressObjects(const char *pFormationID) {
    LevelFormationBlueprint *aBlueprint = LevelFormationBlueprint::GetFormation(pFormationID);
    int aResult = 0;
    if (aBlueprint != NULL) {
        aResult = aBlueprint->CountProgressObjects();
    }
    return aResult;
}

int LevelFormationBlueprint::CountProgressObjects() {
    
    int aResult = 0;
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        if (mTracer[i].IsValid()) {
            aResult += mTracer[i].CountProgressObjects();
        }
    }
    
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelFormationNodeBlueprint *aNodeBlueprint = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
        if (gGame->DoesObjectTypeCountTowardsProgressCount(aNodeBlueprint->mObjectType)) {
            aResult += 1;
        }
    }
    return aResult;
}

int LevelFormationBlueprint::CountObjectsOfType(const char *pFormationID, int pGameObjectType) {
    LevelFormationBlueprint *aBlueprint = LevelFormationBlueprint::GetFormation(pFormationID);
    int aResult = 0;
    if (aBlueprint != NULL) {
        aResult = aBlueprint->CountObjectsOfType(pGameObjectType);
    }
    return aResult;
}

int LevelFormationBlueprint::CountObjectsOfType(int pGameObjectType) {
    
    int aResult = 0;
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        if (mTracer[i].IsValid()) {
            aResult += mTracer[i].CountObjectsOfType(pGameObjectType);
        }
    }
    
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelFormationNodeBlueprint *aNodeBlueprint = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
        if (pGameObjectType == aNodeBlueprint->mObjectType) {
            aResult += 1;
        }
    }
    return aResult;
}

LevelFormationBlueprint *LevelFormationBlueprint::GetFormation(const char *pFormationID) {
    LevelFormationBlueprint *aResult = gFormationCollection.GetBlueprint(pFormationID);
    
    if (aResult == NULL) {
        Log("*** FAILOVER FORMATION => [%s] ***\n\n", pFormationID);
        
        //exit(0);
        
        FJSON aJSON;
        aJSON.Load(pFormationID);
        if (aJSON.mRoot != NULL) {
            FString aDummy;
            aResult = new LevelFormationBlueprint();
            aResult->Load(aJSON.mRoot, aDummy);
        }
    }
    
    return aResult;
}


float LevelFormationBlueprint::GetX(int pIndex) {
    LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.Fetch(pIndex);
    if (aNode != NULL) { return aNode->mEditorX; }
    return 0.0f;
}

float LevelFormationBlueprint::GetY(int pIndex) {
    LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.Fetch(pIndex);
    if (aNode != NULL) { return aNode->mEditorY; }
    return 0.0f;
}

void LevelFormationBlueprint::Set(int pIndex, float pX, float pY) {
    LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.Fetch(pIndex);
    if (aNode != NULL) {
        aNode->mEditorX = pX;
        aNode->mEditorY = pY;
        RefreshNodePositions();
#ifdef EDITOR_MODE
        if (gFormationEditor != NULL) { gFormationEditor->Refresh(); }
#endif
    }
}

void LevelFormationBlueprint::Add(float pX, float pY) {
    LevelFormationNodeBlueprint *aNode = new LevelFormationNodeBlueprint();
    aNode->mEditorX = pX;
    aNode->mEditorY = pY;
    mNodeList.Add(aNode);
    mSelectedNodeIndex = mNodeList.mCount - 1;
    RefreshNodePositions();
#ifdef EDITOR_MODE
    if (gFormationEditor != NULL) { gFormationEditor->Refresh(); }
#endif
}

void LevelFormationBlueprint::Remove(int pIndex) {
    LevelFormationNodeBlueprint *aDeleteNode = (LevelFormationNodeBlueprint *)mNodeList.Fetch(pIndex);
    if (aDeleteNode) {
        if (mSelectedNodeIndex >= pIndex) { mSelectedNodeIndex--; }
        mKillNodeList.Add(aDeleteNode);
        mNodeList.RemoveAtIndex(pIndex);
    }
#ifdef EDITOR_MODE
    if (gFormationEditor != NULL) { gFormationEditor->Refresh(); }
#endif
}

LevelFormationNodeBlueprint *LevelFormationBlueprint::GetNode(int pIndex) {
    LevelFormationNodeBlueprint *aResult = (LevelFormationNodeBlueprint *)mNodeList.Fetch(pIndex);
    return aResult;
}

LevelFormationNodeBlueprint *LevelFormationBlueprint::GetNode() {
    LevelFormationNodeBlueprint *aResult = (LevelFormationNodeBlueprint *)mNodeList.Fetch(mSelectedNodeIndex);
    return aResult;
}

void LevelFormationBlueprint::TracerAdd(float pX, float pY) {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        aTracer->Add(pX, pY);
    }
}

void LevelFormationBlueprint::TracerRemovePoint() {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        aTracer->Remove(aTracer->mSelectedTracerNodeIndex);
    }
}

void LevelFormationBlueprint::TracerNuke() {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        aTracer->Nuke();
    }
#ifdef EDITOR_MODE
    if (gFormationEditor != NULL) { gFormationEditor->Refresh(); }
#endif
}

void LevelFormationBlueprint::TracerSet(int pIndex, float pX, float pY) {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        aTracer->Set(pIndex, pX, pY);
    }
}

float LevelFormationBlueprint::TracerGetX(int pIndex) {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        return aTracer->GetX(pIndex);
    }
    return 0.0f;
}

float LevelFormationBlueprint::TracerGetY(int pIndex) {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        return aTracer->GetY(pIndex);
    }
    return 0.0f;
}

LevelFormationTracerBlueprint *LevelFormationBlueprint::GetTracer() {
    if (mCurrentTracerIndex >= 0 && mCurrentTracerIndex < BLUEPRINT_TRACER_COUNT) {
        return &(mTracer[mCurrentTracerIndex]);
    }
    return NULL;
}

LevelFormationTracerNodeBlueprint *LevelFormationBlueprint::TracerGetNode(int pIndex) {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        return aTracer->GetNode(pIndex);
    }
    return NULL;
}

LevelFormationTracerNodeBlueprint *LevelFormationBlueprint::TracerGetNode() {
    LevelFormationTracerBlueprint *aTracer = GetTracer();
    if (aTracer != NULL) {
        return aTracer->GetNode();
    }
    return NULL;
}

void LevelFormationBlueprint::Print() {
    FJSON aJSON;
    aJSON.mRoot = Save();
    aJSON.Print();
}
      
void LevelFormationBlueprint::RefreshNodePositions() {
    
#ifdef EDITOR_MODE
    float aLeft = gEditor->mGameAreaLeft;
    float aRight = gEditor->mGameAreaRight;
    float aTop = gEditor->mGameAreaTop;
    float aBottom = gEditor->mGameAreaBottom;
    
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
        aNode->mPercentX = ((aNode->mEditorX - aLeft) / (aRight - aLeft)) * 100.0f - 50.0f;
        aNode->mPercentY = ((aNode->mEditorY - aTop) / (aBottom - aTop)) * 100.0f - 50.0f;
    }
    
#endif
}

int LevelFormationBlueprint::GetClosestIndex(float pX, float pY, float &pDist) {
    int aResult = -1;
    if (mNodeList.mCount > 0) {
        LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.First();
        
        aResult = 0;
        float aCheckDist = pDist;
        float aDiffX = aNode->mEditorX - pX;
        float aDiffY = aNode->mEditorY - pY;
        float aDist = aDiffX * aDiffX + aDiffY * aDiffY;
        pDist = aDist;
        for (int i=1;i<mNodeList.mCount;i++) {
            aNode = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
            aDiffX = aNode->mEditorX - pX;
            aDiffY = aNode->mEditorY - pY;
            aDist = aDiffX * aDiffX + aDiffY * aDiffY;
            if (aDist < pDist) {
                pDist = aDist;
                aResult = i;
            }
        }
        if (pDist > aCheckDist) { aResult = -1; }
    }
    
    return aResult;
}

void LevelFormationBlueprint::Build() {
    
#ifdef EDITOR_MODE
    if (gFormationEditor != NULL) {
        Build(&(gFormationEditor->mEditorFormation));
    }
#endif
    
}

void LevelFormationBlueprint::Build(LevelFormation *pFormation) {
    if (pFormation == NULL) { return; }
    
    pFormation->Reset();
    pFormation->mRotation = mRotation;
    
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelFormationNodeBlueprint *aNodeBlueprint = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
        LevelFormationNode *aNode = new LevelFormationNode();
        aNode->mSection = pFormation->mSection;
        aNode->mFormation = pFormation;
        aNode->mBaseX = aNodeBlueprint->mPercentX;
        aNode->mBaseY = aNodeBlueprint->mPercentY;
        aNode->mObjectType = aNodeBlueprint->mObjectType;
        
        aNode->mObjectRotate = aNodeBlueprint->mObjectRotate;
        aNode->mObjectRotationOffset = ((float)aNodeBlueprint->mObjectRotationOffset);
        
        pFormation->mSpawnNodeList.Add(aNode);
    }
    
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        if (mTracer[i].IsValid()) {
            LevelFormationTracer *aTracer = new LevelFormationTracer(pFormation);
            aTracer->mSection = pFormation->mSection;
            mTracer[i].Build(aTracer);
            pFormation->mTracerList.Add(aTracer);
        }
    }
    
}

FJSONNode *LevelFormationBlueprint::Save() {
    
    FJSONNode *aExport = new FJSONNode();
    aExport->mNodeType = JSON_NODE_TYPE_DICTIONARY;
    
    if (mRotation != 0) {
        aExport->AddDictionaryFloat("rotation", mRotation);
    }
    
    FJSONNode *aNodeListNode = new FJSONNode();
    aNodeListNode->mNodeType = JSON_NODE_TYPE_ARRAY;
    for (int i=0;i<mNodeList.mCount;i++) {
        LevelFormationNodeBlueprint *aNode = (LevelFormationNodeBlueprint *)mNodeList.mData[i];
        aNodeListNode->AddArray(aNode->Save(false));
    }
    aExport->AddDictionary("node_list", aNodeListNode);
    
    
    bool aSaveTracers = false;
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        if (mTracer[i].IsValid()) {
            aSaveTracers = true;
        }
    }
    if (aSaveTracers) {
        FJSONNode *aTracerListNode = new FJSONNode();
        aTracerListNode->mNodeType = JSON_NODE_TYPE_ARRAY;
        for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
            
            if (mTracer[i].IsValid()) {
                aTracerListNode->AddArray(mTracer[i].Save());
            }
            
        }
        aExport->AddDictionary("tracer_list", aTracerListNode);
    }
    
    
    
    return aExport;
}

void LevelFormationBlueprint::Load(FJSONNode *pNode, FString pFilePath) {
    Reset();
    
    mName = pFilePath;
    mName.RemovePathAndExtension();
    
    
    if (pNode == NULL) { return; }
    
    mRotation = pNode->GetFloat("rotation", mRotation);
    
    
    FJSONNode *aNodeListNode = pNode->GetArray("node_list");
    if (aNodeListNode != NULL) {
        EnumJSONArray(aNodeListNode, aFormationBlueprintLoadNode) {
            LevelFormationNodeBlueprint *aNode = new LevelFormationNodeBlueprint();
            aNode->Load(aFormationBlueprintLoadNode);
            mNodeList.Add(aNode);
        }
    }
    
    
    FJSONNode *aTracerListNode = pNode->GetArray("tracer_list");
    if (aTracerListNode != NULL) {
        int aIndex = 0;
        EnumJSONArray(aTracerListNode, aTracerBlueprintLoadNode) {
            mTracer[aIndex].Load(aTracerBlueprintLoadNode);
            ++aIndex;
            if (aIndex == BLUEPRINT_TRACER_COUNT) { break; }
        }
    }
}

bool LevelFormationBlueprint::IsValid() {
    if (mNodeList.mCount > 0) {
        return true;
    }
    for (int i=0;i<BLUEPRINT_TRACER_COUNT;i++) {
        if (mTracer[i].IsValid() == true) {
            return true;
        }
    }
    return false;
}

