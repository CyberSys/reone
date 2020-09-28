/*
 * Copyright � 2020 Vsevolod Kremianskii
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

#include <memory>
#include <vector>

#include "object.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include "../../render/scene/modelnode.h"
#include "../../render/walkmesh.h"

namespace reone {

namespace game {

static const float kDefaultDrawDistance = 1024.0f;
static const float kDefaultFadeDistance = 256.0f;

class Item;
class Room;

class SpatialObject : public Object {
public:
    void update(const UpdateContext &ctx) override;

    float distanceTo(const glm::vec2 &point) const;
    float distanceTo(const glm::vec3 &point) const;
    bool contains(const glm::vec3 &point) const;

    void face(const SpatialObject &other);
    void moveItemsTo(SpatialObject &other);

    bool isSelectable() const;

    Room *room() const;
    const glm::vec3 &position() const;
    float heading() const;
    const glm::mat4 &transform() const;
    std::shared_ptr<render::ModelSceneNode> model() const;
    std::shared_ptr<render::Walkmesh> walkmesh() const;
    const std::vector<std::shared_ptr<Item>> &items() const;

    void setRoom(Room *room);
    void setPosition(const glm::vec3 &position);
    void setHeading(float heading);
    void setVisible(bool visible);

    // Animation

    virtual void animate(const std::string &anim, int flags = 0, float speed = 1.0f);
    virtual void animate(const std::string &parent, const std::string &anim, int flags = 0, float speed = 1.0f);

    // END Animation

protected:
    render::SceneGraph *_sceneGraph { nullptr };
    glm::vec3 _position { 0.0f };
    float _heading { 0.0f };
    glm::mat4 _transform { 1.0f };
    std::shared_ptr<render::ModelSceneNode> _model;
    std::shared_ptr<render::Walkmesh> _walkmesh;
    float _drawDistance { kDefaultDrawDistance };
    float _fadeDistance { kDefaultFadeDistance };
    Room *_room { nullptr };
    std::vector<std::shared_ptr<Item>> _items;
    bool _selectable { false };

    SpatialObject(uint32_t id, ObjectType type, render::SceneGraph *sceneGraph);

    virtual void updateTransform();
};

} // namespace game

} // namespace reone
