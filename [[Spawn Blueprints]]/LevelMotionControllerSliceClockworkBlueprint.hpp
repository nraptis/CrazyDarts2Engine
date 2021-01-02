//
//  LevelMotionControllerSliceClockworkBlueprint.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/29/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelMotionControllerSliceClockworkBlueprint_hpp
#define LevelMotionControllerSliceClockworkBlueprint_hpp

#include "LevelMotionControllerSliceClockwork.hpp"
#include "LevelMotionControllerSliceBlueprint.hpp"
#include "FJSON.hpp"

#define MAX_CLOCKWORK_NODE_COUNT 14

class LevelMotionControllerSliceClockworkBlueprintNode {
public:
    LevelMotionControllerSliceClockworkBlueprintNode();
    ~LevelMotionControllerSliceClockworkBlueprintNode();
    
    FJSONNode                                               *Save();
    void                                                    Load(FJSONNode *pNode);
    
    int                                                     mTime;
    
    int                                                     mOffsetX;
    int                                                     mOffsetY;
    int                                                     mRotation;
    
    int                                                     mPause;
    
    bool                                                    mTranslationLinear;
    bool                                                    mRotationLinear;
    
};

class LevelMotionControllerSliceClockworkBlueprint : public LevelMotionControllerSliceBlueprint {
public:
    LevelMotionControllerSliceClockworkBlueprint();
    virtual ~LevelMotionControllerSliceClockworkBlueprint();
    
    void                                                    Build(LevelMotionControllerSliceClockwork *pSlice);
    
    virtual FJSONNode                                       *Save();
    virtual void                                            Load(FJSONNode *pNode);
    
    LevelMotionControllerSliceClockworkBlueprintNode        mNode[MAX_CLOCKWORK_NODE_COUNT + 1];
    int                                                     mSelectedNodeIndex;
    
    int                                                     mNodeCount;
    int                                                     mStartNode;
    bool                                                    mPauseStart;
    bool                                                    mClosed;
    
    void                                                    QuickSetRotation(int pCount);
    void                                                    QuickSetAllTime(int pTime);
    void                                                    QuickSetAllPause(int pTime);
    void                                                    QuickSetAllRotation(int pRotation);
    
    
    void                                                    QuickSetClearX();
    void                                                    QuickSetClearY();
           
    
    void                                                    QuickSetSpin();
    
    
    void                                                    QuickSetMoveX(const char *pInstructions);
    void                                                    QuickSetMoveY(const char *pInstructions);
    
    void                                                    QuickSetRotateBackForth(float pStartAngle, float pEndAngle, int pStepsIn, int pStepsOut);
    
    void                                                    QuickSetRotateFullCircle(float pStartAngle, bool pReverse, int pSteps);

};

#endif
