// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Precompiled.h"
#include "Script/LuaVM.h"
#include "Game/Entity.h"
#include "Game/GameWorld.h"

BE_NAMESPACE_BEGIN

void LuaVM::RegisterGameWorld(LuaCpp::Module &module) {
    LuaCpp::Selector _GameWorld = module["GameWorld"];

    _GameWorld.SetClass<GameWorld>(module["Object"]);
    _GameWorld.AddClassMembers<GameWorld>(
        "time", &GameWorld::GetTime,
        "prev_time", &GameWorld::GetPrevTime,
        "delta_time", &GameWorld::GetDeltaTime,
        "time_scale", &GameWorld::GetTimeScale,
        "set_time_scale", &GameWorld::SetTimeScale,
        "find_entity", &GameWorld::FindEntity,
        "find_entity_by_guid", &GameWorld::FindEntityByGuid,
        "find_entity_by_tag", &GameWorld::FindEntityByTag,
        "find_entities_by_tag", &GameWorld::FindEntitiesByTag,
        "clone_entity", &GameWorld::CloneEntity,
        "loded_map_name", &GameWorld::MapName,
        "restart_game", &GameWorld::RestartGame);

    _GameWorld["meta_object"] = GameWorld::metaObject;
}

BE_NAMESPACE_END
