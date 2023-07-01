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

class IShaders {
public:
    virtual ~IShaders() = default;

    virtual void use(ShaderProgramId programId) = 0;
};

class Shaders : public IShaders, boost::noncopyable {
public:
    Shaders(GraphicsOptions &options) :
        _options(options) {
    }

    ~Shaders() { deinit(); }

    void init();
    void deinit();

    void use(ShaderProgramId programId) override;

private:
    GraphicsOptions &_options;

    bool _inited {false};

    ShaderProgramId _usedProgram {ShaderProgramId::None};
    std::map<std::string, std::string> _sourceFiles;

    // Shader Programs

    std::shared_ptr<ShaderProgram> _spSimpleColor;
    std::shared_ptr<ShaderProgram> _spSimpleTexture;
    std::shared_ptr<ShaderProgram> _spGUI;
    std::shared_ptr<ShaderProgram> _spText;
    std::shared_ptr<ShaderProgram> _spPoints;

    std::shared_ptr<ShaderProgram> _spPointLightShadows;
    std::shared_ptr<ShaderProgram> _spDirectionalLightShadows;
    std::shared_ptr<ShaderProgram> _spModelOpaque;
    std::shared_ptr<ShaderProgram> _spModelTransparent;
    std::shared_ptr<ShaderProgram> _spAABB;
    std::shared_ptr<ShaderProgram> _spWalkmesh;
    std::shared_ptr<ShaderProgram> _spParticle;
    std::shared_ptr<ShaderProgram> _spGrass;
    std::shared_ptr<ShaderProgram> _spBillboard;
    std::shared_ptr<ShaderProgram> _spSSAO;
    std::shared_ptr<ShaderProgram> _spSSR;
    std::shared_ptr<ShaderProgram> _spCombineOpaque;
    std::shared_ptr<ShaderProgram> _spCombineGeometry;

    std::shared_ptr<ShaderProgram> _spBoxBlur4;
    std::shared_ptr<ShaderProgram> _spGaussianBlur9;
    std::shared_ptr<ShaderProgram> _spGaussianBlur13;
    std::shared_ptr<ShaderProgram> _spMedianFilter3;
    std::shared_ptr<ShaderProgram> _spMedianFilter5;
    std::shared_ptr<ShaderProgram> _spFXAA;
    std::shared_ptr<ShaderProgram> _spSharpen;

    // END Shader Programs

    std::shared_ptr<Shader> initShader(ShaderType type, std::vector<std::string> sourceFiles);
    std::shared_ptr<ShaderProgram> initShaderProgram(std::vector<std::shared_ptr<Shader>> shaders);

    ShaderProgram &getProgram(ShaderProgramId id);
};

} // namespace graphics

} // namespace reone
