#ifndef __RT_ISICG_IMPLICIT_DEATH_STAR__
#define __RT_ISICG_IMPLICIT_DEATH_STAR__

#include "base_object.hpp"
#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitDeathStar : public ImplicitSurface
	{
	  public:
		ImplicitDeathStar()		  = delete;
		virtual ~ImplicitDeathStar() = default;

		ImplicitDeathStar( const std::string & p_name, Vec3f p_center, float ra, float rb, float p_dist )
			: ImplicitSurface( p_name ), _center( p_center ), _ra( ra ), _rb( rb ), _distance( p_dist )
		{
		}
		float getSDF( const Vec3f & point ) const override { return _sdf( point ); }

	  private:
		float _sdf( const Vec3f & p_point ) const override {
			Vec3f point = p_point - _center;

			float a = ( _ra * _ra - _rb * _rb + _distance * _distance ) / ( 2.0f * _distance );
			float b = sqrt( glm::max( _ra * _ra - a * a, 0.0f ) );

			Vec2f yz = Vec2f( point.y, point.z );
			Vec2f p	 = Vec2f( point.x, glm::length( yz ) );

			if ( ( p.x * b - p.y * a ) > (_distance * glm::max( b - p.y, 0.0f )) )
				return glm::length( p - Vec2f( a, b ) );
			else
				return glm::max( ( length( p ) - _ra ), -( glm::length( p - Vec2f( _distance, 0.0f ) ) - _rb ) );
		}

		Vec3f _center = VEC3F_ZERO;
		float _distance = 0.0f;
		float _ra	    = 0.0f;
		float _rb	    = 0.0f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_DEATH_STAR__
