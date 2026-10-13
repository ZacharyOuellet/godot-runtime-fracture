#pragma once
#include "godot_cpp/classes/array_mesh.hpp"
#include <igl/per_vertex_normals.h>
#include <Eigen/Dense>

namespace MeshUtils {
godot::Ref<godot::ArrayMesh> toArrayMesh(const Eigen::Matrix<float, Eigen::Dynamic, 3> &V, const Eigen::Matrix<int, Eigen::Dynamic, 3> &F) {
	godot::Array surface_array;
	surface_array.resize(godot::Mesh::ARRAY_MAX);

	godot::PackedVector3Array verts;
	for (int row = 0; row < V.rows(); row++) {
		Eigen::Vector3f pos = V.row(row);
		verts.append(godot::Vector3(pos[0], pos[1], pos[2]));
	}

	godot::PackedInt32Array indices;
	for (int row = 0; row < F.rows(); row++) {
		Eigen::Vector3i face = F.row(row);
		indices.append(face[0]);
		indices.append(face[1]);
		indices.append(face[2]);
	}

	Eigen::MatrixXf N;
	igl::per_vertex_normals(V, F, N);
	N = -N; // Since godot is clockwise
	godot::PackedVector3Array normals;
	for (int row = 0; row < N.rows(); row++) {
		Eigen::Vector3f normal = N.row(row);
		normals.append(godot::Vector3(normal[0], normal[1], normal[2]));
	}

	surface_array[godot::Mesh::ARRAY_VERTEX] = verts;
	surface_array[godot::Mesh::ARRAY_INDEX] = indices;
	surface_array[godot::Mesh::ARRAY_NORMAL] = normals;
	godot::Ref<godot::ArrayMesh> array_mesh;
	array_mesh.instantiate();
	array_mesh->add_surface_from_arrays(godot::Mesh::PRIMITIVE_TRIANGLES, surface_array);

	return array_mesh;
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
		{ 0, 1, 3 },
		{ 0, 3, 2 },

		//top
		{ 4, 7, 5 },
		{ 4, 6, 7 },

		//front
		{ 1, 5, 7 },
		{ 1, 7, 3 },

		//back
		{ 2, 6, 4 },
		{ 2, 4, 0 },

		//left
		{ 0, 4, 5 },
		{ 0, 5, 1 },

		// right
		{ 3, 7, 6 },
		{ 3, 6, 2 },
	};
}
} //namespace MeshUtils
