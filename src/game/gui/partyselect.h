/*
 * Copyright � 2020 Vsevolod Kremianskii
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

#include "../../system/gui/gui.h"

namespace reone {

namespace game {

static const int kNpcCount = 9;

class Game;

class PartySelection : public gui::GUI {
public:
    PartySelection(Game *game, resource::GameVersion version, const render::GraphicsOptions &opts);

    void load() override;

    void update();

private:
    Game *_game { nullptr };
    bool _added[kNpcCount];
    int _selectedNpc { -1 };

    void onClick(const std::string &control) override;
};

} // namespace game

} // namespace reone
