//
//  Stage.cpp
//  hero
//
//  Created by xujianfeng on 13-8-30.
//  Copyright (c) 2013年 HYLD. All rights reserved.
//

#include "Stage.h"

Stage::Stage()
{
    chapter_id_ = stage_id_ = 0;
    depend_chapter_ = depend_stage_ = 0;
    reset_cost_ = 0;
    ssc_ = NULL;
}
Stage::~Stage()
{}
StageRecord::StageRecord()
{
    record_.clear();
}
StageRecord::~StageRecord()
{}
SpecialStageCondition::SpecialStageCondition()
{}
SpecialStageCondition::~SpecialStageCondition()
{}
