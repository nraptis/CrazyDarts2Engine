//
//  LevelSectionBlueprintInfo.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 11/7/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelSectionBlueprintInfo.hpp"

LevelSectionBlueprintInfo::LevelSectionBlueprintInfo(LevelSectionBlueprint *pSection) {
    mSection = pSection;
    
    int aBalloonCount = CountType(GAME_OBJECT_TYPE_BALLOON);
    int aBrickHeadCount = CountType(GAME_OBJECT_TYPE_BRICKHEAD);
    int aTurtleCount = CountType(GAME_OBJECT_TYPE_TURTLE);
    int aBombCount = CountType(GAME_OBJECT_TYPE_BOMB);
    int aCannonCount = CountType(GAME_OBJECT_TYPE_CANNON);
    
    int aPlatformSmallCount = CountType(GAME_OBJECT_TYPE_PLATFORM_SMALL);
    int aPlatformMediumCount = CountType(GAME_OBJECT_TYPE_PLATFORM_MEDIUM);
    int aPlatformLargeCount = CountType(GAME_OBJECT_TYPE_PLATFORM_LARGE);
    int aPlatformExtraLargeCount = CountType(GAME_OBJECT_TYPE_PLATFORM_EXTRA_LARGE);
    
    int aPlatformCount = (aPlatformSmallCount + aPlatformMediumCount + aPlatformLargeCount + aPlatformExtraLargeCount);
    int aTotalCount = (aBalloonCount + aBrickHeadCount + aTurtleCount + aBombCount + aCannonCount + aPlatformCount);
    
    
    Log("aBalloonCount = %d, aBrickHeadCount = %d, aTurtleCount = %d, aTotalCount = %d\n", aBalloonCount, aBrickHeadCount, aTurtleCount, aTotalCount);
    
    
    mOnlyBalloons = (aTotalCount == aBalloonCount);

    mIncludesBrickHead = (aBrickHeadCount > 0);
    mIncludesTurtle = (aTurtleCount > 0);
    mIncludesBombs = (aBombCount > 0);
    mIncludesCannon = (aCannonCount > 0);
    mIncludesPlatform = (aPlatformCount > 0);
    
    
    
    /*
    bool                                        ;
    bool                                        ;
    bool                                        ;
    bool                                        ;
    bool                                        ;

    int                                         mFormationType;

    int                                         mPermType;
    int                                         mPermCount;

    int                                         mWaveType;
    int                                         mWaveCount;
    */
    
}

LevelSectionBlueprintInfo::~LevelSectionBlueprintInfo() {
    
}

int LevelSectionBlueprintInfo::CountType(int pGameObjectType) {
    int aResult = 0;
    aResult += mSection->CountObjectsOfType(pGameObjectType);
    return aResult;
}


