#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float p_sigma ) : _kd( p_kd * INV_PIf ), _sigma( p_sigma ) {};

		inline Vec3f evaluate( const Vec3f & normal, const Vec3f & wi, const Vec3f & wo ) const
		{
			// calcul des cosinus et des sinus des angles d'incidence et d'observation 
			float cosTheta_i = dot( normalize( wi ), normalize( normal ) );
			float cosTheta_o = dot( normalize( wo ), normalize( normal ) );
			// sin^2(x) + cos^2(x) = 1
			float sinAlpha_i = sqrt( 1.0f - cosTheta_i * cosTheta_i );	
			float sinAlpha_o = sqrt( 1.0f - cosTheta_o * cosTheta_o );

			float sinBeta, tanBeta;
			float maxCos = glm::max( fabs( cosTheta_i ), fabs( cosTheta_o ) );	// rugosité

			if ( maxCos == 0.0f ) { return _kd / INV_PIf; }

			// différence angulaire entre wi et wo
			float cosPhi = dot( normalize( wi - normal * cosTheta_i * 2.0f ), normalize( wo - normal * cosTheta_o * 2.0f ) );

			if ( cosPhi < 0.0f ) { sinBeta = sinAlpha_i; }
			else { sinBeta = sinAlpha_o; }

			tanBeta	 = sinBeta / maxCos;	// tan(t) = sin(t) / cos(t)

			float A = 1.0f - 0.5f * ( _sigma * _sigma ) / ( ( _sigma * _sigma ) + 0.33f );
			float B = 0.45f * ( _sigma * _sigma ) / ( ( _sigma * _sigma ) + 0.09f );

			return _kd * ( A + ( B * glm::max( 0.0f, cosPhi ) ) * sinAlpha_i * tanBeta );	// equation de la brdf
		}

		inline const Vec3f & getKd() const { return _kd / INV_PIf; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma = 0.0f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_OREN_NAYAR__
