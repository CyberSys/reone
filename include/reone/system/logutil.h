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

#include <boost/log/sources/channel_logger.hpp>

#include "types.h"

namespace reone {

void initLog(LogSeverity minSeverity = LogSeverity::Info,
             std::set<LogChannel> channels = std::set<LogChannel> {LogChannel::General},
             std::string filename = "");

void error(const std::string &s, LogChannel channel = LogChannel::General);
void error(const boost::format &s, LogChannel channel = LogChannel::General);
void warn(const std::string &s, LogChannel channel = LogChannel::General);
void warn(const boost::format &s, LogChannel channel = LogChannel::General);
void info(const std::string &s, LogChannel channel = LogChannel::General);
void info(const boost::format &s, LogChannel channel = LogChannel::General);
void debug(const std::string &s, LogChannel channel = LogChannel::General);
void debug(const boost::format &s, LogChannel channel = LogChannel::General);

bool isLogSeverityEnabled(LogSeverity severity);
bool isLogChannelEnabled(LogChannel channel);

} // namespace reone
