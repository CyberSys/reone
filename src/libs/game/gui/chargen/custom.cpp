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

#include "reone/game/gui/chargen/custom.h"

#include "reone/game/game.h"
#include "reone/game/gui/chargen.h"
#include "reone/gui/control/button.h"
#include "reone/gui/control/label.h"

using namespace reone::audio;
using namespace reone::gui;
using namespace reone::graphics;
using namespace reone::resource;

namespace reone {

namespace game {

void CustomCharacterGeneration::onGUILoaded() {
    bindControls();

    doSetStep(0);

    _controls.BTN_CANCEL->setOnClick([this]() {
        setStep(0);
        _charGen.openQuickOrCustom();
    });
    _controls.BTN_BACK->setOnClick([this]() {
        if (_step == 0) {
            _charGen.openQuickOrCustom();
        } else {
            setStep(_step - 1);
        }
    });
    _controls.BTN_STEPNAME1->setOnClick([this]() {
        _charGen.openPortraitSelection();
    });
    _controls.BTN_STEPNAME2->setOnClick([this]() {
        _charGen.openAbilities();
    });
    _controls.BTN_STEPNAME3->setOnClick([this]() {
        _charGen.openSkills();
    });
    _controls.BTN_STEPNAME4->setOnClick([this]() {
        _charGen.openFeats();
    });
    _controls.BTN_STEPNAME5->setOnClick([this]() {
        _charGen.openNameEntry();
    });
    _controls.BTN_STEPNAME6->setOnClick([this]() {
        _charGen.finish();
    });

    if (!_game.isTSL()) {
        _controls.LBL_BG->setDiscardColor(glm::vec3(0.0f, 0.0f, 0.082353f));
    }
}

void CustomCharacterGeneration::setStep(int step) {
    if (_step != step) {
        doSetStep(step);
    }
}

void CustomCharacterGeneration::doSetStep(int step) {
    _step = step;

    _controls.LBL_1->setFocusable(false);
    _controls.LBL_2->setFocusable(false);
    _controls.LBL_3->setFocusable(false);
    _controls.LBL_4->setFocusable(false);
    _controls.LBL_5->setFocusable(false);
    _controls.LBL_6->setFocusable(false);
    _controls.BTN_STEPNAME1->setFocusable(false);
    _controls.BTN_STEPNAME2->setFocusable(false);
    _controls.BTN_STEPNAME3->setFocusable(false);
    _controls.BTN_STEPNAME4->setFocusable(false);
    _controls.BTN_STEPNAME5->setFocusable(false);
    _controls.BTN_STEPNAME6->setFocusable(false);

    _controls.LBL_1->setDisabled(_step != 0);
    _controls.LBL_2->setDisabled(_step != 1);
    _controls.LBL_3->setDisabled(_step != 2);
    _controls.LBL_4->setDisabled(_step != 3);
    _controls.LBL_5->setDisabled(_step != 4);
    _controls.LBL_6->setDisabled(_step != 5);
    _controls.BTN_STEPNAME1->setDisabled(_step != 0);
    _controls.BTN_STEPNAME2->setDisabled(_step != 1);
    _controls.BTN_STEPNAME3->setDisabled(_step != 2);
    _controls.BTN_STEPNAME4->setDisabled(_step != 3);
    _controls.BTN_STEPNAME5->setDisabled(_step != 4);
    _controls.BTN_STEPNAME6->setDisabled(_step != 5);

    _controls.LBL_1->setFocus(_step == 0);
    _controls.LBL_2->setFocus(_step == 1);
    _controls.LBL_3->setFocus(_step == 2);
    _controls.LBL_4->setFocus(_step == 3);
    _controls.LBL_5->setFocus(_step == 4);
    _controls.LBL_6->setFocus(_step == 5);
    _controls.LBL_NUM1->setFocus(_step == 0);
    _controls.LBL_NUM2->setFocus(_step == 1);
    _controls.LBL_NUM3->setFocus(_step == 2);
    _controls.LBL_NUM4->setFocus(_step == 3);
    _controls.LBL_NUM5->setFocus(_step == 4);
    _controls.LBL_NUM6->setFocus(_step == 5);
    _controls.BTN_STEPNAME1->setFocus(_step == 0);
    _controls.BTN_STEPNAME2->setFocus(_step == 1);
    _controls.BTN_STEPNAME3->setFocus(_step == 2);
    _controls.BTN_STEPNAME4->setFocus(_step == 3);
    _controls.BTN_STEPNAME5->setFocus(_step == 4);
    _controls.BTN_STEPNAME6->setFocus(_step == 5);
}

void CustomCharacterGeneration::goToNextStep() {
    if (_step < 6) {
        doSetStep(_step + 1);
    }
}

} // namespace game

} // namespace reone
