//
//  GameOverlayEffectsEffects.cpp
//  Crazy Darts 2 iOS
//
//  Created by Nick Raptis on 9/3/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "core_includes.h"
#include "GameContainer.hpp"
#include "GameOverlayEffects.hpp"
#include "EffectBalloonBurst.hpp"
#include "EffectSmallDirectionalStar.hpp"
#include "EffectLargeDirectionalStar.hpp"
#include "EffectExplosionSmoke.hpp"
#include "EffectSizzleParticle.hpp"
#include "EffectVanishPoof.hpp"
#include "EffectBlimpBlast.hpp"
#include "EffectCannonSmoke.hpp"
#include "EffectDartFadeStar.hpp"

#include "FloatingLifeBubble.hpp"
#include "FloatingComboBubble.hpp"

#include "FAnimation.hpp"
#include "Dart.hpp"
#include "StuckDart.hpp"
#include "Balloon.hpp"
#include "BrickHead.hpp"
#include "Turtle.hpp"
#include "Bomb.hpp"
#include "Dart.hpp"
#include "PrizeBox.hpp"
#include "Star.hpp"
#include "FreeLife.hpp"
#include "Cannon.hpp"
#include "PlatformSmall.hpp"
#include "PlatformMedium.hpp"
#include "PlatformLarge.hpp"
#include "PlatformExtraLarge.hpp"
#include "Blimp.hpp"
#include "Game.hpp"
#include "FLine.hpp"
#include "FloatingTextBubble.hpp"

GameOverlayEffects *gEffectsOverlay = NULL;

GameOverlayEffects::GameOverlayEffects() {
    gEffectsOverlay = this;
    mConsumesTouches = false;
    mName = "GameOverlayEffects";
    
    mGameCorner1X = 0.0f;
    mGameCorner1Y = 0.0f;
    mGameCorner2X = 0.0f;
    mGameCorner2Y = 0.0f;
    mGameCorner3X = 0.0f;
    mGameCorner3Y = 0.0f;
    mGameCorner4X = 0.0f;
    mGameCorner4Y = 0.0f;
    
    BuildTextBubbleExploreGrid();
}

GameOverlayEffects::~GameOverlayEffects() {
    if (gEffectsOverlay == this) { gEffectsOverlay = NULL; }
    
    mEffectListBalloonBursts.Free();
    mEffectListDartFadeStar.Free();
    mEffectListDirectionalStar.Free();
    mFloatingTextBubbleList.Free();
    mEffectListExplosionSmokePuffs.Free();
    mEffectListVanishPuffs.Free();
    
    mEffectListSizzleParticles.Free();
    mEffectListBlimpBlast.Free();
    
}

void GameOverlayEffects::Layout() {
    
}

void GameOverlayEffects::Update() {
    
    if ((gGameContainer != NULL) && ((gGameContainer->mPaused == true) || (gGameContainer->mBackgroundPaused == true))) {
        return;
    }
    
    mEffectListBalloonBursts.Update();
    mEffectListDartFadeStar.Update();
    mEffectListDirectionalStar.Update();
    mFloatingTextBubbleList.Update();
    mEffectListExplosionSmokePuffs.Update();
    mEffectListVanishPuffs.Update();
    mEffectListSizzleParticles.Update();
    mEffectListBlimpBlast.Update();
    
}

