#include "whitted_integrator.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		return trace( p_scene, p_ray, p_tMin, p_tMax, 0, false );
	}

	Vec3f WhittedIntegrator::_directLighting( const HitRecord & p_hitRecord,
													 const Ray &	   p_ray,
													 const BaseLight * p_light,
													 const Scene &	   p_scene,
													 const float	   p_tMin ) const
	{
		Vec3f		finalColor	= VEC3F_ZERO;
		LightSample lightSample = p_light->sample( p_hitRecord._point );
		Ray			shadowRay	= Ray( p_hitRecord._point, lightSample._direction );
		shadowRay.offset( p_hitRecord._normal ); // offset pour débruiter

		bool isInShadow = p_scene.intersectAny( shadowRay, p_tMin, lightSample._distance );

		if ( !isInShadow )
		{
			float cosTheta = glm::max( 0.0f, dot( p_hitRecord._normal, lightSample._direction ) );
			Vec3f directContribution = p_hitRecord._object->getMaterial()->shade( p_ray, p_hitRecord, lightSample )
									   * lightSample._radiance * cosTheta;

			finalColor += directContribution;
		}
		return finalColor;
	}

	Vec3f WhittedIntegrator::trace( const Scene & p_scene,
									const Ray &	  p_ray,
									const float	  p_tMin,
									const float	  p_tMax,
									int			  p_nbBounces,
									bool		  p_inObject ) const
	{
		if ( p_nbBounces >= _nbBounces ) return BLACK; // lorsqu'on dépasse le nombre de rebonds, on retourne la couleur noire

		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getMaterial()->isMirror() ) // partie mirroir
			{
				// calcul de la direction du rayon réfléchi
				Vec3f reflectedDirection = glm::reflect( p_ray.getDirection(), hitRecord._normal );

				Ray reflectedRay = Ray( hitRecord._point, reflectedDirection );
				reflectedRay.offset( hitRecord._normal );

				// appel récursif de trace pour calculer la couleur réfléchie
				return trace( p_scene, reflectedRay, p_tMin, p_tMax, p_nbBounces + 1, p_inObject );
			}

			// Cf. https://en.wikipedia.org/wiki/Fresnel_equations
			else if ( hitRecord._object->getMaterial()->isTransparent() ) // partie transparente 
			{
				// calcul du rayon réfléchi
				Ray reflectRay = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflectRay.offset( hitRecord._normal );

				float iorI = p_inObject ? hitRecord._object->getMaterial()->getIOR() : 1.0f;
				float iorT = p_inObject ? 1.0f : hitRecord._object->getMaterial()->getIOR();

				float cosThetaI = dot( hitRecord._normal, -p_ray.getDirection() );
				float sinThetaI = sqrt( 1 - cosThetaI * cosThetaI );
				float sinThetaT = iorI / iorT * sinThetaI;

				if ( sinThetaT >= 1.0 )	// permet de vérifier la réflexion totale
				{
					return trace( p_scene, reflectRay, p_tMin, p_tMax, p_nbBounces + 1, p_inObject );
				}

				float cosThetaT = sqrt( 1 - sinThetaT * sinThetaT );

				float Rs = ( ( iorI * cosThetaI ) - ( iorT * cosThetaT ) ) / ( ( iorI * cosThetaI ) + ( iorT * cosThetaT ) );
				float Rp = ( ( iorI * cosThetaT ) - ( iorT * cosThetaI ) ) / ( ( iorI * cosThetaT ) + ( iorT * cosThetaI ) );

				float kr = ( Rs * Rs + Rp * Rp ) / 2.0f;	// proportion de lumière réfléchie

				// calcul du rayon réfracté
				Ray	  refractRay = Ray( hitRecord._point, glm::refract( p_ray.getDirection(), hitRecord._normal, iorI / iorT ) );
				refractRay.offset( -hitRecord._normal );

				return ( 1 - kr ) * trace( p_scene, refractRay, p_tMin, p_tMax, p_nbBounces + 1, !p_inObject )
						+ kr * trace( p_scene, reflectRay, p_tMin, p_tMax, p_nbBounces + 1, p_inObject );
			}

			else  // partie éclairage direct 
			{
				// Si le rayon intersecte un objet dans la scène
				Vec3f finalColor = VEC3F_ZERO;

				// Calcul de l'éclairage direct
				for ( auto light : p_scene.getLights() )
				{
					if ( light->getIsSurface() )
					{
						for ( int i = 0; i < _nbLightSamples; i++ )
						{
							finalColor += _directLighting( hitRecord, p_ray, light, p_scene, p_tMin );
						}
						finalColor /= _nbLightSamples;
					}
					else { finalColor += _directLighting( hitRecord, p_ray, light, p_scene, p_tMin ); }
				}

				return finalColor;
			}
		}
		else { return _backgroundColor; }
	}

} // namespace RT_ISICG
