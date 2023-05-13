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

#include "reone/resource/di/module.h"

#include "../options.h"

#include "services.h"

namespace reone {

namespace audio {

class AudioModule : boost::noncopyable {
public:
    AudioModule(AudioOptions &options, resource::ResourceModule &resource) :
        _options(options),
        _resource(resource) {
    }

    ~AudioModule() { deinit(); }

    void init();
    void deinit();

    AudioContext &audioContext() { return *_audioContext; }
    AudioFiles &audioFiles() { return *_audioFiles; }
    AudioPlayer &audioPlayer() { return *_audioPlayer; }

    AudioServices &services() { return *_services; }

private:
    AudioOptions &_options;
    resource::ResourceModule &_resource;

    std::unique_ptr<AudioContext> _audioContext;
    std::unique_ptr<AudioFiles> _audioFiles;
    std::unique_ptr<AudioPlayer> _audioPlayer;

    std::unique_ptr<AudioServices> _services;
};

} // namespace audio

} // namespace reone
