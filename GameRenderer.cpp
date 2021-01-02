//
//  GameRenderer.cpp
//  Crazy Darts 2 iOS
//
//  Created by Nicholas Raptis on 2/26/19.
//  Copyright © 2019 Froggy Studios. All rights reserved.
//

#include "GameRenderer.hpp"
#include "Game.hpp"
#include "GFXApp.hpp"

#ifdef EDITOR_MODE
#include "GameEditor.hpp"
#endif

GameRenderer::GameRenderer(Game *pGame, FloatingCamera *pCamera) {
    mGame = pGame;
    mCamera = pCamera;
    //
    //mDart = &gApp->mDart;
    
    
    //FUniformsLightPhong                 mUniformPhong;
    //FUniformsLightDiffuse        mUniformDiffuse;
    //FUniformsLightAmbient               mUniformAmbient;
    
    
    //mAmbientIntensity = 0.000000f;
    //mDiffuseIntensity = 0.605346f;
    //mSpecularIntensity = 3.832547f;
    //mDirX = 0.502698f;
    //mDirY = -0.078952f;
    //mDirZ = -0.860849f;
    //mShininess = 40.251583f;
    
    
    
    
    mUniformDartBody.mLight.mAmbientIntensity = 0.3f;
    mUniformDartBody.mLight.mDiffuseIntensity = 0.7f;
    mUniformDartBody.mLight.mDirX = -0.399262f;
    mUniformDartBody.mLight.mDirY = 0.491924f;
    mUniformDartBody.mLight.mDirZ = -0.773693f;

    mUniformDartTip.mLight.mAmbientIntensity = 0.3f;
    mUniformDartTip.mLight.mDiffuseIntensity = 0.7f;
    mUniformDartTip.mLight.mDirX = -0.545275f;
    mUniformDartTip.mLight.mDirY = -0.399271f;
    mUniformDartTip.mLight.mDirZ = -0.737060f;
    
    mUniformDartGrip.mLight.mAmbientIntensity = 0.5f;
    mUniformDartGrip.mLight.mDiffuseIntensity = 0.5f;
    mUniformDartGrip.mLight.mSpecularIntensity = 1.25f;
    mUniformDartGrip.mLight.mDirX = -0.323847f;
    mUniformDartGrip.mLight.mDirY = -0.911071f;
    mUniformDartGrip.mLight.mDirZ = -0.255094f;
    mUniformDartGrip.mLight.mShininess = 27.0f;;
    
    mUniformBalloon.mLight.mAmbientIntensity = 0.30f;
    mUniformBalloon.mLight.mDiffuseIntensity = 0.70f;
    mUniformBalloon.mLight.mSpecularIntensity = 3.2;
    mUniformBalloon.mLight.mDirX = -0.518735f;
    mUniformBalloon.mLight.mDirY = 0.619300f;
    mUniformBalloon.mLight.mDirZ = -0.589391f;
    mUniformBalloon.mLight.mShininess = 60.0f;
    
    mUniformDartBat.mLight.mAmbientIntensity = 0.36f;
    mUniformDartBat.mLight.mDiffuseIntensity = 0.64f;
    mUniformDartBat.mLight.mSpecularIntensity = 3.5f;
    mUniformDartBat.mLight.mDirX = -0.583237f;
    mUniformDartBat.mLight.mDirY = -0.462011f;
    mUniformDartBat.mLight.mDirZ = -0.668117f;
    mUniformDartBat.mLight.mShininess = 45.0f;
    
    mUniformBrickHeadCage.mLight.mAmbientIntensity = 0.3f;
    mUniformBrickHeadCage.mLight.mDiffuseIntensity = 0.7f;
    mUniformBrickHeadCage.mLight.mSpecularIntensity = 0.75f;
    mUniformBrickHeadCage.mLight.mDirX = -0.431157f;
    mUniformBrickHeadCage.mLight.mDirY = 0.693582f;
    mUniformBrickHeadCage.mLight.mDirZ = -0.577102f;
    mUniformBrickHeadCage.mLight.mShininess = 24.0f;
    
    mUniformBrickHeadSpikes.mLight.mAmbientIntensity = 0.303011f;
    mUniformBrickHeadSpikes.mLight.mDiffuseIntensity = 0.700736f;
    mUniformBrickHeadSpikes.mLight.mDirX = -0.387749f;
    mUniformBrickHeadSpikes.mLight.mDirY = 0.807075f;
    mUniformBrickHeadSpikes.mLight.mDirZ = -0.445287f;
    
    mUniformBrickHead.mLight.mAmbientIntensity = 0.320792f;
    mUniformBrickHead.mLight.mDiffuseIntensity = 0.685995f;
    mUniformBrickHead.mLight.mDirX = -0.159402f;
    mUniformBrickHead.mLight.mDirY = 0.389269f;
    mUniformBrickHead.mLight.mDirZ = -0.907227f;
    
    mUniformTurtleBody.mLight.mAmbientIntensity = 0.3f;
    mUniformTurtleBody.mLight.mDiffuseIntensity = 0.7f;
    mUniformTurtleBody.mLight.mDirX = -0.759005f;
    mUniformTurtleBody.mLight.mDirY = 0.372714f;
    mUniformTurtleBody.mLight.mDirZ = -0.533850f;
    
    mUniformTurtleRims.mLight.mAmbientIntensity = 0.3f;
    mUniformTurtleRims.mLight.mDiffuseIntensity = 0.7f;
    mUniformTurtleRims.mLight.mSpecularIntensity = 0.55f;
    mUniformTurtleRims.mLight.mDirX = -0.632086f;
    mUniformTurtleRims.mLight.mDirY = 0.486674f;
    mUniformTurtleRims.mLight.mDirZ = -0.603006f;
    mUniformTurtleRims.mLight.mShininess = 4.0f;
    
    mUniformCannonBody.mLight.mAmbientIntensity = 0.425f;
    mUniformCannonBody.mLight.mDiffuseIntensity = 0.575f;
    mUniformCannonBody.mLight.mDirX = -0.430209f;
    mUniformCannonBody.mLight.mDirY = 0.774111f;
    mUniformCannonBody.mLight.mDirZ = -0.464405f;
    
    mUniformCannonLip.mLight.mAmbientIntensity = 0.3f;
    mUniformCannonLip.mLight.mDiffuseIntensity = 0.7f;
    mUniformCannonLip.mLight.mSpecularIntensity = 2.65f;
    mUniformCannonLip.mLight.mDirX = -0.430209f;
    mUniformCannonLip.mLight.mDirY = 0.774111f;
    mUniformCannonLip.mLight.mDirZ = -0.464405f;
    mUniformCannonLip.mLight.mShininess = 35.0f;
    
    
    mUniformBlimpBody.mLight.mAmbientIntensity = 0.60f;
    mUniformBlimpBody.mLight.mDiffuseIntensity = 0.40f;
    mUniformBlimpBody.mLight.mDirX = 0.842381f;
    mUniformBlimpBody.mLight.mDirY = -0.380257f;
    mUniformBlimpBody.mLight.mDirZ = -0.381836f;
    
    
    mUniformBlimpTail.mLight.mAmbientIntensity = 0.4f;
    mUniformBlimpTail.mLight.mDiffuseIntensity = 0.6f;
    mUniformBlimpTail.mLight.mSpecularIntensity = 1.65f;
    mUniformBlimpTail.mLight.mDirX = -0.412534f;
    mUniformBlimpTail.mLight.mDirY = 0.515425f;
    mUniformBlimpTail.mLight.mDirZ = -0.751101f;
    mUniformBlimpTail.mLight.mShininess = 12.197318f;
    
    
    mUniformStarFaces.mLight.mAmbientIntensity = 0.40f;
    mUniformStarFaces.mLight.mDiffuseIntensity = 0.60f;
    mUniformStarFaces.mLight.mDirX = -0.452561f;
    mUniformStarFaces.mLight.mDirY = 0.602681f;
    mUniformStarFaces.mLight.mDirZ = -0.657240f;
    
    
    mUniformStarRims.mLight.mAmbientIntensity = 0.3f;
    mUniformStarRims.mLight.mDiffuseIntensity = 0.7f;
    mUniformStarRims.mLight.mSpecularIntensity = 1.65f;
    mUniformStarRims.mLight.mDirX = -0.318566f;
    mUniformStarRims.mLight.mDirY = 0.837150f;
    mUniformStarRims.mLight.mDirZ = -0.444631f;
    mUniformStarRims.mLight.mShininess = 28.0f;
    
    
    /*
    
    */
    
    
    
    
    
    //Turtle Body
    /*
    mRed = 1.000000f;
    mGreen = 1.000000f;
    mBlue = 1.000000f;
    
    mShininess = 0.000000f;
    */
    
    
    //Turtle Rims
    /*
    mRed = 1.000000f;
    mGreen = 1.000000f;
    mBlue = 1.000000f;
    
    */
    
    
}

