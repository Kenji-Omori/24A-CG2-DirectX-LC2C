#include "Math.h"
#include <math.h>
#include <cassert>
Matrix4x4 Matrix4x4::MakeMatrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    Matrix4x4 m;
    m.m[0][0] = m00;
    m.m[0][1] = m01;
    m.m[0][2] = m02;
    m.m[0][3] = m03;
    m.m[1][0] = m10;
    m.m[1][1] = m11;
    m.m[1][2] = m12;
    m.m[1][3] = m13;
    m.m[2][0] = m20;
    m.m[2][1] = m21;
    m.m[2][2] = m22;
    m.m[2][3] = m23;
    m.m[3][0] = m30;
    m.m[3][1] = m31;
    m.m[3][2] = m32;
    m.m[3][3] = m33;
    return m;
}
Matrix4x4 Matrix4x4::MakeIdentity4x4()
{
    Matrix4x4 identity;
    for (int i = 0; i < 4; i++)
    {
        identity.m[i][i] = 1;
    }
    return identity;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate)

{
    Matrix4x4 affine = {
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    };

    Matrix4x4 scaleMat = {
      scale.x,0,0,0,
      0,scale.y,0,0,
      0,0,scale.z,0,
      0,0,0,1
    };

    Matrix4x4 rotXMat = {
      1,0,0,0,
      0,cosf(rot.x),-sinf(rot.x),0,
      0,sinf(rot.x), cosf(rot.x),0,
      0,0,0,1
    };

    Matrix4x4 rotYMat = {
      cosf(rot.y),0,sinf(rot.y),0,
      0,1,0,0,
      -sinf(rot.y),0,cosf(rot.y),0,
      0,0,0,1
    };

    Matrix4x4 rotZMat = {
      cosf(rot.z), -sinf(rot.z), 0, 0,
      sinf(rot.z),  cosf(rot.z) , 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };

    Matrix4x4 translateMat = {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      translate.x, translate.y, translate.z, 1
    };

    return scaleMat * rotZMat * rotXMat * rotYMat * translateMat;
}

Matrix4x4 Matrix4x4::MakeOrthographicMatrix(const Rect& rect, float n, float f)
{
    return Matrix4x4::MakeMatrix4x4(
        2 / rect.width, 0, 0, 0,
        0, 2 / rect.height, 0, 0,
        0, 0, 1 / (f - n), 0,

        (rect.left + rect.GetRight()) / (rect.left - rect.GetRight()),
        (rect.top + rect.GetBottom()) / (rect.GetBottom() - rect.top),
        n / (n - f),
        1
    );
}

Matrix4x4 Matrix4x4::MakePerspectiveMatrix(float y, float a, float n, float f)
{
    return Matrix4x4::MakeMatrix4x4(
        1 / a * 1 / (tanf(y / 2)), 0, 0, 0,
        0, 1 / tanf(y / 2), 0, 0,
        0, 0, f / (f - n), 1,
        0, 0, -n * f / (f - n), 0
    );
}

Matrix4x4 Matrix4x4::MakeViewportMatrix(const Rect& rect, float minDepth, float maxDepth)
{
    return Matrix4x4::MakeMatrix4x4(
        rect.width / 2, 0, 0, 0,
        0, 1 / rect.height, 0, 0,
        0, 0, maxDepth - minDepth, 0,
        rect.left + rect.width / 2, rect.top + rect.height / 2, minDepth, 1
    );
}

