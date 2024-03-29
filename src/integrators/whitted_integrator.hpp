#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		virtual const IntegratorType getType() const override { return IntegratorType::WHITTED; }

		virtual Vec3f Li( const Scene & p_scene,
						  const Ray &	p_ray,
						  const float	p_tMin,
						  const float	p_tMax ) const override;

	  private:
		Vec3f _directLighting( const HitRecord & p_hitRecord,
							   const Ray &		 p_ray,
							   const BaseLight * p_light,
							   const Scene &	 p_scene,
							   const float		 p_tMin ) const;

		Vec3f trace( const Scene & p_scene,
					 const Ray &   p_ray,
					 const float   p_tMin,
					 const float   p_tMax,
					 int		   p_nbBounces,
					 bool		   p_inObject ) const;

		int _nbBounces = 5;
		int _nbLightSamples = 32;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__
