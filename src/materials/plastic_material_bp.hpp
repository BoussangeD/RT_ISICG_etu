#ifndef __RT_ISICG_PLASTIC_MATERIAL_BP__
#define __RT_ISICG_PLASTIC_MATERIAL_BP__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/blinn_phong_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterialBP : public BaseMaterial	// Classe pour tester BlinnPhong
	{
	  public:
		PlasticMaterialBP( const std::string & p_name,
						 const Vec3f &		 p_diffuse,
						 const Vec3f &		 p_speculaire,
						 const float		 p_shininess )
			: BaseMaterial( p_name ), _lamberBrdf( p_diffuse ), _blinnPhongBrdf( p_speculaire, p_shininess )
		{
		}

		virtual ~PlasticMaterialBP() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			// ici la formule complète avec ( kd / pi ) au début
			return _lamberBrdf.evaluate() + _blinnPhongBrdf.evaluate( p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection() );
		}

		inline const Vec3f & getFlatColor() const override { return _lamberBrdf.getKd(); }

	  protected:
		BlinnPhongBRDF	_blinnPhongBrdf;
		LambertBRDF     _lamberBrdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL_BP__
