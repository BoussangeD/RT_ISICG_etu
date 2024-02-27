#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
	  public:
		CookTorranceBRDF( const Vec3f & p_ks, const float p_rugosite ) : _ks( p_ks ), _rugosite( p_rugosite ) {};

		inline Vec3f evaluate( const Vec3f & normal, const Vec3f & wi, const Vec3f & wo ) const
		{
			Vec3f h = normalize( wi + wo ); // demi vecteur h

			// calcul NDF
			float alpha  = _rugosite * _rugosite;
			float alpha2 = alpha * alpha;
			float nh2	 = dot( normal, h ) * dot( normal, h );
			float den    = nh2 * ( alpha2 - 1 ) + 1;	// denominateur a mettre au carré ensuite

			float D = alpha2 / ( PIf * ( den * den ) );

			// modèle Smith (GGX)
			// x -> produit scalaire entre normale et direction wi et wo
			float k = ( alpha2 + 1 ) / 8.0f;
			float G1_wi = dot( normal, wi ) / ( ( dot( normal, wi ) * ( 1 - k ) + k ) );
			float G1_wo = dot( normal, wo ) / ( ( dot( normal, wo ) * ( 1 - k ) + k ) );

			float G	= G1_wi * G1_wo;

			// approximation de Schlick
			float hwo = 1 - dot( h, wo );
			float hwo5 = hwo * hwo * hwo * hwo * hwo;

			Vec3f F = _ks + ( 1.0f - _ks ) * hwo5;

			return ( D * F * G ) / ( 4 * dot( normal, wo ) * dot( normal, wi ) );
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks		= WHITE;
		float _rugosite = 0.0f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_COOK_TORRANCE__
