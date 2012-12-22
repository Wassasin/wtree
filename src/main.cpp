#include "wtree.hpp"

#include "boost/random.hpp" 
#include <iostream>
#include <ctime>

#include "wtree2_viewer.hpp"
#include "wtree3_viewer.hpp"

typedef wtree<2> tree_t;
typedef tree_t::element_t element_t;

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

float distance(element_t x, element_t y)
{
	float result = 0.0;
	
	for(size_t i = 0; i < x.size(); i++)
		result += std::pow(x[i] - y[i], 2.0);
	
	return std::pow(result, 0.5);
}

int main(int argc, char** argv)
{
	const size_t n = 1000;

	distribution_t distribution(0.0, 1.0);
	twister_t twister;
	generator_t g(twister, distribution);
	twister.seed(std::time(0)); // seed with the current time

	std::cout << "Building a " << tree_t::dimension << "-dimensional tree with " << n << " elements, approximately " << sizeof(tree_t) * n << " bytes" << std::endl;

	std::vector<element_t> els;
	els.reserve(n);
	
	for(size_t i = 0; i < n; i++)
		els.emplace_back(create_element(g));
	
	std::cout << "Dataset generated" << std::endl;
	
	/*
	std::sort(els.begin(), els.end(), [&](const element_t& x, const element_t& y) {
		return distance(x, {{0.5, 0.5, 0.5}}) < distance(y, {{0.5, 0.5, 0.5}});
	});
	*/
	
	std::sort(els.begin(), els.end(), [&](const element_t& x, const element_t& y) {
		return distance(x, {{0.5, 0.5}}) < distance(y, {{0.5, 0.5}});
	});
	
	std::cout << "Dataset sorted" << std::endl;
	
	assert(!els.empty());

	tree_t t(els[0]);
	for(size_t i = 1; i < n; i++)
		t.insert(els[i]);

	std::cout << "Tree built" << std::endl;
	
	wtree2_viewer::run(argc, argv, t);
}
