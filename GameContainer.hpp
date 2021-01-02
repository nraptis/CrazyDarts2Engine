//
//  GameContainerContainer.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/10/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef GameContainerContainer_hpp
#define GameContainerContainer_hpp

#include "GFXApp.hpp"
#include "GameOverMenu.hpp"
#include "LevelCompleteMenu.hpp"
#include "GameOverlayEffects.hpp"
#include "GameOverlayInterface.hpp"
#include "EditorMenuGameTestUtils.hpp"
#include "EditorMenuGameTest.hpp"
#include "SpeedControlMenu.hpp"
#include "LevelUpGameOverMenu.hpp"
#include "SectionNavigatorMenu.hpp"
#include "StatsMenu.hpp"
#include "StreakMenu.hpp"
#include "LevelCompleteOverlay.hpp"
#include "GameOverOverlay.hpp"

//GameFlowTestMenu.hpp

class GameFlowTestMenu;
class CameraMenu;
class Game;
class NewLightConfigurationScene;
class Level;
class LevelOverlay;


class GameContainer : public FCanvas {
public:
    GameContainer();
    virtual ~GameContainer();
    
    virtual void                                Layout() override;
    virtual void                                BaseUpdate() override;
    virtual void                                Draw() override;
    
    void                                        Draw3D();
    
    virtual void                                TouchDown(float pX, float pY, void *pData) override;
    virtual void                                TouchMove(float pX, float pY, void *pData) override;
    virtual void                                TouchUp(float pX, float pY, void *pData) override;
    virtual void                                TouchFlush() override;
    
    virtual void                                KeyDown(int pKey) override;
    virtual void                                KeyUp(int pKey) override;
    
    virtual void                                Notify(void *pSender, const char *pNotification) override;
    
    //After the game + editor are ready to go.
    void                                        Realize();
    void                                        ReorderOverlays();
    
    void                                        EnterBackground();
    void                                        EnterForeground();
    
    
    
    Game                                        *mGame;
    FCanvas                                     *mContainer;
    
    //GameTestEditorOverlay                       *mGameTestEditorOverlay;
    //GameTestRunningOverlay                      *mGameTestRunningOverlay;
    
    //This is essentially a scaled-down 2-D overlay which rests atop the game...
    GameOverlayEffects                          *mEffectsOverlay;
    GameOverlayInterface                        *mInterfaceOverlay;
    
    
    LevelOverlay                                *mLevelOverlay;
    
    
    float                                       mGameCorner1X;
    float                                       mGameCorner1Y;
    
    float                                       mGameCorner2X;
    float                                       mGameCorner2Y;
    
    float                                       mGameCorner3X;
    float                                       mGameCorner3Y;
    
    float                                       mGameCorner4X;
    float                                       mGameCorner4Y;
    
    
    
    
    EditorMenuGameTest                          *mEditorMenu;
    EditorMenuGameTestUtils                     *mEditorMenuUtils;
    SpeedControlMenu                            *mMenuSpeed;
    StatsMenu                                   *mMenuStats;
    StreakMenu                                  *mMenuStreaks;
    CameraMenu                                  *mMenuCamera;
    LevelUpGameOverMenu                         *mMenuLUGO;
    
    GameFlowTestMenu                            *mMenuGameFlow;
    SectionNavigatorMenu                        *mMenuSectionNavigator;
    
    
    
    NewLightConfigurationScene                  *mLightConfig;
    
    
    //Including Safe Area...
    //float                                       mInterfaceLeftWidth;
    //float                                       mInterfaceRightWidth;
    //float                                       mInterfaceTopHeight;
    //float                                       mInterfaceBottomHeight;
    
    
    //GameMenu                                    *mGameMenu;
    //FCanvasAnimation                            *mGameMenuAnimation;
    //bool                                        mGameMenuAnimatingIn;
    
    bool                                        CanShowGameMenu();
    
    
    GameOverOverlay                             *mGameOverOverlay;
    GameOverMenu                                *mGameOverMenu;
    FCanvasAnimation                            *mGameOverMenuAnimation;
    bool                                        mGameOverMenuAnimatingIn;
    void                                        ShowGameOverOverlay();
    void                                        ShowGameOverMenu();
    void                                        RemoveGameOverMenu();
    
    LevelCompleteOverlay                        *mLevelCompleteOverlay;
    LevelCompleteMenu                           *mLevelCompleteMenu;
    FCanvasAnimation                            *mLevelCompleteMenuAnimation;
    bool                                        mLevelCompleteMenuAnimatingIn;
    void                                        ShowLevelCompleteMenu();
    
    void                                        ShowLevelCompleteOverlay();
    void                                        RemoveLevelCompleteMenu();
    
    void                                        Pause();
    void                                        Unpause();
    
    void                                        BeginPauseFadeIn();
    void                                        BeginPauseFadeOut();
    
    bool                                        IsPauseFadeInComplete();
    bool                                        IsPauseFadeOutComplete();
    
    bool                                        mPaused;
    bool                                        mPauseFadeEnabled;
    float                                       mPauseFade;
    float                                       mPauseFadeMax;
    
    bool                                        mBackgroundPaused;
    
    void                                        OpenEditorTestMenus();
    
    int                                         mPlaybackSpeedCategory;
    int                                         mPlaybackSlowMotionTimer;
    int                                         mPlaybackUpdateCount;
    
    void                                        SetUpForLevel(Level *pLevel);
    
};

extern GameContainer *gGameContainer;

#endif
