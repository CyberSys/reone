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

#include "reone/resource/format/erfreader.h"

#include "reone/system/exception/validation.h"

namespace reone {

namespace resource {

void ErfReader::onLoad() {
    checkSignature();
    ignore(8);

    _numEntries = readUint32();

    ignore(4);

    _offKeys = readUint32();
    _offResources = readUint32();

    loadKeys();
    loadResources();
}

void ErfReader::checkSignature() {
    if (_size < 8) {
        throw ValidationException("Invalid binary resource size");
    }
    auto signature = readString(8);
    bool erf = signature == std::string("ERF V1.0", 8);
    bool mod = signature == std::string("MOD V1.0", 8);
    if (!erf && !mod) {
        throw ValidationException("Invalid ERF/MOD signature: " + signature);
    }
}

void ErfReader::loadKeys() {
    _keys.reserve(_numEntries);
    seek(_offKeys);

    for (int i = 0; i < _numEntries; ++i) {
        _keys.push_back(readKeyEntry());
    }
}

ErfReader::KeyEntry ErfReader::readKeyEntry() {
    auto resRef = boost::to_lower_copy(readCString(16));
    auto resId = readUint32();
    auto resType = readUint16();
    ignore(2); // unused

    auto key = KeyEntry();
    key.resId = ResourceId(std::move(resRef), static_cast<ResourceType>(resType));

    return std::move(key);
}

void ErfReader::loadResources() {
    _resources.reserve(_numEntries);
    seek(_offResources);

    for (int i = 0; i < _numEntries; ++i) {
        _resources.push_back(readResourceEntry());
    }
}

ErfReader::ResourceEntry ErfReader::readResourceEntry() {
    auto offset = readUint32();
    auto size = readUint32();

    ResourceEntry resource;
    resource.offset = offset;
    resource.size = size;

    return std::move(resource);
}

} // namespace resource

} // namespace reone
