//
//  GameOverlayInterfaceInterface.cpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 9/3/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "core_includes.h"
#include "GameContainer.hpp"
#include "GameOverlayInterface.hpp"
#include "FAnimation.hpp"
#include "Game.hpp"
#include "FMath.hpp"
#include "FLine.hpp"
#include "GFXApp.hpp"
#include "FloatingLifeBubble.hpp"
#include "GameMenu.hpp"

GameOverlayInterface *gInterfaceOverlay = NULL;

GameOverlayInterface::GameOverlayInterface() {
    
    //mClipEnabled = true;
    
    gInterfaceOverlay = this;
    
    //mProgressBar = NULL;
    //mProgressWheel = NULL;
    mTimeClock = NULL;
    
    
    
    mPauseButton = NULL;
    
    mTopBar = NULL;
    
    mConsumesTouches = false;
    
    mName = "GameOverlayInterface";
    
    
    mWallInsetSpan = 0.0f;
    mWallInsetSides = 0.0f;
    mWallInsetBottom = 0.0f;
    mWallInsetTop = 0.0f;
    
    mWallLeftX = 0.0f;
    mWallRightX = 0.0f;
    
    mWallTopY = 0.0f;
    mWallBottomY = 0.0f;
    
    mWallWidth2 = 0.0f;
    mWallHeight2 = 0.0f;
    
    mWallWhiteWidth2 = 0.0f;
    mWallWhiteHeight2 = 0.0f;
    
    mWallShadoweWidth2 = 0.0f;
    mWallShadowHeight2 = 0.0f;
    
    mGameCorner1X = 0.0f;
    mGameCorner1Y = 0.0f;
    mGameCorner2X = 0.0f;
    mGameCorner2Y = 0.0f;
    mGameCorner3X = 0.0f;
    mGameCorner3Y = 0.0f;
    mGameCorner4X = 0.0f;
    mGameCorner4Y = 0.0f;
    
    mPauseButton = new ShadowButton();
    AddChild(mPauseButton);
    mPauseButton->mWhiteSprite = &gWadGameInterface.mPauseButtonWhite;
    mPauseButton->mShadowSprite = &gWadGameInterface.mPauseButtonShadow;
    mPauseButton->SetUp(&gWadGameInterface.mPauseButtonUp, &gWadGameInterface.mPauseButtonUp, &gWadGameInterface.mPauseButtonDown);
    mPauseButton->mRecievesConsumedTouches = true;
    mPauseButton->mDrawManual = true;
    mPauseButton->mDrawSetsSpriteAlphaPipelineState = false;
    gNotify.Register(this, mPauseButton, "button_click");
    
    //mProgressBar = new ProgressBar();
    //AddChild(mProgressBar);
    //mProgressBar->mDrawManual = true;
    
    //mProgressWheel = new ProgressWheel;
    //AddChild(mProgressWheel);
    //mProgressWheel->mDrawManual = true;
    
    
    mTimeClock = new TimeClock();
    AddChild(mTimeClock);
    mTimeClock->mDrawManual = true;
    
    
    
    mTopBar = new TopBar();
    AddChild(mTopBar);
    mTopBar->mDrawManual = true;
    
}

GameOverlayInterface::~GameOverlayInterface() {
    if (gInterfaceOverlay == this) { gInterfaceOverlay = NULL; }
    
    mPauseButton = NULL;
    mTimeClock = NULL;
}

