#ifndef GEOMETRICTOOLS_H_
#define GEOMETRICTOOLS_H_

#include <vector>
#include <array>

namespace GeometricTools
{
    // Unit triangle (3 vertices × 2 components = 6 floats)
    constexpr std::array<float, 3*2> UnitTriangle2D = { 
        -0.5f, -0.5f,  // Bottom left
         0.5f, -0.5f,  // Bottom right
         0.0f,  0.5f   // Top center
    };

    // Unit square (4 vertices × 2 components = 8 floats)
    constexpr std::array<float, 4*2> UnitSquare2D = {
        -0.5f, -0.5f,  // Bottom left
         0.5f, -0.5f,  // Bottom right
         0.5f,  0.5f,  // Top right
        -0.5f,  0.5f   // Top left
    };

    // Unit cube (8 vertices x 3 components = 24 floats)
    constexpr std::array<float, 8*3> UnitCubeGeometry3D = {

        // Front facing square
        -0.5f, -0.5f, 0.5f,     // Bottom left front
         0.5f, -0.5f, 0.5f,     // Bottom right front
         0.5f,  0.5f, 0.5f,     // Top right front
        -0.5f,  0.5f, 0.5f,      // Top left front

        // Back facing square
        -0.5f, -0.5f, -0.5f,    // Bottom left back
         0.5f, -0.5f, -0.5f,    // Bottom right back
         0.5f,  0.5f, -0.5f,    // Top right back
        -0.5f,  0.5f, -0.5f     // Top left back
    };

    // Unit cube with normals (24 vertices × 6 floats = 144 floats)
    // Each vertex: [x, y, z, nx, ny, nz]
    // Vertices are duplicated so each face has its own set with correct normals
    constexpr std::array<float, 3 * 24 * 2> UnitCube3D24WNormals = {
        // Front face (normal: 0, 0, 1) - indices 0-3
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 0: Bottom left front
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 1: Bottom right front
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 2: Top right front
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 3: Top left front

        // Back face (normal: 0, 0, -1) - indices 4-7
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 4: Bottom right back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 5: Bottom left back
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 6: Top left back
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 7: Top right back

        // Right face (normal: 1, 0, 0) - indices 8-11
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 8: Bottom front right
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 9: Bottom back right
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 10: Top back right
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 11: Top front right

        // Left face (normal: -1, 0, 0) - indices 12-15
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 12: Bottom back left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 13: Bottom front left
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 14: Top front left
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 15: Top back left

        // Top face (normal: 0, 1, 0) - indices 16-19
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // 16: Front left top
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // 17: Front right top
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 18: Back right top
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 19: Back left top

        // Bottom face (normal: 0, -1, 0) - indices 20-23
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 20: Back left bottom
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 21: Back right bottom
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 22: Front right bottom
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f   // 23: Front left bottom
    };

    // Unit cube topology for 24-vertex cube with normals - 6 faces × 2 triangles × 3 indices = 36 indices
    constexpr std::array<unsigned int, 6 * 3 * 2> UnitCube3D24WNormalsTopologyTriangles = {
        // Front face (indices 0-3)
        0, 1, 2,    2, 3, 0,
        // Back face (indices 4-7)
        4, 5, 6,    6, 7, 4,
        // Right face (indices 8-11)
        8, 9, 10,   10, 11, 8,
        // Left face (indices 12-15)
        12, 13, 14, 14, 15, 12,
        // Top face (indices 16-19)
        16, 17, 18, 18, 19, 16,
        // Bottom face (indices 20-23)
        20, 21, 22, 22, 23, 20
    };

    // Unit cube topology - 6 faces x 2 triangles x 3 indices = 36 indices
    constexpr std::array<unsigned int, 36> UnitCubeTopologyTriangles = {

        // Front face
        0, 1, 2,    2, 3, 0,
        // Right face
        1, 5, 6,    6, 2, 1,
        // Back face
        5, 4, 7,    7, 6, 5,
        // Left face
        4, 0, 3,    3, 7, 4,
        // Top face
        3, 2, 6,    6, 7, 3,
        // Bottom face
        4, 5, 1,    1, 0, 4
    };

