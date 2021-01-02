//
//  Game.cpp
//  MetalLearning
//
//  Created by Nicholas Raptis on 2/21/19.
//  Copyright © 2019 Nicholas Raptis. All rights reserved.
//

#include "core_includes.h"
#include "GFXApp.hpp"
#include "Game.hpp"
#include "GameContainer.hpp"
#include "FAnimation.hpp"
#include "StuckDart.hpp"
#include "EffectDartFadeStar.hpp"
#include "EffectBalloonBurst.hpp"
#include "LevelCollection.hpp"
#include "LevelOverlay.hpp"
#include "MissedGameObjectCluster.hpp"
#include "MusicManager.hpp"
#include "TimeClock.hpp"
#include "MetaData.hpp"
#include "StarsData.hpp"
#include "AchievementBubbleRenderer.hpp"

#include "FLine.hpp"

#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

Game *gGame = NULL;
FPointList gTrajectoryList;

Game::Game() {
    
    gGame = this;
    
    mInvulnerable = false;
    
    mRenderShiftX = 0.0f;
    mRenderShiftY = 0.0f;
    
    SetWidth(GAME_WIDTH);
    SetHeight(GAME_HEIGHT);
    SetTransformAnchor(0.5f, 0.5f);
    
    mLevelData = NULL;
    
    mDidLoad = false;
    
    mProgressCount = 0;
    mProgress = 0;
    
    mTime = 0;
    mTimeWhenSectionStarted = 0;
    mTimeWhenFreeLifeSpawned = 360;
    
    mLevelCompleteDelayTimer = 0;
    
    mCamera = new FloatingCamera();
    
    mCamera->mAspect = ((float)GAME_WIDTH) / ((float)GAME_HEIGHT);
    
    mRenderer = new GameRenderer(this, mCamera);
    
    mDartSpawnX = gDeviceWidth2;
    mDartSpawnY = gDeviceHeight2;
    
    mDartReleaseVelocityMin = 12.0f;
    mDartReleaseVelocityMax = 42.0f;
    
    mGravity = 0.24f;
    
    mPlayAreaTop = 0.0f;
    mPlayAreaRight = 0.0f;
    mPlayAreaBottom = 0.0f;
    mPlayAreaLeft = 0.0f;
    
    mGameAreaTop = 0.0f;
    mGameAreaRight = 0.0f;
    mGameAreaBottom = 0.0f;
    mGameAreaLeft = 0.0f;
    
    mOffScreenAreaTop = 0.0f;
    mOffScreenAreaRight = 0.0f;
    mOffScreenAreaBottom = 0.0f;
    mOffScreenAreaLeft = 0.0f;
    
    mHitZoneTop = 0.0f;
    mHitZoneRight = 0.0f;
    mHitZoneBottom = 0.0f;
    mHitZoneLeft = 0.0f;
    
    mSpawnZoneTop = 0.0f;
    mSpawnZoneRight = 0.0f;
    mSpawnZoneBottom = 0.0f;
    mSpawnZoneLeft = 0.0f;
    
    mPeekZoneTop = 0.0f;
    mPeekZoneRight = 0.0f;
    mPeekZoneLeft = 0.0f;
    
    mExitZoneTop = 0.0f;
    mExitZoneRight = 0.0f;
    mExitZoneLeft = 0.0f;
    
    mQuarterZoneTop = 0.0f;
    mQuarterZoneRight = 0.0f;
    mQuarterZoneLeft = 0.0f;
    mQuarterZoneBottom = 0.0f;
    
    mKillZoneTop = 0.0f;
    mKillZoneRight = 0.0f;
    mKillZoneBottom = 0.0f;
    mKillZoneLeft = 0.0f;
    
    mTrajectoryKillZoneTop = 0.0f;
    mTrajectoryKillZoneRight = 0.0f;
    mTrajectoryKillZoneBottom = 0.0f;
    mTrajectoryKillZoneLeft = 0.0f;
    
    mGameContainerBorderTop = 0.0f;
    mGameContainerBorderRight = 0.0f;
    mGameContainerBorderBottom = 0.0f;
    mGameContainerBorderLeft = 0.0f;
    
    mRumbleMagnitude = 0.0f;
    mRumbleX = 0.0f;
    mRumbleY = 0.0f;
    
    mCurrentDart = NULL;
    mCurrentDartRespawnTimer = 0;
    
    mDartPullbackRangeMin = 20.0f;
    mDartPullbackRangeMax = 250.0f;
    mDartPullbackRangeFingerMin = 20.0f;
    mDartPullbackRangeFingerMax = 250.0f;
    
    mDartTargetPullX = 0.0f;
    mDartTargetPullY = 0.0f;
    mDartPullX = 0.0f;
    mDartPullY = 0.0f;
    mDartTargetPullRotation = 0.0f;
    mDartPullRotation = 0.0f;
    mDartResetStartPullX = 0.0f;
    mDartResetStartPullY = 0.0f;
    mDartResetStartRotation = 0.0f;
    mDartTouch = NULL;
    mDartTouchStartX = 0.0f;
    mDartTouchStartY = 0.0f;
    mIsDartBeingPulled = false;
    mDartResetAnimation = false;
    mDartResetAnimationTick = 0;
    mDartResetAnimationTime = 200;
    
    mTrajectoryExists = false;
    mReleaseDirX = 0.0f;
    mReleaseDirY = 0.0f;
    mReleaseVelocity = 0.0f;
    mReleaseFactor = 0.0f;
    
    
    mScore = 0;
    
    
    mLivesMax = 6;
    mLives = mLivesMax;
    mLivesLost = 0;
    
    
    mMissCount = 0;
    
    mPopDelayBalloon = 0;
    mPopDelayFreeLife = 0;
    mPopDelayBlimp = 0;
    
    
    mPopSoundIndex = 0;
    mPopSoundAlternateTick = 1000;
    mTurtleKnockDownSoundDelay = 0;
    mKnockDownSoundDelay = 0;
    
    mIsLevelComplete = false;
    mLevelCompleteDidStartDriftAway = false;
    mLevelCompleteMode = 0;
    mLevelCompleteTimer = 0;
    
    mLevelCompleteDidCompleteDriftAway = false;
    
    mLevelCompleteDriftAwayTimer = 0;
    
    mIsGameOver = false;
    mGameOverMode = 0;
    mGameOverTimer = 0;
    mGameOverDidShowMenu = false;
    
    mGameOverBlimpSpawnTimer = 0;
    mGameOverBlimp = NULL;
    mGameOverDidSpawnBlimp = false;
    mGameOverDidStartDriftAway = false;
    mGameOverDidCompleteDriftAway = false;
    
    mFlashTipTimer = 100 + gRand.Get(100);
    
    mTrajectoryIsNarrow = false;
    
    mTrajectoryStartX = 0.0f;
    mTrajectoryStartY = 0.0f;
    
    mTrajectoryStartTargetX = 0.0f;
    mTrajectoryStartTargetY = 0.0f;
    
#ifdef EDITOR_MODE
    mInvulnerable = true;
#endif
    
    
    if (music_isPlaying()) {
        gMusicManager.FadeInLoop("game_loop.mp3", 120);
    } else {
        gMusicManager.Loop("game_loop.mp3");
    }
}

Game::~Game() {
    
    if (mGameOverBlimp != NULL) {
        delete mGameOverBlimp;
        mGameOverBlimp = NULL;
    }
    
    if (mCamera != NULL) {
        delete mCamera;
        mCamera = NULL;
    }
    
    if (mRenderer != NULL) {
        delete mRenderer;
        mRenderer = NULL;
    }
    
    mProgressCount = 0;
    mProgress = 0;
    
    Clear();
    
    mDartList.Free();
    mBalloonList.Free();
    mFreeLifeList.Free();
    mBrickHeadList.Free();
    mTurtleList.Free();
    mBombList.Free();
    mPlatformList.Free();
    mCannonList.Free();
    mStarList.Free();
    mPrizeBoxList.Free();
    mBlimpList.Free();
    
    if (mLevelData != NULL) {
        delete mLevelData;
        mLevelData = NULL;
    }
    
    if (gGame == this) {
        gGame = NULL;
    }
}

void Game::SetFrame(float pX, float pY, float pWidth, float pHeight) {
    FCanvas::SetFrame(pX, pY, pWidth, pHeight);
    
}

void Game::LayoutTransform() {
    
    if (mCamera) {
        mCamera->mAspect = gDeviceWidth / gDeviceHeight;
        
    }
    
    mDartTouch = NULL;
    
    //We re-compute the center of our game window...
    float aAbsoluteCenterX = gAppBase->mWindowMain.mRoot.mX + gAppBase->mWindowMain.mRoot.mWidth2;
    float aAbsoluteCenterY = gAppBase->mWindowMain.mRoot.mY + gAppBase->mWindowMain.mRoot.mHeight2;
    mCamera->mCenterOffset = FVec2(gDeviceWidth2 - aAbsoluteCenterX, gDeviceHeight2 - aAbsoluteCenterY);
    
    //float aGameWidth = mWidth - (gSafeAreaInsetLeft + gSafeAreaInsetRight);
    //float aGameHeight = mHeight - (gSafeAreaInsetTop + gSafeAreaInsetBottom);
    
    mDartSpawnX = mWidth2;
    mDartSpawnY = mHeight * 0.75f;
    
    
    float aSpawnShiftY = 314.0f;
    
    //if (aSpawnShiftY < 70.0f) aSpawnShiftY = 70.0f;
    //if (aSpawnShiftY > 480.0f) aSpawnShiftY = 480.0f;
    
    mDartPullbackGrabRangeSquared = 70.0f;
    //if (mDartPullbackGrabRangeSquared < 60.0f) {
    //    mDartPullbackGrabRangeSquared = 60.0f;
    //}
    mDartPullbackGrabRangeSquared = mDartPullbackGrabRangeSquared * mDartPullbackGrabRangeSquared;
    
    mDartPullbackRangeMin = aSpawnShiftY * 0.125f;
    if (mDartPullbackRangeMin < 12.0f) { mDartPullbackRangeMin = 12.0f; }
    if (mDartPullbackRangeMin > 28.0f) { mDartPullbackRangeMin = 28.0f; }
    
    mDartPullbackRangeMax = aSpawnShiftY * 0.65f;
    
    mDartPullbackRangeFingerMin = mDartPullbackRangeMin;
    mDartPullbackRangeFingerMax = aSpawnShiftY * 1.05f;
    
    
    float aScreenMinDimension = mWidth;
    if (mHeight < aScreenMinDimension) { aScreenMinDimension = mHeight; }
    
    float aScreenMaxDimension = mWidth;
    if (mHeight > aScreenMaxDimension) { aScreenMaxDimension = mHeight; }
    
    mCamera->mDistance = 20.0f;
    
    for (int aCalibrationLoops=0;aCalibrationLoops<2048;aCalibrationLoops++) {
        
        // The algebra for this conversion does not
        // seem to work out, se we calibrate the camera
        // by easing towards the best distance...
        
        float aTestSceneLeft = Convert2DXTo3D(0.0f);
        float aTestSceneRight = Convert2DXTo3D(mWidth);
        //float aExpectedWidth = 34.0f;
        float aExpectedWidth = 40.0f;
        
        float aActualWidth = (aTestSceneRight - aTestSceneLeft);
        if (aActualWidth < aExpectedWidth) {
            mCamera->mDistance += 0.05f;
        } else {
            mCamera->mDistance -= 0.05f;
        }
    }
    
    //Keep in mind, the hit zone we are expanding based on screen size...
    float aHitZonePaddingH = 12.0f;
    float aHitZonePaddingV = 60.0f;
    
    mHitZoneTop = Convert2DYTo3D(-aHitZonePaddingV);
    mHitZoneRight = Convert2DXTo3D(mWidth + aHitZonePaddingH);
    mHitZoneBottom = Convert2DYTo3D(mHeight + aHitZonePaddingV);
    mHitZoneLeft = Convert2DXTo3D(-aHitZonePaddingH);
    
    mGameAreaTop = 0.0f;
    mGameAreaRight = mWidth;
    mGameAreaBottom = mHeight;
    mGameAreaLeft = 0.0f;
    
    
    float aPlayAreaPadding = mWidth;
    if (mHeight < mWidth) aPlayAreaPadding = mHeight;
    
    aPlayAreaPadding = aPlayAreaPadding * 0.065f;
    
    if (aPlayAreaPadding < 24.0f) {
        aPlayAreaPadding = 24.0f;
    }
    
    mPlayAreaTop = aPlayAreaPadding;
    mPlayAreaRight = (mWidth) - aPlayAreaPadding;
    mPlayAreaBottom = mDartSpawnY - aPlayAreaPadding;
    mPlayAreaLeft = aPlayAreaPadding;
    
    
    float aOffScreenZonePadding = 40.0f;
    mOffScreenAreaTop = -aOffScreenZonePadding;
    mOffScreenAreaRight = mWidth + aOffScreenZonePadding;
    mOffScreenAreaBottom = mHeight + aOffScreenZonePadding;
    mOffScreenAreaLeft = -aOffScreenZonePadding;
    
    
    float aSpawnZonePadding = 100.0f;
    mSpawnZoneTop = -aSpawnZonePadding;
    mSpawnZoneRight = mWidth + aSpawnZonePadding;
    mSpawnZoneBottom = mHeight;
    mSpawnZoneLeft = -aSpawnZonePadding;
    
    float aPeekZonePaddingTop = 88.0f;
    float aPeekZonePaddingSides = 60.0f;
    mPeekZoneTop = aPeekZonePaddingTop;
    mPeekZoneRight = mWidth - aPeekZonePaddingSides;
    mPeekZoneLeft = aPeekZonePaddingSides;
    
    float aQuarterZoneHeight = mPlayAreaBottom;
    mQuarterZoneTop = aQuarterZoneHeight / 4.0f;
    mQuarterZoneRight = mWidth - mWidth / 4.0f;
    mQuarterZoneLeft = mWidth / 4.0f;
    mQuarterZoneBottom = mPlayAreaBottom - aQuarterZoneHeight / 4.0f;
    
    float aExitZonePaddingTop = aPeekZonePaddingTop + 120.0f;
    float aExitZonePaddingSides = aPeekZonePaddingSides + 120.0f;
    mExitZoneTop = -aExitZonePaddingTop;
    mExitZoneRight = mWidth + aExitZonePaddingSides;
    mExitZoneLeft = -aExitZonePaddingSides;
    
    float aKillZonePaddingTop = ((float)GAME_HEIGHT);
    float aKillZonePaddingBottom = 60.0f;
    float aKillZonePaddingSides = 220.0f;
    mKillZoneTop = -aKillZonePaddingTop;
    mKillZoneRight = mWidth + aKillZonePaddingSides;
    mKillZoneBottom = mHeight + aKillZonePaddingBottom;
    mKillZoneLeft = -aKillZonePaddingSides;
    
    float mTrajectoryKillZonePadding = 20.0f;
    float mTrajectoryKillZonePaddingTop = 200.0f;
    
    mTrajectoryKillZoneTop = -mTrajectoryKillZonePaddingTop;
    mTrajectoryKillZoneRight = GAME_WIDTH - mTrajectoryKillZonePadding;
    mTrajectoryKillZoneBottom = GAME_HEIGHT - mTrajectoryKillZonePadding;
    mTrajectoryKillZoneLeft = mTrajectoryKillZonePadding;
    
    if (gGameContainer != NULL) {
        
        mGameContainerBorderTop = 0.0f;
        mGameContainerBorderRight = gGameContainer->mWidth;
        mGameContainerBorderBottom = gGameContainer->mHeight;
        mGameContainerBorderLeft = 0.0f;
        
        FVec2 aPos1 = FCanvas::Convert(mGameContainerBorderLeft, mGameContainerBorderTop, gGameContainer, this);
        FVec2 aPos2 = FCanvas::Convert(mGameContainerBorderRight, mGameContainerBorderBottom, gGameContainer, this);
        
        mGameContainerBorderLeft = aPos1.mX;
        mGameContainerBorderTop  = aPos1.mY;
        mGameContainerBorderRight = aPos2.mX;
        mGameContainerBorderBottom = aPos2.mY;
        
        mGameContainerBorderTop = Convert2DYTo3D(mGameContainerBorderTop);
        mGameContainerBorderRight = Convert2DXTo3D(mGameContainerBorderRight);
        mGameContainerBorderBottom = Convert2DYTo3D(mGameContainerBorderBottom);
        mGameContainerBorderLeft = Convert2DXTo3D(mGameContainerBorderLeft);
        
    }
    
    if (mRenderer) {
        mRenderer->ScreenBoundaryRefresh();
    }
}

void Game::Layout() {
    
}

void Game::Update() {
    
}

