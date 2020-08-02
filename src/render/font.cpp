#include "font.h"

#include <stdexcept>

#include "GL/glew.h"

#include "SDL2/SDL_opengl.h"

#include "glm/ext.hpp"

#include "shadermanager.h"

namespace reone {

namespace render {

static const int kVertexValuesPerGlyph = 20;
static const int kIndicesPerGlyph = 6;

void Font::load(const std::shared_ptr<Texture> &texture) {
    if (!texture) {
        throw std::invalid_argument("Invalid font texture");
    }
    _texture = texture;

    const TextureFeatures &features = _texture->features();

    _glyphCount = features.numChars;
    _height = features.fontHeight * 100.0f;

    _vertices.resize(kVertexValuesPerGlyph * _glyphCount);
    _indices.resize(kIndicesPerGlyph * _glyphCount);
    _glyphWidths.resize(_glyphCount);

    for (int i = 0; i < _glyphCount; ++i) {
        glm::highp_dvec3 ul(features.upperLeftCoords[i]);
        glm::highp_dvec3 lr(features.lowerRightCoords[i]);

        float w = lr.x - ul.x;
        float h = ul.y - lr.y;
        float aspect = w / h;
        float width = aspect * _height;

        float *pv = &_vertices[kVertexValuesPerGlyph * i];
        pv[0] = 0.0f; pv[1] = _height; pv[2] = 0.0f; pv[3] = ul.x; pv[4] = lr.y;
        pv[5] = width; pv[6] = _height; pv[7] = 0.0f; pv[8] = lr.x; pv[9] = lr.y;
        pv[10] = width; pv[11] = 0.0f; pv[12] = 0.0f; pv[13] = lr.x; pv[14] = ul.y;
        pv[15] = 0.0f; pv[16] = 0.0f; pv[17] = 0.0f; pv[18] = ul.x; pv[19] = ul.y;

        uint16_t *iv = &_indices[kIndicesPerGlyph * i];
        int off = 4 * i;
        iv[0] = off + 0; iv[1] = off + 1; iv[2] = off + 2;
        iv[3] = off + 2; iv[4] = off + 3; iv[5] = off + 0;

        _glyphWidths[i] = width;
    }
}

void Font::initGL() {
    if (_glInited) return;

    assert(!_vertices.empty() && !_indices.empty());

    glGenBuffers(1, &_vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint16_t), &_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &_vertexArrayId);
    glBindVertexArray(_vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);

    int stride = 5 * sizeof(float);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(0));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(12));

    glBindVertexArray(0);

    _glInited = true;

    if (_texture) _texture->initGL();
}

void Font::render(const std::string &text, const glm::mat4 &transform, const glm::vec3 &color, TextAlign align) const {
    if (text.empty()) return;

    ShaderManager &shaders = ShaderManager::instance();
    shaders.activate(ShaderProgram::GUIText);
    shaders.setUniform("textColor", color);

    assert(_texture);
    glActiveTexture(GL_TEXTURE0);
    _texture->bind();

    glBindVertexArray(_vertexArrayId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

    float textWidth = measure(text);
    glm::vec3 textOffset;

    switch (align) {
        case TextAlign::Left:
            textOffset = glm::vec3(-textWidth, -0.5f * _height, 0.0f);
            break;
        case TextAlign::Center:
            textOffset = glm::vec3(-0.5f * textWidth, -0.5f * _height, 0.0f);
            break;
        case TextAlign::Right:
            textOffset = glm::vec3(0.0f, -0.5f * _height, 0.0f);
            break;
    }

    glm::mat4 textTransform(glm::translate(transform, textOffset));

    for (auto &glyph : text) {
        assert(glyph < _glyphCount);
        shaders.setUniform("model", textTransform);

        int off = kIndicesPerGlyph * glyph * sizeof(uint16_t);
        glDrawElements(GL_TRIANGLES, kIndicesPerGlyph, GL_UNSIGNED_SHORT, reinterpret_cast<void *>(off));

        float w = _glyphWidths[glyph];
        textTransform = glm::translate(textTransform, glm::vec3(w, 0.0f, 0.0f));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _texture->unbind();
    shaders.deactivate();
}

float Font::measure(const std::string &text) const {
    float w = 0.0f;
    for (auto &glyph : text) {
        w += _glyphWidths[glyph];
    }

    return w;
}

float Font::height() const {
    return _height;
}

} // namespace render

} // namespace reone
