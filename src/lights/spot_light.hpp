#ifndef __RT_ISICG_SPOT_LIGHT__
#define __RT_ISICG_SPOT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class SpotLight : public BaseLight
	{
	  public:
		SpotLight( const Vec3f & p_position,
				   const Vec3f & p_direction,
				   const Vec3f & p_color,
				   const float	 p_power,
				   const float	 p_spotAngleWidth,
				   const float	 p_cosFalloffStart )
			: BaseLight( p_color, p_power ), _position( p_position ), _direction( normalize( p_direction ) ),
			  _spotAngleWidth( p_spotAngleWidth ), _cosFalloffStart( p_cosFalloffStart )
		{
			_isSurface = false;
		}
		virtual ~SpotLight() = default;

		virtual LightSample sample( const Vec3f & p_point ) const;

	  private:
		float lightFallOff( float cosTheta ) const;

		Vec3f _position	 = VEC3F_ZERO;
		Vec3f _direction = VEC3F_ZERO;
		float _spotAngleWidth  = 0.9f;	// angle du cône
		float _cosFalloffStart = 0.7f;	// angle qui débute la décroissance de lumière
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPOT_LIGHT__