void Game::UpdateMain() {
    
#ifdef SCREENSHOT_MODE
    if (gGameContainer != NULL) {
        if (gGameContainer->mPaused == true) {
            return;
        }
    }
#endif
    
    
    
    if ((mIsGameOver == false) && (mIsLevelComplete == false)) {
        mTime++;
    }
    
    if (mPopSoundAlternateTick < 1000) {
        mPopSoundAlternateTick++;
    }
    
    if (mPopDelayBalloon > 0) { --mPopDelayBalloon; }
    if (mPopDelayFreeLife > 0) { --mPopDelayFreeLife; }
    if (mPopDelayBlimp > 0) { --mPopDelayBlimp; }
    
    
    
    
    
    EnumList(MissedGameObjectCluster, aCluster, mMissedObjectClusterList) {
        aCluster->mTick--;
        if (aCluster->mTick <= 0) {
            //Log("Cluster expired [%p] with %d objects from [%f, %f]\n", aCluster, aCluster->mCount, aCluster->GetCenterX(), aCluster->GetCenterY());
            mMissedObjectClusterKillList.Add(aCluster);
        }
    }
    
    EnumList(MissedGameObjectCluster, aCluster, mMissedObjectClusterKillList) {
        int aLivesLost = aCluster->mCount;
        if ((mIsGameOver == false) && (mIsLevelComplete == false)) {
            LifeLose(aLivesLost);
            mMissCount += aLivesLost;
            float aX = aCluster->GetCenterX();
            float aY = aCluster->GetCenterY();
            if (gEffectsOverlay != NULL) {
                gEffectsOverlay->Get2DPos(aX, aY);
                gEffectsOverlay->SpawnLifeBubble(aX, aY, false, aLivesLost);
            }
        }
        mMissedObjectClusterList.Remove(aCluster);
        mMissedObjectClusterQueueList.Add(aCluster);
    }
    
    mMissedObjectClusterKillList.RemoveAll();
    
    if (mRumbleMagnitude > 0.0f) {
        mRumbleMagnitude *= 0.985f;
        mRumbleMagnitude -= 0.075f;
        if (mRumbleMagnitude > 0.0f) {
            RumbleRefresh();
        } else {
            mRumbleMagnitude = 0.0f;
            mRumbleX = 0.0f;
            mRumbleY = 0.0f;
        }
        
    } else {
        mRumbleMagnitude = 0.0f;
        mRumbleX = 0.0f;
        mRumbleY = 0.0f;
    }
    
    bool aCanFlashTip = true;
    
    if (mLevelData != NULL) {
        if (gApp->mLevelID == LearningBasics) {
            if (mLevelData->mCurrentSectionIndex <= 0) {
                aCanFlashTip = false;
            }
        }
    }
    
    if ((mIsGameOver == true) || (mIsLevelComplete == true)) {
        aCanFlashTip = false;
    }
    
    mFlashTipTimer--;
    if (mFlashTipTimer <= 0) {
        if (mCurrentDart != NULL) {
            if (mCurrentDart->mFlashingTip == false) {
                if (aCanFlashTip == true) {
                    mCurrentDart->FlashTip();
                }
                mFlashTipTimer = gRand.Get(400) + gRand.Get(2000) + 200;
            }
        }
        if (mFlashTipTimer == 0) {
            mFlashTipTimer = 200 + gRand.Get(200);
        }
    }
    
    
    if (mTurtleKnockDownSoundDelay > 0) { mTurtleKnockDownSoundDelay--; }
    if (mKnockDownSoundDelay > 0) { mKnockDownSoundDelay--; }
    
    if (mLevelData != NULL) {
        if (mDidLoad == true) {
            if ((mIsGameOver == false) && (mIsLevelComplete == false)) {
                mLevelData->Update();
                
                if ((gInterfaceOverlay != NULL) && (gInterfaceOverlay->mTimeClock != NULL)) {
                    TimeClock *aClock = gInterfaceOverlay->mTimeClock;
                    
                    int aTime = mLevelData->mClockTime;
                    int aTicks = mLevelData->mClockTicks;
                    int aFlyInTicksRemaining = mLevelData->mClockFlyingInTicksRemaining;
                    
                    if (mLevelData->mClockMode == Uninitialized) { aClock->SetModeUninitialized(); }
                    if (mLevelData->mClockMode == FlyingIn) { aClock->SetModeFlyingIn(aTime, aFlyInTicksRemaining); }
                    if (mLevelData->mClockMode == FlyingOut) { aClock->SetModeFlyingOut(); }
                    if (mLevelData->mClockMode == CountingDown) { aClock->SetModeCountingDown(aTime, aTicks); }
                    if (mLevelData->mClockMode == AllSectionsCompleted) { aClock->SetModeAllSectionsCompleted(); }
                }
            }
        }
    }
    
    if (mDartResetAnimation) {
        mDartResetAnimationTick += 1;
        if (mDartResetAnimationTick >= mDartResetAnimationTime) {
            ResetDartTouch();
        } else {
            float aAnimationPercent = ((float)mDartResetAnimationTick) / ((float)mDartResetAnimationTime);
            float aSnapbackPercent = FAnimation::EaseOutBack(aAnimationPercent, 0.25f);
            aSnapbackPercent = 1.0f - aSnapbackPercent;
            
            mDartPullX = mDartResetStartPullX * aSnapbackPercent;
            mDartPullY = mDartResetStartPullY * aSnapbackPercent;
            
            float aRotationPercent = FAnimation::EaseOutBack(aAnimationPercent, 0.25f);
            mDartPullRotation = mDartResetStartRotation + mDartResetRotationLength * aRotationPercent;
        }
    }
    
    if (mCurrentDart) {
        mCurrentDart->Update();
    } else {
        if (--mCurrentDartRespawnTimer <= 0) {
            mCurrentDartRespawnTimer = 0;
            mCurrentDart = new Dart();
            
            mCurrentDart->mTransform.mX = mDartSpawnX;
            mCurrentDart->mTransform.mY = mDartSpawnY;
            
            mCurrentDart->SpawnAnimation();
        }
    }

    mDartList.Update();
    mBalloonList.Update();
    mFreeLifeList.Update();
    mBrickHeadList.Update();
    mBombList.Update();
    mTurtleList.Update();
    mPlatformList.Update();
    mCannonList.Update();
    mStarList.Update();
    mPrizeBoxList.Update();
    mBlimpList.Update();
    if ((mGameOverBlimp != NULL) && (mGameOverBlimp->mKill == 0)) {
        mGameOverBlimp->UpdateGameOverSequence();
        mGameOverBlimp->Update();
    }
    
    if ((mIsGameOver == false) && (mIsLevelComplete == false)) {
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationTurtlesPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                    DartMovingInterpolationTurtles(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                DartMovingInterpolationTurtles(aDart, 1.0, true);
            }
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationPlatformsPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                    DartMovingInterpolationPlatforms(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                DartMovingInterpolationPlatforms(aDart, 1.0, true);
            }
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationCannonsPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                    DartMovingInterpolationCannons(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                DartMovingInterpolationCannons(aDart, 1.0, true);
            }
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationBrickHeadsPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false) && (aDart->mIsFacingAway == false)) {
                    DartMovingInterpolationBrickHeads(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false) && (aDart->mIsFacingAway == false)) {
                DartMovingInterpolationBrickHeads(aDart, 1.0, true);
            }
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationBombsPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false) && (aDart->mIsFacingAway == false)) {
                    DartMovingInterpolationBombs(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false) && (aDart->mIsFacingAway == false)) {
                DartMovingInterpolationBombs(aDart, 1.0, true);
            }
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationBalloonsPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                    DartMovingInterpolationBalloons(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                DartMovingInterpolationBalloons(aDart, 1.0, true);
            }
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            DartMovingInterpolationFreeLivesPrepare(aDart);
            for (float aPercent=0.0f;aPercent<1.0f;aPercent+=0.05f) {
                if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                    DartMovingInterpolationFreeLives(aDart, aPercent, false);
                }
            }
            if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                DartMovingInterpolationFreeLives(aDart, 1.0, true);
            }
        }
    }
        
    //Move dart to final position, if still flying...
    EnumList(Dart, aDart, mDartList.mObjectList) {
        if ((aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false)) {
            aDart->mTransform.mX = aDart->mUpdateInterpEndX;
            aDart->mTransform.mY = aDart->mUpdateInterpEndY;
            aDart->mTransform.mRotation = aDart->mUpdateInterpEndRotation;
            aDart->GetTipPoint(aDart->mTipX, aDart->mTipY);
        }
    }
    
    if (mDartTouch != NULL && mDartResetAnimation == false) {
        float aDiffX = mDartTargetPullX - mDartPullX;
        float aDiffY = mDartTargetPullY - mDartPullY;
        float aLength = aDiffX * aDiffX + aDiffY * aDiffY;
        float aLengthFactor = 0.175f;
        if (aLength > SQRT_EPSILON) {
            aLength = sqrtf(aLength);
            aDiffX /= aLength;
            aDiffY /= aLength;
            float aMoveDist = aLength / 7.0f + 3.5f;
            if (aMoveDist >= aLength) {
                mDartPullX = mDartTargetPullX;
                mDartPullY = mDartTargetPullY;
            } else {
                mDartPullX += aDiffX * aMoveDist;
                mDartPullY += aDiffY * aMoveDist;
            }
            aLengthFactor = aLength / (mDartPullbackRangeMin * 2.0f);
            if (aLengthFactor > 1.5f) {
                aLengthFactor = 1.5f;
            }
            
        } else {
            mDartPullX = mDartTargetPullX;
            mDartPullY = mDartTargetPullY;
        }
        
        float aAngleDiff = DistanceBetweenAngles(mDartPullRotation, mDartTargetPullRotation);
        if (aAngleDiff > 2.0f) {
            mDartPullRotation += 1.0f;
        }
        if (aAngleDiff < -2.0f) {
            mDartPullRotation -= 1.0f;
        }
        aAngleDiff = DistanceBetweenAngles(mDartPullRotation, mDartTargetPullRotation);
        mDartPullRotation += aAngleDiff * 0.32f * aLengthFactor;
    }
    
    //So... to correct perspective distortion for rotation, we need to convert
    //our pull vector into "scene" coords, and adjust the rotation accordingly...
    
    
    if (mCurrentDart) {
        mCurrentDart->mTransform.mX = mDartSpawnX + mDartPullX;
        mCurrentDart->mTransform.mY = mDartSpawnY + mDartPullY;
        mCurrentDart->mTransform.mRotation = mDartPullRotation;
    }
    
    if ((mDartTouch != NULL) && (mIsDartBeingPulled == true) && (mCurrentDart != NULL) && (mTrajectoryList.mCount > 0)) {
        float aTrajectoryDiffX = (mTrajectoryStartTargetX - mTrajectoryStartX);
        float aTrajectoryDiffY = (mTrajectoryStartTargetY - mTrajectoryStartY);
        
        float aTrajectoryDiffLen = aTrajectoryDiffX * aTrajectoryDiffX + aTrajectoryDiffY * aTrajectoryDiffY;
        
        if (aTrajectoryDiffLen > SQRT_EPSILON) {
            
            aTrajectoryDiffLen = sqrtf(aTrajectoryDiffLen);
            aTrajectoryDiffX /= aTrajectoryDiffLen;
            aTrajectoryDiffY /= aTrajectoryDiffLen;
            
            //float aShiftAmount = (aTrajectoryDiffLen * 0.085f) + 0.2f * gSpriteDrawScale;
            //float aShiftAmount = (aTrajectoryDiffLen * 0.05f) + 0.05f * gSpriteDrawScale;
            float aShiftAmount = (aTrajectoryDiffLen * 0.05f) + 0.05f;
            
            //float aShiftAmount = 0.01f;
            
            if (aShiftAmount > aTrajectoryDiffLen) {
                mTrajectoryStartX = mTrajectoryStartTargetX;
                mTrajectoryStartY = mTrajectoryStartTargetY;
                
            } else {
                mTrajectoryStartX = mTrajectoryStartX + aTrajectoryDiffX * aShiftAmount;
                mTrajectoryStartY = mTrajectoryStartY + aTrajectoryDiffY * aShiftAmount;
            }
        } else {
            mTrajectoryStartX = mTrajectoryStartTargetX;
            mTrajectoryStartY = mTrajectoryStartTargetY;
            
        }
        
        float aDiffX = mTrajectoryStartX - mDartTouchStartX;
        float aDiffY = mTrajectoryStartY - mDartTouchStartY;
        
        float aLength = aDiffX * aDiffX + aDiffY * aDiffY;
        if (aLength > SQRT_EPSILON) {
            aLength = sqrtf(aLength);
            if (aLength > mDartPullbackRangeFingerMax) {
                aDiffX /= aLength;
                aDiffY /= aLength;
                aDiffX *= mDartPullbackRangeFingerMax;
                aDiffY *= mDartPullbackRangeFingerMax;
            }
        }
        CorrectFingerRange(aDiffX, aDiffY);
        ComputeTrajectory(aDiffX, aDiffY);
    }
    
    if (mIsGameOver) {
        UpdateGameOver();
    }
    if (mIsLevelComplete) {
        UpdateLevelComplete();
    }
    
    if (mInvulnerable == false) {
        if (ShouldBeginGameOver()) {
            BeginGameOver();
        }
    }
    
    if (ShouldBeginLevelComplete()) {
        if (mLevelCompleteDelayTimer == 0) {
            mLevelCompleteDelayTimer = 7;
        } else {
            mLevelCompleteDelayTimer--;
            if (mLevelCompleteDelayTimer <= 0) {
                BeginLevelComplete();
            }
        }
    }
    
    mWind.Update();
    
    if (mRenderer != NULL) {
        mRenderer->Update();
    }
    
}

void Game::Draw() {
    
    
    Graphics::PipelineStateSetShape2DAlphaBlending();
    #ifdef EDITOR_MODE
        if (gEditor == NULL) {
            Graphics::SetColor(0.825f, 0.5f, 0.35f, 0.85f);
            Graphics::OutlineRect(0.0f, 0.0f, mWidth, mHeight, 200.0f);
        }
    #else
    
    #endif
    
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    Graphics::SetColor();
    
    mBrickHeadList.Draw();
    mBalloonList.Draw();
    mFreeLifeList.Draw();
    mDartList.Draw();
    mTurtleList.Draw();
    mBombList.Draw();
    
    mPlatformList.Draw();
    mCannonList.Draw();
    mStarList.Draw();
    mPrizeBoxList.Draw();
    mBlimpList.Draw();
    
    if (mGameOverBlimp != NULL) { mGameOverBlimp->Draw(); }
    
    
    if (mCurrentDart) {
        mCurrentDart->Draw();
    }
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    Graphics::SetColor();
    //
    
    /*
     FSprite *aDart = &(gApp->mRay[3]);
     if (mIsDartBeingPulled) Graphics::SetColor(1.0f, 0.5f, 0.5f, 1.0f);
     aDart->Draw(mDartSpawnX + mDartPullX, mDartSpawnY + mDartPullY,
     1.5f, mDartPullRotation + 180.0f);
     */
    
    
    /*
     float aIndicatorCenterX = mDartSpawnX - mWidth * 0.1f;
     float aIndicatorCenterY = mDartSpawnY;
     
     float aIndicatorWidth = 30.0f;
     float aIndicatorHeight = 100.0f;
     
     float aIndicatorX = aIndicatorCenterX - (aIndicatorWidth / 2.0f);
     float aIndicatorY = aIndicatorCenterY - (aIndicatorHeight / 2.0f);
     
     Graphics::PipelineStateSetShape2DNoBlending();
     
     Graphics::SetColor(0.66f, 0.66f, 0.66f, 0.35f);
     Graphics::DrawRect(aIndicatorX - 2.0f, aIndicatorY - 2.0f, aIndicatorWidth + 4.0f, aIndicatorHeight + 4.0f);
     
     if (mIsDartBeingPulled) {
     Graphics::SetColor(0.045f, 0.045f, 0.0626f, 0.45f);
     } else {
     Graphics::SetColor(0.045f, 0.045f, 0.1226f, 0.45f);
     }
     Graphics::DrawRect(aIndicatorX, aIndicatorY, aIndicatorWidth, aIndicatorHeight);
     
     float aPullLength = mDartPullX * mDartPullX + mDartPullY * mDartPullY;
     float aPullPercent = 0.0f;
     if (aPullLength > SQRT_EPSILON) {
     aPullLength = (float)(sqrtf(aPullLength));
     aPullPercent = (aPullLength - mDartPullbackRangeMin) / (mDartPullbackRangeMax - mDartPullbackRangeMin);
     }
     aPullPercent *= 1.10f;
     if (aPullPercent > 1.0f) aPullPercent = 1.0f;
     if (aPullPercent < 0.0f) aPullPercent = 0.0f;
     aPullPercent = FAnimation::EaseInCirc(aPullPercent) * 0.5f + aPullPercent * 0.5f;
     
     if (mIsDartBeingPulled) {
     Graphics::SetColor(1.0f, 0.35f, 0.25f, 0.525f);
     float aPowerBarHeight = aPullPercent * aIndicatorHeight;
     float aPowerBarY = (aIndicatorY + aIndicatorHeight) - aPowerBarHeight;
     Graphics::DrawRect(aIndicatorX, aPowerBarY, aIndicatorWidth, aPowerBarHeight);
     } else {
     
     }
     */
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    Graphics::SetColor(0.75f);
    if (mLevelData != NULL) {
        mLevelData->DrawDeadObjectMarkers();
    }
    Graphics::SetColor();
    
    
    Graphics::PipelineStateSetShape2DAlphaBlending();
    
    if (mTrajectoryIsNarrow) {
        if (gApp->mDarkMode) {
            Graphics::SetColor(1.0f, 0.25f, 0.25f, 1.0f);
        } else {
            Graphics::SetColor(1.0f, 0.25f, 0.25f, 0.25f);
        }
    } else {
        if (gApp->mDarkMode) {
            Graphics::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        } else {
            Graphics::SetColor(1.0f, 1.0f, 1.0f, 0.25f);
        }
    }
    
    
    
    /*
    for (int i=0;i<mTrajectoryList.mCount;i++) {
        float aX = mTrajectoryList.mX[i];
        float aY = mTrajectoryList.mY[i];
        Graphics::DrawPoint(aX, aY, 3.0f);
    }
    */
    
    DrawTransform();
    
}

void Game::Draw3D() {
    if (mRenderer != NULL) {
        mRenderer->Draw3D();
    }
}

void Game::DrawDeadObjectMarker(float pX, float pY) {
    if ((mIsLevelComplete == false) && (mIsGameOver == false)) {

        if (gSpriteDrawScale >= 2.5f) {
            gWadGameInterface.mDeadObjectMarkerCircle.Draw(pX, pY, 0.75f * 0.5f);
        } else {
            gWadGameInterface.mDeadObjectMarkerCircle.Draw(pX, pY, 0.75f);
        }

    }
}

void Game::TouchDown(float pX, float pY, void *pData) {
    
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        Log("Intercept Touch\n");
        return;
    }
#endif
    
#ifdef SCREENSHOT_MODE
    
    if (pX > (mWidth - 60.0f * gSpriteDrawScale)) {
        TestNextSection();
    } else if (pX < (60.0f * gSpriteDrawScale)) {
        TestPrevSection();
    } else {
        
        //
        if (pY < (60.0f * gSpriteDrawScale)) {
        
            FAchievement *aAchievement = new FAchievement();
            aAchievement->mTitle = "blimp my ride";
            gApp->mAchievementBubbleRenderer->Spawn(aAchievement);
        } else if (pY < (120.0f * gSpriteDrawScale)) {
            FAchievement *aAchievement = new FAchievement();
            aAchievement->mTitle = "get to the point";
            gApp->mAchievementBubbleRenderer->Spawn(aAchievement);
            
        }
    }
    
#endif
    
    
#ifdef VIDEO_MODE
    if (gTouch.mTouchCount > 1) {
        if (pY < mHeight2 * 0.2f) {
            if (pX > (mWidth - 80.0f * gSpriteDrawScale)) {
                Log("A\n");
                
            } else if (pX < (80.0f * gSpriteDrawScale)) {
                Log("B\n");
                
            }
        } else {
            if (pX > (mWidth - 80.0f * gSpriteDrawScale)) {
                TestNextSection();
            } else if (pX < (80.0f * gSpriteDrawScale)) {
                TestPrevSection();
            }
        }
    }
#endif
    
     
    
    
    /*
    if ((gTouch.mTouchCount > 1) && (gGameContainer != NULL)) {
        if ((gGameContainer != NULL) && (gGameContainer->mLevelCompleteMenu == NULL) && (gGameContainer->mGameOverMenu == NULL)) {
            
    #ifndef EDITOR_MODE
            gGameContainer->ShowLevelCompleteMenu();
            KillAllObjects();
    #endif
            
        }
    }
     */
    
    if (gGameContainer != NULL) {
        if ((gGameContainer->mBackgroundPaused == true) && (gApp->mIsLoadingComplete == true)) {
            gGameContainer->mBackgroundPaused = false;
            gGameContainer->mPauseFadeEnabled = false;
            gGameContainer->mPauseFade = 0.0f;
        }
        
        if (((gGameContainer->mPaused == true) || (gGameContainer->mBackgroundPaused == true))) {
            TouchFlush();
            return;
        }
    }
    
    if (mDartTouch != NULL) {
        ResetDartTouch();
        return;
    }
    
    if ((mIsLevelComplete == false) && (mIsGameOver == false)) {
        if ((mDartTouch == NULL) && (mDartResetAnimation == false) && (mCurrentDart != NULL)) {
            float aDistanceSquared = DistanceSquared(pX, pY, mDartSpawnX, mDartSpawnY);
            if (aDistanceSquared <= mDartPullbackGrabRangeSquared) {
                
                mTrajectoryStartX = pX;
                mTrajectoryStartY = pY;
                
                mTrajectoryStartTargetX = pX;
                mTrajectoryStartTargetY = pY;
                
                
                
                mDartTouch = pData;
                mDartTouchStartX = mDartSpawnX;
                mDartTouchStartY = mDartSpawnY;
                mDartTargetPullX = 0.0f;
                mDartTargetPullY = 0.0f;
                mDartPullX = 0.0f;
                mDartPullY = 0.0f;
                mDartTargetPullRotation = 0.0f;
                mDartPullRotation = 0.0f;
                mIsDartBeingPulled = false;
                mCurrentDart->SpawnAnimationForceComplete();
            }
        }
    }
    
    //See if there is any cannon to fire...
    Cannon *aFireCannon = NULL;
    
    if ((mIsLevelComplete == false) && (mIsGameOver == false)) {
        if (pData != mDartTouch) {
            
            float aBestDist = 150.0f * 150.0f;
            float aDist = 0.0f;
            float aDiffX = 0.0f;
            float aDiffY = 0.0f;
            
            EnumList(Cannon, aCannon, mCannonList.mObjectList) {
                if ((aCannon->mKill == 0) && (aCannon->mKnockedDown == false) && (aCannon->mStoredDarts.mCount > 0)) {
                    aDiffX = aCannon->mTransform.mX - pX;
                    aDiffY = aCannon->mTransform.mY - pY;
                    aDist = aDiffX * aDiffX + aDiffY * aDiffY;
                    if (aDist < aBestDist) {
                        aBestDist = aDist;
                        aFireCannon = aCannon;
                    }
                }
            }
            
            if (aFireCannon != NULL) {
                FireCannon(aFireCannon);
            }
        }
    }
    
}

