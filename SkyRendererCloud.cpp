//
//  SkyRendererCloud.cpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 12/6/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "SkyRendererCloud.hpp"
#include "SkyRenderer.hpp"
#include "GFXApp.hpp"

SkyRendererCloud::SkyRendererCloud(SkyRenderer *pRenderer, FSprite *pSprite, int pIndex) {
    
    mX = 0.0f;
    mY = 0.0f;
    
    mRenderer = pRenderer;
    mSprite = pSprite;
    
    //The "scale" which converts real 2-D to fake 2-D...
    mConversionScale = 1.0f;
    
    mZoneLeft = 0.0f;
    mZoneRight = 0.0f;
    
    mZoneTop = 0.0f;
    mZoneBottom = 0.0f;
    
    mZoneWidth = 0.0f;
    mZoneHeight = 0.0f;
    
    mSpriteWidth = 0.0f;
    mSpriteHeight = 0.0f;
    
    mHotizontalMovementFactor = gRand.GetFloat(0.98f, 1.2f);
    
    mVerticalSin = gRand.GetRotation();
    mVerticalSinSpeed = gRand.GetFloat(0.24f, 0.25f);
    mVerticalSpread = gRand.GetFloat(6.0f, 8.0f);
    
    if (mSprite != NULL) {
        mSpriteWidth = mSprite->mWidth;
        mSpriteHeight = mSprite->mHeight;
    }
    
    mRotation = 0.0f;
    mScale = 1.0f;
    
    mIndex = pIndex;
    
    SetTopLeftPositions(-10.0f, -10.0f);
    SetTopRightPositions(10.0f, -10.0f);
    SetBottomLeftPositions(-10.0f, 10.0f);
    SetBottomRightPositions(10.0f, 10.0f);
    
    SetTopLeftTextureCoords(0.0f, 0.0f);
    SetTopRightTextureCoords(1.0f, 0.0f);
    SetBottomLeftTextureCoords(0.0f, 1.0f);
    SetBottomRightTextureCoords(1.0f, 1.0f);
    
    if (mSprite != NULL) {
        SetTopLeftTextureCoords(mSprite->GetStartU(), mSprite->GetStartV());
        SetTopRightTextureCoords(mSprite->GetEndU(), mSprite->GetStartV());
        SetBottomLeftTextureCoords(mSprite->GetStartU(), mSprite->GetEndV());
        SetBottomRightTextureCoords(mSprite->GetEndU(), mSprite->GetEndV());
    }
}

SkyRendererCloud::~SkyRendererCloud() {
    
}

void SkyRendererCloud::Update() {
    
    float aKillZoneLeft = 0.0f - 200.0f * gSpriteDrawScale;
    float aKillZoneRight = gAppWidth + 200.0f * gSpriteDrawScale;
    
    if (mX < aKillZoneLeft) { Kill(); }
    if (mX > aKillZoneRight) { Kill(); }
    
    mVerticalSin += mVerticalSinSpeed;
    if (mVerticalSin >= 360.0f) { mVerticalSin -= 360.0f; }
    if (mVerticalSin < 0.0f) { mVerticalSin += 360.0f; }
    
}

void SkyRendererCloud::Draw() {
    if ((mSprite != NULL) && (mSprite->mTexture != NULL)) {
        
        float aOffsetY = Sin(mVerticalSin) * mVerticalSpread * gSpriteDrawScale;
        
        float aPercentX = (mX / gAppWidth);
        float aPercentY = ((mY + aOffsetY) / gAppHeight);
        
        float aX = mZoneLeft + mZoneWidth * aPercentX;
        float aY = mZoneTop + mZoneHeight * aPercentY;
        
        FMatrix aHoldModelView = Graphics::MatrixModelViewGet();
        
        FMatrix aModelView;
        aModelView.Set(aHoldModelView);
        aModelView.Translate(aX, aY);
        
        Graphics::MatrixModelViewSet(aModelView);
        //Graphics::SetColor();
        Graphics::UniformBind();
        
        Graphics::DrawModel(mPositions, mTextureCoords, NULL, 6, mSprite->mTexture);
        Graphics::MatrixModelViewSet(aHoldModelView);
    }
}

