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

#include "aabbnode.h"

namespace reone {

namespace render {

AABBSceneNode::AABBSceneNode(const AABB &aabb, const glm::mat4 &transform) : SceneNode(transform), _aabb(aabb) {
}

const AABB &AABBSceneNode::aabb() const {
    return _aabb;
}

} // namespace render

} // namespace reone