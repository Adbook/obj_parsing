#ifndef OBJ_PARSING_STRUCT_H
#define OBJ_PARSING_STRUCT_H

#include <vector>

#define MAX_FACE_SIZE 5

typedef struct Vertex{
    float v[3]; //vertex coordinates
}Vertex;

typedef struct Normal{
    float n[3]; //vertex normal coordinates
}Normal;

typedef struct Texture{
    int dim;
    float vt[3];
}Texture;

typedef struct Face{ //only contains indices to match with the other object data
    int nb;
    int v[MAX_FACE_SIZE];
    int vn[MAX_FACE_SIZE];
    int vt[MAX_FACE_SIZE];
}Face;

typedef struct Object{
    std::vector<Vertex> vertices;
    std::vector<Texture> texture_coords;
    std::vector<Normal> normals;
    std::vector<Face> faces;
}Object;

#endif //OBJ_PARSING_STRUCT_H
