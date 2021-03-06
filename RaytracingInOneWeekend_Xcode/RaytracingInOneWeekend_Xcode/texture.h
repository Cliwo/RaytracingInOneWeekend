//
//  texture.h
//  RaytracingInOneWeekend_Xcode
//
//  Created by SEUNGCHAN on 2020/07/19.
//  Copyright © 2020 SEUNGCHAN. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include "rtweekend.h"

#define STB_IMAGE_IMPLEMENTATION
//https://stackoverflow.com/questions/24088002/stb-image-h-in-visual-studio-unresolved-external-symbol
#include "rtw_stb_image.h"
#include "perlin.h"

#include <iostream>

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue)
      : solid_color(color(red,green,blue)) {}

    virtual color value(double u, double v, const vec3& p) const {
        return color_value;
    }
private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) : even(t0), odd(t1) {}
    checker_texture(color c1, color c2)
    : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)) {}
    
    virtual color value(double u, double v, const vec3& p) const {
        auto coef = 10;
        auto sines = sin(coef*p.x()) * sin(coef*p.y()) * sin(coef*p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u,v,p);
    }
    
public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(double sc) : scale(sc) {}
    virtual color value(double u, double v, const vec3& p) const {
        return color(1,1,1) * 0.5 * (1 + sin(scale*p.z() + 10*noise.turb(p)));
    }
public:
    perlin noise;
    double scale;
};

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;
    
    image_texture()
    : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
    
    image_texture(const char * filename) {
        auto components_per_pixel = bytes_per_pixel;
        
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
        if(!data) {
            std::cerr << "ERROR: Could not load texture image file '"<< filename << "'.\n'";
            width = height = 0;
        }
        
        bytes_per_scanline = bytes_per_pixel * width;
    }
    
    ~image_texture() {
        delete data;
    }
    
    virtual color value(double u, double v, const vec3& p) const override {
        if (data == nullptr)
            return color(0, 1, 1);
        
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates
        
        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);
        
        if (i >= width) i = width-1;
        if (j >= height) j = height-1;
        
        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;
        
        color val = color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        return val;
    }
private:
    unsigned char * data;
    int width, height;
    int bytes_per_scanline;
};

#endif /* texture_h */
