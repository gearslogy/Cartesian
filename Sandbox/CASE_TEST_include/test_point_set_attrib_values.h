#pragma once

#include "test_base.h"


class TEST_POINT_SET_ATTRIB_VALUES :public TEST_CASE
{
public:
	static std::shared_ptr<TEST_POINT_SET_ATTRIB_VALUES> creator() {
		return std::make_shared<TEST_POINT_SET_ATTRIB_VALUES>();
	}

	void TEST(sol::state* lua);
};

