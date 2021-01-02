//
//  GameContainerContainer.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/10/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//
#include "core_includes.h"
#include "GameContainer.hpp"
#include "FAnimation.hpp"
#include "Game.hpp"
#include "CameraMenu.hpp"
#include "GameFlowTestMenu.hpp"
#include "GFXApp.hpp"
#include "GameMenu.hpp"

#include "NewLightConfigurationScene.hpp"

#include "LevelOverlay.hpp"
#include "Level.hpp"

#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

GameContainer *gGameContainer = NULL;

GameContainer::GameContainer() {
    
    gGameContainer = this;
    
    mName = "GameContainer";
    
    mRecievesOutsideTouches = true;
    
    mEffectsOverlay = NULL;
    mInterfaceOverlay = NULL;
    //mGameMenu = NULL;
    //mGameMenuAnimation = NULL;
    //mGameMenuAnimatingIn = false;
    
    
    
    mGameOverOverlay = NULL;
    mGameOverMenu = NULL;
    mGameOverMenuAnimation = NULL;
    mGameOverMenuAnimatingIn = false;
    
    mLightConfig = NULL;
    
    mLevelCompleteOverlay = NULL;
    mLevelCompleteMenu = NULL;
    mLevelCompleteMenuAnimation = NULL;
    mLevelCompleteMenuAnimatingIn = false;
    
    mLevelOverlay = NULL;
    
    mPlaybackSpeedCategory = GAME_PLAYBACK_NORMAL;
    mPlaybackSlowMotionTimer = 0;
    mPlaybackUpdateCount = 1;
    
    mEditorMenu = NULL;
    mEditorMenuUtils = NULL;
    mMenuSpeed = NULL;
    mMenuStats = NULL;
    mMenuStreaks = NULL;
    mMenuCamera = NULL;
    mMenuGameFlow = NULL;
    mMenuSectionNavigator = NULL;
    mMenuLUGO = NULL;
    
    
    
    
    mGameCorner1X = 0.0f;
    mGameCorner1Y = 0.0f;
    
    mGameCorner2X = 100.0f;
    mGameCorner2Y = 0.0f;
    
    mGameCorner3X = 100.0f;
    mGameCorner3Y = 100.0f;
    
    mGameCorner4X = 0.0f;
    mGameCorner4Y = 100.0f;
    
    
    SetWidth(1000.0f);
    SetHeight(1000.0f);
    
    mContainer = new FCanvas();
    mContainer->mDrawManual = true;
    //mContainer->mColor = FColor(0.25f, 1.0f, 1.0f, 0.66f);
    mContainer->SetTransformAnchor(0.5f, 0.5f);
    //mContainer->SetTransformRotation(2.0f);
    
    mBackgroundPaused = false;
    
    mPaused = false;
    mPauseFadeEnabled = false;
    
    mPauseFade = 0.0f;
    mPauseFadeMax = 0.8f;
    
    
    AddChild(mContainer);
    mContainer->mName = "GameContainer-InnerContainer";
    
    mGame = new Game();
    mContainer->AddChild(mGame);
    
    mEffectsOverlay = new GameOverlayEffects();
    mEffectsOverlay->mDrawManual = true;
    AddChild(mEffectsOverlay);
    
    
    mInterfaceOverlay = new GameOverlayInterface();
    mInterfaceOverlay->mDrawManual = true;
    AddChild(mInterfaceOverlay);
    
    
    
#ifdef LIGHT_CONFIG
    
    /*
    mLightConfig = new NewLightConfigurationScene(mGame, mGame->mCamera);
    gApp->mWindowTools.AddChild(mLightConfig);
    
    float aWidth = gDeviceHeight;
    if (aWidth > 580.0f) { aWidth = 580.0f; }
    
    mLightConfig->SetFrame(0.0f, 0.0f, aWidth, gDeviceHeight);
    */
    
#else
    
    #ifndef EDITOR_MODE
    
    /*
    if (gDeviceWidth > 800.0f) {
    
        float aSpeedMenuWidth = 470.0f;
        if (aSpeedMenuWidth > gDeviceWidth) { aSpeedMenuWidth = gDeviceWidth; }
        
        float aSpeedMenuHeight = 160.0f;
        mMenuSpeed = new SpeedControlMenu();
        gApp->mWindowTools.AddChild(mMenuSpeed);
        mMenuSpeed->SetFrame(gSafeAreaInsetLeft + 10.0f, gDeviceHeight - (gSafeAreaInsetBottom + aSpeedMenuHeight + 10.0f), aSpeedMenuWidth, aSpeedMenuHeight);
        
        
        mMenuGameFlow = new GameFlowTestMenu();
        gApp->mWindowTools.AddChild(mMenuGameFlow);
        float aFlowMenuHeight = 210.0f;
        float aFlowMenuWidth = 452.0f;
        mMenuGameFlow->SetFrame(gSafeAreaInsetLeft + 10.0f, gSafeAreaInsetTop + 30.0f, aFlowMenuWidth, aFlowMenuHeight);
        
        
        float aSectionNavMenuWidth = 470.0f;
        float aSectionNavMenuHeight = 112.0f;
        mMenuSectionNavigator = new SectionNavigatorMenu();
        gApp->mWindowTools.AddChild(mMenuSectionNavigator);
        mMenuSectionNavigator->SetFrame(gSafeAreaInsetLeft + 10.0f, gDeviceHeight2 - (aSectionNavMenuHeight / 2.0f) + 200.0f, aSectionNavMenuWidth, aSectionNavMenuHeight);
        
        float aLUGOMenuWidth = 470.0f;
        float aLUGOMenuHeight = 220.0f;
        mMenuLUGO = new LevelUpGameOverMenu();
        gApp->mWindowTools.AddChild(mMenuLUGO);
        mMenuLUGO->SetFrame(gDeviceWidth - (gSafeAreaInsetRight + 5.0f + aLUGOMenuWidth), gSafeAreaInsetTop + 10.0f, aLUGOMenuWidth, aLUGOMenuHeight);
        
        float aStreakMenuHeight = 238.0f;
        mMenuStreaks = new StreakMenu();
        gApp->mWindowTools.AddChild(mMenuStreaks);
        //mMenuStreaks->SetFrame(gSafeAreaInsetLeft + 16.0f, gDeviceHeight - (gSafeAreaInsetBottom + aStreakMenuHeight + 16.0f), 340.0f, aStreakMenuHeight);
        mMenuStreaks->SetFrame(gSafeAreaInsetLeft + 6.0f, gSafeAreaInsetTop + 250.0f, 340.0f, aStreakMenuHeight);
        
    }
    */
    
    #endif
    
#endif
    
    
    

    
}

