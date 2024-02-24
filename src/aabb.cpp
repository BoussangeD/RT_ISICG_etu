#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		// Cf. https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
		Vec3f invDir = 1.0f / p_ray.getDirection();

		float tmin = FLT_MIN;
		float tmax = FLT_MAX;

		if ( invDir.x >= 0 )	// intersection avec l'axe x
		{
			tmin = ( _min.x - p_ray.getOrigin().x ) * invDir.x;
			tmax = ( _max.x - p_ray.getOrigin().x ) * invDir.x;
		}
		else
		{
			tmin = ( _max.x - p_ray.getOrigin().x ) * invDir.x;
			tmax = ( _min.x - p_ray.getOrigin().x ) * invDir.x;
		}

		tmin = std::max( tmin, p_tMin );
		tmax = std::min( tmax, p_tMax );

		float tymin, tymax;

		if ( invDir.y >= 0 )	// intersection avec l'axe y
		{
			tymin = ( _min.y - p_ray.getOrigin().y ) * invDir.y;
			tymax = ( _max.y - p_ray.getOrigin().y ) * invDir.y;
		}
		else
		{
			tymin = ( _max.y - p_ray.getOrigin().y ) * invDir.y;
			tymax = ( _min.y - p_ray.getOrigin().y ) * invDir.y;
		}

		if ( ( tmin > tymax ) || ( tymin > tmax ) ) return false;

		tmin = std::max( tymin, tmin );
		tmax = std::min( tymax, tmax );

		float tzmin, tzmax;

		if ( invDir.z >= 0 )	// intersection avec l'axe z
		{
			tzmin = ( _min.z - p_ray.getOrigin().z ) * invDir.z;
			tzmax = ( _max.z - p_ray.getOrigin().z ) * invDir.z;
		}
		else
		{
			tzmin = ( _max.z - p_ray.getOrigin().z ) * invDir.z;
			tzmax = ( _min.z - p_ray.getOrigin().z ) * invDir.z;
		}

		if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return false;

		return true;	// si toutes les intersections sont valides, on retourne vrai
	}

} // namespace RT_ISICG
