#ifndef __RT_ISICG_CSG__
#define __RT_ISICG_CSG__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class CSG : public ImplicitSurface
	{
	  public:
		enum class Operation
		{
			UNION,
			SUBTRACTION,
			INTERSECTION,
			XOR,
			SMOOTH_UNION,
			SMOOTH_SUBTRACTION,
			SMOOTH_INTERSECTION
		};

		CSG( const std::string & p_name, Operation op, ImplicitSurface * obj1, ImplicitSurface * obj2, float k )
			: ImplicitSurface( p_name ), _operation( op ), _obj1( obj1 ), _obj2( obj2 ), _k( k )
		{
		}
		virtual ~CSG() = default;

		float getSDF( const Vec3f & point ) const { return _sdf( point ); }

	  private:
		float _sdf( const Vec3f & p_point ) const override
		{
			float sdf1 = _obj1->getSDF( p_point );
			float sdf2 = _obj2->getSDF( p_point );

			switch ( _operation )
			{
				case Operation::UNION: return opUnion( sdf1, sdf2 );
				case Operation::SUBTRACTION: return opSubtraction( sdf1, sdf2 );
				case Operation::INTERSECTION: return opIntersection( sdf1, sdf2 );
				case Operation::XOR: return opXor( sdf1, sdf2 );
				case Operation::SMOOTH_UNION: return opSmoothUnion( sdf1, sdf2, _k );
				case Operation::SMOOTH_SUBTRACTION: return opSmoothSubtraction( sdf1, sdf2, _k );
				case Operation::SMOOTH_INTERSECTION: return opSmoothIntersection( sdf1, sdf2, _k );
				default: return 0.0f;
			}
		}

		// Cf. https://iquilezles.org/articles/distfunctions/ (Primitive combinations)
		static float opUnion( float d1, float d2 ) { return std::min( d1, d2 ); }

		static float opSubtraction( float d1, float d2 ) { return std::max( -d1, d2 ); }

		static float opIntersection( float d1, float d2 ) { return std::max( d1, d2 ); }

		static float opXor( float d1, float d2 ) { return std::max( std::min( d1, d2 ), -std::max( d1, d2 ) ); }

		static float opSmoothUnion( float d1, float d2, float k )
		{
			float h = glm::clamp( 0.5f + 0.5f * ( d2 - d1 ) / k, 0.0f, 1.0f );
			return glm::mix( d2, d1, h ) - k * h * ( 1.0f - h );
		}

		static float opSmoothSubtraction( float d1, float d2, float k )
		{
			float h = glm::clamp( 0.5f - 0.5f * ( d2 + d1 ) / k, 0.0f, 1.0f );
			return glm::mix( d2, -d1, h ) + k * h * ( 1.0f - h );
		}

		static float opSmoothIntersection( float d1, float d2, float k )
		{
			float h = glm::clamp( 0.5f - 0.5f * ( d2 - d1 ) / k, 0.0f, 1.0f );
			return glm::mix( d2, d1, h ) + k * h * ( 1.0f - h );
		}

		Operation		  _operation;
		ImplicitSurface * _obj1;
		ImplicitSurface * _obj2;
		float			  _k;	// contrôle le degré de douceur de l'opération CSG
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_CSG__
