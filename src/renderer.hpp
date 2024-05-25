#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "cameras/base_camera.hpp"
#include "defines.hpp"
#include "integrators/base_integrator.hpp"
#include "texture.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
	class Renderer
	{
	  public:
		Renderer();
		~Renderer() { delete _integrator; }

		void setIntegrator( const IntegratorType p_integratorType );
		void setBackgroundColor( const Vec3f & p_color );

		inline void setNbPixelSamples( const int p_nbPixelSamples ) { _nbPixelSamples = p_nbPixelSamples; }
		void		setNbLightSamples( int p_NblightSamples ) { _integrator->setNbLightSamples( p_NblightSamples ); }
		void		setNbBounces( int p_NbBounces ) { _integrator->setNbBounces( p_NbBounces ); }

		float renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture );
		float renderImage( const Scene & scene, const std::vector<BaseCamera *> & cameras, Texture & texture );

	  private:
		BaseIntegrator * _integrator	 = nullptr;
		int				 _nbPixelSamples = 1;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
