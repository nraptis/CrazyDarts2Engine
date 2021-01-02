//
//  LevelData.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/10/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelData_hpp
#define LevelData_hpp

#include "GameObject.hpp"
#include "LevelWave.hpp"
#include "LevelSection.hpp"

enum LEVEL_DATA_CLOCK_MODE: int {
    Uninitialized = 0,
    FlyingIn = 1,
    FlyingOut = 2,
    CountingDown = 3,
    AllSectionsCompleted = 4
};

class LevelData {
public:
    LevelData();
    virtual ~LevelData();
    
    bool                                        mEditorPreviewMode;
    
    void                                        Update();
    
    void                                        DisposeObject(GameObject *pObject);
    void                                        DisposeSection(LevelSection *pLevelSection);
    void                                        DisposeAllObjects();
    
    void                                        FlyOutPreviousSection();
    
    //There is an assumption - we ALWAYS have called FlyOutPreviousSection before calling this...
    void                                        FlyInCurrentSection();
    
    void                                        Print(const char *pName);
    
    int                                         GetProgressCount();
    
    void                                        AddSection(LevelSection *pLevelSection);
    
    void                                        KnockDownAllObjectsFromSection(LevelSection *pLevelSection);
    
    void                                        DrawDeadObjectMarkers();
    
    FList                                       mSectionList;
    
    LevelSection                                *mCurrentSection;
    LevelSection                                *mPreviousSection;
    
    int                                         mCurrentSectionIndex;
    
    int                                         mDelayedDispose;
    
    int                                         mInitialDelay;
    
    bool                                        mIsComplete;
    
    void                                        Halt();
    bool                                        mIsHalted;
    
    LevelSection                                *GetSectionForObject(GameObject *pObject);
    LevelWave                                   *GetWaveForObject(GameObject *pObject);
    LevelSectionPerm                            *GetPermForObject(GameObject *pObject);
    
    
    LEVEL_DATA_CLOCK_MODE                       mClockMode;
    int                                         mClockTime;
    int                                         mClockTicks;
    int                                         mClockFlyingInTicksRemaining;
    
};

#endif /* LevelData_hpp */
