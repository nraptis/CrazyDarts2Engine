//
//  MagicArrow.hpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 1/7/20.
//  Copyright © 2020 Froggy Studios. All rights reserved.
//

#ifndef MagicArrow_hpp
#define MagicArrow_hpp

#include "FPointList.hpp"
#include "FPolyPath.hpp"
#include "FTextureSnake.hpp"
#include "FTextureSnakeWithBookend.hpp"
#include "FDrawQuad.hpp"

class MagicArrow {
public:
    MagicArrow();
    ~MagicArrow();
    
    void                                    Hide();
    void                                    Show();
    
    void                                    Update();
    
    void                                    Refresh(FPointList *pPointList, bool pIsNarrow);
    void                                    Draw();
    
    FPointList                              mPointList;
    FPolyPath                               mPolyPath;
    
    FPointList                              mBoneList;
    
    //FDrawNodeList                           mNodeList;
    
    
    bool                                    mHidden;
    
    FTextureSnakeWithBookend                 mSnake;
    FTextureSnakeWithBookend                 mSnakeWhite;
    FTextureSnakeWithBookend                 mSnakeShadow;
    
    float                                   mHeadRotation;
    
    float                                   mHeadX;
    float                                   mHeadY;
    
    float                                   mHeadDirX;
    float                                   mHeadDirY;
    
    float                                   GetShadowShiftX();
    float                                   GetShadowShiftY();
    
    
private:
    
    void                                    ComputeHeadRotation(); //Based on the Bone List
    
    void                                    TruncateBoneList();
    
    
    
};

#endif