void GameOverlayEffects::Draw() {
    
    Graphics::SetColor();
    
    //Graphics::PipelineStateSetSpritePremultipliedBlending();
    Graphics::PipelineStateSetSpriteWhiteBlending();
    Graphics::SetColor();
    mEffectListBalloonBursts.Draw();
    
    
    Graphics::PipelineStateSetSpriteWhiteBlending();
    Graphics::SetColor();
    mEffectListBlimpBlast.Draw();
    
    
    
    Graphics::PipelineStateSetSpriteAdditiveBlending();
    mEffectListDartFadeStar.Draw();
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    mEffectListExplosionSmokePuffs.Draw();
    
    Graphics::PipelineStateSetSpriteAdditiveBlending();
    Graphics::SetColor();
    
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    //Graphics::PipelineStateSetSpriteAlphaBlending();
    
    
    Graphics::SetColor();
    mEffectListDirectionalStar.Draw();
    
    
    Graphics::PipelineStateSetSpriteAdditiveBlending();
    Graphics::SetColor();
    mEffectListSizzleParticles.Draw();
    
    
    Graphics::PipelineStateSetSpriteWhiteBlending();
    Graphics::SetColor();
    mEffectListVanishPuffs.Draw();
    
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    Graphics::SetColor();
    EnumList(FloatingTextBubble, aBubble, mFloatingTextBubbleList.mObjectList) {
        if (aBubble->mKill == 0) {
            aBubble->DrawShadow();
        }
    }
    
    Graphics::PipelineStateSetSpritePremultipliedBlending();
    Graphics::SetColor();
    EnumList(FloatingTextBubble, aBubble, mFloatingTextBubbleList.mObjectList) {
        if (aBubble->mKill == 0) {
            aBubble->DrawWhite();
        }
    }
    
    Graphics::PipelineStateSetSpriteAlphaBlending();
    Graphics::SetColor();
    EnumList(FloatingTextBubble, aBubble, mFloatingTextBubbleList.mObjectList) {
        if (aBubble->mKill == 0) {
            aBubble->Draw();
        }
    }
    
    //mFloatingTextBubbleList.Draw();
    
    
    //  //  //  //  //  //
    //                  //
    //                  //
    //                  //
    //  //  //  //  //  //
    //                  //
    //                  //
    //                  //
    //  //  //  //  //  //
    
    
    
    /*
    
    float aEdgePadding = 10.0f;
    float aBottomPadding = 40.0f;
    
    Graphics::PipelineStateSetShape2DAlphaBlending();
    Graphics::SetColor();
    
    float aX = mGameCorner1X;
    float aY = mGameCorner1Y;
    float aWidth = mGameCorner2X - mGameCorner1X;
    float aHeight = mGameCorner3Y - mGameCorner1Y;
    
    Graphics::OutlineRectInside(aX + 5.0f, aY + 5.0f, aWidth - 10.0f, aHeight - 10.0f, 6.0f);
    
    */
    
    //Graphics::DrawLine(aX + aEdgePadding, mHeight - aBottomPadding, mWidth - aEdgePadding, mHeight - aBottomPadding);
    
    
    
}

void GameOverlayEffects::Get2DPos(GameObject *pObject, float &pX, float &pY) {
    pX = mWidth2;
    pY = mHeight2;
    if (pObject != NULL) {
        pX = pObject->mTransform.mX;
        pY = pObject->mTransform.mY;
        FCanvas::ConvertPoint(pX, pY, gGame, this);
    }
}

void GameOverlayEffects::Get2DPos(float &pX, float &pY) {
    FCanvas::ConvertPoint(pX, pY, gGame, this);
}

void GameOverlayEffects::GenerateBalloonPopAnimation(Balloon *pBalloon) {
    
    if (pBalloon == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pBalloon, aX, aY);
    
    GenerateBalloonPopAnimation(aX, aY);
}

void GameOverlayEffects::GenerateBalloonPopAnimation(float pX, float pY) {
    EffectBalloonBurst *aBurst = new EffectBalloonBurst();
    aBurst->mX = pX;
    aBurst->mY = pY;
    mEffectListBalloonBursts.Add(aBurst);
}

void GameOverlayEffects::GenerateFreeLifePopAnimation(FreeLife *pFreeLife) {
    
    if (pFreeLife == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pFreeLife, aX, aY);
    
    GenerateFreeLifePopAnimation(aX, aY);
}

void GameOverlayEffects::GenerateFreeLifePopAnimation(float pX, float pY) {
    
    SpawnLifeBubble(pX, pY, true, 1);
    
    EffectBalloonBurst *aBurst = new EffectBalloonBurst();
    aBurst->mX = pX;
    aBurst->mY = pY;
    mEffectListBalloonBursts.Add(aBurst);
    
    float aArmMin = 6.5f * gSpriteDrawScale;
    float aArmMax = 7.5f * gSpriteDrawScale;
    
    for (int k=0;k<6;k++) {
    
        float aOffsetRotation = gRand.GetRotation();
        
        int aCount = 2;
        if (k == 1) { aCount = 3; }
        if (k == 2) { aCount = 5; }
        if (k == 3) { aCount = 7; }
        if (k == 4) { aCount = 10; }
        if (k == 5) { aCount = 13; }
        
        for (int i=0;i<aCount;i++) {
            
            float aPercent = ((float)i) / ((float)aCount);
            float aRotation = aPercent * 360.0f + aOffsetRotation;
            if (aRotation >= 360.0f) { aRotation -= 360.0f; }
            
            EffectSizzleParticle *aSizzle = new EffectSizzleParticle();
            
            float aPunch = gRand.GetFloat(aArmMin, aArmMax);
            
            float aWidthFactor = 0.75f;
            
            float aDirX = Sin(aRotation);
            float aDirY = -Cos(aRotation);
            
            aSizzle->mBaseX = pX + (aDirX * aPunch) * aWidthFactor;
            aSizzle->mBaseY = pY + (aDirY * aPunch);
            aSizzle->SetSpeed(0.0f, gRand.GetFloat(0.15f, 0.75f) * gSpriteDrawScale);
            mEffectListSizzleParticles.Add(aSizzle);
        }
        aArmMin += 3.25f * gSpriteDrawScale;
        aArmMax += 3.50f * gSpriteDrawScale;
    }
}