GameContainer::~GameContainer() {

    if (gGameContainer == this) {
        gGameContainer = NULL;
    }
    
    if (gApp != NULL) {
        gApp->FlushModalMenuStack();
    }
    
    if (mLevelCompleteMenu != NULL) {
        mLevelCompleteMenu->Kill();
        mLevelCompleteMenu = NULL;
    }
    
    if (mLevelCompleteMenuAnimation != NULL) {
        delete mLevelCompleteMenuAnimation;
        mLevelCompleteMenuAnimation = NULL;
    }
    
    if (mGameOverMenu != NULL) {
        mGameOverMenu->Kill();
        mGameOverMenu = NULL;
    }
    
    if (mGameOverOverlay != NULL) {
        mGameOverOverlay = NULL;
    }
    
    if (mLevelCompleteOverlay != NULL) {
        mLevelCompleteOverlay = NULL;
    }
    
    //mLevelOverlay
    
    if (mGameOverMenuAnimation != NULL) {
        delete mGameOverMenuAnimation;
        mGameOverMenuAnimation = NULL;
    }
    
    if (mEditorMenu != NULL) {
        mEditorMenu->Kill();
        mEditorMenu = NULL;
    }
    
    if (mEditorMenuUtils != NULL) {
        mEditorMenuUtils->Kill();
        mEditorMenuUtils = NULL;
    }
    
    if (mMenuSpeed != NULL) {
        mMenuSpeed->Kill();
        mMenuSpeed = NULL;
    }
    
    if (mMenuStats != NULL) {
        mMenuStats->Kill();
        mMenuStats = NULL;
    }
    
    if (mMenuStreaks != NULL) {
        mMenuStreaks->Kill();
        mMenuStreaks = NULL;
    }
    
    if (mMenuCamera != NULL) {
        mMenuCamera->Kill();
        mMenuCamera = NULL;
    }
    
    if (mMenuSectionNavigator != NULL) {
        mMenuSectionNavigator->Kill();
        mMenuSectionNavigator = NULL;
    }
    
    if (mMenuGameFlow != NULL) {
        mMenuGameFlow->Kill();
        mMenuGameFlow = NULL;
    }
    
    if (mMenuLUGO != NULL) {
        mMenuLUGO->Kill();
        mMenuLUGO = NULL;
    }
    
    if (mLightConfig != NULL) {
        mLightConfig->Kill();
        mLightConfig = NULL;
    }
    
    mContainer = NULL;
    mEffectsOverlay = NULL;
    mInterfaceOverlay = NULL;
    mLevelOverlay = NULL;
}

