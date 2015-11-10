#include <Seed/Graphics/model/instancedModel.hpp>

InstancedModel::~InstancedModel()
{
	//free VBO and VAO
	if (VBO_vertices)
		glDeleteBuffers(1, &VBO_vertices);
	if (VBO_normals)
		glDeleteBuffers(1, &VBO_normals);
	if (VBO_tangents)
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
InstancedModel::InstancedModel(const aiMesh *mesh, std::string p) : Model(mesh, GL_STREAM_DRAW, p)
{
}	
InstancedModel::InstancedModel(Geometry *g) : Model(g, GL_STREAM_DRAW)
{
}

void InstancedModel::render(int nb)
{
	//bind VAO
	glBindVertexArray(this->VAO);
	if (this->geometry->hasFaces())
	{
		unsigned int sizeFaces = this->geometry->getNumFaces() * this->geometry->getNumIndicesPerFace();
		if (this->geometry->getNumIndicesPerFace() == 3)
			glDrawElementsInstanced(GL_TRIANGLES, sizeFaces, GL_UNSIGNED_INT, 0, nb);
		else
			glDrawElementsInstanced(GL_QUADS, sizeFaces, GL_UNSIGNED_INT, 0, nb);
	}
	else
	{
		if (this->geometry->getNumIndicesPerFace() == 3)
			glDrawArraysInstanced(GL_TRIANGLES, 0, this->geometry->getNumVertices(), nb);
		else
			glDrawArraysInstanced(GL_QUADS, 0, this->geometry->getNumVertices(), nb);
	}
	//free VAO
	glBindVertexArray(0);
}