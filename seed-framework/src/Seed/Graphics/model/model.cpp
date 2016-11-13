//SEED INCLUDES
#include <Seed/Graphics/model/model.hpp>
#include <Seed/Graphics/model/geometry.hpp>

Model::~Model()
{
	//free VBO and VAO
	if (m_VBO_vertices)
		glDeleteBuffers(1, &m_VBO_vertices);
	if (m_VBO_normals)
		glDeleteBuffers(1, &m_VBO_normals);
	if(m_VBO_tangents)
		glDeleteBuffers(1, &m_VBO_tangents);
	if (m_VBO_coordText)
		glDeleteBuffers(1, &m_VBO_coordText);
	if (m_VBO_faces)
		glDeleteBuffers(1, &m_VBO_faces);
	if (m_VAO)
		glDeleteVertexArrays(1, &m_VAO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	delete m_geometry;
}
Model::Model(const aiMesh *mesh, GLuint frequency, std::string p)
{
    initializeOpenGLFunctions();

	m_geometry = new Geometry(mesh);
	m_name = mesh->mName.C_Str();
	m_pathMesh = p;
	loadInGPU(frequency);
}

Model::Model(Geometry *g, GLuint frequency)
{
    initializeOpenGLFunctions();
	
	m_geometry = g;
	m_name = "default";
	loadInGPU(frequency);
}

void Model::render()
{
	//std::cout << "render model" << std::endl;
	//bind VAO
	glBindVertexArray(m_VAO);
	if (m_geometry->hasFaces())
	{
		unsigned int sizeFaces = m_geometry->getNumFaces() * m_geometry->getNumIndicesPerFace();
		if (m_geometry->getNumIndicesPerFace() == 3)
		{
			//std::cout << "glDrawRangeElements model" << std::endl;
			//printValue("nb faces", sizeFaces);
			//printValue("size VBO", )
			glDrawRangeElements(GL_TRIANGLES, 0, sizeFaces, sizeFaces, GL_UNSIGNED_INT, 0);
			//std::cout << "Fin glDrawRangeElements model" << std::endl;
		}
		else
			glDrawRangeElements(GL_QUADS, 0, sizeFaces, sizeFaces, GL_UNSIGNED_INT, 0);
	}
	else
	{
		if (m_geometry->getNumIndicesPerFace() == 3)
			glDrawArrays(GL_TRIANGLES, 0, m_geometry->getNumVertices());
		else
			glDrawArrays(GL_QUADS, 0, m_geometry->getNumVertices());
	}
	//std::cout << "Fin render model" << std::endl;


	//free VAO
	glBindVertexArray(0);
}

void Model::render(int nb)
{
	//bind VAO
	glBindVertexArray(m_VAO);
	//if the model has faces
	if (m_geometry->hasFaces())
	{
		//process the size of a face
		unsigned int sizeFaces = m_geometry->getNumFaces() * m_geometry->getNumIndicesPerFace();
		//if the face has 3 vertices
		if (m_geometry->getNumIndicesPerFace() == 3)
			glDrawElementsInstanced(GL_TRIANGLES, sizeFaces, GL_UNSIGNED_INT, 0, nb);
		//if the face has 4 vertices
		else
			glDrawElementsInstanced(GL_QUADS, sizeFaces, GL_UNSIGNED_INT, 0, nb);
	}
	//if the model has no face
	else
	{
		//if the face has 3 vertices
		if (m_geometry->getNumIndicesPerFace() == 3)
			glDrawArraysInstanced(GL_TRIANGLES, 0, m_geometry->getNumVertices(), nb);
		//if the face has 4 vertices
		else
			glDrawArraysInstanced(GL_QUADS, 0, m_geometry->getNumVertices(), nb);
	}
	//free VAO
	glBindVertexArray(0);
}



std::string Model::getPathName()
{
	return m_pathMesh;
}

void Model::afficher()
{
	std::cout << "Model : \n pathname : " << m_pathMesh << " \n nb de vertices : " << m_geometry->getNumVertices() << "\n nb faces " << m_geometry->getNumFaces() << std::endl;
}

void Model::loadInGPU(GLuint frequency)
{
	int numVerticesPerFace = m_geometry->getNumIndicesPerFace();
	int nbVertices = m_geometry->getNumVertices();
	int nbFaces = m_geometry->getNumFaces();
	unsigned int sizeFaces = nbFaces * numVerticesPerFace * sizeof(GLuint);
	int strideVert;
	if (numVerticesPerFace == 3)
		strideVert = sizeof(glm::vec3);
	else
		strideVert = sizeof(glm::vec4);

	//generate a VAO buffer
	glGenVertexArrays(1, &m_VAO);
	//bind VAO
	glBindVertexArray(m_VAO);

	if (m_geometry->hasFaces())
	{
		glGenBuffers(1, &m_VBO_faces);
		//bind buffer for faces
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_faces);
		//allocate memory for the faces
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeFaces, 0, frequency);
		//put the faces in the buffer
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeFaces, m_geometry->getFaces());
	}

	//taille vertices in bytes
	int verticesBytes = nbVertices * sizeof(glm::vec3);

	if (m_geometry->hasVertices())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &m_VBO_vertices);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_vertices);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, m_geometry->getVertices());
	}
	if (m_geometry->hasNormals())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &m_VBO_normals);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, m_geometry->getNormals());
	}
	if (m_geometry->hasTangents())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &m_VBO_tangents);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_tangents);
		glBufferData(GL_ARRAY_BUFFER, verticesBytes, 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, m_geometry->getTangents());
	}
	if (m_geometry->hasTexCoords())
	{
		//Put the datas' mesh in a VBOBuffer
		glGenBuffers(1, &m_VBO_coordText);
		//bind buffer VBO for datas
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_coordText);
		glBufferData(GL_ARRAY_BUFFER, nbVertices * sizeof(glm::vec2), 0, frequency);
		//copy data in GPU memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, nbVertices * sizeof(glm::vec2), m_geometry->getTexCoords());
	}


	//specify to opengl where is the data
	//the position
	if (m_geometry->hasVertices())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_vertices);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideVert, 0);
		glEnableVertexAttribArray(0);
	}
	if (m_geometry->hasNormals())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_normals);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideVert, 0);
		glEnableVertexAttribArray(1);
	}
	if (m_geometry->hasTangents())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_tangents);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, strideVert, 0);
		glEnableVertexAttribArray(2);
	}
	
	if (m_geometry->hasTexCoords())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO_coordText);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glEnableVertexAttribArray(3);
	}
	//free VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//free VAO
	glBindVertexArray(0);
	printErrorOpenGL();
}


