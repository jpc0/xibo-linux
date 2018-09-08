#pragma once

#include "adaptors/IFixedLayoutAdaptor.hpp"

#include <gmock/gmock.h>

class MockFixedLayoutAdaptor : public IFixedLayoutAdaptor
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_METHOD3(addChild, void(IWidgetAdaptor& child, int left, int top));
    MOCK_METHOD0(removeChildren, void());
    MOCK_METHOD0(show, void());
    MOCK_METHOD1(apply, void(AdaptorVisitor& visitor));
};
