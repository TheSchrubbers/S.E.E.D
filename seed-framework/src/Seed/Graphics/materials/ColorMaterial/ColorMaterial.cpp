//SEED INCLUDES
#include <Seed/Graphics/materials/ColorMaterial/ColorMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>

ColorMaterial::ColorMaterial(std::shared_ptr<Scene> sce, const std::string n, glm::vec3 col, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	m_color = col;
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "ColorMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}

void ColorMaterial::init()
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
	m_colorID = glGetUniformLocation(programID, "color");
	m_VLightID = glGetUniformLocation(programID, "VPLight");
	m_biasID = glGetUniformLocation(programID, "bias");
	m_shadowsActiveID = glGetUniformLocation(programID, "shadowsActive");
}

ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::render(Model *model)
{
	if (m_shader->useProgram())
	{
		//UNIFORMS
		m_Normal_Matrix = glm::transpose(glm::inverse(m_M));
		//set the uniform variable MVP
		glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M[0][0]);
		glUniformMatrix4fv(m_NMID, 1, GL_FALSE, &m_Normal_Matrix[0][0]);
		glUniform1i(m_NMACTIVEID, Scene::normalMapActive);
		glUniform1i(m_SMACTIVEID, Scene::specularMapActive);
		glUniform1i(m_SMVIEWID, Scene::specularMapView);
		glUniform1i(m_SMVIEWID, Scene::specularMapView);
		glUniform1i(m_shadowsActiveID, Scene::shadowMapActive);
		glUniform1f(m_biasID, Scene::bias);
		glUniform2f(m_matID, m_mat.Ks, m_mat.Kr);
		glUniform3f(m_colorID, m_color.x, m_color.y, m_color.z);
		glm::mat4 VLight = glm::lookAt(glm::vec3(0.0,3.0,8.0), glm::vec3(0.0), glm::vec3(0.0,1.0,0.0));
		glm::mat4 PLight = glm::perspective(
			180.0f,         //angle d'ouverture de la caméra
			(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
			1.0f,        // la ou commence le frustrum
			30.0f// la ou finit le frustrum
		);
		//glm::mat4 PLight = glm::ortho(-10.0, 10.0, -10.0, 10.0, 1.0, 30.0);
		glm::mat4 VPLight = PLight * VLight;
		glUniformMatrix4fv(m_VLightID, 1, GL_FALSE, &(VPLight[0][0]));
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		//TEXTURES
		glUniform1i(glGetUniformLocation(m_shader->getID(), "gShadowMap"), 0);
		glUniform1i(glGetUniformLocation(m_shader->getID(), "gShadowMap2"), 1);
		
		//RENDER
		//render model
		model->render();

	}
}

void ColorMaterial::translateModel(glm::vec3 T)
{
	m_M = translate(m_M, T);
}

void ColorMaterial::scaleModel(glm::vec3 T)
{
	m_M = scale(m_M, T);
}

void ColorMaterial::rotateModel(glm::vec3 T)
{
	m_M = rotate(m_M, T);
}