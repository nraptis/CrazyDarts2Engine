//
//  LevelWave.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/14/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelWave_hpp
#define LevelWave_hpp

#include "GameObject.hpp"
#include "LevelPath.hpp"
#include "LevelWaveSpawn.hpp"

class LevelSection;
class LevelWave {
public:
    LevelWave();
    ~LevelWave();
    
    bool                                mEditorPreviewMode;
    void                                EditorDisposeAll();
    
    bool                                IsClearForShortCircuitExternal();
    bool                                IsClear();
    
    bool                                IsResting(GameObject *pObject); //Returns true if the current object is resting...
    
    
    void                                Reset();
    //void                                Restart();
    
    void                                Prepare();
    
    void                                Update();
    void                                Draw();
    void                                DrawDeadObjectMarkers();
    void                                DisableDeadObjectMarkers();
    bool                                mDisableDeadObjectMarkers;
    
    
    void                                HandOffAllGameObjects(FList *pList);
    
    void                                DisposeObject(GameObject *pObject);
    bool                                ContainsObject(GameObject *pObject);
    void                                DisposeAllObjects();
    
    bool                                mIsComplete;
    
    //int                                 mExitType;
    
    LevelPath                           mPath;
    
    
    bool                                mCreationRequiresPrevWaveStart;
    bool                                mCreationRequiresPrevWaveComplete;
    bool                                mCreationRequiresScreenWavesClear;
    
    int                                 mLoopCount;
    
    int                                 mCreationDelay;
    
    FList                               mSpawnList;
    int                                 mSpawnIndex;
    
    float                               mSpawnSpacing;
    
    LevelSection                        *mSection;
    void                                RefreshSection(LevelSection *pSection);
    
    void                                RefreshClockTime();
    
    
    int                                 mKillTimer;
    
private:
    FList                               mSpawnKillList;
    FList                               mSpawnDeleteList;
    FList                               mRecentlyCompletedList;
    
    void                                HandleSpawnComplete(LevelWaveSpawn *pSpawn);
    
};


#endif /* LevelWave_hpp */
