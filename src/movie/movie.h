/*
 * Copyright (c) 2020-2021 The reone project contributors
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

#include "../audio/source.h"
#include "../common/types.h"
#include "../graphics/texture.h"

#include "videostream.h"

namespace reone {

namespace graphics {

struct GraphicsServices;

}

namespace audio {

struct AudioServices;

}

namespace movie {

class BikReader;

class Movie {
public:
    Movie(
        graphics::GraphicsServices &graphicsSvc,
        audio::AudioServices &audioSvc) :
        _graphicsSvc(graphicsSvc),
        _audioSvc(audioSvc) {
    }

    void init();
    void deinit();

    void update(float dt);
    void render();

    void finish() { _finished = true; }

    bool isFinished() const { return _finished; }

    void setVideoStream(std::shared_ptr<VideoStream> stream) { _videoStream = std::move(stream); }
    void setAudioStream(std::shared_ptr<audio::AudioStream> stream) { _audioStream = std::move(stream); }

private:
    graphics::GraphicsServices &_graphicsSvc;
    audio::AudioServices &_audioSvc;

    int _width {0};
    int _height {0};

    float _time {0.0f};
    bool _finished {false};

    std::shared_ptr<VideoStream> _videoStream;
    std::shared_ptr<audio::AudioStream> _audioStream;

    std::shared_ptr<graphics::Texture> _texture;
    std::shared_ptr<audio::AudioSource> _audioSource;
};

} // namespace movie

} // namespace reone
