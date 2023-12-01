/*
 * Copyright (c) 2020-2023 The reone project contributors
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

#include "shader.h"
#include "shaderprogram.h"

namespace reone {

namespace graphics {

struct GraphicsOptions;

enum class ShaderProgramId {
    None,

    SimpleColor,
    SimpleTexture,
    GUI,
    Text,
    Points,

    PointLightShadows,
    DirectionalLightShadows,
    ModelOpaque,
    ModelTransparent,
    AABB,
    Walkmesh,
    Particle,
    Grass,
    Billboard,
    SSAO,
    SSR,
    CombineOpaque,
    CombineGeometry,

    BoxBlur4,
    GaussianBlur9,
    GaussianBlur13,
    MedianFilter3,
    MedianFilter5,
    FXAA,
    Sharpen
};

class IShaderManager {
public:
    virtual ~IShaderManager() = default;

    virtual void use(ShaderProgramId programId) = 0;
};

class ShaderManager : public IShaderManager, boost::noncopyable {
public:
    void associate(ShaderProgramId programId, std::shared_ptr<ShaderProgram> program) {
        _idToProgram[programId] = std::move(program);
    }

    void use(ShaderProgramId programId) override {
        if (_usedProgram == programId) {
            return;
        }
        if (programId != ShaderProgramId::None) {
            auto programIter = _idToProgram.find(programId);
            if (programIter == _idToProgram.end()) {
                throw std::runtime_error("Shader program not found by id: " + std::to_string(static_cast<int>(programId)));
            }
            programIter->second->use();
        }
        _usedProgram = programId;
    }

private:
    std::map<ShaderProgramId, std::shared_ptr<ShaderProgram>> _idToProgram;

    ShaderProgramId _usedProgram {ShaderProgramId::None};
};

} // namespace graphics

} // namespace reone
