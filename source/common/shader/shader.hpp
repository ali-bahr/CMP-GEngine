#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            this->program = glCreateProgram();
        }
        ~ShaderProgram(){
            glDeleteProgram(this->program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(this->program);
        }

        GLuint getUniformLocation(const std::string &name) {
            return glGetUniformLocation(this->program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            glUniform1f(this->getUniformLocation(uniform),value);
        }

        void set(const std::string &uniform, GLuint value) {
            glUniform1ui(this->getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            glUniform1i(this->getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            glUniform2fv(this->getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec3 value) {
            glUniform3fv(this->getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec4 value) {
            glUniform4fv(this->getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            glUniformMatrix4fv(this->getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        //Question: Why do we delete the copy constructor and assignment operator?
        //We delete the copy constructor and assignment operator
        //because we don't want to have multiple instances of the same shader program.
    };

}

#endif