void Game::TouchMove(float pX, float pY, void *pData) {
    
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            Log("Intercept Touch Move\n");
            return;
        }
    #endif
    
    if ((gGameContainer != NULL) && ((gGameContainer->mPaused == true) || (gGameContainer->mBackgroundPaused == true))) {
         TouchFlush();
        return;
    }
    
    
    if (mDartTouch == pData && mDartTouch != NULL) {
        
        //mTrajectoryStartX = pX;
        //mTrajectoryStartY = pY;
        
        mTrajectoryStartTargetX = pX;
        mTrajectoryStartTargetY = pY;
        
        
        float aDiffX = pX - mDartTouchStartX;
        float aDiffY = pY - mDartTouchStartY;
        float aDistanceSquared = aDiffX * aDiffX + aDiffY * aDiffY;
        if (aDistanceSquared > (mDartPullbackRangeMin * mDartPullbackRangeMin)) {
            
            if (mIsDartBeingPulled == false) {
                
                mDartResetAnimation = false;
                mDartResetAnimationTick = 0;
                
                mIsDartBeingPulled = true;
                gApp->mSoundDartPullback.Play();
                
                mTrajectoryStartX = pX;
                mTrajectoryStartY = pY;
                
                
                //mDartPullX = aDiffX;
                //mDartPullY = aDiffY;
                //mDartPullRotation = FaceTarget(aDiffX, aDiffY);
                
                //mDartTargetPullX = mDartPullX;
                //mDartTargetPullY = mDartPullY;
                //mDartTargetPullRotation = mDartPullRotation;
                
                //mDartTargetPullRotation
                //mDartPullRotation
                //mDartPullX = mDartTargetPullX;
                //mDartPullY = mDartTargetPullY;
                
                //mDartTargetPullX = 0.0f;
                //mDartTargetPullY = 0.0f;
                //mDartTargetPullRotation = 0.0f;
                //ResetTrajectory();
                
            }
            
            if (mTrajectoryList.mCount == 0) {
                float aLength = aDiffX * aDiffX + aDiffY * aDiffY;
                if (aLength > SQRT_EPSILON) {
                    aLength = sqrtf(aLength);
                    if (aLength > mDartPullbackRangeFingerMax) {
                        aDiffX /= aLength;
                        aDiffY /= aLength;
                        aDiffX *= mDartPullbackRangeFingerMax;
                        aDiffY *= mDartPullbackRangeFingerMax;
                    }
                }
                CorrectFingerRange(aDiffX, aDiffY);
                ComputeTrajectory(aDiffX, aDiffY);
            }
            
        } else {
            mIsDartBeingPulled = false;
            mDartTargetPullX = 0.0f;
            mDartTargetPullY = 0.0f;
            mDartTargetPullRotation = 0.0f;
            ResetTrajectory();
        }
        
        
        /*
        float aDiffX = pX - mDartTouchStartX;
        float aDiffY = pY - mDartTouchStartY;
        float aDistanceSquared = aDiffX * aDiffX + aDiffY * aDiffY;
        if (aDistanceSquared > (mDartPullbackRangeMin * mDartPullbackRangeMin)) {
            
            if (mIsDartBeingPulled == false) {
                mIsDartBeingPulled = true;
                gApp->mSoundDartPullback.Play();
            }
            
            float aLength = aDiffX * aDiffX + aDiffY * aDiffY;
            if (aLength > SQRT_EPSILON) {
                aLength = sqrtf(aLength);
                if (aLength > mDartPullbackRangeMax) {
                    aDiffX /= aLength;
                    aDiffY /= aLength;
                    aDiffX *= mDartPullbackRangeMax;
                    aDiffY *= mDartPullbackRangeMax;
                }
            }
            
            ComputeTrajectory(aDiffX, aDiffY);
            
        } else {
            mIsDartBeingPulled = false;
            mDartTargetPullX = 0.0f;
            mDartTargetPullY = 0.0f;
            mDartTargetPullRotation = 0.0f;
            ResetTrajectory();
        }
         
        */
    }
    
    
}

void Game::TouchUp(float pX, float pY, void *pData) {
    
    #ifdef EDITOR_MODE
        if (gEditor != NULL) {
            Log("Intercept Touch Up\n");
            return;
        }
    #endif
    
    if ((gGameContainer != NULL) && ((gGameContainer->mPaused == true) || (gGameContainer->mBackgroundPaused == true))) {
         TouchFlush();
         return;
    }
    
    if (mDartTouch) {
        if (mIsDartBeingPulled) {
            ReleaseDart();
            ResetTrajectory();
        } else {
            ResetDartTouch();
            ResetTrajectory();
        }
    }
}

void Game::TouchFlush() {
    ResetDartTouch();
}

void Game::KeyDown(int pKey) {
    
    /*
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        return;
    }
#endif
    
    if (gKeyDownCtrl) {
        if (pKey == __KEY__S) {
         
            Log("SAVING GAME STATE (STIFLED):\n");
            //SaveState();
            return;
        }
        if (pKey == __KEY__L) {
            Log("LOADING GAME STATE:\n");
            LoadState();
            return;
        }
    }
    
    if (pKey == __KEY__T) {
    
        if (gTrajectoryList.mCount > 0) {
            gTrajectoryList.RemoveAll();
        }
        
        gTrajectoryList.Add(&mTrajectoryList);
    }
    
    
    if (gKeyDownCtrl) {
        
        if (pKey == __KEY__R) {
            Restart();
        }
        
        
        if (pKey == __KEY__G) {
            if (mIsGameOver == false && mIsLevelComplete == false) {
                BeginGameOver();
            }
        }
        
        if (pKey == __KEY__L) {
            if (mIsGameOver == false && mIsLevelComplete == false) {
                BeginLevelComplete();
            }
        }
        
        if (pKey == __KEY__W) {
            
            EnumList(GameObject, aObject, mBalloonList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            
        }
        
        if (pKey == __KEY__P) {
            
            EnumList(GameObject, aObject, mBalloonList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
        }
        
        if (pKey == __KEY__K) {
            
            EnumList(GameObject, aObject, mBalloonList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
                DisposeObject(aObject);
            }
        }
    }
    
    if (gKeyDownShift) {
        
        if (pKey == __KEY__W) {
            
            EnumList(GameObject, aObject, mBalloonList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mDartList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mBrickHeadList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mTurtleList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mBombList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            
            EnumList(GameObject, aObject, mBlimpList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mPlatformList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            
            EnumList(GameObject, aObject, mCannonList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mStarList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mPrizeBoxList.mObjectList) {
                if (aObject->mDidOriginateOnWave == true) { DisposeObject(aObject); }
            }
            
        }
        
        if (pKey == __KEY__P) {
            
            EnumList(GameObject, aObject, mBalloonList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mDartList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mBrickHeadList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mTurtleList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mBombList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            
            EnumList(GameObject, aObject, mBlimpList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mPlatformList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            
            
            EnumList(GameObject, aObject, mCannonList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mStarList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            EnumList(GameObject, aObject, mPrizeBoxList.mObjectList) {
                if (aObject->mDidOriginateAsPermanent == true) { DisposeObject(aObject); }
            }
            
            
        }
        
        if (pKey == __KEY__K) {
            
            EnumList(GameObject, aObject, mBalloonList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mDartList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mBrickHeadList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mTurtleList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mBombList.mObjectList) {
                DisposeObject(aObject);
            }
            
            EnumList(GameObject, aObject, mBlimpList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mPlatformList.mObjectList) {
                DisposeObject(aObject);
            }
            
            
            EnumList(GameObject, aObject, mCannonList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mStarList.mObjectList) {
                DisposeObject(aObject);
            }
            EnumList(GameObject, aObject, mPrizeBoxList.mObjectList) {
                DisposeObject(aObject);
            }
        }
    }
    
    if (pKey == __KEY__L) {
        if (gKeyDownCtrl) {
            LifeGain();
        }
    }
    
    
    if (pKey == __KEY__D) {
        
        EnumList(GameObject, aObject, mBalloonList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(FreeLife, aObject, mFreeLifeList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mDartList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mBrickHeadList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mTurtleList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mBombList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mBlimpList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mPlatformList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mCannonList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mStarList.mObjectList) {
            KnockDown(aObject);
        }
        EnumList(GameObject, aObject, mPrizeBoxList.mObjectList) {
            KnockDown(aObject);
        }
    }
     
    */
}

void Game::KeyUp(int pKey) {
    
}

void Game::Notify(void *pSender, const char *pNotification) {
    
}

void Game::Clear() {
    
    if (mCurrentDart) {
        delete mCurrentDart;
        mCurrentDart = NULL;
    }
    
    KillAllObjects();
    
    mTrajectoryList.Reset();
    
    if (mGameOverBlimp != NULL) {
        delete mGameOverBlimp;
        mGameOverBlimp = NULL;
    }
    
    FreeList(MissedGameObjectCluster, mMissedObjectClusterList);
    FreeList(MissedGameObjectCluster, mMissedObjectClusterKillList);
    FreeList(MissedGameObjectCluster, mMissedObjectClusterQueueList);
    
    
    EnumList(GameObject, aObject, mDartList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mDartList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mBalloonList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mBalloonList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mFreeLifeList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mFreeLifeList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mBrickHeadList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mBrickHeadList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mTurtleList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mTurtleList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mBombList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mBombList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mBlimpList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mBlimpList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mPlatformList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mPlatformList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mCannonList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mCannonList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mStarList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mStarList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    EnumList(GameObject, aObject, mPrizeBoxList.mObjectList) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    EnumList(GameObject, aObject, mPrizeBoxList.mObjectListKill) { aObject->mPermSpawn = NULL; aObject->mWaveSpawn = NULL; aObject->mWave = NULL; aObject->mSection = NULL; }
    
    if (mLevelData != NULL) {
        delete mLevelData;
        mLevelData = NULL;
    }
    
    mProgressCount = 0;
    mProgress = 0;
}

void Game::DartMovingInterpolationBalloonsPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mBalloonList.mObjectList.mCount;n++) {
        Balloon *aBalloon = (Balloon *)mBalloonList.mObjectList.mData[n];
        if (aBalloon->mKill == 0) {
            if (aBalloon->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aBalloon);
            }
        }
    }
}

void Game::DartMovingInterpolationFreeLivesPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mFreeLifeList.mObjectList.mCount;n++) {
        FreeLife *aFreeLife = (FreeLife *)mFreeLifeList.mObjectList.mData[n];
        if (aFreeLife->mKill == 0) {
            if (aFreeLife->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aFreeLife);
            }
        }
    }
}

void Game::DartMovingInterpolationBrickHeadsPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mBrickHeadList.mObjectList.mCount;n++) {
        BrickHead *aBrickHead = (BrickHead *)mBrickHeadList.mObjectList.mData[n];
        if ((aBrickHead->mKill == 0) && (aBrickHead->mKnockedDown == false)) {
            if (aBrickHead->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aBrickHead);
            }
        }
    }
}

void Game::DartMovingInterpolationBombsPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mBombList.mObjectList.mCount;n++) {
        Bomb *aBomb = (Bomb *)mBombList.mObjectList.mData[n];
        if ((aBomb->mKill == 0) && (aBomb->mKnockedDown == false)) {
            if (aBomb->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aBomb);
            }
        }
    }
}

void Game::DartMovingInterpolationTurtlesPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mTurtleList.mObjectList.mCount;n++) {
        Turtle *aTurtle = (Turtle *)mTurtleList.mObjectList.mData[n];
        if ((aTurtle->mKill == 0) && (aTurtle->mKnockedDown == false)) {
            if (aTurtle->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aTurtle);
            }
        }
    }
}

void Game::DartMovingInterpolationCannonsPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mCannonList.mObjectList.mCount;n++) {
        Cannon *aCannon = (Cannon *)mCannonList.mObjectList.mData[n];
        if ((aCannon->mKill == 0) && (aCannon->mKnockedDown == false) && (pDart->mFiredCannon != aCannon)) {
            if (aCannon->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aCannon);
            }
        }
    }
}

void Game::DartMovingInterpolationPlatformsPrepare(Dart *pDart) {
    mTempObjectList.RemoveAll();
    for (int n=0;n<mPlatformList.mObjectList.mCount;n++) {
        Platform *aPlatform = (Platform *)mPlatformList.mObjectList.mData[n];
        if ((aPlatform->mKill == 0) && (aPlatform->mKnockedDown == false)) {
            if (aPlatform->mIgnoreDartList.Exists(pDart) == false) {
                mTempObjectList.Add(aPlatform);
            }
        }
    }
}

void Game::DartMovingInterpolationUpdateTip(Dart *pDart, float pPercent, bool pEnd) {
    if (pEnd) {
        pDart->mTransform.mX = pDart->mUpdateInterpEndX;
        pDart->mTransform.mY = pDart->mUpdateInterpEndY;
        pDart->mTransform.mRotation = pDart->mUpdateInterpEndRotation;
    } else {
        pDart->mTransform.mX = pDart->mUpdateInterpStartX
        + (pDart->mUpdateInterpEndX - pDart->mUpdateInterpStartX) * pPercent;
        pDart->mTransform.mY = pDart->mUpdateInterpStartY
        + (pDart->mUpdateInterpEndY - pDart->mUpdateInterpStartY) * pPercent;
        pDart->mTransform.mRotation = pDart->mUpdateInterpStartRotation
        + (pDart->mUpdateInterpEndRotation - pDart->mUpdateInterpStartRotation) * pPercent;
    }
    pDart->GetTipPoint(pDart->mTipX, pDart->mTipY);
}

void Game::DartMovingInterpolationBalloons(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        Balloon *aBalloon = (Balloon *)mTempObjectList.mData[n];
        if (aBalloon->mKill == 0) {
            if (aBalloon->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithBalloon(pDart, aBalloon);
            }
        }
    }
}

void Game::DartMovingInterpolationFreeLives(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        FreeLife *aFreeLife = (FreeLife *)mTempObjectList.mData[n];
        if (aFreeLife->mKill == 0) {
            if (aFreeLife->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithFreeLife(pDart, aFreeLife);
            }
        }
    }
}

void Game::DartMovingInterpolationBombs(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        Bomb *aBomb = (Bomb *)mTempObjectList.mData[n];
        if (aBomb->mKill == 0) {
            if (aBomb->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithBomb(pDart, aBomb);
            }
        }
    }
}

void Game::DartMovingInterpolationBrickHeads(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        BrickHead *aBrickHead = (BrickHead *)mTempObjectList.mData[n];
        if (aBrickHead->mKill == 0) {
            if (aBrickHead->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithBrickhead(pDart, aBrickHead);
            }
        }
    }
}

void Game::DartMovingInterpolationTurtles(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        Turtle *aTurtle = (Turtle *)mTempObjectList.mData[n];
        if ((aTurtle->mKill == 0) && (aTurtle->mKnockedDown == false)) {
            if (aTurtle->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithTurtle(pDart, aTurtle);
            }
        }
    }
}

void Game::DartMovingInterpolationCannons(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        Cannon *aCannon = (Cannon *)mTempObjectList.mData[n];
        if (aCannon->mKill == 0) {
            if (aCannon->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithCannon(pDart, aCannon);
            }
        }
    }
}

void Game::DartMovingInterpolationPlatforms(Dart *pDart, float pPercent, bool pEnd) {
    DartMovingInterpolationUpdateTip(pDart, pPercent, pEnd);
    for (int n=0;n<mTempObjectList.mCount;n++) {
        Platform *aPlatform = (Platform *)mTempObjectList.mData[n];
        if (aPlatform->mKill == 0) {
            if (aPlatform->WillCollide(pDart->mPrevTipX, pDart->mPrevTipY, pDart->mTipX, pDart->mTipY)) {
                DartCollideWithPlatform(pDart, aPlatform);
            }
        }
    }
}

bool Game::ShouldSpawnFreeLife() {
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        return false;
    } else {
        return ShouldSpawnFreeLifeMain();
    }
#endif
    
    return ShouldSpawnFreeLifeMain();
}

bool Game::ShouldSpawnFreeLifeMain() {
    if (mLevelData != NULL) {
        if (mLevelData->mCurrentSection != NULL) {
            if (mLevelData->mCurrentSection->mBlockFreeLives == true) {
                return false;
            }
        }
    }
    
    if (gApp != NULL) {
        if (gApp->mLevelID == LearningBasics) {
            return false;
        }
    }
    
    bool aResult = false;
    
    if ((mTimeWhenFreeLifeSpawned > 0) && ((mTime - mTimeWhenFreeLifeSpawned) > 1200)) {
        aResult = (gRand.Get(48) == 14);
    } else if (mTimeWhenFreeLifeSpawned <= 0) {
        aResult = (gRand.Get(48) == 12);
    }
    
    if (aResult == true) {
        mTimeWhenFreeLifeSpawned = mTime;
    }
    
    return aResult;
}

void Game::NotifyDidSpawnFreeLife() {
    //Log("We spawned FL...\n");
    
}

void Game::HandleRapidBonus(Dart *pDart, int pNumber) {
    
    float aX = pDart->mTransform.mX;
    float aY = pDart->mTransform.mY;
    
    if (pDart->mRecentCollider) {
        aX = pDart->mRecentColliderX;
        aY = pDart->mRecentColliderY;
    }
    
    if (gEffectsOverlay != NULL) {
        gEffectsOverlay->Get2DPos(aX, aY);
    }
    
    int aWordIndex = gRand.Get(3);
    
    if (pNumber <= 5) {
        
        if (aWordIndex == 0) { gEffectsOverlay->SpawnComboBubble(aX, aY, "SUPERB", "+100"); }
        if (aWordIndex == 1) { gEffectsOverlay->SpawnComboBubble(aX, aY, "AWESOME", "+100"); }
        if (aWordIndex == 2) { gEffectsOverlay->SpawnComboBubble(aX, aY, "COOL", "+100"); }
        
        mScore += 100;
        gApp->mSoundCombo.Play();
    } else if (pNumber == 6) {
        
        if (aWordIndex == 0) { gEffectsOverlay->SpawnComboBubble(aX, aY, "SUPER", "+150"); }
        if (aWordIndex == 1) { gEffectsOverlay->SpawnComboBubble(aX, aY, "GREAT", "+150"); }
        if (aWordIndex == 2) { gEffectsOverlay->SpawnComboBubble(aX, aY, "BEAUTIFUL", "+150"); }
        
        mScore += 150;
        gApp->mSoundCombo.Play();
    } else if (pNumber == 7) {
        
        if (aWordIndex == 0) { gEffectsOverlay->SpawnComboBubble(aX, aY, "WONDERFUL", "+200"); }
        if (aWordIndex == 1) { gEffectsOverlay->SpawnComboBubble(aX, aY, "GLORIOUS", "+200"); }
        if (aWordIndex == 2) { gEffectsOverlay->SpawnComboBubble(aX, aY, "SUPERB", "+200"); }
        
        mScore += 200;
        gApp->mSoundCombo.Play();
    } else {
        if (aWordIndex == 0) { gEffectsOverlay->SpawnComboBubble(aX, aY, "BRILLIANT", "+250"); }
        if (aWordIndex == 1) { gEffectsOverlay->SpawnComboBubble(aX, aY, "MAGNIFICENT", "+250"); }
        if (aWordIndex == 2) { gEffectsOverlay->SpawnComboBubble(aX, aY, "TREMENDOUS", "+250"); }
        
        mScore += 250;
        gApp->mSoundCombo.Play();
    }
    
}