void GameOverlayEffects::GenerateBlimpPopAnimation(Blimp *pBlimp) {
    
    if (pBlimp == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pBlimp, aX, aY);
    
    GenerateBlimpPopAnimation(aX, aY);
}

void GameOverlayEffects::GenerateBlimpPopAnimation(float pX, float pY) {
    
    EffectBalloonBurst *aBurst = new EffectBalloonBurst();
    aBurst->mX = pX;
    aBurst->mY = pY;
    mEffectListBalloonBursts.Add(aBurst);
    
    
    for (float aRot=0.0f;aRot<360.0f;aRot += 20.0f) {
        EffectSmallDirectionalStar *aStar = new EffectSmallDirectionalStar();
        aStar->mX = pX;
        aStar->mY = pY;
        aStar->SetColor(0.65f);
        aStar->Fling(aRot, gRand.GetFloat(5.0f, 6.0f) * gSpriteDrawScale, gRand.GetFloat(1.85f, 2.0f) * gSpriteDrawScale);
        aStar->SetAccel(0.95f);
        mEffectListDirectionalStar.Add(aStar);
    }
    
    EffectBlimpBlast *aBlast1 = new EffectBlimpBlast();
    aBlast1->SetPos(pX, pY);
    aBlast1->SetRotation(0.0f);
    mEffectListBlimpBlast.Add(aBlast1);
    
    EffectBlimpBlast *aBlast2 = new EffectBlimpBlast();
    aBlast2->SetPos(pX, pY);
    aBlast2->SetRotation(120.0f);
    mEffectListBlimpBlast.Add(aBlast2);
    
    EffectBlimpBlast *aBlast3 = new EffectBlimpBlast();
    aBlast3->SetPos(pX, pY);
    aBlast3->SetRotation(240.0f);
    mEffectListBlimpBlast.Add(aBlast3);
}

void GameOverlayEffects::GenerateDartBrickCollideAnimation(Dart *pDart) {
    if (pDart == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pDart, aX, aY);
    
    GenerateDartBrickCollideAnimation(aX, aY, pDart->mTransform.mRotation);
}

void GameOverlayEffects::GenerateDartBrickCollideAnimation(float pX, float pY, float pDartRotation) {
    
    float aDirX = Sin(pDartRotation);
    float aDirY = -Cos(pDartRotation);
    
    float aIncidentAngle1 = pDartRotation + 30.0f;
    float aIncidentAngle2 = pDartRotation - 30.0f;
    
    float aSpawnX = pX + aDirX * (6.0f * gSpriteDrawScale);
    float aSpawnY = pY + aDirY * (6.0f * gSpriteDrawScale);
    
    float aMoveSpeed = 2.45f * gSpriteDrawScale;
    
    EffectDartFadeStar *aStar1 = new EffectDartFadeStar();
    aStar1->SetPos(aSpawnX, aSpawnY);
    aStar1->SetSpeed(-Sin(aIncidentAngle1) * aMoveSpeed, Cos(aIncidentAngle1) * aMoveSpeed);
    aStar1->SetAccel(0.95f);
    mEffectListDirectionalStar.Add(aStar1);
    
    EffectDartFadeStar *aStar2 = new EffectDartFadeStar();
    aStar2->SetPos(aSpawnX, aSpawnY);
    aStar2->SetSpeed(-Sin(aIncidentAngle2) * aMoveSpeed, Cos(aIncidentAngle2) * aMoveSpeed);
    aStar2->SetAccel(0.95f);
    mEffectListDirectionalStar.Add(aStar2);
    
    EffectDartFadeStar *aStar3 = new EffectDartFadeStar();
    aStar3->SetPos(aSpawnX, aSpawnY);
    aStar3->SetSpeed(-Sin(pDartRotation) * aMoveSpeed, Cos(pDartRotation) * aMoveSpeed);
    aStar3->SetAccel(0.95f);
    mEffectListDirectionalStar.Add(aStar3);
}

