#pragma once

#include "core/types.h"

struct Matrix4x4
{
    constexpr Matrix4x4() = default;
    constexpr Matrix4x4(flt mat[4][4]);
    constexpr Matrix4x4(flt t00, flt t01, flt t02, flt t03,
                        flt t10, flt t11, flt t12, flt t13,
                        flt t20, flt t21, flt t22, flt t23,
                        flt t30, flt t31, flt t32, flt t33);

    constexpr bool operator==(const Matrix4x4 &m2) const;
    constexpr bool operator!=(const Matrix4x4 &m2) const;

    static constexpr Matrix4x4 mul(const Matrix4x4 &m1, const Matrix4x4 &m2);
    friend constexpr Matrix4x4 transpose(const Matrix4x4 &);
    friend constexpr Matrix4x4 inverse(const Matrix4x4 &);
    inline void print(FILE *f) const;

    flt m[4][4]
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
};

constexpr bool Matrix4x4::operator==(const Matrix4x4 &m2) const
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (m[i][j] != m2.m[i][j])
                return false;
    return true;
}
constexpr bool Matrix4x4::operator!=(const Matrix4x4 &m2) const
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (m[i][j] != m2.m[i][j])
                return true;
    return false;
}

constexpr Matrix4x4 transpose(const Matrix4x4& m)
{
   return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
                    m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
                    m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
                    m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

constexpr Matrix4x4 Matrix4x4::mul(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 r;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            r.m[i][j] = m1.m[i][0] * m2.m[0][j] +
                        m1.m[i][1] * m2.m[1][j] +
                        m1.m[i][2] * m2.m[2][j] +
                        m1.m[i][3] * m2.m[3][j];
    return r;
}

inline void Matrix4x4::print(FILE* f) const
{
    fprintf(f, "[ ");
    for (int i = 0; i < 4; ++i)
    {
        fprintf(f, "  [ ");
        for (int j = 0; j < 4; ++j)
        {
            fprintf(f, "%f", m[i][j]);
            if (j != 3) fprintf(f, ", ");
        }
        fprintf(f, " ]\n");
    }
    fprintf(f, " ] ");
}
