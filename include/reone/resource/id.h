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

#include "resref.h"
#include "types.h"
#include "typeutil.h"

namespace reone {

namespace resource {

struct ResourceId {
    ResRef resRef;
    ResType type {ResType::Invalid};

    ResourceId() = default;

    ResourceId(std::string resRef, ResType type) :
        resRef(ResRef(std::move(resRef))),
        type(type) {
    }

    ResourceId(ResRef resRef, ResType type) :
        resRef(std::move(resRef)),
        type(type) {
    }

    std::string string() const {
        return resRef.value() + "." + getExtByResType(type);
    }

    bool operator==(const ResourceId &rhs) const {
        return resRef == rhs.resRef && type == rhs.type;
    }

    bool operator!=(const ResourceId &rhs) const {
        return resRef != rhs.resRef || type != rhs.type;
    }
};

struct ResourceIdHasher {
    size_t operator()(const ResourceId &id) const {
        size_t seed = 0;
        boost::hash_combine(seed, id.resRef.value());
        boost::hash_combine(seed, id.type);
        return seed;
    }
};

struct ResourceIdComparer {
    bool operator()(const ResourceId &lhs, const ResourceId &rhs) const {
        if (lhs.resRef < rhs.resRef) {
            return true;
        }
        if (lhs.resRef > rhs.resRef) {
            return false;
        }
        return lhs.type < rhs.type;
    }
};

using ResourceIdSet = std::unordered_set<ResourceId, ResourceIdHasher>;

} // namespace resource

} // namespace reone
