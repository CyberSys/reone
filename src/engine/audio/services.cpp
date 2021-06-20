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

#include "services.h"

#include "../resource/services.h"

#include "files.h"
#include "player.h"

using namespace std;

using namespace reone::resource;

namespace reone {

namespace audio {

AudioServices::AudioServices(AudioOptions options, ResourceServices &resource) :
    _options(move(options)),
    _resource(resource) {
}

void AudioServices::init() {
    _files = make_unique<AudioFiles>(_resource.resources());

    _player = make_unique<AudioPlayer>(_options, *_files);
    _player->init();
}

} // namespace audio

} // namespace reone
