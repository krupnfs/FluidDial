// Copyright (c) 2023 - Mitch Bradley
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Menu.h"
#include "System.h"
#include "Drawing.h"

void do_nothing(void* foo) {}

void RoundButton::show(const Point& where) {
    drawOutlinedCircle(where, _radius, _highlighted ? _hl_fill_color : _fill_color, _highlighted ? _hl_outline_color : _outline_color);
    text(name().substr(0, 1), where, _highlighted ? MAROON : WHITE, MEDIUM);
}
void ImageButton::show(const Point& where) {
#ifdef LGFX_ESP32_3248S035
    Point whereCircle = Point(where.x, where.y - 11);
#else
    Point whereCircle = Point(where.x, where.y);
#endif
    if (_highlighted) {
        drawFilledCircle(whereCircle, _radius + 3, _disabled ? DARKGREY : _outline_color);
    } else {
        drawFilledCircle(whereCircle, _radius - 2, _disabled ? DARKGREY : LIGHTGREY);
    }
    drawPngFile(_filename, where);
}
void RectangularButton::show(const Point& where) {
    drawOutlinedRect(where, _width, _height, _highlighted ? BLUE : _outline_color, _bg_color);
    text(_text, where, _text_color, SMALL);
}

void Menu::removeAllItems() {
    for (auto const& item : _items) {
        delete item;
    }
    _items.clear();
    _positions.clear();
    _num_items = 0;
}

void Menu::reDisplay() {
    menuBackground();
    show_items();
    refreshDisplay();
}
void Menu::rotate(int delta) {
    if (_selected != -1) {
        _items[_selected]->unhighlight();
    }

    int previous = _selected;
    do {
        _selected += delta;
        while (_selected < 0) {
            _selected += _num_items;
        }
        while (_selected >= _num_items) {
            _selected -= _num_items;
        }
        if (!_items[_selected]->hidden()) {
            break;
        }
        // If we land on a hidden item, move to the next item in the
        // same direction.
        delta = delta < 0 ? -1 : 1;
    } while (_selected != previous);

    _items[_selected]->highlight();
    reDisplay();
}
