//
//  LevelMotionControllerSliceClockwork.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/29/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelMotionControllerSliceClockwork_hpp
#define LevelMotionControllerSliceClockwork_hpp

#include "LevelMotionControllerSlice.hpp"

class LevelMotionControllerSliceClockworkNode {
public:
    LevelMotionControllerSliceClockworkNode();
    ~LevelMotionControllerSliceClockworkNode();
    
    float                               mOffsetX;
    float                               mOffsetY;
    float                               mRotation;
    
    int                                 mPause;
    
    int                                 mTime;
    
    bool                                mTranslationLinear;
    bool                                mRotationLinear;
    
    
    
};

class LevelMotionControllerSliceClockwork : public LevelMotionControllerSlice {
public:
    LevelMotionControllerSliceClockwork();
    virtual ~LevelMotionControllerSliceClockwork();
    
    virtual void                                Update();
    
    virtual void                                Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual float                               ApplyToObjectRotation();
    
    virtual bool                                ShouldPauseExit();
    virtual bool                                IsPauseExitComplete();
    
    void                                        AdvanceIndex();
    
    
    
    //Assumption, the rest of the structure is loaed / populated...
    //This should be called once before spawning...
    void                                        ApplyStartingCondition(int pStartNode, bool pPauseStart);
    
    
    FList                                       mNodeList;
    
    int                                         mPauseTimer;
    
    bool                                        mClosed;
    
    int                                         mCurrentNodeIndex;
    int                                         mCurrentNodeTimer;
    
    float                                       mOffsetX;
    float                                       mOffsetY;
    float                                       mRotation;
    
    LevelMotionControllerSliceClockworkNode     *mNodeCurrent;
    LevelMotionControllerSliceClockworkNode     *mNodeNext;
    
    
    
    
    
    
};


#endif /* LevelMotionControllerSliceClockwork_hpp */
