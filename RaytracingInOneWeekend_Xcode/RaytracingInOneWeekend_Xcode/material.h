//
//  material.h
//  RaytracingInOneWeekend_Xcode
//
//  Created by SEUNGCHAN on 2020/07/07.
//  Copyright © 2020 SEUNGCHAN. All rights reserved.
//

#pragma once
#include "vec3.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color & attenuation, ray & scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, color & attenuation, ray & scattered) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
//        vec3 scatter_direction = random_in_hemisphere();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
    
public:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1){}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

