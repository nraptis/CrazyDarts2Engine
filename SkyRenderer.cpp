//
//  SkyRenderer.cpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 9/14/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "SkyRenderer.hpp"
#include "GFXApp.hpp"
#include "Game.hpp"

SkyRenderer::SkyRenderer() {
    mGrid.Size(100, 6);
    
    mTop = 0.0f;
    mRight = 100.0f;
    mBottom = 100.0f;
    mLeft = 0.0f;
    
    mRecentSpawnTimer = 0;
    
}

SkyRenderer::~SkyRenderer() {
    
    mCloudList.Free();
    
}

void SkyRenderer::Refresh(float pTop, float pRight, float pBottom, float pLeft, float pPadding) {
 
    
    mTop = pTop;
    mRight = pRight;
    mBottom = pBottom;
    mLeft = pLeft;
    
    
    pTop -= pPadding;
    pBottom += pPadding;
    
    pRight += pPadding;
    pLeft -= pPadding;
    
    //FColor("b2dce1").Print("b2dce1");
    //FColor("aedce3").Print("aedce3");
    //FColor("8ee1f5").Print("8ee1f5");
    //FColor("54d4f5").Print("54d4f5");
    //FColor("3abceb").Print("3abceb");
    //FColor("37baea").Print("37baea");
    
    int aGridWidth = mGrid.mWidth;
    int aGridHeight = mGrid.mHeight;
    
    float aWidth = (pRight - pLeft);
    float aHeight = (pBottom - pTop);
    
    
    float aPercentY[6];
    
    aPercentY[0] = 0.0f;
    aPercentY[1] = 0.125f;
    aPercentY[2] = 0.265f;
    aPercentY[3] = 1.0f - aPercentY[2];
    aPercentY[4] = 1.0f - aPercentY[1];
    aPercentY[5] = 1.0f;
    
    //float aLeft = pLeft - pPadding;
    for (int i=0;i<aGridWidth;i++) {
        float aPercentX = ((float)i) / ((float)(aGridWidth - 1));
        for (int n=0;n<aGridHeight;n++) {
            float aX = pLeft + (aWidth * aPercentX);
            float aY = pTop + (aHeight * aPercentY[n]);
            
            //Log("Sky[%d][%d] = (%f | %f)\n", i, n, aX, aY);
            
            
            mGrid.SetXYZ(i, n, aX, aY, 0.0f);
            //mGrid.SetRGBA(i, n, gRand.F(), gRand.F(), gRand.F(), 0.5f + gRand.F(0.5f));
            
        }
    }
    
    float aSeedColorR[6];
    float aSeedColorG[6];
    float aSeedColorB[6];
    
    aSeedColorR[0] = 0.698039f;
    aSeedColorR[1] = 0.682353f;
    aSeedColorR[2] = 0.556863f;
    aSeedColorR[3] = 0.329412f;
    aSeedColorR[4] = 0.227451f;
    aSeedColorR[5] = 0.215686f;
    
    aSeedColorG[0] = 0.862745f;
    aSeedColorG[1] = 0.862745f;
    aSeedColorG[2] = 0.882353f;
    aSeedColorG[3] = 0.831373f;
    aSeedColorG[4] = 0.737255f;
    aSeedColorG[5] = 0.729412f;
    
    aSeedColorB[0] = 0.882353f;
    aSeedColorB[1] = 0.890196f;
    aSeedColorB[2] = 0.960784f;
    aSeedColorB[3] = 0.960784f;
    aSeedColorB[4] = 0.921569f;
    aSeedColorB[5] = 0.917647f;
    
    for (int i=0;i<6;i++) {
        //aSeedColorR[i] *= 0.50f;
        //aSeedColorG[i] *= 0.55f;
        //aSeedColorB[i] *= 0.65f;
    }
    
    for (int n=0;n<aGridHeight;n++) {
        for (int i=0;i<aGridWidth;i++) {
            mGrid.SetRGBA(i, n, aSeedColorR[n], aSeedColorG[n], aSeedColorB[n], 1.0f);
        }
    }
    
    AttemptInitialLayout();
    
    EnumList(SkyRendererCloud, aCloud, mCloudList.mObjectList) {
        aCloud->HandleRefresh(mTop, mRight, mBottom, mLeft);
    }
    
}

void SkyRenderer::Update() {
    
    mRecentSpawnTimer++;
    if (mRecentSpawnTimer >= 100) {
        mRecentSpawnTimer = 0;
        SpawnCloud();
    }
    
    EnumList(SkyRendererCloud, aCloud, mCloudList.mObjectList) {
        if ((aCloud->mKill == 0) && (gGame != NULL)) {
            //aCloud->mX += gGame->mWind.mPower * 0.325f * gSpriteDrawScale * aCloud->mHotizontalMovementFactor;
            aCloud->mX += 0.245f * gSpriteDrawScale * aCloud->mHotizontalMovementFactor;
        }
    }
    
    mCloudList.Update();
    
}

void SkyRenderer::Draw() {
    mGrid.Draw();
}

void SkyRenderer::DrawClouds() {
    mCloudList.Draw();
    Graphics::SetColor();
}

