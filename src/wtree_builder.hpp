#pragma once

#include <list>

#include "wtree.hpp"
#include "distance.hpp"

template<std::size_t D>
class wtree_builder
{
public:
	typedef wtree<D> tree_t;	
	typedef typename tree_t::element_t element_t;
	typedef std::list<element_t> list_t;
	
	typedef std::array<std::array<typename tree_t::component_t, 2>, D> bounds_t;

private:
	static typename list_t::iterator find_best_match(list_t& l, const element_t& goal)
	{
		float best_distance = std::numeric_limits<float>::max();
		typename list_t::iterator best_match;
		
		for(auto i = l.begin(); i != l.end(); i++)
		{
			float d = distance<D>(*i, goal);
		
			if(best_distance <= d)
				continue;
			
			best_match = i;
			best_distance = d;
		}
		
		return best_match;
	}
	
	static element_t derive_center(const bounds_t& b)
	{
		element_t result;
		for(size_t d = 0; d < D; d++)
			result[d] = 0.5 * (b[d][0] + b[d][1]);
		
		return result;
	}

	static void place(tree_t& t, const bounds_t& b, list_t& l)
	{
		std::array<list_t, tree_t::node_count> lists;
		
		while(!l.empty())
		{
			const element_t& x = l.front();
		
			lists[t.find_index(x)].emplace_back(x);
			l.pop_front();
		}
	
		for(size_t i = 0; i < tree_t::node_count; i++)
		{
			if(lists[i].empty())
				continue;
			
			bounds_t local = b;
			
			for(size_t d = 0; d < D; d++)
				local[d][i & (1 << d) ? 0 : 1] = t.element[d];
			
			auto best = find_best_match(lists[i], derive_center(local));
			t.insert(*best);
			lists[i].erase(best);
			
			place(*t.nodes[i].wtree, local, lists[i]);
		}
	}

public:
	static tree_t create(std::list<element_t>& l)
	{
		bounds_t b;
		for(size_t d = 0; d < D; d++)
			b[d] = {{0.0, 1.0}};
	
		auto best = find_best_match(l, derive_center(b));
		tree_t result(*best);
		l.erase(best);
	
		place(result, b, l);
	
		return result;
	}
};
