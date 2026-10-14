#pragma once
#include "godot_cpp/classes/array_mesh.hpp"
#include "godot_cpp/classes/surface_tool.hpp"
#include <igl/per_face_normals.h>
#include <Eigen/Dense>

namespace MeshUtils {

godot::Ref<godot::ArrayMesh> toArrayMesh(const Eigen::MatrixXf &V, const Eigen::MatrixXi &F) {
	Eigen::MatrixXf N;
	igl::per_face_normals(V, F, N);

	godot::PackedVector3Array verts, normals;
	godot::PackedInt32Array indices;

	for (int row = 0; row < F.rows(); row++) {
		Eigen::Vector3i vertex_ids = F.row(row);
		Eigen::Vector3f face_normal = N.row(row);
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

void create_cube(Eigen::Matrix<float, Eigen::Dynamic, 3> &out_V, Eigen::Matrix<int, Eigen::Dynamic, 3> &out_F) {
	out_V = Eigen::Matrix<float, Eigen::Dynamic, 3>{
		{ -1, -1, -1 }, //0
		{ -1, -1, 1 }, //1
		{ 1, -1, -1 }, //2
		{ 1, -1, 1 }, //3
		{ -1, 1, -1 }, //4
		{ -1, 1, 1 }, //5
		{ 1, 1, -1 }, //6
		{ 1, 1, 1 }, //7
	};

	out_F = Eigen::Matrix<int, Eigen::Dynamic, 3>{
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
} //namespace MeshUtils
