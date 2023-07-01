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

#include <gtest/gtest.h>

#include "reone/system/stream/bytearrayoutput.h"
#include "reone/system/textwriter.h"

#include "../checkutil.h"

using namespace reone;

TEST(text_writer, should_write_text) {
    // given
    auto expectedBytes = ByteArray {'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n', 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};

    // when
    auto bytes = ByteArray();
    auto stream = ByteArrayOutputStream(bytes);
    auto text = TextWriter(stream);
    text.putLine("Hello, world!");
    text.put("Hello, world!");

    // then
    EXPECT_EQ(expectedBytes, bytes) << notEqualMessage(expectedBytes, bytes);
}
