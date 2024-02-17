#include "quad_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f randomPos = Vec3f( _position + _u * randomFloat() + _v * randomFloat() ); // position aléatoire sur le quad
		float distance	= glm::distance( randomPos, p_point ); // distance entre le rayon et le point observé
		Vec3f direction = normalize( randomPos - p_point );	   // direction du rayon entre le point observé et la position sur le quad.

		float pdf	   = ( ( 1 / _area ) * ( distance * distance ) ) / dot( _n, direction );
		Vec3f radiance = ( _color * _power ) / pdf;
		return LightSample( direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG