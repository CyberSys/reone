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

#include "dialog.h"

namespace reone {

namespace resource {

class Gffs;
class Gff;
class Strings;

} // namespace resource

namespace game {

class IDialogs {
public:
    virtual ~IDialogs() = default;

    virtual void invalidate() = 0;

    virtual std::shared_ptr<Dialog> get(const std::string &key) = 0;
};

class Dialogs : public IDialogs {
public:
    Dialogs(resource::Gffs &gffs, resource::Strings &strings) :
        _gffs(gffs),
        _strings(strings) {
    }

    void invalidate() override {
        _objects.clear();
    }

    std::shared_ptr<Dialog> get(const std::string &key) override {
        auto maybeObject = _objects.find(key);
        if (maybeObject != _objects.end()) {
            return maybeObject->second;
        }
        auto object = doGet(key);
        return _objects.insert(make_pair(key, std::move(object))).first->second;
    }

private:
    resource::Gffs &_gffs;
    resource::Strings &_strings;

    std::unordered_map<std::string, std::shared_ptr<Dialog>> _objects;

    std::shared_ptr<Dialog> doGet(std::string resRef);

    std::unique_ptr<Dialog> loadDialog(const resource::Gff &dlg);

    Dialog::EntryReplyLink getEntryReplyLink(const resource::Gff &gffs) const;
    Dialog::EntryReply getEntryReply(const resource::Gff &gffs) const;
    Dialog::Stunt getStunt(const resource::Gff &gffs) const;
    Dialog::ParticipantAnimation getParticipantAnimation(const resource::Gff &gffs) const;
};

} // namespace game

} // namespace reone