GameRenderer::~GameRenderer() {
    
}

void GameRenderer::Update() {
    
    mSky.Update();
}

void GameRenderer::Draw3D() {
    
    FMatrix aProjection = mCamera->GetProjection();
    FMatrix aProjectionRumbleDamped = mCamera->GetProjectionRumbleDamped();
    
    //mUniformPhong.mProjection.Set(aProjection);
    
    mUniformBalloon.mProjection.Set(aProjection);
    mUniformDartBody.mProjection.Set(aProjection);
    mUniformDartTip.mProjection.Set(aProjection);
    mUniformDartGrip.mProjection.Set(aProjection);
    mUniformDartBat.mProjection.Set(aProjection);
    mUniformBrickHead.mProjection.Set(aProjection);
    mUniformBrickHeadCage.mProjection.Set(aProjection);
    mUniformBrickHeadSpikes.mProjection.Set(aProjection);
    mUniformTurtleBody.mProjection.Set(aProjection);
    mUniformTurtleRims.mProjection.Set(aProjection);
    mUniformCannonBody.mProjection.Set(aProjection);
    mUniformCannonLip.mProjection.Set(aProjection);
    mUniformBlimpTail.mProjection.Set(aProjection);
    mUniformBlimpBody.mProjection.Set(aProjection);
    
    
    Graphics::MatrixProjectionSet(aProjectionRumbleDamped);
    
    Graphics::MatrixModelViewReset();
    
    Graphics::SetColor();
    Graphics::DepthDisable();
    Graphics::CullFacesSetBack();
    Graphics::PipelineStateSetShapeNodeNoBlending();
    
#ifdef EDITOR_MODE
    
    if (gEditor == NULL) {
        Graphics::PipelineStateSetShapeNodeAlphaBlending();
        mSky.Draw();
        
        Graphics::SetColor(0.32f);
        Graphics::PipelineStateSetSimpleModelAlphaBlending();
        mSky.DrawClouds();
    }
    
#else
    
    mSky.Draw();
    
    Graphics::SetColor(0.32f);
    Graphics::PipelineStateSetSimpleModelAlphaBlending();
    mSky.DrawClouds();
    
#endif
    
    Graphics::MatrixProjectionSet(aProjection);
    
    
    Graphics::SetColor();
    Graphics::CullFacesSetDisabled();
    Graphics::DepthEnable();
    //Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    
    //mUniformPhong.mModelView.Set(aModelView);
    //Graphics::UniformBind(&mUniformPhong);
    
    
    //Graphics::PipelineStateSetShape3DAlphaBlending();
    
    //Graphics::MatrixProjectionSet(aProjection);
    //Graphics::MatrixModelViewReset();
    
    //Graphics::DepthDisable();
    //Graphics::CullFacesSetFront();
    Graphics::PipelineStateSetShapeNodeNoBlending();
    Graphics::SetColor();
    
    EnumList (Balloon, aBalloon, mGame->mBalloonList.mObjectList) {
        if (aBalloon->mKill == 0) {
            aBalloon->Draw3DThread();
        }
    }
    EnumList (FreeLife, aFreeLife, mGame->mFreeLifeList.mObjectList) {
        if (aFreeLife->mKill == 0) {
            aFreeLife->Draw3DThread();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Dart, aDart, mGame->mDartList.mObjectList) {
        if ((aDart->mKill == 0) && (aDart->mIsHidden == false)) {
            aDart->Draw3D();
            aDart->Draw3DTip();
        }
    }
    
    if (mGame->mCurrentDart != NULL) {
        mGame->mCurrentDart->Draw3D();
        mGame->mCurrentDart->Draw3DTip();
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (Dart, aDart, mGame->mDartList.mObjectList) {
        if ((aDart->mKill == 0) && (aDart->mIsHidden == false)) {
            aDart->Draw3DGrip();
        }
    }
    if (mGame->mCurrentDart != NULL) {
        mGame->mCurrentDart->Draw3DGrip();
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    Graphics::PipelineStateSetModelIndexedLightedPhongOverlayNoBlending();
    EnumList (Balloon, aBalloon, mGame->mBalloonList.mObjectList) {
        if (aBalloon->mKill == 0) {
            aBalloon->Draw3D();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (FreeLife, aFreeLife, mGame->mFreeLifeList.mObjectList) {
        if (aFreeLife->mKill == 0) {
            aFreeLife->Draw3D();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (BrickHead, aBrickHead, mGame->mBrickHeadList.mObjectList) {
        if (aBrickHead->mKill == 0) {
            aBrickHead->Draw3D();
            aBrickHead->Draw3DSpikes();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (BrickHead, aBrickHead, mGame->mBrickHeadList.mObjectList) {
        if (aBrickHead->mKill == 0) {
            aBrickHead->Draw3DCage();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Bomb, aBomb, mGame->mBombList.mObjectList) {
        if (aBomb->mKill == 0) {
            aBomb->Draw3D();
        }
    }
    
    EnumList (Bomb, aBomb, mGame->mBombList.mObjectList) {
        if (aBomb->mKill == 0) {
            aBomb->Draw3DWick();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (Bomb, aBomb, mGame->mBombList.mObjectList) {
        if (aBomb->mKill == 0) {
            aBomb->Draw3DTop();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedAlphaBlending();
    Graphics::SetColor();
    EnumList (Bomb, aBomb, mGame->mBombList.mObjectList) {
        if (aBomb->mKill == 0) {
            aBomb->Draw3DSparkBillboard();
        }
    }
    
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Turtle, aTurtle, mGame->mTurtleList.mObjectList) {
        if (aTurtle->mKill == 0) {
            aTurtle->Draw3D();
            aTurtle->Draw3DPropeller();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (Turtle, aTurtle, mGame->mTurtleList.mObjectList) {
        if (aTurtle->mKill == 0) {
            aTurtle->Draw3DRimsAndSpikes();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedAlphaBlending();
    Graphics::SetColor(0.40f);
    EnumList (Turtle, aTurtle, mGame->mTurtleList.mObjectList) {
        if (aTurtle->mKill == 0) {
            aTurtle->Draw3DBillboard();
        }
    }
    Graphics::SetColor();
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Cannon, aCannon, mGame->mCannonList.mObjectList) {
        if (aCannon->mKill == 0) {
            aCannon->Draw3D();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (Cannon, aCannon, mGame->mCannonList.mObjectList) {
        if (aCannon->mKill == 0) {
            aCannon->Draw3DLip();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Star, aStar, mGame->mStarList.mObjectList) {
        if (aStar->mKill == 0) {
            aStar->Draw3D();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (Star, aStar, mGame->mStarList.mObjectList) {
        if (aStar->mKill == 0) {
            aStar->Draw3DRim();
        }
    }
    
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Blimp, aBlimp, mGame->mBlimpList.mObjectList) {
        if (aBlimp->mKill == 0) {
            aBlimp->Draw3D();
        }
    }
    if ((mGame->mGameOverBlimp != NULL) && (mGame->mGameOverBlimp->mKill == 0)) {
        mGame->mGameOverBlimp->Draw3D();
    }
    
    Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    EnumList (Blimp, aBlimp, mGame->mBlimpList.mObjectList) {
        if (aBlimp->mKill == 0) {
            aBlimp->Draw3DTail();
        }
    }
    if ((mGame->mGameOverBlimp != NULL) && (mGame->mGameOverBlimp->mKill == 0)) {
        mGame->mGameOverBlimp->Draw3DTail();
    }
    
    //Graphics::PipelineStateSetModelIndexedLightedPhongNoBlending();
    Graphics::PipelineStateSetModelIndexedLightedDiffuseNoBlending();
    EnumList (Platform, aPlatform, mGame->mPlatformList.mObjectList) {
        if (aPlatform->mKill == 0) {
            aPlatform->Draw3D();
        }
    }
    
    
    
#ifdef EDITOR_MODE
    
    if (gEditor != NULL) {
        if (gEditor->mEditorShowReferenced) {
            
            
            //LevelWave *aWave = gEditor->mSection.mCurrentWave;
            LevelWave *aWave = ((LevelWave *)gEditor->mEditorSection.mWaveList.Fetch(gEditor->WaveIndex()));
            
            if (aWave != NULL) {
                for (int i=0;i<aWave->mPath.mNodeList.mCount;i++) {
                    GameObject *aObject = (GameObject *)gEditor->mEditorObjectList.Fetch(i);
                    if (aObject != NULL) {
                        aObject->Draw3D();
                    }
                }
            }
            
            
            
        }
    }
#endif
    
    Graphics::DepthDisable();
    Graphics::CullFacesSetBack();
}


void GameRenderer::ScreenBoundaryRefresh() {
    
    if (mGame == NULL) { return; }
    if (gGame != mGame) { return; }
    
    float aGameContainerBorderTop = mGame->mGameContainerBorderTop;
    float aGameContainerBorderRight = mGame->mGameContainerBorderRight;
    float aGameContainerBorderBottom = mGame->mGameContainerBorderBottom;
    float aGameContainerBorderLeft = mGame->mGameContainerBorderLeft;
    
    //aGameAreaTop = mGame->Convert2DYTo3D(aGameAreaTop);
    //aGameAreaRight = mGame->Convert2DXTo3D(aGameAreaRight);
    //aGameAreaBottom = mGame->Convert2DYTo3D(aGameAreaBottom);
    //aGameAreaLeft = mGame->Convert2DXTo3D(aGameAreaLeft);
    
    float aWidth = (aGameContainerBorderRight - aGameContainerBorderLeft);
    float aHeight = (aGameContainerBorderBottom - aGameContainerBorderTop);
    
    if (aWidth < 0.25f || aHeight < 0.25f) {
        
        Log("Play Area Too SMALL [%.2f %.2f %.2f %.2f] (%.2f x %.2f)", aGameContainerBorderTop, aGameContainerBorderRight, aGameContainerBorderBottom, aGameContainerBorderLeft, aWidth, aHeight);
        return;
    }
    
    float aPadding = 0.1f;
    mSky.Refresh(aGameContainerBorderTop, aGameContainerBorderRight, aGameContainerBorderBottom, aGameContainerBorderLeft, aPadding);
    
    
    
    
}
