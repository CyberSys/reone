/*
 * Copyright (c) 2020-2022 The reone project contributors
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

#include "format/tlkreader.h"

#include "types.h"

namespace reone {

namespace resource {

class IStrings {
public:
    virtual ~IStrings() = default;
};

class Strings : public IStrings {
public:
    Strings() = default;

    void init(const boost::filesystem::path &gameDir);

    /**
     * @return string or empty string when not found
     */
    std::string get(int strRef);

    /**
     * @return ResRef or empty string when not found
     */
    std::string getSound(int strRef);

    void setTalkTable(std::shared_ptr<TalkTable> table) {
        _table = std::move(table);
    }

private:
    std::shared_ptr<TalkTable> _table;

    void process(std::string &str);
    void stripDeveloperNotes(std::string &str);
};

} // namespace resource

} // namespace reone
