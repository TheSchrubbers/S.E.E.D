//SEED INCLUDES
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/model/geometry.hpp>

InstancedModel::~InstancedModel()
{
	//free VBO and VAO
	if (m_VBO_vertices)
		glDeleteBuffers(1, &m_VBO_vertices);
	if (m_VBO_normals)
		glDeleteBuffers(1, &m_VBO_normals);
	if (m_VBO_tangents)
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
}
InstancedModel::InstancedModel(const aiMesh *mesh, std::string p) : Model(mesh, GL_STREAM_DRAW, p)
{
}	
InstancedModel::InstancedModel(Geometry *g) : Model(g, GL_STREAM_DRAW)
{
}

void InstancedModel::render(int nb)
{
	//bind VAO
	glBindVertexArray(m_VAO);
	if (m_geometry->hasFaces())
	{
		unsigned int sizeFaces = m_geometry->getNumFaces() * m_geometry->getNumIndicesPerFace();
		if (m_geometry->getNumIndicesPerFace() == 3)
			glDrawElementsInstanced(GL_TRIANGLES, sizeFaces, GL_UNSIGNED_INT, 0, nb);
		else
			glDrawElementsInstanced(GL_QUADS, sizeFaces, GL_UNSIGNED_INT, 0, nb);
	}
	else
	{
		if (m_geometry->getNumIndicesPerFace() == 3)
			glDrawArraysInstanced(GL_TRIANGLES, 0, m_geometry->getNumVertices(), nb);
		else
			glDrawArraysInstanced(GL_QUADS, 0, m_geometry->getNumVertices(), nb);
	}
	//free VAO
	glBindVertexArray(0);
}