    // Configurable template
    template<int DivisionsX, int DivisionsY>
    constexpr auto UnitGridGeometry2D() {

        // Number of vertices in each direction (x,y) will always be divisions + 1 (2x2 grid have 3 points)
        constexpr int numVertices = (DivisionsX + 1) * (DivisionsY + 1);
        // 2 floats for each coordinate
        constexpr int numFloats = numVertices * 2;

        // Array to hold vertices
        std::array<float, numFloats> vertices {};
        // Step distance for each split on the grid in each direction
        constexpr float stepX = 1.0f / DivisionsX;
        constexpr float stepY = 1.0f / DivisionsY;

        int index = 0;
        // Insert all vertices in the array
        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                vertices[index++] = -0.5f + x * stepX; 
                vertices[index++] = -0.5f + y * stepY; 
            }
        }

        return vertices;
    }

    template<int DivisionsX, int DivisionsY>
    constexpr auto UnitGridTopologyTriangles() {

        // Each cell becomes 2 tringles, each triangle has 3 indices
        constexpr int numCells = DivisionsX * DivisionsY;
        constexpr int numIndices = numCells * 2 * 3;

        // Array to hold the indices
        std::array<unsigned int, numIndices> indices{};

        int index = 0;

        // Loop trough each cell in the grid
        for (int y = 0; y < DivisionsY; ++y) {
            for (int x = 0; x < DivisionsX; ++x) {

                // Calculate the 4 corner vertex indices of the current cell
                unsigned int bottomLeft  = y * (DivisionsX + 1) + x;
                unsigned int bottomRight = y * (DivisionsX + 1) + (x + 1);
                unsigned int topLeft     = (y + 1) * (DivisionsX + 1) + x;
                unsigned int topRight    = (y + 1) * (DivisionsX + 1) + (x + 1); 

                // First triangle (bottom-left triangle of the cell)
                indices[index++] = bottomLeft;
                indices[index++] = bottomRight;
                indices[index++] = topLeft;

                // Second triangle (top-right triangle of the cell)
                indices[index++] = bottomRight;
                indices[index++] = topRight;
                indices[index++] = topLeft;
            }
        }

        return indices;
    }

    template<unsigned int DivisionsX, unsigned int DivisionsY>
    constexpr auto UnitGridGeometry2DWTCoords() {

        // Number of vertices in each direction (x,y) will always be divisions + 1 (2x2 grid have 3 points)
        constexpr int numVertices = (DivisionsX + 1) * (DivisionsY + 1);
        // 4 floats for each coordinate (2 for pos and 2 for texture)
        constexpr int numFloats = numVertices * 4;

        // Array to hold vertices
        std::array<float, numFloats> vertices {};

        // Step distance for each split on the grid in each direction
        constexpr float stepX = 1.0f / DivisionsX;
        constexpr float stepY = 1.0f / DivisionsY;

        int index = 0;
        // Insert all vertices with interleaved position and texture coord in the array
        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                // Position coordinates (range -0.5 to 0.5)
                vertices[index++] = -0.5f + x * stepX; 
                vertices[index++] = -0.5f + y * stepY; 

                // Texture coordinates (range 0.0 to 1.0)
                vertices[index++] = static_cast<float>(x) / DivisionsX;
                vertices[index++] = static_cast<float>(y) / DivisionsY;
            }
        }
        return vertices;
    }

    inline std::vector<float> UnitGridGeometry2DWTCoords(unsigned int DivisionsX, unsigned int DivisionsY) {

        int numVertices = (DivisionsX + 1) * (DivisionsY + 1);
        int numFloats = numVertices * 4;

        std::vector<float> vertices(numFloats);

        float stepX = 1.0f / DivisionsX;
        float stepY = 1.0f / DivisionsY;

        int index = 0;

        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                // Position coordinates (range -0.5 to 0.5)
                vertices[index++] = -0.5f + x * stepX; 
                vertices[index++] = -0.5f + y * stepY; 

                // Texture coordinates (range 0.0 to 1.0)
                vertices[index++] = static_cast<float>(x) / DivisionsX;
                vertices[index++] = static_cast<float>(y) / DivisionsY;
            }
        }
        return vertices;
    }
}

#endif // GEOMETRICTOOLS_H_