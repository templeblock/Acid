#include "MeshSimple.hpp"

#include <algorithm>
#include "Maths/Maths.hpp"

namespace Flounder
{
	MeshSimple::MeshSimple(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale) :
		Model(),
		m_sideLength(sideLength),
		m_squareSize(squareSize),
		m_vertexCount(vertexCount),
		m_textureScale(textureScale)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		// Creates and stores vertices.
		for (int col = 0; col < vertexCount; col++)
		{
			for (int row = 0; row < vertexCount; row++)
			{
				// Creates and stores vertices.
				const Vector3 position = GetPosition(
						(row * squareSize) - (sideLength / 2.0f),
						(col * squareSize) - (sideLength / 2.0f)
				);
				const Vector2 uv = Vector2(
						textureScale * static_cast<float>(col) / static_cast<float>(vertexCount),
						textureScale * static_cast<float>(row) / static_cast<float>(vertexCount)
				);
				const Vector3 normal = GetNormal(position);
				const Vector3 tangent = GetColour(position, normal);
				vertices.push_back(Vertex(position, uv, normal, tangent));
			}
		}

		// Creates and stores indices.
		for (int col = 0; col < vertexCount - 1; col++)
		{
			for (int row = 0; row < vertexCount - 1; row++)
			{
				const uint32_t topLeft = (row * vertexCount) + col;
				const uint32_t topRight = topLeft + 1;
				const uint32_t bottomLeft = ((row + 1) * vertexCount) + col;
				const uint32_t bottomRight = bottomLeft + 1;

				//	indices.push_back(topLeft);
				//	indices.push_back(bottomLeft);
				//	indices.push_back(bottomRight);
				//	indices.push_back(topLeft);
				//	indices.push_back(bottomRight);
				//	indices.push_back(topRight);
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		Model::Set(vertices, indices);
	}

	Vector3 MeshSimple::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, 0.0f, z);
	}

	Vector3 MeshSimple::GetNormal(const Vector3 &position)
	{
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	Vector3 MeshSimple::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		return Vector3(1.0f, 1.0f, 1.0f);
	}
}