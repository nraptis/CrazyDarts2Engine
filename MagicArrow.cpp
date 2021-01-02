//
//  MagicArrow.cpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 1/7/20.
//  Copyright © 2020 Froggy Studios. All rights reserved.
//

#include "MagicArrow.hpp"
#include "os_core_graphics.h"
#include "GFXApp.hpp"
#include "Game.hpp"

MagicArrow::MagicArrow() {
    mHidden = true;
    
    mSnake.SetSprite(&gWadGameInterface.mAimArrowBody);
    mSnake.SetSpriteStart(&gWadGameInterface.mAimArrowButt);
    
    mSnakeWhite.SetSprite(&gWadGameInterface.mAimArrowBodyWhite);
    mSnakeWhite.SetSpriteStart(&gWadGameInterface.mAimArrowButtWhite);
    
    mSnakeShadow.SetSprite(&gWadGameInterface.mAimArrowBodyShadow);
    mSnakeShadow.SetSpriteStart(&gWadGameInterface.mAimArrowButtShadow);
    
    mHeadRotation = 0.0f;
    
    mHeadX = 0.0f;
    mHeadY = 0.0f;
    
    mHeadDirX = 0.0f;
    mHeadDirY = -1.0f;
}

MagicArrow::~MagicArrow() {
    
}

void MagicArrow::Update() {
    
    
    
}

void MagicArrow::Hide() {
    mHidden = true;
}

void MagicArrow::Show() {
    mHidden = false;
}

void MagicArrow::Refresh(FPointList *pPointList, bool pIsNarrow) {
    
    mPointList.RemoveAll();
    mPolyPath.RemoveAll();
    mBoneList.RemoveAll();
    
    float aX = 0.0f;
    float aY = 0.0f;
    
    if (pPointList != NULL) {
        
        for (int i=0;i<pPointList->mCount;i++) {
            
            aX = pPointList->mX[i];
            aY = pPointList->mY[i];
            
            FCanvas::ConvertPoint(aX, aY, gGame, gInterfaceOverlay);
            mPointList.Add(aX, aY);
            
        }
    }
    
    for (int i=0;i<mPointList.mCount;i++) {
        aX = mPointList.mX[i];
        aY = mPointList.mY[i];
        
        mPolyPath.Add(aX, aY);
    }
    
    mPolyPath.Generate();
    
    if (mPolyPath.mLength < 80.0f) {
        Hide();
        return;
    }
    
    int aBoneCount = 160;
    
    float aBoneCap = ((float)(aBoneCount - 1));
    float aPercent = 0.0f;
    for (int i=0;i<aBoneCount;i++) {
        aPercent = ((float)i) / aBoneCap;
        mPolyPath.GetWithPercent(aPercent, aX, aY);
        mBoneList.Add(aX, aY);
    }
    
    TruncateBoneList();
    
    ComputeHeadRotation();
    
    mSnake.Reset();
    mSnakeWhite.Reset();
    mSnakeShadow.Reset();
    
    float aShadowShiftX = GetShadowShiftX();
    float aShadowShiftY = GetShadowShiftY();
    
    for (int i=0;i<mBoneList.mCount;i++) {
        
        aX = mBoneList.mX[i];
        aY = mBoneList.mY[i];
        
        mSnake.AddPoint(aX, aY);
        mSnakeWhite.AddPoint(aX, aY);
        mSnakeShadow.AddPoint(aX + aShadowShiftX, aY + aShadowShiftY);
    }
    
    bool aNarrow = false;
    if (gGame != NULL) { aNarrow = gGame->mTrajectoryIsNarrow; }
    
    if (aNarrow) {
        mSnake.GenerateWithFixedNormals(1.0f, 0.0f);
        mSnakeWhite.GenerateWithFixedNormals(1.0f, 0.0f);
        mSnakeShadow.GenerateWithFixedNormals(1.0f, 0.0f);
    } else {
        mSnake.Generate();
        mSnakeWhite.Generate();
        mSnakeShadow.Generate();
    }
    
}

