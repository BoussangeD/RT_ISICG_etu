#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		QuadLight( const Vec3f & p_position,
				   const Vec3f & p_u,
				   const Vec3f & p_v,	
				   const Vec3f & p_color,
				   const float	 p_power )
			: BaseLight( p_color, p_power ), _position( p_position ), _u( p_u ), _v( p_v ), 
			_n( normalize( cross( p_u, p_v ) ) ), _area( glm::length( p_u ) * glm::length( p_v ) )	// length pour la taille des vecteurs
		{}
		virtual ~QuadLight() = default;

		virtual LightSample sample( const Vec3f & p_point ) const;

	  private:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _u		= VEC3F_ZERO;	// arete coin _u
		Vec3f _v		= VEC3F_ZERO;	// arete coin _v
		Vec3f _n		= VEC3F_ZERO;	// normale
		float _area		= 0.0f;			// aire du quad
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_QUAD_LIGHT__
