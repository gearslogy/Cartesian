#pragma once

#include "test_base.h"


class TEST_POINT_ATTRIB_TABLE :public TEST_CASE {
public:

	static std::shared_ptr<TEST_POINT_ATTRIB_TABLE> creator() {
		return std::make_shared<TEST_POINT_ATTRIB_TABLE>();
	}

	void TEST(sol::state* lua);

};
