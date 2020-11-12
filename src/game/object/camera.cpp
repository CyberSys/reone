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

#include "camera.h"

#include "glm/gtx/euler_angles.hpp"

using namespace std;

using namespace reone::resource;
using namespace reone::scene;

namespace reone {

namespace game {

CameraObject::CameraObject(uint32_t id, SceneGraph *sceneGraph) : SpatialObject(id, ObjectType::Camera, sceneGraph) {
}

void CameraObject::load(const GffStruct &gffs) {
    _cameraId = gffs.getInt("CameraID");
    _fieldOfView = gffs.getFloat("FieldOfView");

    Vector3 position(gffs.getVector("Position"));
    float height = gffs.getFloat("Height");

    _position = glm::vec3(position.x, position.y, position.z + height);

    Quaternion orientation(gffs.getOrientation("Orientation"));
    float pitch = gffs.getFloat("Pitch");

    _orientation = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
    _orientation *= glm::quat_cast(glm::eulerAngleX(glm::radians(pitch)));

    updateTransform();
}

int CameraObject::cameraId() const {
    return _cameraId;
}

float CameraObject::fieldOfView() const {
    return _fieldOfView;
}

} // namespace game

} // namespace reone
