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

#include "reone/resource/format/tlkwriter.h"

#include "reone/system/binarywriter.h"
#include "reone/system/stream/fileoutput.h"

#include "reone/resource/talktable.h"

using namespace std;

namespace reone {

namespace resource {

struct StringFlags {
    static constexpr int textPresent = 1;
    static constexpr int soundPresent = 2;
    static constexpr int soundLengthPresent = 4;
};

void TlkWriter::save(const boost::filesystem::path &path) {
    auto tlk = FileOutputStream(path, OpenMode::Binary);
    save(tlk);
}

void TlkWriter::save(IOutputStream &out) {
    vector<StringDataElement> strData;

    uint32_t offString = 0;
    for (int i = 0; i < _talkTable.getStringCount(); ++i) {
        auto &str = _talkTable.getString(i);
        auto strSize = static_cast<uint32_t>(str.text.length());

        StringDataElement strDataElem;
        strDataElem.soundResRef = str.soundResRef;
        strDataElem.offString = offString;
        strDataElem.stringSize = strSize;
        strData.push_back(std::move(strDataElem));

        offString += strSize;
    }

    BinaryWriter writer(out);
    writer.putString("TLK V3.0");
    writer.putUint32(0); // language id
    writer.putUint32(_talkTable.getStringCount());
    writer.putUint32(20 + 40 * _talkTable.getStringCount()); // offset to string entries

    for (int i = 0; i < _talkTable.getStringCount(); ++i) {
        const StringDataElement &strDataElem = strData[i];
        writer.putUint32(7); // flags

        string soundResRef(strDataElem.soundResRef);
        soundResRef.resize(16);
        writer.putString(soundResRef);

        writer.putUint32(0); // volume variance
        writer.putUint32(0); // pitch variance
        writer.putUint32(strDataElem.offString);
        writer.putUint32(strDataElem.stringSize);
        writer.putFloat(0.0f); // sound length
    }

    for (int i = 0; i < _talkTable.getStringCount(); ++i) {
        writer.putString(_talkTable.getString(i).text);
    }
}

} // namespace resource

} // namespace reone