void GameOverlayEffects::GenerateDartBrickBeginFadeAnimation(Dart *pDart) {
    if (pDart == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pDart, aX, aY);
    
    GenerateDartBrickBeginFadeAnimation(aX, aY, pDart->mTransform.mRotation);
}

void GameOverlayEffects::GenerateDartBrickBeginFadeAnimation(float pX, float pY, float pDartRotation) {
    
    
    /*
    for (float aRot=45.0f;aRot<360.0f;aRot += 90.0f) {
        EffectVanishPoof *aPoof = new EffectVanishPoof();
        aPoof->mX = pX;
        aPoof->mY = pY;
        aPoof->Fling(aRot, gRand.GetFloat(2.0f, 2.25f) * gSpriteDrawScale, 1.65 * gSpriteDrawScale);
        aPoof->SetAccel(0.925f);
        mEffectListVanishPuffs.Add(aPoof);
    
    }
    */
    
    float aArmMin = 2.5f * gSpriteDrawScale;
    float aArmMax = 3.5f * gSpriteDrawScale;
    
    for (int k=0;k<3;k++) {
    
        float aOffsetRotation = gRand.GetRotation();
        
        int aCount = 2;
        if (k == 1) { aCount = 3; }
        if (k == 2) { aCount = 4; }
        
        for (int i=0;i<aCount;i++) {
            
            float aPercent = ((float)i) / ((float)aCount);
            float aRotation = aPercent * 360.0f + aOffsetRotation;
            if (aRotation >= 360.0f) { aRotation -= 360.0f; }
            
            EffectSizzleParticle *aSizzle = new EffectSizzleParticle();
            
            float aPunch = gRand.GetFloat(aArmMin, aArmMax);
            
            float aDirX = Sin(aRotation);
            float aDirY = -Cos(aRotation);
            
            aSizzle->SetScale(0.65f);
            
            aSizzle->mBaseX = pX + (aDirX * aPunch);
            aSizzle->mBaseY = pY + (aDirY * aPunch);
            aSizzle->SetSpeed(0.0f, gRand.GetFloat(0.15f, 0.75f) * gSpriteDrawScale);
            
            aSizzle->SetDelayAlphaAddSpeed(gRand.Get(10) + 46, -0.0015f);
            
            mEffectListSizzleParticles.Add(aSizzle);
        }
        aArmMin += 2.25f * gSpriteDrawScale;
        aArmMax += 2.50f * gSpriteDrawScale;
    }
    
}

void GameOverlayEffects::GenerateDartBrickEndFadeAnimation(Dart *pDart) {
    if (pDart == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pDart, aX, aY);
    
    GenerateDartBrickEndFadeAnimation(aX, aY, pDart->mTransform.mRotation);
}

void GameOverlayEffects::GenerateDartBrickEndFadeAnimation(float pX, float pY, float pDartRotation) {
    
    /*
    EffectVanishPoof *aPoof = new EffectVanishPoof();
    aPoof->mX = pX;
    aPoof->mY = pY;
    aPoof->mScale = 0.025f;
    aPoof->mScaleSpeed = 0.1350f;
    aPoof->mScaleSpeedAdd = -0.0068f;
    mEffectListVanishPuffs.Add(aPoof);
    */
    
}

void GameOverlayEffects::GenerateCannonImpactSuccessAnimation(Cannon *pCannon) {
    if (pCannon == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pCannon, aX, aY);
    
    GenerateCannonImpactSuccessAnimation(aX, aY, pCannon->mTransform.mRotation);
}

void GameOverlayEffects::GenerateCannonImpactSuccessAnimation(float pX, float pY, float pRotation) {
    
    for (float aRot=45.0f;aRot<360.0f;aRot += 90.0f) {
        EffectSmallDirectionalStar *aStar = new EffectSmallDirectionalStar();
        aStar->mX = pX;
        aStar->mY = pY;
        aStar->Fling(aRot, 4.0f * gSpriteDrawScale, 1.55f * gSpriteDrawScale);
        aStar->SetAccel(0.895f);
        aStar->SetScale(0.25f, 0.04f, 0.94f);
        aStar->mRed = gRand.GetFloat(0.05f) + 0.95f;
        aStar->mGreen = gRand.GetFloat(0.05f) + 0.95f;
        aStar->mBlue = gRand.GetFloat(0.05f) + 0.95f;
        mEffectListDirectionalStar.Add(aStar);
    }
}

