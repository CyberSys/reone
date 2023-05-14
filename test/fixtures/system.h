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

#include <gmock/gmock.h>

#include "reone/system/di/services.h"

namespace reone {

class MockClock : public IClock, boost::noncopyable {
public:
    uint32_t ticks() const override {
        return 0;
    }

    uint64_t performanceFrequency() const override {
        return 0;
    }

    uint64_t performanceCounter() const override {
        return 0;
    };
};

class TestSystemModule : boost::noncopyable {
public:
    void init() {
        _clock = std::make_unique<MockClock>();

        _services = std::make_unique<SystemServices>(*_clock);
    }

    SystemServices &services() {
        return *_services;
    }

private:
    std::unique_ptr<MockClock> _clock;

    std::unique_ptr<SystemServices> _services;
};

} // namespace reone
