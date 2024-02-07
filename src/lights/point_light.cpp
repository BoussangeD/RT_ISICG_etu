#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point ) const 
	{
		float distance = glm::distance( _position, p_point );
		Vec3f radiance	= ( _color * _power ) / ( distance * distance );	// inverse de la distance entre le point d'éclairage et la source lumineuse au carré
		Vec3f direction = normalize( _position - p_point );

		return LightSample( direction, distance, radiance, 1.0f );
	}
} // namespace RT_ISICG