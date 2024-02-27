#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float p_shininess ) : _ks( p_ks ), _shininess( p_shininess ) {};

		inline Vec3f evaluate( const Vec3f & normal, const Vec3f & wi, const Vec3f & wo ) const
		{
			Vec3f wr = reflect( -wi, normal );	// reflect normalise deja

			float cosTheta_i = glm::max( 0.0f, dot( wi, normal ) );
			float cosAlpha_s = glm::max( 0.0f, dot( wr, wo ) );

			return ( ( _ks / cosTheta_i ) * pow( cosAlpha_s, _shininess ) ); // uniquement la partie spéculaire
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks	 = WHITE;
		float _shininess = 1.0f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
