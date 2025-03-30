#include <gtest/gtest.h>
#include "gina.h"

using namespace gina;

TEST(gina_test, Sum) 
{
    EXPECT_EQ(Sum(1, 2), 3);
    EXPECT_EQ(Sum(2, 7), 9);
    EXPECT_EQ(Sum(0, 0), 0);
}
