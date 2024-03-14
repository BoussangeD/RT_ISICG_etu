#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, float & p_u, float & p_v ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		// Cf. https://www.graphics.cornell.edu/pubs/1997/MT97.pdf ou https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
		float epsilon = 0.000001f;
		Vec3f edge1, edge2, tvec, pvec, qvec;
		float det = 0.0f;
		float inv_det = 0.0f;

		edge1 = v1 - v0;
		edge2 = v2 - v0;

		pvec = cross( d, edge2 );
		det	 = dot( edge1, pvec );

		if ( det > -epsilon && det < epsilon ) return false; // rayon parallèle au triangle

		inv_det = 1.0f / det;
		tvec = o - v0;
		p_u = inv_det * dot( tvec, pvec );
		if ( p_u < 0.0f || p_u > 1.0f ) return false;

		qvec = cross( tvec, edge1 );
		p_v = inv_det * dot( d, qvec );
		if ( p_v < 0.0f || ( p_u + p_v ) > 1.0f ) return false;

		p_t = inv_det * dot( edge2, qvec );

		if ( p_t > epsilon ) // ray intersection
		{
			return true;
		}
		else return false;
	}

	const Vec3f TriangleMeshGeometry::getNormalInterpolation( float & p_u, float & p_v ) const {
		Vec3f n = ( 1 - p_u - p_v ) * _refMesh->_normals[ _v0 ] + p_u * _refMesh->_normals[ _v1 ]
				  + p_v * _refMesh->_normals[ _v2 ];
		return n;
	}
} // namespace RT_ISICG