void GameOverlayInterface::Layout() {
    
    
    
    mWallInsetSpan = roundf(8.0f * gSpriteDrawScale);
    mWallInsetSides = roundf(7.0f * gSpriteDrawScale);
    mWallInsetBottom = roundf(7.0f * gSpriteDrawScale);
    mWallInsetTop = roundf(4.0f * gSpriteDrawScale);
    
    mWallLeftX = roundf(mGameCorner1X - mWallInsetSides);
    mWallRightX = roundf(mGameCorner2X + mWallInsetSides);
    
    mWallTopY = roundf(mGameCorner1Y - mWallInsetTop);
    mWallBottomY = roundf(mGameCorner3Y + mWallInsetBottom);
    
    mWallWidth2 = roundf(gWadGameInterface.mEdgeWallVertical.mWidth * 0.5f);
    mWallHeight2 = roundf(gWadGameInterface.mEdgeWallHorizontal.mHeight * 0.5f);
    
    mWallWhiteWidth2 = roundf(gWadGameInterface.mEdgeWallVerticalWhite.mWidth * 0.5f);
    mWallWhiteHeight2 = roundf(gWadGameInterface.mEdgeWallHorizontalWhite.mHeight * 0.5f);
    
    mWallShadoweWidth2 = roundf(gWadGameInterface.mEdgeWallVerticalShadow.mWidth * 0.5f);
    mWallShadowHeight2 = roundf(gWadGameInterface.mEdgeWallHorizontalShadow.mHeight * 0.5f);
    
    float aPaddingBottom = roundf(3.0f * gSpriteDrawScale);
    float aFloor = roundf(mGameCorner3Y - aPaddingBottom);
    
    
    float aPauseButtonRight = mGameCorner1X;
    if (mPauseButton != NULL) {
        float aWidth = roundf(gWadGameInterface.mPauseButtonUp.mWidth + 2.0f * gSpriteDrawScale);
        float aHeight = roundf(gWadGameInterface.mPauseButtonUp.mHeight + 2.0f * gSpriteDrawScale);
        float aPaddingLeft = roundf(3.0f * gSpriteDrawScale + gSafeAreaInsetLeft);
        
        mPauseButton->SetFrame(roundf(mGameCorner1X + aPaddingLeft), roundf(aFloor - aHeight), aWidth, aHeight);
        
        aPauseButtonRight = mPauseButton->GetRight();
    }
    
    if (mTimeClock != NULL) {
        
        float aLeft = roundf(aPauseButtonRight + (4.0f * gSpriteDrawScale));
        float aRight = roundf(mWallRightX - (12.0f * gSpriteDrawScale));
        
        float aMinWidth = roundf(164.0f * gSpriteDrawScale);
        float aMinLeft = roundf(mWallLeftX + (mWallRightX - mWallLeftX) * 0.35f);
        
        if (aLeft < aMinLeft) {
            aLeft = aMinLeft;
        }
        
        float aWidth = roundf(aRight - aLeft);
        float aHeight = roundf(40.0f * gSpriteDrawScale);
        
        mTimeClock->SetFrame(aLeft, roundf(aFloor - aHeight), roundf(aRight - aLeft), aHeight);
    }
}

void GameOverlayInterface::Update() {
    
    mMagicArrow.Update();
    
    if ((gGameContainer != NULL) && ((gGameContainer->mPaused == true) || (gGameContainer->mBackgroundPaused == true))) {
        return;
    }
    
    mAchievementBubbleList.Update();
    
    //if (mProgressWheel != NULL) {
    //    mProgressBar->Update();
    //}
    
    
}

