#include <gtest/gtest.h>
#include "core/gina_math.h"

using namespace gina;

TEST(MathTest, Float2BasicOperations)
{
    float2 a(1.0f, 2.0f);
    float2 b(3.0f, 4.0f);

    float2 sum = a + b;
    EXPECT_FLOAT_EQ(sum.x, 4.0f);
    EXPECT_FLOAT_EQ(sum.y, 6.0f);

    float2 diff = b - a;
    EXPECT_FLOAT_EQ(diff.x, 2.0f);
    EXPECT_FLOAT_EQ(diff.y, 2.0f);

    float2 scaled = a * 2.0f;
    EXPECT_FLOAT_EQ(scaled.x, 2.0f);
    EXPECT_FLOAT_EQ(scaled.y, 4.0f);

    float2 divided = b / 2.0f;
    EXPECT_FLOAT_EQ(divided.x, 1.5f);
    EXPECT_FLOAT_EQ(divided.y, 2.0f);
}

TEST(MathTest, Float2Length)
{
    float2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.lengthSquared(), 25.0f);
    EXPECT_FLOAT_EQ(v.length(), 5.0f);
}

TEST(MathTest, Float2Normalize)
{
    float2 v(3.0f, 4.0f);
    float2 normalized = v.normalized();
    const float TEST_EPSILON = 1e-3f;
    EXPECT_NEAR(normalized.length(), 1.0f, TEST_EPSILON);
    EXPECT_NEAR(normalized.x, 0.6f, TEST_EPSILON);
    EXPECT_NEAR(normalized.y, 0.8f, TEST_EPSILON);
}

TEST(MathTest, Float2Dot)
{
    float2 a(1.0f, 2.0f);
    float2 b(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(dot(a, b), 11.0f);
}

TEST(MathTest, Float2Lerp)
{
    float2 a(1.0f, 2.0f);
    float2 b(3.0f, 4.0f);
    float2 result = lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 3.0f);
}

TEST(MathTest, Float2Reflect)
{
    float2 vec(1.0f, 1.0f);
    float2 normal(0.0f, 1.0f);
    float2 reflected = reflect(vec, normal);
    EXPECT_FLOAT_EQ(reflected.x, 1.0f);
    EXPECT_FLOAT_EQ(reflected.y, -1.0f);
}

TEST(MathTest, Float2Project)
{
    float2 vec(3.0f, 4.0f);
    float2 onto(1.0f, 0.0f);
    float2 projected = project(vec, onto);
    EXPECT_FLOAT_EQ(projected.x, 3.0f);
    EXPECT_FLOAT_EQ(projected.y, 0.0f);
}

TEST(MathTest, Float2Distance)
{
    float2 a(1.0f, 1.0f);
    float2 b(4.0f, 5.0f);
    EXPECT_FLOAT_EQ(distance(a, b), 5.0f);
}

TEST(MathTest, Float2Angle)
{
    float2 a(1.0f, 0.0f);
    float2 b(0.0f, 1.0f);
    float angleRad = angle(a, b);
    EXPECT_FLOAT_EQ(angleRad, PI / 2.0f);
}

TEST(MathTest, FastInvSqrt)
{
    float x = 4.0f;
    float invSqrt = fastInvSqrt(x);
    float expected = 1.0f / std::sqrt(x);
    EXPECT_NEAR(invSqrt, expected, 0.001f);
}

TEST(MathTest, MathDispatch)
{
    EXPECT_NE(detail::MathDispatch::length2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::dot2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::normalize2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::add2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::sub2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::mul2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::div2Impl, nullptr);
    EXPECT_NE(detail::MathDispatch::lerp2Impl, nullptr);
} 