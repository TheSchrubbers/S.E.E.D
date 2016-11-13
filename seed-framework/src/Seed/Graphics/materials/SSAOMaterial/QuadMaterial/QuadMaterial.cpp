//SEED INCLUDES
#include <Seed/Graphics/materials/SSAOMaterial/QuadMaterial/QuadMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>
//OTHER INCLUDES
#include <glm/gtc/random.hpp>


QuadMaterial::QuadMaterial(const aiMaterial *material, std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(material, sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "QuadMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}
QuadMaterial::QuadMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "QuadMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}

void QuadMaterial::init()
{
	for (int i = 0; i < NBKERNEL; i++)
	{
		m_kernel[i].value = glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
		m_kernel[i].value = glm::normalize(m_kernel[i].value);
		m_kernel[i].value *= glm::linearRand(0.0f, 1.0f);
		float scale = (float)i / (float)NBKERNEL;
		scale = 0.1f + scale * scale * 0.9f;
		m_kernel[i].value *= scale;
	}

	for (int i = 0; i < 4; i++)
	{
		m_noise[i].value = glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 0.0f), 0.0f);
		m_noise[i].value = glm::normalize(m_noise[i].value);
	}

	m_uboKernels = new UBOBuffer();
	m_uboKernels->create(NBKERNEL * sizeof(struct Kernel));
	m_uboKernels->update((void*)&m_kernel[0], NBKERNEL * sizeof(struct Kernel));
	/*m_uboNoise = new UBOBuffer();
	m_uboNoise->createBuffer(16 * sizeof(struct Noise));
	m_uboNoise->updateBuffer((void*)&m_noise[0], 16 * sizeof(struct Noise));*/

	GLuint programID = m_shader->getID();

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	m_block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	m_block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	m_block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	m_block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	m_block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
	m_kernelsID = glGetUniformBlockIndex(programID, "gKernelBuffer");
	//m_noiseID = glGetUniformBlockIndex(programID, "gNoiseBuffer");
}

QuadMaterial::~QuadMaterial()
{
}

void QuadMaterial::render(Model *model)
{
	if (m_shader->useProgram())
	{
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glClear(GL_COLOR_BUFFER_BIT);
		m_scene->getFBOBuffer()->bindRead(m_shader->getID());

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

		glBindBufferBase(GL_UNIFORM_BUFFER, 5, m_uboKernels->getID());
		glUniformBlockBinding(m_shader->getID(), m_kernelsID, 5);

		/*glBindBufferBase(GL_UNIFORM_BUFFER, 6, m_uboNoise->getID());
		glUniformBlockBinding(m_shader->getID(), m_noiseID, 6);*/

		//RENDER;
		//render model
		model->render();
	}
}
