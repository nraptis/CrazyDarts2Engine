//
//  LevelSectionBlueprintInfo.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 11/7/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef LevelSectionBlueprintInfo_hpp
#define LevelSectionBlueprintInfo_hpp

#include "LevelSectionBlueprint.hpp"

class LevelSectionBlueprintInfo {
public:
    LevelSectionBlueprintInfo(LevelSectionBlueprint *pSection);
    ~LevelSectionBlueprintInfo();
    
    LevelSectionBlueprint                       *mSection;
    
    bool                                        mOnlyBalloons;

    bool                                        mIncludesBrickHead;
    bool                                        mIncludesTurtle;
    bool                                        mIncludesCannon;
    bool                                        mIncludesPlatform;
    bool                                        mIncludesBombs;

    int                                         mFormationType;

    int                                         mPermType;
    int                                         mPermCount;

    int                                         mWaveType;
    int                                         mWaveCount;
    
    int                                         CountType(int pGameObjectType);
    
};

#endif
