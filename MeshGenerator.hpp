/*
* Author:		Christopher Richards
* email:		chritopher.richards@snhu.edu
* Date:			07/24/2023
* File:			MeshGenerator.h
* Description:	This is a library I created to generate the vertex and index
*				data for generating primitive shapes as our modules go on.
*				I am certain there are already libraries built for this, but
*				I wanted to see if I could do it myself.
*/

// Includes
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#pragma once

// This library will be part of the namespace std;
namespace std {

	// Struct Color for generating colors.
	struct Color {
		float r, g, b, a;
		Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f) {
			r = max(0.0f, min(1.0f, red));
			g = max(0.0f, min(1.0f, green));
			b = max(0.0f, min(1.0f, blue));
			a = max(0.0f, min(1.0f, alpha));
		}

		Color(int red = 255, int green = 255, int blue = 255, int alpha = 255) {
			r = (max(0, min(255, red)) * 1.0f / 255.0f);
			g = (max(0, min(255, green)) * 1.0f / 255.0f);
			b = (max(0, min(255, blue)) * 1.0f / 255.0f);
			a = (max(0, min(255, alpha)) * 1.0f / 255.0f);
		}

		// Prebuilt colors that are a static member of MeshGenerator::Color
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color WHITE;
		static const Color BLACK;
	};

	// Vertex container;
	typedef struct Vertex {
		float x, y, z, r, g, b, a, s = NULL, t = NULL;
		friend ostream& operator<<(ostream& out, Vertex vertex) {
			out << "Pos(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")\t";
			out << "rgba(" << vertex.r << ", " << vertex.g << ", " << vertex.b << ", " << vertex.a << ")\t";
			out << "texCoord(" << vertex.s << ", " << vertex.t << ")";
			return out;
		}
	};

	// Position container for choosing world position when generating a mesh.
	typedef struct Position {
		float x, y, z;
		Position(float posX, float posY, float posZ) {
			x = posX;
			y = posY;
			z = posZ;
		}
	};

	// Struct to hold mesh data
	struct Mesh {
		vector<float> vertices;
		vector<short> indices;
		bool isTextured = false;
		unsigned int texture  = NULL;
		Mesh addVertex(const Vertex vertex) {
			vertices.push_back(vertex.x);
			vertices.push_back(vertex.y);
			vertices.push_back(vertex.z);
			if (vertex.r != NULL) {
				vertices.push_back(vertex.r);
			}
			if (vertex.g != NULL) {
				vertices.push_back(vertex.g);
			}
			if (vertex.b != NULL) {
				vertices.push_back(vertex.b);
			}
			if (vertex.a != NULL) {
				vertices.push_back(vertex.a);
			}
			if (vertex.s != NULL) {
				vertices.push_back(vertex.s);
			}
			if (vertex.t != NULL) {
				vertices.push_back(vertex.t);
			}
			return *this;
		}

	};

	// Function declarations for primitive shapes.
	Mesh generateCylinder(const float radius, const float height, const int segments, const bool isTextured = false, const Color color = Color::WHITE);
	Mesh generateCylinder(const float radius, const float height, const int segments, const Position pos, const bool isTextured = false, const Color color = Color::WHITE);
	Mesh generateCube(const float length, const float width, const float height, const bool isTextured = false, const Color color = Color::WHITE);
	Mesh generateCube(const float length, const float width, const float height, const Position pos, const bool isTextured = false, const Color color = Color::WHITE);
	Mesh generatePlane(const float length, const float width, const Position pos, const bool isTextured = false, const Color color = Color::WHITE);
	Mesh generatePyramid(const float baseLength, const float height, const bool isTextured = false, const Color color = Color::WHITE);
	Mesh generatePyramid(const float baseLength, const float height, const Position pos, const bool isTextured = false, const Color color = Color::WHITE);
}