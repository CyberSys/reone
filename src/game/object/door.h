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

#include "spatial.h"

#include "../../resources/gfffile.h"

#include "../blueprint/door.h"

namespace reone {

namespace game {

class Door : public SpatialObject {
public:
    Door(uint32_t id, render::SceneGraph *sceneGraph);

    void load(const resources::GffStruct &gffs);
    virtual void open(const std::shared_ptr<Object> &triggerrer);
    void close(const std::shared_ptr<Object> &triggerrer);
    void saveTo(AreaState &state) const override;
    void loadState(const AreaState &state) override;

    bool isOpen() const;
    bool isStatic() const;
    const std::string &linkedToModule() const;
    const std::string &linkedTo() const;
    const std::string &transitionDestin() const;

private:
    std::shared_ptr<DoorBlueprint> _blueprint;
    bool _open { false };
    std::string _linkedToModule;
    std::string _linkedTo;
    std::string _transitionDestin;

    void loadBlueprint(const std::string &resRef);
};

} // namespace game

} // namespace reone
