#include "ShaderLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
    #include "windows.h"
#else
    #include <dirent.h>
#endif

/*****************************************************************************************/
ShaderLoader::ShaderLoader(){
    addShaderFolder("shaders\\");     //default path
}

/*****************************************************************************************/
ShaderLoader::ShaderLoader(std::string shaderFolder){
    addShaderFolder("shaders\\");
    addShaderFolder(shaderFolder);
    loadShaders();
}

/*****************************************************************************************/
ShaderLoader::~ShaderLoader(){
    compiledShaders.clear();
    shaderPaths.clear();
}

/*****************************************************************************************/
std::vector<std::string> ShaderLoader::getFilenames(){
    std::vector<std::string> foundedFiles;

    for(uint32_t i = 0; i < shaderPaths.size(); i++){
        glLogTracer->SendLog("Looking for shaders in " + shaderPaths[i] + " folder");
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
                        glLogTracer->SendLog("Founded file " + foundedFiles.back());
                    }
                } while (FindNextFile(hFind, &data) != 0);
                FindClose(hFind);
                if(foundedFiles.size() > 0) {
                    glLogTracer->SendLog("Founded " + std::to_string(foundedFiles.size()) + " shaders in dirrectory " + shaderPaths[i]);
                    pathPrefix = shaderPaths[i];
                    break;
                }else{
                    glLogTracer->SendLog("Can't found shaders in dirrectory " + shaderPaths[i]);
                    continue;
                }
            }
        #else
            DIR* dirp = opendir(shaderPaths[i].c_str());
            struct dirent * dp;
            while ((dp = readdir(dirp)) != NULL) {
                std::string temp = dp->d_name;
                if(temp.find(".glsl") != std::string::npos || temp.find(".vs") != std::string::npos || temp.find(".fs") != std::string::npos) {
                    foundedFiles.push_back(temp);
                    glLogTracer->SendLog("Founded file " + foundedFiles.back());
                }
            }
            closedir(dirp);
            if(foundedFiles.size() > 0) {
                glLogTracer->SendLog("Founded " + std::to_string(foundedFiles.size()) + " shaders in dirrectory " + shaderPaths[i]);
                pathPrefix = shaderPaths[i];
                break;
            }else{
                glLogTracer->SendLog("Can't found shaders in dirrectory " + shaderPaths[i]);
                continue;
            }
        #endif
    }
    return foundedFiles;
}

/*****************************************************************************************/
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