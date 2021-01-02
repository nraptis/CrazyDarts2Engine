//
//  LevelMotionControllerSlice.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/19/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelMotionControllerSlice.hpp"
#include "LevelPath.hpp"
#include "FMath.hpp"
#include "GFXApp.hpp"

LevelMotionControllerSlice::LevelMotionControllerSlice() {
    mType = LEVEL_MOTION_SLICE_TYPE_NONE;
    mSpeedClass = SPEED_CLASS_MEDIUM;
    
    mSpeedNegateAlways = false;
    mSpeedNegateRandomly = false;
    mRotatesObject = false;
    mRotatesCannons = false;
    mRotatesPlatforms = false;
    
    mPauseExit = false;
    
    mSpeedNegateRandomlyFlag = gRand.GetBool();
    
    mKillTimer = 8;
}

LevelMotionControllerSlice::~LevelMotionControllerSlice() {
    
}

bool LevelMotionControllerSlice::ShouldPauseExit() {
    return false;
}

void LevelMotionControllerSlice::BeginPauseExit() {
    mPauseExit = true;
}

bool LevelMotionControllerSlice::IsPauseExitComplete() {
    return true;
}


LevelMotionControllerSliceNegate::LevelMotionControllerSliceNegate() {
    mType = LEVEL_MOTION_SLICE_TYPE_NEGATE;
    
    mNegateHAlways = false;
    mNegateHRandomly = true;
    mNegateHRandomlyFlag = gRand.GetBool();
    
    mNegateVAlways = false;
    mNegateVRandomly = true;
    mNegateVRandomlyFlag = gRand.GetBool();
}

LevelMotionControllerSliceNegate::~LevelMotionControllerSliceNegate() {
    
}

void LevelMotionControllerSliceNegate::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    if (pX != NULL && pY != NULL) {
        if ((mNegateHAlways == true) || (mNegateHRandomly == true && mNegateHRandomlyFlag == true)) {
            *pX = pReferenceX + (pReferenceX - *pX);
        }
        if ((mNegateVAlways == true) || (mNegateVRandomly == true && mNegateVRandomlyFlag == true)) {
            *pY = pReferenceY + (pReferenceY - *pY);
        }
    }
}

void LevelMotionControllerSliceNegate::Update() {
    
}



LevelMotionControllerSliceFixedMod::LevelMotionControllerSliceFixedMod() {
    mType = LEVEL_MOTION_SLICE_TYPE_FIXED_MOD;
    mOffsetX = 0.0f;
    mOffsetY = 0.0f;
    mRotation = 0.0f;
}

LevelMotionControllerSliceFixedMod::~LevelMotionControllerSliceFixedMod() {
    
}

void LevelMotionControllerSliceFixedMod::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    if (pX != NULL && pY != NULL) {
        FVec2 aPos = FVec2(*pX, *pY);
        FVec2 aCenter = FVec2(pReferenceX, pReferenceY);
        aPos = PivotPoint(aPos, mRotation, aCenter);
        *pX = aPos.mX + mOffsetX;
        *pY = aPos.mY + mOffsetY;
    }
}

float LevelMotionControllerSliceFixedMod::ApplyToObjectRotation() {
    return mRotation;
}

void LevelMotionControllerSliceFixedMod::Update() {
    
}



LevelMotionControllerSliceRotate::LevelMotionControllerSliceRotate() {
    mType = LEVEL_MOTION_SLICE_TYPE_ROTATE;
    mPivotOffsetX = 0.0f;
    mPivotOffsetY = 0.0f;
    mRotation = 0.0f;
}

LevelMotionControllerSliceRotate::~LevelMotionControllerSliceRotate() {
    
}

void LevelMotionControllerSliceRotate::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    if (pX != NULL && pY != NULL) {
        FVec2 aPos = FVec2(*pX, *pY);
        FVec2 aCenter = FVec2(pReferenceX + mPivotOffsetX, pReferenceY + mPivotOffsetY);
        aPos = PivotPoint(aPos, mRotation, aCenter);
        *pX = aPos.mX;
        *pY = aPos.mY;
    }
}