void GameOverlayInterface::Draw() {
    
    
    
    /*
    aTop += 28.0f * gSpriteDrawScale;
    gApp->mSysFontBold.Draw(aStringStreakBalloon, aLeft, aTop);
    
    aTop += 28.0f * gSpriteDrawScale;
    gApp->mSysFontBold.Draw(aStringStreakDart, aLeft, aTop);
    */
    
    mMagicArrow.Draw();
    
    DrawTransform();
    Graphics::PipelineStateSetSpriteAlphaBlending();
    mAchievementBubbleList.Draw();
    
    
    DrawTransform();
    Graphics::PipelineStateSetSpriteAlphaBlending();

    
    DrawTransform();
    
    DrawBorders();
    
    if (mPauseButton != NULL) {
        Graphics::SetColor();
        mPauseButton->DrawTransform();
        Graphics::PipelineStateSetSpriteAlphaBlending();
        mPauseButton->DrawShadow();
        Graphics::PipelineStateSetSpritePremultipliedBlending();
        mPauseButton->DrawWhite();
        Graphics::PipelineStateSetSpriteAlphaBlending();
        mPauseButton->Draw();
    }
    
    if (mTimeClock != NULL) {
        mTimeClock->DrawManual();
        Graphics::SetColor();
    }
    
    
    //DrawTransform();
    
    /*
    Graphics::PipelineStateSetShape2DAlphaBlending();
    Graphics::SetColor(0.76f, 0.85f, 0.45f, 0.75f);
    
    Graphics::DrawPoint(mGameCorner1X, mGameCorner1Y, 7.0f);
    
    Graphics::SetColor(0.56f, 0.25f, 0.45f, 0.75f);
    Graphics::DrawPoint(mGameCorner2X, mGameCorner2Y, 7.0f);
    
    Graphics::SetColor(0.9f, 0.5f, 0.85f, 0.75f);
    Graphics::DrawPoint(mGameCorner3X, mGameCorner3Y, 7.0f);
    
    
    Graphics::SetColor(0.125f, 0.86f, 0.35f, 0.75f);
    Graphics::DrawPoint(mGameCorner4X, mGameCorner4Y, 7.0f);
    
    Graphics::SetColor(1.0f, 1.0f, 1.0f, 0.85f);
    Graphics::DrawLine(mGameCorner1X, mGameCorner1Y, mGameCorner2X, mGameCorner2Y);
    
    Graphics::DrawLine(mGameCorner2X, mGameCorner2Y, mGameCorner3X, mGameCorner3Y);
    
    Graphics::DrawLine(mGameCorner3X, mGameCorner3Y, mGameCorner4X, mGameCorner4Y);
    
    Graphics::DrawLine(mGameCorner4X, mGameCorner4Y, mGameCorner1X, mGameCorner1Y);
    
    */
    
    /*
    
    float aX = 40.0f * gSpriteDrawScale;
    float aY = 70.0f * gSpriteDrawScale;
    
    
    
    for (int i=0;i<21;i++) {
        
        FSprite *aSprite = &(gWadGameInterface.mLifeTextPlus1);
        FSprite *aSpriteWhite = &(gWadGameInterface.mLifeTextPlus1White);
        FSprite *aSpriteShadow = &(gWadGameInterface.mLifeTextPlus1Shadow);
        
        if (i > 0) {
            aSprite = &(gWadGameInterface.mLifeTextMinus[i - 1]);
            aSpriteWhite = &(gWadGameInterface.mLifeTextMinusWhite[i - 1]);
            aSpriteShadow = &(gWadGameInterface.mLifeTextMinusShadow[i - 1]);
        }
        
        
        Graphics::PipelineStateSetShape2DNoBlending();
        
        Graphics::SetColor(1.0f, 0.25f, 0.25f, 0.75f);
        
        
        float aWidth = FloatingLifeBubble::GetWidth(i == 0, i);
        float aHeight = FloatingLifeBubble::GetHeight(i == 0, i);
        
        Graphics::OutlineRect(aX - aWidth * 0.5f, aY - aHeight * 0.5f, aWidth, aHeight, 3.0f);
        
        
        
        
        Graphics::SetColor();
        
        
        Graphics::PipelineStateSetSpriteAlphaBlending();
        aSpriteShadow->Center(aX, aY);
        
        Graphics::PipelineStateSetSpriteWhiteBlending();
        aSpriteWhite->Center(aX, aY);
        
        Graphics::PipelineStateSetSpritePremultipliedBlending();
        aSprite->Center(aX, aY);
        
        
        
        
        
        
        aX += 90.0f * gSpriteDrawScale;
        if (aX > (mWidth - 40.0f * gSpriteDrawScale)) {
            aX = 40.0f * gSpriteDrawScale;
            aY += 76.0f * gSpriteDrawScale;
        }
        
    }
    
    */
    
    /*
    
    DrawTransform();
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    Graphics::SetColor();
    gWadMenuInterface.mFontLargeTitle.Center(FString(gGame->mLevelData->mClockFlyingInTicksRemaining).c(), mWidth2, mHeight2);
    
    */
    
}

