//
//  LevelWaveLinkBlueprint.hpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 3/8/20.
//  Copyright © 2020 Froggy Studios. All rights reserved.
//

#ifndef LevelWaveLinkBlueprint_hpp
#define LevelWaveLinkBlueprint_hpp

#include "FJSON.hpp"

class LevelWaveLink;
//LevelWaveLink.hpp
class LevelWaveLinkBlueprint {
public:
    LevelWaveLinkBlueprint();
    ~LevelWaveLinkBlueprint();
    
    int                                     mLink_1_1;
    int                                     mLink_1_2;
    
    int                                     mLink_2_1;
    int                                     mLink_2_2;
    
    int                                     mLink_3_1;
    int                                     mLink_3_2;
    
    int                                     mLink_4_1;
    int                                     mLink_4_2;
    
    void                                    Reset();
    
    bool                                    ShouldSave();
    FJSONNode                               *Save();
    void                                    Load(FJSONNode *pNode);
    
};

#endif /* LevelWaveLinkBlueprint_hpp */