void GameContainer::Layout() {
    
    SetFrame(0.0f, 0.0f, gAppWidth, gAppHeight);
    
    //mInterfaceLeftWidth = gSafeAreaInsetLeft;
    //mInterfaceRightWidth = gSafeAreaInsetRight;
    
    //Includes the safe area height.
    //mInterfaceTopHeight = gSafeAreaInsetTop + 60.0f * gSpriteDrawScale;
    //mInterfaceBottomHeight = gSafeAreaInsetBottom + 20.0f * gSpriteDrawScale;
    
    
    if ((mGame != NULL) && (mContainer != NULL) && (mInterfaceOverlay != NULL)) {
        
        float aTopBarHeight = 0.0f;
        float aBottomBarMinimumHeight = 4.0f * gSpriteDrawScale;
        float aBottomBarPreferredHeight = 16.0f * gSpriteDrawScale;
        
        float aMinimumPaddingLeft = gSafeAreaInsetLeft + 20.0f * gSpriteDrawScale;
        float aMinimumPaddingRight = gSafeAreaInsetRight + 20.0f * gSpriteDrawScale;
        
        TopBar *aTopBar = mInterfaceOverlay->mTopBar;
        if (aTopBar != NULL) {
            
            //float aMainHeight = roundf(gAppHeight - (gSafeAreaInsetTop));
            aTopBarHeight = aTopBar->GetRequiredHeight();
            
            float aAvailableGameHeight = roundf(gAppHeight - (aTopBarHeight + gSafeAreaInsetTop + aBottomBarMinimumHeight + gSafeAreaInsetBottom + gAdBannerHeight));
            
            if ((aAvailableGameHeight > 60.0f) && (gAppWidth > 60.0f)) {
                
                float aGameWidth = GAME_WIDTH;
                float aGameHeight = GAME_HEIGHT;
                
                float aScale = 1.0f;
                float aGameAreaWidth = mWidth - (aMinimumPaddingLeft + aMinimumPaddingRight);
                
                FRect aFrame = FRect(0.0f, 0.0f, aGameAreaWidth, aAvailableGameHeight);
                FRect aFit = FRect::FitAspectFit(aFrame, aGameWidth, aGameHeight, 0.0f, aScale);
                
                aFit.mX = roundf(aFit.mX);
                aFit.mY = roundf(aFit.mY);
                aFit.mWidth = roundf(aFit.mWidth);
                aFit.mHeight = roundf(aFit.mHeight);
                
                float aContainerWidth = roundf(aGameAreaWidth / aScale);
                float aContainerHeight = roundf(aAvailableGameHeight / aScale);
                
                float aConsumedHeight = roundf(((float)GAME_HEIGHT) * aScale + aTopBarHeight + aBottomBarMinimumHeight);
                
                //Assumption: aConsumedHeight <= aWiggleHeight
                
                float aOvershoot = roundf(gAppHeight - (aConsumedHeight + gSafeAreaInsetTop + gSafeAreaInsetBottom + gAdBannerHeight));
                
                float aBottomBarHeight = aBottomBarMinimumHeight;
                
                //Log("Fit Frame Bottom Bar Minimum Height: %f  Preferred Height: %f\n", aBottomBarMinimumHeight, aBottomBarPreferredHeight);
                
                
                //Correct the padding for bottom bar.
                if (aOvershoot > 0.0f) {
                    float aCorrection = aOvershoot;
                    if (aBottomBarPreferredHeight < aCorrection) {
                        aCorrection = aBottomBarPreferredHeight;
                    }
                    aOvershoot -= aCorrection;
                    aBottomBarHeight += aCorrection;
                    //Log("Fit Frame Bottom Bar Correction: %f\n", aCorrection);
                    
                }
                
                //Log("Fit Frame Bottom Bar CHOSEN Height: %f\n", aBottomBarHeight);
                
                
                
                
                //Center the screen now, based on overshoot...
                
                if (aOvershoot > 0) {
                    float aAdjustmentTop = roundf(aOvershoot * 0.5f);
                    float aAdjustmentBottom = roundf(aOvershoot - aAdjustmentTop);
                    aTopBarHeight += aAdjustmentTop;
                    aBottomBarHeight += aAdjustmentBottom;
                    
                    //Log("Fit Frame Top Adjustment: %f\n", aTopBarHeight);
                    //Log("Fit Frame Bottom Adjustment: %f\n", aBottomBarHeight);
                }
                
                aTopBarHeight += gSafeAreaInsetTop;
                
                aTopBar->SetFrame(0.0f, 0.0f, mWidth, aTopBarHeight);
                
                mGameCorner1X = roundf(mWidth2 - aGameWidth * aScale * 0.5f);
                mGameCorner1Y = roundf(aTopBarHeight);
                
                mGameCorner2X = roundf(mWidth2 + aGameWidth * aScale * 0.5f);
                mGameCorner2Y = roundf(aTopBarHeight);
                
                mGameCorner3X = roundf(mGameCorner2X);
                mGameCorner3Y = roundf(aTopBarHeight + aGameHeight * aScale);
                
                mGameCorner4X = roundf(mGameCorner1X);
                mGameCorner4Y = roundf(aTopBarHeight + aGameHeight * aScale);
                
                if (mInterfaceOverlay != NULL) {
                    mInterfaceOverlay->mGameCorner1X = mGameCorner1X;
                    mInterfaceOverlay->mGameCorner1Y = mGameCorner1Y;
                    
                    mInterfaceOverlay->mGameCorner2X = mGameCorner2X;
                    mInterfaceOverlay->mGameCorner2Y = mGameCorner2Y;
                    
                    mInterfaceOverlay->mGameCorner3X = mGameCorner3X;
                    mInterfaceOverlay->mGameCorner3Y = mGameCorner3Y;
                    
                    mInterfaceOverlay->mGameCorner4X = mGameCorner4X;
                    mInterfaceOverlay->mGameCorner4Y = mGameCorner4Y;
                }
                
                if (mEffectsOverlay != NULL) {
                    mEffectsOverlay->mGameCorner1X = mGameCorner1X;
                    mEffectsOverlay->mGameCorner1Y = mGameCorner1Y;
                    
                    mEffectsOverlay->mGameCorner2X = mGameCorner2X;
                    mEffectsOverlay->mGameCorner2Y = mGameCorner2Y;
                    
                    mEffectsOverlay->mGameCorner3X = mGameCorner3X;
                    mEffectsOverlay->mGameCorner3Y = mGameCorner3Y;
                    
                    mEffectsOverlay->mGameCorner4X = mGameCorner4X;
                    mEffectsOverlay->mGameCorner4Y = mGameCorner4Y;
                }
                
                
                float aCenterX = roundf(mGameCorner1X + (mGameCorner2X - mGameCorner1X) * 0.5f);
                float aCenterY = roundf(mGameCorner1Y + (mGameCorner3Y - mGameCorner1Y) * 0.5f);
                
                mContainer->SetTransformScale(aScale);
                mContainer->SetFrame(roundf(-(aContainerWidth / (2.0f))), - roundf((aContainerHeight / (2.0f))), aContainerWidth, aContainerHeight);
                
                //mContainer->SetTransformTranslate(aCenterX, aCenterY);
                
                mContainer->SetTransformTranslate(aCenterX, aCenterY);
                
                mGame->mRenderShiftX = -((aMinimumPaddingRight - aMinimumPaddingLeft) / 2.0f) / aScale;
                mGame->mRenderShiftY = -(((aBottomBarHeight + gSafeAreaInsetBottom + gAdBannerHeight) - aTopBarHeight) / 2.0f) / aScale;
                
                //mGame->mRenderShiftX = -((mGameCorner2X - mGameCorner1X) / 2.0f) / aScale;
                //mGame->mRenderShiftY = -((mGameCorner3Y - mGameCorner1Y) / 2.0f) / aScale;
                
                //mContainer->SetTransformTranslate(mWidth2 - (mInterfaceRightWidth - mInterfaceLeftWidth) / 2.0f, mHeight2 - (mInterfaceBottomHeight - mInterfaceTopHeight) / 2.0f);
                
                //mGame->mRenderShiftX = -((mInterfaceRightWidth - mInterfaceLeftWidth) / 2.0f) / aScale;
                //mGame->mRenderShiftY = -((mInterfaceBottomHeight - mInterfaceTopHeight) / 2.0f) / aScale;
                
                float aGameX = roundf(mContainer->mWidth2 - mGame->mWidth2);
                float aGameY = roundf(mContainer->mHeight2 - mGame->mHeight2);
                
                mGame->SetX(aGameX);
                mGame->SetY(aGameY);
                
                if (mEffectsOverlay != NULL) {
                    //float aContainerCenterX = (int)(mInterfaceLeftWidth + (aGameAreaWidth) * 0.5f);
                    //float aContainerCenterY = (int)(mInterfaceTopHeight + (aGameAreaHeight) * 0.5f);
                    //mEffectsOverlay->SetFrame(aContainerCenterX - mGame->mWidth2 * aScale, aContainerCenterY - mGame->mHeight2 * aScale, mGame->mWidth * aScale, mGame->mHeight * aScale);
                    
                    mEffectsOverlay->SetFrame(0.0f, 0.0f, mWidth, mHeight);
                    
                }
                
                if (mInterfaceOverlay != NULL) {
                    mInterfaceOverlay->SetFrame(0.0f, 0.0f, mWidth, mHeight);
                }
                
                if (mLevelCompleteOverlay != NULL) {
                    mLevelCompleteOverlay->SetFrame(mGameCorner1X, mGameCorner1Y, roundf(mGameCorner3X - mGameCorner1X), roundf(mGameCorner3Y - mGameCorner1Y));
                }
                
                if (mGameOverOverlay != NULL) {
                    mGameOverOverlay->SetFrame(mGameCorner1X, mGameCorner1Y, roundf(mGameCorner3X - mGameCorner1X), roundf(mGameCorner3Y - mGameCorner1Y));
                }
                
                
                
            } else {
                Log("??? - Screen Is Unreasonably Small ???");
                
                
            }
            
            
            
            
            
            //float gAdBannerWidth = 0.0f;
            //float gAdBannerHeight = 0.0f;

            
            
            
        }
    }
    
    
    /*
    if (mGame != NULL && mContainer != NULL) {
        
        float aScale = 1.0f;
        float aGameAreaWidth = mWidth - (mInterfaceLeftWidth + mInterfaceRightWidth);
        float aGameAreaHeight = mHeight - (mInterfaceTopHeight + mInterfaceBottomHeight);
        
        FRect aFrame = FRect(0.0f, 0.0f, aGameAreaWidth, aGameAreaHeight);
        FRect aFit = FRect::FitAspectFit(aFrame, mGame->mWidth, mGame->mHeight, 0.0f, aScale);
        float aContainerWidth = roundf(aGameAreaWidth / aScale);
        float aContainerHeight = roundf(aGameAreaHeight / aScale);
        mContainer->SetTransformScale(aScale);
        mContainer->SetFrame(roundf(-(aContainerWidth / (2.0f))), - roundf((aContainerHeight / (2.0f))), aContainerWidth, aContainerHeight);
        //mContainer->SetTransformTranslate(mWidth2 + mInterfaceLeftWidth * aScale, mHeight2 + mInterfaceTopHeight * aScale);
        
        //mContainer->SetTransformTranslate(mWidth2, mHeight2);
        
        mContainer->SetTransformTranslate(mWidth2 - (mInterfaceRightWidth - mInterfaceLeftWidth) / 2.0f, mHeight2 - (mInterfaceBottomHeight - mInterfaceTopHeight) / 2.0f);
        
        mGame->mRenderShiftX = -((mInterfaceRightWidth - mInterfaceLeftWidth) / 2.0f) / aScale;
        mGame->mRenderShiftY = -((mInterfaceBottomHeight - mInterfaceTopHeight) / 2.0f) / aScale;
        
        float aGameX = roundf(mContainer->mWidth2 - mGame->mWidth2);
        float aGameY = roundf(mContainer->mHeight2 - mGame->mHeight2);
        
        mGame->SetX(aGameX);
        mGame->SetY(aGameY);
        
        if (mEffectsOverlay != NULL) {
            float aContainerCenterX = (int)(mInterfaceLeftWidth + (aGameAreaWidth) * 0.5f);
            float aContainerCenterY = (int)(mInterfaceTopHeight + (aGameAreaHeight) * 0.5f);
            mEffectsOverlay->SetFrame(aContainerCenterX - mGame->mWidth2 * aScale, aContainerCenterY - mGame->mHeight2 * aScale, mGame->mWidth * aScale, mGame->mHeight * aScale);
        }
        
        if (mInterfaceOverlay != NULL) {
            mInterfaceOverlay->SetFrame(0.0f, 0.0f, mWidth, mHeight);
        }
        
        if (mLevelCompleteOverlay != NULL) {
            mLevelCompleteOverlay->SetFrame(5.0f, 5.0f, mWidth - 10.0f, mHeight - 10.0f);
        }
        
    }
    */
    
    if (mLevelOverlay != NULL) {
        mLevelOverlay->SetFrame(0.0f, 0.0f, mWidth, mHeight);
    }
    
    if (mLightConfig != NULL) {
        mLightConfig->SetFrame(0.0f, 0.0f, gDeviceWidth, gDeviceHeight);
    }    
}

