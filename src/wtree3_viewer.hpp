#pragma once

#include <boost/optional.hpp>
#include <functional>

#include "wtree.hpp"

class wtree3_viewer
{
private:
	wtree3_viewer() {}
	static void draw();
	
public:
	static void run(int argc, char** argv, const wtree<3>& tree);
};
