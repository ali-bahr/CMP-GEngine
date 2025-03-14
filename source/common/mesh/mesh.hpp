#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
        void setVertexAttribPointer(GLuint index, GLint size, GLenum type, size_t offset,GLboolean normalized = GL_FALSE){
            glVertexAttribPointer(index, size, type, normalized, sizeof(Vertex), (void*)offset);
            glEnableVertexAttribArray(index);
        }
        void initializeBuffers(){
            glGenBuffers(1,&VBO);
            glGenBuffers(1,&EBO);
            glGenVertexArrays(1,&VAO);
        }

        void setDataInBuffer(GLenum type, GLuint buffer, GLsizeiptr size, const void* data){
            glBindBuffer(type,buffer);
            glBufferData(type,size,data,GL_STATIC_DRAW);
        }

    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            elementCount = static_cast<GLsizei>(elements.size());

            //First We create our buffers (Vertex Buffer, Element Buffer and Vertex Array to describe how to read from the vertex buffer
            initializeBuffers();
            // We Need to bind with these buffers in order to define the needed things
            glBindVertexArray(VAO);

            setDataInBuffer(GL_ARRAY_BUFFER,VBO, vertices.size() * sizeof(Vertex),vertices.data());

            setDataInBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO, elements.size() * sizeof(unsigned int),elements.data());
            // Define First Attribute: position
            setVertexAttribPointer(ATTRIB_LOC_POSITION,3,GL_FLOAT,offsetof(Vertex,position));

            //Define Second Attribute: color
            setVertexAttribPointer(ATTRIB_LOC_COLOR,4,GL_UNSIGNED_BYTE,offsetof(Vertex,color),GL_TRUE);

            //Define Third Attribute: Texture Coordinates
            setVertexAttribPointer(ATTRIB_LOC_TEXCOORD,2,GL_FLOAT,offsetof(Vertex,tex_coord));

            //Define Last Attribute: Normal
            setVertexAttribPointer(ATTRIB_LOC_NORMAL,3,GL_FLOAT,offsetof(Vertex,normal));

            glBindVertexArray(0);
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES,elementCount,GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}