//
//  LevelMotionControllerSlice.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nicholas Raptis on 4/19/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelMotionControllerSlice_hpp
#define LevelMotionControllerSlice_hpp

#define LEVEL_MOTION_SLICE_TYPE_NONE 0
#define LEVEL_MOTION_SLICE_TYPE_NEGATE 1
#define LEVEL_MOTION_SLICE_TYPE_ROTATE 2
#define LEVEL_MOTION_SLICE_TYPE_OSCILLATE_ROTATION 3
#define LEVEL_MOTION_SLICE_TYPE_OSCILLATE_VERTICAL 4
#define LEVEL_MOTION_SLICE_TYPE_OSCILLATE_HORIZONTAL 5
#define LEVEL_MOTION_SLICE_TYPE_CLOCKWORK 6
#define LEVEL_MOTION_SLICE_TYPE_FIXED_MOD 7

#include "GameObject.hpp"

class LevelMotionControllerSlice {
public:
    LevelMotionControllerSlice();
    virtual ~LevelMotionControllerSlice();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY) { }
    virtual float                       ApplyToObjectRotation() { return 0.0f; }
    
    virtual bool                        ShouldPauseExit();
    void                                BeginPauseExit();
    virtual bool                        IsPauseExitComplete();
    
    
    virtual void                        Update() { }
    
    bool                                mPauseExit;
    
    int                                 mType;
    
    //The "primary" speed class for this slice. For example, if we're
    //a rotation movement slice, this will effect the rotation speed.
    int                                 mSpeedClass;
    
    bool                                mSpeedNegateAlways;
    bool                                mSpeedNegateRandomly;
    
    bool                                mRotatesObject;
    bool                                mRotatesCannons;
    bool                                mRotatesPlatforms;
    
    
    
    bool                                mSpeedNegateRandomlyFlag;
    
    int                                 mKillTimer;
};

//

class LevelMotionControllerSliceNegate : public LevelMotionControllerSlice {
public:
    LevelMotionControllerSliceNegate();
    virtual ~LevelMotionControllerSliceNegate();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual void                        Update();
    
    bool                                mNegateHAlways;
    bool                                mNegateHRandomly;
    bool                                mNegateHRandomlyFlag;
    
    bool                                mNegateVAlways;
    bool                                mNegateVRandomly;
    bool                                mNegateVRandomlyFlag;
};

class LevelMotionControllerSliceFixedMod : public LevelMotionControllerSlice {
public:
    LevelMotionControllerSliceFixedMod();
    virtual ~LevelMotionControllerSliceFixedMod();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual float                       ApplyToObjectRotation();
    
    virtual void                        Update();
    
    float                               mOffsetX;
    float                               mOffsetY;
    float                               mRotation;
    
};

class LevelMotionControllerSliceRotate : public LevelMotionControllerSlice {
public:
    LevelMotionControllerSliceRotate();
    virtual ~LevelMotionControllerSliceRotate();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual float                       ApplyToObjectRotation();
    
    
    virtual void                        Update();
    
    float                               mPivotOffsetX;
    float                               mPivotOffsetY;
    
    
    float                               mRotation;
};

class LevelMotionControllerSliceOscillateGeneric : public LevelMotionControllerSlice {
public:
    LevelMotionControllerSliceOscillateGeneric();
    virtual ~LevelMotionControllerSliceOscillateGeneric();
    
    virtual void                        Update();
    float                               GetPercent();
    
    virtual bool                        ShouldPauseExit();
    virtual bool                        IsPauseExitComplete();
    
    void                                ApplyPauseStartType();
    
    float                               mOscillateSin;
    
    int                                 mLinearTimer;
    int                                 mLinearTime;
    int                                 mLinearDir;
    
    bool                                mIsPaused;
    int                                 mPauseSide;
    int                                 mPauseTimer;
    
    bool                                mLinear;
    
    int                                 mPause1;
    int                                 mPause2;
    
    int                                 mPauseStartType;
};

class LevelMotionControllerSliceOscillateRotation : public LevelMotionControllerSliceOscillateGeneric {
public:
    LevelMotionControllerSliceOscillateRotation();
    virtual ~LevelMotionControllerSliceOscillateRotation();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual float                       ApplyToObjectRotation();
    virtual void                        Update();
    
    float                               mAngleSpan;
    float                               mAngleSpanOffsetStart;
    float                               mAngleSpanOffsetEnd;
    
    
};

class LevelMotionControllerSliceOscillateHorizontal : public LevelMotionControllerSliceOscillateGeneric {
public:
    LevelMotionControllerSliceOscillateHorizontal();
    virtual ~LevelMotionControllerSliceOscillateHorizontal();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual void                        Update();
    
    float                               mRadius;
};

class LevelMotionControllerSliceOscillateVertical : public LevelMotionControllerSliceOscillateGeneric {
public:
    LevelMotionControllerSliceOscillateVertical();
    virtual ~LevelMotionControllerSliceOscillateVertical();
    
    virtual void                        Apply(float pReferenceX, float pReferenceY, float *pX, float *pY);
    virtual void                        Update();
    
    float                               mRadius;
};



#endif /* LevelMotionControllerSlice_hpp */
