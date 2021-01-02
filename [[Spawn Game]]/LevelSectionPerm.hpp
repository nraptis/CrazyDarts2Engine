//
//  LevelSectionPerm.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/5/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelSectionPerm_hpp
#define LevelSectionPerm_hpp

#include "GameObject.hpp"
#include "LevelFormation.hpp"
#include "LevelPath.hpp"
#include "LevelPermSpawn.hpp"
#include "LevelMotionController.hpp"
#include "LevelStyleController.hpp"
#include "LevelFormationConfiguration.hpp"

class LevelSection;
class LevelSectionPerm {
public:
    LevelSectionPerm(LevelSection *pSection);
    ~LevelSectionPerm();
    
    void                                        EditorDisposeAll();
    
    void                                        Reset();
    void                                        SoftReset();
    
    
    void                                        Prepare();
    void                                        Spawn();
    
    bool                                        mDidSpawn;
    
    bool                                        IsClearForSectionCompletion();
    bool                                        HasAnyObjects();
    void                                        HandOffAllGameObjects(FList *pList);
    
    
    void                                        Update();
    void                                        Draw();
    
    void                                        PositionObject();
    
    void                                        DisposeObject(GameObject *pObject);
    bool                                        ContainsObject(GameObject *pObject);
    void                                        DisposeAllObjects();
    
    
    //Possibility 1.) We have a formation...
    FString                                     mFormationID;
    LevelFormationConfiguration                 mFormationConfiguration;
    
    //Possibility 2.) We have an object...
    int                                         mObjectType;
    
    FVec2                                       ConvertLocalPointToGame(FVec2 pPos);
    
    //Since the permanents will be positioned relative
    //to the "section" we need a base position and real
    float                                       mBaseX;
    float                                       mBaseY;
    
    float                                       mX;
    float                                       mY;
    
    GameObject                                  *mObject;
    LevelFormation                              *mFormation;
    
    int                                         mObjectID;
    
    float                                       mObjectX;
    float                                       mObjectY;
    float                                       mObjectRotation;
    
    LevelSection                                *mSection;
    
    void                                        RefreshSection(LevelSection *pSection);
    
    LevelPath                                   mPath;
    
    LevelMotionController                       mMotionController;
    bool                                        mIsPlayingEnter;
    bool                                        mIsPlayingExit;
    
    LevelStyleController                        mStyleController;
    
    FList                                       mSpawnList; //of LevelPermSpawn
    bool                                        mSpawnEqualSpacing;
    
    //Base spacing for spawns, if we are not using equal spacing...
    int                                         mSpawnSpacing;
    
    int                                         mSpawnOffset;
    
    int                                         mKillTimer;
    
    
    bool                                        mPauseExit;
    bool                                        ShouldPauseExit();
    void                                        BeginPauseExit();
    bool                                        IsPauseExitComplete();
    
    void                                        DrawDeadObjectMarkers();
    void                                        DisableDeadObjectMarkers();
    bool                                        mDisableDeadObjectMarkers;
    
    bool                                        mShortCircuit;
    bool                                        mShortCircuitExternal;
    
private:
    
    FList                                       mSpawnKillList;
    FList                                       mSpawnDeleteList;
    
    bool                                        mDidShortCircuit;
    bool                                        mDidKnockDown;

};


#endif /* LevelSectionPerm_hpp */
