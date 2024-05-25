#include "cameras/perspective_camera.hpp"
#include "defines.hpp"
#include "renderer.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	enum class TP
	{
		TP1,
		TP2,
		TP3,
		TP4,
		TP5,
		TP5Phong,
		TP5CookTorrance,
		TP6Mirror,
		TP6Transparent,
		TP7Sphere,
		TP7DeathStar,
		TP7Mandelbulb,
		Projet1,
		Projet2,
		ImageFinale
	};

	int main( int argc, char ** argv )
	{
		// Changer ici pour choisir le TP voulu, la taille de l'image ou le nombre de samples
		TP		  choixTP	= TP::ImageFinale;
		const int imgWidth	= 600;
		const int imgHeight = 400;

		int nbSamples      = 32;	// nb pixels
		int nbLightSamples = 32;	// nb lights
		int nbBounces	   = 5;	    // nb bounces

		int     nbCameras = 20;		// nombre de caméras pour l'effet de profondeur de champ
		float	aperture  = 0.1f;

		// Create a texture to render the scene.
		Texture img = Texture( imgWidth, imgHeight );

		// position de la caméra et de l'intégrateur (ne pas changer ici)
		Vec3f _choixPosition = VEC3F_ZERO;
		Vec3f _choixLookAt	 = VEC3F_ZERO;
		IntegratorType _choixIntegrator = IntegratorType::WHITTED;

		// position a tp1
		Vec3f _position_a = Vec3f( 0.0f, 0.0f, -2.0f );
		Vec3f _lookAt_a	  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position b tp1
		Vec3f _position_b = Vec3f( 1.0f, 0.0f, 0.0f );
		Vec3f _lookAt_b	  = Vec3f( 1.0f, 0.0f, 1.0f );

		// position c tp1
		Vec3f _position_c = Vec3f( 0.0f, 1.0f, 0.0f );
		Vec3f _lookAt_c	  = Vec3f( 0.0f, 1.0f, 1.0f );

		// position d tp1
		Vec3f _position_d = Vec3f( 4.0f, -1.0f, 0.0f );
		Vec3f _lookAt_d	  = Vec3f( -1.0f, -1.0f, 2.0f );

		// position TP4
		Vec3f _position_4 = Vec3f( 0.0f, 2.0f, -7.0f );
		Vec3f _lookAt_4	  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position TP4_conf
		Vec3f _position_4c = Vec3f( -250.0f, 500.0f, 330.0f );
		Vec3f _lookAt_4c   = Vec3f( 0.0f, 350.0f, 100.0f );

		// position TP5
		Vec3f _position_5 = Vec3f( 0.0f, 0.0f, 0.0f );
		Vec3f _lookAt_5	  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position TP6
		Vec3f _position_6 = Vec3f( 0.0f, 2.0f, -6.0f );
		Vec3f _lookAt_6	  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position TP7Sphere
		Vec3f _position_7s = Vec3f( 0.0f, 0.0f, -2.0f );
		Vec3f _lookAt_7s   = Vec3f( 0.0f, 0.0f, 79.0f );

		// position TP7DeathStar
		Vec3f _position_7ds = Vec3f( 6.0f, 0.0f, 5.0f );
		Vec3f _lookAt_7ds	= Vec3f( -100.0f, 0.0f, 0.0f );

		// position TP7Mandelbulb
		Vec3f _position_7mb = Vec3f( 0.0f, 0.0f, -4.0f );
		Vec3f _lookAt_7mb	= Vec3f( 0.0f, 0.0f, 79.0f );

		// position CSG
		Vec3f _position_csg = Vec3f( -3.0f, 5.0f, -6.0f );
		Vec3f _lookAt_csg	= Vec3f( 0.0f, -30.0f, 79.0f );

		// position DoF
		Vec3f _position_dof = Vec3f( 0.0f, 0.0f, -6.0f );
		Vec3f _lookAt_dof	= Vec3f( 0.0f, 0.0f, 0.0f );

		// position Image Finale
		Vec3f _position_imgF = Vec3f( 0.0f, 2.0f, -10.0f );
		Vec3f _lookAt_imgF	= Vec3f( 0.0f, 0.0f, 5.0f );

		// Create and init scene.
		Scene scene;
		bool  depthOfField = false;
		switch ( choixTP )
		{
		case TP::TP1: 
			scene._initSceneTP1();
			_choixPosition = _position_a;
			_choixLookAt   = _lookAt_a;
			_choixIntegrator = IntegratorType::RAY_CAST;
			depthOfField	 = false;
			break;

		case TP::TP2:
			scene._initSceneTP2();
			_choixPosition = _position_a;
			_choixLookAt   = _lookAt_a;
			_choixIntegrator = IntegratorType::DIRECT_LIGHTING;
			depthOfField	 = false;
			break;

		case TP::TP3: 
			scene._initSceneTP3();
			_choixPosition = _position_a;
			_choixLookAt   = _lookAt_a;
			_choixIntegrator = IntegratorType::DIRECT_LIGHTING;
			depthOfField	 = false;
			break;

		case TP::TP4:
			scene._initSceneTP4();
			_choixPosition = _position_4;
			_choixLookAt   = _lookAt_4;
			_choixIntegrator = IntegratorType::DIRECT_LIGHTING;
			depthOfField	 = false;
			break;

		case TP::TP5: 
			scene._initSceneTP5();
			_choixPosition = _position_5;
			_choixLookAt   = _lookAt_5;
			_choixIntegrator = IntegratorType::DIRECT_LIGHTING;
			depthOfField	 = false;
			break;

		case TP::TP5Phong:
			scene._initSceneTP5Phong();
			_choixPosition = _position_5;
			_choixLookAt   = _lookAt_5;
			_choixIntegrator = IntegratorType::DIRECT_LIGHTING;
			depthOfField	 = false;
			break;

		case TP::TP5CookTorrance: 
			scene._initSceneTP5CookTorrance();
			_choixPosition = _position_5;
			_choixLookAt   = _lookAt_5;
			_choixIntegrator = IntegratorType::DIRECT_LIGHTING;
			depthOfField	 = false;
			break;

		case TP::TP6Mirror:
			scene._initSceneTP6Mirror();
			_choixPosition = _position_6;
			_choixLookAt   = _lookAt_6;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;

		case TP::TP6Transparent: 
			scene._initSceneTP6Transparent();
			_choixPosition = _position_6;
			_choixLookAt   = _lookAt_6;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;

		case TP::TP7Sphere:
			scene._initSceneTP7Sphere();
			_choixPosition = _position_7s;
			_choixLookAt   = _lookAt_7s;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;

		case TP::TP7DeathStar: 
			scene._initSceneTP7DeathStar();
			_choixPosition = _position_7ds;
			_choixLookAt   = _lookAt_7ds;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;

		case TP::TP7Mandelbulb:
			scene._initSceneTP7Mandelbulb();
			_choixPosition = _position_7mb;
			_choixLookAt   = _lookAt_7mb;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;

		case TP::Projet1: 
			scene._initSceneProjet();
			_choixPosition = _position_csg;
			_choixLookAt   = _lookAt_csg;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;

		case TP::Projet2: default:
			scene._initSceneProjet2();
			_choixPosition = _position_dof;
			_choixLookAt   = _lookAt_dof;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = true;
			break;

		case TP::ImageFinale:
			scene._initSceneImageFinale();
			nbBounces		 = 50;
			_choixPosition	 = _position_imgF;
			_choixLookAt	 = _lookAt_imgF;
			_choixIntegrator = IntegratorType::WHITTED;
			depthOfField	 = false;
			break;
		}

		// Create and setup the renderer.
		Renderer renderer;
		renderer.setIntegrator( _choixIntegrator );
		renderer.setBackgroundColor( GREY );
		renderer.setNbPixelSamples( nbSamples );
		renderer.setNbLightSamples( nbLightSamples );
		renderer.setNbBounces( nbBounces );

		// Launch rendering.
		std::cout << "Rendering..." << std::endl;
		std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;
		float renderingTime = 0.0f;

		// Create a perspective camera.
		PerspectiveCamera * camera = nullptr;
		if ( depthOfField == false ) 
		{
			// camera = new PerspectiveCamera( float( imgWidth ) / imgHeight );	// camera par défaut
			camera = new PerspectiveCamera( _choixPosition, _choixLookAt, Vec3f( 0.0f, 1.0f, 0.0f ), 60.0f, float( imgWidth ) / imgHeight );
			renderingTime = renderer.renderImage( scene, camera, img );
		}

		else
		{
			std::vector<BaseCamera *> cameras;
			// Launch rendering with each camera
			for ( int i = 0; i < nbCameras; i++ )
			{
				// position aléatoire pour la caméra via l'aperture
				Vec3f randomPosition = Vec3f( randomFloat() * aperture + _choixPosition.x,
											  randomFloat() * aperture + _choixPosition.y,
											  _choixPosition.z );

				// nouvelle caméra avec la position aléatoire et l'ajouter à la liste
				cameras.push_back( new PerspectiveCamera(
					randomPosition, _choixLookAt, Vec3f( 0.0f, 1.0f, 0.0f ), 60.0f, float( imgWidth ) / imgHeight ) );

			}
			std::cout << "Rendering image with " << nbCameras << " cameras. " << std::endl;

			renderingTime = renderer.renderImage( scene, cameras, img );
		}

		std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

		// Save rendered image.
		const std::string imgName = "image.jpg";
		img.saveJPG( RESULTS_PATH + imgName );

		return EXIT_SUCCESS;
	}
} // namespace RT_ISICG

int main( int argc, char ** argv )
{
	try
	{
		return RT_ISICG::main( argc, argv );
	}
	catch ( const std::exception & e )
	{
		std::cerr << "Exception caught:" << std::endl << e.what() << std::endl;
	}
}
