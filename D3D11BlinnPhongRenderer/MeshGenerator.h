#pragma once
#include "Vertex.h"

class MeshGenerator {
public:
    static std::pair<std::vector<Vertex>, std::vector<UINT>> GeneratePoint();
    static std::pair<std::vector<Vertex>, std::vector<UINT>> GenerateSquare();
    static std::pair<std::vector<Vertex>, std::vector<UINT>> GenerateCube();
    static std::pair<std::vector<Vertex>, std::vector<UINT>> GenerateSphere(const float radius = 0.5f, const int sectorCount = 64, const int stackCount = 64);
};