void SkyRendererCloud::SetTopLeftPositions(float pX, float pY) {
    mPositions[0] = pX;
    mPositions[1] = pY;
    mPositions[2] = 0.0f;
}

void SkyRendererCloud::SetTopRightPositions(float pX, float pY) {
    mPositions[3] = pX;
    mPositions[4] = pY;
    mPositions[5] = 0.0f;
    
    mPositions[12] = pX;
    mPositions[13] = pY;
    mPositions[14] = 0.0f;
}

void SkyRendererCloud::SetBottomLeftPositions(float pX, float pY) {
    mPositions[6] = pX;
    mPositions[7] = pY;
    mPositions[8] = 0.0f;
    
    mPositions[ 9] = pX;
    mPositions[10] = pY;
    mPositions[11] = 0.0f;
}

void SkyRendererCloud::SetBottomRightPositions(float pX, float pY) {
    mPositions[15] = pX;
    mPositions[16] = pY;
    mPositions[17] = 0.0f;
}

void SkyRendererCloud::SetTopLeftTextureCoords(float pU, float pV) {
    mTextureCoords[0] = pU;
    mTextureCoords[1] = pV;
    mTextureCoords[2] = 0.0f;
}

void SkyRendererCloud::SetTopRightTextureCoords(float pU, float pV) {
    mTextureCoords[3] = pU;
    mTextureCoords[4] = pV;
    mTextureCoords[5] = 0.0f;
    
    mTextureCoords[12] = pU;
    mTextureCoords[13] = pV;
    mTextureCoords[14] = 0.0f;
}

void SkyRendererCloud::SetBottomLeftTextureCoords(float pU, float pV) {
    mTextureCoords[6] = pU;
    mTextureCoords[7] = pV;
    mTextureCoords[8] = 0.0f;
    
    mTextureCoords[ 9] = pU;
    mTextureCoords[10] = pV;
    mTextureCoords[11] = 0.0f;
}

void SkyRendererCloud::SetBottomRightTextureCoords(float pU, float pV) {
    mTextureCoords[15] = pU;
    mTextureCoords[16] = pV;
    mTextureCoords[17] = 0.0f;
}

void SkyRendererCloud::HandleRefresh(float pTop, float pRight, float pBottom, float pLeft) {
    
    mZoneLeft = pLeft;
    mZoneRight = pRight;
    
    mZoneTop = pTop;
    mZoneBottom = pBottom;
    
    mZoneWidth  = (pRight - pLeft);
    mZoneHeight = (pBottom - pTop);
    
    if (mZoneWidth <= 1.0f) { return; }
    if (mZoneHeight <= 1.0f) { return; }
    if (mSprite == NULL) { return; }
    
    mConversionScale = mZoneWidth / gAppWidth;
    
    float aNodeWidth2 = (mSpriteWidth * 0.5f * mConversionScale);
    float aNodeHeight2 = (mSpriteHeight * 0.5f * mConversionScale);

    SetTopLeftPositions(-aNodeWidth2, -aNodeHeight2);
    SetTopRightPositions(aNodeWidth2, -aNodeHeight2);
    SetBottomLeftPositions(-aNodeWidth2, aNodeHeight2);
    SetBottomRightPositions(aNodeWidth2, aNodeHeight2);
    
    if (mSprite != NULL) {
        SetTopLeftTextureCoords(mSprite->GetStartU(), mSprite->GetStartV());
        SetTopRightTextureCoords(mSprite->GetEndU(), mSprite->GetStartV());
        SetBottomLeftTextureCoords(mSprite->GetStartU(), mSprite->GetEndV());
        SetBottomRightTextureCoords(mSprite->GetEndU(), mSprite->GetEndV());
    }
    
}