void Game::HandleStreakUpdate(Dart *pDart) {
    
    if (gEffectsOverlay == NULL) { return; }
    if (pDart == NULL) { return; }
    
    float aX = pDart->mTransform.mX;
    float aY = pDart->mTransform.mY;
    
    if (pDart->mRecentCollider == true) {
        aX = pDart->mRecentColliderX;
        aY = pDart->mRecentColliderY;
    }
    
    if (gEffectsOverlay != NULL) {
        gEffectsOverlay->Get2DPos(aX, aY);
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 10) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 10", "+100");
        mScore += 100;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 20) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 20", "+200");
        mScore += 200;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 30) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 30", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 40) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 40", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 50) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 50", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 60) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 60", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 70) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 70", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 80) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 80", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 90) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 90", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 100) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "STREAK 100", "+300");
        mScore += 300;
        gApp->mSoundCombo.Play();
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 10) {
        gAch.ProgressGroup("streak_10");
    }
    
    if (mStreakController.mConsecutiveStreakBalloonCount == 50) {
        gAch.ProgressGroup("streak_50");
    }
}


void Game::NotifyDartDidStopRapidStreak(Dart *pDart) {
    
    if (mIsLevelComplete == true) { return; }
    if (mIsGameOver == true) { return; }
    if (pDart == NULL) { return; }
    if (gEffectsOverlay == NULL) { return; }
    
    //Log("NotifyDartDidStopRapidStreak( %d )\n", pDart->mHitCountRapid);
    
    int aRapidStreak = pDart->mHitCountRapid;
    
    if (pDart->mHitCountRapidTurtle >= 2) {
        aRapidStreak += 2;
    }
    
    if (aRapidStreak < 5) { return; }
    
    pDart->mHitCountRapid = 0;
    pDart->mHitCountRapidTurtle = 0;
    
    //We subtract all the current hits for subsequent bonuses...
    pDart->mHitCountSubtract = pDart->mHitCount;
    
    HandleRapidBonus(pDart, aRapidStreak);
    
}

void Game::NotifyDartDidStopStreak(Dart *pDart) {
    
    if (mIsLevelComplete == true) { return; }
    if (mIsGameOver == true) { return; }
    if (pDart == NULL) { return; }
    if (gEffectsOverlay == NULL) { return; }
    
    if (pDart->mHitCount == pDart->mHitCountRapid) {
        //Log("aExtraBonus routed to RAPID bonus...\n");
        
        NotifyDartDidStopRapidStreak(pDart);
        pDart->mHitCountRapid = 0;
        pDart->mHitCountRapidTurtle = 0;
        return;
    }
    
    if (pDart->mShotFromCannon == true) {
        //Log("Shot from cannon, no non-rapid logic here...\n");
        
        NotifyDartDidStopRapidStreak(pDart);
        pDart->mHitCountRapid = 0;
        pDart->mHitCountRapidTurtle = 0;
        return;
    }
    
    if ((pDart->mHitCount < 5) || (pDart->mHitCountSubtract == 0)) {
        //Log("aExtraBonus blocked off (less than 5) HC: %d Sub: %d\n", pDart->mHitCount, pDart->mHitCountSubtract);
        
        return;
    }
    
    int aExtraBonus = (pDart->mHitCount - pDart->mHitCountSubtract);
    
    if (aExtraBonus <= 0) {
        return;
    }
    
    if (pDart->mHitCountRapidTurtle >= 3) {
        aExtraBonus += 2;
    }
    
    //Log("aExtraBonus!!! [%d]\n", aExtraBonus);
    
    float aX = pDart->mTransform.mX;
    float aY = pDart->mTransform.mY;
    
    if (pDart->mRecentCollider) {
        aX = pDart->mRecentColliderX;
        aY = pDart->mRecentColliderY;
    }
    
    if (gEffectsOverlay != NULL) {
        gEffectsOverlay->Get2DPos(aX, aY);
    }
    
    if (aExtraBonus == 1) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "TRICKY", "+50");
        mScore += 50;
        gApp->mSoundCombo.Play();
    } else if (aExtraBonus == 2) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "BONUS", "+100");
        mScore += 100;
        gApp->mSoundCombo.Play();
    } else if (aExtraBonus == 3) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "TOPPER", "+150");
        mScore += 150;
        gApp->mSoundCombo.Play();
    } else if (aExtraBonus == 4) {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "AMAZING", "+200");
        mScore += 200;
        gApp->mSoundCombo.Play();
    } else {
        gEffectsOverlay->SpawnComboBubble(aX, aY, "PERFECT", "+250");
        mScore += 250;
        gApp->mSoundCombo.Play();
    }
}

void Game::NotifyDartDidExitScreen(Dart *pDart) {
    
    if (mIsLevelComplete == true) {
        return;
    }
    
    if (mIsGameOver == true) {
        return;
    }
    
    if (pDart == NULL) { return; }
    
    mStreakController.RecordDartComplete(pDart);
    
    NotifyDartDidStopStreak(pDart);
}

void Game::NotifyDartDidLeavePlayArea(Dart *pDart) {
    
    if (mIsLevelComplete == true) {
        return;
    }
    
    if (mIsGameOver == true) {
        return;
    }
    //Log("NotifyDartDidLeavePlayArea(%p)\n", pDart);
    
    if (pDart == NULL) { return; }
    
    NotifyDartDidStopRapidStreak(pDart);
    
    //
    // Basically, what we want to do with this hook
    // is to reward for a "quick streak" such as popping 5+ balloons
    // in rapid succession...
    //
    
}

bool Game::IsWaveClearForSpawn() {
    
    EnumList (FreeLife, aFreeLife, mFreeLifeList.mObjectList) {
        if ((aFreeLife->mKill == 0) && (aFreeLife->mDidOriginateOnWave == true)) {
            return false;
        }
    }
    EnumList (Balloon, aBalloon, mBalloonList.mObjectList) {
        if ((aBalloon->mKill == 0) && (aBalloon->mDidOriginateOnWave == true)) {
            return false;
        }
    }
    EnumList (Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mKill == 0) && (aTurtle->mDidOriginateOnWave == true) && (aTurtle->mKnockedDown == false) && (aTurtle->mBalloon != NULL)) {
            return false;
        }
    }
    EnumList (BrickHead, aBrickHead, mBrickHeadList.mObjectList) {
        if ((aBrickHead->mKill == 0) && (aBrickHead->mKnockedDown == false) && (aBrickHead->mDidOriginateOnWave == true)) {
            return false;
        }
    }
    EnumList (Bomb, aBomb, mBombList.mObjectList) {
        if ((aBomb->mKill == 0) && (aBomb->mDidOriginateOnWave == true)) {
            return false;
        }
    }
    
    return true;
}

bool Game::IsScreenClearForSpawn(bool pIncludePerms) {
    
    int aPermCount = 0;
    int aWaveCount = 0;
    
    EnumList (FreeLife, aFreeLife, mFreeLifeList.mObjectList) {
        if (aFreeLife->mKill == 0) {
            if (aFreeLife->mDidOriginateAsPermanent == true) {
                ++aPermCount;
            }
            if (aFreeLife->mDidOriginateOnWave == true) {
                ++aWaveCount;
            }
        }
    }
    
    EnumList (Balloon, aBalloon, mBalloonList.mObjectList) {
        if (aBalloon->mKill == 0) {
            if (aBalloon->mDidOriginateAsPermanent == true) {
                ++aPermCount;
            }
            if (aBalloon->mDidOriginateOnWave == true) {
                ++aWaveCount;
            }
        }
    }
    
    EnumList (Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mKill == 0) && (aTurtle->mKnockedDown == false) && (aTurtle->mBalloon != NULL)) {
            if (aTurtle->mDidOriginateAsPermanent == true) {
                ++aPermCount;
            }
            if (aTurtle->mDidOriginateOnWave == true) {
                ++aWaveCount;
            }
        }
    }
    
    EnumList (BrickHead, aBrickHead, mBrickHeadList.mObjectList) {
        
        if ((aBrickHead->mKill == 0) && (aBrickHead->mKnockedDown == false)) {
            if (aBrickHead->mDidOriginateAsPermanent == true) {
                ++aPermCount;
            }
            if (aBrickHead->mDidOriginateOnWave == true) {
                ++aWaveCount;
            }
        }
    }
    EnumList (Bomb, aBomb, mBombList.mObjectList) {
        if ((aBomb->mKill == 0) && (aBomb->mDidOriginateOnWave == true)) {
            if (aBomb->mDidOriginateAsPermanent == true) {
                ++aPermCount;
            }
            if (aBomb->mDidOriginateOnWave == true) {
                ++aWaveCount;
            }
        }
    }
    
    if (pIncludePerms) {
        if (aPermCount > 0 || aWaveCount > 0) {
            return false;
        }
    } else {
        if (aWaveCount > 0) {
            return false;
        }
    }
    
    return true;
}

void Game::ComputeTrajectory(float pDiffX, float pDiffY) {
    
    mDartTargetPullX = pDiffX;
    mDartTargetPullY = pDiffY;
    mDartTargetPullRotation = FaceTarget(pDiffX, pDiffY);
    
    mTrajectoryExists = false;
    
    mTrajectoryList.RemoveAll();
    
    if ((mCurrentDart != NULL) && (mIsDartBeingPulled == true)) {
        
        //How much kapow do we give this dart?
        mReleaseFactor = 0.0f;
        
        mReleaseDirX = -pDiffX;
        mReleaseDirY = -pDiffY;
        
        float aPullLength = mReleaseDirX * mReleaseDirX + mReleaseDirY * mReleaseDirY;
        if (aPullLength > SQRT_EPSILON) {
            mTrajectoryExists = true;
            
            aPullLength = (float)(sqrtf(aPullLength));
            
            mReleaseDirX /= aPullLength;
            mReleaseDirY /= aPullLength;
            
            mReleaseFactor = (aPullLength - mDartPullbackRangeMin) / (mDartPullbackRangeMax - mDartPullbackRangeMin);
            //aReleaseFactor *= 1.10f;
            
            if (mReleaseFactor < 0.0f) { mReleaseFactor = 0.0f; }
            if (mReleaseFactor > 1.0f) { mReleaseFactor = 1.0f; }
            
            //aReleaseFactor = FAnimation::EaseInCirc(aReleaseFactor) * 0.5f + aReleaseFactor * 0.5f;
            
            
            mReleaseVelocity = mDartReleaseVelocityMin + (mDartReleaseVelocityMax - mDartReleaseVelocityMin) * mReleaseFactor;
            
            float aTrajectoryX = mCurrentDart->mTransform.mX;
            float aTrajectoryY = mCurrentDart->mTransform.mY;
            
            float aTrajectoryVelocityX = mReleaseDirX * mReleaseVelocity;
            float aTrajectoryVelocityY = mReleaseDirY * mReleaseVelocity;
            
            float aTrajectoryTargetRotation = FaceTarget(-aTrajectoryVelocityX, -aTrajectoryVelocityY);
            float aTrajectoryRotation = aTrajectoryTargetRotation;
            
            float aAngleDiff = 0.0f;
            
            float aTipX = 0.0f;
            float aTipY = 0.0f;
            
            mCurrentDart->GetTipPoint(aTrajectoryX, aTrajectoryY, 1.0f, aTrajectoryRotation, aTipX, aTipY);
            
            bool aLoop = true;
            
            
            //int aTESTlooopCoUnT = 0;
            while (aLoop == true) {
                mTrajectoryList.Add(aTipX, aTipY);
                aTrajectoryVelocityY += mGravity;
                
                if (fabsf(aTrajectoryVelocityX) > 0.001f || fabsf(aTrajectoryVelocityY) > 0.001f) {
                    aTrajectoryTargetRotation = FaceTarget(-aTrajectoryVelocityX, -aTrajectoryVelocityY);
                }
                
                aAngleDiff = DistanceBetweenAngles(aTrajectoryRotation, aTrajectoryTargetRotation);
                if (aAngleDiff > 2.0f) { aTrajectoryRotation += 1.0f; }
                if (aAngleDiff < -2.0f) { aTrajectoryRotation -= 1.0f; }
                aAngleDiff = DistanceBetweenAngles(aTrajectoryRotation, aTrajectoryTargetRotation);
                aTrajectoryRotation += aAngleDiff * 0.035f;
                aTrajectoryX += aTrajectoryVelocityX;
                aTrajectoryY += aTrajectoryVelocityY;
                
                mCurrentDart->GetTipPoint(aTrajectoryX, aTrajectoryY, 1.0f, aTrajectoryRotation, aTipX, aTipY);
                
                if ((aTipX < mTrajectoryKillZoneLeft) ||
                    (aTipX > mTrajectoryKillZoneRight) ||
                    (aTipY < mTrajectoryKillZoneTop) ||
                    (aTipY > mTrajectoryKillZoneBottom)) {
                    ComputeTrajectoryAddFinalPoint(aTipX, aTipY);
                    break;
                }
            }
            
            ProcessTrajectory();
            
            mCurrentDart->mSharpTurn = false;
            if (mTrajectoryIsNarrow == true) {
                
                mCurrentDart->mSharpTurn = true;
                
                aTrajectoryX = mCurrentDart->mTransform.mX;
                aTrajectoryY = mCurrentDart->mTransform.mY;
                aTrajectoryVelocityX = mReleaseDirX * mReleaseVelocity;
                aTrajectoryVelocityY = mReleaseDirY * mReleaseVelocity;
                aTrajectoryRotation = FaceTarget(-aTrajectoryVelocityX, -aTrajectoryVelocityY);
                
                bool aSharpTurnDidBegin = false;
                int aSharpTurnTime = 28;
                int aSharpTurnTimer = 0;
                
                float aSharpTurnStartRotation = fmodf(aTrajectoryRotation, 360.0f);
                if (aSharpTurnStartRotation < 0.0f) {
                    aSharpTurnStartRotation += 360.0f;
                }
                float aAngleDiff = (180.0f - aSharpTurnStartRotation);
                float aSharpTurnEndRotation = 180.0f - aAngleDiff;
                
                float aDistFromCenter = 48.0f * 1.0f;
                //float aDirX = Sin(aTrajectoryRotation);
                //float aDirY = -Cos(aTrajectoryRotation);
                
                
                float aTipPercent = 0.0f;
                float aTipCompressionPercent = 0.0f;
                
                float aTipDirX = Sin(aTrajectoryRotation);
                float aTipDirY = -Cos(aTrajectoryRotation);
                float aTipDist = aDistFromCenter;
                
                aTipX = aTrajectoryX + aTipDirX * aTipDist;
                aTipY = aTrajectoryY + aTipDirY * aTipDist;
                
                aLoop = true;
                mTrajectoryList.Reset();
                while (aLoop == true) {
                    mTrajectoryList.Add(aTipX, aTipY);
                    aTrajectoryVelocityY += mGravity;
                    aTrajectoryX += aTrajectoryVelocityX;
                    aTrajectoryY += aTrajectoryVelocityY;

                    if ((aTrajectoryVelocityY > 0.0f) && (aSharpTurnDidBegin == false)) {
                         aSharpTurnDidBegin = true;
                    }
                    
                    aTipPercent = 1.0f;
                    
                    
                    aTipDist = aDistFromCenter;
                    
                    
                    aTrajectoryRotation = aSharpTurnStartRotation;
                    
                    aTipDirX = Sin(aTrajectoryRotation);
                    aTipDirY = -Cos(aTrajectoryRotation);
                    
                    if (aSharpTurnDidBegin == true) {
                        aSharpTurnTimer++;
                        if (aSharpTurnTimer >= aSharpTurnTime) {
                            aSharpTurnTimer = aSharpTurnTime;
                            aTrajectoryRotation = aSharpTurnEndRotation;
                        } else {
                            
                            aTipPercent = ((float)aSharpTurnTimer) / ((float)aSharpTurnTime);
                            //Log("aTipPercent = %f\n", aTipPercent);
                            
                            if (aTipPercent <= 0.5f) {
                                aTipCompressionPercent = aTipPercent * 2.0f;
                                aTipCompressionPercent = (1.0f - aTipCompressionPercent);
                                aTipDist *= aTipCompressionPercent;
                                
                                
                            } else {
                                
                                aTipCompressionPercent = (aTipPercent - 0.5f) * 2.0f;
                                aTipDist *= aTipCompressionPercent;
                            }
                            
                            aTrajectoryRotation = aSharpTurnStartRotation + (aSharpTurnEndRotation - aSharpTurnStartRotation) * aTipPercent;
                        }
                    }
                    
                    if (aSharpTurnDidBegin == true) {
                        if (aSharpTurnTimer >= aSharpTurnTime) {
                            aTipDirY = -aTipDirY;
                        } else {
                            if (aSharpTurnTimer <= (aSharpTurnTime >> 1)) {
                                
                            } else {
                                aTipDirY = -aTipDirY;
                            }
                        }
                    }
                    
                    aTipX = aTrajectoryX + aTipDirX * aTipDist;
                    aTipY = aTrajectoryY + aTipDirY * aTipDist;
                    
                    if ((aTipX < mTrajectoryKillZoneLeft) ||
                        (aTipX > mTrajectoryKillZoneRight) ||
                        (aTipY < mTrajectoryKillZoneTop) ||
                        (aTipY > mTrajectoryKillZoneBottom)) {
                        ComputeTrajectoryAddFinalPoint(aTipX, aTipY);
                        break;
                    }
                }
            }
        }
    }
    
    if (gInterfaceOverlay != NULL) {
        
        //gInterfaceOverlay->mTrajectoryPath.RemoveAll();
        
        if (mTrajectoryList.mCount >= 4) {
            gInterfaceOverlay->mMagicArrow.Refresh(&mTrajectoryList, mTrajectoryIsNarrow);
            gInterfaceOverlay->mMagicArrow.Show();
            
        } else {
            gInterfaceOverlay->mMagicArrow.Hide();
        }
    }
}

bool Game::TrajectoryKillZoneCollisionPoint(float pX1, float pY1, float pX2, float pY2, float pLineX1, float pLineY1, float pLineX2, float pLineY2, float &pResultX, float &pResultY) {
    
    /*
    bool aParallel = false;
    
    float aParallelEpsilon = 0.01f;
    
    if (fabsf(pLineX1 - pLineX2) > fabsf(pLineY1 - pLineY2)) {
        
        //In this case, the "border" line is a horizontal line..
        
        if (fabsf(pY2 - pY1) <= aParallelEpsilon) {
         
            Log("Blocking Parallel H!!!\n");
            
            return false;
        }
    } else {
        //In this case, the "border" line is a horizontal line..
        if (fabsf(pX2 - pX1) <= aParallelEpsilon) {
            
            Log("Blocking Parallel V!!!\n");
            
            return false;
        }
    }
    */
    
    float aCollisionX = 0.0f;
    float aCollisionY = 0.0f;
    float aRayDirX = (pX2 - pX1);
    float aRayDirY = (pY2 - pY1);
    float aRayLength = aRayDirX * aRayDirX + aRayDirY * aRayDirY;
    if (aRayLength > SQRT_EPSILON) {
        aRayLength = sqrtf(aRayLength);
        aRayDirX /= aRayLength;
        aRayDirY /= aRayLength;
        float aCollisionDistance = 0.0f;
        
        //Log("Ray Pos1 [%.3f, %.3f] Pos2 [%.3f, %.3f] Dir [%.3f %.3f]\n", pX1, pY1, pX2, pY2, aRayDirX, aRayDirY);
        //Log("Line Pos [%.1f %.1f] to [%.1f %.1f]\n", pLineX1, pLineY1, pLineX2, pLineY2);
        
        if (FLine::SegmentRayIntersection(pLineX1, pLineY1, pLineX2, pLineY2, pX1, pY1, aRayDirX, aRayDirY, aRayLength, aCollisionX, aCollisionY, aCollisionDistance)) {
            pResultX = aCollisionX;
            pResultY = aCollisionY;
            return true;
        }
        
    }
    
    return false;
}