void GameContainer::BaseUpdate() {
    
    if (mMenuSpeed != NULL) {
        gApp->mWindowTools.mRoot.mChildren.MoveToLast(mMenuSpeed);
    }
    if (mMenuGameFlow != NULL) {
        gApp->mWindowTools.mRoot.mChildren.MoveToLast(mMenuGameFlow);
    }
    if (mMenuSectionNavigator != NULL) {
        gApp->mWindowTools.mRoot.mChildren.MoveToLast(mMenuSectionNavigator);
    }
    
    if (mMenuLUGO != NULL) {
        gApp->mWindowTools.mRoot.mChildren.MoveToLast(mMenuLUGO);
    }
    if (mMenuStreaks != NULL) {
        gApp->mWindowTools.mRoot.mChildren.MoveToLast(mMenuStreaks);
    }
    
    
    if (mPauseFadeEnabled == true) {
        if (mPauseFade < mPauseFadeMax) {
            mPauseFade += 0.01325f;
            if (mPauseFade >= mPauseFadeMax) {
                mPauseFade = mPauseFadeMax;
            }
        }
    } else {
        if (mPauseFade > 0.0f) {
            mPauseFade -= 0.01325f;
            if (mPauseFade <= 0.0f) {
                mPauseFade = 0.0f;
                if (gApp->MenuStackClear() == true) {
                    Unpause();
                }
            }
        } else if (mPaused == true) {
            if (gApp->MenuStackClear() == true) {
                Unpause();
            }
        }
    }
    
    if ((mPaused == true) || (mBackgroundPaused == true)) {
        return;
    }
    
    if (mGameOverMenu != NULL) {
        if (mGameOverMenuAnimation != NULL) {
            mGameOverMenuAnimation->Update();
            if (mGameOverMenuAnimation->IsComplete()) {
                delete mGameOverMenuAnimation;
                mGameOverMenuAnimation = NULL;
                if (mGameOverMenuAnimatingIn == false) {
                    mGameOverMenu->Kill();
                    mGameOverMenu = NULL;
                } else {
                    mGameOverMenu->NotifyDidFinishAnimatingIn();
                }
            }
        }
    }
    
    if (mLevelCompleteMenu != NULL) {
        if (mLevelCompleteMenuAnimation != NULL) {
            mLevelCompleteMenuAnimation->Update();
            if (mLevelCompleteMenuAnimation->IsComplete()) {
                delete mLevelCompleteMenuAnimation;
                mLevelCompleteMenuAnimation = NULL;
                if (mLevelCompleteMenuAnimatingIn == false) {
                    mLevelCompleteMenu->Kill();
                    mLevelCompleteMenu = NULL;
                } else {
                    mLevelCompleteMenu->NotifyDidFinishAnimatingIn();
                }
            }
        }
    }
    
    if (mGame != NULL) {
        mPlaybackUpdateCount = 0;
        
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_XXX_SLOW) {
            mPlaybackSlowMotionTimer++;
            if (mPlaybackSlowMotionTimer >= 25) {
                mPlaybackSlowMotionTimer = 0;
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
                mPlaybackUpdateCount = 1;
            }
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_XX_SLOW) {
            mPlaybackSlowMotionTimer++;
            if (mPlaybackSlowMotionTimer >= 10) {
                mPlaybackSlowMotionTimer = 0;
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
                mPlaybackUpdateCount = 1;
            }
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_X_SLOW) {
            mPlaybackSlowMotionTimer++;
            if (mPlaybackSlowMotionTimer >= 5) {
                mPlaybackSlowMotionTimer = 0;
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
                mPlaybackUpdateCount = 1;
            }
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_SLOW) {
            mPlaybackSlowMotionTimer++;
            if (mPlaybackSlowMotionTimer >= 2) {
                mPlaybackSlowMotionTimer = 0;
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
                mPlaybackUpdateCount = 1;
            }
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_NORMAL) {
            FCanvas::BaseUpdate();
            mGame->UpdateMain();
            mPlaybackSlowMotionTimer = 0;
            mPlaybackUpdateCount = 1;
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_FAST) {
            for (int i=0;i<2;i++) {
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
            }
            mPlaybackUpdateCount = 2;
            mPlaybackSlowMotionTimer = 0;
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_X_FAST) {
            for (int i=0;i<5;i++) {
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
            }
            mPlaybackUpdateCount = 5;
            mPlaybackSlowMotionTimer = 0;
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_XX_FAST) {
            for (int i=0;i<25;i++) {
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
            }
            mPlaybackUpdateCount = 25;
            mPlaybackSlowMotionTimer = 0;
        }
        if (mPlaybackSpeedCategory == GAME_PLAYBACK_XXX_FAST) {
            for (int i=0;i<100;i++) {
                FCanvas::BaseUpdate();
                mGame->UpdateMain();
            }
            mPlaybackUpdateCount = 100;
            mPlaybackSlowMotionTimer = 0;
        }
    }
    
    
}

