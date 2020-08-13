#pragma once
#include "test_base.h"

class TEST_POINT_ADD_SET_GET :public TEST_CASE
{
public:
	static std::shared_ptr<TEST_POINT_ADD_SET_GET> creator() {
		return std::make_shared<TEST_POINT_ADD_SET_GET>();
	}

	void TEST(sol::state* lua);
};

