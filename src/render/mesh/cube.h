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

#include "mesh.h"

namespace reone {

namespace render {

class CubeMesh : public Mesh {
public:
    static CubeMesh &instance();

    void render(const glm::mat4 &transform) const;

private:
    CubeMesh();
};

#define TheCubeMesh render::CubeMesh::instance()

} // namespace render

} // namespace reone