void GameContainer::Draw() {
    
    if (mContainer != NULL) { mContainer->DrawManual(); }
    if (mEffectsOverlay != NULL) { mEffectsOverlay->DrawManual(); }
    
    
    //if (mLevelCompleteOverlay != NULL) { mLevelCompleteOverlay->DrawManual(); }
    
    //mInterfaceOverlay
    
    float aRumbleX = 0.0f;
    float aRumbleY = 0.0f;
    if (gGame != NULL) {
        aRumbleX = gGame->mRumbleX;
        aRumbleY = gGame->mRumbleY;
    }
    
    
    if (mLevelCompleteOverlay != NULL) {
        mLevelCompleteOverlay->SetTransformTranslate(aRumbleX, aRumbleY);
        mLevelCompleteOverlay->DrawManual();
    }
    
    if (mGameOverOverlay != NULL) {
        mGameOverOverlay->SetTransformTranslate(aRumbleX, aRumbleY);
        mGameOverOverlay->DrawManual();
    }
    
    #ifndef EDITOR_MODE
    
    if (mInterfaceOverlay != NULL) {
        mInterfaceOverlay->SetTransformTranslate(aRumbleX, aRumbleY);
        mInterfaceOverlay->DrawManual();
    }
    
    if (mLevelCompleteOverlay != NULL) {
        mLevelCompleteOverlay->SetTransformTranslate(aRumbleX, aRumbleY);
        mLevelCompleteOverlay->DrawManualOver();
    }
    
    if (mGameOverOverlay != NULL) {
        mGameOverOverlay->SetTransformTranslate(aRumbleX, aRumbleY);
        mGameOverOverlay->DrawManualOver();
    }
    
    if (mInterfaceOverlay != NULL) {
        mInterfaceOverlay->SetTransformTranslate(aRumbleX, aRumbleY);
        mInterfaceOverlay->DrawTopBarManual();
    }
    
    #else
        
        if (gEditor == NULL) {
            if (mInterfaceOverlay != NULL) {
                mInterfaceOverlay->mHidden = false;
                mInterfaceOverlay->DrawManual();
            }
        } else {
            if (mInterfaceOverlay != NULL) {
                mInterfaceOverlay->mHidden = true;
            }
        }
        
    #endif
    
    if (mPauseFade > 0.0f) {
        DrawTransform();
        Graphics::PipelineStateSetShape2DAlphaBlending();
        Graphics::SetColor(0.0f, 0.0f, 0.0f, mPauseFade);
        //Graphics::SetColor();
        Graphics::DrawRect(0.0f, 0.0f, mWidth, mHeight);
        
        Graphics::SetColor();
        if (mBackgroundPaused == true) {
            Graphics::PipelineStateSetSpriteAlphaBlending();
            gWadGameInterface.mBackgroundPausedInfoText.Center(mWidth2, mHeight2);
        }
    }
    
    if (mGameOverMenu != NULL) {
        //mGameOverMenu->DrawManual();
    
    }
    
    if (mLevelCompleteMenu != NULL) {
        //mLevelCompleteMenu->DrawManual();
        
    }
    
}

