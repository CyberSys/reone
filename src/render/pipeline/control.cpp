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

#include "control.h"

#include "glm/ext.hpp"

#include "GL/glew.h"

#include "../mesh/quad.h"
#include "../scene/scenegraph.h"
#include "../shaders.h"

using namespace std;

namespace reone {

namespace render {

ControlRenderPipeline::ControlRenderPipeline(SceneGraph *sceneGraph, const glm::ivec4 &extent) :
    _sceneGraph(sceneGraph),
    _extent(extent),
    _geometry(_extent[2], _extent[3]) {
}

void ControlRenderPipeline::init() {
    _geometry.init();
}

void ControlRenderPipeline::render(const glm::ivec2 &offset) const {
    ShaderManager &shaders = Shaders;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Render to framebuffer
    {
        glViewport(0, 0, _extent[2], _extent[3]);

        _geometry.bind();

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _sceneGraph->render();
        _geometry.unbind();

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }
    // Render control
    {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(_extent[0] + offset.x, _extent[1] + offset.y, 100.0f));
        transform = glm::scale(transform, glm::vec3(_extent[2], _extent[3], 1.0f));

        GlobalUniforms globals;
        globals.projection = glm::ortho(
            0.0f,
            static_cast<float>(viewport[2]),
            static_cast<float>(viewport[3]),
            0.0f,
            -100.0f, 100.0f);

        shaders.setGlobalUniforms(globals);

        LocalUniforms locals;
        locals.model = move(transform);

        shaders.activate(ShaderProgram::GUIGUI, locals);

        glActiveTexture(GL_TEXTURE0);
        _geometry.bindColorBuffer(0);

        DefaultQuad.render(GL_TRIANGLES);

        _geometry.unbindColorBuffer();
    }
}

} // namespace render

} // namespace reone
