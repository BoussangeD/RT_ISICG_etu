#include "ray_cast_integrator.hpp"

namespace RT_ISICG
{
	Vec3f RayCastIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			Vec3f normal = hitRecord._normal;
			Vec3f rayDir = p_ray.getDirection();

			float cosTheta = glm::max( 0.0f, dot( normal, -rayDir ) );	// cosinus de l'angle entre la normale et la direction du rayon
			return hitRecord._object->getMaterial()->getFlatColor() * cosTheta;
		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG
