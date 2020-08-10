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

#include "util.h"

#include "../../resources/resources.h"
#include "../../script/execution.h"
#include "../../script/types.h"

#include "routines.h"

using namespace std;

using namespace reone::resources;
using namespace reone::script;

namespace reone {

namespace game {

int runScript(const string &resRef, uint32_t callerId, uint32_t triggererId) {
    shared_ptr<ScriptProgram> program(ResMan.findScript(resRef));
    return runScript(move(program), callerId, triggererId);
}

int runScript(const std::shared_ptr<ScriptProgram> &program, uint32_t callerId, uint32_t triggererId) {
    ExecutionContext ctx;
    ctx.routines = &RoutineMan;
    ctx.callerId = callerId;
    ctx.triggererId = triggererId;

    return ScriptExecution(program, move(ctx)).run();
}

} // namespace game

} // namespace reone