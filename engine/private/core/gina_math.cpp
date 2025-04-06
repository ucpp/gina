#include "core/gina_math.h"

namespace gina
{
    namespace detail
    {
        MathDispatch::Length2Func MathDispatch::length2Impl = nullptr;
        MathDispatch::Dot2Func MathDispatch::dot2Impl = nullptr;
        MathDispatch::Normalize2Func MathDispatch::normalize2Impl = nullptr;
        MathDispatch::Add2Func MathDispatch::add2Impl = nullptr;
        MathDispatch::Sub2Func MathDispatch::sub2Impl = nullptr;
        MathDispatch::Mul2Func MathDispatch::mul2Impl = nullptr;
        MathDispatch::Div2Func MathDispatch::div2Impl = nullptr;
        MathDispatch::Lerp2Func MathDispatch::lerp2Impl = nullptr;
        bool MathDispatch::initialized = (MathDispatch::initialize(), true);

        bool MathDispatch::isSSE2Supported() noexcept
        {
#if defined(GINA_SSE2_ENABLED)
    #if defined(_MSC_VER)
            int info[4];
            __cpuid(info, 1);
            return (info[3] & (1 << 26)) != 0;
    #else
            unsigned int eax, ebx, ecx, edx;
            __cpuid(1, eax, ebx, ecx, edx);
            return (edx & (1 << 26)) != 0;
    #endif
#else
            return false;
#endif
        }

        float BasicMathImpl::length2(const float2& vec) noexcept
        {
            return vec.x * vec.x + vec.y * vec.y;
        }

        float BasicMathImpl::dot2(const float2& lhs, const float2& rhs) noexcept
        {
            return lhs.x * rhs.x + lhs.y * rhs.y;
        }

        void BasicMathImpl::normalize2(float2& vec) noexcept
        {
            const float lenSq = vec.x * vec.x + vec.y * vec.y;
            if (lenSq < EPSILON)
            {
                vec.x = vec.y = 0.0f;
                return;
            }

            const float invLen = 1.0f / std::sqrt(lenSq);
            vec.x *= invLen;
            vec.y *= invLen;
        }

        void BasicMathImpl::add2(float2& result, const float2& lhs, const float2& rhs) noexcept
        {
            result.x = lhs.x + rhs.x;
            result.y = lhs.y + rhs.y;
        }

        void BasicMathImpl::sub2(float2& result, const float2& lhs, const float2& rhs) noexcept
        {
            result.x = lhs.x - rhs.x;
            result.y = lhs.y - rhs.y;
        }

        void BasicMathImpl::mul2(float2& result, const float2& vec, float scalar) noexcept
        {
            result.x = vec.x * scalar;
            result.y = vec.y * scalar;
        }

        void BasicMathImpl::div2(float2& result, const float2& vec, float scalar) noexcept
        {
            if (std::fabs(scalar) < EPSILON)
            {
                result.x = result.y = 0.0f;
                return;
            }
            float invScalar = 1.0f / scalar;
            result.x = vec.x * invScalar;
            result.y = vec.y * invScalar;
        }

        void BasicMathImpl::lerp2(float2& result, const float2& lhs, const float2& rhs, float t) noexcept
        {
            result.x = lhs.x + (rhs.x - lhs.x) * t;
            result.y = lhs.y + (rhs.y - lhs.y) * t;
        }

#if defined(GINA_SSE2_ENABLED)
        float SSE2MathImpl::length2(const float2& vec) noexcept
        {
            __m128 v = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
            __m128 sq = _mm_mul_ps(v, v);
            return _mm_cvtss_f32(_mm_add_ss(sq, _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(1, 1, 1, 1))));
        }

