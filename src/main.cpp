#include "cameras/perspective_camera.hpp"
#include "defines.hpp"
#include "renderer.hpp"

namespace RT_ISICG
{
	int main( int argc, char ** argv )
	{
		const int imgWidth	= 600;
		const int imgHeight = 400;

		// Create a texture to render the scene.
		Texture img = Texture( imgWidth, imgHeight );

		// Create and init scene.
		Scene scene;
		scene.init();

		// position a
		Vec3f _position_a = Vec3f( 0.0f, 0.0f, -2.0f );
		Vec3f _lookAt_a	  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position b
		Vec3f _position_b = Vec3f( 1.0f, 0.0f, 0.0f ); 
		Vec3f _lookAt_b	  = Vec3f( 1.0f, 0.0f, 1.0f );

		// position c
		Vec3f _position_c = Vec3f( 0.0f, 1.0f, 0.0f );
		Vec3f _lookAt_c	  = Vec3f( 0.0f, 1.0f, 1.0f );

		// position d
		Vec3f _position_d = Vec3f( 4.0f, -1.0f, 0.0f );
		Vec3f _lookAt_d	  = Vec3f( -1.0f, -1.0f, 2.0f );

		// Create a perspective camera.
		//PerspectiveCamera camera( float( imgWidth ) / imgHeight );	// camera par défaut
		PerspectiveCamera camera( _position_a, _lookAt_a, Vec3f( 0.0f, 1.0f, 0.0f ), 60.0f, float( imgWidth ) / imgHeight );

		// Create and setup the renderer.
		Renderer renderer;
		renderer.setIntegrator( IntegratorType::RAY_CAST );
		renderer.setBackgroundColor( GREY );
		renderer.setNbPixelSamples( 4 );

		// Launch rendering.
		std::cout << "Rendering..." << std::endl;
		std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;

		float renderingTime = renderer.renderImage( scene, &camera, img );

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
