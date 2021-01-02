//
//  SkyRendererCloud.hpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 12/6/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef SkyRendererCloud_hpp
#define SkyRendererCloud_hpp

#include "FObject.hpp"
#include "FSprite.hpp"

class SkyRenderer;

class SkyRendererCloud: public FObject {
public:
    SkyRendererCloud(SkyRenderer *pRenderer, FSprite *pSprite, int pIndex);
    ~SkyRendererCloud();
    
    virtual void                            Update() override;
    virtual void                            Draw() override;
    
    float                                   mX;
    float                                   mY;
    
    
    int                                     mIndex;
    
    //The "scale" which converts real 2-D to fake 2-D...
    float                                   mConversionScale;
    
    float                                   mRotation;
    float                                   mScale;
    
    float                                   mZoneLeft;
    float                                   mZoneRight;
    
    float                                   mZoneTop;
    float                                   mZoneBottom;
    
    float                                   mZoneWidth;
    float                                   mZoneHeight;
    
    
    float                                   mSpriteWidth;
    float                                   mSpriteHeight;
    
    float                                   mHotizontalMovementFactor;
    
    FSprite                                 *mSprite;
    
    SkyRenderer                             *mRenderer;
    
    float                                   mPositions[18];
    float                                   mTextureCoords[18];
    
    float                                   mVerticalSin;
    float                                   mVerticalSinSpeed;
    float                                   mVerticalSpread;
    
    
    void                                    SetTopLeftPositions(float pX, float pY);
    void                                    SetTopRightPositions(float pX, float pY);
    void                                    SetBottomLeftPositions(float pX, float pY);
    void                                    SetBottomRightPositions(float pX, float pY);
    
    void                                    SetTopLeftTextureCoords(float pU, float pV);
    void                                    SetTopRightTextureCoords(float pU, float pV);
    void                                    SetBottomLeftTextureCoords(float pU, float pV);
    void                                    SetBottomRightTextureCoords(float pU, float pV);
    
    void                                    HandleRefresh(float pTop, float pRight, float pBottom, float pLeft);
    
    
    
};

#endif
