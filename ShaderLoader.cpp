#include "ShaderLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
    #include "windows.h"
#else
    #include <dirent.h>
#endif

ShaderLoader::ShaderLoader(){
    addShaderPath("shaders\\");     //default path
}

ShaderLoader::ShaderLoader(std::string shaderFolder){
    addShaderPath("shaders\\");
    addShaderPath(shaderFolder);
    loadShaders();
}

ShaderLoader::~ShaderLoader(){

}

std::vector<std::string> ShaderLoader::getFilenames(){
    std::vector<std::string> foundedFiles;

    for(uint32_t i = 0; i < shaderPaths.size(); i++){
        std::cout << "Looking for shaders in " << shaderPaths[i] << " folder" << std::endl;
        #ifdef _WIN32
            std::string pattern(shaderPaths[i]);
            pattern.append("\\*");
            WIN32_FIND_DATA data;
            HANDLE hFind;
            if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
                do {
                    std::string temp = data.cFileName;
                    if(temp.find(".glsl") != std::string::npos || temp.find(".vs") != std::string::npos || temp.find(".fs") != std::string::npos) {
                        foundedFiles.push_back(temp);
                        std::cout << "Founded file " << foundedFiles.back() << std::endl;
                    }
                } while (FindNextFile(hFind, &data) != 0);
                FindClose(hFind);
                if(foundedFiles.size() > 0) {
                    std::cout << "Founded " << foundedFiles.size() << " shaders in folder " << shaderPaths[i] << std::endl;
                    pathPrefix = shaderPaths[i];
                    break;
                }else{
                    std::cout << "Can't found shaders in " << shaderPaths[i] << " folder." << std::endl;
                    continue;
                }
            }
        #else
            DIR* dirp = opendir(shaderFolder.c_str());
            struct dirent * dp;
            while ((dp = readdir(dirp)) != NULL) {
                std::string temp = data.cFileName;
                if(temp.find(".glsl") != std::string::npos) {
                    foundedFiles.push_back(temp);
                    std::cout << "Founded file " << foundedFiles.back() << std::endl;
                }
            }
            closedir(dirp);
            if(foundedFiles.size() > 0) {
                std::cout << "Founded " << foundedFiles.size() << " shaders in folder " << shaderPaths[i] << std::endl;
                pathPrefix = shaderPaths[i];
                break;
            }else{
                std::cout << "Can't found shaders in " << shaderPaths[i] << " folder." << std::endl;
                continue;
            }
        #endif
    }
    return foundedFiles;
}

void ShaderLoader::loadShaders(){
    std::string readedShaderCode;
    std::ifstream shaderFile;

    std::vector<std::string> foundedFiles = getFilenames();

    int success;
    char infoLog[512];

    programm_ID = glCreateProgram();

    for(uint32_t i = 0; i < foundedFiles.size(); i++){
        GLuint temporaryShaderId;
        bool okFlag = false;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            shaderFile.open(pathPrefix+foundedFiles[i]);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();	
            // close file handlers
            shaderFile.close();
            // convert stream into string
            readedShaderCode   = shaderStream.str();	
        }catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* shaderCode = readedShaderCode.c_str();

        if(foundedFiles[i].find("vertex") != std::string::npos || foundedFiles[i].find(".vs") != std::string::npos){
            temporaryShaderId = glCreateShader(GL_VERTEX_SHADER);
            std::cout << "Compiling vertex shader..." << std::endl;
            okFlag = true;
        }else if(foundedFiles[i].find("fragment") != std::string::npos || foundedFiles[i].find(".fs") != std::string::npos){
            temporaryShaderId = glCreateShader(GL_FRAGMENT_SHADER);
            std::cout << "Compiling fragment shader..." << std::endl;
            okFlag = true;
        }else{
            std::cout << "Error. Unknown shader " << pathPrefix+foundedFiles[i] << std::endl;
        }

        if(okFlag){
            glShaderSource(temporaryShaderId, 1, &shaderCode, NULL);
            glCompileShader(temporaryShaderId);
            glGetShaderiv(temporaryShaderId, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(temporaryShaderId, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }else{
                glAttachShader(programm_ID, temporaryShaderId);
                compiledShaders.push_back(temporaryShaderId);
            }
        }
    }

    std::cout << "Shaders compiled: " << compiledShaders.size() << std::endl;
    glLinkProgram(programm_ID);

    glGetProgramiv(programm_ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(programm_ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    for(uint32_t i = 0; i < compiledShaders.size(); i++)
        glDeleteShader(compiledShaders[i]);
}