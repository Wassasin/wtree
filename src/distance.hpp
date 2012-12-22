#pragma once

#include <array>

template<std::size_t D = 2, typename T = float, typename RT = float>
RT distance(const std::array<T, D>& x, const std::array<T, D>& y)
{
	RT result = 0.0;
	
	for(size_t i = 0; i < x.size(); i++)
		result += std::pow(x[i] - y[i], 2.0);
	
	return std::pow(result, 0.5);
}
