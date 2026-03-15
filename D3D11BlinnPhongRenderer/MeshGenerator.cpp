#include "pch.h"
#include "MeshGenerator.h"

std::pair<std::vector<Vertex>, std::vector<UINT>> MeshGenerator::GeneratePoint() {
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    vertices.resize(1);
    vertices[0].pos = glm::vec3(0.0f, 0.0f, 0.0f);

    indices.push_back(0);

    return { vertices, indices };
}

std::pair<std::vector<Vertex>, std::vector<UINT>> MeshGenerator::GenerateSquare() {
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    vertices.resize(4);

    vertices[0].pos = glm::vec3(-0.5f, 0.5f, 0.0f);
    vertices[1].pos = glm::vec3(0.5f, 0.5f, 0.0f);
    vertices[2].pos = glm::vec3(0.5f, -0.5f, 0.0f);
    vertices[3].pos = glm::vec3(-0.5f, -0.5f, 0.0f);

    vertices[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[3].normal = glm::vec3(0.0f, 0.0f, -1.0f);

    vertices[0].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[1].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[2].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[3].texcoord = glm::vec2(0.0f, 1.0f);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    return { vertices, indices };
}

std::pair<std::vector<Vertex>, std::vector<UINT>> MeshGenerator::GenerateCube() {
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    vertices.resize(24);

    // front
    vertices[0].pos = glm::vec3(-0.5f, 0.5f, -0.5f);
    vertices[1].pos = glm::vec3(0.5f, 0.5f, -0.5f);
    vertices[2].pos = glm::vec3(0.5f, -0.5f, -0.5f);
    vertices[3].pos = glm::vec3(-0.5f, -0.5f, -0.5f);

    vertices[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[3].normal = glm::vec3(0.0f, 0.0f, -1.0f);

    vertices[0].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[1].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[2].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[3].texcoord = glm::vec2(0.0f, 1.0f);

    // back
    vertices[4].pos = glm::vec3(0.5f, 0.5f, 0.5f);
    vertices[5].pos = glm::vec3(-0.5f, 0.5f, 0.5f);
    vertices[6].pos = glm::vec3(-0.5f, -0.5f, 0.5f);
    vertices[7].pos = glm::vec3(0.5f, -0.5f, 0.5f);

    vertices[4].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices[5].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices[6].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices[7].normal = glm::vec3(0.0f, 0.0f, 1.0f);

    vertices[4].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[5].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[6].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[7].texcoord = glm::vec2(0.0f, 1.0f);

    // top
    vertices[8].pos = glm::vec3(-0.5f, 0.5f, 0.5f);
    vertices[9].pos = glm::vec3(0.5f, 0.5f, 0.5f);
    vertices[10].pos = glm::vec3(0.5f, 0.5f, -0.5f);
    vertices[11].pos = glm::vec3(-0.5f, 0.5f, -0.5f);

    vertices[8].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[9].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[10].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[11].normal = glm::vec3(0.0f, 1.0f, 0.0f);

    vertices[8].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[9].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[10].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[11].texcoord = glm::vec2(0.0f, 1.0f);

    // bottom
    vertices[12].pos = glm::vec3(-0.5f, -0.5f, -0.5f);
    vertices[13].pos = glm::vec3(0.5f, -0.5f, -0.5f);
    vertices[14].pos = glm::vec3(0.5f, -0.5f, 0.5f);
    vertices[15].pos = glm::vec3(-0.5f, -0.5f, 0.5f);

    vertices[12].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vertices[13].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vertices[14].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vertices[15].normal = glm::vec3(0.0f, -1.0f, 0.0f);

    vertices[12].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[13].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[14].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[15].texcoord = glm::vec2(0.0f, 1.0f);

    // left
    vertices[16].pos = glm::vec3(-0.5f, 0.5f, 0.5f);
    vertices[17].pos = glm::vec3(-0.5f, 0.5f, -0.5f);
    vertices[18].pos = glm::vec3(-0.5f, -0.5f, -0.5f);
    vertices[19].pos = glm::vec3(-0.5f, -0.5f, 0.5f);

    vertices[16].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vertices[17].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vertices[18].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vertices[19].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    
    vertices[16].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[17].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[18].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[19].texcoord = glm::vec2(0.0f, 1.0f);
    
    // right
    vertices[20].pos = glm::vec3(0.5f, 0.5f, -0.5f);
    vertices[21].pos = glm::vec3(0.5f, 0.5f, 0.5f);
    vertices[22].pos = glm::vec3(0.5f, -0.5f, 0.5f);
    vertices[23].pos = glm::vec3(0.5f, -0.5f, -0.5f);
    
    vertices[20].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[21].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[22].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[23].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    
    vertices[20].texcoord = glm::vec2(0.0f, 0.0f);
    vertices[21].texcoord = glm::vec2(1.0f, 0.0f);
    vertices[22].texcoord = glm::vec2(1.0f, 1.0f);
    vertices[23].texcoord = glm::vec2(0.0f, 1.0f);

    for (int i = 0; i < 6; i++) {
        int offset = i * 4;
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);

        indices.push_back(offset + 0);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
    }

    return { vertices, indices };
}

// https://www.songho.ca/opengl/gl_sphere.html
std::pair<std::vector<Vertex>, std::vector<UINT>> MeshGenerator::GenerateSphere(const float radius, const int sectorCount, const int stackCount) {
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    const float sectorStep = 2 * glm::pi<float>() / sectorCount;
    const float stackStep = glm::pi<float>() / stackCount;

    for (int i = 0; i <= stackCount; i++) {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;
        float xz = radius * glm::cos(stackAngle);
        float y = radius * glm::sin(stackAngle);

        for (int j = 0; j <= sectorCount; j++) {
            float sectorAngle = j * sectorStep;

            Vertex vertex;

            vertex.pos.x = xz * glm::cos(sectorAngle);
            vertex.pos.y = y;
            vertex.pos.z = xz * glm::sin(sectorAngle);

            vertex.normal = glm::normalize(vertex.pos);

            vertex.texcoord.x = (float)j / sectorCount;
            vertex.texcoord.y = (float)i / stackCount;

            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stackCount; i++) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; j++, k1++, k2++) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k2);
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
            }
        }
    }

    return { vertices, indices };
}