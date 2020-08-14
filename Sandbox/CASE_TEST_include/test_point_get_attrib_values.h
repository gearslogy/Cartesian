#pragma once

#include "test_base.h"


class TEST_GET_ATTRIB_VALUES :public TEST_CASE
{
public:
	static std::shared_ptr<TEST_GET_ATTRIB_VALUES> creator() {
		return std::make_shared<TEST_GET_ATTRIB_VALUES>();
	}

	void TEST(sol::state* lua);
};

