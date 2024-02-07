#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		p_t1 = -1.f;
		p_t2 = -1.f;

		Vec3f rayOriginCenter = p_ray.getOrigin() - _center;

		float a	= glm::dot( p_ray.getDirection(), p_ray.getDirection() );
		float b = 2.0f * glm::dot( rayOriginCenter, p_ray.getDirection() );
		float c = glm::dot( rayOriginCenter, rayOriginCenter ) - _radius * _radius;

		float delta = b * b - 4 * a * c;

		if (delta < 0) {
			return false;
		}

		else {
			if (delta == 0) {
				p_t1 = -b / ( 2.0f );
			}
			else {
				p_t1 = ( -b - glm::sqrt( delta ) ) / ( 2.0f * a );
				p_t2 = ( -b + glm::sqrt( delta ) ) / ( 2.0f * a );
				float p_temp = 0.0f;

				if ( p_t1 > p_t2 ) { 
					p_temp = p_t1;
					p_t1   = p_t2;
					p_t2   = p_temp;
				}
			}

			return true;
		}

		return false;
	}

} // namespace RT_ISICG
