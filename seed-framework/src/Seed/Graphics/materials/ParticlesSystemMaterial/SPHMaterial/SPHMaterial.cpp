//SEED INCLUDES
#include <Seed/Graphics/materials/ParticlesSystemMaterial/SPHMaterial/SPHMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/engine/shader.hpp>


SPHMaterial::SPHMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	//load shaders
	m_shader = std::make_shared<Shader>(pathToMaterials + "SPHMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		init();
	else
		writeLog("Material : " + n + " loading fails");
}

void SPHMaterial::init()
{
	if (m_shader)
	{
		GLuint programID = m_shader->getID();
		m_complight.ambiant = 0.1;
		m_complight.diffuse = 0.8;
		m_complight.specular = 0.1;
		// Get a handle for our "MVP" uniform.
		// Only at initialisation time.
		m_matID = glGetUniformLocation(programID, "mat");
		m_complight.ambiantID = glGetUniformLocation(programID, "light.ambiant");
		m_complight.diffuseID = glGetUniformLocation(programID, "light.diffuse");
		m_complight.specularID = glGetUniformLocation(programID, "light.specular");
		m_block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
		m_block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
		m_block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
		m_block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
		m_block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
		m_uniformSSBOID = glGetProgramResourceIndex(m_shader->getID(), GL_SHADER_STORAGE_BLOCK, "ParticlesBuffer");
		glShaderStorageBlockBinding(m_shader->getID(), m_uniformSSBOID, 0);
		m_sph = new SPH(SPH::nbParticles, SPH::radiusParticle, SPH::radiusNeighbouring, m_scene);
	}
}

SPHMaterial::~SPHMaterial()
{
}

void SPHMaterial::render(Model *model)
{
	if (SPH::reset == true)
	{
		delete m_sph;
		m_sph = new SPH(SPH::nbParticles, SPH::radiusParticle, SPH::radiusNeighbouring, m_scene);
		SPH::reset = false;
	}
	m_sph->algorithm();
	if (m_shader->useProgram())
	{
		//UNIFORMS
		//set the uniform variable MVP
		glUniform1fv(m_complight.ambiantID, 1, &(m_complight.ambiant));
		glUniform1fv(m_complight.diffuseID, 1, &(m_complight.diffuse));
		glUniform1fv(m_complight.specularID, 1, &(m_complight.specular));
		glUniform1i(m_NMACTIVEID, Scene::normalMapActive);
		glUniform1i(m_SMACTIVEID, Scene::specularMapActive);
		glUniform1i(m_SMVIEWID, Scene::specularMapView);
		glUniform2f(m_matID, m_mat.Ks, m_mat.Kr);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
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
		//bind ssbo particles
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_sph->getSSBOID());

		//RENDER
		//render model
		model->render(m_sph->getNbParticles());
	}
}