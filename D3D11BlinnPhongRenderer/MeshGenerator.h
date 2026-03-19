#pragma once
#include "Vertex.h"

class MeshGenerator {
public:
    static std::tuple<std::vector<Vertex>, std::vector<UINT>, D3D11_PRIMITIVE_TOPOLOGY> GeneratePoint();
    static std::tuple<std::vector<Vertex>, std::vector<UINT>, D3D11_PRIMITIVE_TOPOLOGY> GenerateSquare();
    static std::tuple<std::vector<Vertex>, std::vector<UINT>, D3D11_PRIMITIVE_TOPOLOGY> GenerateCube();
    static std::tuple<std::vector<Vertex>, std::vector<UINT>, D3D11_PRIMITIVE_TOPOLOGY> GenerateSphere(const float radius = 1.0f, const int sectorCount = 64, const int stackCount = 64);
    static std::tuple<std::vector<Vertex>, std::vector<UINT>, D3D11_PRIMITIVE_TOPOLOGY> GenerateCubeSphere(const float radius = 1.0f, const int subdivision = 1);
};