void GameOverlayInterface::DrawTopBarManual() {
    if (mTopBar != NULL) {
        mTopBar->DrawManual();
        Graphics::SetColor();
    }
}

void GameOverlayInterface::TouchDown(float pX, float pY, void *pData) {
    
}

void GameOverlayInterface::TouchMove(float pX, float pY, void *pData) {
    
}

void GameOverlayInterface::TouchUp(float pX, float pY, void *pData) {
    
}

void GameOverlayInterface::TouchFlush() {
    
}

void GameOverlayInterface::Notify(void *pSender, const char *pNotification) {
    

#ifndef EDITOR_MODE
    if (pSender == mPauseButton && mPauseButton != NULL) {
        if (gGameContainer != NULL) {
            if (gApp->MenuStackClear() == true) {
                
                if (gGameContainer->CanShowGameMenu() == true) {
                    gApp->PushModalMenu(new GameMenu());
                }
            }
        }
    }
#endif
    
}

void GameOverlayInterface::DrawBorders() {
    
    /*
    float aWallInsetSpan = 8.0f * gSpriteDrawScale;
    float aWallInsetSides = 7.0f * gSpriteDrawScale;
    float aWallInsetBottom = 7.0f * gSpriteDrawScale;
    float aWallInsetTop = 4.0f * gSpriteDrawScale;
    
    float aWallLeftX = mGameCorner1X - aWallInsetSides;
    float aWallRightX = mGameCorner2X + aWallInsetSides;
    
    float aWallTopY = mGameCorner1Y - aWallInsetTop;
    float aWallBottomY = mGameCorner3Y + aWallInsetBottom;
    
    float                                       mWallInsetSpan;
    float                                       mWallInsetSides;
    float                                       mWallInsetBottom;
    float                                       mWallInsetTop;
    
    float                                       mWallLeftX;
    float                                       mWallRightX;
    
    float                                       mWallTopY;
    float                                       mWallBottomY;
    
    
    float                                       aWallWidth2;
    float                                       aWallHeight2;
    
    float                                       aWallWhiteWidth2;
    float                                       aWallWhiteHeight2;
    
    float                                       aWallShadoweWidth2;
    float                                       aWallShadowHeight2;
    */
    
    
    float aBracketX1 = mWallLeftX;
    float aBracketY1 = mWallBottomY;
    
    float aBracketX2 = mWallRightX;
    float aBracketY2 = mWallBottomY;
    
    #ifdef EDITOR_MODE
        Graphics::PipelineStateSetSpriteAlphaBlending();
        Graphics::SetColor(0.5f);
    #else
        Graphics::PipelineStateSetSpriteNoBlending();
        Graphics::SetColor();
    #endif
    
    DrawOutside(mWallTopY, mWallRightX, mWallBottomY, mWallLeftX);
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    gWadGameInterface.mEdgeWallHorizontalShadow.DrawRepeatingH(mWallLeftX + mWallInsetSpan, mWallBottomY - mWallShadowHeight2, (mWallRightX - mWallLeftX) - (mWallInsetSpan + mWallInsetSpan));
    gWadGameInterface.mEdgeWallVerticalShadow.DrawRepeatingV(mWallLeftX - mWallShadoweWidth2, mWallTopY, (mWallBottomY - mWallTopY) - (mWallInsetSpan));
    gWadGameInterface.mEdgeWallVerticalShadow.DrawRepeatingV(mWallRightX - mWallShadoweWidth2, mWallTopY, (mWallBottomY - mWallTopY) - mWallInsetSpan);
    gWadGameInterface.mCornerBracketURShadow.Center(aBracketX1, aBracketY1);
    gWadGameInterface.mCornerBracketULShadow.Center(aBracketX2, aBracketY2);
    
    
    
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    
    gWadGameInterface.mEdgeWallHorizontalWhite.DrawRepeatingH(mWallLeftX + mWallInsetSpan, mWallBottomY - mWallWhiteHeight2, (mWallRightX - mWallLeftX) - (mWallInsetSpan + mWallInsetSpan));
    gWadGameInterface.mEdgeWallVerticalWhite.DrawRepeatingV(mWallLeftX - mWallWhiteWidth2, mWallTopY, (mWallBottomY - mWallTopY) - (mWallInsetSpan));
    gWadGameInterface.mEdgeWallVerticalWhite.DrawRepeatingV(mWallRightX - mWallWhiteWidth2, mWallTopY, (mWallBottomY - mWallTopY) - mWallInsetSpan);
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    
    
    gWadGameInterface.mEdgeWallHorizontal.DrawRepeatingH(mWallLeftX + mWallInsetSpan, mWallBottomY - mWallHeight2, (mWallRightX - mWallLeftX) - (mWallInsetSpan + mWallInsetSpan));
    gWadGameInterface.mEdgeWallVertical.DrawRepeatingV(mWallLeftX - mWallWidth2, mWallTopY, (mWallBottomY - mWallTopY) - (mWallInsetSpan));
    gWadGameInterface.mEdgeWallVertical.DrawRepeatingV(mWallRightX - mWallWidth2, mWallTopY, (mWallBottomY - mWallTopY) - mWallInsetSpan);
    
    
    
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    gWadGameInterface.mCornerBracketURWhite.Center(aBracketX1, aBracketY1);
    gWadGameInterface.mCornerBracketULWhite.Center(aBracketX2, aBracketY2);
    

    Graphics::PipelineStateSetSpriteAlphaBlending();
    gWadGameInterface.mCornerBracketUR.Center(aBracketX1, aBracketY1);
    gWadGameInterface.mCornerBracketUL.Center(aBracketX2, aBracketY2);
    
    
    /*
    Graphics::PipelineStateSetShape2DAlphaBlending();
    Graphics::SetColor(0.5f, 0.125f, 0.85f, 0.66f);
    Graphics::DrawLine(aWallLeftX, aWallTopY, aWallRightX, aWallTopY);
    Graphics::DrawLine(aWallRightX, aWallTopY, aWallRightX, aWallBottomY);
    Graphics::DrawLine(aWallLeftX, aWallBottomY, aWallRightX, aWallBottomY);
    Graphics::DrawLine(aWallLeftX, aWallTopY, aWallLeftX, aWallBottomY);
    */
    
}

