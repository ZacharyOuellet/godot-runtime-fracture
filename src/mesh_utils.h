#pragma once
#include "godot_cpp/classes/array_mesh.hpp"
#include "godot_cpp/classes/mesh.hpp"
#include "godot_cpp/classes/surface_tool.hpp"

#include <igl/per_face_normals.h>
#include <igl/remove_duplicate_vertices.h>
#include <Eigen/Dense>

namespace MeshUtils {

godot::Ref<godot::ArrayMesh> toArrayMesh(const Eigen::MatrixX3d &V, const Eigen::MatrixX3i &F) {
	Eigen::MatrixX3d N;
	igl::per_face_normals(V, F, N);

	godot::PackedVector3Array verts, normals;
	godot::PackedInt32Array indices;

	for (int row = 0; row < F.rows(); row++) {
		Eigen::Vector3i vertex_ids = F.row(row);
		Eigen::Vector3d face_normal = N.row(row);
		// Change winding order because godot is clockwise
		for (int i : { 1, 0, 2 }) {
			indices.append(verts.size());
			verts.append(godot::Vector3(V(vertex_ids(i), 0), V(vertex_ids(i), 1), V(vertex_ids(i), 2)));
			normals.append(godot::Vector3(face_normal[0], face_normal[1], face_normal[2]));
		}
	}

	godot::Array surface_array;
	surface_array.resize(godot::Mesh::ARRAY_MAX);
	surface_array[godot::Mesh::ARRAY_VERTEX] = verts;
	surface_array[godot::Mesh::ARRAY_INDEX] = indices;
	surface_array[godot::Mesh::ARRAY_NORMAL] = normals;

	godot::Ref<godot::ArrayMesh> mesh;
	mesh.instantiate();
	mesh->add_surface_from_arrays(godot::Mesh::PRIMITIVE_TRIANGLES, surface_array);
	return mesh;
}

void create_cube(Eigen::MatrixX3d &out_V, Eigen::MatrixX3i &out_F) {
	out_V = Eigen::MatrixX3d{
		{ -1, -1, -1 }, //0
		{ -1, -1, 1 }, //1
		{ 1, -1, -1 }, //2
		{ 1, -1, 1 }, //3
		{ -1, 1, -1 }, //4
		{ -1, 1, 1 }, //5
		{ 1, 1, -1 }, //6
		{ 1, 1, 1 }, //7
	};

	out_F = Eigen::MatrixX3i{
		//bottom
		{ 0, 3, 1 },
		{ 0, 2, 3 },

		//top
		{ 4, 5, 7 },
		{ 4, 7, 6 },

		//front
		{ 1, 7, 5 },
		{ 1, 3, 7 },

		//back
		{ 2, 4, 6 },
		{ 2, 0, 4 },

		//left
		{ 0, 5, 4 },
		{ 0, 1, 5 },

		// right
		{ 3, 6, 7 },
		{ 3, 2, 6 },
	};
}

bool to_igl_mesh(godot::Ref<godot::Mesh> mesh, Eigen::MatrixX3d &out_V, Eigen::MatrixX3i &out_F) {
	if (mesh.is_null()) {
		return false;
	}

	godot::Array surface_array = mesh->surface_get_arrays(0);
	godot::PackedVector3Array verts = surface_array[godot::Mesh::ARRAY_VERTEX];
	godot::PackedInt32Array indices = surface_array[godot::Mesh::ARRAY_INDEX];

	Eigen::MatrixX3d temp_V(verts.size(), 3);
	for (int i = 0; i < verts.size(); i++) {
		godot::Vector3 v = verts[i];
		temp_V(i, 0) = v.x;
		temp_V(i, 1) = v.y;
		temp_V(i, 2) = v.z;
	}

	Eigen::MatrixX3i temp_F(indices.size() / 3, 3);
	for (int i = 0; i < indices.size() / 3; i++) {
		// Change winding order because godot is clockwise
		temp_F(i, 0) = indices[i * 3 + 1];
		temp_F(i, 1) = indices[i * 3 + 0];
		temp_F(i, 2) = indices[i * 3 + 2];
	}
	Eigen::VectorXi SVI, SVJ;
	igl::remove_duplicate_vertices(temp_V, temp_F, 1e-6, out_V, SVI, SVJ, out_F);
	return true;
}
} //namespace MeshUtils
