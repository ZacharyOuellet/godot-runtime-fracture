#include "example_class.h"
#include <igl/cotmatrix.h>
#include <Eigen/Dense>
#include <Eigen/Sparse>

void ExampleClass::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("hello_world"), &ExampleClass::hello_world);
}

void ExampleClass::hello_world() const {
	print_line("Hello from c++ with libigl");

	Eigen::MatrixXd V(4, 2);
	V << 0, 0,
		1, 0,
		1, 1,
		0, 1;
	Eigen::MatrixXi F(2, 3);
	F << 0, 1, 2,
		0, 2, 3;
	Eigen::SparseMatrix<double> L;
	igl::cotmatrix(V, F, L);
	std::ostringstream ss;
	ss << L * V;

	print_line(vformat("L * V:\n%s", String::utf8(ss.str().c_str())));
}
