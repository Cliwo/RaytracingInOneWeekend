//
//  perlin.h
//  RaytracingInOneWeekend_Xcode
//
//  Created by SEUNGCHAN on 2020/08/06.
//  Copyright © 2020 SEUNGCHAN. All rights reserved.
//

#ifndef perlin_h
#define perlin_h

#include "rtweekend.h"

class perlin {
public:
    perlin(){
        ranfloat = new double[point_count];
        for(int i = 0 ; i < point_count; ++i) {
            ranfloat[i] = random_double();
        }
        
        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }
    
    ~perlin() {
        delete[] ranfloat;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }
    
    double noise(const point3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());

        auto i = static_cast<int>(4*p.x()) & 255;
        //255면 1byte, &를 하게되면 255초과의 숫자는 255로 고정된다. (엥 이러면 uniform 한 random이 아닌거 같은데)
        //255 is 1byte. after & is done, one that is bigger thann 255 goes to 255. (? In this case this random is not uniform)
        auto j = static_cast<int>(4*p.y()) & 255;
        auto k = static_cast<int>(4*p.z()) & 255;

        return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }
    
private:
    static const int point_count = 256;
    double * ranfloat;
    int* perm_x;
    int* perm_y;
    int* perm_z;
    
    static int* perlin_generate_perm() {
        auto p = new int[point_count];
        
        for(int i = 0 ; i < perlin::point_count; i++)
            p[i] = i;
        
        permute(p, point_count);
        
        return p;
    }
    
    static void permute(int* p , int n) {
        for (int i = n-1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }
};


#endif /* perlin_h */
