//SEED INCLUDES
#include <Seed/Graphics/engine/camera.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 upVector, float FOV, float width, float height, float n, float f , float WA, float HA, float s, float ms)
{
	m_position = pos;
	m_direction = glm::normalize(pos - look);
	m_up = glm::normalize(upVector);
	m_WAngle = WA;
	m_HAngle = HA;
	m_FoV = f;
	m_near = n;
	m_far = f;
	m_speed = s;
	m_mouseSpeed = ms;
	m_right = glm::normalize(glm::cross(m_up, m_direction));
	m_matrix.V = glm::lookAt(pos, look, upVector);
	m_matrix.P = glm::perspective(
		FOV,         //angle d'ouverture de la caméra
		width/height, // ratio de la résolution de l'ecran
		m_near,        // la ou commence le frustrum
		m_far       // la ou finit le frustrum
	);

	//create a UBObuffer
	m_camBuf = new UBOBuffer();
	m_camBuf->create(sizeof(struct Matrix));
}

Camera::~Camera()
{
	delete m_camBuf;
}

glm::mat4 Camera::getViewMatrix()
{
	return m_matrix.V;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return m_matrix.P;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}

glm::vec3 Camera::getDirection()
{
	return m_direction;
}

glm::vec3 Camera::getUp()
{
	return m_up;
}

float Camera::getWAngle()
{
	return m_WAngle;
}

float Camera::getHAngle()
{
	return m_HAngle;
}

float Camera::getNear()
{
	return m_near;
}

float Camera::getFar()
{
	return m_far;
}

float Camera::getSpeed()
{
	return m_speed;
}

float Camera::getMouseSpeed()
{
	return m_mouseSpeed;
}

float Camera::getFoV()
{
	return m_FoV;
}

const float Camera::getInitFoV()
{
	return m_initFoV;
}

void Camera::setHAngle(float HA)
{
	m_HAngle = HA;
}

void Camera::setWAngle(float WA)
{
	m_WAngle = WA;
}


void Camera::setProjectionMatrix(const float &F, const int width, const int height, const float &n, const float &fa)
{
	m_FoV = F;
	m_near = n;
	m_far = fa;
	m_matrix.P = glm::perspective(F, (float)width / (float)height, n, fa);
}

void Camera::setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &u)
{
	m_position = pos;
	m_direction = dir;
	m_up = u;
	m_matrix.V = glm::lookAt(pos, pos + dir, u);
}

void Camera::setViewMatrix(glm::vec3 &dir, glm::vec3 &up)
{
	m_direction = dir;
	m_up = up;
	m_matrix.V = glm::lookAt(m_position, m_position + dir, up);
}

void Camera::setViewMatrix(glm::vec3 &pos)
{
	m_position = pos;
	m_matrix.V = glm::lookAt(pos, pos + m_direction, m_up);
}

void Camera::setViewMatrix(glm::mat4 &V)
{
	m_matrix.V = V;
}

void Camera::setMouseSpeed(const float speed)
{
	m_mouseSpeed = speed;
}

glm::vec3 Camera::getRight()
{
	return m_right;
}

void Camera::setRight(glm::vec3 &r)
{
	m_right = r;
}

void Camera::updateUBO()
{
	m_matrix.V_inverse = glm::inverse(m_matrix.V);
	//send data
	m_camBuf->update(&m_matrix, sizeof(struct Matrix));
}

GLuint Camera::getUBOId()
{
	return m_camBuf->getID();
}

void Camera::setUp(glm::vec3 &u)
{
	m_up = u;
}
