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

#include "../audio/soundinstance.h"
#include "../audio/types.h"
#include "../gui/gui.h"
#include "../render/window.h"
#include "../resources/types.h"

#include "gui/classsel.h"
#include "gui/container.h"
#include "gui/debug.h"
#include "gui/dialog.h"
#include "gui/hud.h"
#include "gui/mainmenu.h"
#include "gui/portraitsel.h"

#include "area.h"
#include "console.h"
#include "module.h"

namespace reone {

namespace game {

/**
 * Entry point for the game logic. Contains the main game loop.
 */
class Game : public render::IEventHandler, public IRoutineCallbacks {
public:
    Game(
        resources::GameVersion version,
        const boost::filesystem::path &path,
        const Options &opts);

    int run();
    void loadModule(const std::string &name, const PartyConfiguration &party, std::string entry = "");
    bool handle(const SDL_Event &event) override;

    // Routine callbacks

    void delayCommand(uint32_t timestamp, const script::ExecutionContext &ctx) override;
    int eventUserDefined(int eventNumber) override;
    void signalEvent(uint32_t objectId, int eventId) override;

    // Objects

    Module *getModule() override;
    Area *getArea() override;
    std::shared_ptr<Object> getObjectById(uint32_t id) override;
    std::shared_ptr<Object> getObjectByTag(const std::string &tag, int nth) override;
    std::shared_ptr<Object> getWaypointByTag(const std::string &tag) override;
    std::shared_ptr<Object> getPlayer() override;

    // END Objects

    // Globals/locals

    bool getGlobalBoolean(const std::string &name) const override;
    int getGlobalNumber(const std::string &name) const override;
    bool getLocalBoolean(uint32_t objectId, int index) const override;
    int getLocalNumber(uint32_t objectId, int index) const override;

    void setGlobalBoolean(const std::string &name, bool value) override;
    void setGlobalNumber(const std::string &name, int value) override;
    void setLocalBoolean(uint32_t objectId, int index, bool value) override;
    void setLocalNumber(uint32_t objectId, int index, int value) override;

    // END Globals/locals

    // END Routine callbacks

protected:
    enum class Screen {
        None,
        MainMenu,
        ClassSelection,
        PortraitSelection,
        InGame,
        Dialog,
        Container
    };

    resources::GameVersion _version { resources::GameVersion::KotOR };
    Options _opts;
    std::unique_ptr<ObjectFactory> _objectFactory;
    std::shared_ptr<Module> _module;
    std::string _nextModule;
    Screen _screen { Screen::None };
    std::shared_ptr<DialogGui> _dialogGui;
    bool _pickDialogReplyEnabled { true };

    virtual void initObjectFactory();
    virtual void configure();
    virtual void configureModule();
    virtual void update();
    virtual void loadNextModule();
    virtual void startDialog(uint32_t ownerId, const std::string &resRef);
    virtual void onDialogReplyPicked(uint32_t index);
    virtual void onDialogFinished();

private:
    boost::filesystem::path _path;
    render::RenderWindow _window;
    uint32_t _ticks { 0 };
    bool _quit { false };
    std::string _nextEntry;
    GameState _state;
    std::shared_ptr<audio::SoundInstance> _music;
    Console _console;

    // GUI
    std::shared_ptr<MainMenu> _mainMenu;
    std::shared_ptr<ClassSelectionGui> _classesGui;
    std::shared_ptr<PortraitSelectionGui> _portraitsGui;
    std::shared_ptr<HUD> _hud;
    std::shared_ptr<DebugOverlay> _debugOverlay;
    std::shared_ptr<ContainerGui> _containerGui;

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    // Loading
    void loadMainMenu();
    void loadClassSelectionGui();
    void loadPortraitsGui();
    void loadHUD();
    void loadDebugOverlay();
    void loadDialogGui();
    void loadContainerGui();
    void loadCursor();

    // Rendering
    void drawWorld();
    void drawGUI();
    void drawGUI3D();
    void drawCursor();

    void runMainLoop();
    std::shared_ptr<gui::GUI> currentGUI() const;
    float getDeltaTime();

    void onDialogSpeakerChanged(uint32_t from, uint32_t to);
};

} // namespace game

} // namespace reone
