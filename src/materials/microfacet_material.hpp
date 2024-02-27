#ifndef __RT_ISICG_MICROFACET_MATERIAL__
#define __RT_ISICG_MICROFACET_MATERIAL__

#include "base_material.hpp"
#include "brdfs/oren_nayar_brdf.hpp"
#include "brdfs/cook_torrance_brdf.hpp"

namespace RT_ISICG
{
	class MicrofacetMaterial : public BaseMaterial
	{
	  public:
		MicrofacetMaterial( const std::string & p_name,
						 const Vec3f &		 p_diffuse,
						 const Vec3f &		 p_specular,
						 const float		 p_rugosite,
						 const float		 p_metalness )
			: BaseMaterial( p_name ), _orenNayarBrdf( p_diffuse, p_rugosite ),
			  _cookTorranceBrdf( p_specular, p_rugosite ), _metalness( p_metalness )
		{
		}

		virtual ~MicrofacetMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return ( 1.0f - _metalness ) * _orenNayarBrdf.evaluate( p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection() )
				   + _metalness * _cookTorranceBrdf.evaluate( p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection() );
		}

		inline const Vec3f & getFlatColor() const override { return _orenNayarBrdf.getKd(); }

	  protected:
		OrenNayarBRDF	 _orenNayarBrdf;
		CookTorranceBRDF _cookTorranceBrdf;
		float			 _metalness = 0.0f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MICROFACET_MATERIAL__
