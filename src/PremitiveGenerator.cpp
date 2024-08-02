#include "PremitiveGenerator.h"
#include <glm/ext/scalar_uint_sized.hpp>

using std::array, std::vector;

PremitiveGenerator::PremitiveGenerator() {
}

mesh_t PremitiveGenerator::createSphere(float radius, glm::uint32_t segLatitude, glm::uint32 segLongitude) {
    
    mesh_t Sphere; 

    for (unsigned int lat = 0; lat <= segLatitude; ++lat) {
        float theta = lat * M_PI / segLatitude;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (unsigned int lon = 0; lon <= segLongitude; ++lon) {
            float phi = lon * 2 * M_PI / segLongitude;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            glm::vec3 vertex;
            vertex.x = radius * cosPhi * sinTheta;
            vertex.y = radius * cosTheta;
            vertex.z = radius * sinPhi * sinTheta;

            glm::vec3 normal;
            normal.x = cosPhi * sinTheta;
            normal.y = cosTheta;
            normal.z = sinPhi * sinTheta;

            Sphere.vertices.push_back(vertex.x);
            Sphere.vertices.push_back(vertex.y);
            Sphere.vertices.push_back(vertex.z);
            Sphere.vertices.push_back(-1);
            Sphere.vertices.push_back(-1);
            Sphere.vertices.push_back(normal.x);
            Sphere.vertices.push_back(normal.y);
            Sphere.vertices.push_back(normal.z);
        }
    }

    for (unsigned int lat = 0; lat < segLatitude; ++lat) {
        for (unsigned int lon = 0; lon < segLongitude; ++lon) {
            unsigned int first = (lat * (segLongitude + 1)) + lon;
            unsigned int second = first + segLongitude + 1;

            Sphere.indices.push_back(first);
            Sphere.indices.push_back(second);
            Sphere.indices.push_back(first + 1);

            Sphere.indices.push_back(second);
            Sphere.indices.push_back(second + 1);
            Sphere.indices.push_back(first + 1);
        }
    }

    return Sphere;
}

glm::vec3 PremitiveGenerator::calcNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    glm::vec3 u = p2 - p1;
    glm::vec3 v = p3 - p1;
    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    return normal;
}

std::vector<polygon_t> PremitiveGenerator::generatePolygons(std::vector<dot_t> &dots) {
    std::vector<polygon_t> polygons;
    
    if (dots.size() < 3) {
        std::cerr << "Error: At least 3 points are required to generate a polygon." << std::endl;
        return polygons;
    }
    
    // Алгоритм Ear Clipping для генерации треугольников (наивный подход)
    for (size_t i = 1; i < dots.size() - 1; ++i) {
        polygon_t polygon;
        polygon.vertices.push_back(dots[0]);
        polygon.vertices.push_back(dots[i]);
        polygon.vertices.push_back(dots[i + 1]);
        polygon.vertices[0].normal = calcNormal(dots[0].pos, dots[i].pos, dots[i + 1].pos);
        polygon.vertices[1].normal = calcNormal(dots[0].pos, dots[i].pos, dots[i + 1].pos);
        polygon.vertices[2].normal = calcNormal(dots[0].pos, dots[i].pos, dots[i + 1].pos);
        polygons.push_back(polygon);
    }

    return polygons;
}
