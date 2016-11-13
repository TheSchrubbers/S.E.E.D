//SEED INCLUDES
#include <Seed/Graphics/materials/SSAOMaterial/DeferredMaterial/DeferredMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>

DeferredMaterial::DeferredMaterial(const aiMaterial *material, std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(material, sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "DeferredMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}
DeferredMaterial::DeferredMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "DeferredMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}

void DeferredMaterial::init()
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
	m_NMACTIVEID = glGetUniformLocation(programID, "NormalMappingActive");
	m_SMACTIVEID = glGetUniformLocation(programID, "SpecularMappingActive");
	m_SMVIEWID = glGetUniformLocation(programID, "SpecularMappingView");
}

DeferredMaterial::~DeferredMaterial()
{
}

void DeferredMaterial::render(Model *model)
{
	//TEXTURES
	if (m_shader->useProgram())
	{

		//OPTIONS
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		//glEnable(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		//Enable depth test
		glDepthMask(GL_TRUE);
		
		glDisable(GL_BLEND);
		
		
		//m_activeTextures(m_shader->getID());
		//UNIFORMS
		m_Normal_Matrix = glm::transpose(glm::inverse(m_M));
		//set the uniform variable MVP
		glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M[0][0]);
		glUniformMatrix4fv(m_NMID, 1, GL_FALSE, &m_Normal_Matrix[0][0]);
		glUniform1i(m_NMACTIVEID, Scene::normalMapActive);
		glUniform1i(m_SMACTIVEID, Scene::specularMapActive);
		glUniform1i(m_SMVIEWID, Scene::specularMapView);
		glUniform2f(m_matID, m_mat.Ks, m_mat.Kr);
		
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, m_scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(m_shader->getID(), m_block_index_camera, 4);
		//RENDER
		//render model
		model->render();
		//RELEASE
		//m_releaseTextures();
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		m_shader->release();
	}
}

void DeferredMaterial::translateModel(glm::vec3 T)
{
	m_M = translate(m_M, T);
}

void DeferredMaterial::scaleModel(glm::vec3 T)
{
	m_M = scale(m_M, T);
}

void DeferredMaterial::rotateModel(glm::vec3 T)
{
	m_M = rotate(m_M, T);
}