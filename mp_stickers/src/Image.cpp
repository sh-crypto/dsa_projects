#include "Image.h" 

void Image::darken() {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.l -= 0.1;
            if (p1.l > 1.0) { p1.l = 1.0; }
            if (p1.l < 0.0) { p1.l = 0.0; }
        }
    }
}

void Image::darken(double amount) {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.l -= amount;
            if (p1.l > 1.0) { p1.l = 1.0; }
            if (p1.l < 0.0) { p1.l = 0.0; }
        }
    }
}

void Image::desaturate() {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.s -= 0.1;
            if (p1.s > 1.0) { p1.s = 1.0; }
            if (p1.s < 0.0) { p1.s = 0.0; }
        }
    }
}

void Image::desaturate(double amount) {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.s -= amount;
            if (p1.s > 1.0) { p1.s = 1.0; }
            if (p1.s < 0.0) { p1.s = 0.0; }
        }
    }
}

void Image::grayscale() {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.s -= 0.0;
        }
    }
}

void Image::illinify() {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.h = (p1.h < 113.5 && p1.h >= 293.5) ? 11 : 216;
        }
    }
}

void Image::lighten() {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.l += 0.1;
            if (p1.l > 1.0) { p1.l = 1.0; }
            if (p1.l < 0.0) { p1.l = 0.0; }
        }
    }
}

void Image::lighten(double amount) {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.l += amount;
            if (p1.l > 1.0) { p1.l = 1.0; }
            if (p1.l < 0.0) { p1.l = 0.0; }
        }
    }
}

void Image::rotateColor(double degrees) {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.h += degrees;
            while (p1.h > 360.0) {
                p1.h -= 360.0;
            }
            while (p1.h < 0.0) {
                p1.h += 360.0;
            }
        }
    }
}

void Image::saturate() {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.s += 0.1;
            if (p1.s > 1.0) { p1.s = 1.0; }
            if (p1.s < 0.0) { p1.s = 0.0; }
        }
    }
}

void Image::saturate(double amount) {
    unsigned int width = PNG::width();
    unsigned int height = PNG::height();
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& p1 = getPixel(x, y);
            p1.s += amount;
            if (p1.s > 1.0) { p1.s = 1.0; }
            if (p1.s < 0.0) { p1.s = 0.0; }
        }
    }
}

void Image::scale(double factor) {
    Image* tmp = new Image();
    *tmp = *this;
    unsigned int width = PNG::width() * factor;
    unsigned int height = PNG::height() * factor;
    resize(width, height);
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            cs225::HSLAPixel& resize_pixel = getPixel(x, y);
            cs225::HSLAPixel& tmp_pixel = tmp->getPixel(x / factor, y / factor);
            resize_pixel = tmp_pixel;
        }
    }
    delete tmp;
}

void Image::scale(unsigned width, unsigned height) {
    double x_factor = ((double)width) / ((double)PNG::width());
    double y_factor = ((double)height) / ((double)PNG::height());
    if (x_factor > y_factor) { scale(y_factor); }
    else { scale(x_factor); }
}
