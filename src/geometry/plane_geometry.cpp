#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t1 ) const
	{
		// cours FIG
		float den = glm::dot( _normal, p_ray.getDirection() );
		if (den != 0.0f) {
			Vec3f po = _point - p_ray.getOrigin();
			p_t1	 = glm::dot( po, _normal ) / den;
			return p_t1 > 0 ? true : false;	// si l'intersection est devant
		}
		return false;	// pas d'intersection sinon
	}
} // namespace RT_ISICG
