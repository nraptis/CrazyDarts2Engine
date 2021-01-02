//
//  LevelWaveSpawnSpawn.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/23/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelWaveSpawnSpawn_hpp
#define LevelWaveSpawnSpawn_hpp

//We are a "spawn" and we can either control
//a single balloon, or a cluster of ANYTHING (Space Lords? Maybe?)

#include "GameObject.hpp"
#include "LevelFormation.hpp"
#include "LevelMotionController.hpp"
#include "LevelStyleController.hpp"
#include "LevelFormationConfiguration.hpp"

class LevelWave;
class LevelPath;
class LevelSection;

class LevelWaveSpawn {
public:
    LevelWaveSpawn(LevelWave *pWave, LevelPath *pPath);
    virtual ~LevelWaveSpawn();
    
    bool                                mEditorPreviewMode;
    
    void                                Spawn();
    void                                Reset();
    
    void                                Update();
    void                                Draw();
    void                                DrawDeadObjectMarkers();
    
    void                                DisposeObject(GameObject *pObject);
    bool                                ContainsObject(GameObject *pObject);
    void                                DisposeAllObjects();
    
    bool                                DidStart();
    bool                                IsClear();
    bool                                IsClearForSectionCompletion();
    
    bool                                HasAnyObjects();
    
    bool                                mPauseExit;
    int                                 mPauseExitCompletionDelayTimer;
    bool                                ShouldPauseExit();
    void                                BeginPauseExit();
    bool                                IsPauseExitComplete();
    
    LevelMotionController               mMotionController;
    bool                                mIsPlayingEnter;
    bool                                mIsPlayingExit;
    
    LevelStyleController                mStyleController;
    
    LevelWave                           *mWave;
    LevelPath                           *mPath;
    GameObject                          *mObject;
    LevelFormation                      *mFormation;
    LevelFormationConfiguration         mFormationConfiguration;
    
    int                                 mObjectID;
    
    float                               mObjectX;
    float                               mObjectY;
    float                               mObjectRotation;
    
    int                                 mPathIndex;
    bool                                mIsComplete;
    bool                                mDidSpawn;
    bool                                mDidUpdateAfterSpawn;
    
    float                               mBaseX;
    float                               mBaseY;
    float                               mDistanceTraveled;
    
    bool                                mShortCircuit;
    bool                                mShortCircuitExternal;
    
    bool                                mDidShortCircuit;
    bool                                mDidKnockDown;
    
    
    
    int                                 mWaitTimer;
    
    //Possibility 1.) We have a formation...
    FString                             mFormationID;
    
    LevelSection                        *mSection;
    
    void                                RefreshSection(LevelSection *pSection);

    
    //Possibility 2.) We have an object...
    int                                 mObjectType;
    
    void                                HandOffAllGameObjects(FList *pList);
    
    float                               mOffsetSpawnDistance;
    
    int                                 mKillTimer;
    
    int                                 mClockTicks;
    
    int                                 mCurrentWaitTime;
    int                                 mCurrentWaitTick;
    
    void                                EditorDisposeAll();
};


#endif

