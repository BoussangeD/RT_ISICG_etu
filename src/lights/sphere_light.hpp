#ifndef __RT_ISICG_SPHERE_LIGHT__
#define __RT_ISICG_SPHERE_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class SphereLight : public BaseLight
	{
	  public:
		SphereLight( const Vec3f & p_center, const float p_radius, const Vec3f & p_color, const float p_power )
			: BaseLight( p_color, p_power ), _center( p_center ), _radius( p_radius ),
			  _area( 4.0f * PIf * ( p_radius * p_radius ) )
		{
			_isSurface = true;
		}
		virtual ~SphereLight() = default;

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  protected:
		Vec3f		_center = VEC3F_ZERO;
		const float _radius;
		const float _area;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE_LIGHT__