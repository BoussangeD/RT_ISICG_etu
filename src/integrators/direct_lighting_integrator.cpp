#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::_directLighting( const HitRecord & p_hitRecord,
													 const BaseLight * p_light,
													 const Scene &	   p_scene,
													 const float	   p_tMin ) const
	{
		Vec3f		finalColor	= VEC3F_ZERO;
		LightSample lightSample = p_light->sample( p_hitRecord._point );
		Ray			shadowRay( p_hitRecord._point, lightSample._direction );
		shadowRay.offset( p_hitRecord._normal ); // offset pour débruiter

		bool isInShadow = p_scene.intersectAny( shadowRay, p_tMin, lightSample._distance );

		if ( !isInShadow )
		{
			float cosTheta = glm::max( 0.0f, dot( p_hitRecord._normal, lightSample._direction ) );
			Vec3f directContribution = p_hitRecord._object->getMaterial()->getFlatColor() * lightSample._radiance * cosTheta;

			finalColor += directContribution;
		}
		return finalColor;
	}

	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			Vec3f finalColor = VEC3F_ZERO;
			for ( auto light : p_scene.getLights() )
			{
				if ( light->getIsSurface() ) // lancer un rayon d'ombrage en cas de source surfacique
				{
					for ( int i = 0; i < _nbLightSamples; i++ )
					{
						finalColor += _directLighting( hitRecord, light, p_scene, p_tMin );
					}
					finalColor /= _nbLightSamples;
				}
				else { finalColor += _directLighting( hitRecord, light, p_scene, p_tMin ); }
			}
			return finalColor;
		}
		else { return _backgroundColor; }
	}
} // namespace RT_ISICG