void GameOverlayEffects::GenerateCannonImpactFailAnimation(Cannon *pCannon) {
    if (pCannon == NULL) { return; }
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pCannon, aX, aY);
    GenerateCannonImpactFailAnimation(aX, aY, pCannon->mTransform.mRotation);
}

void GameOverlayEffects::GenerateCannonImpactFailAnimation(float pX, float pY, float pRotation) {
    
    
    for (float aRot=45.0f;aRot<360.0f;aRot += 90.0f) {
        
        
        EffectVanishPoof *aPoof = new EffectVanishPoof();
        aPoof->mX = pX;
        aPoof->mY = pY;
        aPoof->Fling(aRot, 3.0f * gSpriteDrawScale, 1.15f * gSpriteDrawScale);
        aPoof->SetAccel(0.915f);
        aPoof->SetAlpha(0.75f);
        aPoof->SetAlphaSpeed(-0.01f);
        //aPoof->SetRR();
        
        aPoof->mScale = 0.25f;
        aPoof->mScaleSpeed = 0.125f;
        aPoof->mScaleSpeedAdd = -0.0085f;
        
        mEffectListVanishPuffs.Add(aPoof);
        
        /*
        EffectLargeDirectionalStar *
        
        EffectSmallDirectionalStar *aStar = new EffectSmallDirectionalStar();
        aStar->mX = aX;
        aStar->mY = aY;
        aStar->mScale = 0.125f;
        aStar->Fling(aRot, gRand.GetFloat(1.75f, 2.25f) * gSpriteDrawScale, gRand.GetFloat(7.0f, 10.0f) * gSpriteDrawScale);
        aStar->mRed = gRand.GetFloat(0.1f) + 0.4f;
        aStar->mGreen = gRand.GetFloat(0.5f) + 0.4f;
        aStar->mBlue = gRand.GetFloat(0.1f) + 0.4f;
        mEffectListDirectionalStar.Add(aStar);
        */
    }
}

void GameOverlayEffects::GenerateCannonBlastAnimation(Cannon *pCannon) {
    if (pCannon == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pCannon, aX, aY);
    
    GenerateCannonBlastAnimation(aX, aY, pCannon->mTransform.mRotation);
}

void GameOverlayEffects::GenerateCannonBlastAnimation(float pX, float pY, float pRotation) {
    
    for (int aLoop=0;aLoop<3;aLoop++) {
        
        float aSpeed = 0.65f;
        if (aLoop == 1) { aSpeed = 1.25f; }
        if (aLoop == 2) { aSpeed = 1.85f; }
        
        float aOffsetRotation = gRand.GetRotation();
    
        int aCount = 16;
        
        float aArmLength = 24.0f * gSpriteDrawScale;
        float aRadius = 10.0f * gSpriteDrawScale;
        float aSquishFactor = 0.85f;
        
        FVec2 aPoint;
        FVec2 aCenter;
        aCenter.mX = pX;
        aCenter.mY = pY;
        
        for (int i=0;i<aCount;i++) {
            
            float aPercent = ((float)i) / ((float)aCount);
            float aRotation = aPercent * 360.0f + aOffsetRotation;
            if (aRotation >= 360.0f) { aRotation -= 360.0f; }
            
            EffectCannonSmoke *aSmoke = new EffectCannonSmoke();
            
            float aX = pX;
            float aY = pY;
            
            float aDirX = Sin(aRotation);
            float aDirY = -Cos(aRotation);
            
            aX += aDirX * aRadius;
            aY += aDirY * aRadius * aSquishFactor;
            
            aY -= aArmLength;
            
            aPoint.mX = aX;
            aPoint.mY = aY;
            
            aPoint = PivotPoint(aPoint, pRotation, aCenter, 1.0f, 1.0f);
            
            float aBlastDirX = aPoint.mX - pX;
            float aBlastDirY = aPoint.mY - pY;
            
            float aBlastLength = aBlastDirX * aBlastDirX + aBlastDirY * aBlastDirY;
            if (aBlastLength > SQRT_EPSILON) {
                aBlastLength = sqrtf(aBlastLength);
                aBlastDirX /= aBlastLength;
                aBlastDirY /= aBlastLength;
            }
            
            aSmoke->SetSpeed(aBlastDirX * aSpeed * gSpriteDrawScale, aBlastDirY * aSpeed * gSpriteDrawScale);
            
            aSmoke->SetPos(aPoint.mX, aPoint.mY);
            mEffectListExplosionSmokePuffs.Add(aSmoke);
        }
    }
}

