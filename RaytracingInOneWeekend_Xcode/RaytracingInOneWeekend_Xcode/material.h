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
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere() );
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

//이거 schlick이 뭘 근사하는거였지? 수식으로 정확히 복습하고 넘어가자
double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0) * pow((1 - cosine), 5);
}

class dielectric : public material {
public :
    dielectric(double ri) : ref_idx(ri) {}
    
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        attenuation = color(1.0, 1.0, 1.0);// ? if water is dyed in blue, should I change this value?
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx); //The medium is always air before ray hits surface?
        
        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        
        if(etai_over_etat * sin_theta > 1.0) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        double reflect_prob = schlick(cos_theta, etai_over_etat);
        if(random_double() < reflect_prob)
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }
    
    double ref_idx;
};