void GameContainer::Draw3D() {
    if (mGame != NULL) {
        mGame->Draw3D();
    }
    
    if (mLightConfig != NULL) {
        mLightConfig->Draw3D();
    }
}

void GameContainer::TouchDown(float pX, float pY, void *pData) {
    
}

void GameContainer::TouchMove(float pX, float pY, void *pData) {
    
}

void GameContainer::TouchUp(float pX, float pY, void *pData) {
    
}

void GameContainer::TouchFlush() {
    
}

void GameContainer::KeyDown(int pKey) {
    
#ifndef RELEASE_READY
    bool aShift = gKeyDownShift;
    bool aCtrl = gKeyDownCtrl;
    bool aAlt = gKeyDownAlt;
    
    if (pKey == __KEY__E) {
#ifdef EDITOR_MODE
        if (aShift == false && aCtrl == true && aAlt == false) {
            if (mEditorMenu != NULL) {
                gApp->EditorTestSwitchToEditor();
            }
        }
#endif
    }
    
#endif
}

void GameContainer::KeyUp(int pKey) {
    
}

void GameContainer::Notify(void *pSender, const char *pNotification) {
    
}

//After the game + editor are ready to go.
void GameContainer::Realize() {
    
}

void GameContainer::ReorderOverlays() {
    
    
    if (mEffectsOverlay != NULL) { BringChildToFront(mEffectsOverlay); }
    if (mLevelOverlay != NULL) { BringChildToFront(mLevelOverlay); }
    if (mInterfaceOverlay != NULL) { BringChildToFront(mInterfaceOverlay); }
    
    if (mLevelCompleteOverlay != NULL) { BringChildToFront(mLevelCompleteOverlay); }
    if (mGameOverOverlay != NULL) { BringChildToFront(mGameOverOverlay); }
    
    if (mLevelCompleteMenu != NULL) { BringChildToFront(mLevelCompleteMenu); }
    if (mGameOverMenu != NULL) { BringChildToFront(mGameOverMenu); }
    
    //if (mGameMenu != NULL) { BringChildToFront(mGameMenu); }
    
}

