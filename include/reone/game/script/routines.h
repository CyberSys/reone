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

#include "reone/script/routine.h"
#include "reone/script/routines.h"

#include "../types.h"

namespace reone {

namespace game {

struct ServicesView;
struct RoutineContext;

class Game;

class Routines : public script::IRoutines, boost::noncopyable {
public:
    Routines(GameID gameId, Game *game, ServicesView *services) :
        _gameId(gameId),
        _game(game),
        _services(services) {
    }

    void init();

    script::Routine &get(int index) override;

    int getNumRoutines() const override { return static_cast<int>(_routines.size()); }
    int getIndexByName(const std::string &name) const override;

private:
    GameID _gameId;
    Game *_game;
    ServicesView *_services;

    std::vector<script::Routine> _routines;

    void initForKotOR();
    void initForTSL();

    void add(
        std::string name,
        script::VariableType retType,
        std::vector<script::VariableType> argTypes,
        script::Variable (*fn)(const std::vector<script::Variable> &args, const RoutineContext &ctx));

    void add(
        std::string name,
        script::VariableType retType,
        std::vector<script::VariableType> argTypes,
        script::Variable (*fn)(const std::vector<script::Variable> &args, const RoutineContext &ctx),
        script::Variable defRetValue);
};

} // namespace game

} // namespace reone
