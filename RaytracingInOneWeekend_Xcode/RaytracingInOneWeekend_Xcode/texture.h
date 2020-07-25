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

class checkter_texture : public texture {
public:
    checkter_texture() {}
    checkter_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) : even(t0), odd(t1) {}
    checkter_texture(color c1, color c2)
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

#endif /* texture_h */