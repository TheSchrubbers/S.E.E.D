//SEED INCLUDES
#include <Seed/Graphics/materials/ParticlesSystemMaterial/ParticlesWaterSystemMaterial/ParticlesWaterSystemMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/particles/particlesSystem.hpp>
#include <Seed/Graphics/particles/particlesWaterSystem.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/engine/shader.hpp>

ParticlesWaterSystemMaterial::ParticlesWaterSystemMaterial(std::shared_ptr<Scene> sce, const int &nb, const std::string n, unsigned int *flag) : Material(sce, n, flag, 0.0, 0.0)
{
	m_nbParticles = nb;
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "ParticlesSystemMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
	{
		init();
		ParticlesWaterSystem *system = new ParticlesWaterSystem(sce, m_nbParticles, SEED_POINT, glm::vec3(0.0));
	}
	else
		writeLog("Material : " + n + " loading fails");
}

void ParticlesWaterSystemMaterial::init()
{
	m_M = glm::mat4(1.0);
	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	m_MID = glGetUniformLocation(m_shader->getID(), "M");
	m_block_index_camera = glGetUniformBlockIndex(m_shader->getID(), "CameraBuffer");
}

ParticlesWaterSystemMaterial::~ParticlesWaterSystemMaterial()
{
}

void ParticlesWaterSystemMaterial::render(Model *model)
{
	InstancedModel *m = dynamic_cast<InstancedModel*>(model);
	if (m_shader->useProgram())
	{
		//UNIFORMS
		//set the uniform variable MVP
		glUniformMatrix4fv(m_MID, 1, GL_FALSE, &m_M[0][0]);
		//OPTIONS
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//TEXTURES
		//m_activeTextures();

		//BUFFERS
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(m_shader->getID(), m_block_index_camera, 0);

		//RENDER
		//render model
		m->render(m_nbParticles);

		//RELEASE
		//m_releaseTextures();
	}
}