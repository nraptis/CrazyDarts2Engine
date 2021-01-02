//
//  SkyRenderer.hpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 9/14/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef SkyRenderer_hpp
#define SkyRenderer_hpp

#include "FDrawQuad.hpp"
#include "SkyRendererCloud.hpp"
#include "FIntList.hpp"

class SkyRenderer {
public:
    SkyRenderer();
    ~SkyRenderer();
    
    void                        Refresh(float pTop, float pRight, float pBottom, float pLeft, float pPadding);
    FDrawQuadGrid               mGrid;
    
    FObjectList                 mCloudList;
    
    void                        Update();
    void                        Draw();
    void                        DrawClouds();
    
    void                        SpawnCloud();
    
    float                       mTop;
    float                       mRight;
    float                       mBottom;
    float                       mLeft;
    
    int                         mRecentSpawnTimer;
    
    FIntList                    mTempCloudIndexList;
    FSprite                     *GetCloudSpriteForIndex(int pIndex);
    int                         GetCloudIndexCount();
    
    void                        AttemptInitialLayout();
    
    bool                        IsCloudBlocked(FSprite *pCloudSprite, float pX, float pY);
    
    int                         GetSpawnIndex();
    
};


#endif /* SkyRenderer_hpp */
