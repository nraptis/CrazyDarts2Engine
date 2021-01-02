//
//  LevelFormationConfigurationBlueprint.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/8/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelFormationConfigurationBlueprint_hpp
#define LevelFormationConfigurationBlueprint_hpp

#include "FJSON.hpp"
#include "LevelMotionControllerBlueprint.hpp"

class LevelFormationConfiguration;
class LevelFormationConfigurationBlueprint {
public:
    LevelFormationConfigurationBlueprint();
    ~LevelFormationConfigurationBlueprint();
    
    void                                        Reset();
    
    FJSONNode                                   *Save();
    void                                        Load(FJSONNode *pNode);
    
    bool                                        ShouldSave();
    
    void                                        Build(LevelFormationConfiguration *pConfiguration);
    
    bool                                        mTracerPlayOnEnterGlobal;
    bool                                        mTracerPlayOnExitGlobal;
    
    
    int                                         mTracerOffsetGlobal;
    int                                         mTracerOffset1;
    int                                         mTracerOffset2;
    int                                         mTracerOffset3;
    int                                         mTracerOffset4;
    int                                         mTracerOffset5;
    int                                         mTracerOffset6;
    int                                         mTracerOffset7;
    int                                         mTracerOffset8;
    
    
    int                                         mTracerSpeedGlobal;
    int                                         mTracerSpeed1;
    int                                         mTracerSpeed2;
    int                                         mTracerSpeed3;
    int                                         mTracerSpeed4;
    int                                         mTracerSpeed5;
    int                                         mTracerSpeed6;
    int                                         mTracerSpeed7;
    int                                         mTracerSpeed8;
    
    bool                                        mTracerReverseGlobal;
    bool                                        mTracerReverse1;
    bool                                        mTracerReverse2;
    bool                                        mTracerReverse3;
    bool                                        mTracerReverse4;
    bool                                        mTracerReverse5;
    bool                                        mTracerReverse6;
    bool                                        mTracerReverse7;
    bool                                        mTracerReverse8;
    
    
};

#endif /* LevelFormationConfigurationBlueprint_hpp */
