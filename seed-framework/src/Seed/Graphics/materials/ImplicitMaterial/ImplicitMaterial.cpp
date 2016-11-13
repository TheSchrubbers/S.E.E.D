//SEED INCLUDES
#include <Seed/Graphics/materials/ImplicitMaterial/ImplicitMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/implicitSurface/iSphere.hpp>
#include <Seed/Graphics/engine/shader.hpp>

ImplicitMaterial::ImplicitMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "ImplicitMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}

void ImplicitMaterial::init()
{
	GLuint programID = m_shader->getID();

	m_complight.ambiant = 0.1;
	m_complight.diffuse = 0.8;
	m_complight.specular = 0.1;

	m_M = glm::mat4(1.0);
	m_M = glm::translate(m_M, glm::vec3(1.0, 0.0, 0.0));

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	m_MID = glGetUniformLocation(programID, "M");
	m_NMID = glGetUniformLocation(programID, "Normal_Matrix");
	m_matID = glGetUniformLocation(programID, "mat");
	m_complight.ambiantID = glGetUniformLocation(programID, "light.ambiant");
	m_complight.diffuseID = glGetUniformLocation(programID, "light.diffuse");
	m_complight.specularID = glGetUniformLocation(programID, "light.specular");
	m_block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	m_block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	m_block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	m_block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	m_block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
	m_NMACTIVEID = glGetUniformLocation(programID, "NormalMappingActive");
	m_SMACTIVEID = glGetUniformLocation(programID, "SpecularMappingActive");
	m_SMVIEWID = glGetUniformLocation(programID, "SpecularMappingView");

	//get the discret surface of the implicit surface ->marching cubes method
	ImplicitSphere *isphere = new ImplicitSphere(1.0, glm::vec3(0.0));
	MarchingCubes *MC = new MarchingCubes(isphere, 4);
	m_pts = MC->construcMesh();
	std::vector<glm::vec3> p, n;
	for (int i = 0; i < m_pts.size(); i++)
	{
		//we get the vertices points of the implicit mesh
		p.push_back(m_pts[i]->p);
		n.push_back(m_pts[i]->n);
	}

	//put points like a suit of triangles in a VBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBOVertices);
	glGenBuffers(1, &m_VBONormals);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, p.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_pts.size() * sizeof(glm::vec3), &p[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
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
	if (m_shader->useProgram())
	{
		//UNIFORMS
		//m_M *= glm::rotate(0.005f, glm::vec3(0, 1, 0));
		m_Normal_Matrix = glm::transpose(glm::inverse(m_M));
		//set the uniform variable MVP
		glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M[0][0]);
		glUniformMatrix4fv(m_NMID, 1, GL_FALSE, &m_Normal_Matrix[0][0]);
		glUniform1fv(m_complight.ambiantID, 1, &(m_complight.ambiant));
		glUniform1fv(m_complight.diffuseID, 1, &(m_complight.diffuse));
		glUniform1fv(m_complight.specularID, 1, &(m_complight.specular));
		glUniform1i(m_NMACTIVEID, false);
		glUniform1i(m_SMACTIVEID, false);
		glUniform1i(m_SMVIEWID, false);
		glUniform2f(m_matID, m_mat.Ks, m_mat.Kr);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//TEXTURES
		activeTextures(m_shader->getID());

		//BUFFERS
		for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, m_scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(m_shader->getID(), m_block_index_lights[i], i);
		}
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, m_scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(m_shader->getID(), m_block_index_camera, 4);
		glBindVertexArray(m_VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, m_pts.size());
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//RELEASE
		releaseTextures();
	}
}