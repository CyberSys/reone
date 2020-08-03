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

#include <functional>
#include <vector>

#include "../resources/types.h"

#include "types.h"
#include "variable.h"

namespace reone {

namespace script {

class RoutineDecl {
public:
    RoutineDecl() = default;
    RoutineDecl(const std::string &name, VariableType retType, const std::vector<VariableType> &argTypes);
    RoutineDecl(const std::string &name, VariableType retType, const std::vector<VariableType> &argTypes, const std::function<Variable(const std::vector<Variable> &, ExecutionContext &ctx)> &fn);

    Variable invoke(const std::vector<Variable> &args, ExecutionContext &ctx) const;

    const std::string &name() const;
    VariableType returnType() const;
    VariableType argumentType(int index) const;

private:
    std::string _name;
    VariableType _returnType { VariableType::Void };
    std::vector<VariableType> _argumentTypes;
    std::function<Variable(const std::vector<Variable> &, ExecutionContext &ctx)> _func;
};

class RoutineManager {
public:
    static RoutineManager &instance();

    void init(resources::GameVersion version);

    const RoutineDecl &get(int index);

private:
    std::vector<RoutineDecl> _routines;

    RoutineManager() = default;

    RoutineManager(const RoutineManager &) = delete;
    RoutineManager &operator=(const RoutineManager &) = delete;

    void addKotorRoutines();
    void addTslRoutines();

    // Routine implementations
    Variable delayCommand(const std::vector<Variable> &args, ExecutionContext &ctx);
    Variable getEnteringObject(const std::vector<Variable> &args, ExecutionContext &ctx);
    Variable getIsPC(const std::vector<Variable> &args, ExecutionContext &ctx);
};

#define RoutineMan script::RoutineManager::instance()

} // namespace script

} // namespace reone