        float SSE2MathImpl::dot2(const float2& lhs, const float2& rhs) noexcept
        {
            __m128 va = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
            __m128 vb = _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x);
            __m128 mul = _mm_mul_ps(va, vb);
            return _mm_cvtss_f32(_mm_add_ss(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 1, 1, 1))));
        }

        /**
         * Normalizes a 2D vector using SSE2 instructions
         * 
         * This implementation provides optimized vector normalization using:
         * - Parallel SSE2 operations for maximum speed
         * - Hardware-accelerated inverse square root approximation
         * - Single Newton-Raphson refinement step
         * 
         * The normalization process:
         * 1. Calculates vector length as sqrt(x*x + y*y)
         * 2. Divides both components by this length
         * 
         * Optimization details:
         * - Uses _mm_rsqrt_ss for initial inverse square root (12-bit precision)
         * - Applies one Newton-Raphson iteration (improves to 23-bit precision)
         * - Early check for zero-length vectors
         * - Efficient SIMD operations after initial check
         * 
         * Reference: Intel Intrinsics Guide documentation
         */
        void SSE2MathImpl::normalize2(float2& vec) noexcept
        {
            __m128 v = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
            __m128 sq = _mm_mul_ps(v, v);

            __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(1, 1, 1, 1)));
            if (_mm_cvtss_f32(sum) < EPSILON)
            {
                vec.x = vec.y = 0.0f;
                return;
            }

            __m128 rsqrt = _mm_rsqrt_ss(sum);
            const __m128 half = _mm_set_ss(0.5f);
            const __m128 three = _mm_set_ss(3.0f);

            __m128 nr = _mm_mul_ss(_mm_mul_ss(sum, rsqrt), rsqrt);
            rsqrt = _mm_mul_ss(rsqrt, _mm_sub_ss(three, nr));
            rsqrt = _mm_mul_ss(rsqrt, half);

            rsqrt = _mm_shuffle_ps(rsqrt, rsqrt, _MM_SHUFFLE(0, 0, 0, 0));
            v = _mm_mul_ps(v, rsqrt);

            _mm_store_ss(&vec.x, v);
            _mm_store_ss(&vec.y, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        void SSE2MathImpl::add2(float2& result, const float2& lhs, const float2& rhs) noexcept
        {
            __m128 va = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
            __m128 vb = _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x);
            __m128 res = _mm_add_ps(va, vb);
            _mm_store_ss(&result.x, res);
            _mm_store_ss(&result.y, _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        void SSE2MathImpl::sub2(float2& result, const float2& lhs, const float2& rhs) noexcept
        {
            __m128 va = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
            __m128 vb = _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x);
            __m128 res = _mm_sub_ps(va, vb);
            _mm_store_ss(&result.x, res);
            _mm_store_ss(&result.y, _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        void SSE2MathImpl::mul2(float2& result, const float2& vec, float scalar) noexcept
        {
            __m128 v = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
            __m128 s = _mm_set1_ps(scalar);
            __m128 res = _mm_mul_ps(v, s);
            _mm_store_ss(&result.x, res);
            _mm_store_ss(&result.y, _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        void SSE2MathImpl::div2(float2& result, const float2& vec, float scalar) noexcept
        {
            if (std::fabs(scalar) < EPSILON)
            {
                result.x = result.y = 0.0f;
                return;
            }
            __m128 v = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
            __m128 s = _mm_set1_ps(1.0f / scalar);
            __m128 res = _mm_mul_ps(v, s);
            _mm_store_ss(&result.x, res);
            _mm_store_ss(&result.y, _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        void SSE2MathImpl::lerp2(float2& result, const float2& lhs, const float2& rhs, float t) noexcept
        {
            __m128 va = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
            __m128 vb = _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x);
            __m128 tvec = _mm_set1_ps(t);
            __m128 res = _mm_add_ps(va, _mm_mul_ps(_mm_sub_ps(vb, va), tvec));
            _mm_store_ss(&result.x, res);
            _mm_store_ss(&result.y, _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1)));
        }
#endif

        void MathDispatch::initialize() noexcept
        {
            if (initialized) return;
            
#if defined(GINA_SSE2_ENABLED)
            if (isSSE2Supported())
            {
                useSSE2();
            }
            else
            {
                useBasic();
            }
#else
            useBasic();
#endif
            initialized = true;
        }

        void MathDispatch::useSSE2() noexcept
        {
#if defined(GINA_SSE2_ENABLED)
            length2Impl = &SSE2MathImpl::length2;
            dot2Impl = &SSE2MathImpl::dot2;
            normalize2Impl = &SSE2MathImpl::normalize2;
            add2Impl = &SSE2MathImpl::add2;
            sub2Impl = &SSE2MathImpl::sub2;
            mul2Impl = &SSE2MathImpl::mul2;
            div2Impl = &SSE2MathImpl::div2;
            lerp2Impl = &SSE2MathImpl::lerp2;
#endif
        }

        void MathDispatch::useBasic() noexcept
        {
            length2Impl = &BasicMathImpl::length2;
            dot2Impl = &BasicMathImpl::dot2;
            normalize2Impl = &BasicMathImpl::normalize2;
            add2Impl = &BasicMathImpl::add2;
            sub2Impl = &BasicMathImpl::sub2;
            mul2Impl = &BasicMathImpl::mul2;
            div2Impl = &BasicMathImpl::div2;
            lerp2Impl = &BasicMathImpl::lerp2;
        }
    }

    const float2 float2::Zero = float2(0.0f, 0.0f);

    float float2::lengthSquared() const noexcept
    {
        return detail::MathDispatch::length2Impl(*this);
    }

    float float2::length() const noexcept
    {
        return std::sqrt(lengthSquared());
    }

    bool float2::isZero() const noexcept
    {
        return std::fabs(x) < EPSILON && std::fabs(y) < EPSILON;
    }

    float2 float2::normalized() const noexcept
    {
        float2 result = *this;
        result.normalize();
        return result;
    }

    void float2::normalize() noexcept
    {
        detail::MathDispatch::normalize2Impl(*this);
    }

    float2& float2::operator+=(const float2& other) noexcept
    {
        detail::MathDispatch::add2Impl(*this, *this, other);
        return *this;
    }

    float2& float2::operator-=(const float2& other) noexcept
    {
        detail::MathDispatch::sub2Impl(*this, *this, other);
        return *this;
    }

    float2& float2::operator*=(float scalar) noexcept
    {
        detail::MathDispatch::mul2Impl(*this, *this, scalar);
        return *this;
    }

    float2& float2::operator/=(float scalar) noexcept
    {
        detail::MathDispatch::div2Impl(*this, *this, scalar);
        return *this;
    }

    bool float2::operator==(const float2& other) const noexcept
    {
        return gina::isZero(x - other.x) && gina::isZero(y - other.y);
    }

    bool float2::operator!=(const float2& other) const noexcept
    {
        return !(*this == other);
    }

    float2 float2::operator-() const noexcept
    {
        return float2(-x, -y);
    }

    float dot(const float2& a, const float2& b) noexcept
    {
        return detail::MathDispatch::dot2Impl(a, b);
    }

    float2 reflect(const float2& vec, const float2& normal) noexcept
    {
        float dotProduct = dot(vec, normal);
        return vec - 2.0f * dotProduct * normal;
    }

    float2 project(const float2& vec, const float2& onto) noexcept
    {
        float dotProduct = dot(vec, onto);
        float ontoLengthSq = onto.lengthSquared();
        return onto * (dotProduct / ontoLengthSq);
    }

    float distance(const float2& a, const float2& b) noexcept
    {
        return (a - b).length();
    }

    float angle(const float2& a, const float2& b) noexcept
    {
        float dotProduct = dot(a, b);
        float lenA = a.length();
        float lenB = b.length();
        return std::acos(dotProduct / (lenA * lenB));
    }

    float2 lerp(const float2& a, const float2& b, float t) noexcept
    {
        float2 result;
        detail::MathDispatch::lerp2Impl(result, a, b, t);
        return result;
    }

    float2 operator+(const float2& lhs, const float2& rhs) noexcept
    {
        float2 result;
        detail::MathDispatch::add2Impl(result, lhs, rhs);
        return result;
    }

    float2 operator-(const float2& lhs, const float2& rhs) noexcept
    {
        float2 result;
        detail::MathDispatch::sub2Impl(result, lhs, rhs);
        return result;
    }

    float2 operator*(const float2& vec, float scalar) noexcept
    {
        float2 result;
        detail::MathDispatch::mul2Impl(result, vec, scalar);
        return result;
    }

    float2 operator*(float scalar, const float2& vec) noexcept
    {
        return vec * scalar;
    }

    float2 operator/(const float2& vec, float scalar) noexcept
    {
        float2 result;
        detail::MathDispatch::div2Impl(result, vec, scalar);
        return result;
    }
}