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

#include "lipanimation.h"

namespace reone {

namespace resource {

class Resources;

}

namespace graphics {

class ILipAnimations {
public:
    virtual ~ILipAnimations() = default;

    virtual void invalidate() = 0;

    virtual std::shared_ptr<LipAnimation> get(const std::string &key) = 0;
};

class LipAnimations : public ILipAnimations {
public:
    LipAnimations(resource::Resources &resources) :
        _resources(resources) {
    }

    void invalidate() override {
        _objects.clear();
    }

    std::shared_ptr<LipAnimation> get(const std::string &key) override {
        auto maybeObject = _objects.find(key);
        if (maybeObject != _objects.end()) {
            return maybeObject->second;
        }
        auto object = doGet(key);
        return _objects.insert(make_pair(key, std::move(object))).first->second;
    }

private:
    resource::Resources &_resources;

    std::unordered_map<std::string, std::shared_ptr<LipAnimation>> _objects;

    std::shared_ptr<LipAnimation> doGet(std::string resRef);
};

} // namespace graphics

} // namespace reone
