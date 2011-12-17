/*
 * MeshBuilder.h
 *
 *  Created on: Dec 11, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef MESHBUILDER_H_
#define MESHBUILDER_H_

#include "math/Vector3f.hpp"
#include <vector>

namespace stein {

typedef std::pair<float, float> UV;

class MeshBuilder {
    struct Face {
        Face& setUvIndices(unsigned int a, unsigned int b, unsigned int c);
        Face& setNormalIndices(unsigned int a, unsigned int b, unsigned int c);
    private:
        friend class MeshBuilder;
        const static unsigned int NA = -1;
        Face(unsigned int a, unsigned int b, unsigned int c) :
            v1(a), v2(b), v3(c), n1(NA), n2(NA), n3(NA), t1(NA), t2(NA), t3(NA) {
        }
        unsigned int v1, v2, v3;
        unsigned int n1, n2, n3;
        unsigned int t1, t2, t3;
    };
public:
    // builder functions
    void addVertex(float x, float y, float z);
    void addVertex(const Vector3f&p);

    void addUV(float u, float v);

    void addNormal(float x, float y, float z);
    void addNormal(const Vector3f&n);

    Face& addFace(unsigned int v1, unsigned int v2, unsigned int v3);

    // modifiers
    void centerAndNormalizeMesh();
    void normalizeNormals();

    // getters
    const std::vector<Vector3f>& vertices() const;
    const std::vector<UV>& uvs() const;
    const std::vector<Vector3f>& normals() const;

    /**
     * This function flattens the indices, the normals and uvs
     */
    void unpack( //
        std::vector<unsigned int> &indices, //
        std::vector<Vector3f>& vertices, //
        std::vector<Vector3f>& normals, //
        std::vector<UV>& uvs) const;

private:
    std::vector<Vector3f> m_Vertices;
    std::vector<Vector3f> m_Normals;
    std::vector<UV> m_Uvs;
    std::vector<Face> m_Faces;
};

} // namespace stein

#endif /* MESHBUILDER_H_ */
