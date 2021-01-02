//
//  Game.hpp
//  MetalLearning
//
//  Created by Nicholas Raptis on 2/21/19.
//  Copyright © 2019 Nicholas Raptis. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "GFXApp.hpp"
#include "FPolyPath.hpp"
#include "GameRenderer.hpp"
#include "FloatingCamera.hpp"
#include "Level.hpp"
#include "LevelData.hpp"
#include "LevelPath.hpp"
#include "Transform2D.hpp"
#include "Transform3D.hpp"
#include "HangingThread.hpp"
#include "GameOverlayEffects.hpp"
#include "GameOverlayInterface.hpp"
#include "WindSpeedSimulator.hpp"
#include "Dart.hpp"
#include "Balloon.hpp"
#include "BrickHead.hpp"
#include "Turtle.hpp"
#include "Bomb.hpp"
#include "PrizeBox.hpp"
#include "Star.hpp"
#include "FreeLife.hpp"
#include "Cannon.hpp"
#include "PlatformSmall.hpp"
#include "PlatformMedium.hpp"
#include "PlatformLarge.hpp"
#include "PlatformExtraLarge.hpp"
#include "Blimp.hpp"
#include "StreakController.hpp"

#define GAME_WIDTH 768
#define GAME_HEIGHT 1280

#define GAME_PLAYBACK_XXX_SLOW 0
#define GAME_PLAYBACK_XX_SLOW 1
#define GAME_PLAYBACK_X_SLOW 2
#define GAME_PLAYBACK_SLOW 3
#define GAME_PLAYBACK_NORMAL 4
#define GAME_PLAYBACK_FAST 5
#define GAME_PLAYBACK_X_FAST 6
#define GAME_PLAYBACK_XX_FAST 7
#define GAME_PLAYBACK_XXX_FAST 8

class Game : public FCanvas {
public:
    Game();
    virtual ~Game();
    
    virtual void                                SetFrame(float pX, float pY, float pWidth, float pHeight) override;
    
    virtual void                                LayoutTransform() override;
    virtual void                                Layout() override;
    virtual void                                Update() override;
    void                                        UpdateMain();
    
    virtual void                                Draw() override;
    
    void                                        Draw3D();
    
    void                                        DrawDeadObjectMarker(float pX, float pY);
    
    virtual void                                TouchDown(float pX, float pY, void *pData) override;
    virtual void                                TouchMove(float pX, float pY, void *pData) override;
    virtual void                                TouchUp(float pX, float pY, void *pData) override;
    virtual void                                TouchFlush() override;
    
    virtual void                                KeyDown(int pKey) override;
    virtual void                                KeyUp(int pKey) override;
    
    virtual void                                Notify(void *pSender, const char *pNotification) override;
    
    void                                        Clear();
    
    bool                                        IsWaveClearForSpawn();
    bool                                        IsScreenClearForSpawn(bool pIncludePerms);
    
    // Note: This is intended to be used for preliminary check (FROM EDITOR)...
    //When computing actual progress count, there will need to be
    //foreknowledge about which replacements will occur. For example,
    //a "free life" in editor does not count towards progress, but if a replacement occurs,
    //then total progress cannot include the object (balloon replaced by free life, as an example)
    bool                                        DoesObjectTypeCountTowardsProgressCount(int pObjectType);
    
    bool                                        DoesObjectTypeCountTowardsProgress(int pObjectType);
    
    bool                                        DoesObjectTypeRequireClearing(int pObjectType);
    
    void                                        Convert2DTransformTo3D(Transform2D *p2D, Transform3D *p3D);
    
    FVec3                                       Convert2DCoordsTo3D(float pX, float pY);
    float                                       Convert2DXTo3D(float pX);
    float                                       Convert2DYTo3D(float pY);
    
    FVec2                                       Convert3DCoordsTo2D(float pX, float pY);
    float                                       Convert3DXTo2D(float pX);
    float                                       Convert3DYTo2D(float pY);
    
    //When the object is being added to the play board.
    void                                        RealizeObject(GameObject *pObject);
    
    //When the object is ready to be removed from the play board.
    void                                        DisposeObject(GameObject *pObject);
    void                                        DisposeObjectFromLevelData(GameObject *pObject);
    
    //Mainly used by editor to "flush" screen before re-spawn...
    void                                        DisposeAllObjectsWave();
    void                                        DisposeAllObjectsPerm();
    void                                        KillAllObjects();
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    ////             These are "events" which occur in certain object's life cycles...
    ////
    void                                        FlyOffEscape(GameObject *pObject);
    
    void                                        HandleStreakUpdate(Dart *pDart);
    