void Game::ComputeTrajectoryAddFinalPoint(float pTrajectoryX, float pTrajectoryY) {
    
    if ((pTrajectoryX < mTrajectoryKillZoneLeft) ||
        (pTrajectoryX > mTrajectoryKillZoneRight) ||
        (pTrajectoryY < mTrajectoryKillZoneTop) ||
        (pTrajectoryY > mTrajectoryKillZoneBottom)) {
        
        if (mTrajectoryList.mCount <= 0) { return; }
        
        float aLastX = mTrajectoryList.mX[mTrajectoryList.mCount - 1];
        float aLastY = mTrajectoryList.mY[mTrajectoryList.mCount - 1];
        
        bool aFoundWall = false;
        
        float aBestDistance = 10000.0f * 10000.0f;
        
        float aCollisionX = 0.0f;
        float aCollisionY = 0.0f;
        
        float aBestX = 0.0f;
        float aBestY = 0.0f;
        
        //Left wall
        if (SegmentsIntersect(FVec2(aLastX, aLastY), FVec2(pTrajectoryX, pTrajectoryY), FVec2(mTrajectoryKillZoneLeft, mTrajectoryKillZoneTop), FVec2(mTrajectoryKillZoneLeft, mTrajectoryKillZoneBottom))) {
            if (TrajectoryKillZoneCollisionPoint(aLastX, aLastY, pTrajectoryX, pTrajectoryY, mTrajectoryKillZoneLeft, mTrajectoryKillZoneTop, mTrajectoryKillZoneLeft, mTrajectoryKillZoneBottom, aCollisionX, aCollisionY)) {
                float aDistance = DistanceSquared(aCollisionX, aCollisionY, aLastX, aLastY);
                
                if (aDistance < aBestDistance) {
                    aBestX = aCollisionX;
                    aBestY = aCollisionY;
                    aBestDistance = aDistance;
                    aFoundWall = true;
                }
            }
        }
        
        //Right Wall...
        if (SegmentsIntersect(FVec2(aLastX, aLastY), FVec2(pTrajectoryX, pTrajectoryY), FVec2(mTrajectoryKillZoneRight, mTrajectoryKillZoneTop), FVec2(mTrajectoryKillZoneRight, mTrajectoryKillZoneBottom))) {
            if (TrajectoryKillZoneCollisionPoint(aLastX, aLastY, pTrajectoryX, pTrajectoryY, mTrajectoryKillZoneRight, mTrajectoryKillZoneTop, mTrajectoryKillZoneRight, mTrajectoryKillZoneBottom, aCollisionX, aCollisionY)) {
                float aDistance = DistanceSquared(aCollisionX, aCollisionY, aLastX, aLastY);
                if (aDistance < aBestDistance) {
                    aBestX = aCollisionX;
                    aBestY = aCollisionY;
                    aBestDistance = aDistance;
                    aFoundWall = true;
                }
            }
        }
        
        //Top Wall
        if (SegmentsIntersect(FVec2(aLastX, aLastY), FVec2(pTrajectoryX, pTrajectoryY), FVec2(mTrajectoryKillZoneLeft, mTrajectoryKillZoneTop), FVec2(mTrajectoryKillZoneRight, mTrajectoryKillZoneTop))) {
            if (TrajectoryKillZoneCollisionPoint(aLastX, aLastY, pTrajectoryX, pTrajectoryY, mTrajectoryKillZoneLeft, mTrajectoryKillZoneTop, mTrajectoryKillZoneRight, mTrajectoryKillZoneTop, aCollisionX, aCollisionY)) {
                float aDistance = DistanceSquared(aCollisionX, aCollisionY, aLastX, aLastY);
                if (aDistance < aBestDistance) {
                    aBestX = aCollisionX;
                    aBestY = aCollisionY;
                    aBestDistance = aDistance;
                    aFoundWall = true;
                }
            }
        }
        
        //Bottom Wall
        if (SegmentsIntersect(FVec2(aLastX, aLastY), FVec2(pTrajectoryX, pTrajectoryY), FVec2(mTrajectoryKillZoneLeft, mTrajectoryKillZoneBottom), FVec2(mTrajectoryKillZoneRight, mTrajectoryKillZoneBottom))) {
            if (TrajectoryKillZoneCollisionPoint(aLastX, aLastY, pTrajectoryX, pTrajectoryY, mTrajectoryKillZoneLeft, mTrajectoryKillZoneBottom, mTrajectoryKillZoneRight, mTrajectoryKillZoneBottom, aCollisionX, aCollisionY)) {
                float aDistance = DistanceSquared(aCollisionX, aCollisionY, aLastX, aLastY);
                if (aDistance < aBestDistance) {
                    aBestX = aCollisionX;
                    aBestY = aCollisionY;
                    aBestDistance = aDistance;
                    aFoundWall = true;
                }
            }
        }
        
        if (aFoundWall == true) {
            mTrajectoryList.Add(aBestX, aBestY);
        }
        
    }
}

void Game::ProcessTrajectory() {
    
    mTrajectoryIsNarrow = false;
    
    float aTrajectoryPeakX = mTrajectoryList.mX[0];
    float aTrajectoryPeakY = mTrajectoryList.mY[0];
    
    float aTrajectoryFrontX1 = 0.0f;
    float aTrajectoryFrontY1 = 0.0f;
    
    float aTrajectoryFrontX2 = 0.0f;
    float aTrajectoryFrontY2 = 0.0f;
    
    float aTrajectoryRearX1 = 0.0f;
    float aTrajectoryRearY1 = 0.0f;
    
    float aTrajectoryRearX2 = 0.0f;
    float aTrajectoryRearY2 = 0.0f;
    
    int aPeakIndex = 0;
    
    for (int i=0;i<mTrajectoryList.mCount;i++) {
        if (mTrajectoryList.mY[i] < aTrajectoryPeakY) {
            aTrajectoryPeakX = mTrajectoryList.mX[i];
            aTrajectoryPeakY = mTrajectoryList.mY[i];
            aPeakIndex = i;
        }
    }
    
    int aRequiredRange = 22;
    bool aCancel = false;
    
    
    int aFrontCount = aPeakIndex;
    int aBackCount = (mTrajectoryList.mCount - aPeakIndex);
    
    if (aFrontCount < aRequiredRange) {
        aCancel = true;
    }
    
    if (aBackCount < aRequiredRange) { aCancel = true; }
    if (aCancel == true) {
        aTrajectoryPeakX = 0.0f;
        aTrajectoryPeakY = 0.0f;
        return;
    }
    
    int aFrontHops1 = (int)(((float)aFrontCount) * 0.50f);
    int aFrontHops2 = (int)(((float)aFrontCount) * 0.80f);
    
    int aFrontIndex1 = aPeakIndex - aFrontHops1;
    int aFrontIndex2 = aPeakIndex - aFrontHops2;

    aTrajectoryFrontX1 = mTrajectoryList.mX[aFrontIndex1];
    aTrajectoryFrontY1 = mTrajectoryList.mY[aFrontIndex1];
    
    aTrajectoryFrontX2 = mTrajectoryList.mX[aFrontIndex2];
    aTrajectoryFrontY2 = mTrajectoryList.mY[aFrontIndex2];
    
    int aBackHops1 = (int)(((float)aBackCount) * 0.25f);
    int aBackHops2 = (int)(((float)aBackCount) * 0.50f);
    
    int aBackIndex1 = aPeakIndex + aBackHops1;
    int aBackIndex2 = aPeakIndex + aBackHops2;
    
    aTrajectoryRearX1 = mTrajectoryList.mX[aBackIndex1];
    aTrajectoryRearY1 = mTrajectoryList.mY[aBackIndex1];
    
    aTrajectoryRearX2 = mTrajectoryList.mX[aBackIndex2];
    aTrajectoryRearY2 = mTrajectoryList.mY[aBackIndex2];
    
    float aRange1 = fabsf(aTrajectoryFrontX1 - aTrajectoryRearX1);
    float aRange2 = fabsf(aTrajectoryFrontX2 - aTrajectoryRearX2);
    
    if (aRange2 <= 100.0f) { mTrajectoryIsNarrow = true; }
    if (aRange2 < (aRange1 * 0.8f)) { mTrajectoryIsNarrow = true; }
}

void Game::ResetTrajectory() {
    
    mTrajectoryList.RemoveAll();
    if (gInterfaceOverlay) {
        gInterfaceOverlay->mMagicArrow.Hide();
        //gInterfaceOverlay->mTrajectoryPath.RemoveAll();
    }
}

void Game::Convert2DTransformTo3D(Transform2D *p2D, Transform3D *p3D) {
    
    if (p2D == NULL) return;
    if (p3D == NULL) return;
    
    p3D->mX = Convert2DXTo3D(p2D->mX + p2D->mOffsetX + p2D->mShiftX);
    p3D->mY = Convert2DYTo3D(p2D->mY + p2D->mOffsetY + p2D->mShiftY);
    //p3D->mZ = ???
    
    p3D->mScale = p2D->mScale * p2D->mOffsetScale;
    p3D->mScaleX = p2D->mScaleX;
    p3D->mScaleY = p2D->mScaleY;
    p3D->mScaleZ = p2D->mScaleZ;
    //p3D->mScaleZ = ???;
    
    p3D->mRotation2D = p2D->mRotation + p2D->mOffsetRotation;
}

FVec3 Game::Convert2DCoordsTo3D(float pX, float pY) {
    FVec3 aResult;
    aResult.mX = Convert2DXTo3D(pX);
    aResult.mY = Convert2DYTo3D(pY);
    aResult.mZ = 0.0f;
    return aResult;
}

float Game::Convert2DXTo3D(float pX) {
    return (mTransformAbsolute.mScale * (pX - mWidth2 + mRenderShiftX) / gDeviceWidth2) * mCamera->mDistance * mCamera->mAspect;
}

float Game::Convert2DYTo3D(float pY) {
    return (mTransformAbsolute.mScale * (pY - mHeight2 + mRenderShiftY) / gDeviceHeight2) * mCamera->mDistance;
}

FVec2 Game::Convert3DCoordsTo2D(float pX, float pY) {
    FVec2 aResult;
    aResult.mX = Convert3DXTo2D(pX);
    aResult.mY = Convert3DYTo2D(pY);
    return aResult;
}

float Game::Convert3DXTo2D(float pX) {
    return (pX * gDeviceWidth2) / (mTransformAbsolute.mScale * mCamera->mDistance * mCamera->mAspect) + mWidth2 - mRenderShiftX;
}

float Game::Convert3DYTo2D(float pY) {
    return ((pY * gDeviceHeight2) / (mTransformAbsolute.mScale * mCamera->mDistance)) + mHeight2 - mRenderShiftY;
}

void Game::CorrectFingerRange(float &pDiffX, float &pDiffY) {
    
    float aDiffX = pDiffX;
    float aDiffY = pDiffY;
    
    float aDist = aDiffX * aDiffX + aDiffY * aDiffY;
    
    if (aDist > SQRT_EPSILON) {
        aDist = sqrtf(aDist);
        
        float aPercent = (aDist - mDartPullbackRangeFingerMin) / (mDartPullbackRangeFingerMax - mDartPullbackRangeFingerMin);
        //Log("aPercent = %f\n", aPercent);
        
        if (aPercent < 0.0f) { aPercent = 0.0f; }
        if (aPercent > 1.0f) { aPercent = 1.0f; }
        
        float aCorrectedDist = mDartPullbackRangeMin + (mDartPullbackRangeMax - mDartPullbackRangeMin) * aPercent;
        
        //Log("Old Dist: %f Correct: %f\n", aDist, aCorrectedDist);
        
        aDiffX /= aDist;
        aDiffY /= aDist;
        
        pDiffX = aDiffX * aCorrectedDist;
        pDiffY = aDiffY * aCorrectedDist;
    }
}


void Game::RealizeObject(GameObject *pObject) {
    if (pObject != NULL) {
        
        //if ((gGameContainer != NULL) && (gGameContainer->mLevelOverlay != NULL)) {
        //    gGameContainer->mLevelOverlay->ObjectDidAppear(pObject->mSectionIndex, pObject->mObjectIndex);
        //}
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            if ((aDart->mKill == 0) && (aDart->mKnockedDown == false) && (aDart->mIdle == false)) {
                pObject->mIgnoreDartList.Add(aDart);
            }
        }
    }
}

