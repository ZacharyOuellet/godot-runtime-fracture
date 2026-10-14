#include "destructible_mesh.h"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "mesh_utils.h"

void DestructibleMesh::_bind_methods() {
	MeshInstance3D::_bind_methods();
}

void DestructibleMesh::_ready() {
	MeshInstance3D::_ready();
	bool meshLoaded = MeshUtils::to_igl_mesh(get_mesh(), V, F);
	if (!meshLoaded) {
		MeshUtils::create_cube(V, F);
	}
	display_mesh();
}

void DestructibleMesh::display_mesh() {
	set_mesh(MeshUtils::toArrayMesh(V, F));
}