float LevelMotionControllerSliceRotate::ApplyToObjectRotation() {
    return mRotation;
}

void LevelMotionControllerSliceRotate::Update() {
    
    float aSpeed = 0.230f; //SPEED_CLASS_MEDIUM
    if (mSpeedClass == SPEED_CLASS_EXTRA_SLOW) {
        aSpeed = 0.125f;
    } else if (mSpeedClass == SPEED_CLASS_SLOW) {
        aSpeed = 0.155f;
    } else if (mSpeedClass == SPEED_CLASS_MEDIUM_SLOW) {
        aSpeed = 0.185f;
    } else if (mSpeedClass == SPEED_CLASS_MEDIUM_FAST) {
        aSpeed = 0.35f;
    } else if (mSpeedClass == SPEED_CLASS_FAST) {
        aSpeed = 0.55f;
    } else if (mSpeedClass == SPEED_CLASS_EXTRA_FAST) {
        aSpeed = 0.85f;
    } else if (mSpeedClass == SPEED_CLASS_INSANE) {
        aSpeed = 1.25f;
    }
    
    if (mSpeedNegateAlways == true) {
        aSpeed = -fabsf(aSpeed);
    }
    
    if (mSpeedNegateRandomly == true && mSpeedNegateRandomlyFlag == true) {
        aSpeed = -fabsf(aSpeed);
    }
    
    mRotation += aSpeed;
    if (mRotation >= 360.0f) { mRotation -= 360.0f; }
    if (mRotation < 0.0f) { mRotation += 360.0f; }
}

LevelMotionControllerSliceOscillateGeneric::LevelMotionControllerSliceOscillateGeneric() {
    mOscillateSin = 0.0f;
    mIsPaused = false;
    mPauseSide = 0;
    mPauseTimer = 0;
    mLinearTimer = 90;
    mLinearTime = 180;
    mLinearDir = 1;
    mLinear = false;
    mPause1 = 0;
    mPause2 = 0;
    mPauseStartType = 0;
}

LevelMotionControllerSliceOscillateGeneric::~LevelMotionControllerSliceOscillateGeneric() {
    
}

void LevelMotionControllerSliceOscillateGeneric::Update() {
    if (mPauseTimer > 0) {
        if (mPauseExit == true) {
            
        } else {
            mPauseTimer -= 1;
        }
        return;
    }
    if (mLinearDir > 0) {
        mLinearTimer += 1;
        if (mLinearTimer >= mLinearTime) {
            mLinearDir = -1;
            mLinearTimer = mLinearTime;
            if (mPause1 > 0) {
                mPauseTimer = mPause1;
            }
        }
    } else {
        mLinearTimer -= 1;
        if (mLinearTimer <= 0) {
            mLinearDir = 1;
            mLinearTimer = 0;
            if (mPause2 > 0) {
                mPauseTimer = mPause2;
            }
        }
    }
}

bool LevelMotionControllerSliceOscillateGeneric::ShouldPauseExit() {
    return ((mPause1 > 0) || (mPause2 > 0));
}

bool LevelMotionControllerSliceOscillateGeneric::IsPauseExitComplete() {
    if (ShouldPauseExit() == true) {
        return (mPauseTimer > 0);
    }
    return true;
}


float LevelMotionControllerSliceOscillateGeneric::GetPercent() {
    if (mLinear == false) {
        if (mLinearTime > 0) {
            float aResult = -90.0f + ((float)mLinearTimer * 180.0f) / ((float)mLinearTime);
            aResult = Sin(aResult);
            if (mSpeedNegateAlways == true) { aResult = -aResult; }
            else if (mSpeedNegateRandomly == true && mSpeedNegateRandomlyFlag == true) { aResult = -aResult; }
            
            return aResult;
        }
        return 0.0f;
    } else {
        if (mLinearTime > 0) {
            float aResult = -1.0f + ((float)mLinearTimer * 2.0f) / ((float)mLinearTime);
            if (mSpeedNegateAlways == true) { aResult = -aResult; }
            else if (mSpeedNegateRandomly == true && mSpeedNegateRandomlyFlag == true) { aResult = -aResult; }
            return aResult;
        }
        return 0.0f;
    }
}

