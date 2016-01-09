#include <Seed/Graphics/model/model.hpp>

Model::~Model()
{
	//free VBO and VAO
	if (VBO_vertices)
		glDeleteBuffers(1, &VBO_vertices);
	if (VBO_normals)
		glDeleteBuffers(1, &VBO_normals);
	if(VBO_tangents)
		glDeleteBuffers(1, &VBO_tangents);
	if (VBO_coordText)
		glDeleteBuffers(1, &VBO_coordText);
	if (VBO_faces)
		glDeleteBuffers(1, &VBO_faces);
	if (VAO)
		glDeleteVertexArrays(1, &VAO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	delete this->geometry;
}
Model::Model(const aiMesh *mesh, GLuint frequency, std::string p)
{
	this->geometry = new Geometry(mesh);
	this->name = mesh->mName.C_Str();
	this->pathMesh = p;
	this->loadInGPU(frequency);
}

Model::Model(Geometry *g, GLuint frequency)
{
	this->geometry = g;
	this->name = "default";
	this->loadInGPU(frequency);
}

void Model::render()
{
	//bind VAO
	glBindVertexArray(this->VAO);
	if (this->geometry->hasFaces())
	{
		unsigned int sizeFaces = this->geometry->getNumFaces() * this->geometry->getNumIndicesPerFace();
		if (this->geometry->getNumIndicesPerFace() == 3)
		{
			glDrawRangeElements(GL_TRIANGLES, 0, sizeFaces, sizeFaces, GL_UNSIGNED_INT, 0);
		}
		else
			glDrawRangeElements(GL_QUADS, 0, sizeFaces, sizeFaces, GL_UNSIGNED_INT, 0);
	}
	else
	{
		if (this->geometry->getNumIndicesPerFace() == 3)
			glDrawArrays(GL_TRIANGLES, 0, this->geometry->getNumVertices());
		else
			glDrawArrays(GL_QUADS, 0, this->geometry->getNumVertices());
	}

	//free VAO
	glBindVertexArray(0);
}

void Model::render(int nb)
{
	//bind VAO
	glBindVertexArray(this->VAO);
	//if the model has faces
	if (this->geometry->hasFaces())
	{
		//process the size of a face
		unsigned int sizeFaces = this->geometry->getNumFaces() * this->geometry->getNumIndicesPerFace();
		//if the face has 3 vertices
		if (this->geometry->getNumIndicesPerFace() == 3)
			glDrawElementsInstanced(GL_TRIANGLES, sizeFaces, GL_UNSIGNED_INT, 0, nb);
		//if the face has 4 vertices
		else
			glDrawElementsInstanced(GL_QUADS, sizeFaces, GL_UNSIGNED_INT, 0, nb);
	}
	//if the model has no face
	else
	{
		//if the face has 3 vertices
		if (this->geometry->getNumIndicesPerFace() == 3)
			glDrawArraysInstanced(GL_TRIANGLES, 0, this->geometry->getNumVertices(), nb);
		//if the face has 4 vertices
		else
			glDrawArraysInstanced(GL_QUADS, 0, this->geometry->getNumVertices(), nb);
	}
	//free VAO
	glBindVertexArray(0);
}



std::string Model::getPathName()
{
	return this->pathMesh;
}

void Model::afficher()
{
	//std::cout << "Model : \n nb de vertices : " << this << "\n nb de normales : " << this->m_normals.size() << "\n nb de tangents " << this->m_tangents.size() << "\n nb de text coord " << this->m_textCoords.size() << "\n nb faces " << this->m_faces.size() << std::endl;
}

void Model::loadInGPU(GLuint frequency)
{
	int numVerticesPerFace = this->geometry->getNumIndicesPerFace();
	int nbVertices = this->geometry->getNumVertices();
	int nbFaces = this->geometry->getNumFaces();
	unsigned int sizeFaces = nbFaces * numVerticesPerFace * sizeof(GLuint);
	int strideVert;
	if (numVerticesPerFace == 3)
	{
		strideVert = sizeof(glm::vec3);
	}
	else
	{
		strideVert = sizeof(glm::vec4);
	}

	//generate a VAO buffer
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);

	if (geometry->hasFaces())
	{
		glGenBuffers(1, &VBO_faces);
		//bind buffer for faces
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_faces);
		//allocate memory for the faces
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeFaces, 0, frequency);
		//put the faces in the buffer
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeFaces, &(this->geometry->getFaces()->at(0)));
	}

	//taille vertices in bytes
	int verticesBytes = nbVertices * sizeof(glm::vec3);

	if (this->geometry->hasVertices())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_vertices);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, &(this->geometry->getVertices()->at(0)));
	}
	if (this->geometry->hasNormals())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_normals);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, &(this->geometry->getNormals()->at(0)));
	}
	if (this->geometry->hasTangents())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_tangents);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_tangents);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, &(this->geometry->getTangents()->at(0)));
	}
	if (this->geometry->hasTexCoords())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_coordText);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_coordText);
		glBufferData(GL_ARRAY_BUFFER, nbVertices * sizeof(glm::vec2), 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, nbVertices * sizeof(glm::vec2), &(this->geometry->getTexCoords()->at(0)));
	}


	//specify to opengl where is the data
	//the position
	if (this->geometry->hasVertices())
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideVert, 0);
		glEnableVertexAttribArray(0);
	}
	if (this->geometry->hasNormals())
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideVert, 0);
		glEnableVertexAttribArray(1);
	}
	if (this->geometry->hasTangents())
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_tangents);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, strideVert, 0);
		glEnableVertexAttribArray(2);
	}
	
	if (this->geometry->hasTexCoords())
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_coordText);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glEnableVertexAttribArray(3);
	}
	//free VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//free VAO
	glBindVertexArray(0);
	printErrorOpenGL();
}