void GameOverlayEffects::GenerateBombExplodeAnimation(Bomb *pBomb) {
    
    if (pBomb == NULL) { return; }
    
    float aX = 0.0f, aY = 0.0f;
    Get2DPos(pBomb, aX, aY);
    
    GenerateBombExplodeAnimation(aX, aY);
}

void GameOverlayEffects::GenerateBombExplodeAnimation(float pX, float pY) {
    
    float aAngleOffset = gRand.Rot();
    for (float aRot=0.0f;aRot<360.0f;aRot += 10.0f) {
        EffectExplosionSmoke *aSmoke = new EffectExplosionSmoke();
        aSmoke->mX = pX;
        aSmoke->mY = pY;
        aSmoke->ConfigureBlack();
        aSmoke->Fling(aRot + aAngleOffset, gRand.GetFloat(10.0f, 12.0f) * gSpriteDrawScale, gRand.GetFloat(1.25f, 1.35f) * gSpriteDrawScale);
        mEffectListExplosionSmokePuffs.Add(aSmoke);
    }
    
    aAngleOffset = gRand.Rot();
    for (float aRot=0.0f;aRot<360.0f;aRot += 15.0f) {
        EffectExplosionSmoke *aSmoke = new EffectExplosionSmoke();
        aSmoke->mX = pX;
        aSmoke->mY = pY;
        aSmoke->ConfigureRed();
        aSmoke->Fling(aRot + aAngleOffset, gRand.GetFloat(7.0f, 10.0f) * gSpriteDrawScale, gRand.GetFloat(0.65f, 0.95f) * gSpriteDrawScale);
        mEffectListExplosionSmokePuffs.Add(aSmoke);
    }
    
    aAngleOffset = gRand.Rot();
    for (float aRot=0.0f;aRot<360.0f;aRot += 30.0f) {
        EffectExplosionSmoke *aSmoke = new EffectExplosionSmoke();
        aSmoke->mX = pX;
        aSmoke->mY = pY;
        aSmoke->ConfigureYellow();
        aSmoke->Fling(aRot + aAngleOffset, gRand.GetFloat(4.0f, 8.0f) * gSpriteDrawScale, gRand.GetFloat(0.125f, 0.45f) * gSpriteDrawScale);
        mEffectListExplosionSmokePuffs.Add(aSmoke);
    }
}

void GameOverlayEffects::SpawnLifeBubble(float pX, float pY, bool pPlus, int pNumber) {
    
    if (gGame == NULL) { return; }
    
    FloatingLifeBubble *aBubble = new FloatingLifeBubble();
    aBubble->SetUp(pPlus, pNumber);
    PlaceFloatingText(aBubble, pX, pY);
}

void GameOverlayEffects::SpawnComboBubble(float pX, float pY, const char *pTitle, const char *pText) {
    
    if (gGame == NULL) { return; }
    
    FloatingComboBubble *aBubble = new FloatingComboBubble();
    
    aBubble->SetUp(pTitle, pText);
    
    PlaceFloatingText(aBubble, pX, pY);
}

// Assume gated by (game != null)
void GameOverlayEffects::PlaceFloatingText(FloatingTextBubble *pBubble, float pX, float pY) {
 
    if (pBubble == NULL) { return; }
    
    float aEdgePadding = 4.0f * gSpriteDrawScale;
    float aBottomPadding = 44.0f * gSpriteDrawScale;
    float aTopPadding = 4.0f * gSpriteDrawScale;
    
    float aBubbleWidth = pBubble->GetWidth();
    float aBubbleHeight = pBubble->GetHeight();
    
    float aWorldLeft = mGameCorner1X + (aEdgePadding + aBubbleWidth * 0.5f);
    float aWorldRight = mGameCorner2X - (aEdgePadding + aBubbleWidth * 0.5f);
    float aWorldBottom = mGameCorner3Y - (aBottomPadding + aBubbleHeight * 0.5f);
    float aWorldTop = mGameCorner1Y + (aTopPadding + aBubbleHeight * 0.5f);
    
    float aX = pX;
    float aY = pY;
    
    if (aX < aWorldLeft) { aX = aWorldLeft; }
    if (aX > aWorldRight) { aX = aWorldRight; }
    if (aY < aWorldTop) { aY = aWorldTop; }
    if (aY > aWorldBottom) { aY = aWorldBottom; }
    
    AttemptToPlaceBubble(aX, aY, aBubbleWidth, aBubbleHeight, &mTextBubbleExploreList);
    
    pBubble->mX = aX;
    pBubble->mY = aY;
    
    pBubble->mHitRectX = aX - (aBubbleWidth * 0.5f);
    pBubble->mHitRectY = aY - (aBubbleHeight * 0.5f);
    pBubble->mHitRectWidth = aBubbleWidth;
    pBubble->mHitRectHeight = aBubbleHeight;
    
    mFloatingTextBubbleList.Add(pBubble);
    
}

