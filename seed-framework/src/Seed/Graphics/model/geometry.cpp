#include <Seed/Graphics/model/geometry.hpp>

Geometry::Geometry(const aiMesh *mesh)
{
	unsigned int i = 0;

	//Number of vertices and faces
	this->m_numVertices = mesh->mNumVertices;
	this->m_numFaces = mesh->mNumFaces;
	this->m_maxIndicesByFace = mesh->mFaces->mNumIndices;

	//copy datas
	try
	{
		for (i = 0; i < this->m_numVertices; i++)
		{
			if (mesh->HasPositions())
			{
				//copy vertices
				this->m_vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			}
			if (mesh->HasTangentsAndBitangents())
			{
				//copy tangents
				this->m_tangents.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
			}
			if (mesh->HasTextureCoords(0))
			{
				//copy texture coordinates
				this->m_textCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			}
			if (mesh->HasNormals())
			{
				//copy normals
				this->m_normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			}
		}
		if (mesh->HasFaces())
		{
			//we take each indice of each face
			for (i = 0; i < mesh->mNumFaces; i++)
			{
				for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				{
					//copy faces
					this->m_faces.push_back(mesh->mFaces[i].mIndices[j]);
				}
			}
		}
	}
	catch (std::out_of_range &)
	{
		std::cout << "ERROR: Out of range of array in file model.cpp" << std::endl;
		return;
	}
	catch (std::exception &)
	{
		std::cout << "ERROR: error in model.cpp file" << std::endl;
		return;
	}
}

Geometry::Geometry()
{

}

Geometry::~Geometry()
{}

bool Geometry::hasVertices()
{
	if (this->m_vertices.size())
		return true;
	return false;
}

bool Geometry::hasNormals()
{
	if (this->m_normals.size())
		return true;
	return false;
}

bool Geometry::hasTangents()
{
	if (this->m_tangents.size())
		return true;
	return false;
}

bool Geometry::hasTexCoords()
{
	if (this->m_textCoords.size())
		return true;
	return false;
}

bool Geometry::hasFaces()
{
	if (this->m_faces.size())
	{
		return true;
	}
	return false;
}

unsigned int Geometry::getNumVertices()
{
	return this->m_numVertices;
}

unsigned int Geometry::getNumFaces()
{
	return this->m_numFaces;
}

unsigned int Geometry::getNumIndicesPerFace()
{
	return this->m_maxIndicesByFace;
}

std::vector<glm::vec3> * Geometry::getVertices()
{
	return &(this->m_vertices);
}

std::vector<glm::vec3> * Geometry::getNormals()
{
	return &(this->m_normals);
}

std::vector<glm::vec3> * Geometry::getTangents()
{
	return &(this->m_tangents);
}

std::vector<glm::vec2> * Geometry::getTexCoords()
{
	return &(this->m_textCoords);
}

std::vector<GLuint> * Geometry::getFaces()
{
	return &(this->m_faces);
}

void Geometry::setVertices(GLfloat *vert, int nb, int nbVerticePerFace) 
{
	for (int i = 0; i < nb/nbVerticePerFace; i++)
	{
			this->m_vertices.push_back(glm::vec3(vert[i * 3], vert[i * 3 + 1], vert [i * 3 + 2]));
	}
	this->m_numVertices = nb/nbVerticePerFace;
	this->m_maxIndicesByFace = nbVerticePerFace;
}

bool Geometry::setFaces(GLuint type, GLuint *faces, int nb)
{
	for (int i = 0; i < nb; i++)
	{ 
		this->m_faces.push_back(faces[i]);
	}
	switch (type)
	{
		case GL_TRIANGLES:
			this->m_numFaces = nb / 3;
			this->m_maxIndicesByFace = 3;
			return true;
		case GL_QUADS:
			this->m_numFaces = nb / 4;
			this->m_maxIndicesByFace = 4;
			return true;
		default:
			std::cout << "Error the type of face is nor valid" << std::endl;
			return false;
	}
}

void Geometry::setNormals(GLfloat *norm, int nb)
{
	for (int i = 0; i < nb/3; i++)
	{
		this->m_normals.push_back(glm::vec3(norm[i * 3], norm[i * 3 + 1], norm[i * 3 + 2]));
	}
}

void Geometry::setTangents(GLfloat *ta, int nb)
{
	for (int i = 0; i < nb/3; i++)
	{
		this->m_tangents.push_back(glm::vec3(ta[i * 3], ta[i * 3 + 1], ta[i * 3 + 2]));
	}
}

void Geometry::setTexCoords(GLfloat *tc, int nb)
{
	for (int i = 0; i < nb/2; i++)
	{
		this->m_textCoords.push_back(glm::vec2(tc[i * 2], tc[i * 2 + 1]));
	}
}