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

#include "reone/system/threadutil.h"

namespace reone {

static std::thread::id g_mainThreadId;
static std::unordered_map<std::thread::id, std::string> g_threadNames;
static std::mutex g_threadNamesMutex;
static std::queue<std::function<void()>> g_mainThreadTasks;
static std::mutex g_mainThreadTasksMutex;

void markMainThread() {
    g_mainThreadId = std::this_thread::get_id();
    setThreadName("main");
}

void checkMainThread() {
    if (std::this_thread::get_id() != g_mainThreadId) {
        throw std::logic_error("Operation forbidden outside the main thread");
    }
}

void runOnMainThread(std::function<void()> task) {
    if (isMainThread()) {
        task();
        return;
    }
    std::lock_guard<std::mutex> lock {g_mainThreadTasksMutex};
    g_mainThreadTasks.push(std::move(task));
}

void runMainThreadTasks() {
    std::queue<std::function<void()>> tasks;
    {
        std::lock_guard<std::mutex> lock {g_mainThreadTasksMutex};
        std::swap(tasks, g_mainThreadTasks);
    }
    while (!tasks.empty()) {
        tasks.front()();
        tasks.pop();
    }
}

bool isMainThread() {
    return std::this_thread::get_id() == g_mainThreadId;
}

void setThreadName(std::string name) {
    std::lock_guard<std::mutex> lock {g_threadNamesMutex};
    g_threadNames.insert_or_assign(std::this_thread::get_id(), std::move(name));
}

const std::string &threadName() {
    std::lock_guard<std::mutex> lock {g_threadNamesMutex};
    auto id = std::this_thread::get_id();
    if (g_threadNames.count(id) == 0) {
        std::ostringstream stream;
        stream << id;
        g_threadNames.insert({id, stream.str()});
    }
    return g_threadNames.at(id);
}

} // namespace reone