Matrix4x4 Matrix4x4::Inverse()
{
    float absM =
        +m[0][0] * m[1][1] * m[2][2] * m[3][3] + m[0][0] * m[1][2] * m[2][3] * m[3][1] + m[0][0] * m[1][3] * m[2][1] * m[3][2]
        - m[0][0] * m[1][3] * m[2][2] * m[3][1] - m[0][0] * m[1][2] * m[2][1] * m[3][3] - m[0][0] * m[1][1] * m[2][3] * m[3][2]
        - m[0][1] * m[1][0] * m[2][2] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1] - m[0][3] * m[1][0] * m[2][1] * m[3][2]
        + m[0][3] * m[1][0] * m[2][2] * m[3][1] + m[0][2] * m[1][0] * m[2][1] * m[3][3] + m[0][1] * m[1][0] * m[2][3] * m[3][2]
        + m[0][1] * m[1][2] * m[2][0] * m[3][3] + m[0][2] * m[1][3] * m[2][0] * m[3][1] + m[0][3] * m[1][1] * m[2][0] * m[3][2]
        - m[0][3] * m[1][2] * m[2][0] * m[3][1] - m[0][2] * m[1][1] * m[2][0] * m[3][3] - m[0][1] * m[1][3] * m[2][0] * m[3][2]
        - m[0][1] * m[1][2] * m[2][3] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] - m[0][3] * m[1][1] * m[2][2] * m[3][0]
        + m[0][3] * m[1][2] * m[2][1] * m[3][0] + m[0][2] * m[1][1] * m[2][3] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0];
    assert(abs != 0);
    Matrix4x4 result = {};
    result.m[0][0] = +(m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][3] * m[2][2] * m[3][1] - m[1][2] * m[2][1] * m[3][3] - m[1][1] * m[2][3] * m[3][2]) / absM;
    result.m[0][1] = -(m[0][1] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][1] + m[0][3] * m[2][1] * m[3][2] - m[0][3] * m[2][2] * m[3][1] - m[0][2] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][2]) / absM;
    result.m[0][2] = +(m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][3] * m[1][2] * m[3][1] - m[0][2] * m[1][1] * m[3][3] - m[0][1] * m[1][3] * m[3][2]) / absM;
    result.m[0][3] = -(m[0][1] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][1] + m[0][3] * m[1][1] * m[2][2] - m[0][3] * m[1][2] * m[2][1] - m[0][2] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][2]) / absM;

    result.m[1][0] = -(m[1][0] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][2] - m[1][3] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][3] - m[1][0] * m[2][3] * m[3][2]) / absM;
    result.m[1][1] = +(m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][3] * m[2][2] * m[3][0] - m[0][2] * m[2][0] * m[3][3] - m[0][0] * m[2][3] * m[3][2]) / absM;
    result.m[1][2] = -(m[0][0] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][2] - m[0][3] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][3] - m[0][0] * m[1][3] * m[3][2]) / absM;
    result.m[1][3] = +(m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][3] * m[1][2] * m[2][0] - m[0][2] * m[1][0] * m[2][3] - m[0][0] * m[1][3] * m[2][2]) / absM;

    result.m[2][0] = +(m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][3] * m[2][1] * m[3][0] - m[1][1] * m[2][0] * m[3][3] - m[1][0] * m[2][3] * m[3][1]) / absM;
    result.m[2][1] = -(m[0][0] * m[2][1] * m[3][3] + m[0][1] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][1] - m[0][3] * m[2][1] * m[3][0] - m[0][1] * m[2][0] * m[3][3] - m[0][0] * m[2][3] * m[3][1]) / absM;
    result.m[2][2] = +(m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][3] * m[1][1] * m[3][0] - m[0][1] * m[1][0] * m[3][3] - m[0][0] * m[1][3] * m[3][1]) / absM;
    result.m[2][3] = -(m[0][0] * m[1][1] * m[2][3] + m[0][1] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][1] - m[0][3] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][3] - m[0][0] * m[1][3] * m[2][1]) / absM;

    result.m[3][0] = -(m[1][0] * m[2][1] * m[3][2] + m[1][1] * m[2][2] * m[3][0] + m[1][2] * m[2][0] * m[3][1] - m[1][2] * m[2][1] * m[3][0] - m[1][1] * m[2][0] * m[3][2] - m[1][0] * m[2][2] * m[3][1]) / absM;
    result.m[3][1] = +(m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][2] * m[2][1] * m[3][0] - m[0][1] * m[2][0] * m[3][2] - m[0][0] * m[2][2] * m[3][1]) / absM;
    result.m[3][2] = -(m[0][0] * m[1][1] * m[3][2] + m[0][1] * m[1][2] * m[3][0] + m[0][2] * m[1][0] * m[3][1] - m[0][2] * m[1][1] * m[3][0] - m[0][1] * m[1][0] * m[3][2] - m[0][0] * m[1][2] * m[3][1]) / absM;
    result.m[3][3] = +(m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1]) / absM;

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other)
{
    Matrix4x4 result = {};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

float Rect::GetRight() const
{
    return left + width;
}

float Rect::GetBottom() const
{
    return top + height;
}

Vector2 Rect::GetCenter()
{
    return Vector2(left + width / 2, top + height / 2);
}

void Rect::SetCenter(const Vector2& center, float width, float height)
{
    left = center.x - width / 2;
    top = center.y - width / 2;
    this->width = width;
    this->height = height;
}