FTextureRect cOutsideFillRect;
void GameOverlayInterface::DrawOutside(float pWallTopY, float pWallRightX, float pWallBottomY, float pWallLeftX) {
    
    float aBaseStartU = gWadGameInterface.mSideFill.GetStartU();
    float aBaseStartV = gWadGameInterface.mSideFill.GetStartV();
    float aBaseEndU = gWadGameInterface.mSideFill.GetEndU();
    float aBaseEndV = gWadGameInterface.mSideFill.GetEndV();
    
    float aWidth = roundf(gWadGameInterface.mSideFill.mWidth);
    float aHeight = roundf(gWadGameInterface.mSideFill.mHeight);
    
    FTexture *aTexture = gWadGameInterface.mSideFill.mTexture;
    
    if (aTexture == NULL) { return; }
    if (aBaseStartU >= aBaseEndU) { return; }
    if (aBaseStartV >= aBaseEndV) { return; }
    
    float aLeftX = -2.0f * gSpriteDrawScale;
    float aRightX = mWidth + 2.0f * gSpriteDrawScale;
    float aBottomY = mHeight + 2.0f * gSpriteDrawScale;
    float aTopY = pWallTopY - 2.0f * gSpriteDrawScale;
    
    float aLeftInnerX = pWallLeftX;
    float aRightOuterX = pWallRightX;
    float aBottomInnerY = pWallBottomY;
    
    float aLefttU = 1.0f;
    float aRightU = 1.0f;
    
    float aBottomV = 1.0f;
    
    float aX = aLeftX;
    float aY = aTopY;
    
    float aNextX = 0.0f;
    float aNextY = 0.0f;
    
    float aPercentX = 0.0f;
    float aPercentY = 0.0f;
    
    //Find the left U
    while (aX < pWallLeftX) {
        aNextX = aX + aWidth;
        aLeftInnerX = aNextX;
        if (aNextX > pWallLeftX) {
            aPercentX = (pWallLeftX - aX) / (aNextX - aX);
            aLefttU = aBaseStartU + (aBaseEndU - aBaseStartU) * aPercentX;
            break;
        } else {
            
        }
        aX = aNextX;
    }
    
    //Find the right U
    aX = aLeftX;
    while (aX < pWallRightX) {
        aNextX = aX + aWidth;
        aRightOuterX = aNextX;
        if (aNextX > pWallRightX) {
            aPercentX = (pWallRightX - aX) / (aNextX - aX);
            aRightU = aBaseStartU + (aBaseEndU - aBaseStartU) * aPercentX;
            break;
        } else {
            
        }
        aX = aNextX;
    }
    
    //Find the bottom V
    while (aY < pWallBottomY) {
        aNextY = aY + aHeight;
        aBottomInnerY = aNextY;
        if (aNextY > pWallBottomY) {
            aPercentY = (pWallBottomY - aY) / (aNextY - aY);
            aBottomV = aBaseStartV + (aBaseEndV - aBaseStartV) * aPercentY;
            break;
        } else {
            
        }
        aY = aNextY;
    }
    
    aX = aLeftX;
    
    bool aBreak = false;
    while (true) {
        aNextX = aX + aWidth;
        aY = aTopY;
        while (aY < aBottomY) {
            aNextY = aY + aHeight;
            if (aNextX > pWallLeftX) {
                cOutsideFillRect.SetUVQuad(aBaseStartU, aBaseStartV, aLefttU, aBaseEndV);
                cOutsideFillRect.SetQuad(aX, aY, pWallLeftX, aNextY);
                Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
                aBreak = true;
            } else {
                cOutsideFillRect.SetUVQuad(aBaseStartU, aBaseStartV, aBaseEndU, aBaseEndV);
                cOutsideFillRect.SetQuad(aX, aY, aNextX, aNextY);
                Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
            }
         
            aY = aNextY;
        }
        aX = aNextX;
        if (aBreak) {
            break;
        }
    }
    
    //Now we draw the bottom stretch...
    aX = pWallLeftX;
    if ((aLefttU != 1.0f) && (aLeftInnerX != pWallLeftX)) {
        if ((aBottomV != 1.0f) && (aBottomInnerY != pWallBottomY)) {
            
            //Just cap the top Y here...
            cOutsideFillRect.SetUVQuad(aLefttU, aBottomV, aBaseEndU, aBaseEndV);
            cOutsideFillRect.SetQuad(pWallLeftX, pWallBottomY, aLeftInnerX, aBottomInnerY);
            Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
            aY = aBottomInnerY;
        } else {
            aY = pWallBottomY;
        }
        
        while (aY < aBottomY) {
            aNextY = aY + aHeight;
            cOutsideFillRect.SetUVQuad(aLefttU, aBaseStartV, aBaseEndU, aBaseEndV);
            cOutsideFillRect.SetQuad(pWallLeftX, aY, aLeftInnerX, aNextY);
            Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
            aY = aNextY;
        }
        aX = aLeftInnerX;
    }
    
    aBreak = false;
    while (true) {
        aNextX = aX + aWidth;
        if (aNextX > pWallRightX) {
            if ((aBottomV != 1.0f) && (aBottomInnerY != pWallBottomY)) {
                //Just cap the top Y here...
                cOutsideFillRect.SetUVQuad(aBaseStartU, aBottomV, aRightU, aBaseEndV);
                cOutsideFillRect.SetQuad(aX, pWallBottomY, pWallRightX, aBottomInnerY);
                Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
                aY = aBottomInnerY;
            } else {
                aY = pWallBottomY;
            }
            while (aY < aBottomY) {
                aNextY = aY + aHeight;
                cOutsideFillRect.SetUVQuad(aBaseStartU, aBaseStartV, aRightU, aBaseEndV);
                cOutsideFillRect.SetQuad(aX, aY, pWallRightX, aNextY);
                Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
                aY = aNextY;
            }
            aBreak = true;
        } else {
            
            if ((aBottomV != 1.0f) && (aBottomInnerY != pWallBottomY)) {
                //Just cap the top Y here...
                cOutsideFillRect.SetUVQuad(aBaseStartU, aBottomV, aBaseEndU, aBaseEndV);
                cOutsideFillRect.SetQuad(aX, pWallBottomY, aNextX, aBottomInnerY);
                Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
                aY = aBottomInnerY;
            } else {
                aY = pWallBottomY;
            }
            while (aY < aBottomY) {
                aNextY = aY + aHeight;
                cOutsideFillRect.SetUVQuad(aBaseStartU, aBaseStartV, aBaseEndU, aBaseEndV);
                cOutsideFillRect.SetQuad(aX, aY, aNextX, aNextY);
                Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
                aY = aNextY;
            }
        }
        aX = aNextX;
        if (aBreak) {
            break;
        }
    }
    

    
    //Now we draw the right, end-cap segment...
    aX = pWallRightX;
    if ((aRightU != 1.0f) && (pWallRightX != aRightOuterX)) {
        aY = aTopY;
        while (aY < aBottomY) {
            aNextY = aY + aHeight;
            cOutsideFillRect.SetUVQuad(aRightU, aBaseStartV, aBaseEndU, aBaseEndV);
            cOutsideFillRect.SetQuad(pWallRightX, aY, aRightOuterX, aNextY);
            Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
            aY = aNextY;
        }
        aX = aRightOuterX;
    }
    
    while (aX < aRightX) {
        aNextX = aX + aWidth;
        aY = aTopY;
        while (aY < aBottomY) {
            aNextY = aY + aHeight;
            cOutsideFillRect.SetUVQuad(aBaseStartU, aBaseStartV, aBaseEndU, aBaseEndV);
            cOutsideFillRect.SetQuad(aX, aY, aNextX, aNextY);
            Graphics::DrawSprite(cOutsideFillRect.mPositions, cOutsideFillRect.mTextureCoords, aTexture);
            aY = aNextY;
        }
        aX = aNextX;
    }
}

