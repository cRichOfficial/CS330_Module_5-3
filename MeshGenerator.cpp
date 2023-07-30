#include "MeshGenerator.hpp"

namespace std {
    const Color Color::RED = Color(1.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
    const Color Color::BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);
    const Color Color::WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
    const Color Color::BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);

	Mesh generateCylinder(const float radius, const float height, const int segments, const bool isTextured, const Color color) {
        // Declare the cylinder mesh we will return
        Mesh cylinder;

        // Define how many radians are to be used for x and z calculations per segment
        const float RADS_PER_SEG = (2 * M_PI) / segments * 1.0f;

        // Create and add the center vertex for the bottom base of the cylinder
        Vertex  center;
        center.x = 0.0f;
        cylinder.vertices.push_back(center.x);
        center.y = 0.0f;
        cylinder.vertices.push_back(center.y);
        center.z = 0.0f;
        cylinder.vertices.push_back(center.z);
        // vertex color
        center.r = color.r;
        cylinder.vertices.push_back(center.r);
        center.g = color.g;
        cylinder.vertices.push_back(center.g);
        center.b = color.b;
        cylinder.vertices.push_back(center.b);
        center.a = color.a;
        cylinder.vertices.push_back(center.a);
        // add texture coordinates if true
        if (isTextured) {
            cylinder.vertices.push_back(0.5f);
            cylinder.vertices.push_back(0.5f);

        }

        // Iteratively create the vertex for the ends of each segment and add to cylinder vertices
        for (int i = 0; i < segments; i++) {
            float currAngle = RADS_PER_SEG * i;
            Vertex vert;
            vert.x = cos(currAngle) * radius;
            cylinder.vertices.push_back(vert.x);
            // vert.y = sin(currAngle) * radius;
            vert.y = 0.0f;
            cylinder.vertices.push_back(vert.y);
            // vert.z = 0.0f;
            vert.z = sin(currAngle) * radius;
            cylinder.vertices.push_back(vert.z);
            // vertex color
            vert.r = color.r;
            cylinder.vertices.push_back(vert.r);
            vert.g = color.g;
            cylinder.vertices.push_back(vert.g);
            vert.b = color.b;
            cylinder.vertices.push_back(vert.b);
            vert.a = color.a;
            cylinder.vertices.push_back(vert.a);
            // add texture coordinates if true
            if (isTextured) {
                cout << currAngle << " radians\t";
                cylinder.vertices.push_back(max(0.0f, min(1.0f, (cos(currAngle) * 0.5f + 0.5f))));
                cout << max(0.0f, min(1.0f, (cos(currAngle) * 0.5f + 0.5f))) << ", ";
                cylinder.vertices.push_back(max(0.0f, min(1.0f, (sin(currAngle) * 0.5f + 0.5f))));
                cout << max(0.0f, min(1.0f, (sin(currAngle) * 0.5f + 0.5f))) << endl;

            }
        }

        // Create the indices that make up the triangles for the face of the bottom cylinder.
        // The pattern goes [0, 1, 2], [0,2,3], [0,3,4],...
        int centerIndex = segments * 0;
        for (int i = 0; i < segments; ++i) {
            int index = centerIndex;
            cylinder.indices.push_back(index);
            index = centerIndex + i + 1;
            cylinder.indices.push_back(index);
            index = (i + 2) > segments ? centerIndex + 1 : centerIndex + i + 2;
            cylinder.indices.push_back(index);
        }

        // Create and add the center vertex for the top base of the cylinder
        center.x = 0.0f;
        cylinder.vertices.push_back(center.x);
        // center.y = 0.0f;
        center.y = 0.0f + height;
        cylinder.vertices.push_back(center.y);
        // center.z = 0.0f + height;
        center.z = 0.0f;
        // vertex color
        cylinder.vertices.push_back(center.z);
        center.r = color.r;
        cylinder.vertices.push_back(center.r);
        center.g = color.g;
        cylinder.vertices.push_back(center.g);
        center.b = color.b;
        cylinder.vertices.push_back(center.b);
        center.a = color.a;
        cylinder.vertices.push_back(center.a);
        // add texture coordinates if true
        if (isTextured) {
            cylinder.vertices.push_back(0.5f);
            cylinder.vertices.push_back(0.5f);

        }

        // Create the vertices for the ends of the segments of the top cylinder and add them
        for (int i = 0; i < segments; i++) {
            float currAngle = RADS_PER_SEG * i;
            Vertex vert;
            vert.x = cos(currAngle) * radius;
            cylinder.vertices.push_back(vert.x);
            // vert.y = sin(currAngle) * radius;
            vert.y = 0.0f + height;
            cylinder.vertices.push_back(vert.y);
            // vert.z = 0.0f + height;
            vert.z = sin(currAngle) * radius;
            cylinder.vertices.push_back(vert.z);
            // vertex color
            vert.r = color.r;
            cylinder.vertices.push_back(vert.r);
            vert.g = color.g;
            cylinder.vertices.push_back(vert.g);
            vert.b = color.b;
            cylinder.vertices.push_back(vert.b);
            vert.a = color.a;
            cylinder.vertices.push_back(vert.a);
            // add texture coordinates if true
            if (isTextured) {
                cylinder.vertices.push_back((cos(currAngle) * 0.5f) + 0.5f);
                cylinder.vertices.push_back((sin(currAngle)* 0.5f) + 0.5f);

            }
        }

        // Create the indices that make up the triangles for the faces for the top cylinder
        // The pattern goes [(top center index), +i+1, +i+2]
        centerIndex = segments * 1 + 1;
        for (int i = 0; i < segments; ++i) {
            int index = centerIndex;
            cylinder.indices.push_back(index);
            index = centerIndex + i + 1;
            cylinder.indices.push_back(index);
            index = (i + 2) > segments ? centerIndex + 1 : centerIndex + i + 2;
            cylinder.indices.push_back(index);
        }

        // Create the indices that make up the triangles that make up the rectangular faces between the top and bottom bases.
        // Each rectangle is made of two triangles. The pattern basically goes (up->right)(down->left) rotating around the cylinder.
        // Careful attention is made to make sure that if we reach a center index or reach beyond the bounds of the indices, we wrap the index
        // number back to the first index of the appropriate base, completing the cylinder wall.
        int bottomCenterIndex = 0;
        int topCenterIndex = segments + 1;
        for (int i = 0; i < segments; ++i) {
            int leftTriangle[3];
            int rightTriangle[3];
            leftTriangle[0] = (bottomCenterIndex + i + 1) % (topCenterIndex) == 0 ? bottomCenterIndex + 1 : bottomCenterIndex + i + 1;
            leftTriangle[1] = (bottomCenterIndex + i + 2) % (topCenterIndex) == 0 ? bottomCenterIndex + 1 : bottomCenterIndex + i + 2;
            leftTriangle[2] = (topCenterIndex + i + 1) % (topCenterIndex + segments + 1) == 0 ? topCenterIndex + 1 : topCenterIndex + i + 1;
            cylinder.indices.push_back(leftTriangle[0]);
            cylinder.indices.push_back(leftTriangle[1]);
            cylinder.indices.push_back(leftTriangle[2]);

            rightTriangle[0] = (topCenterIndex + i + 1) % (topCenterIndex + segments + 1) == 0 ? topCenterIndex + 1 : topCenterIndex + i + 1;
            rightTriangle[1] = (topCenterIndex + i + 2) % (topCenterIndex + segments + 1) == 0 ? topCenterIndex + 1 : topCenterIndex + i + 2;
            rightTriangle[2] = (bottomCenterIndex + i + 2) % (topCenterIndex) == 0 ? bottomCenterIndex + 1 : bottomCenterIndex + i + 2;
            cylinder.indices.push_back(rightTriangle[0]);
            cylinder.indices.push_back(rightTriangle[1]);
            cylinder.indices.push_back(rightTriangle[2]);
        }

        return cylinder;
	}

    // This function is identical, except that the values of pos.x,y,z are added to each vector.x,y,z to move the object in the world.
    Mesh generateCylinder(const float radius, const float height, const int segments, const Position pos, const bool isTextured, const Color color) {
        Mesh cylinder;
        const float RADS_PER_SEG = (2 * M_PI) / segments * 1.0f;
        Vertex  center;
        center.x = 0.0f + pos.x;
        cylinder.vertices.push_back(center.x);
        center.y = 0.0f + pos.y;
        cylinder.vertices.push_back(center.y);
        center.z = 0.0f + pos.z;
        cylinder.vertices.push_back(center.z);
        // vertex color
        center.r = color.r;
        cylinder.vertices.push_back(center.r);
        center.g = color.g;
        cylinder.vertices.push_back(center.g);
        center.b = color.b;
        cylinder.vertices.push_back(center.b);
        center.a = color.a;
        cylinder.vertices.push_back(center.a);
        // add texture coordinates if true
        if (isTextured) {
            cylinder.vertices.push_back(0.5f);
            cylinder.vertices.push_back(0.5f);

        }

        for (int i = 0; i < segments; i++) {
            float currAngle = RADS_PER_SEG * i;
            Vertex vert;
            vert.x = cos(currAngle) * radius + pos.x;
            cylinder.vertices.push_back(vert.x);
            vert.y = 0.0f + pos.y;
            cylinder.vertices.push_back(vert.y);
            vert.z = sin(currAngle) * radius + pos.z;
            cylinder.vertices.push_back(vert.z);
            // vertex color
            vert.r = color.r;
            cylinder.vertices.push_back(vert.r);
            vert.g = color.g;
            cylinder.vertices.push_back(vert.g);
            vert.b = color.b;
            cylinder.vertices.push_back(vert.b);
            vert.a = color.a;
            cylinder.vertices.push_back(vert.a);
            // add texture coordinates if true
            // add texture coordinates if true
            if (isTextured) {
                cylinder.vertices.push_back(cos(currAngle));
                cylinder.vertices.push_back(sin(currAngle));

            }
        }

        int centerIndex = segments * 0;
        for (int i = 0; i < segments; ++i) {
            int index = centerIndex;
            cylinder.indices.push_back(index);
            index = centerIndex + i + 1;
            cylinder.indices.push_back(index);
            index = (i + 2) > segments ? centerIndex + 1 : centerIndex + i + 2;
            cylinder.indices.push_back(index);
        }

        center.x = 0.0f + pos.x;
        cylinder.vertices.push_back(center.x);
        center.y = 0.0f + height + pos.y;
        cylinder.vertices.push_back(center.y);
        center.z = 0.0f + pos.z;
        cylinder.vertices.push_back(center.z);
        // vertex color
        center.r = color.r;
        cylinder.vertices.push_back(center.r);
        center.g = color.g;
        cylinder.vertices.push_back(center.g);
        center.b = color.b;
        cylinder.vertices.push_back(center.b);
        center.a = color.a;
        cylinder.vertices.push_back(center.a);

        for (int i = 0; i < segments; i++) {
            float currAngle = RADS_PER_SEG * i;
            Vertex vert;
            vert.x = cos(currAngle) * radius + pos.x;
            cylinder.vertices.push_back(vert.x);
            vert.y = 0.0f + height + pos.y;
            cylinder.vertices.push_back(vert.y);
            vert.z = sin(currAngle) * radius + pos.z;
            // vertex color
            cylinder.vertices.push_back(vert.z);
            vert.r = color.a;
            cylinder.vertices.push_back(vert.r);
            vert.g = color.g;
            cylinder.vertices.push_back(vert.g);
            vert.b = color.b;
            cylinder.vertices.push_back(vert.b);
            vert.a = color.a;
            cylinder.vertices.push_back(vert.a);
            // add texture coordinates if true
            if (isTextured) {
                cylinder.vertices.push_back(cos(currAngle));
                cylinder.vertices.push_back(sin(currAngle));

            }
        }

        centerIndex = segments * 1 + 1;
        for (int i = 0; i < segments; ++i) {
            int index = centerIndex;
            cylinder.indices.push_back(index);
            index = centerIndex + i + 1;
            cylinder.indices.push_back(index);
            index = (i + 2) > segments ? centerIndex + 1 : centerIndex + i + 2;
            cylinder.indices.push_back(index);
        }

        int bottomCenterIndex = 0;
        int topCenterIndex = segments + 1;
        for (int i = 0; i < segments; ++i) {
            int leftTriangle[3];
            int rightTriangle[3];
            leftTriangle[0] = (bottomCenterIndex + i + 1) % (topCenterIndex) == 0 ? bottomCenterIndex + 1 : bottomCenterIndex + i + 1;
            leftTriangle[1] = (bottomCenterIndex + i + 2) % (topCenterIndex) == 0 ? bottomCenterIndex + 1 : bottomCenterIndex + i + 2;
            leftTriangle[2] = (topCenterIndex + i + 1) % (topCenterIndex + segments + 1) == 0 ? topCenterIndex + 1 : topCenterIndex + i + 1;
            cylinder.indices.push_back(leftTriangle[0]);
            cylinder.indices.push_back(leftTriangle[1]);
            cylinder.indices.push_back(leftTriangle[2]);

            rightTriangle[0] = (topCenterIndex + i + 1) % (topCenterIndex + segments + 1) == 0 ? topCenterIndex + 1 : topCenterIndex + i + 1;
            rightTriangle[1] = (topCenterIndex + i + 2) % (topCenterIndex + segments + 1) == 0 ? topCenterIndex + 1 : topCenterIndex + i + 2;
            rightTriangle[2] = (bottomCenterIndex + i + 2) % (topCenterIndex) == 0 ? bottomCenterIndex + 1 : bottomCenterIndex + i + 2;
            cylinder.indices.push_back(rightTriangle[0]);
            cylinder.indices.push_back(rightTriangle[1]);
            cylinder.indices.push_back(rightTriangle[2]);
        }

        return cylinder;
    }

    // This function generates a cube.
    Mesh generateCube(const float length, const float width, const float height, const bool isTextured, const Color color) {
        Mesh cube;
        Vertex vert;

        // bottom front right vert [0]
        vert.x = (width / 2.0f);
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f);
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(0.0f);

        }

        // bottom front left vert [1]
        vert.x = (width / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f);
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(0.0f);

        }

        // bottom back left vert [2]
        vert.x = (width / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(0.0f);

        }

        // bottom back right vert [3]
        vert.x = (width / 2.0f);
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(0.0f);

        }

        // top front right vert [4]
        vert.x = (width / 2.0f);
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f);
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(1.0f);

        }

        // top front left vert [5]
        vert.x = (width / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f);
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(1.0f);

        }

        // top back left vert [6]
        vert.x = (width / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(1.0f);

        }

        // top back right vert [7]
        vert.x = (width / 2.0f);
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(1.0f);

        }
        
        
        cube.indices = {
                        0,1,2,
                        0,3,2,
                        4,5,6,
                        4,7,6,
                        4,5,1,
                        4,0,1,
                        7,6,2,
                        7,3,2,
                        7,4,0,
                        7,3,0,
                        6,5,1,
                        6,2,1
        };
        return cube;
    }

    // This function is identical, but adds pos.x,y,z to vertex.x,y,z to place the cube in world space.
    Mesh generateCube(const float length, const float width, const float height, const Position pos, const bool isTextured, const Color color) {
        Mesh cube;
        Vertex vert;

        // bottom front right vert [0]
        vert.x = (width / 2.0f) + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(0.0f);

        }

        // bottom front left vert [1]
        vert.x = (width / 2.0f) * -1.0f + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(0.0f);

        }

        // bottom back left vert [2]
        vert.x = (width / 2.0f) * -1.0f + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(0.0f);

        }

        // bottom back right vert [3]
        vert.x = (width / 2.0f) + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(0.0f);

        }

        // top front right vert [4]
        vert.x = (width / 2.0f) + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(1.0f);

        }

        // top front left vert [5]
        vert.x = (width / 2.0f) * -1.0f + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.g;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(1.0f);

        }

        // top back left vert [6]
        vert.x = (width / 2.0f) * -1.0f + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f + pos.z;
        cube.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(1.0f);
            cube.vertices.push_back(1.0f);

        }

        // top back right vert [7]
        vert.x = (width / 2.0f) + pos.x;
        cube.vertices.push_back(vert.x);
        vert.y = 0.0f + height + pos.y;
        cube.vertices.push_back(vert.y);
        vert.z = (length / 2.0f) * -1.0f + pos.z;
        // vertex color
        cube.vertices.push_back(vert.z);
        vert.r = color.r;
        cube.vertices.push_back(vert.r);
        vert.g = color.g;
        cube.vertices.push_back(vert.g);
        vert.b = color.b;
        cube.vertices.push_back(vert.b);
        vert.a = color.a;
        cube.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            cube.vertices.push_back(0.0f);
            cube.vertices.push_back(1.0f);

        }
        
        cube.indices = {
                        0,1,2,
                        0,3,2,
                        4,5,6,
                        4,7,6,
                        4,5,1,
                        4,0,1,
                        7,6,2,
                        7,3,2,
                        7,4,0,
                        7,3,0,
                        6,5,1,
                        6,2,1
        };
        return cube;
    }

    Mesh generatePlane(const float length, const float width, const Position pos, const bool isTextured, const Color color) {
        Mesh plane;
        Vertex vert;

        // Top Right [0]
        vert.x = (width / 2.0f) + pos.x;
        vert.y = 0.0f + pos.y;
        vert.z = (length / 2.0f) + pos.z;
        vert.r = color.r;
        vert.g = color.g;
        vert.b = color.b;
        vert.a = color.a;
        if (isTextured) {
            vert.s = 1.0f;
            vert.t = 1.0f;
        }
        cout << "Top-Right: " << vert << endl;
        plane.addVertex(vert);
        
        // Bottom Right [1]
        vert.x = (width / 2.0f) + pos.x;
        vert.y = 0.0f + pos.y;
        vert.z = ((length / 2.0f) * -1.0f) + pos.z;
        vert.r = color.r;
        vert.g = color.g;
        vert.b = color.b;
        vert.a = color.a;
        if (isTextured) {
            vert.s = 1.0f;
            vert.t = 0.0f;
        }
        cout << "Bottom-Right: " << vert << endl;
        plane.addVertex(vert);

        // Bottom Left [2]
        vert.x = ((width / 2.0f) * -1.0f) + pos.x;
        vert.y = 0.0f + pos.y;
        vert.z = ((length / 2.0f) * -1.0f) + pos.z;
        vert.r = color.r;
        vert.g = color.g;
        vert.b = color.b;
        vert.a = color.a;
        if (isTextured) {
            vert.s = 0.0f;
            vert.t = 0.0f;
        }
        cout << "Bottom-Left: " << vert << endl;
        plane.addVertex(vert);

        // Top Left
        vert.x = ((width / 2.0f) * -1.0) + pos.x;
        vert.y = 0.0f + pos.y;
        vert.z = (length / 2.0f) + pos.z;
        vert.r = color.r;
        vert.g = color.g;
        vert.b = color.b;
        vert.a = color.a;
        if (isTextured) {
            vert.s = 0.0f;
            vert.t = 1.0f;
        }
        cout << "Top-Left: " << vert << endl;
        plane.addVertex(vert);

        plane.indices = {
            0, 3, 1,
            2, 3, 1
        };

        return plane;
    }

    Mesh generatePyramid(const float baseLength, const float height, const bool isTextured, const Color color) {
        Mesh pyramid;
        pyramid.isTextured = isTextured;
        // Triangle vertices that make up the pyramid (4 triangles make up the sides, 2 triangles make up the square base)
        Vertex vert;

        // Top of pyramid [0]
        vert.x = 0.0f;
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f + (height);
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f;
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(0.5f);
            pyramid.vertices.push_back(1.0f);
        }
        
        // Left-front vert [1]
        vert.x = 0.0f - (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f + (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(0.0f);
            pyramid.vertices.push_back(0.0f);
        }

        // right-front vert [2]
        vert.x = 0.0f + (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f + (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(1.0f);
            pyramid.vertices.push_back(0.0f);
        }


        // left-back vert [3]
        vert.x = 0.0f - (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f - (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(1.0f);
            pyramid.vertices.push_back(0.0f);
        }

        // right-back vert[4]
        vert.x = 0.0f + (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f - (baseLength / 2.0f);
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(0.0f);
            pyramid.vertices.push_back(0.0f);
        }

        // mesh indices
        pyramid.indices = {
            0, 1, 2,
            0, 1, 3,
            0, 2, 4,
            0, 3, 4,
            1, 3, 2,
            2, 4, 3
        };

        return pyramid;
    }

    // generate pyramid. Add texture and color parameters
    Mesh generatePyramid(const float baseLength, const float height, const Position pos, const bool isTextured, const Color color) {
        Mesh pyramid;
        pyramid.isTextured = isTextured;
        // Triangle vertices that make up the pyramid (4 triangles make up the sides, 2 triangles make up the square base)
        Vertex vert;

        // Top of pyramid [0]
        vert.x = 0.0 + pos.x;
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f + height + pos.y;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f + pos.z;
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        // add texture coordinates if true
        pyramid.vertices.push_back(vert.a);
        if (isTextured) {
            pyramid.vertices.push_back(0.5f);
            pyramid.vertices.push_back(1.0f);
        }


        // Left-front vert [1]
        vert.x = 0.0f - (baseLength / 2.0f) + pos.x;
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f + (baseLength / 2.0f) + pos.z;
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        // add texture coordinates if true
        pyramid.vertices.push_back(vert.a);
        if (isTextured) {
            pyramid.vertices.push_back(0.0f);
            pyramid.vertices.push_back(0.0f);
        }

        // right-front vert [2]
        vert.x = 0.0f + (baseLength / 2.0f) + pos.x;
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f + (baseLength / 2.0f) + pos.z;
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(1.0f);
            pyramid.vertices.push_back(0.0f);
        }


        // left-back vert [3]
        vert.x = 0.0f - (baseLength / 2.0f) + pos.x;
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f - (baseLength / 2.0f) + pos.z;
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(1.0f);
            pyramid.vertices.push_back(0.0f);
        }

        // right-back vert [4]
        vert.x = 0.0f + (baseLength / 2.0f) + pos.x;
        pyramid.vertices.push_back(vert.x);
        vert.y = 0.0f + pos.y;
        pyramid.vertices.push_back(vert.y);
        vert.z = 0.0f - (baseLength / 2.0f) + pos.z;
        pyramid.vertices.push_back(vert.z);
        // vertex color
        vert.r = color.r;
        pyramid.vertices.push_back(vert.r);
        vert.g = color.g;
        pyramid.vertices.push_back(vert.g);
        vert.b = color.b;
        pyramid.vertices.push_back(vert.b);
        vert.a = color.a;
        pyramid.vertices.push_back(vert.a);
        // add texture coordinates if true
        if (isTextured) {
            pyramid.vertices.push_back(0.0f);
            pyramid.vertices.push_back(0.0f);
        }

        // mesh indices
        pyramid.indices = {
            0, 1, 2,
            0, 1, 3,
            0, 2, 4,
            0, 3, 4,
            1, 3, 2,
            2, 4, 3
        };

        return pyramid;
    }

	
}