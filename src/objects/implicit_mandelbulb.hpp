#ifndef __RT_ISICG_IMPLICIT_MANDELBULB__
#define __RT_ISICG_IMPLICIT_MANDELBULB__

#include "base_object.hpp"
#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitMandelbulb : public ImplicitSurface
	{
	  public:
		ImplicitMandelbulb()			 = delete;
		virtual ~ImplicitMandelbulb() = default;

		ImplicitMandelbulb( const std::string & p_name, Vec3f p_center, float p_power, int p_nbIterations )
			: ImplicitSurface( p_name ), _center( p_center ), _power( p_power ), _nbIterations( p_nbIterations )
		{
		}

	  private:
		float _sdf( const Vec3f & p_point ) const override
		{
			// Cf. https://www.shadertoy.com/view/tsc3Rj
			Vec3f z		= p_point - _center;
			float dr	= 1.0f;
			float r		= 0.0f;

			for ( int i = 0; i < _nbIterations; i++ ) {
				r = glm::length( z );
				if ( r > 1.5f ) break;

				float theta = acos( z.y / r );
				float phi	= atan2( z.z, z.x );

				dr = pow( r, _power - 1.0f ) * _power * dr + 1.0f;

				float zr = pow( r, _power );
				theta	 = theta * _power;
				phi	 = phi * _power;

				//z = zr * Vec3f( sin( theta ) * cos( phi ), cos( theta ), sin( phi ) * sin( theta ) );
				z = zr * Vec3f( cos( theta ) * cos( phi ), sin( theta ), cos( theta ) * sin( phi ) );
				z += p_point;
			}
			return 0.5f * glm::log( r ) * r / dr;
		}

		Vec3f _center = VEC3F_ZERO;
		const float _power;
		const int	_nbIterations;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_MANDELBULB__
