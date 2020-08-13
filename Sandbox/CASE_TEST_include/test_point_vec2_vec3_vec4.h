#pragma once

#include "test_base.h"


class TEST_POINT_VEC2_VEC3_VEC4 :public TEST_CASE
{
public:
	static std::shared_ptr<TEST_POINT_VEC2_VEC3_VEC4> creator() {
		return std::make_shared<TEST_POINT_VEC2_VEC3_VEC4>();
	}

	void TEST(sol::state* lua);
};