void GameOverlayEffects::AttemptToPlaceBubble(float &pX, float &pY, float pWidth, float pHeight, FPointList *pExploreList) {
    
    if (pExploreList == NULL) { return; }
    
    float aEdgePadding = 4.0f * gSpriteDrawScale;
    float aBottomPadding = 44.0f * gSpriteDrawScale;
    float aTopPadding = 4.0f * gSpriteDrawScale;
    
    float aWorldLeft = mGameCorner1X + (aEdgePadding);
    float aWorldRight = mGameCorner2X - (aEdgePadding);
    float aWorldBottom = mGameCorner3Y - (aBottomPadding);
    float aWorldTop = mGameCorner1Y + (aTopPadding);
    
    float aBubbleX = pX;
    float aBubbleY = pY;
    
    int aCount = pExploreList->mCount;
    float *aArrX = pExploreList->mX;
    float *aArrY = pExploreList->mY;
    
    FloatingTextBubble *aCompareBubble = NULL;
    bool aBlocked = true;
    bool aPlaced = false;
    
    float aWidth2 = pWidth * 0.5f;
    float aHeight2 = pHeight * 0.5f;
    
    float aLeft = 0.0f, aRight = 0.0f, aTop = 0.0f, aBottom = 0.0f;
    
    FRect aNewBubbleRect;
    float aHitBubbleWidth = 0.0f, aHitBubbleHeight = 0.0f;
    for (int n=0;n<mFloatingTextBubbleList.mObjectList.mCount;n++) {
        aCompareBubble = (FloatingTextBubble *)mFloatingTextBubbleList.mObjectList.mData[n];
        aHitBubbleWidth = aCompareBubble->GetWidth();
        aHitBubbleHeight = aCompareBubble->GetHeight();
        aCompareBubble->mHitRectX = aCompareBubble->mX - (aHitBubbleWidth * 0.5f);
        aCompareBubble->mHitRectY = aCompareBubble->mY - (aHitBubbleHeight * 0.5f);
        aCompareBubble->mHitRectWidth = aHitBubbleWidth;
        aCompareBubble->mHitRectHeight = aHitBubbleHeight;
    }
    
    for (int i=0;(i<aCount) && (aPlaced == false);i++) {
        
        aBlocked = false;
        
        float aX = aArrX[i] * gSpriteDrawScale + aBubbleX;
        float aY = aArrY[i] * gSpriteDrawScale + aBubbleY;
        
        aLeft = aX - aWidth2;
        aRight = aX + aWidth2;
        aTop = aY - aHeight2;
        aBottom = aY + aHeight2;
        
        if ((aLeft < aWorldLeft) || (aTop < aWorldTop) || (aRight > aWorldRight) || (aBottom > aWorldBottom)) {
            aBlocked = true;
        }
        
        aNewBubbleRect.mX = aLeft;
        aNewBubbleRect.mY = aTop;
        aNewBubbleRect.mWidth = pWidth;
        aNewBubbleRect.mHeight = pHeight;
        
        for (int n=0;(n<mFloatingTextBubbleList.mObjectList.mCount) && (aBlocked == false);n++) {
            aCompareBubble = (FloatingTextBubble *)mFloatingTextBubbleList.mObjectList.mData[n];
            if (aCompareBubble->ShouldBlock() == true) {
                if (aNewBubbleRect.Intersects(aCompareBubble->mHitRectX,
                                              aCompareBubble->mHitRectY,
                                              aCompareBubble->mHitRectWidth,
                                              aCompareBubble->mHitRectHeight)) {
                    aBlocked = true;
                }
            }
        }
        
        if (aBlocked == false) {
            aPlaced = true;
            pX = aX;
            pY = aY;
        }
    }
}

