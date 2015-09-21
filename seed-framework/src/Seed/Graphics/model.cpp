#include <Seed/Graphics/model.hpp>

Model::~Model()
{
	//free VBO and VAO
	glDeleteBuffers(1, &VBO_vertices);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &VBO_tangents);
	glDeleteBuffers(1, &VBO_coordText);
	glDeleteBuffers(1, &VBO_faces);
	glDeleteVertexArrays(1, &VAO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
Model::Model(const aiMesh *mesh)
{
	//matrix of transformation of the model
	//glm::mat4 M = glm::mat4(1.0);
	this->geometry = new Geometry(mesh);
	int nbVertices = this->geometry->getNumVertices();
	int nbFaces = this->geometry->getNumFaces();
	this->name = mesh->mName.C_Str();

	if (geometry->hasFaces())
	{
		glGenBuffers(1, &VBO_faces);
		//bind buffer for faces
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_faces);
		unsigned int sizeFaces = nbFaces * this->geometry->getNumIndices() * sizeof(GLuint);
		//allocate memory for the faces
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeFaces, 0, GL_STATIC_DRAW);
		//std::cout << this->geometry->getFaces()[0] << std::endl;
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
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, GL_STATIC_DRAW);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, &(this->geometry->getVertices()->at(0)));
	}
		
	if (this->geometry->hasNormals())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_normals);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, GL_STATIC_DRAW);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, &(this->geometry->getNormals()->at(0)));
	}
	if (this->geometry->hasTangents())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_tangents);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_tangents);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, GL_STATIC_DRAW);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, &(this->geometry->getTangents()->at(0)));
	}
	if (this->geometry->hasTexCoords())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &VBO_coordText);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, VBO_coordText);
		glBufferData(GL_ARRAY_BUFFER, nbVertices * sizeof(glm::vec2), 0, GL_STATIC_DRAW);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, nbVertices * sizeof(glm::vec2), &(this->geometry->getTexCoords()->at(0)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//generate a VAO buffer
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	//specify to opengl where is the data
	//the position
	if (this->geometry->hasVertices())
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
		glEnableVertexAttribArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	if (this->geometry->hasNormals())
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
		glEnableVertexAttribArray(1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tangents);
	if (this->geometry->hasTangents())
	{
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
		glEnableVertexAttribArray(2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO_coordText);
	if (this->geometry->hasTexCoords())
	{
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glEnableVertexAttribArray(3);
	}
	//free VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//free VAO
	glBindVertexArray(0);
	printErrorOpenGL();

}

void Model::render()
{
	if (this->geometry->hasFaces())
	{
		unsigned int sizeFaces = this->geometry->getNumFaces() * this->geometry->getNumIndices();
		//bind VAO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_faces);
		glDrawRangeElements(GL_TRIANGLES, 0, sizeFaces, sizeFaces, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, this->m_faces.size(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, this->m_vertices.size());
		//free VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Model::afficher()
{
	//std::cout << "Model : \n nb de vertices : " << this << "\n nb de normales : " << this->m_normals.size() << "\n nb de tangents " << this->m_tangents.size() << "\n nb de text coord " << this->m_textCoords.size() << "\n nb faces " << this->m_faces.size() << std::endl;
}



