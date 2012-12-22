#include <boost/random.hpp>
#include <iostream>
#include <ctime>
#include <list>

#include "wtree.hpp"

#include "wtree2_viewer.hpp"
#include "wtree3_viewer.hpp"

#include "wtree_builder.hpp"
#include "wtree_counter.hpp"

typedef wtree<3> tree_t;
typedef tree_t::element_t element_t;
typename wtree_counter<tree_t::dimension>::bounds_t b;

typedef boost::uniform_real<> distribution_t; 
typedef boost::mt19937 twister_t; 
typedef boost::variate_generator<twister_t&, distribution_t> generator_t;

element_t create_element(generator_t& g)
{
	element_t e;
	
	for(size_t i = 0; i < e.size(); i++)
		e[i] = g();
		
	return e;
}

int main(int argc, char** argv)
{
	const size_t n = 1e3;

	distribution_t distribution(0.0, 1.0);
	twister_t twister;
	generator_t g(twister, distribution);
	twister.seed(std::time(0)); // seed with the current time

	std::cout << "Building a " << tree_t::dimension << "-dimensional tree with " << n << " elements, approximately " << sizeof(tree_t) * n << " bytes" << std::endl;

	std::list<element_t> els;
	
	for(size_t i = 0; i < n; i++)
		els.emplace_back(create_element(g));
	
	std::cout << "Dataset generated" << std::endl;
	
	tree_t t = wtree_builder<tree_t::dimension>::create(els);
	
	std::cout << "Tree built" << std::endl;
	
	for(float i = 0.0; i <= 1.0; i += 0.01)
	{
		for(size_t d = 0; d < tree_t::dimension; d++)
			b[d] = {{0.0, i}};
	
		std::cout << "Currently " << wtree_counter<tree_t::dimension>::count(t, b) << " elements between 0.0 and " << i << std::endl;
	}
	
	wtree3_viewer::run(argc, argv, t);
}
