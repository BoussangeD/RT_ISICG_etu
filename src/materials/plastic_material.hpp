#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/lambert_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float p_shininess )
			: BaseMaterial( p_name ), _lamberBrdf( p_diffuse ), _phongBrdf( p_specular, p_shininess )
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			// ici la formule complète avec ( kd / pi ) au début
			return _lamberBrdf.evaluate() + _phongBrdf.evaluate( p_hitRecord._normal, p_lightSample._direction, -p_ray.getDirection() );
		}

		inline const Vec3f & getFlatColor() const override { return _lamberBrdf.getKd(); }

	  protected:
		PhongBRDF   _phongBrdf;
		LambertBRDF _lamberBrdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__
