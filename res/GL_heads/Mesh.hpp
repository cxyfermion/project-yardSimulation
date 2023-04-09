#pragma once

#include<string>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Shader.h"
#define MAX_BONE_INFLUENCE 4
using namespace std;
using namespace glm;

//顶点数据结构体，包括顶点坐标、[颜色、]纹理坐标、光照均值
struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    //// tangent
    //glm::vec3 Tangent;
    //// bitangent
    //glm::vec3 Bitangent;
    ////bone indexes which will influence this vertex
    //int m_BoneIDs[MAX_BONE_INFLUENCE];
    ////weights from each bone
    //float m_Weights[MAX_BONE_INFLUENCE];
};

//材质包
struct Texture
{
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	//Mesh Data
	vector<Vertex> vertices;
	//indecies for EBO
	vector<unsigned int> indices;
	vector<Texture> textures;
   
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indecies, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indecies;
        this->textures = textures;

        // Now that we have all the required data, set the vertex buffers and its attribute pointers.
        this->setupMesh();
    }

    // Render the mesh
    void Draw(Shader& shader)
    {
        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        //GLuint normalNr = 1;
        //GLuint heightNr = 1;
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            // Active proper texture unit before binding
            //一次开启材质种类，材质种类编号共32种，从0x84C0开始，是连续的，所以可以加i索引
            glActiveTexture(GL_TEXTURE0 + i);
            // Retrieve texture number (the N in diffuse_textureN)
            //stringstream ss;
            string number;
            string name = this->textures[i].type;
            if (name == "texture_diffuse")
            {
                number = to_string(diffuseNr++);
                //ss << diffuseNr++; // Transfer GLuint to stream
            }
            else if (name == "texture_specular")
            {
                number = to_string(specularNr++);
                //ss << specularNr++; // Transfer GLuint to stream
            }
            //else if (name == "texture_normal")
            //    number = std::to_string(normalNr++); // transfer unsigned int to string
            //else if (name == "texture_height")
            //    number = std::to_string(heightNr++); // transfer unsigned int to string
            //number = ss.str();
            // Now set the sampler to the correct texture unit
            shader.setFloat(("material." + name + number).c_str(), (float)i);
            //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }

        // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
        //shader.setFloat("material.shininess", 16.0f);
        //glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16.0f);

        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)(this->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Always good practice to set everything back to defaults once configured.
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

private:

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals（不需要color attribute）
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        //// vertex tangent
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        //// vertex bitangent
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        //// ids
        //glEnableVertexAttribArray(5);
        //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
        //// weights
        //glEnableVertexAttribArray(6);
        //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};
