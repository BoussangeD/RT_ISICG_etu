#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::DIRECT_LIGHTING: _integrator = new DirectLightingIntegrator(); break;	// nouvel integrateur
		case IntegratorType::WHITTED: _integrator = new WhittedIntegrator(); break;
		case IntegratorType::RAY_CAST:
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();

		#pragma omp parallel for
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				// Exercice 1
				//float blueColor = 0.0f;
				//float redColor = (float)i / (width - 1);		// l’abscisse
				//float greenColor = (float)j / (height - 1);	// ordonnée
				//p_texture.setPixel( i, j, Vec3f( redColor, greenColor, blueColor ) );

				// Exercice 2
				//float sx = (float)(i + 0.5f) / ( width - 1 );	// on fait + 0.5f pour lancer au centre du pixel
				//float sy = (float)(j + 0.5f) / ( height - 1 );
				//Ray ray = p_camera->generateRay(sx, sy);
				//Vec3f colorRay = (ray.getDirection() + 1.0f) * 0.5f;
				//p_texture.setPixel( i, j, colorRay );

				// Exercice 3 - 4
				//float sx = (float)(i + 0.5f) / ( width - 1 );	// on fait + 0.5f pour lancer au centre du pixel
				//float sy = (float)(j + 0.5f) / ( height - 1 );
				//Ray	  ray	   = p_camera->generateRay( sx, sy );
				//Vec3f colorRay = _integrator->Li( p_scene, ray, 0.0f, 1000.0f );
				//p_texture.setPixel( i, j, colorRay );

				// Exercice 5
				Vec3f finalColor = VEC3F_ZERO;
				for (int k = 0; k < _nbPixelSamples; k++) {
					// randomFloat du header random.hpp
					float sx = (float)(i + randomFloat()) / width;
					float sy = (float)(j + randomFloat()) / height;
					Ray	  ray	   = p_camera->generateRay( sx, sy );
					Vec3f colorRay = _integrator->Li( p_scene, ray, 0.0f, 10000.0f );
					finalColor	   += colorRay;
				}
				finalColor /= _nbPixelSamples;

				finalColor = glm::clamp( finalColor, 0.0f, 1.0f ); // restreindre l'intervalle [0;255] sur [0;1]

				p_texture.setPixel( i, j, finalColor );
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}

	// version depth of field
	// Cf. https://pathtracing.home.blog/depth-of-field/ et https://stackoverflow.com/questions/10012219/how-to-implement-depth-of-field-in-ray-tracer
	float Renderer::renderImage( const Scene &					   p_scene,
								 const std::vector<BaseCamera *> & p_cameras,
								 Texture &						   p_texture )
	{
		const int width		= p_texture.getWidth();
		const int height	= p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height * (int)p_cameras.size(), 50 );
		chrono.start();

		std::vector<std::vector<Vec3f>> pixelTab( width, std::vector<Vec3f>( height, VEC3F_ZERO ) );

		#pragma omp parallel for
		for ( int camIdx = 0; camIdx < p_cameras.size(); camIdx++ )	// on fait le rendu autant de fois qu'on a de cameras
		{
			for ( int j = 0; j < height; j++ )
			{
				for ( int i = 0; i < width; i++ )
				{
					Vec3f finalColor = VEC3F_ZERO;
					for ( int k = 0; k < _nbPixelSamples; k++ )
					{
						// randomFloat du header random.hpp
						float sx	   = (float)( i + randomFloat() ) / width;
						float sy	   = (float)( j + randomFloat() ) / height;
						Ray	  ray	   = p_cameras[camIdx]->generateRay( sx, sy );
						Vec3f colorRay = _integrator->Li( p_scene, ray, 0.0f, 10000.0f );
						finalColor += colorRay;
					}
					finalColor /= _nbPixelSamples;

					finalColor = glm::clamp( finalColor, 0.0f, 1.0f );	// restreindre l'intervalle [0;255] sur [0;1]

					pixelTab[ i ][ j ] += finalColor;	// ajout de la couleur au tableau de pixels

					// calcule la couleur finale d'un pixel en moyennant les valeurs de couleur stockées dans le tableau
					Vec3f finalColor2 = pixelTab[ i ][ j ] / (float)p_cameras.size();	
					p_texture.setPixel( i, j, glm::clamp( finalColor2, 0.f, 1.f ) );
				}
				progressBar.next();
			}
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