    //Generally these are only going to be bricks to start..?
    void                                        KnockDown(GameObject *pObject);
    bool                                        IsAnyObjectBeingKnockedDown();
    
    void                                        DartCollideWithBalloon(Dart *pDart, Balloon *pBalloon);
    void                                        DartCollideWithBrickhead(Dart *pDart, BrickHead *pBrickHead);
    void                                        DartCollideWithBomb(Dart *pDart, Bomb *pBomb);
    void                                        DartCollideWithTurtle(Dart *pDart, Turtle *pTurtle);
    void                                        DartCollideWithFreeLife(Dart *pDart, FreeLife *pFreeLife);
    void                                        DartCollideWithPlatform(Dart *pDart, Platform *pPlatform);
    void                                        DartCollideWithCannon(Dart *pDart, Cannon *pCannon);
    
    void                                        PopBalloon(Balloon *pBalloon);
    void                                        PopFreeLife(FreeLife *pFreeLife);
    void                                        PopBlimp(Blimp *pBlimp);
    
    void                                        TurtleDidLoseBalloon(Turtle *pTurtle);
    void                                        FireCannon(Cannon *pCannon);
    void                                        ExplodeBomb(Bomb *pBomb);
    
    ////
    ////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    ////
    
    void                                        DartMovingInterpolationUpdateTip(Dart *pDart, float pPercent, bool pEnd);
    
    void                                        DartMovingInterpolationBalloons(Dart *pDart, float pPercent, bool pEnd);
    void                                        DartMovingInterpolationFreeLives(Dart *pDart, float pPercent, bool pEnd);
    void                                        DartMovingInterpolationBrickHeads(Dart *pDart, float pPercent, bool pEnd);
    void                                        DartMovingInterpolationBombs(Dart *pDart, float pPercent, bool pEnd);
    void                                        DartMovingInterpolationTurtles(Dart *pDart, float pPercent, bool pEnd);
    void                                        DartMovingInterpolationCannons(Dart *pDart, float pPercent, bool pEnd);
    void                                        DartMovingInterpolationPlatforms(Dart *pDart, float pPercent, bool pEnd);
    
    void                                        DartMovingInterpolationBalloonsPrepare(Dart *pDart);
    void                                        DartMovingInterpolationFreeLivesPrepare(Dart *pDart);
    void                                        DartMovingInterpolationBrickHeadsPrepare(Dart *pDart);
    void                                        DartMovingInterpolationBombsPrepare(Dart *pDart);
    void                                        DartMovingInterpolationTurtlesPrepare(Dart *pDart);
    void                                        DartMovingInterpolationCannonsPrepare(Dart *pDart);
    void                                        DartMovingInterpolationPlatformsPrepare(Dart *pDart);
    
    bool                                        ShouldSpawnFreeLife();
    bool                                        ShouldSpawnFreeLifeMain();
    
    void                                        NotifyDidSpawnFreeLife();
    
    //We will either call this:
    void                                        NotifyDartDidStopStreak(Dart *pDart);
    
    //Or call this:
    void                                        NotifyDartDidStopRapidStreak(Dart *pDart);
    
    void                                        HandleRapidBonus(Dart *pDart, int pNumber);
    
    void                                        NotifyDartDidLeavePlayArea(Dart *pDart);
    void                                        NotifyDartDidExitScreen(Dart *pDart);
    
    void                                        LifeLose(int pCount);
    
    float                                       mRenderShiftX;
    float                                       mRenderShiftY;
    
    LevelData                                   *mLevelData;
    //GameLevelController                         *mLevelController;
    
    
    GameRenderer                                *mRenderer;
    FloatingCamera                              *mCamera;
    
    WindSpeedSimulator                          mWind;
    
    int                                         mLevelCompleteDelayTimer;
    
    float                                       mGravity;
    
    FObjectList                                 mDartList;
    FObjectList                                 mBalloonList;
    FObjectList                                 mFreeLifeList;
    FObjectList                                 mBrickHeadList;
    FObjectList                                 mTurtleList;
    FObjectList                                 mBombList;
    FObjectList                                 mPlatformList;
    FObjectList                                 mCannonList;
    FObjectList                                 mStarList;
    FObjectList                                 mPrizeBoxList;
    FObjectList                                 mBlimpList;
    
    FList                                       mTempDartList;
    FList                                       mTempObjectList;
    
    void                                        RecordFlyingDartsForWaveBlocking();
    FList                                       mFlyingDartWaveBlockingDartList;
    
    Dart                                        *mCurrentDart;
    int                                         mCurrentDartRespawnTimer;
    
    void                                        ResetDartTouch();
    void                                        ReleaseDart();
    
    float                                       mReleaseDirX;
    float                                       mReleaseDirY;
    float                                       mReleaseVelocity;
    float                                       mReleaseFactor;
    
