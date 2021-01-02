//
//  LevelMotionController.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/13/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelMotionController_hpp
#define LevelMotionController_hpp

#include "GameObject.hpp"
#include "LevelMotionControllerSlice.hpp"
#include "LevelMotionControllerSliceClockwork.hpp"

class LevelMotionController {
public:
    LevelMotionController();
    ~LevelMotionController();
    
    void                                Reset();
    
    void                                Update();
    void                                Draw();
    
    bool                                ShouldPauseExit();
    void                                BeginPauseExit();
    bool                                IsPauseExitComplete();
    
    

    //void                                Apply(float pReferenceX, float pReferenceY, GameObject *pObject);
    void                                Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    float                               ApplyRotation();
    float                               ApplyRotationCannon();
    float                               ApplyRotationPlatform();
    
    
    //float                               ApplyRotationForced();
    
    
    
    
    bool                                mPlayOnEnter;
    bool                                mPlayOnExit;
    
    bool                                mSkipUpdate;
    
    int                                 mKillTimer;
    
    FList                               mSliceList;
    
private:
    FList                               mKillList;
    FList                               mDeleteList;
    
    
};


#endif
