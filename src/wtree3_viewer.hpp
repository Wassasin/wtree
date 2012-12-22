#pragma once

#include "wtree.hpp"

class wtree3_viewer
{
private:
	static const wtree<3>* tree_ptr;
	static uint64_t i;
	
	wtree3_viewer() {}
	static void draw();
	
public:
	static void run(int argc, char** argv, const wtree<3>& tree);
};
