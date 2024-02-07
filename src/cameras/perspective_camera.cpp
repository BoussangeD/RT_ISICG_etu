#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		_w = normalize( p_position - p_lookAt );
		_u = normalize( cross( p_up, _w ) );	// ici, p_up correspond à l'axe y, ceci est indiqué dans le main
		_v = normalize( cross( _w, _u ) );
		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float _viewportHeight = 2.0f * _focalDistance * glm::tan( glm::radians( _fovy / 2.0f ) );
		float _viewportWidth  = _aspectRatio * _viewportHeight;

		_viewportU = _u * _viewportWidth;
		_viewportV = _v * _viewportHeight;
		_viewportTopLeftCorner = getPosition() - ( _w * _focalDistance ) - ( _viewportU / 2.0f ) + ( _viewportV / 2.0f );
	}

} // namespace RT_ISICG
