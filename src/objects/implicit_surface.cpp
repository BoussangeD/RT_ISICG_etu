#include "implicit_surface.hpp"

namespace RT_ISICG
{
	// Cf. https://graphics.stanford.edu/courses/cs348b-20-spring-content/uploads/hart.pdf (page 12)
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float t = p_tMin;	
		float d = 0;		// distance
		Vec3f point_t = VEC3F_ZERO;	

		while ( t < p_tMax )
		{
			point_t = p_ray.pointAtT( t );
			d = _sdf( point_t );

			if ( d < 0.0001f )
			{
				p_hitRecord._distance = d;
				p_hitRecord._normal	  = _evaluateNormal( point_t );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._point	= point_t;
				p_hitRecord._object = this;
				return true;
			}

			t += d;
		}

		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t		  = p_tMin;
		float d		  = 0;	// distance
		Vec3f point_t = VEC3F_ZERO;

		while ( t < p_tMax ) {
			point_t = p_ray.pointAtT( t );
			d		= _sdf( point_t );

			if ( d < 0.0001f ) { 
				return true;
			}

			t += d;
		}

		return false;
	}
} // namespace RT_ISICG
