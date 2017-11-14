//============================================================
// STUDENT NAME:HU XUEJUN 
// NUS User ID.:t0915037
// COMMENTS TO GRADER: 
// 
// 
// ============================================================
//
// FILE: Sphere.cpp

#include <algorithm>
#include <cmath>
#include "Sphere.h"

using namespace std;



bool Sphere::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    double a, b, c, d, t, t0, t1;
    Vector3d rayOrigin = r.origin() - center;
    
    //at^2 + bt + c = 0
    a = 1;
    b = dot(r.direction(), rayOrigin)*2;
    c = dot(rayOrigin, rayOrigin) - radius * radius;
    
    //d = b^2 - 4ac
    d = pow(b ,2) - 4 *a *c;
    
    if (d < 0) {
        return false;
    }
    //get the nearest t value
    t0 = (-b + pow(d,0.5)) /(2 * a);
    t1 = (-b - pow(d,0.5)) /(2 * a);
    
    if (t0 >= tmin && t1 >= tmin && t0 <= tmax && t1 <= tmax) {
        t = min(t0, t1);
    }
    
    else if(t0 < tmin && t1 < tmin){
        return false;
    }
    
    else if (t0 < tmin && t1 >= tmin &&t1 <= tmax) {
        t = t1;
    }
    else if (t0 >= tmin && t0 <= tmax && t1 > tmax) {
        t = t0;
    }
    
    rec.t = t;
    rec.p = r.pointAtParam(t);
    Vector3d pointToCenter = rec.p - center;
    rec.normal = pointToCenter / pointToCenter.length();
    rec.mat_ptr = matp;
    
    return true;
}




bool Sphere::shadowHit( const Ray &r, double tmin, double tmax ) const
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    double a, b, c, d, t, t0, t1;
    Vector3d rayOri = r.origin() - center;
    
    //at^2 + bt + c = 0
    a = 1;
    b = dot(r.direction(), rayOri) * 2;
    c = dot(rayOri, rayOri) - radius * radius;
    
    //d = b^2 - 4ac
    d = pow(b, 2) - 4 * a *c;
    
    if (d < 0) {
        return false;
    }
    //get the nearest t value
    t0 = (-b + pow(d, 0.5)) / (2 * a);
    t1 = (-b - pow(d, 0.5)) / (2 * a);
    
    if (t0 < tmin && t1 < tmin) {
        return false;
    }
    else if (t0 >= tmin && t1 <= tmin) {
        t = t0;
    }
    else if (t0 <= tmin && t1 >= tmin) {
        t = t1;
    }
    else {
        t = min(t0, t1);
    }
    if (t <= tmax) {
        return true;
    }
    else {
        return false;
    }
}

