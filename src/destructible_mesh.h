#pragma once

#include "Eigen/Dense"
#include "godot_cpp/classes/mesh_instance3d.hpp"
class DestructibleMesh : public godot::MeshInstance3D {
	GDCLASS(DestructibleMesh, godot::MeshInstance3D)

private:
	Eigen::MatrixX3d V;
	Eigen::MatrixX3i F;

protected:
	static void _bind_methods();

public:
	DestructibleMesh() = default;
	~DestructibleMesh() override = default;
	virtual void _ready() override;
	void display_mesh();
};
