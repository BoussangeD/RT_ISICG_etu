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

		// TP1-3
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

		// position TP4
		Vec3f _position_4 = Vec3f( 0.0f, 2.0f, -7.0f );
		Vec3f _lookAt_4  = Vec3f( 0.0f, 0.0f, 79.0f );

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
		Vec3f _lookAt_7s  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position TP7DeathStar
		Vec3f _position_7ds = Vec3f( 6.0f, 0.0f, 5.0f );
		Vec3f _lookAt_7ds	= Vec3f( -100.0f, 0.0f, 0.0f );

		// position TP7Mandelbulb
		Vec3f _position_7mb = Vec3f( 0.0f, 0.0f, -4.0f );
		Vec3f _lookAt_7mb	  = Vec3f( 0.0f, 0.0f, 79.0f );

		// position CSG
		Vec3f _position_csg = Vec3f( -3.0f, 5.0f, -4.0f );
		Vec3f _lookAt_csg   = Vec3f( 0.0f, -30.0f, 79.0f );

		// Create a perspective camera.
		//PerspectiveCamera camera( float( imgWidth ) / imgHeight );	// camera par défaut
		PerspectiveCamera camera( _position_csg, _lookAt_csg, Vec3f( 0.0f, 1.0f, 0.0f ), 60.0f, float( imgWidth ) / imgHeight );

		// Create and setup the renderer.
		Renderer renderer;
		renderer.setIntegrator( IntegratorType::WHITTED ); // RAY_CAST -> TP1, DIRECT_LIGHTING -> TP2-5, WHITTED -> TP6
		renderer.setBackgroundColor( GREY );
		renderer.setNbPixelSamples( 128 );

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
