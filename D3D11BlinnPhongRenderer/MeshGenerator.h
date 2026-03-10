#pragma once
#include "Mesh.h"

class MeshGenerator {
public:
    static void GenerateCube(Mesh &mesh);
    static void GenerateSphere(Mesh &mesh, const float radius = 0.5f, const int sectorCount = 64, const int stackCount = 64);
};