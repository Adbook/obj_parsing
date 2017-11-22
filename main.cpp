#include "struct.h"
#include <iostream>
#include <regex>

std::ostream & operator<<(std::ostream &os, const Vertex &v){
    os << "Vertex: " << v.v[0] << " " << v.v[1] << " " << v.v[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Normal &vn){
    os << "Normal: " << vn.n[0] << " " << vn.n[1] << " " << vn.n[2];
    return os;
}
std::ostream & operator<<(std::ostream &os, const Texture &vt){
    os << "Texture coordinates: " << vt.vt[0] << " " << vt.vt[1];
    if(vt.dim == 3) os << " " << vt.vt[2];
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


int main(int argc, char ** argv){

    return 0;
}