#include "pch.h"
#include "MeshGenerator.h"

void MeshGenerator::GenerateCube(Mesh &mesh) {
    mesh.vertices.clear();
    mesh.indices.clear();

    mesh.vertices.resize(24);

    // front
    mesh.vertices[0].pos = glm::vec3(-0.5f, 0.5f, -0.5f);
    mesh.vertices[1].pos = glm::vec3(0.5f, 0.5f, -0.5f);
    mesh.vertices[2].pos = glm::vec3(0.5f, -0.5f, -0.5f);
    mesh.vertices[3].pos = glm::vec3(-0.5f, -0.5f, -0.5f);

    mesh.vertices[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    mesh.vertices[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    mesh.vertices[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    mesh.vertices[3].normal = glm::vec3(0.0f, 0.0f, -1.0f);

    mesh.vertices[0].texcoord = glm::vec2(0.0f, 0.0f);
    mesh.vertices[1].texcoord = glm::vec2(1.0f, 0.0f);
    mesh.vertices[2].texcoord = glm::vec2(1.0f, 1.0f);
    mesh.vertices[3].texcoord = glm::vec2(0.0f, 1.0f);

    // back
    mesh.vertices[4].pos = glm::vec3(0.5f, 0.5f, 0.5f);
    mesh.vertices[5].pos = glm::vec3(-0.5f, 0.5f, 0.5f);
    mesh.vertices[6].pos = glm::vec3(-0.5f, -0.5f, 0.5f);
    mesh.vertices[7].pos = glm::vec3(0.5f, -0.5f, 0.5f);

    mesh.vertices[4].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    mesh.vertices[5].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    mesh.vertices[6].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    mesh.vertices[7].normal = glm::vec3(0.0f, 0.0f, 1.0f);

    mesh.vertices[4].texcoord = glm::vec2(0.0f, 0.0f);
    mesh.vertices[5].texcoord = glm::vec2(1.0f, 0.0f);
    mesh.vertices[6].texcoord = glm::vec2(1.0f, 1.0f);
    mesh.vertices[7].texcoord = glm::vec2(0.0f, 1.0f);

    // top
    mesh.vertices[8].pos = glm::vec3(-0.5f, 0.5f, 0.5f);
    mesh.vertices[9].pos = glm::vec3(0.5f, 0.5f, 0.5f);
    mesh.vertices[10].pos = glm::vec3(0.5f, 0.5f, -0.5f);
    mesh.vertices[11].pos = glm::vec3(-0.5f, 0.5f, -0.5f);

    mesh.vertices[8].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    mesh.vertices[9].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    mesh.vertices[10].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    mesh.vertices[11].normal = glm::vec3(0.0f, 1.0f, 0.0f);

    mesh.vertices[8].texcoord = glm::vec2(0.0f, 0.0f);
    mesh.vertices[9].texcoord = glm::vec2(1.0f, 0.0f);
    mesh.vertices[10].texcoord = glm::vec2(1.0f, 1.0f);
    mesh.vertices[11].texcoord = glm::vec2(0.0f, 1.0f);

    // bottom
    mesh.vertices[12].pos = glm::vec3(-0.5f, -0.5f, -0.5f);
    mesh.vertices[13].pos = glm::vec3(0.5f, -0.5f, -0.5f);
    mesh.vertices[14].pos = glm::vec3(0.5f, -0.5f, 0.5f);
    mesh.vertices[15].pos = glm::vec3(-0.5f, -0.5f, 0.5f);

    mesh.vertices[12].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    mesh.vertices[13].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    mesh.vertices[14].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    mesh.vertices[15].normal = glm::vec3(0.0f, -1.0f, 0.0f);

    mesh.vertices[12].texcoord = glm::vec2(0.0f, 0.0f);
    mesh.vertices[13].texcoord = glm::vec2(1.0f, 0.0f);
    mesh.vertices[14].texcoord = glm::vec2(1.0f, 1.0f);
    mesh.vertices[15].texcoord = glm::vec2(0.0f, 1.0f);

    //left
    mesh.vertices[16].pos = glm::vec3(-0.5f, 0.5f, 0.5f);
    mesh.vertices[17].pos = glm::vec3(-0.5f, 0.5f, -0.5f);
    mesh.vertices[18].pos = glm::vec3(-0.5f, -0.5f, -0.5f);
    mesh.vertices[19].pos = glm::vec3(-0.5f, -0.5f, 0.5f);

    mesh.vertices[16].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    mesh.vertices[17].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    mesh.vertices[18].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    mesh.vertices[19].normal = glm::vec3(-1.0f, 0.0f, 0.0f);

    mesh.vertices[16].texcoord = glm::vec2(0.0f, 0.0f);
    mesh.vertices[17].texcoord = glm::vec2(1.0f, 0.0f);
    mesh.vertices[18].texcoord = glm::vec2(1.0f, 1.0f);
    mesh.vertices[19].texcoord = glm::vec2(0.0f, 1.0f);

    // right
    mesh.vertices[20].pos = glm::vec3(0.5f, 0.5f, -0.5f);
    mesh.vertices[21].pos = glm::vec3(0.5f, 0.5f, 0.5f);
    mesh.vertices[22].pos = glm::vec3(0.5f, -0.5f, 0.5f);
    mesh.vertices[23].pos = glm::vec3(0.5f, -0.5f, -0.5f);

    mesh.vertices[20].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    mesh.vertices[21].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    mesh.vertices[22].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    mesh.vertices[23].normal = glm::vec3(1.0f, 0.0f, 0.0f);

    mesh.vertices[20].texcoord = glm::vec2(0.0f, 0.0f);
    mesh.vertices[21].texcoord = glm::vec2(1.0f, 0.0f);
    mesh.vertices[22].texcoord = glm::vec2(1.0f, 1.0f);
    mesh.vertices[23].texcoord = glm::vec2(0.0f, 1.0f);

    for (int i = 0; i < 6; i++) {
        int offset = i * 4;
        mesh.indices.push_back(offset + 0);
        mesh.indices.push_back(offset + 1);
        mesh.indices.push_back(offset + 2);

        mesh.indices.push_back(offset + 0);
        mesh.indices.push_back(offset + 2);
        mesh.indices.push_back(offset + 3);
    }
}

// https://www.songho.ca/opengl/gl_sphere.html
void MeshGenerator::GenerateSphere(Mesh &mesh, const float radius, const int sectorCount, const int stackCount) {
    mesh.vertices.clear();
    mesh.indices.clear();

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

            mesh.vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stackCount; i++) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; j++, k1++, k2++) {
            if (i != 0) {
                mesh.indices.push_back(k1);
                mesh.indices.push_back(k1 + 1);
                mesh.indices.push_back(k2);
            }

            if (i != (stackCount - 1)) {
                mesh.indices.push_back(k2);
                mesh.indices.push_back(k1 + 1);
                mesh.indices.push_back(k2 + 1);
            }
        }
    }
}