void Game::DisposeObject(GameObject *pObject) {
    
    if (pObject == NULL) {
        return;
    }
    
    if (pObject->mKill != 0) {
        return;
    }
    
    //if ((gGameContainer != NULL) && (gGameContainer->mLevelOverlay != NULL)) {
    //    gGameContainer->mLevelOverlay->ObjectDidDisappear(pObject->mSectionIndex, pObject->mObjectIndex);
    //}
    
    /*
    if (pObject->mFormationNode != NULL) {
        if (pObject->mFormationNode->mObject == pObject) {
            pObject->mFormationNode->mObject = NULL;
        }
    }
    */
    
    if (mLUGOObjectList.mCount > 0) {
        mLUGOObjectList.Remove(pObject);
    }
    
#ifndef EDITOR_MODE
    //Log("Disposing: (%s) %x\n", pObject->TypeString().c() , pObject);
#endif
    
    if ((mIsLevelComplete == false) && (mIsGameOver == false)) {
        if (DoesObjectTypeCountTowardsProgress(pObject->mGameObjectType)) {
            mProgress++;
            if (gInterfaceOverlay != NULL) {
                gInterfaceOverlay->NotifyProgressChanged();
            }
        }
    }
    
    pObject->Kill();
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_CANNON) {
        Cannon *aCannon = ((Cannon *)pObject);
        mTempDartList.RemoveAll();
        EnumList(Dart, aDart, aCannon->mStoredDarts) {
            aDart->mCannon = NULL;
            mTempDartList.Add(aDart);
        }
        aCannon->mStoredDarts.RemoveAll();
        EnumList(Dart, aDart, mTempDartList) {
            mStreakController.RecordDartComplete(aDart);
            NotifyDartDidExitScreen(aDart);
            
            DisposeObject(aDart);
        }
        
        EnumList(Dart, aDart, mDartList.mObjectList) {
            if (aDart->mCannon == aCannon) {
                aDart->mCannon = NULL;
            }
        }
        EnumList(Dart, aDart, mDartList.mObjectList) {
            if (aDart->mFiredCannon == aCannon) {
                aDart->mFiredCannon = NULL;
            }
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_TURTLE) {
        Turtle *aTurtle = ((Turtle *)pObject);
        Balloon *aBalloon = aTurtle->mBalloon;
        if ((aTurtle->mKnockedDown == false) && (aBalloon != NULL)) {
            aTurtle->mBalloon = NULL;
            FlyOffEscape(aBalloon);
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BRICKHEAD) {
        BrickHead *aBrickHead = ((BrickHead *)pObject);
        
        mTempDartList.RemoveAll();
        EnumList(StuckDart, aStuck, aBrickHead->mStuckDartList) {
            if (aStuck->mDart != NULL) {
                mTempDartList.Add(aStuck->mDart);
                aStuck->mDart = NULL;
            }
        }
        EnumList(Dart, aDart, mTempDartList) {
            DisposeObject(aDart);
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BALLOON) {
        Balloon *aBalloon = ((Balloon *)pObject);
        EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
            if (aTurtle->mBalloon == aBalloon) {
                aTurtle->mBalloon = NULL;
                
                if ((mIsLevelComplete == true) || (mIsGameOver == true)) {
                    DisposeObject(aTurtle);
                } else {
                    if (aTurtle->mKnockedDown == false) {
                        aTurtle->KnockDown();
                        if (mLUGOObjectList.mCount > 0) { mLUGOObjectList.Remove(aTurtle); }
                        if (mTurtleKnockDownSoundDelay == 0) {
                            mTurtleKnockDownSoundDelay = 2;
                            gApp->mSoundKnockDownTurtle.Play();
                        }
                    }
                    
                }
            }
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_FREE_LIFE) {
        FreeLife *aFreeLife = ((FreeLife *)pObject);
        EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
            if (aTurtle->mBalloon == aFreeLife) {
                aTurtle->mBalloon = NULL;
                
                if ((mIsLevelComplete == true) || (mIsGameOver == true)) {
                    DisposeObject(aTurtle);
                } else {
                    if (aTurtle->mKnockedDown == false) {
                        aTurtle->KnockDown();
                        if (mLUGOObjectList.mCount > 0) { mLUGOObjectList.Remove(aTurtle); }
                        if (mTurtleKnockDownSoundDelay == 0) {
                            mTurtleKnockDownSoundDelay = 2;
                            gApp->mSoundKnockDownTurtle.Play();
                        }
                    }
                }
            }
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_DART) {
        
        Dart *aDart = (Dart *)pObject;
        
        mFlyingDartWaveBlockingDartList.Remove(aDart);
        
        BrickHead *aBrickHead = aDart->mBrickHead;
        if (aBrickHead != NULL) {
            EnumList(StuckDart, aStuck, aBrickHead->mStuckDartList) {
                if (aStuck->mDart == aDart) {
                    aStuck->mDart = NULL;
                }
            }
        }
        
        Cannon *aCannon = aDart->mCannon;
        if (aCannon != NULL) {
            aCannon->mStoredDarts.Remove(aDart);
        }
        
        EnumList(Balloon, aIgnoreObject, mBalloonList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(FreeLife, aIgnoreObject, mFreeLifeList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(BrickHead, aIgnoreObject, mBrickHeadList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(Bomb, aIgnoreObject, mBombList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(Turtle, aIgnoreObject, mTurtleList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(Platform, aIgnoreObject, mPlatformList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(Cannon, aIgnoreObject, mCannonList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
        EnumList(PrizeBox, aIgnoreObject, mPrizeBoxList.mObjectList) {
            if (aIgnoreObject->mIgnoreDartList.Exists(aDart)) {
                aIgnoreObject->mIgnoreDartList.Remove(aDart);
            }
        }
        
    } else {
        if (mLevelData != NULL) {
            mLevelData->DisposeObject(pObject);
        }
    }
    
#ifdef EDITOR_MODE
    if (gEditor != NULL) {
        gEditor->DisposeObject(pObject);
    }
#endif
    
}

void Game::DisposeObjectFromLevelData(GameObject *pObject) {
    if (mLevelData != NULL) {
        mLevelData->DisposeObject(pObject);
    }
}

void Game::KillAllObjects() {
    FList aDiscardList;
    EnumList(GameObject, aObject, mDartList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mBalloonList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mFreeLifeList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mBrickHeadList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mTurtleList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mBombList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mPlatformList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mCannonList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mStarList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mPrizeBoxList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, mBlimpList.mObjectList) { aDiscardList.Add(aObject); }
    EnumList(GameObject, aObject, aDiscardList) {
        if (aObject->mKill == 0) {
            
            aObject->mPermSpawn = NULL;
            aObject->mWaveSpawn = NULL;
            aObject->mWave = NULL;
            aObject->mSection = NULL;
            
            
            aObject->Kill();
            DisposeObjectFromLevelData(aObject);
        }
    }
}

void Game::DisposeAllObjectsWave() {
    EnumList (GameObject, aObject, mDartList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBalloonList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mFreeLifeList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBrickHeadList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mTurtleList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBombList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBlimpList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mPlatformList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mCannonList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mStarList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mPrizeBoxList.mObjectList) {
        if (aObject->mDidOriginateOnWave) { DisposeObject(aObject); }
    }
}

void Game::DisposeAllObjectsPerm() {
    
    EnumList (GameObject, aObject, mDartList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBalloonList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mFreeLifeList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBrickHeadList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mTurtleList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBombList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mBlimpList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mPlatformList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mCannonList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mStarList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    EnumList (GameObject, aObject, mPrizeBoxList.mObjectList) {
        if (aObject->mDidOriginateAsPermanent) { DisposeObject(aObject); }
    }
    
}


void Game::FlyOffEscape(GameObject *pObject) {
    
    if (pObject == NULL) { return; }
    
    //Log("FlyOffEscape(%d)\n", pObject->mGameObjectType);
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_TURTLE) {
        Turtle *aTurtle = ((Turtle *)pObject);
        Balloon *aBalloon = aTurtle->mBalloon;
        if ((aTurtle->mKnockedDown == false) && (aBalloon != NULL)) {
            aTurtle->mBalloon = NULL;
            FlyOffEscape(aBalloon);
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BALLOON) {
        //LifeLose(pObject->m);
        RegisterMissedObject(pObject);
    }
    
    DisposeObject(pObject);
}

void Game::RegisterMissedObject(GameObject *pObject) {
    
    
    if (pObject != NULL) {
        
        if (pObject->mDidRegisterMiss == true) {
            return;
        }
        
        pObject->mDidRegisterMiss = true;
        
        MissedGameObjectCluster *aChosenCluster = NULL;
        
        
        LevelFormation *aFormation = NULL;
        
        if (pObject->mFormationNode != NULL) {
            aFormation = pObject->mFormationNode->mFormation;
        }
        
        float aObjectX = pObject->mTransform.mX;
        float aObjectY = pObject->mTransform.mY;
        
        if (aFormation != NULL) {
            aObjectX = aFormation->mX;
            aObjectY = aFormation->mY;
        
            //Log("Formation Pos: %f, %f\n", aObjectX, aObjectY);
            
        } else {
            //Log("Object Pos: %f, %f\n", aObjectX, aObjectY);
            
        }
        
        LevelSection *aSection = pObject->mSection;
        if (aSection != NULL) {
            if (pObject->mPermSpawn != NULL) {
                aObjectX += aSection->mX;
                aObjectY += aSection->mY;
            }
        }
        
        //if (aObjectX < 0.0f) { aObjectX = 0.0f; }
        //if (aObjectX > mWidth) { aObjectX = mWidth; }
        
        //if (aObjectY < 0.0f) { aObjectY = 0.0f; }
        //if (aObjectY > mHeight) { aObjectY = mHeight; }
        
        
        
        float aThresholdDist = 20.0f * gSpriteDrawScale;
        EnumList(MissedGameObjectCluster, aCluster, mMissedObjectClusterList) {
            if ((aCluster->mWaveSpawn != NULL) && (aCluster->mWaveSpawn == pObject->mWaveSpawn)) {
                aChosenCluster = aCluster;
            } else if ((aCluster->mPermSpawn != NULL) && (aCluster->mPermSpawn == pObject->mPermSpawn)) {
                aChosenCluster = aCluster;
            } else {
                float aDiffX = aCluster->GetCenterX() - aObjectX;
                float aDiffY = aCluster->GetCenterY() - aObjectY;
                float aDistSquared = (aDiffX * aDiffX) + (aDiffY * aDiffY);
                if (aDistSquared < aThresholdDist) {
                    aChosenCluster = aCluster;
                }
            }
        }
        
        if (aChosenCluster == NULL) {
            if (mMissedObjectClusterQueueList.mCount > 0) {
                aChosenCluster = (MissedGameObjectCluster *)(mMissedObjectClusterQueueList.PopLast());
                aChosenCluster->Reset();
            } else {
                aChosenCluster = new MissedGameObjectCluster();
            }
            mMissedObjectClusterList.Add(aChosenCluster);
        }
        
        aChosenCluster->AddObject(pObject, aObjectX, aObjectY);
    }
    
}

void Game::KnockDown(GameObject *pObject) {
    
    if (pObject == NULL) { return; }
    if (pObject->mKill > 0) { return; }
    
    if (mLUGOObjectList.mCount > 0) { mLUGOObjectList.Remove(pObject); }
    
    bool aPlayKnockDown = false;
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BRICKHEAD) {
        BrickHead *aBrickHead = ((BrickHead *)pObject);
        if (aBrickHead->mKnockedDown == false) {
            aBrickHead->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_CANNON) {
        Cannon *aCannon = ((Cannon *)pObject);
        if (aCannon->mKnockedDown == false) {
            aCannon->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BOMB) {
        Bomb *aBomb = ((Bomb *)pObject);
        if (aBomb->mKnockedDown == false) {
            aBomb->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL) {
        PlatformSmall *aPlatform = ((PlatformSmall *)pObject);
        if (aPlatform->mKnockedDown == false) {
            aPlatform->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM) {
        PlatformMedium *aPlatform = ((PlatformMedium *)pObject);
        if (aPlatform->mKnockedDown == false) {
            aPlatform->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE) {
        PlatformLarge *aPlatform = ((PlatformLarge *)pObject);
        if (aPlatform->mKnockedDown == false) {
            aPlatform->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE) {
        PlatformExtraLarge *aPlatform = ((PlatformExtraLarge *)pObject);
        if (aPlatform->mKnockedDown == false) {
            aPlatform->KnockDown();
            aPlayKnockDown = true;
        }
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_TURTLE) {
        Turtle *aTurtle = ((Turtle *)pObject);
        if (aTurtle->mBalloon != NULL && aTurtle->mBalloon->mKill == 0) { PopBalloon(aTurtle->mBalloon); }
        if (aTurtle->mKnockedDown == false) {
            aTurtle->KnockDown();
            if (mTurtleKnockDownSoundDelay == 0) {
                mTurtleKnockDownSoundDelay = 2;
                gApp->mSoundKnockDownTurtle.Play();
            }
        }
    }
    
    if (aPlayKnockDown == true) {
        if (mKnockDownSoundDelay == 0) {
            mKnockDownSoundDelay = 2;
            gApp->mSoundKnockDown.Play();
        }
    }
}

bool Game::IsAnyObjectBeingKnockedDown() {
    bool aResult = false;
    EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mKill == 0) && (aTurtle->mKnockedDown == true)) { aResult = true; }
    }
    EnumList(BrickHead, aBrickHead, mBrickHeadList.mObjectList) {
        if ((aBrickHead->mKill == 0) && (aBrickHead->mKnockedDown == true)) { aResult = true; }
    }
    EnumList(Bomb, aBomb, mBombList.mObjectList) {
        if ((aBomb->mKill == 0) && (aBomb->mKnockedDown == true)) { aResult = true; }
    }
    EnumList(Cannon, aCannon, mCannonList.mObjectList) {
        if ((aCannon->mKill == 0) && (aCannon->mKnockedDown == true)) { aResult = true; }
    }
    EnumList(Platform, aPlatform, mPlatformList.mObjectList) {
        if ((aPlatform->mKill == 0) && (aPlatform->mKnockedDown == true)) { aResult = true; }
    }
    EnumList(PrizeBox, aPrizeBox, mPrizeBoxList.mObjectList) {
        if ((aPrizeBox->mKill == 0) && (aPrizeBox->mKnockedDown == true)) { aResult = true; }
    }
    return aResult;
}

void Game::PlayPopSound() {
    
    if (mPopDelayBalloon == 0) {
        mPopDelayBalloon = 1;
        
        if (mPopSoundAlternateTick > 15) {
            //Some time has passed since the last pop sound...
            mPopSoundIndex = 0;
            gApp->mSoundHitBalloonPrimary.Play();
            
        } else {
            //Pop sounds in rapid succession...
            mPopSoundIndex++;
            if (mPopSoundIndex >= 3) { mPopSoundIndex = 0; }
            
            if (mPopSoundIndex == 1) {
                gApp->mSoundHitBalloonSecondary.Play();
            } else {
                gApp->mSoundHitBalloonPrimary.Play();
            }
        }
        mPopSoundAlternateTick = 0;
    }
}

void Game::PopBalloon(Balloon *pBalloon) {
    
    if (pBalloon != NULL) {
        
        PlayPopSound();
        
        if (gEffectsOverlay != NULL) {
            gEffectsOverlay->GenerateBalloonPopAnimation(pBalloon);
        }
        
        //Important: The turtle loses the balloon before the balloon is disposed...
        EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
            if ((aTurtle->mKill == 0) && (aTurtle->mBalloon == pBalloon)) {
                TurtleDidLoseBalloon(aTurtle);
            }
        }
        
        DisposeObject(pBalloon);
    }
}

void Game::PopFreeLife(FreeLife *pFreeLife) {
    if (pFreeLife != NULL) {
        
        if (mPopDelayFreeLife == 0) {
            mPopDelayFreeLife = 1;
            gApp->mSoundHitBlimp.Play();
            gApp->mSoundFreeLife.Play();
        }
        
        if (gEffectsOverlay != NULL) {
            gEffectsOverlay->GenerateFreeLifePopAnimation(pFreeLife);
        }
        
        if (mLives < mLivesMax) {
            mLives++;
            if (gInterfaceOverlay != NULL) { gInterfaceOverlay->NotifyLivesChanged(); }
        } else {
            if (gInterfaceOverlay != NULL) {
                float aX = pFreeLife->mTransform.mX;
                float aY = pFreeLife->mTransform.mY;
                
                gEffectsOverlay->Get2DPos(aX, aY);
                
                mScore += 1000;
                gEffectsOverlay->SpawnComboBubble(aX, aY, "LUCKY", "+1000");
            }
        }
        
        //Important: The turtle loses the life before the balloon is disposed...
        EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
            if ((aTurtle->mKill == 0) && (aTurtle->mBalloon == pFreeLife)) {
                TurtleDidLoseBalloon(aTurtle);
            }
        }
        
        DisposeObject(pFreeLife);
    }
}

void Game::PopBlimp(Blimp *pBlimp) {
    if (pBlimp != NULL) {
        if (mPopDelayBlimp == 0) {
            mPopDelayBlimp = 1;
            gApp->mSoundHitBlimp.Play();
        }
        
        if (gEffectsOverlay != NULL) {
            gEffectsOverlay->GenerateBlimpPopAnimation(pBlimp);
        }
        DisposeObject(pBlimp);
    }
}

void Game::DartCollideWithBalloon(Dart *pDart, Balloon *pBalloon) {
    
    Turtle *aOwningTurtle = NULL;
    EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mKill == 0) && (aTurtle->mBalloon == pBalloon)) { aOwningTurtle = aTurtle; }
    }
    
    if (pDart != NULL) {
        pDart->mBalloonCount++;
        pDart->mHitCount++;
        pDart->mHitCountRapid++;
        pDart->mStreakCount++;
        
#ifdef SCREENSHOT_MODE
        
        if (pDart->mHitCount == 3) {
            
            float aX = pDart->mTransform.mX;
            float aY = pDart->mTransform.mY;
            
            if (pDart->mRecentCollider) {
                aX = pDart->mRecentColliderX;
                aY = pDart->mRecentColliderY;
            }
            
            gEffectsOverlay->Get2DPos(aX, aY);
            gEffectsOverlay->SpawnComboBubble(aX, aY, "SUPERB", "+100");
        }
        
#endif
        
        if (pDart->mIsFacingUp == true) {
            pDart->mHitCountGoingUp++;
        }
        
        if (pDart->mIsFacingDown == true) {
            pDart->mHitCountGoingDown++;
        }
        
        if (pBalloon != NULL) {
            pDart->mRecentColliderX = pBalloon->mTransform.mX;
            pDart->mRecentColliderY = pBalloon->mTransform.mY;
            pDart->mRecentCollider = true;
        }
        
        if (aOwningTurtle != NULL) {
            pDart->mTurtleBalloonCount++;
            pDart->mHitCountTurtle++;
            pDart->mHitCountRapidTurtle++;
        }
    }
    
    mStreakController.RecordDartHitBalloon(pDart, pBalloon, aOwningTurtle);
    
    HandleStreakUpdate(pDart);
    
    PopBalloon(pBalloon);
    
    mScore += 25;
    if (aOwningTurtle != NULL) { mScore += 25; }
    
    
    
    gAch.ProgressGroup("pop_total");
    if (aOwningTurtle != NULL) { gAch.ProgressGroup("pop_total_turtle"); }
    
    if (pBalloon != NULL) {
        
        if (pBalloon->mStyleIndex == 0) {
            //Log("Red Balloon?\n"); //Yes
            gAch.ProgressGroup("pop_total_red");
        } else if (pBalloon->mStyleIndex == 1) {
            //Log("Green Balloon?\n"); //Yes
            gAch.ProgressGroup("pop_total_green");
        } else if (pBalloon->mStyleIndex == 2) {
            //Log("Blue Balloon?\n"); //Yes
            gAch.ProgressGroup("pop_total_blue");
        } else if (pBalloon->mStyleIndex == 3) {
            //Log("Orange Balloon?\n"); //Yes
            gAch.ProgressGroup("pop_total_orange");
        } else if (pBalloon->mStyleIndex == 4) {
            //Log("Yellow Balloon?\n"); //Yes
            gAch.ProgressGroup("pop_total_yellow");
        } else if (pBalloon->mStyleIndex == 5) {
            //Log("Purple Balloon?\n"); //Yes
            gAch.ProgressGroup("pop_total_purple");
        } else if (pBalloon->mStyleIndex == 6) {
            //Log("Teal Balloon?\n"); //Yes
            //gAch.ProgressGroup("pop_total_teal");
            
        }
    }
    
    
}

void Game::DartCollideWithFreeLife(Dart *pDart, FreeLife *pFreeLife) {
    
    Turtle *aOwningTurtle = NULL;
    EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mKill == 0) && (aTurtle->mBalloon == pFreeLife)) { aOwningTurtle = aTurtle; }
    }
    
    if (pDart != NULL) {
        pDart->mBalloonCount++;
        pDart->mHitCount++;
        pDart->mHitCountRapid++;
        pDart->mStreakCount++;
        
        if (pDart->mIsFacingUp == true) {
            pDart->mHitCountGoingUp++;
        }
        
        if (pDart->mIsFacingDown == true) {
            pDart->mHitCountGoingDown++;
        }
        
        if (pFreeLife != NULL) {
            pDart->mRecentColliderX = pFreeLife->mTransform.mX;
            pDart->mRecentColliderY = pFreeLife->mTransform.mY;
            pDart->mRecentCollider = true;
        }
        
        if (aOwningTurtle != NULL) {
            pDart->mTurtleBalloonCount++;
            pDart->mHitCountTurtle++;
            pDart->mHitCountRapidTurtle++;
        }
    }
    
    mStreakController.RecordDartHitFreeLife(pDart, pFreeLife, aOwningTurtle);
    
    HandleStreakUpdate(pDart);
    
    PopFreeLife(pFreeLife);
    
    mScore += 25;
    if (aOwningTurtle != NULL) { mScore += 25; }
    
    gAch.ProgressGroup("extra_life_total");
    
    if (aOwningTurtle != NULL) { gAch.ProgressGroup("pop_total_turtle"); }
}

void Game::DartCollideWithBrickhead(Dart *pDart, BrickHead *pBrickHead) {
    if (pDart != NULL) {
        
        pDart->mStuck = true;
        
        if (pBrickHead != NULL) {
            
            pDart->mBrickHeadCount++;
            
            mStreakController.RecordDartComplete(pDart);
            mStreakController.RecordDartHitBrickHead(pDart, pBrickHead);
            NotifyDartDidStopStreak(pDart);
            
            pDart->mBrickHead = pBrickHead;
            
            gApp->mSoundHitBrickhead.Play();
            
            StuckDart *aStuck = new StuckDart();
            aStuck->mDart = pDart;
            aStuck->mStartDartRotation = pDart->mTransform.mRotation;
            aStuck->mStartReferenceRotation = (pBrickHead->mTransform.mRotation + pBrickHead->mTransform.mOffsetRotation);
            
            //
            //We don't factor in the dart's offset because that is independent of
            //the collider object. The dart's new position will be dependent on the collider...
            //
            
            float aDeltaX = (pDart->mTransform.mX) - (pBrickHead->mTransform.mX + pBrickHead->mTransform.mOffsetX);
            float aDeltaY = (pDart->mTransform.mY) - (pBrickHead->mTransform.mY + pBrickHead->mTransform.mOffsetY);
            
            aStuck->mStartXDiff = aDeltaX;
            aStuck->mStartYDiff = aDeltaY;
            
            pBrickHead->mStuckDartList.Add(aStuck);
            pBrickHead->Rumble(pDart->mTransform.mRotation);
            
            if (gEffectsOverlay != NULL) {
                gEffectsOverlay->GenerateDartBrickCollideAnimation(pDart);
            }
            
            gAch.ProgressGroup("brick_total");
        }
    }
}

void Game::DartCollideWithBomb(Dart *pDart, Bomb *pBomb) {
    
    if (pDart != NULL) {
        if (pBomb != NULL) {
            
            pDart->mBombCount++;
            
            mStreakController.RecordDartComplete(pDart);
            mStreakController.RecordDartHitBomb(pDart, pBomb);
            NotifyDartDidStopStreak(pDart);
            
            pDart->BlastAway(pBomb->mTransform.mX, pBomb->mTransform.mY);
            
            ExplodeBomb(pBomb);
            
            gAch.ProgressGroup("bomb_total");
        }
    }
}


void Game::DartCollideWithTurtle(Dart *pDart, Turtle *pTurtle) {
    if (pDart != NULL) {
        pDart->KnockDown();
        if (pTurtle != NULL) {
            
            pDart->mTurtleCupCount++;
            
            mStreakController.RecordDartComplete(pDart);
            mStreakController.RecordDartHitTurtleCup(pDart, pTurtle);
            NotifyDartDidStopStreak(pDart);
            
            pTurtle->Impact(pDart->mTransform.mRotation);
            gApp->mSoundHitTurtle.Play();
            
            //gAch.ProgressGroup("turtle_cup_total");
            
        }
    }
}

void Game::DartCollideWithPlatform(Dart *pDart, Platform *pPlatform) {
    if (pDart != NULL) {
        pDart->KnockDown();
        if (pPlatform != NULL) {
            
            pDart->mPlatformCount++;
            
            mStreakController.RecordDartComplete(pDart);
            mStreakController.RecordDartHitPlatform(pDart, pPlatform);
            NotifyDartDidStopStreak(pDart);
            
            pPlatform->Impact(pDart->mTransform.mRotation);
            gApp->mSoundHitPlatform.Play();
        }
    }
}

void Game::DartCollideWithCannon(Dart *pDart, Cannon *pCannon) {
    
    if ((pDart != NULL) && (pCannon != NULL)) {
    
        pDart->mCannonCount++;
        NotifyDartDidStopStreak(pDart);
        
        if (pCannon->mStoredDartsFull == true) {
            mStreakController.RecordDartComplete(pDart);
            pDart->BlastAwayBump(pCannon->mTransform.mX, pCannon->mTransform.mY);
            
            if (gEffectsOverlay != NULL) {
                gEffectsOverlay->GenerateCannonImpactFailAnimation(pCannon);
            }
            
            pCannon->FullImpact(pDart->mTransform.mRotation);
            
            gApp->mSoundHitPlatform.Play();
            
        } else {
            
            mStreakController.RecordDartHitCannon(pDart, pCannon);
            
            pDart->mFiredCannon = NULL;
            pDart->mCannon = pCannon;
            pDart->mIdle = true;
            pDart->mIsHidden = true;
            pCannon->AddStoredDart(pDart);
            pDart->mIsLoadedIntoCannon = true;
            pDart->ResetScreenStatus();
            
            if (gEffectsOverlay != NULL) {
                gEffectsOverlay->GenerateCannonImpactSuccessAnimation(pCannon);
            }
            
            pCannon->Impact(pDart->mTransform.mRotation);
            
            gApp->mSoundHitCannon.Play();
        }
    }
}

void Game::TurtleDidLoseBalloon(Turtle *pTurtle) {
    if (pTurtle != NULL) {
        pTurtle->mBalloon = NULL;
        pTurtle->KnockDown();
        if (mLUGOObjectList.mCount > 0) {
            mLUGOObjectList.Remove(pTurtle);
        }
        if (mTurtleKnockDownSoundDelay == 0) {
            mTurtleKnockDownSoundDelay = 2;
            gApp->mSoundKnockDownTurtle.Play();
        }
    }
}

void Game::FireCannon(Cannon *pCannon) {
    
    if (pCannon == NULL) { return; }
    
    Dart *aDart = pCannon->Fire();
    gApp->mSoundCannonBlast.Play(0.75f);
    
    if (aDart == NULL) { return; }
    
    float aRotation = pCannon->mTransform.mRotation;
    float aHandleLength = 36.0f;
    float aDirX = Sin(aRotation);
    float aDirY = -Cos(aRotation);
    
    aDart->mFiredCannon = pCannon;
    
    aDart->mTransform.mRotation = aRotation;
    aDart->mTargetRotation = aRotation;
    
    aDart->mTransform.mX = pCannon->mTransform.mX + aDirX * aHandleLength;
    aDart->mTransform.mY = pCannon->mTransform.mY + aDirY * aHandleLength;
    
    float aVelocity = mDartReleaseVelocityMin + (mDartReleaseVelocityMax - mDartReleaseVelocityMin) * 0.40f;
    
    aDart->mVelX = aDirX * aVelocity;
    aDart->mVelY = aDirY * aVelocity;
    aDart->GetTipPoint(aDart->mTipX, aDart->mTipY);
    aDart->mPrevTipX = aDart->mTipX;
    aDart->mPrevTipY = aDart->mTipY;
    
    aDart->mIsHidden = false;
    aDart->mIdle = false;
    aDart->mIsLoadedIntoCannon = false;
    aDart->mShotFromCannon = true;
    
    Rumble(3.0f * gSpriteDrawScale);
    
    if (gEffectsOverlay != NULL) { gEffectsOverlay->GenerateCannonBlastAnimation(pCannon); }
    
    gAch.ProgressGroup("cannon_total");
    
}

void Game::ExplodeBomb(Bomb *pBomb) {
    if (pBomb == NULL) { return; }
    
    if (gEffectsOverlay != NULL) {
        gEffectsOverlay->GenerateBombExplodeAnimation(pBomb);
        
        float aX = 0.0f, aY = 0.0f;
        gEffectsOverlay->Get2DPos(pBomb, aX, aY);
        
        gEffectsOverlay->SpawnLifeBubble(aX, aY, false, 1);
    }
    
    Rumble(5.0f * gSpriteDrawScale);
    
    LifeLose(1);
    
    gApp->mSoundHitBomb.Play();
    
    DisposeObject(pBomb);
}

//

void Game::GetAllGameObjectsLUGO(FList *pList) {
    
    if (pList == NULL) {
        return;
    }
    
    FList aIgnoreList;
    
    EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mKill == 0) && (aTurtle->mKnockedDown == false)) {
            pList->Add(aTurtle);
            Balloon *aBalloon = aTurtle->mBalloon;
            //aTurtle->mBalloon = NULL;
            if (aBalloon != NULL) {
                aIgnoreList.Add(aBalloon);
            }
        }
    }
    
    EnumList (FreeLife, aFreeLife, mFreeLifeList.mObjectList) {
        if (aFreeLife->mKill == 0) {
            if (aIgnoreList.Exists(aFreeLife) == false) {
                pList->Add(aFreeLife);
            }
        }
    }
    
    EnumList(Balloon, aBalloon, mBalloonList.mObjectList) {
        if (aBalloon->mKill == 0) {
            if (aIgnoreList.Exists(aBalloon) == false) {
                pList->Add(aBalloon);
            }
        }
    }
    
    EnumList(BrickHead, aBrickHead, mBrickHeadList.mObjectList) {
        if ((aBrickHead->mKill == 0) && (aBrickHead->mKnockedDown == false)) {
            pList->Add(aBrickHead);
        }
    }
    
    EnumList(Bomb, aBomb, mBombList.mObjectList) {
        if (aBomb->mKill == 0) {
            pList->Add(aBomb);
        }
    }
    
    EnumList(Cannon, aCannon, mCannonList.mObjectList) {
        if ((aCannon->mKill == 0) && (aCannon->mKnockedDown == false)) {
            pList->Add(aCannon);
        }
    }
    
    EnumList(Platform, aPlatform, mPlatformList.mObjectList) {
        if ((aPlatform->mKill == 0) && (aPlatform->mKnockedDown == false)) {
            pList->Add(aPlatform);
        }
    }
    
    EnumList(PrizeBox, aPrizeBox, mPrizeBoxList.mObjectList) {
        if ((aPrizeBox->mKill == 0) && (aPrizeBox->mKnockedDown == false)) {
            pList->Add(aPrizeBox);
        }
    }
    
    HeightSortGameObjects(pList);
}

GameObject *Game::ObjectForOverlay(int pSectionIndex, int pObjectIndex) {
    GameObject *aResult = NULL;
    EnumList (FreeLife, aFreeLife, mFreeLifeList.mObjectList) {
        if ((aFreeLife->mSectionIndex == pSectionIndex) && (aFreeLife->mObjectIndex == pObjectIndex)) { aResult = aFreeLife; }
    }
    EnumList(Balloon, aBalloon, mBalloonList.mObjectList) {
        if ((aBalloon->mSectionIndex == pSectionIndex) && (aBalloon->mObjectIndex == pObjectIndex)) { aResult = aBalloon; }
    }
    EnumList(Turtle, aTurtle, mTurtleList.mObjectList) {
        if ((aTurtle->mSectionIndex == pSectionIndex) && (aTurtle->mObjectIndex == pObjectIndex)) { aResult = aTurtle; }
    }
    EnumList(BrickHead, aBrickHead, mBrickHeadList.mObjectList) {
        if ((aBrickHead->mSectionIndex == pSectionIndex) && (aBrickHead->mObjectIndex == pObjectIndex)) { aResult = aBrickHead; }
    }
    EnumList(Bomb, aBomb, mBombList.mObjectList) {
        if ((aBomb->mSectionIndex == pSectionIndex) && (aBomb->mObjectIndex == pObjectIndex)) { aResult = aBomb; }
    }
    EnumList(Cannon, aCannon, mCannonList.mObjectList) {
        if ((aCannon->mSectionIndex == pSectionIndex) && (aCannon->mObjectIndex == pObjectIndex)) { aResult = aCannon; }
    }
    EnumList(Platform, aPlatform, mPlatformList.mObjectList) {
        if ((aPlatform->mSectionIndex == pSectionIndex) && (aPlatform->mObjectIndex == pObjectIndex)) { aResult = aPlatform; }
    }
    EnumList(PrizeBox, aPrizeBox, mPrizeBoxList.mObjectList) {
        if ((aPrizeBox->mSectionIndex == pSectionIndex) && (aPrizeBox->mObjectIndex == pObjectIndex)) { aResult = aPrizeBox; }
    }
    return aResult;
}

bool Game::IsObjectForOverlayResting(int pSectionIndex, int pObjectIndex) {
    GameObject *aObject = ObjectForOverlay(pSectionIndex, pObjectIndex);
    if (aObject != NULL) {
        LevelSection *aSection = GetSectionForObject(aObject);
        if (aSection != NULL) {
            if (aSection->IsResting() == true) {
                return true;
            }
        }
        if (aObject->mDidOriginateOnWave == true) {
            LevelWave *aWave = GetWaveForObject(aObject);
            if (aWave != NULL) {
                if (aWave->IsResting(aObject) == true) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Game::HeightSortGameObjects(FList *pList) {
    if (pList == NULL) { return; }
    GameObject *aHold = NULL;
    int i = 0;
    int j = 0;
    for (i = 1; i<pList->mCount; i++) {
        aHold = (GameObject *)(pList->mData[i]);
        j = i;
        while ((j>0) && HeightCompareGameObjects( (GameObject *)(pList->mData[j - 1]), aHold)) {
            pList->mData[j] = pList->mData[j-1];
            j--;
        }
        pList->mData[j] = aHold;
    }
}

bool Game::HeightCompareGameObjects(GameObject *pObject1, GameObject *pObject2) {
    if (fabsf(pObject1->mTransform.mY - pObject2->mTransform.mY) < 2.0f) {
        return (pObject1->mTransform.mX > pObject2->mTransform.mX);
    } else if (pObject1->mTransform.mY > pObject2->mTransform.mY) {
        return true;
    } else {
        return false;
    }
}

void Game::LifeLose(int pCount) {
    
    if (mIsGameOver == true) {
        return;
    }
    
    if (mIsLevelComplete == true) {
        return;
    }
    
    if (pCount < 0) { pCount = 0; }
    
    if (pCount > 0) {
        gApp->mSoundLoseLife.Play();
    }
    
    mLivesLost += pCount;
    mLives -= pCount;
    
    if (mLives <= 0) { mLives = 0; }
    if (gInterfaceOverlay != NULL) { gInterfaceOverlay->NotifyLivesChanged(); }
    if (ShouldBeginGameOver()) { BeginGameOver(); }
    
}

void Game::RecordFlyingDartsForWaveBlocking() {
    mFlyingDartWaveBlockingDartList.RemoveAll();
    for (int i=0;i<mDartList.mObjectList.mCount;i++) {
        Dart *aDart = (Dart *)mDartList.mObjectList.mData[i];
        if (aDart->mKill == 0) {
            mFlyingDartWaveBlockingDartList.Add(aDart);
        }
    }
}

void Game::ReleaseDart() {
    
    if (mCurrentDart != NULL) {
        if (mTrajectoryExists == true) {
            
            mCurrentDart->Fling(mReleaseDirX * mReleaseVelocity, mReleaseDirY * mReleaseVelocity);
            
            mCurrentDart->mSharpTurn = mTrajectoryIsNarrow;
            
            mCurrentDart->RefreshScreenStatus();
            
            mDartList.Add(mCurrentDart);
            
            gApp->mSoundDartPullback.Stop();
            gApp->mSoundDartRelease[gRand.Get(3)].Play();
            
            
            mStreakController.RecordDartThrown(mCurrentDart);
            mCurrentDart = NULL;
            
        }
        
        mDartTouch = NULL;
        ResetDartTouch();
        
        mCurrentDartRespawnTimer = 8;
    }
}

void Game::ResetDartTouch() {
    
    if (gInterfaceOverlay != NULL) {
        gInterfaceOverlay->mMagicArrow.Hide();
    }
    
    if (mDartTouch != NULL) {
        
        mDartTouch = NULL;
        mIsDartBeingPulled = false;
        
        mDartResetAnimation = true;
        mDartResetAnimationTick = 0;
        
        mDartResetStartPullX = mDartPullX;
        mDartResetStartPullY = mDartPullY;
        mDartResetStartRotation = mDartPullRotation;
        
        mDartResetRotationLength = DistanceBetweenAngles(mDartResetStartRotation, 0.0f);
        
        float aFactorRotation = fabsf(mDartResetRotationLength) / 120.0f;
        if (aFactorRotation > 1.0f) { aFactorRotation = 1.0f; }
        
        float aPullLength = Distance(0.0f, 0.0f, mDartPullX, mDartPullY);
        
        float aFactorDistance = aPullLength / ((mDartPullbackRangeMin + mDartPullbackRangeMax) * 0.38f);
        if (aFactorDistance > 1.0f) { aFactorDistance = 1.0f; }
        
        float aAnimationFactor = aFactorRotation;
        if (aFactorDistance > aFactorRotation) aAnimationFactor = aFactorDistance;
        
        float aTime = 5.0f + aAnimationFactor * 40.0f;
        
        mDartResetAnimationTime = (int)(aTime + 0.5f);
        
    } else {
        mDartTouch = NULL;
        mIsDartBeingPulled = false;
        mDartResetAnimation = false;
        mDartResetAnimationTick = 0;
        mDartPullX = 0.0f;
        mDartPullY = 0.0f;
        mDartPullRotation = 0.0f;
        mDartTargetPullRotation = 0.0f;
        mDartTargetPullX = 0.0f;
        mDartTargetPullY = 0.0f;
    }
}

LevelSection *Game::GetSectionForObject(GameObject *pObject) {
    LevelSection *aResult = NULL;
    if (mLevelData != NULL) {
        aResult = mLevelData->GetSectionForObject(pObject);
    }
    return aResult;
}

LevelWave *Game::GetWaveForObject(GameObject *pObject) {
    LevelWave *aResult = NULL;
    if (mLevelData != NULL) {
        aResult = mLevelData->GetWaveForObject(pObject);
    }
    return aResult;
}

LevelSectionPerm *Game::GetPermForObject(GameObject *pObject) {
    LevelSectionPerm *aResult = NULL;
    if (mLevelData != NULL) {
        aResult = mLevelData->GetPermForObject(pObject);
    }
    return aResult;
}

bool Game::IsGameObjectOutsideKillZone(GameObject *pObject) {
    if (pObject != NULL) {
        return IsPositionOutsideKillZone(pObject->mTransform.mX, pObject->mTransform.mY);
    }
    return false;
}

bool Game::IsGameObjectOutsideOffScreenZone(GameObject *pObject) {
    if (pObject != NULL) {
        return IsPositionOutsideOffScreenZone(pObject->mTransform.mX, pObject->mTransform.mY);
    }
    return false;
}

bool Game::IsGameObjectOutsideOffScreenZoneTop(GameObject *pObject) {
    if (pObject != NULL) {
        return IsPositionOutsideOffScreenZoneTop(pObject->mTransform.mX, pObject->mTransform.mY);
    }
    return false;
}



bool Game::IsPositionOutsideKillZone(float pX, float pY) {
    if (pX < mKillZoneLeft) { return true; }
    if (pX > mKillZoneRight) { return true; }
    if (pY < mKillZoneTop) { return true; }
    if (pY > mKillZoneBottom) { return true; }
    return false;
}

bool Game::IsPositionOutsideOffScreenZone(float pX, float pY) {
    if (pX < mOffScreenAreaTop) { return true; }
    if (pX > mOffScreenAreaRight) { return true; }
    if (pY < mOffScreenAreaLeft) { return true; }
    if (pY > mOffScreenAreaBottom) { return true; }
    return false;
}

bool Game::IsPositionOutsideOffScreenZoneTop(float pX, float pY) {
    if (pY < mOffScreenAreaTop) { return true; }
    return false;
    
}

bool Game::IsClearForShortCircuitExternal() {
    if (mLevelData != NULL && mLevelData->mCurrentSection != NULL) {
        return mLevelData->mCurrentSection->IsClearForShortCircuitExternal();
    }
    return false;
}

void Game::Rumble(float pRumble) {
    mRumbleMagnitude = pRumble;
    RumbleRefresh();
}

void Game::RumbleRefresh() {
    float aRotation = gRand.GetRotation();
    float aDirX = Sin(aRotation);
    float aDirY = -Cos(aRotation);
    mRumbleX = aDirX * mRumbleMagnitude;
    mRumbleY = aDirY * mRumbleMagnitude;
}

void Game::TestDestroyNearestObject(float pX, float pY) {
    FList aList;
    TestDestroyBuildListOfObjects(&aList);
    
    GameObject *aPick = NULL;
    float aBestDistance = 250.0f * 250.0f;
    
    EnumList(GameObject, aObject, aList) {
        float aDist = DistanceSquared(aObject->mTransform.mX, aObject->mTransform.mY, pX, pY);
        if (aDist < aBestDistance) {
            aPick = aObject;
            aBestDistance = aDist;
        }
    }
    
    if (aPick == NULL) { return; }
    
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL) { return; }
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM) { return; }
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE) { return; }
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE) { return; }
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_CANNON) { return; }
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_BRICKHEAD) { return; }
    if (aPick->mGameObjectType == GAME_OBJECT_TYPE_BOMB) { return; }
    
    DestroyGameObject(aPick);
}

