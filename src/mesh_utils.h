#pragma once
#include "godot_cpp/classes/array_mesh.hpp"
#include <Eigen/Dense>

namespace MeshUtils {
godot::Ref<godot::ArrayMesh> toArrayMesh(const Eigen::Matrix<float, Eigen::Dynamic, 3> &vertices, const Eigen::Matrix<int, Eigen::Dynamic, 3> &faces) {
	godot::Array surface_array;
	surface_array.resize(godot::Mesh::ARRAY_MAX);

	godot::PackedVector3Array verts;
	for (int row = 0; row < vertices.rows(); row++) {
		Eigen::Vector3f pos = vertices.row(row);
		verts.append(godot::Vector3(pos[0], pos[1], pos[2]));
	}

	godot::PackedInt32Array indices;
	for (int row = 0; row < faces.rows(); row++) {
		Eigen::Vector3i pos = faces.row(row);
		indices.append(pos[0]);
		indices.append(pos[1]);
		indices.append(pos[2]);
	}
	surface_array[godot::Mesh::ARRAY_VERTEX] = verts;
	surface_array[godot::Mesh::ARRAY_INDEX] = indices;

	godot::Ref<godot::ArrayMesh> array_mesh;
	array_mesh.instantiate();
	array_mesh->add_surface_from_arrays(godot::Mesh::PRIMITIVE_TRIANGLES, surface_array);
	return array_mesh;
}

void create_cube(Eigen::Matrix<float, Eigen::Dynamic, 3> &out_V, Eigen::Matrix<int, Eigen::Dynamic, 3> &out_F) {
	out_V = Eigen::Matrix<float, Eigen::Dynamic, 3>{
		{ -1, -1, 1 }, //0
		{ 1, -1, 1 }, //1
		{ -1, 1, 1 }, //2
		{ 1, 1, 1 }, //3
		{ -1, -1, -1 }, //4
		{ 1, -1, -1 }, //5
		{ -1, 1, -1 }, //6
		{ 1, 1, -1 } //7
	};

	out_F = Eigen::Matrix<int, Eigen::Dynamic, 3>{
		//top
		{ 2, 6, 7 },
		{ 2, 3, 7 },

		//Bottom
		{ 0, 4, 5 },
		{ 0, 1, 5 },

		//Left
		{ 0, 2, 6 },
		{ 0, 4, 6 },

		//Right
		{ 1, 3, 7 },
		{ 1, 5, 7 },

		//Front
		{ 0, 2, 3 },
		{ 0, 1, 3 },

		//Back
		{ 4, 6, 7 },
		{ 4, 5, 7 },
	};
}
} //namespace MeshUtils
