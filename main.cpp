#include "struct.h"
#include <iostream>
#include <regex>
#include <fstream>
#include <cassert>

#define OBJ_FILENAME "peach.obj"

std::ostream & operator<<(std::ostream &os, const Vertex &v){
    os << "Vertex: " << v.v[0] << " " << v.v[1] << " " << v.v[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Normal &vn){
    os << "Normal: " << vn.n[0] << " " << vn.n[1] << " " << vn.n[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Texture &vt){
    os << "Texture coordinates: " << vt.t[0] << " " << vt.t[1];
    if(vt.dim == 3) os << " " << vt.t[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Face &f){
    std::cout << "face: " <<std::endl;
    for(int i = 0; i < f.nb; i++){
        std::cout << f.v[i] << "/" << f.vt[i] << "/" << f.vn[i] << std::endl;
    }
    return os;
}

std::vector<std::string> split(const std::string& input, const std::string& regex) {
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

void parse_line(std::string line, Object *object)
{
    if(line.length() == 0)
        return;

    std::vector<std::string> tokens = split(line, "\\s+");  //\s is whitespace


    if(tokens[0][0] == '#') //comment
        return;

    if(tokens[0].compare("v") == 0) { //vertex
        Vertex v = parse_vertex( {tokens.begin()+1, tokens.end()} );
        object->vertices.push_back(v);
        std::cout << "Added: " << v << std::endl;
    }else if(tokens[0].compare("vn") == 0){ //normal
        Normal n = parse_normal( {tokens.begin()+1, tokens.end()} );
        object->normals.push_back(n);
        std::cout << "Added: " << n << std::endl;
    }else if(tokens[0].compare("vt") == 0){ //texture
        Texture t = parse_texture( {tokens.begin()+1, tokens.end()} );
        object->texture_coords.push_back(t);
        std::cout << "Added: " << t << std::endl;
    }else if(tokens[0].compare("f") == 0){ //face

    }
    else std::cerr << "unsupported keyword: " << tokens[0] << std::endl;

}

int main(int argc, char ** argv){
    std::string line;
    std::ifstream is;
    is.open(OBJ_FILENAME);

    Object object;

    if(!is.is_open()){
        std::cerr << "Failed to open file: " << OBJ_FILENAME << std::endl;
        return 1;
    }

    while(std::getline(is, line)){
        parse_line(line, &object);
    }

    return 0;
}