#ifndef _GINA_MATH_H_
#define _GINA_MATH_H_

#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>
#include <cstring>

#if defined(_MSC_VER)
    #include <intrin.h>
    #if defined(_M_IX86_FP) && _M_IX86_FP >= 2
        #define GINA_SSE2_ENABLED 1
    #endif
#elif defined(__SSE2__)
    #include <x86intrin.h>
    #define GINA_SSE2_ENABLED 1
#endif

namespace gina
{
    namespace detail 
    {
        struct BasicMathImpl
        {
            static float length2(float x, float y) noexcept;
            static float dot2(float ax, float ay, float bx, float by) noexcept;
            static void normalize2(float& x, float& y) noexcept;
            static void add2(float& rx, float& ry, float ax, float ay, float bx, float by) noexcept;
            static void sub2(float& rx, float& ry, float ax, float ay, float bx, float by) noexcept;
            static void mul2(float& rx, float& ry, float x, float y, float scalar) noexcept;
            static void div2(float& rx, float& ry, float x, float y, float scalar) noexcept;
            static void lerp2(float& rx, float& ry, float ax, float ay, float bx, float by, float t) noexcept;
        };

        #if defined(GINA_SSE2_ENABLED)
        struct SSE2MathImpl
        {
            static float length2(float x, float y) noexcept;
            static float dot2(float ax, float ay, float bx, float by) noexcept;
            static void normalize2(float& x, float& y) noexcept;
            static void add2(float& rx, float& ry, float ax, float ay, float bx, float by) noexcept;
            static void sub2(float& rx, float& ry, float ax, float ay, float bx, float by) noexcept;
            static void mul2(float& rx, float& ry, float x, float y, float scalar) noexcept;
            static void div2(float& rx, float& ry, float x, float y, float scalar) noexcept;
            static void lerp2(float& rx, float& ry, float ax, float ay, float bx, float by, float t) noexcept;
        };
        #endif

        struct MathDispatch 
        {
            using Length2Func = float(*)(float, float);
            using Dot2Func = float(*)(float, float, float, float);
            using Normalize2Func = void(*)(float&, float&);
            using Add2Func = void(*)(float&, float&, float, float, float, float);
            using Sub2Func = void(*)(float&, float&, float, float, float, float);
            using Mul2Func = void(*)(float&, float&, float, float, float);
            using Div2Func = void(*)(float&, float&, float, float, float);
            using Lerp2Func = void(*)(float&, float&, float, float, float, float, float);

            static Length2Func length2Impl;
            static Dot2Func dot2Impl;
            static Normalize2Func normalize2Impl;
            static Add2Func add2Impl;
            static Sub2Func sub2Impl;
            static Mul2Func mul2Impl;
            static Div2Func div2Impl;
            static Lerp2Func lerp2Impl;

            static void initialize() noexcept;
            static void useSSE2() noexcept;
            static void useBasic() noexcept;

        private:
            static bool initialized;
            static bool isSSE2Supported() noexcept;
        };
    }

    constexpr float EPSILON = 1e-5f;
    constexpr float PI = 3.14159265358979323846f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;

    /**
     * Fast inverse square root approximation (1/sqrt(x))
     * 
     * Famous algorithm from Quake III Arena with the magic number 0x5f3759df
     * 
     * Algorithm steps:
     * 1. Bit-level manipulation of floating point representation
     * 2. Initial approximation using magic number subtraction
     * 3. One Newton-Raphson iteration for refinement
     * 
     * Reference: 
     * - Origin: https://en.wikipedia.org/wiki/Fast_inverse_square_root
     * - IEEE 754 floating-point format exploitation
     * - Newton's method for root approximation refinement
     * 
     * Note: Less accurate than std::sqrt but much faster (~4x)
     *       Accuracy ~0.175% relative error
     */
    inline float fastInvSqrt(float x) noexcept
    {
        static_assert(sizeof(float) == sizeof(int), "Float and int size mismatch");
        
        float xhalf = 0.5f * x;
        int i;
        std::memcpy(&i, &x, sizeof(float));
        i = 0x5f3759df - (i >> 1);
        std::memcpy(&x, &i, sizeof(float));
        x = x * (1.5f - xhalf * x * x);
        return x;
    }

    inline bool isZero(float x) noexcept
    {
        return std::fabs(x) < EPSILON;
    }

    constexpr float ConvertToRadians(float degrees) noexcept { return degrees * DEG_TO_RAD; }
    constexpr float ConvertToDegrees(float radians) noexcept { return radians * RAD_TO_DEG; }

    struct float1
    {
        float value;

        float1() = default;
        constexpr float1(float v) : value(v) {}

        constexpr operator float() const { return value; }
    };

    class float2
    {
    public:
        float x, y;

        constexpr float2() noexcept : x(0), y(0) {}
        constexpr float2(float x, float y) noexcept : x(x), y(y) {}
        
        float* data() noexcept { return &x; }
        const float* data() const noexcept { return &x; }

        static const float2 Zero;

        float lengthSquared() const noexcept;
        float length() const noexcept;
        float2 normalized() const noexcept;
        void normalize() noexcept;
        bool isZero() const noexcept;

        float2& operator+=(const float2& other) noexcept;
        float2& operator-=(const float2& other) noexcept;
        float2& operator*=(float scalar) noexcept;
        float2& operator/=(float scalar) noexcept;
        bool operator==(const float2& other) const noexcept;
        bool operator!=(const float2& other) const noexcept;
        float2 operator-() const noexcept;
    };

    float dot(const float2& a, const float2& b) noexcept;
    float2 reflect(const float2& vec, const float2& normal) noexcept;
    float2 project(const float2& vec, const float2& onto) noexcept;
    float distance(const float2& a, const float2& b) noexcept;
    float angle(const float2& a, const float2& b) noexcept;
    float2 lerp(const float2& a, const float2& b, float t) noexcept;
    float2 operator+(const float2& lhs, const float2& rhs) noexcept;
    float2 operator-(const float2& lhs, const float2& rhs) noexcept;
    float2 operator*(const float2& vec, float scalar) noexcept;
    float2 operator*(float scalar, const float2& vec) noexcept;
    float2 operator/(const float2& vec, float scalar) noexcept;
}

#endif // _GINA_MATH_H_