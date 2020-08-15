// 2020/8/15
// gearslogy liuyangping207@qq.com

#pragma once

#include "test_base.h"


class TEST_ADD_SET_GET_MATRIX :public TEST_CASE
{
public:
	static std::shared_ptr<TEST_ADD_SET_GET_MATRIX> creator() {
		return std::make_shared<TEST_ADD_SET_GET_MATRIX>();
	}

	void TEST(sol::state* lua);
};
