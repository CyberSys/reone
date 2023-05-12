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

#include "reone/resource/provider/folder.h"

#include "reone/resource/typeutil.h"

using namespace std;

namespace reone {

namespace resource {

void Folder::init() {
    loadDirectory(_path);
}

void Folder::loadDirectory(const boost::filesystem::path &path) {
    for (auto &entry : boost::filesystem::directory_iterator(path)) {
        const boost::filesystem::path &childPath = entry.path();
        if (boost::filesystem::is_directory(childPath)) {
            loadDirectory(childPath);
            continue;
        }

        string resRef(childPath.filename().replace_extension("").string());
        boost::to_lower(resRef);

        string ext(childPath.extension().string().substr(1));
        boost::to_lower(ext);

        Resource res;
        res.path = childPath;
        res.type = getResTypeByExt(ext);

        _resources.insert(make_pair(resRef, res));
    }
}

shared_ptr<ByteArray> Folder::find(const ResourceId &id) {
    boost::filesystem::path path;
    for (auto &res : _resources) {
        if (res.first == id.resRef && res.second.type == id.type) {
            path = res.second.path;
            break;
        }
    }
    if (path.empty()) {
        return shared_ptr<ByteArray>();
    }
    boost::filesystem::ifstream in(path, ios::binary);

    in.seekg(0, ios::end);
    size_t size = in.tellg();

    in.seekg(ios::beg);
    ByteArray data(size, '\0');
    in.read(&data[0], size);

    return make_shared<ByteArray>(move(data));
}

} // namespace resource

} // namespace reone