void LevelMotionControllerSliceOscillateGeneric::ApplyPauseStartType() {
    
    
    if (mPauseStartType == 1) {
        mPauseTimer = mPause1;
        mLinearDir = -1;
        mLinearTimer = mLinearTime;
    }
    
    if (mPauseStartType == 2) {
        mPauseTimer = mPause2;
        mLinearDir = 1;
        mLinearTimer = 0;
    }
    
}

LevelMotionControllerSliceOscillateRotation::LevelMotionControllerSliceOscillateRotation() {
    mType = LEVEL_MOTION_SLICE_TYPE_OSCILLATE_ROTATION;
    mAngleSpan = 90.0f;
    mAngleSpanOffsetStart = 0.0f;
    mAngleSpanOffsetEnd = 0.0f;
}

LevelMotionControllerSliceOscillateRotation::~LevelMotionControllerSliceOscillateRotation() {
    
}

void LevelMotionControllerSliceOscillateRotation::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    if (pX != NULL && pY != NULL) {
        float aRotationStart = -(mAngleSpan * 0.5f) - mAngleSpanOffsetStart;
        float aRotationEnd = (mAngleSpan * 0.5f) + mAngleSpanOffsetEnd;
        float aRotation = (GetPercent() + 1.0f) * 0.5f * (aRotationEnd - aRotationStart) + aRotationStart;
        FVec2 aPos = FVec2(*pX, *pY);
        FVec2 aCenter = FVec2(pReferenceX, pReferenceY);
        aPos = PivotPoint(aPos, aRotation, aCenter);
        *pX = aPos.mX;
        *pY = aPos.mY;
    }
}

float LevelMotionControllerSliceOscillateRotation::ApplyToObjectRotation() {
    float aRotationStart = -(mAngleSpan * 0.5f) - mAngleSpanOffsetStart;
    float aRotationEnd = (mAngleSpan * 0.5f) + mAngleSpanOffsetEnd;
    float aRotation = (GetPercent() + 1.0f) * 0.5f * (aRotationEnd - aRotationStart) + aRotationStart;
    return aRotation;
}

void LevelMotionControllerSliceOscillateRotation::Update() {
    LevelMotionControllerSliceOscillateGeneric::Update();
}

LevelMotionControllerSliceOscillateVertical::LevelMotionControllerSliceOscillateVertical() {
    mType = LEVEL_MOTION_SLICE_TYPE_OSCILLATE_VERTICAL;
    mRadius = 32.0f;
}

LevelMotionControllerSliceOscillateVertical::~LevelMotionControllerSliceOscillateVertical() {
    
}

void LevelMotionControllerSliceOscillateVertical::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    if (pX != NULL && pY != NULL) {
        *pX = *pX;
        *pY = *pY + GetPercent() * mRadius;
    }
}

void LevelMotionControllerSliceOscillateVertical::Update() {
    LevelMotionControllerSliceOscillateGeneric::Update();
}

LevelMotionControllerSliceOscillateHorizontal::LevelMotionControllerSliceOscillateHorizontal() {
    mType = LEVEL_MOTION_SLICE_TYPE_OSCILLATE_HORIZONTAL;
    mRadius = 32.0f;
}

LevelMotionControllerSliceOscillateHorizontal::~LevelMotionControllerSliceOscillateHorizontal() {
    
}

void LevelMotionControllerSliceOscillateHorizontal::Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) {
    if (pX != NULL && pY != NULL) {
        *pX = *pX + GetPercent() * mRadius;
        *pY = *pY;
    }
}

void LevelMotionControllerSliceOscillateHorizontal::Update() {
    LevelMotionControllerSliceOscillateGeneric::Update();
}

