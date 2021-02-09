/*
 * Copyright (c) 2020-2021 The reone project contributors
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

#include "program.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "../src/common/pathutil.h"

#include "moduleprobe.h"

using namespace std;

using namespace reone::resource;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace reone {

namespace tools {

static const char kConfigFilename[] = "reone-tools.cfg";

Program::Program(int argc, char **argv) : _argc(argc), _argv(argv) {
}

int Program::run() {
    initOptions();
    loadOptions();
    determineGameID();

    switch (_command) {
        case Command::List:
        case Command::Extract:
        case Command::ToJson:
        case Command::ToTga:
        case Command::ToTwoDa:
        case Command::ToGFF:
            initFileTool();
            switch (_command) {
                case Command::List:
                    _tool->list(_target, _keyPath);
                    break;
                case Command::Extract:
                    _tool->extract(_target, _keyPath, _destPath);
                    break;
                case Command::ToJson:
                    _tool->toJson(_target, _destPath);
                    break;
                case Command::ToTga:
                    _tool->toTga(_target, _destPath);
                    break;
                case Command::ToTwoDa:
                    _tool->to2DA(_target, _destPath);
                    break;
                case Command::ToGFF:
                    _tool->toGFF(_target, _destPath);
                    break;
                default:
                    break;
            }
            break;
        case Command::ModuleProbe:
            ModuleProbe().probe(_target, _gamePath, _destPath);
            break;
        default:
            cout << _cmdLineOpts << endl;
            break;
    }

    return 0;
}

void Program::initOptions() {
    _commonOpts.add_options()
        ("game", po::value<string>(), "path to game directory")
        ("dest", po::value<string>(), "path to destination directory");

    _cmdLineOpts.add(_commonOpts).add_options()
        ("help", "print this message")
        ("list", "list file contents")
        ("extract", "extract file contents")
        ("to-json", "convert 2DA, GFF or TLK file to JSON")
        ("to-tga", "convert TPC image to TGA")
        ("to-2da", "convert JSON to 2DA")
        ("to-gff", "convert JSON to GFF")
        ("modprobe", "probe module and produce a JSON file describing it")
        ("target", po::value<string>(), "target name or path to input file");
}

static fs::path getDestination(const po::variables_map &vars) {
    fs::path result;
    if (vars.count("dest") > 0) {
        result = vars["dest"].as<string>();
    } else if (vars.count("target") > 0) {
        result = fs::path(vars["target"].as<string>()).parent_path();
    } else {
        result = fs::current_path();
    }
    return move(result);
}

void Program::loadOptions() {
    po::positional_options_description positional;
    positional.add("target", 1);

    po::parsed_options parsedCmdLineOpts = po::command_line_parser(_argc, _argv)
        .options(_cmdLineOpts)
        .positional(positional)
        .run();

    po::variables_map vars;
    po::store(parsedCmdLineOpts, vars);
    if (fs::exists(kConfigFilename)) {
        po::store(po::parse_config_file<char>(kConfigFilename, _commonOpts), vars);
    }
    po::notify(vars);

    _gamePath = vars.count("game") > 0 ? vars["game"].as<string>() : fs::current_path();
    _destPath = getDestination(vars);
    _target = vars.count("target") > 0 ? vars["target"].as<string>() : "";
    _keyPath = getPathIgnoreCase(_gamePath, "chitin.key");

    if (vars.count("help")) {
        _command = Command::Help;
    } else if (vars.count("list")) {
        _command = Command::List;
    } else if (vars.count("extract")) {
        _command = Command::Extract;
    } else if (vars.count("to-json")) {
        _command = Command::ToJson;
    } else if (vars.count("to-tga")) {
        _command = Command::ToTga;
    } else if (vars.count("to-2da")) {
        _command = Command::ToTwoDa;
    } else if (vars.count("to-gff")) {
        _command = Command::ToGFF;
    } else if (vars.count("modprobe")) {
        _command = Command::ModuleProbe;
    }
}

void Program::determineGameID() {
    fs::path exePath = getPathIgnoreCase(_gamePath, "swkotor2.exe");
    _gameId = exePath.empty() ? GameID::KotOR : GameID::TSL;
}

void Program::initFileTool() {
    switch (_command) {
        case Command::List:
        case Command::Extract:
        case Command::ToJson:
        case Command::ToTga:
        case Command::ToTwoDa:
        case Command::ToGFF:
            if (!fs::exists(_target)) {
                throw runtime_error("Input file does not exist: " + _target);
            }
            _tool = getFileToolByPath(_gameId, _target);
            break;
        default:
            throw logic_error("Unsupported file tool command: " + to_string(static_cast<int>(_command)));
    }
}

} // namespace tools

} // namespace reone