void GameOverlayEffects::BuildTextBubbleExploreGrid() {
    
    int aGridSize = ((TEXT_BUBBLE_EXPLORE_GRID_RADIUS - 1) + (TEXT_BUBBLE_EXPLORE_GRID_RADIUS - 1) + 1);
    
    float aExploreGridX[aGridSize][aGridSize];
    float aExploreGridY[aGridSize][aGridSize];
    float aExploreEuropeanDistance[aGridSize][aGridSize];
    
    int aGridOffset = (TEXT_BUBBLE_EXPLORE_GRID_RADIUS - 1);
    
    float aX = 0.0f;
    float aY = 0.0f;
    float aDist = 0.0f;
    int aListCount = 0;
    
    float aHopSize = TEXT_BUBBLE_EXPLORE_GRID_STEP_SIZE * gSpriteDrawScale;
    for (int i=(-(TEXT_BUBBLE_EXPLORE_GRID_RADIUS - 1));i<TEXT_BUBBLE_EXPLORE_GRID_RADIUS;i++) {
        aX = ((float)i) * aHopSize;
        for (int n=(-(TEXT_BUBBLE_EXPLORE_GRID_RADIUS - 1));n<TEXT_BUBBLE_EXPLORE_GRID_RADIUS;n++) {
            aY = ((float)n) * aHopSize;
            aExploreGridX[i + aGridOffset][n + aGridOffset] = aX;
            aExploreGridY[i + aGridOffset][n + aGridOffset] = aY;
            aDist = aX * aX + aY * aY;
            aExploreEuropeanDistance[i + aGridOffset][n + aGridOffset] = aDist;
            
            aListCount += 1;
        }
    }
    
    float aListX[aListCount];
    float aListY[aListCount];
    float aListDist[aListCount];
    int aIndex = 0;
    
    for (int i=0;i<aGridSize;i++) {
        for (int n=0;n<aGridSize;n++) {
            aListX[aIndex] = aExploreGridX[i][n];
            aListY[aIndex] = aExploreGridY[i][n];
            aListDist[aIndex] = aExploreEuropeanDistance[i][n];
            
            aIndex++;
        }
    }
    
    float aHoldX = 0.0f;
    float aHoldY = 0.0f;
    float aHoldDist = 0.0f;
    
    int i = 0;
    int j = 0;
    
    for (i=1;i<aListCount;i++) {
        aHoldDist = aListDist[i];
        aHoldX = aListX[i];
        aHoldY = aListY[i];
        
        j = i;
        while ((j>0) && ((aListDist[j-1] > aHoldDist))) {
            aListDist[j] = aListDist[j - 1];
            aListX[j] = aListX[j - 1];
            aListY[j] = aListY[j - 1];
            
            j--;
        }
        aListDist[j] = aHoldDist;
        aListX[j] = aHoldX;
        aListY[j] = aHoldY;
    }
    
    mTextBubbleExploreList.RemoveAll();
    for (int i=0;i<aListCount;i++) {
        mTextBubbleExploreList.Add(aListX[i], aListY[i]);
    }
}

bool GameOverlayEffects::FindGameAreaEdgeToCenterCollisionPoint(float pX, float pY, float pLineX1, float pLineY1, float pLineX2, float pLineY2, float &pResultX, float &pResultY) {
    if (IsNearParallel(pX, pY, pLineX1, pLineY1, pLineX2, pLineY2) == false) {
        float aCollisionX = 0.0f;
        float aCollisionY = 0.0f;
        float aCenterX = mWidth2;
        float aCenterY = mHeight2;
        float aRayDirX = (aCenterX - pX);
        float aRayDirY = (aCenterY - pY);
        float aRayLength = aRayDirX * aRayDirX + aRayDirY * aRayDirY;
        if (aRayLength > SQRT_EPSILON) {
            aRayLength = sqrtf(aRayLength);
            aRayDirX /= aRayLength;
            aRayDirY /= aRayLength;
            float aCollisionDistance = 0.0f;
            if (FLine::SegmentRayIntersection(pLineX1, pLineY1, pLineX2, pLineY2, pX, pY, aRayDirX, aRayDirY, aRayLength, aCollisionX, aCollisionY, aCollisionDistance)) {
                pResultX = aCollisionX;
                pResultY = aCollisionY;
                return true;
            }
        }
    }
    return false;
}

bool GameOverlayEffects::IsNearParallel(float pX, float pY, float pLineX1, float pLineY1, float pLineX2, float pLineY2) {
    float aEpsilon = 3.0f;
    if (fabsf(pLineX2 - pLineX1) > fabsf(pLineY2 - pLineY1)) {
        //We are horizontal...
        if (fabsf(pY - pLineY1) < aEpsilon) {
            return true;
        }
    } else {
        //We are vertical...
        if (fabsf(pX - pLineX1) < aEpsilon) {
            return true;
        }
    }
    return false;
}