void GameContainer::EnterBackground() {
    if ((mPaused == false) && (mBackgroundPaused == false) && (gGame != NULL)) {
        if ((gGame->mIsGameOver == false) && (gGame->mIsLevelComplete == false)) {
            if ((mLevelCompleteMenu == NULL) && (mGameOverMenu == NULL)) {
                mBackgroundPaused = true;
                mPauseFadeEnabled = true;
                mPauseFade = mPauseFadeMax;
            }
        }
    }
}

void GameContainer::EnterForeground() {
    
}

void GameContainer::ShowGameOverMenu() {
    
    gApp->FlushModalMenuStack();
    
    if (mGameOverMenuAnimation != NULL) {
        delete mGameOverMenuAnimation;
        mGameOverMenuAnimation = NULL;
    }
    
    if (mGameOverMenu != NULL) {
        mGameOverMenu->Kill();
        mGameOverMenu = NULL;
    }
    
    mGameOverMenuAnimatingIn = true;
    
    float aWidth = 360.0f * gSpriteDrawScale;
    
    mGameOverMenu = new GameOverMenu();
    mGameOverMenu->SetFrame(mWidth / 2.0f - gAppWidth2, mHeight / 2.0f - gAppHeight2, gAppWidth, gAppHeight);
    gApp->mWindowMain.AddChild(mGameOverMenu);
    
    mGameOverMenuAnimation = new FCanvasAnimation(mGameOverMenu);
    mGameOverMenuAnimation->mStartX = roundf(-aWidth - 36.0f * gSpriteDrawScale);
    mGameOverMenuAnimation->GenerateIn(114);
    mGameOverMenuAnimation->Update();
    
    ReorderOverlays();
    
    BeginPauseFadeIn();
    
    /*
    if (CanShowGameOverMenu() == false) {
        return;
    }
    
    if (mGameOverMenuAnimation != NULL) {
        delete mGameOverMenuAnimation;
        mGameOverMenuAnimation = NULL;
    }
    
    if (mGameMenu != NULL) {
        mGameMenu->Kill();
        mGameMenu = NULL;
    }
    
    mGameOverMenuAnimatingIn = true;
    
    mGameOverMenu = new GameOverMenu();
    mGameOverMenu->SetFrame(mWidth / 2.0f - gAppWidth2, gAppHeight / 2.0f - gAppHeight2, gAppWidth, gAppHeight);
    gApp->mWindowMain.AddChild(mGameOverMenu);
    
    mGameOverMenuAnimation = new FCanvasAnimation(mGameOverMenu);
    mGameOverMenuAnimation->mStartX = -(gVirtualDevWidth / 2.0f + gAppWidth2);
    mGameOverMenuAnimation->GenerateIn(114);
    mGameOverMenuAnimation->Update();
    
    ReorderOverlays();
    */
}

void GameContainer::ShowGameOverOverlay() {
    
    if (mGameOverOverlay != NULL) {
        mGameOverOverlay->Kill();
        mGameOverOverlay = NULL;
    }

    mGameOverOverlay = new GameOverOverlay();
    mGameOverOverlay->mDrawManual = true;

    AddChild(mGameOverOverlay);

    ReorderOverlays();
    Layout();
}

void GameContainer::RemoveGameOverMenu() {
    
    if (mGameOverMenuAnimation != NULL) {
        delete mGameOverMenuAnimation;
        mGameOverMenuAnimation = NULL;
    }
    if (mGameOverMenu == NULL) { return; }
    mGameOverMenu->NotifyDidBeginAnimatingOut();
    mGameOverMenuAnimatingIn = false;
    mGameOverMenuAnimation = new FCanvasAnimation(mGameOverMenu);
    mGameOverMenuAnimation->mTargetX = roundf(gVirtualDevWidth + 36.0f * gSpriteDrawScale);
    mGameOverMenuAnimation->GenerateOut(76);
    mGameOverMenuAnimation->Update();
}

