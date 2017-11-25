#include "struct.h"
#include <iostream>
#include <regex>
#include <fstream>
#include <cassert>

#define OBJ_FILENAME "cup.obj"

std::ostream & operator<<(std::ostream &os, const Vertex &v){
    os << "v " << v.v[0] << " " << v.v[1] << " " << v.v[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Normal &vn){
    os << "vn " << vn.n[0] << " " << vn.n[1] << " " << vn.n[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Texture &vt){
    os << "vt " << vt.t[0] << " " << vt.t[1];
    if(vt.dim == 3) os << " " << vt.t[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Face &f){
    for(int i = 0; i < f.nb; i++){
        os << "f ";
        if(f.v[i] != -1) os << f.v[i];
        os << "/";
        if(f.vt[i] != -1) os << f.vt[i];
        os << "/";
        if(f.vn[i] != -1) os << f.vn[i];
        if(i != f.nb - 1) os << std::endl;
    }
    return os;
}

std::ostream & operator<<(std::ostream &os, const Object &o){
    os << "o " << o.name << std::endl;
    for(auto v : o.vertices){
        os << v << std::endl;
    }
    for(auto t : o.texture_coords){
        os << t << std::endl;
    }
    for(auto n : o.normals){
        os << n << std::endl;
    }
    for(auto f : o.faces){
        os << f << std::endl;
        break;
    }
    return os;
}

typedef struct OpenGLObjData{
    int size;
    float *vertices;
    float *texture_coords;
    int *connectivity;
}OpenGLObjData;

std::vector<std::string> split(const std::string& input, const std::string& regex) { //todo: too slow, redo with strtok !
    std::regex re(regex);
    std::sregex_token_iterator
            first{input.begin(), input.end(), re, -1},
            last;
    return {first, last};
}

Vertex parse_vertex(std::vector<std::string> tokens){
    assert(tokens.size() == 3);
    Vertex vertex;
    int i = 0;
    float value = 0;
    for(auto s : tokens){
        value = std::stof(s);  //should eventually catch std::invalid_argument...
        vertex.v[i] = value;
        i++;
    }

    return vertex;
}

Normal parse_normal(std::vector<std::string> tokens){ //todo: too similar to parse_vertex, remove reuse
    assert(tokens.size() == 3);
    Normal normal;
    int i = 0;
    float value = 0;
    for(auto s : tokens){
        value = std::stof(s);  //should catch std::invalid_argument...
        normal.n[i] = value;
        i++;
    }

    return normal;
}

Texture parse_texture(std::vector<std::string> tokens){ //todo: also very similar to the other parsing functions
    assert(tokens.size() == 2 || tokens.size() == 3);
    Texture tex;
    int i = 0;
    float value = 0;
    for(auto s : tokens){
        value = std::stof(s);  //should catch std::invalid_argument...
        tex.t[i] = value;
        i++;
    }
    tex.dim = i;
    return tex;
}

Face parse_face(std::vector<std::string> tokens){
    assert(tokens.size() <= MAX_FACE_SIZE);
    Face f;
    int i = 0;
    for(auto s : tokens){
        std::vector<std::string> indexes = split(s, "\\/");

        int j = 0;
        int values[3];
        for(auto index : indexes){
            if(index.compare("") != 0)
                values[j] = std::stoi(index);
            else values[j] = -1;
            j++;
        }
        f.v[i] = values[0]; f.vt[i] = values[1]; f.vn[i] = values[2];
        i++;
    }
    f.nb = i;
    return f;
}

void parse_line(std::string line, std::vector<Object*> *objects)
{
    if(line.length() == 0)
        return;

    std::vector<std::string> tokens = split(line, "\\s+");  //\s is whitespace

    if(tokens[0][0] == '#') //comment
        return;

    if(tokens[0].compare("o") == 0){
        Object *obj = new Object;
        obj->name = * (tokens.begin()+1);
        objects->push_back(obj);
        return;
    }

    if(objects->size() == 0) return ;

    Object *object =   (*objects) [objects->size() - 1];

    if(tokens[0].compare("v") == 0) { //vertex
        Vertex v = parse_vertex( {tokens.begin()+1, tokens.end()} );
        object->vertices.push_back(v);
    }else if(tokens[0].compare("vn") == 0){ //normal
        Normal n = parse_normal( {tokens.begin()+1, tokens.end()} );
        object->normals.push_back(n);
    }else if(tokens[0].compare("vt") == 0){ //texture
        Texture t = parse_texture( {tokens.begin()+1, tokens.end()} );
        object->texture_coords.push_back(t);
    }else if(tokens[0].compare("f") == 0){ //face
        Face f = parse_face( {tokens.begin()+1, tokens.end()} );
        object->faces.push_back(f);
    }
    else std::cerr << "unsupported keyword: " << tokens[0] << std::endl;

}

float *generate_vertex_array(Object & o){
    float *array = new float[o.vertices.size()];
    int i = 0;
    for(auto v : o.vertices){
        array[i] = v.v[0];
        array[i+1] = v.v[1];
        array[i+2] = v.v[2];
        i += 3;
    }
    return array;
}

int main(int argc, char ** argv){
    std::string line;
    std::ifstream is;
    is.open(OBJ_FILENAME);

    std::vector<Object*> objects;

    if(!is.is_open()){
        std::cerr << "Failed to open file: " << OBJ_FILENAME << std::endl;
        return 1;
    }

    while(std::getline(is, line)){
        parse_line(line, &objects);
    }

    for(auto o : objects){
        std::cout << *o << std::endl;
    }

    return 0;
}