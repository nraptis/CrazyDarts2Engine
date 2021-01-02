//
//  GameOverlayEffectsEffects.hpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 9/3/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#ifndef GameOverlayEffectsEffects_hpp
#define GameOverlayEffectsEffects_hpp

#include "GFXApp.hpp"
#include "FSprite.hpp"
#include "FPointList.hpp"

class Game;
class GameObject;
class Dart;
class Balloon;
class FreeLife;
class Cannon;
class Blimp;
class FloatingTextBubble;
class Bomb;
class GameOverlayEffects : public FCanvas {
public:
    GameOverlayEffects();
    virtual ~GameOverlayEffects();
    
    virtual void                                Layout() override;
    virtual void                                Update() override;
    virtual void                                Draw() override;
    
    void                                        Get2DPos(GameObject *pObject, float &pX, float &pY);
    void                                        Get2DPos(float &pX, float &pY);
    
    
    void                                        GenerateBalloonPopAnimation(Balloon *pBalloon);
    void                                        GenerateBalloonPopAnimation(float pX, float pY);
    
    void                                        GenerateFreeLifePopAnimation(FreeLife *pFreeLife);
    void                                        GenerateFreeLifePopAnimation(float pX, float pY);
    
    void                                        GenerateBlimpPopAnimation(Blimp *pBlimp);
    void                                        GenerateBlimpPopAnimation(float pX, float pY);
    
    void                                        GenerateCannonImpactSuccessAnimation(Cannon *pCannon);
    void                                        GenerateCannonImpactSuccessAnimation(float pX, float pY, float pRotation);
    
    
    void                                        GenerateCannonImpactFailAnimation(Cannon *pCannon);
    void                                        GenerateCannonImpactFailAnimation(float pX, float pY, float pRotation);
    
    void                                        GenerateCannonBlastAnimation(Cannon *pCannon);
    void                                        GenerateCannonBlastAnimation(float pX, float pY, float pRotation);
    
    void                                        GenerateBombExplodeAnimation(Bomb *pBomb);
    void                                        GenerateBombExplodeAnimation(float pX, float pY);
    
    
    
    void                                        GenerateDartBrickCollideAnimation(Dart *pDart);
    void                                        GenerateDartBrickCollideAnimation(float pX, float pY, float pDartRotation);
    
    void                                        GenerateDartBrickBeginFadeAnimation(Dart *pDart);
    void                                        GenerateDartBrickBeginFadeAnimation(float pX, float pY, float pDartRotation);
    
    void                                        GenerateDartBrickEndFadeAnimation(Dart *pDart);
    void                                        GenerateDartBrickEndFadeAnimation(float pX, float pY, float pDartRotation);
    
    
    
    //This is the 2-D location where the source exists at...
    void                                        PlaceFloatingText(FloatingTextBubble *pBubble, float pX, float pY);
    
    
    FObjectList                                 mFloatingTextBubbleList;
    
    FObjectList                                 mEffectListBalloonBursts;
    FObjectList                                 mEffectListDartFadeStar;
    
    FObjectList                                 mEffectListDirectionalStar;
    
    FObjectList                                 mEffectListExplosionSmokePuffs;
    
    FObjectList                                 mEffectListVanishPuffs;
    
    FObjectList                                 mEffectListSizzleParticles;
    
    FObjectList                                 mEffectListBlimpBlast;
    
    
    //
    void                                        SpawnLifeBubble(float pX, float pY, bool pPlus, int pNumber);
    
    void                                        SpawnComboBubble(float pX, float pY, const char *pTitle, const char *pText);
    
    
    
    
    void                                        AttemptToPlaceBubble(float &pX, float &pY, float pWidth, float pHeight, FPointList *pExploreList);
    
    
    void                                        BuildTextBubbleExploreGrid();
    FPointList                                  mTextBubbleExploreList;
    
    //FPointList                                  mTextBubbleExploreListUL;
    //FPointList                                  mTextBubbleExploreListU;
    //FPointList                                  mTextBubbleExploreListUR;
    
    //FPointList                                  mTextBubbleExploreListL;
    //FPointList                                  mTextBubbleExploreListR;
    
    //FPointList                                  mTextBubbleExploreListDL;
    //FPointList                                  mTextBubbleExploreListD;
    //FPointList                                  mTextBubbleExploreListDR;
    
    float                                       mGameCorner1X;
    float                                       mGameCorner1Y;
    
    float                                       mGameCorner2X;
    float                                       mGameCorner2Y;
    
    float                                       mGameCorner3X;
    float                                       mGameCorner3Y;
    
    float                                       mGameCorner4X;
    float                                       mGameCorner4Y;
    
    
    
private:
    
    bool                                        FindGameAreaEdgeToCenterCollisionPoint(float pX, float pY, float pLineX1, float pLineY1, float pLineX2, float pLineY2, float &pResultX, float &pResultY);
    bool                                        IsNearParallel(float pX, float pY, float pLineX1, float pLineY1, float pLineX2, float pLineY2);
    
};

extern GameOverlayEffects *gEffectsOverlay;

#endif /* GameOverlayEffectsEffects_hpp */
