//
//  LevelFormationTracerTracer.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 3/31/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelFormationTracerTracer_hpp
#define LevelFormationTracerTracer_hpp

#include "LevelFormationTracerNode.hpp"
#include "FList.hpp"
#include "FPointList.hpp"
#include "GameObject.hpp"

#define TRACER_SPECIAL_TYPE_NONE 0
#define TRACER_SPECIAL_TYPE_CIRCLE 1
#define TRACER_SPECIAL_TYPE_ROUNDED_RECT 2

class LevelFormation;
class LevelSection;
class LevelFormationTracer {
public:
    LevelFormationTracer(LevelFormation *pFormation);
    ~LevelFormationTracer();
    
    void                                        Reset();
    void                                        Spawn();
    void                                        EditorPickerSpawn();
    
    void                                        Update();
    void                                        DisposeObject(GameObject *pObject);
    bool                                        ContainsObject(GameObject *pObject);
    void                                        DisposeAllObjects();
    
    void                                        DrawDeadObjectMarkers();
    
    FList                                       mSpawnNodeList;
    
    FList                                       mTracerNodeList;
    
    LevelFormation                              *mFormation;
    
    LevelSection                                *mSection;
    
    void                                        RefreshSection(LevelSection *pSection);
    
    bool                                        mDidOriginateOnWave;
    bool                                        mDidOriginateAsPermanent;
    
    bool                                        mPlayOnEnter;
    bool                                        mPlayOnExit;
    
    bool                                        mReverse;
    
    
    
    bool                                        mStopped;
    
    
    
    //////////////
    //   Copy
    //
    float                                       mSpeed;
  
    //If we are a special type, this is our center.
    
    int                                         mSpecialType;
    
    float                                       mSpecialX;
    float                                       mSpecialY;
    
    float                                       mSpecialRadius;
    float                                       mSpecialCornerRadius;
    
    float                                       mSpecialWidth;
    float                                       mSpecialHeight;
    
    
    //
    //////////////
    
    
    //Note: Path is NOT transformed...
    FPointList                                  mPath;
    FFloatList                                  mDist;
    
    int                                         mKillTimer;
    
    
    int                                         mSpawnPathOffset;
    
    int                                         mPathIndex;
    
    void                                        SetSpeedClass(int pSpeedClass);
    
    
    //We must clone inside of a formation (basically, only the formation will call this)
    LevelFormationTracer                        *Clone(LevelFormation *pFormation);
    
    
    void                                        HandOffAllGameObjects(FList *pList);
    
    
    void                                        GetExtremeties(float &pTop, float &pRight, float &pBottom, float &pLeft);
    void                                        EditorKillAllObjects();
    
    bool                                        HasPauseExitType();
    
    void                                        EditorDisposeAll();
    
private:
    
    FList                                       mTracerNodeKillList;
    FList                                       mTracerNodeDeleteList;
    
    FList                                       mSpawnNodeKillList;
    FList                                       mSpawnNodeDeleteList;
    
    
    void                                        AddDumpPointsCircle(float pX, float pY);
    void                                        AddDumpPointsRoundedRect(float pX, float pY);
    
    
    
    
};
    
#endif /* LevelFormationTracerTracer_hpp */
