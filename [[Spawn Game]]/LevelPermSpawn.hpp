//
//  LevelPermSpawn.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/21/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelPermSpawn_hpp
#define LevelPermSpawn_hpp

#include "GameObject.hpp"
#include "LevelFormation.hpp"
#include "LevelMotionController.hpp"
#include "LevelStyleController.hpp"
#include "LevelFormationConfiguration.hpp"

class LevelSectionPerm;
class LevelPath;
class LevelSection;
class LevelPermSpawn {
public:
    
    LevelPermSpawn(LevelSectionPerm *pPerm, LevelPath *pPath);
    virtual ~LevelPermSpawn();
    
    
    
    
    void                                Spawn();
    bool                                IsClearForSectionCompletion();
    bool                                HasAnyObjects();
    void                                HandOffAllGameObjects(FList *pList);
    
    void                                Reset();
    
    void                                Update();
    void                                Draw();
    
    void                                DrawDeadObjectMarkers();
    
    void                                DisposeObject(GameObject *pObject);
    bool                                ContainsObject(GameObject *pObject);
    void                                DisposeAllObjects();
    
    bool                                IsClear(); //all of the objects are either null or "un-killable" types
    
    void                                RefreshPathPosition();
    void                                SetObjectPosition();
    
    
    bool                                mShortCircuit;
    bool                                mShortCircuitExternal;
    
    //bool                                        IsPauseExitComplete();
    
    LevelMotionController               mMotionController;
    bool                                mIsPlayingEnter;
    bool                                mIsPlayingExit;
    
    
    LevelStyleController                mStyleController;
    
    LevelSectionPerm                    *mPerm;
    LevelPath                           *mPath;
    GameObject                          *mObject;
    LevelFormation                      *mFormation;
    LevelFormationConfiguration         mFormationConfiguration;
    
    int                                 mObjectID;
    
    float                               mObjectX;
    float                               mObjectY;
    float                               mObjectRotation;
    
    int                                 mPathIndex;
    int                                 mWaitTimer;
    
    float                               mSpacingOffset;
    
    bool                                mDidSpawn;
    bool                                mDidUpdateAfterSpawn;
    
    float                               mPathX;
    float                               mPathY;
    
    //Possibility 1.) We have a formation...
    FString                             mFormationID;
    
    LevelSection                        *mSection;
    void                                RefreshSection(LevelSection *pSection);
    
    //Possibility 2.) We have an object...
    int                                 mObjectType;
    
    int                                 mKillTimer;
    
    
    void                                EditorDisposeAll();
    
private:
    
    bool                                        mDidShortCircuit;
    bool                                        mDidKnockDown;
    
};



#endif /* LevelPermSpawn_hpp */
