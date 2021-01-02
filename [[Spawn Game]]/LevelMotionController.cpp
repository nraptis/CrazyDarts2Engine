//
//  LevelMotionController.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/13/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelMotionController.hpp"
#include "LevelSection.hpp"
#include "Game.hpp"
#include "Balloon.hpp"

LevelMotionController::LevelMotionController() {
    
    mPlayOnEnter = true;
    mPlayOnExit = true;
    
    mSkipUpdate = false;
    
    mKillTimer = 8;
}

LevelMotionController::~LevelMotionController() {
    FreeList(LevelMotionControllerSlice, mSliceList);
    FreeList(LevelMotionControllerSlice, mKillList);
}

bool LevelMotionController::ShouldPauseExit() {
    
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        
        //We OR together all the slices...
        if (aSlice->ShouldPauseExit() == true) {
            return true;
        }
    }
    
    return false;
}

void LevelMotionController::BeginPauseExit() {
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        aSlice->BeginPauseExit();
    }
}

bool LevelMotionController::IsPauseExitComplete() {
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        if (aSlice->IsPauseExitComplete() == false) {
            return false;
        }
    }
    return true;
}

void LevelMotionController::Reset() {
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        mKillList.Add(aSlice);
    }
    mSliceList.RemoveAll();
}

void LevelMotionController::Update() {
    
    if (mSkipUpdate == false) {
        EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
            aSlice->Update();
        }
    }
    
    EnumList(LevelMotionControllerSlice, aSlice, mKillList) {
        aSlice->mKillTimer--;
        if (aSlice->mKillTimer <= 0) { mDeleteList.Add(aSlice); }
    }
    EnumList(LevelMotionControllerSlice, aSlice, mDeleteList) {
        mKillList.Remove(aSlice);
        delete aSlice;
    }
    mDeleteList.RemoveAll();
}

void LevelMotionController::Draw() {
    
}

float LevelMotionController::ApplyRotation() {
    float aRotation = 0.0f;
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        if (aSlice->mRotatesObject == true) {
            aRotation = aSlice->ApplyToObjectRotation();
        }
    }
    return aRotation;
}

float LevelMotionController::ApplyRotationCannon() {
    float aRotation = 0.0f;
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        if ((aSlice->mRotatesCannons == true) || (aSlice->mRotatesObject == true)) {
            aRotation = aSlice->ApplyToObjectRotation();
        }
    }
    return aRotation;
}

float LevelMotionController::ApplyRotationPlatform() {
    float aRotation = 0.0f;
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        if ((aSlice->mRotatesPlatforms == true) || (aSlice->mRotatesObject == true)) {
            aRotation = aSlice->ApplyToObjectRotation();
        }
    }
    return aRotation;
}

//float LevelMotionController::ApplyRotationForced() {
//    float aRotation = 0.0f;
//    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
//        aRotation += aSlice->ApplyToObjectRotation();
//    }
//    return aRotation;
//}

void LevelMotionController::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    EnumList(LevelMotionControllerSlice, aSlice, mSliceList) {
        aSlice->Apply(pReferenceX, pReferenceY, pX, pY);
    }
}



