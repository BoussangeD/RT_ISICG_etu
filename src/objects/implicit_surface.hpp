#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "base_object.hpp"

namespace RT_ISICG
{
	class ImplicitSurface : public BaseObject
	{
	  public:
		ImplicitSurface()		   = delete;
		virtual ~ImplicitSurface() = default;

		ImplicitSurface( const std::string & p_name ) : BaseObject( p_name ) {}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;
		virtual float getSDF( const Vec3f & point ) const = 0;	// pour la CSG

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const = 0;

		// Evaluate normal by computing gradient at 'p_point'
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			// Cf. https://iquilezles.org/articles/normalsSDF/
			float h = 0.0001f;
			float x = _sdf( p_point + Vec3f( h, 0, 0 ) ) - _sdf( p_point - Vec3f( h, 0, 0 ) );
			float y = _sdf( p_point + Vec3f( 0, h, 0 ) ) - _sdf( p_point - Vec3f( 0, h, 0 ) );
			float z = _sdf( p_point + Vec3f( 0, 0, h ) ) - _sdf( p_point - Vec3f( 0, 0, h ) );

			x = x / ( 2.0f * h );
			y = y / ( 2.0f * h );
			z = z / ( 2.0f * h );

			return normalize( Vec3f( x, y, z ) );
		}

	  private:
		const float _minDistance = 1e-4f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
