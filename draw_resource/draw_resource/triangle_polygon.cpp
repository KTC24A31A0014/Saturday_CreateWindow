#include "triangle_polygon.h"
#include <cassert>
#include <DirectXMath.h>

namespace
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}

triangle_polygon::~triangle_polygon()
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_-> nullptr;
	}
}