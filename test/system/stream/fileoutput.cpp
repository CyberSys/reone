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

#include "reone/system/stream/fileoutput.h"

#include "../../checkutil.h"

using namespace reone;

TEST(file_output_stream, should_write_to_file) {
    // given

    auto tmpPath = boost::filesystem::temp_directory_path();
    tmpPath.append("reone_test_file_output");

    auto stream = FileOutputStream(tmpPath, OpenMode::Binary);
    auto bytesToWrite = ByteArray("Hello, world!");
    auto expectedOutput = ByteArray("Hello, world!\nHello, world!");

    // when

    stream.write(bytesToWrite);
    auto position = stream.position();
    stream.writeByte('\n');
    stream.writeByte('H');
    stream.write(&bytesToWrite[1], 12);
    stream.close();

    // then

    auto tmpFile = boost::filesystem::ifstream(tmpPath, std::ios::binary);
    tmpFile.seekg(0, std::ios::end);
    auto tmpSize = tmpFile.tellg();
    auto bytes = ByteArray();
    bytes.resize(tmpSize);
    tmpFile.seekg(0);
    tmpFile.read(&bytes[0], tmpSize);
    tmpFile.close();

    EXPECT_EQ(13ll, position);
    EXPECT_EQ(27ll, tmpSize);
    EXPECT_EQ(expectedOutput, bytes) << notEqualMessage(expectedOutput, bytes);

    // cleanup

    boost::filesystem::remove(tmpPath);
}
