//
//  LevelFormationNodeFormationNode.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/31/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelFormationNodeFormationNode_hpp
#define LevelFormationNodeFormationNode_hpp

#include "GameObject.hpp"

class LevelFormation;
class LevelFormationTracer;
class LevelSection;
class LevelFormationNode {
public:
    LevelFormationNode();
    virtual ~LevelFormationNode();
    
    void                                Spawn();
    void                                EditorPickerSpawn();
    
    void                                Reset();
    //void                                Restart();
    
    void                                Update();
    void                                Draw();
    void                                DrawDeadObjectMarkers();
    
    
    void                                DisposeObject(GameObject *pObject);
    bool                                ContainsObject(GameObject *pObject);
    void                                DisposeAllObjects();
    
    void                                PositionObject();
    
    bool                                mDidOriginateOnWave;
    bool                                mDidOriginateAsPermanent;
    
    LevelFormation                      *mFormation;
    LevelFormationTracer                *mTracer;
    
    LevelSection                        *mSection;
    
    void                                RefreshSection(LevelSection *pSection);
    
    GameObject                          *mObject;
    
    int                                 mObjectID;
    
    float                               mObjectX;
    float                               mObjectY;
    float                               mObjectRotation;
    
    
    bool                                mObjectRotate;
    float                               mObjectRotationOffset;
    
    
    int                                 mObjectType;
    
    int                                 mPathIndexOffset;
    
    float                               mBaseX;
    float                               mBaseY;
    
    LevelFormationNode                  *Clone(LevelFormation *pFormation);
    
    
    
    //The positioned X and Y values...
    float                               mX;
    float                               mY;
    
    int                                 mKillTimer;
    
    void                                EditorDisposeAll();
};


#endif /* LevelFormationNodeFormationNode_hpp */
