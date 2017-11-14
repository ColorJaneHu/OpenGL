//============================================================
// STUDENT NAME: HU XUEJUN
// NUS User ID.: t0915037
// COMMENTS TO GRADER: 
// 
// 
// ============================================================
//
// FILE: Raytrace.cpp


#include <cmath>
#include <cfloat>
#include "Vector3d.h"
#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Surface.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "Scene.h"
#include "Raytrace.h"

using namespace std;


// This is for avoiding the "epsilon problem" or the shadow acne problem.
#define DEFAULT_TMIN	10e-6		

// Use this for tmax for non-shadow ray intersection test.
#define DEFAULT_TMAX	DBL_MAX



//////////////////////////////////////////////////////////////////////////////
// Compute the outgoing mirror reflection vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output reflection vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
//////////////////////////////////////////////////////////////////////////////

static Vector3d mirrorReflect( const Vector3d &L, const Vector3d &N )
{
	return ( 2.0 * dot( N, L ) ) * N - L;
}



//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computePhongLighting( const Vector3d &L, const Vector3d &N, const Vector3d &V,
								   const Material &mat, const PointLightSource &ptLight )
{
	Vector3d NN = ( dot( L, N ) >= 0.0 )?  N : -N;

	Vector3d R = mirrorReflect( L, NN );
	float NL = (float) dot( NN, L );
	float RVn = pow( (float) dot( R, V ), (float) mat.n );

	return ptLight.I_source * ( mat.k_d * NL  +  mat.k_r * RVn );
}





//////////////////////////////////////////////////////////////////////////////
// Traces a ray into the scene.
// reflectLevels: specfies number of levels of reflections (0 for no reflection).
// hasShadow: specifies whether to generate shadows.
//////////////////////////////////////////////////////////////////////////////

Color Raytrace::TraceRay( const Ray &ray, const Scene &scene, 
					      int reflectLevels, bool hasShadow )
{
	Ray uRay( ray );
	uRay.makeUnitDirection();  // Normalize ray direction.


// Find whether and where the ray hits some surface. 
// Take the nearest hit point.

	bool hasHitSomething = false;
	double nearest_t = DEFAULT_TMAX;
	SurfaceHitRecord nearestHitRec;

	for ( int i = 0; i < scene.numSurfaces; i++ )
	{
		SurfaceHitRecord tempHitRec;
		bool hasHit = scene.surfacep[i]->hit( uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec );

		if ( hasHit && tempHitRec.t < nearest_t )
		{
			hasHitSomething = true;
			nearest_t = tempHitRec.t;
			nearestHitRec = tempHitRec;
		}
	}

	if ( !hasHitSomething ) return scene.backgroundColor;

	nearestHitRec.normal.makeUnitVector();
	Vector3d N = nearestHitRec.normal;	// Unit vector.
	Vector3d V = -uRay.direction();		// Unit vector.

	Color result( 0.0f, 0.0f, 0.0f );	// The result will be accumulated here.


	////////////////////////////////////
//	result = nearestHitRec.mat_ptr->k_d; // REMOVE THIS LINE AFTER YOU HAVE FINISHED CODE BELOW.
	////////////////////////////////////


// Add to result the phong lighting contributed by each point light source.
// Compute shadow if hasShadow is true.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    
    for(int i=0;i<scene.numPtLights;i++){
        
        int isshadow = 1;
        Vector3d new_raydirection = (scene.ptLight[i].position-nearestHitRec.p).unitVector();
        Ray recRay=*new Ray(nearestHitRec.p,new_raydirection);//initiate a ray
        for (int i = 0; i < scene.numSurfaces; i++) {
            if(scene.surfacep[i]->shadowHit(recRay, DEFAULT_TMIN, (scene.ptLight[i].position-nearestHitRec.p).length())) isshadow=0;
        }//check if there exist a surface may be hit, then mark isshadow as 0
        
        //if isshadow is true, leave the result
        //else result will update according to the computePhonfLighting()
        Vector3d L=(scene.ptLight[i].position-nearestHitRec.p).unitVector();
        if(hasShadow)
        result += (computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]))*isshadow;
        if(!hasShadow)
            result += (computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]));
        }
    


// Add to result the global ambient lighting.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************


    result+=scene.amLight.I_a*nearestHitRec.mat_ptr->k_a;


// Add to result the reflection of the scene.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    if (reflectLevels > 0) {
        Vector3d reflectedRayDirc = mirrorReflect(-uRay.direction(), N);
        Ray reflectedRay (nearestHitRec.p , reflectedRayDirc);
        reflectLevels = reflectLevels -1;
        result = result + nearestHitRec.mat_ptr->k_rg * TraceRay(reflectedRay, scene, reflectLevels, hasShadow);//𝐼 = 𝐼𝑙𝑜𝑐𝑎𝑙 + 𝑘𝑟𝑔 𝐼𝑟𝑒𝑓𝑙𝑒𝑐𝑡𝑒𝑑
    }//iteration to get the final result color
    
	return result;
}
