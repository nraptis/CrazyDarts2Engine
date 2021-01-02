//
//  LevelFormationConfiguration.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 9/8/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "LevelFormationConfiguration.hpp"
#include "LevelFormation.hpp"
#include "LevelPath.hpp"

LevelFormationConfiguration::LevelFormationConfiguration() {
    
    mTracerSpeedGlobal = SPEED_CLASS_DEFAULT;
    mTracerSpeed1 = SPEED_CLASS_DEFAULT;
    mTracerSpeed2 = SPEED_CLASS_DEFAULT;
    mTracerSpeed3 = SPEED_CLASS_DEFAULT;
    mTracerSpeed4 = SPEED_CLASS_DEFAULT;
    mTracerSpeed5 = SPEED_CLASS_DEFAULT;
    mTracerSpeed6 = SPEED_CLASS_DEFAULT;
    mTracerSpeed7 = SPEED_CLASS_DEFAULT;
    mTracerSpeed8 = SPEED_CLASS_DEFAULT;
    
    mTracerOffsetGlobal = 0;
    mTracerOffset1 = 0;
    mTracerOffset2 = 0;
    mTracerOffset3 = 0;
    mTracerOffset4 = 0;
    mTracerOffset5 = 0;
    mTracerOffset6 = 0;
    mTracerOffset7 = 0;
    mTracerOffset8 = 0;
    
    mTracerReverse1 = false;
    mTracerReverse2 = false;
    mTracerReverse3 = false;
    mTracerReverse4 = false;
    mTracerReverse5 = false;
    mTracerReverse6 = false;
    mTracerReverse7 = false;
    mTracerReverse8 = false;
    
    mTracerReverseGlobal = false;
    
    mTracerPlayOnEnterGlobal = false;
    mTracerPlayOnExitGlobal = false;
    
}

LevelFormationConfiguration::~LevelFormationConfiguration() {
    
}

void LevelFormationConfiguration::Reset() {
    
    mTracerSpeedGlobal = SPEED_CLASS_DEFAULT;
    mTracerSpeed1 = SPEED_CLASS_DEFAULT;
    mTracerSpeed2 = SPEED_CLASS_DEFAULT;
    mTracerSpeed3 = SPEED_CLASS_DEFAULT;
    mTracerSpeed4 = SPEED_CLASS_DEFAULT;
    mTracerSpeed5 = SPEED_CLASS_DEFAULT;
    mTracerSpeed6 = SPEED_CLASS_DEFAULT;
    mTracerSpeed7 = SPEED_CLASS_DEFAULT;
    mTracerSpeed8 = SPEED_CLASS_DEFAULT;
    
    mTracerOffsetGlobal = 0;
    mTracerOffset1 = 0;
    mTracerOffset2 = 0;
    mTracerOffset3 = 0;
    mTracerOffset4 = 0;
    mTracerOffset5 = 0;
    mTracerOffset6 = 0;
    mTracerOffset7 = 0;
    mTracerOffset8 = 0;
    
    mTracerReverseGlobal = false;
    mTracerReverse1 = false;
    mTracerReverse2 = false;
    mTracerReverse3 = false;
    mTracerReverse4 = false;
    mTracerReverse5 = false;
    mTracerReverse6 = false;
    mTracerReverse7 = false;
    mTracerReverse8 = false;
    
    mTracerPlayOnEnterGlobal = false;
    mTracerPlayOnExitGlobal = false;
    
}

void LevelFormationConfiguration::Apply(LevelFormation *pFormation) {
    
    if (pFormation == NULL) {
        return;
    }
    
    if (mTracerReverseGlobal == true) {
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            aTracer->mReverse = true;
        }
    }
    
    if (mTracerPlayOnEnterGlobal == true) {
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            aTracer->mPlayOnEnter = true;
        }
    }
    
    if (mTracerPlayOnExitGlobal == true) {
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            aTracer->mPlayOnExit = true;
        }
    }
    
    if (mTracerSpeedGlobal != SPEED_CLASS_DEFAULT) {
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            aTracer->SetSpeedClass(mTracerSpeedGlobal);
        }
    }
    
    if (mTracerSpeed1 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 0) {
                aTracer->SetSpeedClass(mTracerSpeed1);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed2 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 1) {
                aTracer->SetSpeedClass(mTracerSpeed2);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed3 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 2) {
                aTracer->SetSpeedClass(mTracerSpeed3);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed4 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 3) {
                aTracer->SetSpeedClass(mTracerSpeed4);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed5 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 4) {
                aTracer->SetSpeedClass(mTracerSpeed5);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed6 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 5) {
                aTracer->SetSpeedClass(mTracerSpeed6);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed7 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 6) {
                aTracer->SetSpeedClass(mTracerSpeed7);
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerSpeed8 != SPEED_CLASS_DEFAULT) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 7) {
                aTracer->SetSpeedClass(mTracerSpeed8);
                break;
            }
            aIndex++;
        }
    }
    
    
    
    if (mTracerOffsetGlobal != 0) {
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            aTracer->mSpawnPathOffset = mTracerOffsetGlobal;
        }
    }
    
    if (mTracerOffset1 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 0) {
                aTracer->mSpawnPathOffset = mTracerOffset1;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset2 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 1) {
                aTracer->mSpawnPathOffset = mTracerOffset2;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset3 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 2) {
                aTracer->mSpawnPathOffset = mTracerOffset3;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset4 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 3) {
                aTracer->mSpawnPathOffset = mTracerOffset4;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset5 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 4) {
                aTracer->mSpawnPathOffset = mTracerOffset5;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset6 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 5) {
                aTracer->mSpawnPathOffset = mTracerOffset6;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset7 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 6) {
                aTracer->mSpawnPathOffset = mTracerOffset7;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerOffset8 != 0) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 7) {
                aTracer->mSpawnPathOffset = mTracerOffset8;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse1 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 0) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse2 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 1) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse3 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 2) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse4 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 3) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse5 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 4) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse6 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 5) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse7 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 6) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    if (mTracerReverse8 == true) {
        int aIndex = 0;
        EnumList(LevelFormationTracer, aTracer, pFormation->mTracerList) {
            if (aIndex == 7) {
                 aTracer->mReverse = true;
                break;
            }
            aIndex++;
        }
    }
    
    
}
