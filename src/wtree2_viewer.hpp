#pragma once

#include "wtree.hpp"

class wtree2_viewer
{
private:
	static const wtree<2>* tree_ptr;

	wtree2_viewer() {}
	static void draw();
	
public:
	static void run(int argc, char** argv, const wtree<2>& tree);
};
