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

#include "engine.h"

#include "SDL2/SDL.h"

#include "reone/graphics/window.h"
#include "reone/resource/exception/notfound.h"
#include "reone/resource/gameprobe.h"
#include "reone/system/threadutil.h"

using namespace reone::audio;
using namespace reone::game;
using namespace reone::graphics;
using namespace reone::gui;
using namespace reone::movie;
using namespace reone::resource;
using namespace reone::scene;
using namespace reone::script;

#define R_NEO_GAME 1

namespace reone {

void Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
    }
    _window = std::make_unique<Window>(_options.graphics);
    _window->init();

    _optionsView = _options.toView();
    GameProbe probe {_options.game.path};
    auto gameId = probe.probe();

    _clock = std::make_unique<Clock>();
    _clock->init();

    _systemModule = std::make_unique<SystemModule>(*_clock);
    _graphicsModule = std::make_unique<GraphicsModule>(_options.graphics);
    _audioModule = std::make_unique<AudioModule>(_options.audio);
    _movieModule = std::make_unique<MovieModule>();
    _scriptModule = std::make_unique<ScriptModule>();
    _resourceModule = std::make_unique<ResourceModule>(
        gameId,
        _options.game.path,
        _options.graphics,
        _options.audio,
        *_graphicsModule,
        *_audioModule,
        *_scriptModule);
    _sceneModule = std::make_unique<SceneModule>(
        _options.graphics,
        *_resourceModule,
        *_graphicsModule,
        *_audioModule);
    _guiModule = std::make_unique<GUIModule>(
        _options.graphics,
        *_sceneModule,
        *_graphicsModule,
        *_resourceModule);
    _gameModule = std::make_unique<GameModule>(
        gameId,
        *_optionsView,
        *_resourceModule,
        *_graphicsModule,
        *_audioModule,
        *_sceneModule,
        *_scriptModule);
    _systemModule->init();
    _graphicsModule->init();
    _audioModule->init();
    _movieModule->init();
    _scriptModule->init();
    _resourceModule->init();
    _sceneModule->init();
    _guiModule->init();
    _gameModule->init();

    _services = std::make_unique<ServicesView>(
        _gameModule->services(),
        _movieModule->services(),
        _audioModule->services(),
        _graphicsModule->services(),
        _sceneModule->services(),
        _guiModule->services(),
        _scriptModule->services(),
        _resourceModule->services(),
        _systemModule->services());

#if R_NEO_GAME
    _services->resource.director.onModuleLoad("end_m01aa");
    _neoGame = std::make_unique<neo::Game>(
        *_optionsView,
        _services->graphics,
        _services->resource,
        _services->scene);
    _neoGame->init();
    showCursor(false);
    setRelativeMouseMode(true);
#else
    _game = std::make_unique<Game>(gameId, _options.game.path, *_optionsView, *_services);
    _game->init();
#endif

    _profiler = std::make_unique<Profiler>(
        _options.graphics,
        _services->graphics,
        _services->resource,
        _services->system);
    _profiler->init();
}

void Engine::deinit() {
    _profiler.reset();
#if R_NEO_GAME
    _neoGame.reset();
#else
    _game.reset();
#endif
    _services.reset();

    _gameModule.reset();
    _guiModule.reset();
    _sceneModule.reset();
    _resourceModule.reset();
    _scriptModule.reset();
    _movieModule.reset();
    _audioModule.reset();
    _graphicsModule.reset();
    _systemModule.reset();
    _clock.reset();

    _optionsView.reset();
    _window.reset();

    SDL_Quit();
}

int Engine::run() {
    std::thread gameThread {std::bind(&Engine::gameThreadFunc, this)};

    auto &clock = _services->system.clock;
    _ticks = clock.millis();

    while (!_quit.load(std::memory_order::memory_order_acquire)) {
        bool quit;
        processEvents(quit);
        if (quit) {
            _quit.store(true, std::memory_order::memory_order_release);
            break;
        }
        bool focus = _window->isInFocus();
        _focus.store(focus, std::memory_order::memory_order_release);
        if (!focus) {
            std::this_thread::sleep_for(std::chrono::milliseconds {100});
            continue;
        }
        auto ticks = clock.millis();
        auto frameTime = (ticks - _ticks) / 1000.0f;
        _ticks = ticks;
        _profiler->timeInput([this, &quit]() {
            std::queue<input::Event> events;
            {
                std::lock_guard<std::mutex> lock {_eventsMutex};
                std::swap(events, _events);
            }
            while (!events.empty()) {
                auto event = events.front();
                events.pop();
#if R_NEO_GAME
                if (_neoGame->handle(event)) {
                    continue;
                }
#else
                if (_game->handle(event)) {
                    if (_game->isQuitRequested()) {
                        quit = true;
                        break;
                    }
                    continue;
                }
#endif
                _profiler->handle(event);
            }
        });
        if (quit) {
            _quit.store(true, std::memory_order::memory_order_release);
            break;
        }
        _profiler->timeUpdate([this, &frameTime]() {
#if R_NEO_GAME
            _neoGame->update(frameTime);
#else
            _game->update(frameTime);
            bool showcur = _game->cursorType() == CursorType::None;
            bool relmouse = _game->relativeMouseMode();
            showCursor(showcur);
            setRelativeMouseMode(relmouse);
#endif
            _profiler->update(frameTime);
        });
        _profiler->timeRender([this]() {
            _services->graphics.statistic.resetDrawCalls();
            runMainThreadTasks();
            if (_options.graphics.pbr) {
                _services->graphics.pbrTextures.refresh();
            }
            _services->graphics.context.clearColorDepth();
#if R_NEO_GAME
            _neoGame->render();
#else
            _game->render();
#endif
            _profiler->render();
            _window->swap();
            _services->audio.mixer.render();
        });
        _frameState.store(FrameStates::rendered, std::memory_order_release);
    }

    if (gameThread.joinable()) {
        gameThread.join();
    }

    return 0;
}

