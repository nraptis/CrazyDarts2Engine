//
//  LevelWaveLinkBlueprint.cpp
//  Crazy Darts 2 Mac
//
//  Created by Nick Raptis on 3/8/20.
//  Copyright © 2020 Froggy Studios. All rights reserved.
//

#include "GFXApp.hpp"
#include "LevelWaveLinkBlueprint.hpp"

LevelWaveLinkBlueprint::LevelWaveLinkBlueprint() {
    mLink_1_1 = 0;
    mLink_1_2 = 0;

    mLink_2_1 = 0;
    mLink_2_2 = 0;

    mLink_3_1 = 0;
    mLink_3_2 = 0;

    mLink_4_1 = 0;
    mLink_4_2 = 0;
}

LevelWaveLinkBlueprint::~LevelWaveLinkBlueprint() {
    
}

void LevelWaveLinkBlueprint::Reset() {
    mLink_1_1 = 0;
    mLink_1_2 = 0;

    mLink_2_1 = 0;
    mLink_2_2 = 0;

    mLink_3_1 = 0;
    mLink_3_2 = 0;

    mLink_4_1 = 0;
    mLink_4_2 = 0;
}

bool LevelWaveLinkBlueprint::ShouldSave() {
    
    if (mLink_1_1 != mLink_1_2) { return true; }
    if (mLink_2_1 != mLink_2_2) { return true; }
    if (mLink_3_1 != mLink_3_2) { return true; }
    if (mLink_4_1 != mLink_4_2) { return true; }
    
    return false;
}

FJSONNode *LevelWaveLinkBlueprint::Save() {
    FJSONNode *aResult = new FJSONNode();
    aResult->mNodeType = JSON_NODE_TYPE_ARRAY;
    
    if (mLink_1_1 != mLink_1_2) {
        FJSONNode *aLinkNode = new FJSONNode();
        aLinkNode->AddDictionaryInt("link_1", mLink_1_1);
        aLinkNode->AddDictionaryInt("link_2", mLink_1_2);
        aResult->AddArray(aLinkNode);
    }
    
    if (mLink_2_1 != mLink_2_2) {
        FJSONNode *aLinkNode = new FJSONNode();
        aLinkNode->AddDictionaryInt("link_1", mLink_2_1);
        aLinkNode->AddDictionaryInt("link_2", mLink_2_2);
        aResult->AddArray(aLinkNode);
    }
    
    if (mLink_3_1 != mLink_3_2) {
        FJSONNode *aLinkNode = new FJSONNode();
        aLinkNode->AddDictionaryInt("link_1", mLink_3_1);
        aLinkNode->AddDictionaryInt("link_2", mLink_3_2);
        aResult->AddArray(aLinkNode);
    }
    
    if (mLink_4_1 != mLink_4_2) {
        FJSONNode *aLinkNode = new FJSONNode();
        aLinkNode->AddDictionaryInt("link_1", mLink_4_1);
        aLinkNode->AddDictionaryInt("link_2", mLink_4_2);
        aResult->AddArray(aLinkNode);
    }
    
    return aResult;
}

void LevelWaveLinkBlueprint::Load(FJSONNode *pNode) {
    
    if (pNode == NULL) { return; }
    
    int aIndex = 0;
    EnumJSONArray(pNode, aLinkNode) {
        if (aIndex == 0) {
            mLink_1_1 = aLinkNode->GetInt("link_1", 0);
            mLink_1_2 = aLinkNode->GetInt("link_2", 0);
        }
        if (aIndex == 1) {
            mLink_2_1 = aLinkNode->GetInt("link_1", 0);
            mLink_2_2 = aLinkNode->GetInt("link_2", 0);
        }
        if (aIndex == 2) {
            mLink_3_1 = aLinkNode->GetInt("link_1", 0);
            mLink_3_2 = aLinkNode->GetInt("link_2", 0);
        }
        if (aIndex == 3) {
            mLink_4_1 = aLinkNode->GetInt("link_1", 0);
            mLink_4_2 = aLinkNode->GetInt("link_2", 0);
        }
        aIndex++;
    }
}
