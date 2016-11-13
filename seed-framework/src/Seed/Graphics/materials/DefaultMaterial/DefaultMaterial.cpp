//SEED INCLUDES
#include <Seed/Graphics/materials/DefaultMaterial/DefaultMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>

DefaultMaterial::DefaultMaterial(const aiMaterial *material, std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(material, sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "DefaultMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}
DefaultMaterial::DefaultMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "DefaultMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}

void DefaultMaterial::init()
{
	GLuint programID = m_shader->getID();

	m_M = glm::mat4(1.0);

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	m_MID = glGetUniformLocation(programID, "M");
	m_NMID = glGetUniformLocation(programID, "Normal_Matrix");
	m_matID = glGetUniformLocation(programID, "mat");
	m_block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	m_block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	m_block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	m_block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	m_block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
	m_NMACTIVEID = glGetUniformLocation(programID, "NormalMapActive");
	m_NMVIEWID = glGetUniformLocation(programID, "NormalMapView");
	m_SMACTIVEID = glGetUniformLocation(programID, "SpecularMapActive");
	m_SMVIEWID = glGetUniformLocation(programID, "SpecularMapView");
	m_PMVIEWID = glGetUniformLocation(programID, "ParallaxMapView");
	m_PMACTIVEID = glGetUniformLocation(programID, "ParallaxMapActive");
	m_BIASPARALLAXMAPID = glGetUniformLocation(programID, "biasParallax");
}

DefaultMaterial::~DefaultMaterial()
{
}

void DefaultMaterial::render(Model *model)
{
	std::cout << "render DefaultMaterial" << std::endl;
	if (m_shader->useProgram())
	{
		//UNIFORMS
		m_Normal_Matrix = glm::transpose(glm::inverse(m_M));
		//set the uniform variable MVP
		glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M[0][0]);
		glUniformMatrix4fv(m_NMID, 1, GL_FALSE, &m_Normal_Matrix[0][0]);
		glUniform1i(m_NMACTIVEID, Scene::normalMapActive);
		glUniform1i(m_NMVIEWID, Scene::normalMapView);
		glUniform1i(m_SMACTIVEID, Scene::specularMapActive);
		glUniform1i(m_SMVIEWID, Scene::specularMapView);
		glUniform1i(m_PMVIEWID, Scene::parallaxMapView);
		glUniform1i(m_PMACTIVEID, Scene::parallaxMapActive);
		glUniform1f(m_BIASPARALLAXMAPID, Scene::biasParallax);
		glUniform2f(m_matID, m_mat.Ks, m_mat.Kr);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		std::cout << "Active textures" << std::endl;
		//TEXTURES
		activeTextures(m_shader->getID());
		std::cout << "Fin Active textures" << std::endl;
		std::cout << "Buffer camera" << std::endl;

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
		std::cout << "Fin Buffer camera" << std::endl;
		std::cout << "Render Model" << std::endl;
		//RENDER
		//render model
		model->render();

		std::cout << "Fin Render Model" << std::endl;
		std::cout << "Release Texture" << std::endl;
		//RELEASE
		releaseTextures();
		std::cout << "Fin Release Texture" << std::endl;
		//glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
	}
	std::cout << "Fin render DefaultMaterial" << std::endl;

}

void DefaultMaterial::translateModel(glm::vec3 T)
{
	m_M = translate(m_M, T);
}

void DefaultMaterial::scaleModel(glm::vec3 T)
{
	m_M = scale(m_M, T);
}

void DefaultMaterial::rotateModel(glm::vec3 T)
{
	m_M = rotate(m_M, T);
}