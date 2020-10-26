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

#include "../../resource/resources.h"
#include "../../script/execution.h"
#include "../../script/scripts.h"
#include "../../script/types.h"

#include "routines.h"

using namespace std;

using namespace reone::resource;
using namespace reone::script;

namespace reone {

namespace game {

int runScript(const string &resRef, uint32_t callerId, uint32_t triggererId, int userDefinedEventNumber) {
    shared_ptr<ScriptProgram> program(Scripts::instance().get(resRef));
    return runScript(move(program), callerId, triggererId, userDefinedEventNumber);
}

int runScript(const std::shared_ptr<ScriptProgram> &program, uint32_t callerId, uint32_t triggererId, int userDefinedEventNumber) {
    ExecutionContext ctx;
    ctx.routines = &Routines::instance();
    ctx.callerId = callerId;
    ctx.triggererId = triggererId;
    ctx.userDefinedEventNumber = userDefinedEventNumber;

    return ScriptExecution(program, move(ctx)).run();
}

} // namespace game

} // namespace reone
