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

#pragma once

#include <vector>

#include "control.h"

namespace reone {

namespace gui {

static const int kDefaultSlotCount = 6;

class ListBox : public Control {
public:
    struct Item {
        std::string tag;
        std::string text;
    };

    ListBox();
    ListBox(const std::string &tag);

    void loadCustom();
    void add(const Item &item);

    void load(const resources::GffStruct &gffs) override;
    bool handleMouseMotion(int x, int y) override;
    bool handleMouseWheel(int x, int y) override;
    bool handleClick(int x, int y) override;
    void initGL() override;
    void render(const glm::mat4 &transform, const std::string &textOverride) const override;
    void resize(float scaleX, float scaleY) override;

    void setFocus(bool focus) override;
    void setExtent(const Extent &extent) override;

    Control &protoItem() const;
    Control &scrollBar() const;

private:
    std::shared_ptr<Control> _protoItem;
    std::shared_ptr<Control> _scrollBar;
    std::vector<Item> _items;
    int _slotCount { 0 };
    int _itemOffset { 0 };
    int _hilightedIndex { -1 };

    void updateItems();
    int getItemIndex(int y) const;
};

} // namespace gui

} // namespace reone