void MagicArrow::Draw() {
    
    if (mHidden == true) {
        mSnake.Reset();
        mSnakeShadow.Reset();
        return;
    }
    
    Graphics::CullFacesSetDisabled();
    
    Graphics::TextureBind(gWadGameInterface.mAimArrowBodyShadow.mTexture);
    Graphics::PipelineStateSetShapeNodeSpriteAlphaBlending();
    Graphics::SetColor(0.35f);
        
    mSnakeShadow.Draw();
        
    Graphics::PipelineStateSetSpriteAlphaBlending();
    Graphics::SetColor(0.35f);
    gWadGameInterface.mAimArrowHeadShadow.Draw(mHeadX + GetShadowShiftX(), mHeadY + GetShadowShiftY(), 1.0f, mHeadRotation);
    
    
    Graphics::TextureBind(gWadGameInterface.mAimArrowBodyShadow.mTexture);
    Graphics::PipelineStateSetShapeNodeSpritePremultipliedBlending();
    Graphics::SetColor();
    mSnakeWhite.Draw();
        
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    Graphics::SetColor();
    gWadGameInterface.mAimArrowHeadWhite.Draw(mHeadX, mHeadY, 1.0f, mHeadRotation);
    
    Graphics::PipelineStateSetShapeNodeSpritePremultipliedBlending();
    Graphics::SetColor();
    
    mSnake.Draw();
        
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    Graphics::SetColor();
    
    gWadGameInterface.mAimArrowHead.Draw(mHeadX, mHeadY, 1.0f, mHeadRotation);
    
    Graphics::CullFacesSetBack();
}

void MagicArrow::ComputeHeadRotation() {
    
    if (mBoneList.mCount <= 1) { return; }
    
    mHeadX = mBoneList.mX[mBoneList.mCount - 1];
    mHeadY = mBoneList.mY[mBoneList.mCount - 1];
    
    bool aNarrow = false;
    if (gGame != NULL) { aNarrow = gGame->mTrajectoryIsNarrow; }
    
    for (int i=mBoneList.mCount-2;i>=0;i--) {
        
        float aX = mBoneList.mX[i];
        float aY = mBoneList.mY[i];
        
        float aDiffX = mHeadX - aX;
        float aDiffY = mHeadY - aY;
        
        float aDist = aDiffX * aDiffX + aDiffY * aDiffY;
        
        if (aDist >= 3.0f) {
            
            aDist = sqrtf(aDist);
            
            mHeadDirX = aDiffX / aDist;;
            mHeadDirY = aDiffY / aDist;
            
            mHeadRotation = FaceTarget(-mHeadDirX, -mHeadDirY);
            
            return;
        }
    }
}

float MagicArrow::GetShadowShiftX() {
    return -1.0f * gSpriteDrawScale;
}

float MagicArrow::GetShadowShiftY() {
    return 1.0f * gSpriteDrawScale;
}

void MagicArrow::TruncateBoneList() {
    
    if (mBoneList.mCount < 10) {
        return;
    }
    
    float aY = 0.0f;
    int aPeakIndex = 0;
    float aPeakY = mBoneList.mY[0];
    for (int i=0;i<mBoneList.mCount;i++) {
        
        aY = mBoneList.mY[i];
        
        if (aY < aPeakY) {
            aPeakY = aY;
            aPeakIndex = i;
        }
    }
    
    int aTruncateIndex = -1;
    float aFallDist = 72.0f * gSpriteDrawScale;
    
    for (int i=aPeakIndex+1;(i < mBoneList.mCount) && (aTruncateIndex == -1);i++) {
        aY = mBoneList.mY[i];
        float aDist = (aY - aPeakY);
        if (aDist > aFallDist) {
            aTruncateIndex = i - 1;
            break;
        }
    }
    
    if (aTruncateIndex != -1) {
        
        if (aTruncateIndex < (mBoneList.mCount - 1)) {
            
            float aLastX = mBoneList.mX[aTruncateIndex];
            float aLastY = mBoneList.mY[aTruncateIndex];
            float aNextX = mBoneList.mX[aTruncateIndex + 1];
            float aNextY = mBoneList.mY[aTruncateIndex + 1];
            
            float aDistLast = aLastY - aPeakY;
            float aDistMiddle = aFallDist;
            float aDistNext = aNextY - aPeakY;
            
            if ((aDistLast <= aDistMiddle) && (aDistLast < (aDistNext - 0.05f)) && (aDistMiddle <= aDistNext)) {
                float aPercent = (aDistMiddle - aDistLast) / (aDistNext - aDistLast);
                float aX = aLastX + (aNextX - aLastX) * aPercent;
                float aY = aLastY + (aNextY - aLastY) * aPercent;
                
                mBoneList.mX[aTruncateIndex] = aX;
                mBoneList.mY[aTruncateIndex] = aY;
                aTruncateIndex++;
            }
        }
        mBoneList.mCount = aTruncateIndex;
    }
}