void Game::TestDestroyBuildListOfObjects(FList *pList) {
    
    if (pList == NULL) { return; }
    
    EnumList (FreeLife, aFreeLife, mFreeLifeList.mObjectList) {
        if (aFreeLife->mKill == 0) {
            pList->Add(aFreeLife);
        }
    }
    
    EnumList(Balloon, aBalloon, mBalloonList.mObjectList) {
        if (aBalloon->mKill == 0) {
            pList->Add(aBalloon);
        }
    }
    
    EnumList(BrickHead, aBrickHead, mBrickHeadList.mObjectList) {
        if (aBrickHead->mKill == 0) {
            pList->Add(aBrickHead);
        }
    }
    
    EnumList(Bomb, aBomb, mBombList.mObjectList) {
        if (aBomb->mKill == 0) {
            pList->Add(aBomb);
        }
    }
    
    EnumList(Cannon, aCannon, mCannonList.mObjectList) {
        if ((aCannon->mKill == 0) && (aCannon->mStoredDarts.mCount == 0)) {
            pList->Add(aCannon);
        }
    }
    
    EnumList(Platform, aPlatform, mPlatformList.mObjectList) {
        if (aPlatform->mKill == 0) {
            pList->Add(aPlatform);
        }
    }
}

bool Game::ShouldBeginGameOver() {
    if (mInvulnerable == true) { return false; }
    if (mIsGameOver == true) { return false; }
    if (mIsLevelComplete == true) { return false; }
    return (mLives <= 0);
}

bool Game::ShouldBeginLevelComplete() {
    if (mIsLevelComplete == true) { return false; }
    if (mIsGameOver == true) { return false; }
    if (mDidLoad == false) { return false; }
    if (mLevelData != NULL) {
        if (mLevelData->mIsComplete == true) {
            return true;
        }
    }
    
    return false;
}

void Game::BeginGameOver() {
    
#ifdef EDITOR_MODE
    return;
#endif
    
    mIsGameOver = true;
    
    ResetDartTouch();
    
    if (mGameOverBlimp != NULL) {
        delete mGameOverBlimp;
        mGameOverBlimp = NULL;
    }
    
    gGameContainer->ShowGameOverOverlay();
    
    mGameOverMode = 0;
    mGameOverTimer = 0;
    
    mGameOverDidShowMenu = false;
    
    mGameOverBlimpSpawnTimer = 56;
    mGameOverDidSpawnBlimp = false;
    
    mGameOverDidStartDriftAway = false;
    mGameOverDidCompleteDriftAway = false;
    
    if (mLevelData != NULL) {
        mLevelData->Halt();
    }
    
    EnumList(Dart, aDart, mDartList.mObjectList) {
        if (aDart->mKill == 0) {
            mStreakController.RecordDartComplete(aDart);
        }
    }
    
    mLUGOObjectList.RemoveAll();
    GetAllGameObjectsLUGO(&mLUGOObjectList);
    mLUGOObjectList.Reverse();
    
    mStreakController.GameOver();
    WriteLUGOStats();
}


void Game::DestroyGameObject(GameObject *pObject) {
    
    if (pObject == NULL) {
        return;
    }
    
    if (pObject->mGameObjectType == GAME_OBJECT_TYPE_TURTLE) {
        KnockDown(pObject);
    } else if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BRICKHEAD) {
        KnockDown(pObject);
    } else if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BALLOON) {
        Balloon *aBalloon = ((Balloon *)pObject);
        PopBalloon(aBalloon);
    } else if (pObject->mGameObjectType == GAME_OBJECT_TYPE_CANNON) {
        KnockDown(pObject);
    } else if ((pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_SMALL) ||
               (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_MEDIUM) ||
               (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_LARGE) ||
               (pObject->mGameObjectType == GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE)) {
        KnockDown(pObject);
    } else if (pObject->mGameObjectType == GAME_OBJECT_TYPE_FREE_LIFE) {
        FreeLife *aFreeLife = ((FreeLife *)pObject);
        PopFreeLife(aFreeLife);
    } else if (pObject->mGameObjectType == GAME_OBJECT_TYPE_BOMB) {
        Bomb *aBomb = ((Bomb *)pObject);
        ExplodeBomb(aBomb);
    } else {
        DisposeObject(pObject);
    }
}

void Game::BurstGameOverBlimp() {
    
    mGameOverDidSpawnBlimp = true;
    if (mGameOverBlimp != NULL) {
        PopBlimp(mGameOverBlimp);
        delete mGameOverBlimp;
        mGameOverBlimp = NULL;
    }
}

