#pragma once

constexpr unsigned int constpow(unsigned int x, unsigned int y)
{
	return y == 0 ? 1 : x * constpow(x, y-1);
}
