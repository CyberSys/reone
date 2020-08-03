/*
 * Copyright � 2020 Vsevolod Kremianskii
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

#include "random.h"

#include <ctime>
#include <random>

namespace reone {

static std::default_random_engine g_generator(time(nullptr));

float random(float min, float max) {
    std::uniform_real_distribution<float> distr(min, max);
    return distr(g_generator);
}

} // namespace reone
