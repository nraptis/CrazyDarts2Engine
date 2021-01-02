//
//  LevelWaveFormation.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/15/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelFormation.hpp"
#include "LevelFormationTracer.hpp"
#include "LevelWaveSpawn.hpp"
#include "LevelPermSpawn.hpp"
#include "Game.hpp"
#include "core_includes.h"

LevelFormation::LevelFormation() {
    
    mRotation = 0.0f;
    
    mX = 0.0f;
    mY = 0.0f;
    
    mDidOriginateOnWave = false;
    mDidOriginateAsPermanent = false;
    
    mIsPlayingEnter = false;
    mIsPlayingExit = false;
    
    mDidSpawn = false;
    
    mSection = NULL;
    
}

LevelFormation::~LevelFormation() {
    FreeList(LevelFormationNode, mSpawnNodeList);
    FreeList(LevelFormationNode, mNodeKillList);
    
    FreeList(LevelFormationTracer, mTracerList);
    FreeList(LevelFormationTracer, mTracerKillList);
}

void LevelFormation::DisposeObject(GameObject *pObject) {
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->DisposeObject(pObject);
    }
    for (int i=0;i<mTracerList.mCount;i++) {
        LevelFormationTracer *aTracer = ((LevelFormationTracer *)mTracerList.mData[i]);
        aTracer->DisposeObject(pObject);
    }
}

bool LevelFormation::ContainsObject(GameObject *pObject) {
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        if (aNode->ContainsObject(pObject) == true) {
            return true;
        }
    }
    for (int i=0;i<mTracerList.mCount;i++) {
        LevelFormationTracer *aTracer = ((LevelFormationTracer *)mTracerList.mData[i]);
        if (aTracer->ContainsObject(pObject) == true) {
            return true;
        }
    }
    return false;
}

void LevelFormation::DisposeAllObjects() {
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->DisposeAllObjects();
    }
    for (int i=0;i<mTracerList.mCount;i++) {
        LevelFormationTracer *aTracer = ((LevelFormationTracer *)mTracerList.mData[i]);
        aTracer->DisposeAllObjects();
    }
}

void LevelFormation::Reset() {
    
    mDidSpawn = false;
    
    for (int i=0;i<mSpawnNodeList.mCount;i++) {
        LevelFormationNode *aNode = ((LevelFormationNode *)mSpawnNodeList.mData[i]);
        aNode->Reset();
        mNodeKillList.Add(aNode);
    }
    mSpawnNodeList.RemoveAll();
    
    for (int i=0;i<mTracerList.mCount;i++) {
        LevelFormationTracer *aTracer = ((LevelFormationTracer *)mTracerList.mData[i]);
        aTracer->Reset();
        mTracerKillList.Add(aTracer);
    }
    mTracerList.RemoveAll();
}

void LevelFormation::Spawn(LevelMotionController *pMotionController, LevelFormationConfiguration *pConfiguration) {
    
    if (pConfiguration != NULL) {
        pConfiguration->Apply(this);
    }
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->mFormation = this;
        aNode->RefreshSection(mSection);
        aNode->mDidOriginateOnWave = mDidOriginateOnWave;
        aNode->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        aNode->Spawn();
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aTracer->mFormation = this;
        aTracer->RefreshSection(mSection);
        aTracer->mDidOriginateOnWave = mDidOriginateOnWave;
        aTracer->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        aTracer->Spawn();
    }
    
    ApplyMotionController(pMotionController);
    
    mDidSpawn = true;
}

void LevelFormation::EditorPickerSpawn() {
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        //aNode->mFormation = this;
        //aNode->mDidOriginateOnWave = mDidOriginateOnWave;
        //aNode->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        
        
        aNode->EditorPickerSpawn();
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        //aTracer->mFormation = this;
        //aTracer->RefreshSection(mSection);
        //aTracer->mDidOriginateOnWave = mDidOriginateOnWave;
        //aTracer->mDidOriginateAsPermanent = mDidOriginateAsPermanent;
        aTracer->EditorPickerSpawn();
    }
    
    mDidSpawn = true;
}

void LevelFormation::Update(LevelMotionController *pMotionController) {
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->Update();
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        
        if ((mIsPlayingEnter == true) && (aTracer->mPlayOnEnter == false)) {
            aTracer->mStopped = true;
        } else if ((mIsPlayingExit == true) && (aTracer->mPlayOnExit == false)) {
            aTracer->mStopped = true;
        } else {
            aTracer->mStopped = false;
        }
        
        aTracer->Update();
    }
    
    ApplyMotionController(pMotionController);
    
    EnumList(LevelFormationNode, aNode, mNodeKillList) {
        aNode->mKillTimer--;
        if (aNode->mKillTimer <= 0) { mNodeDeleteList.Add(aNode); }
    }
    EnumList(LevelFormationNode, aNode, mNodeDeleteList) {
        mNodeKillList.Remove(aNode);
        delete aNode;
    }
    mNodeDeleteList.RemoveAll();
    
    
    EnumList(LevelFormationTracer, aTracer, mTracerKillList) {
        aTracer->mKillTimer--;
        if (aTracer->mKillTimer <= 0) { mTracerDeleteList.Add(aTracer); }
    }
    EnumList(LevelFormationTracer, aTracer, mTracerDeleteList) {
        mTracerKillList.Remove(aTracer);
        delete aTracer;
    }
    mTracerDeleteList.RemoveAll();
}

