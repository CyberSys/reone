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

namespace reone {

class TestGameData {
public:
    ~TestGameData() {
        deinit();
    };

    void init() {
        auto gamePath = boost::filesystem::temp_directory_path();
        gamePath.append("reone_test_game");
        boost::filesystem::create_directory(gamePath);
        _gamePath = gamePath;

        auto modulesPath = gamePath;
        modulesPath.append("modules");
        boost::filesystem::create_directory(modulesPath);
    }

    void deinit() {
        boost::filesystem::remove_all(_gamePath);
    }

    const boost::filesystem::path &gamePath() const {
        return _gamePath;
    }

private:
    boost::filesystem::path _gamePath;
};

} // namespace reone