void SkyRenderer::SpawnCloud() {
    
    int aPlaceAttemptCount = 40;
    
    
    int aSpawnIndex = GetSpawnIndex();
    if (aSpawnIndex == -1) { return; }
    
    FSprite *aCloudSprite = GetCloudSpriteForIndex(aSpawnIndex);
    if (aCloudSprite == NULL) { return; }
    
    float aWiggleRoom = 40.0f * gSpriteDrawScale;
    
    float aMinX = (-aCloudSprite->mWidth / 2.0f) - aWiggleRoom;
    float aMinY = (-aCloudSprite->mHeight / 2.0f) + 100.0f;
    //float aMaxX = (gAppWidth + aCloudSprite->mWidth / 2.0f) + aWiggleRoom;
    float aMaxY = (gAppHeight + aCloudSprite->mHeight / 2.0f) - 100.0f;
    
    float aX = 0.0f;
    float aY = 0.0f;
    bool aPlaced = false;
    
    for (int k=0;(k < aPlaceAttemptCount) && (aPlaced == false);k++) {
        
        //if (gRand.GetBool()) {
            aX = aMinX;
        //} else {
        //    aX = aMaxX;
        //}
        
        aY = gRand.GetFloat(aMinY, aMaxY);
            
        if (IsCloudBlocked(aCloudSprite, aX, aY) == false) {
            aPlaced = true;
        }
    }
    
    
    if (aPlaced == true) {
        
        SkyRendererCloud *aCloud = new SkyRendererCloud(this, aCloudSprite, aSpawnIndex);
        aCloud->mX = aX;
        aCloud->mY = aY;
        aCloud->HandleRefresh(mTop, mRight, mBottom, mLeft);
        mCloudList.Add(aCloud);
    }
}

FSprite *SkyRenderer::GetCloudSpriteForIndex(int pIndex) {
    FSprite *aResult = NULL;
    aResult = &(gWadGameInterface.mCloud[pIndex]);
    return aResult;
}

int SkyRenderer::GetCloudIndexCount() {
    return 13;
}

void SkyRenderer::AttemptInitialLayout() {

    if (mCloudList.mObjectList.mCount > 0) { return; }
    
    int aSpawnAttemptCount = GetCloudIndexCount();
    int aPlaceAttemptCount = aSpawnAttemptCount;
    
    for (int i=0;i<aSpawnAttemptCount;i++) {
        
        int aSpawnIndex = GetSpawnIndex();
        FSprite *aCloudSprite = GetCloudSpriteForIndex(aSpawnIndex);
        if ((aSpawnIndex != -1) && (aCloudSprite != NULL)) {
            
            float aMinX = (-aCloudSprite->mWidth / 2.0f) + 100.0f;
            float aMinY = (-aCloudSprite->mHeight / 2.0f) + 100.0f;
            float aMaxX = (gAppWidth + aCloudSprite->mWidth / 2.0f) - 100.0f;
            float aMaxY = (gAppHeight + aCloudSprite->mHeight / 2.0f) - 100.0f;
            
            float aX = 0.0f;
            float aY = 0.0f;
            bool aPlaced = false;
            
            for (int k=0;(k < aPlaceAttemptCount) && (aPlaced == false);k++) {
                
                aX = gRand.GetFloat(aMinX, aMaxX);
                aY = gRand.GetFloat(aMinY, aMaxY);
                
                if (IsCloudBlocked(aCloudSprite, aX, aY) == false) {
                    aPlaced = true;
                }
            }
            
            if (aPlaced == true) {
                
                SkyRendererCloud *aCloud = new SkyRendererCloud(this, aCloudSprite, aSpawnIndex);
                aCloud->mX = aX;
                aCloud->mY = aY;
                mCloudList.Add(aCloud);
            }
        }
    }
}

FRect cCloudRect1;
FRect cCloudRect2;

bool SkyRenderer::IsCloudBlocked(FSprite *pCloudSprite, float pX, float pY) {
    
    if (pCloudSprite == NULL) { return true; }
    
    float aCloudWidth = pCloudSprite->mWidth * 1.25f;
    float aCloudHeight = pCloudSprite->mHeight * 1.25f;
    
    cCloudRect1.mX = pX - aCloudWidth  / 2.0f;
    cCloudRect1.mY = pY - aCloudHeight / 2.0f;
    cCloudRect1.mWidth  = aCloudWidth;
    cCloudRect1.mHeight = aCloudHeight;
    
    bool aResult = false;
    EnumList(SkyRendererCloud, aCloud, mCloudList.mObjectList) {
        
        float aCheckCloudWidth = aCloud->mSpriteWidth * 1.25f;
        float aCheckCloudHeight = aCloud->mSpriteHeight * 1.25f;
        
        cCloudRect2.mX = aCloud->mX - aCheckCloudWidth / 2.0f;
        cCloudRect2.mY = aCloud->mY - aCheckCloudHeight / 2.0f;
        cCloudRect2.mWidth  = aCheckCloudWidth;
        cCloudRect2.mHeight = aCheckCloudHeight;
        
        if (cCloudRect1.Intersects(cCloudRect2) == true) {
            aResult = true;
        }
    }
    return aResult;
}

int SkyRenderer::GetSpawnIndex() {
    
    int aResult = -1;
    
    int aCount = GetCloudIndexCount();
    
    mTempCloudIndexList.RemoveAll();
    for (int i=0;i<aCount;i++) {
        
        bool aExists = false;
        EnumList(SkyRendererCloud, aCloud, mCloudList.mObjectList) {
            if (aCloud->mIndex == i) {
                aExists = true;
            }
        }
        
        if (aExists == false) {
            mTempCloudIndexList.Add(i);
        }
    }
    
    if (mTempCloudIndexList.mCount > 0) {
        aResult = mTempCloudIndexList.Get(gRand.Get(mTempCloudIndexList.mCount));
    }
    
    return aResult;
}
