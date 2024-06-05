#include "Object3d.hlsli"

struct transformationmatrix
{
  float32_t4x4 wvp;
};
ConstantBuffer<transformationmatrix> gTransformationMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
  VertexShaderOutput output;
  output.position = mul(input.position, gTransformationMatrix.wvp);
  output.texcoord = input.texcoord;
  return output;
}

