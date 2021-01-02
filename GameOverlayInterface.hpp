//
//  GameOverlayInterfaceInterface.hpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 9/3/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef GameOverlayInterfaceInterface_hpp
#define GameOverlayInterfaceInterface_hpp

#include "GFXApp.hpp"
#include "FButton.hpp"
#include "FCanvas.hpp"
#include "ShadowButton.hpp"
#include "TimeClock.hpp"
#include "TopBar.hpp"
#include "FPolyPath.hpp"
#include "MagicArrow.hpp"
#include "FloatingAchievementBubble.hpp"


#define TEXT_BUBBLE_EXPLORE_GRID_RADIUS 24
#define TEXT_BUBBLE_EXPLORE_GRID_STEP_SIZE 8.0f

class Game;
class GameOverlayInterface : public FCanvas {
public:
    GameOverlayInterface();
    virtual ~GameOverlayInterface();
    
    virtual void                                Layout() override;
    virtual void                                Update() override;
    virtual void                                Draw() override;
    void                                        DrawTopBarManual();
    
    void                                        DrawBorders();
    void                                        DrawOutside(float pWallTopY, float pWallRightX, float pWallBottomY, float pWallLeftX);
    
    
    virtual void                                TouchDown(float pX, float pY, void *pData) override;
    virtual void                                TouchMove(float pX, float pY, void *pData) override;
    virtual void                                TouchUp(float pX, float pY, void *pData) override;
    virtual void                                TouchFlush() override;
    
    virtual void                                Notify(void *pSender, const char *pNotification) override;
    
    void                                        NotifyLivesChanged();
    void                                        NotifyProgressChanged();
    void                                        NotifyDidLoad();
    
    ShadowButton                                *mPauseButton;
    
    //ProgressBar                                 *mProgressBar;
    //ProgressWheel                               *mProgressWheel;
    TimeClock                                   *mTimeClock;
    
    TopBar                                      *mTopBar;
    
    MagicArrow                                  mMagicArrow;
    
    FTextureSnake                               mTestSnake;
    
    float                                       mGameCorner1X;
    float                                       mGameCorner1Y;
    
    float                                       mGameCorner2X;
    float                                       mGameCorner2Y;
    
    float                                       mGameCorner3X;
    float                                       mGameCorner3Y;
    
    float                                       mGameCorner4X;
    float                                       mGameCorner4Y;
    
    FObjectList                                 mAchievementBubbleList;
    
    bool                                        HasAchievementBubble();
    void                                        SpawnAchievementBubble();
    
    float                                       mWallInsetSpan;
    float                                       mWallInsetSides;
    float                                       mWallInsetBottom;
    float                                       mWallInsetTop;
    
    float                                       mWallLeftX;
    float                                       mWallRightX;
    
    float                                       mWallTopY;
    float                                       mWallBottomY;
    
    float                                       mWallWidth2;
    float                                       mWallHeight2;
    
    float                                       mWallWhiteWidth2;
    float                                       mWallWhiteHeight2;
    
    float                                       mWallShadoweWidth2;
    float                                       mWallShadowHeight2;
    
};

extern GameOverlayInterface *gInterfaceOverlay;


#endif