void GameOverlayInterface::NotifyProgressChanged() {
    
    //if (gInterfaceOverlay != NULL) { gInterfaceOverlay->NotifyProgressChanged(); }
    //if (mProgressWheel != NULL) {
    //    mProgressWheel->mProgress = gGame->mProgress;
    //    mProgressWheel->mProgressCount = gGame->mProgressCount;
    //}
    
}

void GameOverlayInterface::NotifyDidLoad() {
    NotifyProgressChanged();
    //if (mProgressWheel != NULL) {
    //    mProgressWheel->SnapToProgress();
    //}
    
}

void GameOverlayInterface::NotifyLivesChanged() {
    if (mTopBar != NULL) {
        mTopBar->RefreshLivesCount();
    }
}

bool GameOverlayInterface::HasAchievementBubble() {
    EnumList(FloatingAchievementBubble, aBubble, mAchievementBubbleList.mObjectList) {
        if (aBubble->mKill == 0) {
            return true;
        }
    }
    return false;
}

void GameOverlayInterface::SpawnAchievementBubble() {
    
    //FloatingAchievementBubble *aBubble = new FloatingAchievementBubble();
    
    //aBubble->mX = mWallLeftX + (mWallRightX - mWallLeftX) * 0.5f;
    //aBubble->mY = mWallTopY + 50.0f * gSpriteDrawScale;
    
    //mAchievementBubbleList.Add(aBubble);
}