void LevelFormation::Draw(int pSelectedIndex) {
    
    Graphics::PipelineStateSetShape2DAlphaBlending();
    
    int aIndex = 0;
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        
        if (pSelectedIndex == aIndex) {
            Graphics::SetColor(1.0f, 0.65f, 0.25f, 0.75f);
        } else {
            Graphics::SetColor(0.65f, 0.65f, 1.0f, 0.25f);
        }
        
        for (int i=0;i<aTracer->mPath.mCount;i++) {
            
            float aX = aTracer->mPath.mX[i];
            float aY = aTracer->mPath.mY[i];
            
            if (mRotation != 0.0f) {
                FVec2 aPoint;
                aPoint.mX = aX; aPoint.mY = aY;
                aPoint = PivotPoint(aPoint, mRotation);
                aX = aPoint.mX; aY = aPoint.mY;
            }
            
            aX += mX;
            aY += mY;
            
            if (pSelectedIndex == aIndex) {
                Graphics::DrawPoint(aX - 2.0f, aY - 2.0f, 5.0f);
            } else {
                Graphics::DrawPoint(aX - 1.0f, aY - 1.0f, 3.0f);
            }
        }
        ++aIndex;
    }
}

void LevelFormation::DrawDeadObjectMarkers() {
    
    if (mDidSpawn == true) {
    
        EnumList(LevelFormationTracer, aTracer, mTracerList) {
            aTracer->DrawDeadObjectMarkers();
        }
        
        EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
            aNode->DrawDeadObjectMarkers();
        }
    }
}

LevelFormation *LevelFormation::Clone() {
    LevelFormation *aClone = new LevelFormation();
    
    aClone->mSection = mSection;
    
    aClone->mX = mX;
    aClone->mY = mY;
    
    aClone->mID = mID;
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        
        aClone->mSpawnNodeList.Add(aNode->Clone(aClone));
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aClone->mTracerList.Add(aTracer->Clone(aClone));
    }
    
    return aClone;
}

bool LevelFormation::IsClear() {
    
    if (mDidSpawn == false) {
        return false;
    }
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL && aNode->mObject->mKill == 0) {
            return false;
        }
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObject != NULL && aNode->mObject->mKill == 0) {
                return false;
            }
        }
    }
    
    return true;
}

void LevelFormation::ApplyMotionController(LevelMotionController *pMotionController) {
    if (pMotionController != NULL) {
        EnumList(LevelFormationTracer, aTracer, mTracerList) {
            EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
                
                aNode->mObjectX = aNode->mX;
                aNode->mObjectY = aNode->mY;
                aNode->mObjectRotation = pMotionController->ApplyRotation();
                
                if (aNode->mObjectType == GAME_OBJECT_TYPE_CANNON) {
                    aNode->mObjectRotation = pMotionController->ApplyRotationCannon();
                }
                
                if (aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL ||
                    aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM ||
                    aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE ||
                    aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE) {
                    aNode->mObjectRotation = pMotionController->ApplyRotationPlatform();
                }
                
                pMotionController->Apply(mX, mY, &(aNode->mObjectX), &(aNode->mObjectY));
                
                aNode->mObjectRotation += aNode->mObjectRotationOffset;
                
                
                if (aNode->mObject != NULL) {
                    aNode->mObject->mTransform.mX = aNode->mObjectX;
                    aNode->mObject->mTransform.mY = aNode->mObjectY;
                    aNode->mObject->mTransform.mRotation = aNode->mObjectRotation;
                }
            }
        }
        EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
            //pMotionController->Apply(mX, mY, aNode->mObject);
            
            aNode->mObjectX = aNode->mX;
            aNode->mObjectY = aNode->mY;
                
            aNode->mObjectRotation = pMotionController->ApplyRotation();
            
            if (aNode->mObjectType == GAME_OBJECT_TYPE_CANNON) {
                aNode->mObjectRotation = pMotionController->ApplyRotationCannon();
            }
            
            if (aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL ||
                aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM ||
                aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE ||
                aNode->mObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE) {
                aNode->mObjectRotation = pMotionController->ApplyRotationPlatform();
            }
            
            pMotionController->Apply(mX, mY, &(aNode->mObjectX), &(aNode->mObjectY));
            
            
            aNode->mObjectRotation += aNode->mObjectRotationOffset;
            
            if (aNode->mObject != NULL) {
                aNode->mObject->mTransform.mX = aNode->mObjectX;
                aNode->mObject->mTransform.mY = aNode->mObjectY;
                aNode->mObject->mTransform.mRotation = aNode->mObjectRotation;
            }
        }
    }
}

