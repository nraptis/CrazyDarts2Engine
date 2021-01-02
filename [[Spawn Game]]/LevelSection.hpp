//
//  LevelSection.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/14/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelSection_hpp
#define LevelSection_hpp

#define SECTION_FLY_NONE 0
#define SECTION_FLY_LEFT 1
#define SECTION_FLY_TOP 2
#define SECTION_FLY_RIGHT 3

#define FLY_TIME_TOP 240
#define FLY_TIME_SIDES 160

#include "GameObject.hpp"
#include "LevelWave.hpp"
#include "LevelSectionPerm.hpp"

class LevelSection {
public:
    LevelSection();
    ~LevelSection();
    
    bool                                        mEditorPreviewMode;
    
    void                                        Reset();
    
    void                                        Spawn();
    void                                        Update();
    void                                        UpdateFlyIn();
    void                                        UpdateFlyOut();
    
    void                                        Draw();
    
    bool                                        IsResting();
    
    
    float                                       mX;
    float                                       mY;
    
    bool                                        mDidSpawn;
    
    //Do we have any objects sitting on the board?
    //Note: This assumes mIsComplete = true (everything is finished spawning)
    bool                                        HasAnyObjects();
    
    bool                                        HasAnyPermanents();
    bool                                        HasAllPermanents();
    bool                                        HasAnyWaves();
    bool                                        HasAllWaves();
    
    
    void                                        HandOffAllGameObjects(FList *pList);
    void                                        HandOffAllPermanentGameObjects(FList *pList);
    
    
    void                                        DrawDeadObjectMarkers();
    
    
    //These can override the fly in and out types for the engine - forcing a certain type.
    //Cases like "forced from top" then "forced to top" interrupt the timing, so they need to be handled...
    int                                         mRequiredFlyInType;
    int                                         mRequiredFlyOutType;
    
    int                                         GetClockTicks();
    int                                         GetClockFlyingInTicksRemaining();
    
    bool                                        mClockCountingDown;
    
    
    void                                        FlyIn(int pType, int pDelay);
    int                                         mFlyInType;
    int                                         mFlyInTime;
    int                                         mFlyInTimer;
    
    int                                         mFlyInDelay;
    
    void                                        FlyOut(int pType);
    int                                         mFlyOutType;
    int                                         mFlyOutTime;
    int                                         mFlyOutTimer;
    
    
    ////////////////////////////////////////////////////////////////////////////
    //
    //                    Externally controlled...
    //
    int                                         mKeepAliveTimer;
    int                                         mForceKillTimer;
    //
    //
    ////////////////////////////////////////////////////////////////////////////
    
    int                                         mProgressCount;
    
    
    void                                        DisposeObject(GameObject *pObject);
    
    void                                        AddWave(LevelWave *pLevelWave);
    FList                                       mWaveList;
    
    void                                        AddPerm(LevelSectionPerm *pPerm);
    FList                                       mPermList;
    
    int                                         mCandidateWaveIndex;
    
    LevelWave                                   *mCandidateWave;
    int                                         mCandidateWaveDelay;
    
    FList                                       mActiveWaveList;
    FList                                       mRemoveActiveWaveList;
    
    //Mainly for editor, if we want to start later in the queue
    int                                         mStartWaveIndex;
    
    bool                                        mIsComplete;
    
    bool                                        mDidFirstUpdate;
    
    bool                                        mKnockDownPermsOnClear;
    bool                                        mBlockFreeLives;
    
    bool                                        mAllWavesComplete;
    bool                                        mAllPermsComplete;
    
    //bool                                      mRequiresPauseExit;
    
    bool                                        mPauseExit;
    int                                         mPauseExitCompletionDelayTimer;
    bool                                        ShouldPauseExit();
    void                                        BeginPauseExit();
    bool                                        IsPauseExitComplete(); //Did all the pause-exit objects reach their paused state?
    
    bool                                        mLoadError;
    void                                        Load(const char *pFile);
    
    FString                                     mName;
    
    int                                         mIndex;
    
    
    bool                                        IsClearForShortCircuitExternal();
    bool                                        IsClearOfAllPermanentProgressObjects();
    
    
    void                                        EditorDisposeAll();
    
    int                                         GenerateObjectIndex();
    int                                         mSpawnObjectIndex;
    
    void                                        RefreshSection();
    
    bool                                        ContainsObject(GameObject *pObject);
    LevelWave                                   *GetWaveForObject(GameObject *pObject);
    LevelSectionPerm                            *GetPermForObject(GameObject *pObject);
    void                                        DisposeAllObjects();
    
private:
    
    FList                                       mKillWaveList;
    FList                                       mDeleteWaveList;
    
    FList                                       mKillPermList;
    FList                                       mDeletePermList;
    
};

#endif /* LevelSection_hpp */
