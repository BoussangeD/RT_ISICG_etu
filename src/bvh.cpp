#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();	// racine
		_buildRec( _root, 0, (int)_triangles->size(), 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		if ( _root ) { return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord ); }	// si la racine existe on peut faire l'intersection
		return false;
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		if ( _root ) { return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax ); }
		return false;
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		// TODO
	}

	// similaire a la fonction MeshTriangl::intersect
	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		float  tClosest = p_tMax; // Hit distance.
		float  u		= 0.0f;
		float  v		= 0.0f;
		size_t hitTri	= _triangles->size(); // Hit triangle id.

		if ( !p_node->isLeaf() )
		{
			HitRecord hitRecordLeft, hitRecordRight;
			hitRecordLeft._distance = FLT_MAX;
			hitRecordRight._distance = FLT_MAX;

			bool hitLeft  = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, hitRecordLeft ); // intersection gauche
			bool hitRight = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, hitRecordRight ); // intersection droite

			if ( hitLeft || hitRight )	// si il ya une intersection
			{							
				if ( hitRecordLeft._distance < hitRecordRight._distance )
				{
					p_hitRecord = hitRecordLeft; // maj de p_hitRecord avec l'enfant gauche
				}
				else
				{
					p_hitRecord = hitRecordRight; // maj de p_hitRecord avec l'enfant droit
				}
				return true; 
			}
		}
		else
		{
			//for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
			for ( size_t i = 0; i < _triangles->size(); i++ )
			{
				float t, temp_u, temp_v;
				if ( ( *_triangles )[ i ].intersect( p_ray, t, temp_u, temp_v ) )
				{
					if ( t >= p_tMin && t <= tClosest )
					{
						tClosest = t;
						hitTri	 = i;
						u		 = temp_u;
						v		 = temp_v;
					}
				}
			}
			// hitTri != p_node->_lastTriangleId
			if ( hitTri != _triangles->size() ) // Intersection found.
			{
				p_hitRecord._point	= p_ray.pointAtT( tClosest );
				p_hitRecord._normal = ( *_triangles )[ hitTri ].getNormalInterpolation( u, v );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = tClosest;

				return true;
			}
		}
		return false;
	}

	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( !p_node->isLeaf() )
		{
			bool hitLeft	= _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
			bool hitRight = _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );

			if ( hitLeft || hitRight ) return true;
		}
		else
		{
			for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
			{
				float t, u, v;
				if ( ( *_triangles )[ i ].intersect( p_ray, t, u, v ) )
				{
					if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
				}
			}
			return false;
		}
		return false;
	}
} // namespace RT_ISICG
