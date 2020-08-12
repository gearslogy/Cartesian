#pragma once
#include "sol.hpp"

#define TEST_RUN_CASE(CLASS,LUA) CLASS::creator()->TEST(LUA);


class TEST_CASE {
public:
	virtual void TEST(sol::state* lua) = 0;
	
};