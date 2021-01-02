//
//  GameRenderer.hpp
//  Crazy Darts 2 iOS
//
//  Created by Nicholas Raptis on 2/26/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef GameRenderer_hpp
#define GameRenderer_hpp

#include "FloatingCamera.hpp"
#include "FModelDataPacked.hpp"
#include "FUniforms.hpp"
#include "SkyRenderer.hpp"

class Game;
class GameRenderer {
public:
    GameRenderer(Game *pGame, FloatingCamera *pCamera);
    ~GameRenderer();
    
    
    void                                Update();
    
    void                                Draw3D();
    void                                ScreenBoundaryRefresh();
    
    Game                                *mGame;
    FloatingCamera                      *mCamera;
    
    SkyRenderer                         mSky;
    
    float                               mLightDirX;
    float                               mLightDirY;
    float                               mLightDirZ;
    
    FUniformsLightPhong                 mUniformBalloon;
    
    FUniformsLightDiffuse               mUniformDartBody;
    FUniformsLightDiffuse               mUniformDartTip;
    FUniformsLightPhong                 mUniformDartGrip;
    
    FUniformsLightPhong                 mUniformDartBat;

    
    FUniformsLightDiffuse               mUniformBrickHead;
    FUniformsLightDiffuse               mUniformBrickHeadSpikes;
    FUniformsLightPhong                 mUniformBrickHeadCage;
    
    FUniformsLightDiffuse               mUniformTurtleBody;
    FUniformsLightPhong                 mUniformTurtleRims;
    
    FUniformsLightDiffuse               mUniformCannonBody;
    FUniformsLightPhong                 mUniformCannonLip;
    
    
    FUniformsLightDiffuse               mUniformBlimpBody;
    FUniformsLightPhong                 mUniformBlimpTail;
    
    
    
    FUniformsLightDiffuse               mUniformStarFaces;
    FUniformsLightPhong                 mUniformStarRims;
    
    
    
    //FUniformsLightPhong                 mUniformPhong;
    //FUniformsLightDiffuse               mUniformDiffuse;
    //FUniformsLightAmbient               mUniformAmbient;
    
    
};

#endif

