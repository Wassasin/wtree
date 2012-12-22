#pragma once

#include "wtree.hpp"

template<std::size_t D>
class wtree_counter
{
public:
	typedef wtree<D> tree_t;
	typedef typename tree_t::element_t element_t;
	typedef std::array<std::array<typename tree_t::component_t, 2>, D> bounds_t;

private:
	static bool within_bounds(const element_t& e, const bounds_t& b)
	{
		for(size_t d = 0; d < D; d++)
			if(e[d] < b[d][0] || e[d] > b[d][1])
				return false;
		
		return true;
	}
	
	static bool within_bounds(const bounds_t& x, const bounds_t& b)
	{
		for(size_t d = 0; d < D; d++)
			if(x[d][0] < b[d][0] || x[d][1] > b[d][1])
				return false;
		
		return true;
	}
	
	static bool overlap_bounds(const bounds_t& x, const bounds_t& b)
	{
		for(size_t d = 0; d < D; d++)
			if(
				(x[d][0] < b[d][0] && x[d][1] < b[d][0]) ||
				(x[d][0] > b[d][1] && x[d][1] > b[d][1])
			)
				return false;
		
		return true;
	}

	static size_t count(const tree_t& t, const bounds_t& tb, const bounds_t& b)
	{
		size_t result = 0;
		
		if(within_bounds(t.element, b))
			result++;
		
		for(size_t i = 0; i < tree_t::node_count; i++)
		{
			if(t.nodes[i].count == 0)
				continue;
		
			bounds_t local = tb;
			
			for(size_t d = 0; d < D; d++)
				local[d][i & (1 << d) ? 0 : 1] = t.element[d];
		
			if(within_bounds(local, b))
			{
				result += t.nodes[i].count;
				continue;
			}
			
			if(overlap_bounds(local, b))
				result += count(*t.nodes[i].wtree, local, b);
		}
		
		return result;
	}

public:
	static size_t count(const tree_t& t, const bounds_t& b)
	{
		bounds_t tb;
		for(size_t d = 0; d < D; d++)
			tb[d] = {{0.0, 1.0}};
	
		return count(t, tb, b);
	}
};
