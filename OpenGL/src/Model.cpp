#include "Model.h"
#include "stb_image.h"

void Model::prepareModel(const std::string& objFilepath, const char* textureFilePath)
{
    loadOBJ(objFilepath);
    loadTexture(textureFilePath);
    createBuffers();
}

void  Model::loadOBJ(const std::string& filepath)
{
    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLuint> vertexIndices, normalIndices, texcoordIndices;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            GLfloat x, y, z;
            iss >> x >> y >> z;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
        else if (prefix == "vn") {
            GLfloat nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
        else if (prefix == "vt") {
            GLfloat u, v;
            iss >> u >> v;
            texcoords.push_back(u);
            texcoords.push_back(v);
        }
        else if (prefix == "f") {
            GLuint vIndex[3], nIndex[3], tIndex[3];
            char slash;

            for (int i = 0; i < 3; ++i) {
                iss >> vIndex[i] >> slash >> tIndex[i] >> slash >> nIndex[i];
                vertexIndices.push_back(vIndex[i] - 1);
                texcoordIndices.push_back(tIndex[i] - 1);
                normalIndices.push_back(nIndex[i] - 1);
            }
        }
    }

    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        Vertex vertex;
        vertex.position[0] = positions[3 * vertexIndices[i]];
        vertex.position[1] = positions[3 * vertexIndices[i] + 1];
        vertex.position[2] = positions[3 * vertexIndices[i] + 2];

        vertex.normal[0] = normals[3 * normalIndices[i]];
        vertex.normal[1] = normals[3 * normalIndices[i] + 1];
        vertex.normal[2] = normals[3 * normalIndices[i] + 2];

        vertex.texcoord[0] = texcoords[2 * texcoordIndices[i]];
        vertex.texcoord[1] = texcoords[2 * texcoordIndices[i] + 1];

        mesh.vertices.push_back(vertex);
        mesh.indices.push_back(mesh.indices.size());
    }
}

void Model::loadTexture(const char* filepath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Model::createBuffers()
{
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texcoord)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Model::render()
{
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Model::clean()
{
    mesh.vertices.clear();
    mesh.indices.clear();
}

