#pragma once

#include "core/types.h"

class Transform
{
public:
    Transform()
    {
    }

    Transform(const flt mat[4][4])
    {
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                      mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                      mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                      mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = Inverse(m);
    }

    Transform(const Matrix4x4& m) : m(m)
                                  , mInv(Inverse(m))
    {
    }

    Transform(const Matrix4x4& m, const Matrix4x4& mInv)
        : m(m)
        , mInv(mInv)
    {
    }

    void Print(FILE* f) const;

    friend Transform Inverse(const Transform& t)
    {
        return Transform(t.mInv, t.m);
    }

    friend Transform Transpose(const Transform& t)
    {
        return Transform(Transpose(t.m), Transpose(t.mInv));
    }

    bool operator==(const Transform& t) const
    {
        return t.m == m && t.mInv == mInv;
    }

    bool operator!=(const Transform& t) const
    {
        return t.m != m || t.mInv != mInv;
    }

    bool operator<(const Transform& t2) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
            {
                if (m.m[i][j] < t2.m.m[i][j]) return true;
                if (m.m[i][j] > t2.m.m[i][j]) return false;
            }
        return false;
    }

    bool IsIdentity() const
    {
        return (m.m[0][0] == 1.f && m.m[0][1] == 0.f &&
            m.m[0][2] == 0.f && m.m[0][3] == 0.f &&
            m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
            m.m[1][2] == 0.f && m.m[1][3] == 0.f &&
            m.m[2][0] == 0.f && m.m[2][1] == 0.f &&
            m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
            m.m[3][0] == 0.f && m.m[3][1] == 0.f &&
            m.m[3][2] == 0.f && m.m[3][3] == 1.f);
    }

    const Matrix4x4& GetMatrix() const { return m; }
    const Matrix4x4& GetInverseMatrix() const { return mInv; }

    bool HasScale() const
    {
        Float la2 = (*this)(Vector3f(1, 0, 0)).LengthSquared();
        Float lb2 = (*this)(Vector3f(0, 1, 0)).LengthSquared();
        Float lc2 = (*this)(Vector3f(0, 0, 1)).LengthSquared();
        #define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
        return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
        #undef NOT_ONE
    }

    template <typename T> Point3<T> operator()(const Point3<T>& p) const;
    template <typename T> Vector3<T> operator()(const Vector3<T>& v) const;
    template <typename T> Normal3<T> operator()(const Normal3<T>&) const;
    template <typename T> void operator()(const Normal3<T>&, Normal3<T>* nt) const;
    inline Ray operator()(const Ray& r) const;
    inline RayDifferential operator()(const RayDifferential& r) const;
    Bounds3f operator()(const Bounds3f& b) const;
    Transform operator*(const Transform& t2) const;
    bool SwapsHandedness() const;
    SurfaceInteraction operator()(const SurfaceInteraction& si) const;
    template <typename T> Point3<T> operator()(const Point3<T>& pt, Vector3<T>* absError) const;
    template <typename T> Point3<T>
    operator()(const Point3<T>& p, const Vector3<T>& pError,
               Vector3<T>* pTransError) const;
    template <typename T> Vector3<T>
    operator()(const Vector3<T>& v, Vector3<T>* vTransError) const;
    template <typename T> Vector3<T>
    operator()(const Vector3<T>& v, const Vector3<T>& vError,
               Vector3<T>* vTransError) const;
    inline Ray operator()(const Ray& r, Vector3f* oError,
                          Vector3f* dError) const;
    inline Ray operator()(const Ray& r, const Vector3f& oErrorIn,
                          const Vector3f& dErrorIn, Vector3f* oErrorOut,
                          Vector3f* dErrorOut) const;

private:
    Matrix4x4 m, mInv;
    friend class AnimatedTransform;
    friend struct Quaternion;
};
