#pragma once

#include <array>
#include <memory>
#include <cmath>
#include <boost/optional.hpp>

#include "constexpr.hpp"

template<std::size_t D, typename DT = float, typename count_t = std::size_t>
class wtree
{
public:
	static constexpr size_t dimension = D;
	static constexpr size_t node_count = constpow(2, dimension);

	typedef std::unique_ptr<wtree> wtree_ptr;
	typedef DT component_t;
	typedef std::array<component_t, dimension> element_t;

	struct node
	{
		count_t count;
		wtree_ptr wtree;

		node()
		: count(0)
		, wtree()
		{}
	};
	
	typedef std::array<node, node_count> nodes_t;

	nodes_t nodes;
	element_t element;

	wtree(const element_t& e)
	: nodes()
	, element(e)
	{}
	
	size_t find_index(const element_t& e)
	{
		size_t result = 0;
	
		for(size_t d = 0; d < dimension; d++)
			if(e[d] >= element[d])
				result += 1 << d;
		
		return result;
	}
	
	void insert(const element_t& e)
	{
		const size_t i = find_index(e);
		
		if(nodes[i].count++ > 0)
			nodes[i].wtree->insert(e);
		else
			nodes[i].wtree.reset(new wtree(e));
	}
};
