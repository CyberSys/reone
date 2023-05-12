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

#include "reone/tools/ssf.h"

#include "reone/system/exception/validation.h"
#include "reone/system/logutil.h"
#include "reone/system/stream/fileinput.h"
#include "reone/game/format/ssfreader.h"
#include "reone/game/format/ssfwriter.h"

#include "tinyxml2.h"

using namespace std;

using namespace tinyxml2;

using namespace reone::game;

namespace reone {

void SsfTool::invoke(
    Operation operation,
    const boost::filesystem::path &input,
    const boost::filesystem::path &outputDir,
    const boost::filesystem::path &gamePath) {

    return invokeBatch(operation, vector<boost::filesystem::path> {input}, outputDir, gamePath);
}

void SsfTool::invokeBatch(
    Operation operation,
    const std::vector<boost::filesystem::path> &input,
    const boost::filesystem::path &outputDir,
    const boost::filesystem::path &gamePath) {

    return doInvokeBatch(input, outputDir, [this, &operation](auto &path, auto &outDir) {
        if (operation == Operation::ToXML) {
            toXML(path, outDir);
        } else if (operation == Operation::ToSSF) {
            toSSF(path, outDir);
        }
    });
}

void SsfTool::toXML(const boost::filesystem::path &path, const boost::filesystem::path &destPath) {
    auto stream = FileInputStream(path, OpenMode::Binary);

    auto reader = SsfReader();
    reader.load(stream);

    auto soundSet = reader.soundSet();

    auto xmlPath = destPath;
    xmlPath.append(path.filename().string() + ".xml");
    auto fp = fopen(xmlPath.string().c_str(), "wb");

    auto printer = XMLPrinter(fp);
    printer.PushHeader(false, true);
    printer.OpenElement("soundset");
    for (size_t i = 0; i < soundSet.size(); ++i) {
        printer.OpenElement("sound");
        printer.PushAttribute("index", static_cast<int>(i));
        printer.PushAttribute("strref", soundSet[i]);
        printer.CloseElement();
    }
    printer.CloseElement();

    fclose(fp);
}

void SsfTool::toSSF(const boost::filesystem::path &path, const boost::filesystem::path &destPath) {
    auto fp = fopen(path.string().c_str(), "rb");

    auto document = XMLDocument();
    document.LoadFile(fp);

    auto rootElement = document.RootElement();
    if (!rootElement) {
        cerr << "XML is empty" << endl;
        fclose(fp);
        return;
    }

    auto soundSet = vector<uint32_t>();
    for (auto element = rootElement->FirstChildElement(); element; element = element->NextSiblingElement()) {
        auto strref = element->UnsignedAttribute("strref");
        soundSet.push_back(strref);
    }

    vector<string> tokens;
    boost::split(
        tokens,
        path.filename().string(),
        boost::is_any_of("."),
        boost::token_compress_on);

    auto ssfPath = boost::filesystem::path(destPath);
    ssfPath.append(tokens[0] + ".ssf");

    auto writer = SsfWriter(move(soundSet));
    writer.save(ssfPath);
}

bool SsfTool::supports(Operation operation, const boost::filesystem::path &input) const {
    return (operation == Operation::ToXML && input.extension() == ".ssf") ||
           (operation == Operation::ToSSF && input.extension() == ".xml");
}

} // namespace reone
