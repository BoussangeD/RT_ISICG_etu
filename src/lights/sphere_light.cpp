#include "sphere_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	LightSample SphereLight::sample( const Vec3f & p_point ) const
	{
		// Cf. https://pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/Sampling_Light_Sources
		
		// distribution uniforme
		float theta = 2.0f * PIf * randomFloat();
		float phi	= PIf * randomFloat(); 

		// Cours 3 Moteur3D
		Vec3f randomPos = _center + _radius * Vec3f( cos( theta ) * sin( phi ), cos( phi ), sin( theta ) * sin( phi ) );
		float distance	= glm::distance( randomPos, p_point );
		Vec3f direction = normalize( randomPos - p_point );

		Vec3f normal	= normalize( _center - randomPos );
		float cosThetaI = dot( direction, normal );

		// on ajuste le cosinus de l'angle theta selon le sens du rayon par rapport à la normale
		if ( cosThetaI < 0.0f )
		{
			cosThetaI = -cosThetaI;
		}

		float pdf = ( distance * distance ) / ( cosThetaI * _area );
		Vec3f radiance = ( _color * _power ) / pdf;
		return LightSample( direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG
