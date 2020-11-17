/*
 * Copyright (c) 2020 Vsevolod Kremianskii
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <atomic>
#include <queue>

#include "../../resource/2dafile.h"
#include "../../resource/gfffile.h"
#include "../../script/types.h"
#include "../effect.h"

#include "../rp/attributes.h"

#include "item.h"
#include "spatial.h"

namespace reone {

namespace game {

class CreatureBlueprint;
class ObjectFactory;

enum class CombatState {
    Idle,
    Attack,
    Defense,
    Cooldown,
    Staggered
};

// TODO: Factions from KOTOR 2
enum class Faction {
    INVALID_STANDARD_FACTION = -1,
    STANDARD_FACTION_HOSTILE_1 = 1,
    STANDARD_FACTION_FRIENDLY_1 = 2,
    STANDARD_FACTION_HOSTILE_2 = 3,
    STANDARD_FACTION_FRIENDLY_2 = 4,
    STANDARD_FACTION_NEUTRAL = 5,
    STANDARD_FACTION_INSANE = 6,
    STANDARD_FACTION_PTAT_TUSKAN = 7,
    STANDARD_FACTION_GLB_XOR = 8,
    STANDARD_FACTION_SURRENDER_1 = 9,
    STANDARD_FACTION_SURRENDER_2 = 10,
    STANDARD_FACTION_PREDATOR = 11,
    STANDARD_FACTION_PREY = 12,
    STANDARD_FACTION_TRAP = 13,
    STANDARD_FACTION_ENDAR_SPIRE = 14,
    STANDARD_FACTION_RANCOR = 15,
    STANDARD_FACTION_GIZKA_1 = 16,
    STANDARD_FACTION_GIZKA_2 = 17
};

class Creature : public SpatialObject {
public:
    enum class MovementType {
        None,
        Walk,
        Run
    };

    enum class Animation {
        UnlockDoor
    };

    struct Path {
        glm::vec3 destination { 0.0f };
        std::vector<glm::vec3> points;
        uint32_t timeFound { 0 };
        int pointIdx { 0 };
    };

    Creature(uint32_t id, ObjectFactory *objectFactory, scene::SceneGraph *sceneGraph);

    void update(float dt) override;
    void clearAllActions() override;

    glm::vec3 selectablePosition() const override;

    void load(const resource::GffStruct &gffs);
    void load(const std::shared_ptr<CreatureBlueprint> &blueprint);
    void load(const CreatureConfiguration &config);

    void playAnimation(Animation anim);
    void updateModelAnimation();

    Gender gender() const;
    int appearance() const;
    std::shared_ptr<render::Texture> portrait() const;
    float walkSpeed() const;
    float runSpeed() const;
    const CreatureAttributes &attributes() const;
    std::shared_ptr<CreatureBlueprint> blueprint() const;

    void setTag(const std::string &tag);
    void setMovementType(MovementType type);
    void setTalking(bool talking);

    // Equipment

    void equip(const std::string &resRef);
    void equip(InventorySlot slot, const std::shared_ptr<Item> &item);
    void unequip(const std::shared_ptr<Item> &item);

    std::shared_ptr<Item> getEquippedItem(InventorySlot slot) const;

    const std::map<InventorySlot, std::shared_ptr<Item>> &equipment() const;

    // END Equipment

    // Pathfinding

    std::shared_ptr<Path> &path();

    void setPath(const glm::vec3 &dest, std::vector<glm::vec3> &&points, uint32_t timeFound);
    void clearPath();

    // END Pathfinding

    // Combat

    /* combat animation interruption */
    bool isInterrupted() { return !(_cbtState == CombatState::Idle || _cbtState == CombatState::Cooldown); }

    CombatState getCombatState() { return _cbtState; }
    void setCombatState(CombatState state) { _cbtState = state;  }

    Faction getFaction() const { return _factionId; }

    void setFaction(Faction faction) { _factionId = faction; }

    // const std::deque<std::unique_ptr<Effect>> &getActiveEffects() { return _activeEffects; }

    void applyEffect(std::unique_ptr<Effect> &&eff) {
        _activeEffects.push_back(std::move(eff)); 
    }

    // END Combat

private:
    enum class ModelType {
        Creature,
        Droid,
        Character
    };

    ObjectFactory *_objectFactory { nullptr };
    CreatureConfiguration _config;
    std::shared_ptr<CreatureBlueprint> _blueprint;
    ModelType _modelType { ModelType::Creature };
    std::shared_ptr<scene::ModelSceneNode> _headModel;
    std::shared_ptr<render::Texture> _portrait;
    std::map<InventorySlot, std::shared_ptr<Item>> _equipment;
    std::shared_ptr<Path> _path;
    float _walkSpeed { 0.0f };
    float _runSpeed { 0.0f };
    MovementType _movementType { MovementType::None };
    bool _talking { false };
    CreatureAttributes _attributes;
    bool _animDirty { true };
    bool _animFireForget { false };

    // Scripts

    std::string _onSpawn;

    // END Scripts

    // combat

    CombatState _cbtState = CombatState::Idle;
    std::deque<std::unique_ptr<Effect>> _activeEffects;
    Faction _factionId = Faction::INVALID_STANDARD_FACTION;

    // END combat

    // Loading

    void loadAppearance(const resource::TwoDaTable &table, int row);
    void loadPortrait(int appearance);

    // END Loading

    void updateModel();

    ModelType parseModelType(const std::string &s) const;

    std::string getBodyModelName() const;
    std::string getBodyTextureName() const;
    std::string getHeadModelName() const;
    std::string getWeaponModelName(InventorySlot slot) const;

    const std::string &getPauseAnimation() const;
    const std::string &getRunAnimation() const;
    const std::string &getWalkAnimation() const;
};

} // namespace game

} // namespace reone
