#include "triangle_mesh.hpp"

namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		// on test l'intersection avec AABB en premier et on retourne faux si pas d'intersection
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }

		//return _bvh.intersect( p_ray, p_tMin, p_tMax, p_hitRecord );

		float  tClosest = p_tMax;			 // Hit distance.
		float  u		= 0.0f;
		float  v		= 0.0f;
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t, temp_u, temp_v;
			if ( _triangles[ i ].intersect( p_ray, t, temp_u, temp_v ) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					tClosest = t;
					hitTri	 = i;
					u		 = temp_u;
					v		 = temp_v;
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			p_hitRecord._point	= p_ray.pointAtT( tClosest );
			p_hitRecord._normal = _triangles[ hitTri ].getNormalInterpolation( u, v );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }

		//return _bvh.intersectAny( p_ray, p_tMin, p_tMax );

		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t, u, v;
			if ( _triangles[ i ].intersect( p_ray, t, u, v ) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
} // namespace RT_ISICG