    float                                       mDartReleaseVelocityMin;
    float                                       mDartReleaseVelocityMax;
    bool                                        mDartResetAnimation;
    int                                         mDartResetAnimationTick;
    int                                         mDartResetAnimationTime;
    float                                       mDartResetStartPullX;
    float                                       mDartResetStartPullY;
    float                                       mDartResetStartRotation;
    float                                       mDartResetRotationLength;
    
    float                                       mDartSpawnX;
    float                                       mDartSpawnY;
    float                                       mDartPullbackRangeMin;
    float                                       mDartPullbackRangeMax;
    
    float                                       mDartPullbackRangeFingerMin;
    float                                       mDartPullbackRangeFingerMax;
    
    float                                       mDartPullbackGrabRangeSquared;
    float                                       mDartTargetPullX;
    float                                       mDartTargetPullY;
    float                                       mDartPullX;
    float                                       mDartPullY;
    float                                       mDartTargetPullRotation;
    float                                       mDartPullRotation;
    
    int                                         mFlashTipTimer;
    
    bool                                        mIsDartBeingPulled;
    void                                        *mDartTouch;
    float                                       mDartTouchStartX;
    float                                       mDartTouchStartY;
    
    float                                       mPlayAreaTop;
    float                                       mPlayAreaRight;
    float                                       mPlayAreaBottom;
    float                                       mPlayAreaLeft;
    
    float                                       mGameAreaTop;
    float                                       mGameAreaRight;
    float                                       mGameAreaBottom;
    float                                       mGameAreaLeft;
    
    
    float                                       mOffScreenAreaTop;
    float                                       mOffScreenAreaRight;
    float                                       mOffScreenAreaBottom;
    float                                       mOffScreenAreaLeft;
    
    //
    // This is the region where collisions are permitted...
    //
    
    float                                       mHitZoneTop;
    float                                       mHitZoneRight;
    float                                       mHitZoneBottom;
    float                                       mHitZoneLeft;
    
    float                                       mSpawnZoneTop;
    float                                       mSpawnZoneRight;
    float                                       mSpawnZoneBottom;
    float                                       mSpawnZoneLeft;
    
    float                                       mPeekZoneTop;
    float                                       mPeekZoneRight;
    float                                       mPeekZoneLeft;
    
    float                                       mQuarterZoneTop;
    float                                       mQuarterZoneRight;
    float                                       mQuarterZoneLeft;
    float                                       mQuarterZoneBottom;
    
    float                                       mExitZoneTop;
    float                                       mExitZoneRight;
    float                                       mExitZoneLeft;
    
    //
    // Once darts, items, etc get into this area, they
    // are automatically killed. "Out of bounds"
    //
    
    float                                       mTrajectoryKillZoneTop;
    float                                       mTrajectoryKillZoneRight;
    float                                       mTrajectoryKillZoneBottom;
    float                                       mTrajectoryKillZoneLeft;
    
    float                                       mKillZoneTop;
    float                                       mKillZoneRight;
    float                                       mKillZoneBottom;
    float                                       mKillZoneLeft;
    
    float                                       mGameContainerBorderTop;
    float                                       mGameContainerBorderRight;
    float                                       mGameContainerBorderBottom;
    float                                       mGameContainerBorderLeft;
    
    LevelSection                                *GetSectionForObject(GameObject *pObject);
    LevelWave                                   *GetWaveForObject(GameObject *pObject);
    LevelSectionPerm                            *GetPermForObject(GameObject *pObject);
    
    
    bool                                        IsGameObjectOutsideKillZone(GameObject *pObject);
    //bool                                        IsGameObjectOutsideGameZone(GameObject *pObject);
    bool                                        IsGameObjectOutsideOffScreenZone(GameObject *pObject);
    bool                                        IsGameObjectOutsideOffScreenZoneTop(GameObject *pObject);
    
    
    bool                                        IsPositionOutsideKillZone(float pX, float pY);
    //bool                                        IsPositionOutsideGameZone(float pX, float pY);
    bool                                        IsPositionOutsideOffScreenZone(float pX, float pY);
    bool                                        IsPositionOutsideOffScreenZoneTop(float pX, float pY);
    
    
    
    
    void                                        CorrectFingerRange(float &pDiffX, float &pDiffY);
    
    
    
    void                                        Load();
    void                                        LoadEditorTest();
    
    void                                        Restart();
    
    bool                                        mDidLoad;
    
    LevelSection                                mTestSection;
    
    //bool                                        mSlowMo;
    //int                                         mSlowMoTimer;
    
    
    bool                                        mInvulnerable;
    
