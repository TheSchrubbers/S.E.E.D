#include <Seed/Graphics/geometry.hpp>

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

unsigned int Geometry::getNumIndices()
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