void Game::UpdateGameOver() {
    
    
    if (mGameOverDidStartDriftAway == true) {
        LevelCompleteUpdateDriftAway();
    }
    
    if (mGameOverMode == 0) {
        mGameOverTimer++;
        if (mGameOverTimer > 40) {
            mGameOverTimer = 0;
            mGameOverMode = 1;
            
            LevelCompleteBeginDriftAway();
        }
    } else if (mGameOverMode == 1) {
        
        if (mGameOverDidSpawnBlimp == false) {
        
            mGameOverBlimpSpawnTimer--;
            if (mGameOverBlimpSpawnTimer <= 0) {
                mGameOverDidSpawnBlimp = true;
                mGameOverBlimpSpawnTimer = 0;
                mGameOverBlimp = new Blimp();
                mGameOverBlimp->BeginGameOverSequence();
            }
        }
    }
    
    //mGameOverDidSpawnBlimp
    
    /*
    //if (mGameOverMode >= 1) {
    if (true) {
        
        mGameOverBlimpTimer++;
        //if ((mGameOverBlimpTimer >= 140) && (mGameOverBlimp == NULL) && (mGameOverDidSpawnBlimp == false)) {
        if ((mGameOverBlimpTimer >= 0) && (mGameOverBlimp == NULL) && (mGameOverDidSpawnBlimp == false) && (mLUGOObjectList.mCount <= 2)) {
            mGameOverDidSpawnBlimp = true;
            mGameOverBlimp = new Blimp();
            mGameOverBlimp->BeginGameOverSequence();
        }
    }
    
    if (mGameOverMode == 0) {
        mGameOverModeTimer++;
        if (mGameOverModeTimer > 120) {
            mGameOverModeTimer = 0;
            mGameOverMode = 1;
        }
    } else if (mGameOverMode == 1) {
        mGameOverStaggerTeardownTimer--;
        if (mGameOverStaggerTeardownTimer <= 0) {
            mGameOverStaggerTeardownTimer = mGameOverStaggerTeardownTime;
            
            Log("ABOUT TO DISPOSE OBJECT [%d LEFT]\n", mLUGOObjectList.mCount);
            
            if (mLUGOObjectList.mCount == 0) {
                
                mGameOverMode = 2;
                mGameOverModeTimer = 0;
            } else {
                GameObject *aObject = ((GameObject *)mLUGOObjectList.PopLast());
                DestroyGameObject(aObject);
            }
        }
    } else if (mGameOverMode == 2) {
        
        bool aFinished = true;
        
        if (mGameOverBlimp != NULL) {
            if (mGameOverBlimp->mGameOverAnimationComplete == false) {
                aFinished = false;
            } else {
                PopBlimp(mGameOverBlimp);
                delete mGameOverBlimp;
                mGameOverBlimp = NULL;
            }
        }
        
        if (aFinished == true) {
            
            mGameOverModeTimer++;
            if (mGameOverModeTimer >= 110) {
                mGameOverModeTimer = 110;
                
                if ((mGameOverDidShowMenu == false) && (gGameContainer != NULL) && (gGameContainer->CanShowGameOverMenu() == true)) {
                    
#ifndef EDITOR_MODE
                    mGameOverDidShowMenu = true;
                    gGameContainer->ShowGameOverMenu();
#endif
                    
                }
            }
        }
    }
    */
     
}

bool Game::LevelCompleteShouldDriftAway() {
    
    if (mLUGOObjectList.mCount > 0) {
        return true;
    }
    return false;
}

void Game::LevelCompleteBeginDriftAway() {
    
    float aGameWidth2 = mWidth * 0.5f;
    float aGameHeight2 = mHeight * 0.5f;
    
    mLevelCompleteDidStartDriftAway = true;
    mGameOverDidStartDriftAway = true;
    
    EnumList(GameObject, aObject, mLUGOObjectList) {
        float aPercentX = (aObject->mTransform.mX - aGameWidth2) / aGameWidth2;
        float aPercentY = (aObject->mTransform.mY - aGameHeight2) / aGameHeight2;
        
        aObject->mFloatAway = true;
        
        aObject->mFloatAwaySpeedX = 0.0f;
        aObject->mFloatAwaySpeedY = 0.0f;
        
        aObject->mFloatAwayStartFactorX = aPercentX;
        aObject->mFloatAwayStartFactorY = aPercentY;
    }
}

void Game::LevelCompleteUpdateDriftAway() {
    
    EnumList(GameObject, aObject, mLUGOObjectList) {
        
        aObject->mTransform.mX += aObject->mFloatAwaySpeedX;
        aObject->mTransform.mY += aObject->mFloatAwaySpeedY;
        
        aObject->mFloatAwayStartFactorX *= 0.92f;
        aObject->mFloatAwayStartFactorY *= 0.92f;
        
        aObject->mFloatAwaySpeedX += aObject->mFloatAwayStartFactorX * 0.0125f;
        aObject->mFloatAwaySpeedY += aObject->mFloatAwayStartFactorY * 0.0125f;
        
        aObject->mFloatAwaySpeedY -= 0.041425f;
        aObject->mFloatAwaySpeedY *= 1.00125f;
        
        // Fast Mode...
        //aObject->mFloatAwaySpeedY -= 0.141425f;
        //aObject->mFloatAwaySpeedY *= 1.10125f;
        
    }
    
    mTempObjectList.RemoveAll();
    
    EnumList(GameObject, aObject, mLUGOObjectList) {
        if (aObject->mTransform.mY < (-60.0f)) {
            mTempObjectList.Add(aObject);
        }
    }
    
    EnumList(GameObject, aObject, mTempObjectList) {
        DisposeObject(aObject);
    }
    
}

void Game::UpdateLevelComplete() {
    
    if (mLevelCompleteDidStartDriftAway == true) {
        LevelCompleteUpdateDriftAway();
        
    }
    
    if (mLevelCompleteMode == 0) {
        mLevelCompleteTimer++;
        if (mLevelCompleteTimer > 40) {
            mLevelCompleteTimer = 0;
            mLevelCompleteMode = 1;
            
            LevelCompleteBeginDriftAway();
        }
    } else if (mLevelCompleteMode == 1) {
        
        if (mLevelCompleteDidCompleteDriftAway == false) {
            
            if (mLUGOObjectList.mCount <= 0) {
                mLevelCompleteDidCompleteDriftAway = true;
                if ((gGameContainer != NULL) && (gGameContainer->mLevelCompleteOverlay != NULL)) {
                    gGameContainer->mLevelCompleteOverlay->Start();
                }
            }
        }
    }
    
}

void Game::BeginLevelComplete() {
    
#ifdef EDITOR_MODE
    return;
#endif
    
    EnumList(Dart, aDart, mDartList.mObjectList) {
        if ((aDart->mKill == 0) && (aDart->mHitCount > 0)) {
            if ((aDart->mBrickHead == NULL) && (aDart->mIdle == false) && (aDart->mKill == 0) && (aDart->mStuck == false) && (aDart->mKnockedDown == false) && (aDart->mIsHidden == false)) {
                NotifyDartDidExitScreen(aDart);
            }
        }
    }
    
    mIsLevelComplete = true;
    
    gGameContainer->ShowLevelCompleteOverlay();
    
    ResetDartTouch();
    
    if (mLevelData != NULL) {
        mLevelData->Halt();
    }
    
    mLevelCompleteMode = 0;
    mLevelCompleteTimer = 0;
    
    mLevelCompleteDidStartDriftAway = false;
    mLevelCompleteDidCompleteDriftAway = false;
    
    mLevelCompleteDriftAwayTimer = 0;
    
    EnumList(Dart, aDart, mDartList.mObjectList) {
        if (aDart->mKill == 0) {
            mStreakController.RecordDartComplete(aDart);
        }
    }
    mStreakController.LevelUp();
    
    mLUGOObjectList.RemoveAll();
    GetAllGameObjectsLUGO(&mLUGOObjectList);
    
    WriteLUGOStats();
}

bool Game::DoesObjectTypeCountTowardsProgressCount(int pObjectType) {
    if (pObjectType == GAME_OBJECT_TYPE_BALLOON) { return true; }
    if (pObjectType == GAME_OBJECT_TYPE_TURTLE) { return true; }
    return false;
}

bool Game::DoesObjectTypeCountTowardsProgress(int pObjectType) {
    if (pObjectType == GAME_OBJECT_TYPE_BALLOON) { return true; }
    if (pObjectType == GAME_OBJECT_TYPE_FREE_LIFE) { return true; }
    return false;
}

bool Game::DoesObjectTypeRequireClearing(int pObjectType) {
    
    if (pObjectType == GAME_OBJECT_TYPE_BALLOON) { return true; }
    if (pObjectType == GAME_OBJECT_TYPE_TURTLE) { return true; }
    if (pObjectType == GAME_OBJECT_TYPE_FREE_LIFE) { return true; }
    
    return false;
}

void Game::Restart() {
    
    KillAllObjects();
    
    if (mLevelData != NULL) {
        delete mLevelData;
        mLevelData = NULL;
    }
    
    if (mLives < mLivesMax) {
        mLives = mLivesMax;
    }
    
    mLivesLost = 0;
    
    mStreakController.Reset();
    
    CancelLevelUpAndGameOver();
    
#ifndef EDITOR_MODE
    Load();
#else
    LoadEditorTest();
#endif
    
}

void Game::Load() {
    
#ifdef LIGHT_CONFIG
    
    Log("Game::BLOCKING LOAD..( LIGHT_CONFIG )\n");
    return;
    
#endif
    
    Clear();
    
    Level *aLevel = gLevelCollection.GetLevel(gApp->mLevelID);
    /*
    if (aLevel != NULL) {
        FString aLevelName = gLevelCollection.GetLevelName(gApp->mLevelID);
        Log("Loading Up Level [ %s ] [ %d ]\n", aLevelName.c(), gApp->mLevelID);
    }
    */
    
    
    if (gGameContainer != NULL) {
        gGameContainer->SetUpForLevel(aLevel);
    }
    
    mLevelData = aLevel->Build();
    mProgressCount = mLevelData->GetProgressCount();
    mProgress = 0;
    mDidLoad = true;
    
    if (gInterfaceOverlay != NULL) {
        gInterfaceOverlay->NotifyDidLoad();
    }
    
    delete aLevel;
    
    /*
#ifdef CALIBRATION_MODE
    SaveState();
#endif
    */
    
}


void Game::LoadEditorTest() {
    Clear();
    CancelLevelUpAndGameOver();
    
    LevelSection *aSection = NULL;
    LevelData *aLevel = new LevelData();
    
    aSection = new LevelSection();
    aSection->Load("editor_test_section.json");
    aLevel->AddSection(aSection);
    
    mLevelData = aLevel;
    mProgressCount = mLevelData->GetProgressCount();
    mProgress = 0;
    
    mDidLoad = true;
    
    if (gInterfaceOverlay != NULL) {
        gInterfaceOverlay->NotifyDidLoad();
    }
}

void Game::WriteLUGOStats() {
    
    LEVEL_ID aLevelID = gApp->mLevelID;
    int aLevelIndex = LevelCollection::GetLevelIndex(aLevelID);
    
    
    gApp->mRecentLevelIndex = aLevelIndex;
    gApp->mRecentLevelNumber = aLevelIndex + 1;
    
    
    //mLivesLost
    
    gApp->mPreviousLevelTime = gMeta.GetBestTime(aLevelID);
    gApp->mPreviousLevelScore = gMeta.GetBestScore(aLevelID);
    gApp->mPreviousLevelStars = gMeta.GetBestStars(aLevelID);
    gApp->mPreviousLevelBestStreak = gMeta.GetBestBestStreak(aLevelID);
    gApp->mPreviousLevelBestShot = gMeta.GetBestBestSingleShot(aLevelID);
    gApp->mPreviousLevelAccuracy100000 = gMeta.GetBestAccuracy(aLevelID);
    
    /*
    Log("LUGO PREV TIME: %d\n", gApp->mPreviousLevelTime);
    Log("LUGO PREV SCORE: %d\n", gApp->mPreviousLevelScore);
    Log("LUGO PREV STARS: %d\n", gApp->mPreviousLevelStars);
    Log("LUGO PREV STREAK: %d\n", gApp->mPreviousLevelBestStreak);
    Log("LUGO PREV SHOT: %d\n", gApp->mPreviousLevelBestShot);
    Log("LUGO PREV ACCURACY: %d\n", gApp->mPreviousLevelAccuracy100000);
    */
    
    int aBonusTime = 7500;
    int aBonusAccuracy = 5000;
    
    gApp->mRecentLevelScoreBonusTime = aBonusTime;
    gApp->mRecentLevelScoreBonusAccuracy = aBonusAccuracy;
    
    gApp->mRecentLevelGameOver = mIsGameOver;
    
    gApp->mRecentLevelTime = mTime;
    gApp->mRecentLevelLivesLost = mLivesLost;
    gApp->mRecentLevelScoreBase = mScore;
    gApp->mRecentLevelScore = mScore + aBonusTime + aBonusAccuracy;
    gApp->mRecentLevelMissCount = mMissCount;
    
    gApp->mRecentLevelBalloonPopCount = mStreakController.mNumberOfBalloonsPopped;
    gApp->mRecentLevelTurtlePopCount = mStreakController.mNumberOfTurtlesPopped;
    gApp->mRecentLevelDartsThrownCount = mStreakController.mNumberOfDartsCompleted;
    gApp->mRecentLevelBestStreak = mStreakController.mBestBalloonStreak;
    gApp->mRecentLevelBestShot = mStreakController.mBestShot;
    gApp->mRecentLevelAccuracy100000 = mStreakController.GetAccuracy100000();
    gApp->mRecentLevelStars = gStarsData.GetStarsForRecentLevel(aLevelID);
    
    /*
    Log("LUGO RECENT TIME: %d\n", gApp->mRecentLevelTime);
    Log("LUGO RECENT SCORE: %d\n", gApp->mRecentLevelScore);
    Log("LUGO RECENT STARS: %d\n", gApp->mRecentLevelStars);
    Log("LUGO RECENT ACCURACY: %d (%s)\n", gApp->mRecentLevelAccuracy100000, gMeta.FormatAccuracyString(gApp->mRecentLevelAccuracy100000));
    Log("LUGO RECENT STREAK: %d\n", gApp->mRecentLevelBestStreak);
    Log("LUGO RECENT SHOT: %d\n", gApp->mRecentLevelBestShot);
    Log("LUGO RECENT DARTS THROWN: %d\n", gApp->mRecentLevelDartsThrownCount);
    */
    
    float aHitRatio = 0.0f;
    if (gApp->mRecentLevelDartsThrownCount > 0) {
        aHitRatio = ((float)(gApp->mRecentLevelBalloonPopCount)) / ((float)(gApp->mRecentLevelDartsThrownCount));
    }
    
    FString aLevelIDString = LevelCollection::GetLevelName(aLevelID);
    
    if (mIsLevelComplete == true) {
        
        gMeta.NotifyRecentTime(aLevelID, gApp->mRecentLevelTime);
        gMeta.NotifyRecentScore(aLevelID, gApp->mRecentLevelScore);
        gMeta.NotifyRecentStars(aLevelID, gApp->mRecentLevelStars);
        gMeta.NotifyRecentAccuracy(aLevelID, gApp->mRecentLevelAccuracy100000);
        gMeta.NotifyRecentBestStreak(aLevelID, gApp->mRecentLevelBestStreak);
        gMeta.NotifyRecentBestSingleShot(aLevelID, gApp->mRecentLevelBestShot);
        
        gMeta.NotifyRecentComplete(aLevelID);
    } else if (mIsGameOver == true) {
        gMeta.NotifyRecentGameOver(aLevelID);
    }
    
    gMeta.ScanRecentCompletionsForAchievements();
    
    gMeta.Save();
}

void Game::CancelLevelUpAndGameOver() {
    
    mStreakController.Reset();
    
    mLives = mLivesMax;
    mLivesLost = 0;
    
    if (gGameContainer != NULL) {
        
        gGameContainer->RemoveGameOverMenu();
        gGameContainer->RemoveLevelCompleteMenu();
        
        if (gGameContainer->mLevelCompleteOverlay != NULL) {
            gGameContainer->mLevelCompleteOverlay->Kill();
            gGameContainer->mLevelCompleteOverlay = NULL;
        }
        
        if (gGameContainer->mGameOverOverlay != NULL) {
            gGameContainer->mGameOverOverlay->Kill();
            gGameContainer->mGameOverOverlay = NULL;
        }
    }
    
    if (mGameOverBlimp != NULL) {
        delete mGameOverBlimp;
        mGameOverBlimp = NULL;
    }
    
    if (music_isPlaying()) {
        gMusicManager.Loop("game_loop.mp3");
    } else {
        gMusicManager.FadeInLoop("game_loop.mp3", 120);
    }
    
    mIsGameOver = false;
    mIsLevelComplete = false;
}

void Game::TestNextSection() {
    if (mLevelData == NULL) { return; }
    TestJumpSection(mLevelData->mCurrentSectionIndex + 1);
}

void Game::TestRestartSection() {
    if (mLevelData == NULL) { return; }
    TestJumpSection(mLevelData->mCurrentSectionIndex);
}

void Game::TestPrevSection() {
    if (mLevelData == NULL) { return; }
    TestJumpSection(mLevelData->mCurrentSectionIndex - 1);
}

void Game::TestJumpSection(int pSectionIndex) {
    
    
    if (mLevelData == NULL) { return; }
    
    LevelData *aData = mLevelData;
    mLevelData = NULL;
    Clear();
    CancelLevelUpAndGameOver();
    
    if (aData->mSectionList.mCount == 0) { return; }
    
    int aTargetIndex = pSectionIndex;
    if (aTargetIndex >= aData->mSectionList.mCount) { aTargetIndex = aData->mSectionList.mCount - 1; }
    if (aTargetIndex < 0) { aTargetIndex = 0; }
    
    aData->mCurrentSectionIndex = 0;
    aData->mDelayedDispose = 0;
    aData->mInitialDelay = 60;
    aData->mCurrentSection = NULL;
    aData->mPreviousSection = NULL;
    aData->mIsComplete = false;
    aData->mIsHalted = false;
    
    int aCheckIndex = 0;
    
    LevelSection *aSpawnedSection = NULL;
    
    FList aNewSectionList;
    EnumList(LevelSection, aSection, aData->mSectionList) {
        LevelSection *aClone = new LevelSection();
        aClone->mIndex = aSection->mIndex;
        aClone->Load(aSection->mName.c());
        aSection->Reset();
        delete aSection;
        aNewSectionList.Add(aClone);
    }
    
    aData->mSectionList.RemoveAll();
    
    EnumList(LevelSection, aSection, aNewSectionList) {
        aData->mSectionList.Add(aSection);
    }
    
    EnumList(LevelSection, aSection, aData->mSectionList) {
        if (aCheckIndex == aTargetIndex) {
            aSection->Spawn();
            aSpawnedSection = aSection;
            break;
        }
        aCheckIndex++;
    }
    
    aData->mCurrentSectionIndex = aTargetIndex;
    aData->mCurrentSection = aSpawnedSection;
    
    if (aSpawnedSection != NULL) {
        aData->mClockTime = aSpawnedSection->GetClockTicks();
        aData->mClockTicks = aData->mClockTime;
    } else {
        aData->mClockTime = 33333333;
        aData->mClockTicks = 22222222;
    }
    
    if (aSpawnedSection != NULL) {
        Log("Game::TestJumpSection(%d) { %s }\n\n", pSectionIndex, aSpawnedSection->mName.c());
    } else {
        Log("Game::TestJumpSection(%d)\n\n", pSectionIndex);
    }
    
    mLevelData = aData;
}

void Game::SaveState() {
    FString aPath = gDirDocuments + "save_state.dat";
    FFile aFile;
    
    aFile.WriteInt(mTimeWhenSectionStarted);
    aFile.WriteInt(mScore);
    aFile.WriteInt(mProgress);
    aFile.WriteInt(mProgressCount);
    
    aFile.WriteInt(mMissCount);
    
    aFile.WriteInt(mLevelData->mCurrentSectionIndex);
    
    
    mStreakController.SaveState(&aFile);
    
    aFile.Save(aPath.c());
}

void Game::LoadState() {
    FString aPath = gDirDocuments + "save_state.dat";
    FFile aFile;
    aFile.Load(aPath.c());
    
    mTime = aFile.ReadInt();
    mTimeWhenSectionStarted = mTime;
    
    mScore = aFile.ReadInt();
    mProgress = aFile.ReadInt();
    mProgressCount = aFile.ReadInt();
       
    mMissCount = aFile.ReadInt();
    
    int aSectionIndex = aFile.ReadInt();
    TestJumpSection(aSectionIndex);
    
    mStreakController.LoadState(&aFile);
}