    bool                                        ShouldBeginGameOver();
    void                                        BeginGameOver();
    void                                        BurstGameOverBlimp();
    bool                                        mIsGameOver;
    
    
    void                                        UpdateGameOver();
    void                                        UpdateLevelComplete();
    
    void                                        Rumble(float pRumble);
    void                                        RumbleRefresh();
    
    float                                       mRumbleMagnitude;
    float                                       mRumbleX;
    float                                       mRumbleY;
    
    
    void                                        ComputeTrajectory(float pDiffX, float pDiffY);
    void                                        ComputeTrajectoryAddFinalPoint(float pTrajectoryX, float pTrajectoryY);
    
    bool                                        TrajectoryKillZoneCollisionPoint(float pX1, float pY1, float pX2, float pY2, float pLineX1, float pLineY1, float pLineX2, float pLineY2, float &pResultX, float &pResultY);
    
    void                                        ProcessTrajectory();
    bool                                        mTrajectoryIsNarrow;
    
    void                                        ResetTrajectory();
    
    bool                                        mTrajectoryExists;
    FPointList                                  mTrajectoryList;
    
    float                                       mTrajectoryStartX;
    float                                       mTrajectoryStartY;
    
    float                                       mTrajectoryStartTargetX;
    float                                       mTrajectoryStartTargetY;
    
    
    int                                         mGameOverMode;
    int                                         mGameOverTimer;
    bool                                        mGameOverDidShowMenu;
    bool                                        mGameOverDidStartDriftAway;
    bool                                        mGameOverDidCompleteDriftAway;
    
    int                                         mGameOverBlimpSpawnTimer;
    bool                                        mGameOverDidSpawnBlimp;
    FList                                       mLUGOObjectList;
    Blimp                                       *mGameOverBlimp;
    
    int                                         mLevelCompleteMode;
    int                                         mLevelCompleteTimer;
    
    bool                                        mLevelCompleteDidStartDriftAway;
    bool                                        mLevelCompleteDidCompleteDriftAway;
    
    int                                         mLevelCompleteDriftAwayTimer;
    
    
    //bool                                        mLevelCompleteOverlayDidComplete;
    
    //int                                         mLevelCompleteShowMenuTimer;
    
    
    void                                        GetAllGameObjectsLUGO(FList *pList);
    void                                        HeightSortGameObjects(FList *pList);
    bool                                        HeightCompareGameObjects(GameObject *pObject1, GameObject *pObject2);
    
    void                                        DestroyGameObject(GameObject *pObject);
    
    void                                        TestDestroyNearestObject(float pX, float pY);
    void                                        TestDestroyBuildListOfObjects(FList *pList);
    
    bool                                        ShouldBeginLevelComplete();
    void                                        BeginLevelComplete();
    bool                                        mIsLevelComplete;
    
    bool                                        LevelCompleteShouldDriftAway();
    void                                        LevelCompleteBeginDriftAway();
    void                                        LevelCompleteUpdateDriftAway();
    
    
    bool                                        IsClearForShortCircuitExternal();
    
    StreakController                            mStreakController;
    
    int                                         mScore;
    
    int                                         mMissCount;
    
    int                                         mTime;
    int                                         mTimeWhenSectionStarted;
    
    int                                         mTimeWhenFreeLifeSpawned;
    
    int                                         mLives;
    int                                         mLivesLost;
    int                                         mLivesMax;
    
    int                                         mProgressCount;
    int                                         mProgress;
    
    FList                                       mMissedObjectClusterList;
    FList                                       mMissedObjectClusterKillList;
    FList                                       mMissedObjectClusterQueueList;
    void                                        RegisterMissedObject(GameObject *pObject);
    
    //MissedGameObjectCluster
    
    void                                        WriteLUGOStats();
    
    int                                         mPopSoundIndex;
    int                                         mPopSoundAlternateTick;
    void                                        PlayPopSound();
    
    int                                         mPopDelayBalloon;
    int                                         mPopDelayFreeLife;
    int                                         mPopDelayBlimp;
    
    
    int                                         mTurtleKnockDownSoundDelay;
    int                                         mKnockDownSoundDelay;
    
    GameObject                                  *ObjectForOverlay(int pSectionIndex, int pObjectIndex);
    bool                                        IsObjectForOverlayResting(int pSectionIndex, int pObjectIndex);
    
    void                                        CancelLevelUpAndGameOver();
    
    void                                        TestNextSection();
    void                                        TestRestartSection();
    void                                        TestPrevSection();
    void                                        TestJumpSection(int pSectionIndex);
    
    void                                        SaveState();
    void                                        LoadState();
    
};

extern Game *gGame;
extern FPointList gTrajectoryList;


#endif

