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

#include <boost/test/unit_test.hpp>

#include "reone/system/stream/bytearrayinput.h"
#include "reone/system/stringbuilder.h"
#include "reone/graphics/format/bwmreader.h"

using namespace reone;
using namespace reone::graphics;

BOOST_AUTO_TEST_SUITE(bwm_reader)

BOOST_AUTO_TEST_CASE(should_load_pwk_dwk) {
    // given
    auto bwmBytes = StringBuilder()
                        .append("BWM V1.0")
                        .append("\x00\x00\x00\x00", 4) // type
                        .append("\x00\x00\x00\x00", 4) // relative use position 1 (X)
                        .append("\x00\x00\x00\x00", 4) // relative use position 1 (Y)
                        .append("\x00\x00\x00\x00", 4) // relative use position 1 (Z)
                        .append("\x00\x00\x00\x00", 4) // relative use position 2 (X)
                        .append("\x00\x00\x00\x00", 4) // relative use position 2 (Y)
                        .append("\x00\x00\x00\x00", 4) // relative use position 2 (Z)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 1 (X)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 1 (Y)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 1 (Z)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 2 (X)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 2 (Y)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 2 (Z)
                        .append("\x00\x00\x00\x00", 4) // position (X)
                        .append("\x00\x00\x00\x00", 4) // position (Y)
                        .append("\x00\x00\x00\x00", 4) // position (Z)
                        .append("\x00\x00\x00\x00", 4) // vertex count
                        .append("\x00\x00\x00\x00", 4) // offset to vertices
                        .append("\x00\x00\x00\x00", 4) // offset to indices
                        .append("\x00\x00\x00\x00", 4) // offset to materials
                        .append("\x00\x00\x00\x00", 4) // offset to normals
                        .append("\x00\x00\x00\x00", 4) // offset to planar distances
                        .build();
    auto bwm = ByteArrayInputStream(bwmBytes);
    auto reader = BwmReader();

    // when
    reader.load(bwm);

    // then
    auto walkmesh = reader.walkmesh();
}

BOOST_AUTO_TEST_CASE(should_load_wok) {
    // given
    auto wokBytes = StringBuilder()
                        .append("BWM V1.0")
                        .append("\x01\x00\x00\x00", 4) // type
                        .append("\x00\x00\x00\x00", 4) // relative use position 1 (X)
                        .append("\x00\x00\x00\x00", 4) // relative use position 1 (Y)
                        .append("\x00\x00\x00\x00", 4) // relative use position 1 (Z)
                        .append("\x00\x00\x00\x00", 4) // relative use position 2 (X)
                        .append("\x00\x00\x00\x00", 4) // relative use position 2 (Y)
                        .append("\x00\x00\x00\x00", 4) // relative use position 2 (Z)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 1 (X)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 1 (Y)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 1 (Z)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 2 (X)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 2 (Y)
                        .append("\x00\x00\x00\x00", 4) // absolute use position 2 (Z)
                        .append("\x00\x00\x00\x00", 4) // position (X)
                        .append("\x00\x00\x00\x00", 4) // position (Y)
                        .append("\x00\x00\x00\x00", 4) // position (Z)
                        .append("\x00\x00\x00\x00", 4) // number of vertices
                        .append("\x00\x00\x00\x00", 4) // offset to vertices
                        .append("\x00\x00\x00\x00", 4) // offset to indices
                        .append("\x00\x00\x00\x00", 4) // offset to materials
                        .append("\x00\x00\x00\x00", 4) // offset to normals
                        .append("\x00\x00\x00\x00", 4) // offset to planar distances
                        .append("\x00\x00\x00\x00", 4) // position
                        .append("\x00\x00\x00\x00", 4) // number of AABBs
                        .append("\x00\x00\x00\x00", 4) // offset to AABBs
                        .append("\x00\x00\x00\x00", 4) // unknown
                        .append("\x00\x00\x00\x00", 4) // number of adjacencies
                        .append("\x00\x00\x00\x00", 4) // offset to adjacencies
                        .append("\x00\x00\x00\x00", 4) // number of edges
                        .append("\x00\x00\x00\x00", 4) // offset to edges
                        .append("\x00\x00\x00\x00", 4) // number of perimeters
                        .append("\x00\x00\x00\x00", 4) // offset to perimeters
                        .build();
    auto wok = ByteArrayInputStream(wokBytes);
    auto reader = BwmReader();

    // when
    reader.load(wok);

    // then
    auto walkmesh = reader.walkmesh();
}

BOOST_AUTO_TEST_SUITE_END()