void GameContainer::ShowLevelCompleteMenu() {
    
    gApp->FlushModalMenuStack();
    
    if (mLevelCompleteMenuAnimation != NULL) {
        delete mLevelCompleteMenuAnimation;
        mLevelCompleteMenuAnimation = NULL;
    }
    
    if (mLevelCompleteMenu != NULL) {
        mLevelCompleteMenu->Kill();
        mLevelCompleteMenu = NULL;
    }
    
    mLevelCompleteMenuAnimatingIn = true;
    
    float aWidth = 360.0f * gSpriteDrawScale;
    
    mLevelCompleteMenu = new LevelCompleteMenu();
    mLevelCompleteMenu->SetFrame(mWidth / 2.0f - gAppWidth2, mHeight / 2.0f - gAppHeight2, gAppWidth, gAppHeight);
    gApp->mWindowMain.AddChild(mLevelCompleteMenu);
    
    mLevelCompleteMenuAnimation = new FCanvasAnimation(mLevelCompleteMenu);
    mLevelCompleteMenuAnimation->mStartX = roundf(-aWidth - 36.0f * gSpriteDrawScale);
    mLevelCompleteMenuAnimation->GenerateIn(114);
    mLevelCompleteMenuAnimation->Update();
    
    ReorderOverlays();
    
    BeginPauseFadeIn();
    
}

void GameContainer::RemoveLevelCompleteMenu() {
    if (mLevelCompleteMenuAnimation != NULL) {
        delete mLevelCompleteMenuAnimation;
        mLevelCompleteMenuAnimation = NULL;
    }
    if (mLevelCompleteMenu == NULL) { return; }
    mLevelCompleteMenu->NotifyDidBeginAnimatingOut();
    mLevelCompleteMenuAnimatingIn = false;
    mLevelCompleteMenuAnimation = new FCanvasAnimation(mLevelCompleteMenu);
    mLevelCompleteMenuAnimation->mTargetX = roundf(gVirtualDevWidth + 36.0f * gSpriteDrawScale);
    mLevelCompleteMenuAnimation->GenerateOut(76);
    mLevelCompleteMenuAnimation->Update();
}

void GameContainer::ShowLevelCompleteOverlay() {
    
    if (mLevelCompleteOverlay != NULL) {
        mLevelCompleteOverlay->Kill();
        mLevelCompleteOverlay = NULL;
    }
    
    mLevelCompleteOverlay = new LevelCompleteOverlay();
    mLevelCompleteOverlay->mDrawManual = true;
    
    AddChild(mLevelCompleteOverlay);
    
    ReorderOverlays();
    Layout();
    
}



bool GameContainer::CanShowGameMenu() {
    
    if (mGame->mIsGameOver) {
        
        return false;
    }
    
    if (mGame->mIsLevelComplete) {
        
        return false;
    }
    
    if (mGameOverMenu != NULL) {
        
        return false;
    }
    
    if (mLevelCompleteMenu != NULL) {
        
        return false;
    }
    
    return true;
}

void GameContainer::Pause() {
    mPaused = true;
    mBackgroundPaused = false;
}

void GameContainer::Unpause() {
    mPaused = false;
    mBackgroundPaused = false;
}

void GameContainer::BeginPauseFadeIn() {
    mPauseFadeEnabled = true;
}

void GameContainer::BeginPauseFadeOut() {
    mPauseFadeEnabled = false;
}

bool GameContainer::IsPauseFadeInComplete() {
    return (mPauseFade >= mPauseFadeMax);
}

bool GameContainer::IsPauseFadeOutComplete() {
    if ((mPauseFadeEnabled == false) && (mPauseFade >= mPauseFadeMax)) {
        return true;
    }
    return false;
}

void GameContainer::OpenEditorTestMenus() {
    
    mEditorMenu = new EditorMenuGameTest();
    gApp->mWindowTools.AddChild(mEditorMenu);
    mEditorMenu->SetFrame(gSafeAreaInsetLeft + 14.0f, gSafeAreaInsetTop + 20.0f, 400.0f, 680.0f);
    
    mEditorMenuUtils = new EditorMenuGameTestUtils();
    gApp->mWindowTools.AddChild(mEditorMenuUtils);
    mEditorMenuUtils->SetFrame(gDeviceWidth - (gSafeAreaInsetRight + 14.0f + 400.0f), gSafeAreaInsetTop + 20.0f, 420.0f, 280.0f);
    
    float aSpeedMenuWidth = 510.0f;
    if (aSpeedMenuWidth > gDeviceWidth) { aSpeedMenuWidth = gDeviceWidth; }
    
    mMenuSpeed = new SpeedControlMenu();
    gApp->mWindowTools.AddChild(mMenuSpeed);
    float aSpeedMenuHeight = 160.0f;
    mMenuSpeed->SetFrame(12.0f, gDeviceHeight - (gSafeAreaInsetBottom + aSpeedMenuHeight + 12.0f), aSpeedMenuWidth, aSpeedMenuHeight);
    
    
    float aCameraMenuWidth = 520.0f;
    mMenuCamera = new CameraMenu(gGame->mCamera);
    gApp->mWindowTools.AddChild(mMenuCamera);
    mMenuCamera->SetFrame(gDeviceWidth - (gSafeAreaInsetRight + aCameraMenuWidth), gDeviceHeight - (530.0f + gSafeAreaInsetBottom), aCameraMenuWidth, 500.0f);
    
}

void GameContainer::SetUpForLevel(Level *pLevel) {
    
    if (mLevelOverlay != NULL) {
        mLevelOverlay->Kill();
        mLevelOverlay = NULL;
    }
    
    if (pLevel == NULL) {
        return;
    }
    
    mLevelOverlay = pLevel->GetOverlay();
    if (mLevelOverlay != NULL) {
        AddChild(mLevelOverlay);
        mLevelOverlay->SetFrame(0.0f, 0.0f, mWidth, mHeight);
        
        ReorderOverlays();
    }
    
}
