#ifndef __RT_ISICG_IMPLICIT_BOX__
#define __RT_ISICG_IMPLICIT_BOX__

#include "base_object.hpp"
#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitBox : public ImplicitSurface
	{
	  public:
		ImplicitBox()		   = delete;
		virtual ~ImplicitBox() = default;

		ImplicitBox( const std::string & p_name, Vec3f p_center, Vec3f p_halfExtents, float p_rotationAngle, Vec3f p_rotationAxis )
			: ImplicitSurface( p_name ), _center( p_center ), _halfExtents( p_halfExtents ),
			  _rotationAngle( p_rotationAngle ), _rotationAxis( p_rotationAxis )
		{
		}

		float getSDF( const Vec3f & point ) const { return _sdf( point ); }

	  private:
		float _sdf( const Vec3f & p_point ) const override
		{
			// appliquer la rotation à p_point avant de calculer la distance signée
			Vec3f rotatedPoint = rotatePoint( p_point, _rotationAngle, _rotationAxis );
			Vec3f q			   = glm::abs( rotatedPoint - _center ) - _halfExtents;
			return glm::length( glm::max( q, 0.0f ) ) + glm::min( glm::max( q.x, glm::max( q.y, q.z ) ), 0.0f );
		}

		Vec3f rotatePoint( const Vec3f & point, float angle, const Vec3f & axis ) const
		{
			float c	   = cos( angle );
			float s	   = sin( angle );
			float t	   = 1.0f - c;
			float x	   = point.x;
			float y	   = point.y;
			float z	   = point.z;
			float xRot = ( t * axis.x * axis.x + c ) * x + ( t * axis.x * axis.y - s * axis.z ) * y
						 + ( t * axis.x * axis.z + s * axis.y ) * z;
			float yRot = ( t * axis.x * axis.y + s * axis.z ) * x + ( t * axis.y * axis.y + c ) * y
						 + ( t * axis.y * axis.z - s * axis.x ) * z;
			float zRot = ( t * axis.x * axis.z - s * axis.y ) * x + ( t * axis.y * axis.z + s * axis.x ) * y
						 + ( t * axis.z * axis.z + c ) * z;
			return Vec3f( xRot, yRot, zRot );
		}

		Vec3f _center	   = VEC3F_ZERO;
		Vec3f _halfExtents = VEC3F_ZERO;
		float _rotationAngle = 0.0f;
		Vec3f _rotationAxis	 = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BOX__