void Engine::gameThreadFunc() {
    setThreadName("game");

    while (!_quit.load(std::memory_order::memory_order_acquire)) {
        if (!_focus.load(std::memory_order::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::milliseconds {100});
            continue;
        }
        int expectedState = FrameStates::rendered;
        if (!_frameState.compare_exchange_strong(
                expectedState,
                FrameStates::updating,
                std::memory_order_acq_rel)) {
            continue;
        }
        // TODO: move input handling and updates here from the main thread
        _frameState.store(FrameStates::updated, std::memory_order::memory_order_release);
    }
}

void Engine::processEvents(bool &quit) {
    std::queue<input::Event> unhandled;
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        if (sdlEvent.type == SDL_QUIT) {
            quit = true;
            break;
        }
        if (!_window->isAssociatedWith(sdlEvent)) {
            continue;
        }
        if (_window->handle(sdlEvent)) {
            if (_window->isCloseRequested()) {
                quit = true;
                break;
            }
            continue;
        }
        auto event = eventFromSDLEvent(sdlEvent);
        if (!event) {
            continue;
        }
        if (_profiler->handle(*event)) {
            continue;
        }
        unhandled.push(*event);
    }
    std::lock_guard<std::mutex> lock {_eventsMutex};
    while (!unhandled.empty()) {
        _events.push(std::move(unhandled.front()));
        unhandled.pop();
    }
}

void Engine::showCursor(bool show) {
    if (_showCursor == show) {
        return;
    }
    SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
    _showCursor = show;
}

void Engine::setRelativeMouseMode(bool relative) {
    if (_relativeMouseMode == relative) {
        return;
    }
    SDL_SetRelativeMouseMode(relative ? SDL_TRUE : SDL_FALSE);
    _relativeMouseMode = relative;
}

std::optional<input::Event> Engine::eventFromSDLEvent(const SDL_Event &sdlEvent) const {
    switch (sdlEvent.type) {
    case SDL_KEYDOWN:
        return input::Event::newKeyDown(input::KeyEvent {
            sdlEvent.key.state == SDL_PRESSED,
            static_cast<input::KeyCode>(sdlEvent.key.keysym.sym),
            sdlEvent.key.keysym.mod,
            static_cast<bool>(sdlEvent.key.repeat)});
    case SDL_KEYUP:
        return input::Event::newKeyUp(input::KeyEvent {
            sdlEvent.key.state == SDL_PRESSED,
            static_cast<input::KeyCode>(sdlEvent.key.keysym.sym),
            sdlEvent.key.keysym.mod,
            static_cast<bool>(sdlEvent.key.repeat)});
    case SDL_MOUSEMOTION:
        return input::Event::newMouseMotion(input::MouseMotionEvent {
            sdlEvent.motion.x,
            sdlEvent.motion.y,
            sdlEvent.motion.xrel,
            sdlEvent.motion.yrel});
    case SDL_MOUSEBUTTONDOWN:
        return input::Event::newMouseButtonDown(input::MouseButtonEvent {
            static_cast<input::MouseButton>(sdlEvent.button.button),
            sdlEvent.button.state == SDL_PRESSED,
            sdlEvent.button.clicks,
            sdlEvent.button.x,
            sdlEvent.button.y});
    case SDL_MOUSEBUTTONUP:
        return input::Event::newMouseButtonUp(input::MouseButtonEvent {
            static_cast<input::MouseButton>(sdlEvent.button.button),
            sdlEvent.button.state == SDL_PRESSED,
            sdlEvent.button.clicks,
            sdlEvent.button.x,
            sdlEvent.button.y});
    case SDL_MOUSEWHEEL: {
        return input::Event::newMouseWheel(input::MouseWheelEvent {
            sdlEvent.wheel.x,
            sdlEvent.wheel.y,
            static_cast<input::MouseWheelDirection>(sdlEvent.wheel.direction)});
    default:
        return std::nullopt;
    }
    }
}

} // namespace reone
