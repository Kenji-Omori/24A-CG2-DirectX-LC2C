#pragma once
#pragma once

struct Vector2
{
    float x;
    float y;
};

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Vector4
{
    float x;
    float y;
    float z;
    float w;
};



struct Rect
{
    float left;
    float top;
    float width;
    float height;
    float GetRight() const;
    float GetBottom() const;
    Vector2 GetCenter();
    void SetCenter(const Vector2& center, float width, float height);
};


struct Matrix4x4
{
    float m[4][4]{
      {1,0,0,0},
      {0,1,0,0},
      {0,0,1,0},
      {0,0,0,1}
    };
    static Matrix4x4 MakeMatrix4x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    );


    static Matrix4x4 MakeIdentity4x4();


    static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
    static Matrix4x4 MakeOrthographicMatrix(const Rect& rect, float nearClip, float farClip);
    static Matrix4x4 MakePerspectiveMatrix(float fieldOfViewY, float aspectRatio, float nearClip, float farClip);
    static Matrix4x4 MakeViewportMatrix(const Rect& rect, float minDepth = 0, float maxDepth = 1);
    Matrix4x4 Inverse();
    Matrix4x4 operator*(const Matrix4x4& other);


};