FList cStyleControllerFormationApplyList;
void LevelFormation::ApplyStyleController(LevelStyleController *pStyleController) {
    
    if (pStyleController == NULL) { return; }
    
    cStyleControllerFormationApplyList.RemoveAll();
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObject != NULL) {
                cStyleControllerFormationApplyList.Add(aNode->mObject);
            }
        }
    }
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL) {
            cStyleControllerFormationApplyList.Add(aNode->mObject);
        }
    }
    pStyleController->ApplyToObjectList(&cStyleControllerFormationApplyList);
}

void LevelFormation::HandOffAllGameObjects(FList *pList) {
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aTracer->HandOffAllGameObjects(pList);
    }
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL && aNode->mObject->mKill == 0 && pList != NULL) {
            pList->Add(aNode->mObject);
        }
        aNode->mObject = NULL;
    }
}

void LevelFormation::SetWaveSpawn(LevelWaveSpawn *pSpawn) {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL && aNode->mObject->mKill == 0) {
            aNode->mObject->mWaveSpawn = pSpawn;
        }
    }
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObject != NULL && aNode->mObject->mKill == 0) {
                aNode->mObject->mWaveSpawn = pSpawn;
            }
        }
    }
}

void LevelFormation::SetPermSpawn(LevelPermSpawn *pSpawn) {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL && aNode->mObject->mKill == 0) {
            aNode->mObject->mPermSpawn = pSpawn;
        }
    }
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObject != NULL && aNode->mObject->mKill == 0) {
                aNode->mObject->mPermSpawn = pSpawn;
            }
        }
    }
}

void LevelFormation::GetExtremeties(float &pTop, float &pRight, float &pBottom, float &pLeft) {
    
    pTop = 0.0f;
    pRight = 0.0f;
    pLeft = 0.0f;
    pBottom = 0.0f;
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mX < pLeft) { pLeft = aNode->mX; }
        if (aNode->mX > pRight) { pRight = aNode->mX; }
        if (aNode->mY < pTop) { pTop = aNode->mY; }
        if (aNode->mY > pBottom) { pBottom = aNode->mY; }
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aTracer->GetExtremeties(pTop, pRight, pBottom, pLeft);
    }
}

bool LevelFormation::HasPauseExitType() {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (GameObject::IsPauseExitType(aNode->mObjectType) == true) {
            return true;
        }
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        if (aTracer->HasPauseExitType()) {
            return true;
        }
    }
    
    return false;
}

void LevelFormation::RefreshSection(LevelSection *pSection) {
    mSection = pSection;
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->RefreshSection(pSection);
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aTracer->RefreshSection(pSection);
    }
}

void LevelFormation::EditorKillAllObjects() {
    
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
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aTracer->EditorKillAllObjects();
    }
    
}

bool LevelFormation::IsClearForSectionCompletion() {
    
    if (mDidSpawn == false) { return false; }
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL) {
            if (aNode->mObject->IsRequiredToClearForSectionCompletion() == true) {
                return false;
            }
        }
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObject != NULL) {
                if (aNode->mObject->IsRequiredToClearForSectionCompletion() == true) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool LevelFormation::HasAnyObjects() {
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        if (aNode->mObject != NULL) {
            return true;
        }
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObject != NULL) {
                return true;
            }
        }
    }
    
    return false;
}

bool LevelFormation::EditorHasAnyTracers() {
    if (mTracerList.mCount > 0) { return true; }
    return false;
}

bool LevelFormation::EditorHasNonTracers() {
    if (mSpawnNodeList.mCount > 0) { return true; }
    return false;
}

bool LevelFormation::EditorHasMixedTypes() {
    
    unordered_set<int> aTypeSet;
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aTypeSet.insert(aNode->mObjectType);
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            aTypeSet.insert(aNode->mObjectType);
        }
    }
    return (aTypeSet.size() > 1);
}

bool LevelFormation::EditorHasBalloonsOnly() {
    unordered_set<int> aTypeSet;
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aTypeSet.insert(aNode->mObjectType);
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            aTypeSet.insert(aNode->mObjectType);
        }
    }
    
    if (aTypeSet.find(GAME_OBJECT_TYPE_BALLOON) != aTypeSet.end()) {
        if (aTypeSet.size() == 1) {
            return true;
        }
    }
    
    return false;
}

bool LevelFormation::EditorHasType(int pType) {
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        
        if (aNode->mObjectType == pType) {
            return true;
        }
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            if (aNode->mObjectType == pType) {
                return true;
            }
        }
    }
    
    return false;
}

int LevelFormation::CountObjects() {
    
    int aResult = 0;
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        ++aResult;
    }
    
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        EnumList(LevelFormationNode, aNode, aTracer->mSpawnNodeList) {
            ++aResult;
        }
    }
    return aResult;
}

void LevelFormation::EditorDisposeAll() {
    
    EnumList(LevelFormationNode, aNode, mSpawnNodeList) {
        aNode->EditorDisposeAll();
    }
    EnumList(LevelFormationTracer, aTracer, mTracerList) {
        aTracer->EditorDisposeAll();
    }
    
    
    
    
}
