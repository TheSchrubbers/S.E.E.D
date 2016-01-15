//SEED INCLUDES
#include <Seed/Graphics/materials/ImplicitMaterial/ImplicitMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/implicitSurface/iSphere.hpp>
#include <Seed/Graphics/engine/shader.hpp>

ImplicitMaterial::ImplicitMaterial(Scene *sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac, pathToImplicitMaterial + "Shaders")
{
	this->init();
}

void ImplicitMaterial::init()
{
	GLuint programID = this->shader->getID();

	this->complight.ambiant = 0.1;
	this->complight.diffuse = 0.8;
	this->complight.specular = 0.1;

	this->M = glm::mat4(1.0);
	this->M = glm::translate(this->M, glm::vec3(1.0, 0.0, 0.0));

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->MID = glGetUniformLocation(programID, "M");
	this->NMID = glGetUniformLocation(programID, "Normal_Matrix");
	this->matID = glGetUniformLocation(programID, "mat");
	this->complight.ambiantID = glGetUniformLocation(programID, "light.ambiant");
	this->complight.diffuseID = glGetUniformLocation(programID, "light.diffuse");
	this->complight.specularID = glGetUniformLocation(programID, "light.specular");
	this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
	this->NMACTIVEID = glGetUniformLocation(programID, "NormalMappingActive");
	this->SMACTIVEID = glGetUniformLocation(programID, "SpecularMappingActive");
	this->SMVIEWID = glGetUniformLocation(programID, "SpecularMappingView");

	//get the discret surface of the implicit surface ->marching cubes method
	ImplicitSphere *isphere = new ImplicitSphere(1.0, glm::vec3(0.0));
	MarchingCubes *MC = new MarchingCubes(isphere, 4);
	this->pts = MC->construcMesh();
	std::vector<glm::vec3> p, n;
	for (int i = 0; i < pts.size(); i++)
	{
		//we get the vertices points of the implicit mesh
		p.push_back(pts[i]->p);
		n.push_back(pts[i]->n);
	}

	//put points like a suit of triangles in a VBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBOVertices);
	glGenBuffers(1, &this->VBONormals);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, p.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pts.size() * sizeof(glm::vec3), &p[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBONormals);
	glBufferData(GL_ARRAY_BUFFER, n.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, n.size() * sizeof(glm::vec3), &n[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ImplicitMaterial::~ImplicitMaterial(){}

void ImplicitMaterial::render()
{
	if (this->activateShader())
	{
		//UNIFORMS
		//this->M *= glm::rotate(0.005f, glm::vec3(0, 1, 0));
		this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
		glUniform1fv(this->complight.ambiantID, 1, &(complight.ambiant));
		glUniform1fv(this->complight.diffuseID, 1, &(complight.diffuse));
		glUniform1fv(this->complight.specularID, 1, &(complight.specular));
		glUniform1i(this->NMACTIVEID, false);
		glUniform1i(this->SMACTIVEID, false);
		glUniform1i(this->SMVIEWID, false);
		glUniform2f(this->matID, this->mat.Ks, this->mat.Kr);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//TEXTURES
		this->activeTextures(this->shader->getID());

		//BUFFERS
		for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(this->shader->getID(), this->block_index_lights[i], i);
		}
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamUBO()->getID());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 4);
		glBindVertexArray(this->VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, this->pts.size());
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//RELEASE
		this->releaseTextures();
	}
}