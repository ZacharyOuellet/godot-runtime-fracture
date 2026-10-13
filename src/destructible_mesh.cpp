#include "destructible_mesh.h"
#include "mesh_utils.h"

void DestructibleMesh::_bind_methods() {
	MeshInstance3D::_bind_methods();
}

void DestructibleMesh::_ready() {
	MeshInstance3D::_ready();
	MeshUtils::create_cube(V, F);
	display_mesh();
}

void DestructibleMesh::display_mesh() {
	set_mesh(MeshUtils::toArrayMesh(V, F));
}
