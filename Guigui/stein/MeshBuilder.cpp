/*
 * MeshBuilder.cpp
 *
 *  Created on: Dec 11, 2011
 *      Author: Guillaume Chatelet
 */

#include "MeshBuilder.h"
#include <stdexcept>
#include <cfloat>
#include <algorithm>

using namespace std;

namespace stein {

struct MinMax {
    Vector3f min;
    Vector3f max;

    MinMax() :
        min(FLT_MAX, FLT_MAX, FLT_MAX), max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {
    }

    void operator()(const Vector3f& value) {
        min.x = std::min(min.x, value.x);
        min.y = std::min(min.y, value.y);
        min.z = std::min(min.z, value.z);
        max.x = std::max(max.x, value.x);
        max.y = std::max(max.y, value.y);
        max.z = std::max(max.z, value.z);
    }
};

struct MoveAndScale {
    const Vector3f add;
    const float mul;

    MoveAndScale(const Vector3f &move, const float scale) :
        add(move), mul(scale) {
    }

    void operator()(Vector3f& value) {
        value += add;
        value *= mul;
    }
};

MeshBuilder::Face& MeshBuilder::Face::setUvIndices(unsigned int a, unsigned int b, unsigned int c) {
    t1 = a;
    t2 = b;
    t3 = c;
    return *this;
}

MeshBuilder::Face& MeshBuilder::Face::setNormalIndices(unsigned int a, unsigned int b, unsigned int c) {
    n1 = a;
    n2 = b;
    n3 = c;
    return *this;
}

void MeshBuilder::addVertex(float x, float y, float z) {
    addVertex(Vector3f(x, y, z));
}

void MeshBuilder::addVertex(const Vector3f&p) {
    m_Vertices.push_back(p);
}

void MeshBuilder::addUV(float u, float v) {
    m_Uvs.push_back(make_pair(u, v));
}

void MeshBuilder::addNormal(float x, float y, float z) {
    addNormal(Vector3f(x, y, z));
}

void MeshBuilder::addNormal(const Vector3f&n) {
    m_Normals.push_back(n);
}

MeshBuilder::Face& MeshBuilder::addFace(unsigned int v1, unsigned int v2, unsigned int v3) {
    m_Faces.push_back(Face(v1, v2, v3));
    return m_Faces.back();
}

void MeshBuilder::centerAndNormalizeMesh() {
    const MinMax minMax = for_each(m_Vertices.begin(), m_Vertices.end(), MinMax());
    const Vector3f size(minMax.max - minMax.min);
    const Vector3f center = (minMax.max + minMax.min) / 2;
    const float maxDimension = max(size.x, max(size.y, size.z));
    const float scale = maxDimension == 0 ? 1 : 1 / maxDimension;
    for_each(m_Vertices.begin(), m_Vertices.end(), MoveAndScale(-center, scale));
}

void MeshBuilder::normalizeNormals() {
    for_each(m_Normals.begin(), m_Normals.end(), mem_fun_ref(&Vector3f::normalize));
}

const vector<Vector3f>& MeshBuilder::vertices() const {
    return m_Vertices;
}

const vector<UV>& MeshBuilder::uvs() const {
    return m_Uvs;
}

const vector<Vector3f>& MeshBuilder::normals() const {
    return m_Normals;
}

void MeshBuilder::unpack( //
    vector<unsigned int> &indices, //
    vector<Vector3f>& vertices, //
    vector<Vector3f>& normals, //
    vector<UV>& uvs) const {

    const size_t elements = m_Faces.size() * 3;

    indices.clear();
    vertices.clear();
    normals.clear();
    uvs.clear();

    indices.reserve(elements);
    vertices.reserve(elements);
    normals.reserve(elements);
    uvs.reserve(elements);

    size_t i = 0;
    for (vector<Face>::const_iterator itr = m_Faces.begin(); itr != m_Faces.end(); ++itr) {
        indices.push_back(i++);
        indices.push_back(i++);
        indices.push_back(i++);
        vertices.push_back(m_Vertices[itr->v1]);
        vertices.push_back(m_Vertices[itr->v2]);
        vertices.push_back(m_Vertices[itr->v3]);
        if (itr->n1 != Face::NA) {
            normals.push_back(m_Normals[itr->n1]);
            normals.push_back(m_Normals[itr->n2]);
            normals.push_back(m_Normals[itr->n3]);
        }
        if (itr->t1 != Face::NA) {
            uvs.push_back(m_Uvs[itr->t1]);
            uvs.push_back(m_Uvs[itr->t2]);
            uvs.push_back(m_Uvs[itr->t3]);
        }
    }
}

} // namespace stein
