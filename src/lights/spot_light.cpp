#include "spot_light.hpp"

namespace RT_ISICG
{
	// Cf. https://www.pbr-book.org/3ed-2018/Light_Sources/Point_Lights#Spotlights
	LightSample SpotLight::sample( const Vec3f & p_point ) const
	{
		Vec3f lightDirection = normalize( _position - p_point );
		float cosTheta = dot( lightDirection, _direction );

		float falloff = lightFallOff( cosTheta );	// décroissance de la lumière en fonction de l'angle

		// contribution de la lumière
		if ( falloff > 0.0f )
		{
			float distance = glm::distance( _position, p_point );
			Vec3f radiance = ( _color * _power ) / ( distance * distance ) * falloff;
			return LightSample( lightDirection, distance, radiance, 1.0f );
		}
		else
		{
			// si le point est en dehors de l'angle d'ouverture du spot on ne renvoi pas de lumière
			return LightSample( VEC3F_ZERO, 0.0f, VEC3F_ZERO, 0.0f );
		}
	}

	float SpotLight::lightFallOff( float cosTheta ) const
	{
		if ( cosTheta < _cosFalloffStart ) return 0.0f;
		if ( cosTheta >= _spotAngleWidth ) return 1.0f;

		// calculer la transition en douceur
		float delta = ( cosTheta - _cosFalloffStart ) / ( _spotAngleWidth - _cosFalloffStart );
		return ( delta * delta ) * ( delta * delta );
	}
} // namespace RT_ISICG
