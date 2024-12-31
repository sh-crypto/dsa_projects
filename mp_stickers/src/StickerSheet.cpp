#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image& picture) {
    base_ = Image(picture);
}

int StickerSheet::addSticker(Image& sticker, int x, int y) {
    for (int i = 0; i < (static_cast<int>(stickers_.size())); i++) {
        if (stickers_.at(i) == nullptr) {
            stickers_[i] = &sticker;
            x_[i] = (x);
            y_[i] = (y);
            return i;
        }
    }
    stickers_.push_back(&sticker);
    x_.push_back(x);
    y_.push_back(y);
    return stickers_.size() - 1;
}


int StickerSheet::layers() const { return static_cast<int>(stickers_.size()); }

int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {
    if (layer >= stickers_.size()) {
        stickers_.push_back(&sticker);
        x_.push_back(x);
        y_.push_back(y);
        return stickers_.size() - 1;
    }
    stickers_[layer] = &sticker;
    x_[layer] = x;
    y_[layer] = y;
    return layer;
}

bool StickerSheet::translate(unsigned index, int x, int y) {
    int num = index;
    if (index < 0 || num > (static_cast<int>(stickers_.size()))) {
        return false;
    }
    x_[index] = x;
    y_[index] = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
    int num = index;
    if (num >= 0 && num < (static_cast<int>(stickers_.size()))) {
        stickers_.erase(stickers_.begin() + index);
        x_.erase(x_.begin() + index);
        y_.erase(y_.begin() + index);
    }
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index >= (static_cast<unsigned>(stickers_.size())) || (static_cast<unsigned>(stickers_.size())) == 0) {
        return NULL;
    }
    return stickers_[index];
}


Image StickerSheet::render() const {
    signed int left = 0; //negative x's
    signed int right = base_.width(); //positive x's
    signed int down = base_.height(); //positive ys
    signed int up = 0; //negative y's
    for (int i = 0; i < (static_cast<int>(stickers_.size())); i++) {
        if (!stickers_[i]) continue;
        if(x_[i] < 0) {
           if (left < abs(x_[i])) { left = abs(x_[i]); }
        }
        if (y_[i] < 0) {
            if (up < abs(y_[i])) { up = abs(y_[i]); }
        }
        int width_sticker = stickers_[i]->width();
        int height_sticker = stickers_[i]->height();
        if (right < (width_sticker + x_[i])) { right = (width_sticker + x_[i]); }
        if (down < (height_sticker + y_[i])) { down = (height_sticker + y_[i]); }
    }
    int x_offset = (left);
    int y_offset = (up);
    Image blank_canvas = Image();
    blank_canvas.resize(right + (left), down + (up));
    Image render = base_;
    for (unsigned x = 0; x < base_.width(); x++) {
        for (unsigned y = 0; y < base_.height(); y++) {
            cs225::HSLAPixel& canvas_pixel = blank_canvas.getPixel(x + x_offset, y + y_offset);
            cs225::HSLAPixel& base_pixel = render.getPixel(x, y);
            canvas_pixel = base_pixel;
        }
    }
    for (int i = 0; i < (static_cast<int>(stickers_.size())); i++) {
        if (!stickers_[i]) continue;
        for (unsigned x = 0; x < stickers_[i]->width(); x++) {
            for (unsigned y = 0; y < stickers_[i]->height(); y++) {
                cs225::HSLAPixel& image_pixel = blank_canvas.getPixel(x + (x_[i]) + x_offset, y + (y_[i]) + y_offset);
                cs225::HSLAPixel& sticker_pixel = stickers_[i]->getPixel(x, y);
                if (sticker_pixel.a != 0) { image_pixel = sticker_pixel; }
            }
        }
    }
    return blank_canvas;
}