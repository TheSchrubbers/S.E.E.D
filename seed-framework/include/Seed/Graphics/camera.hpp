/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
* \file camera.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/*! \class Camera
* \brief Camera class with matrix transformation attributes
*/
class Camera
{
public:
	/*!
	* \brief Constructor of class Camera
	* \param pos : Position of the camera
	* \param look : Position who looks the camera
	* \param upVector : The vertical vector of camera
	* \param FOV : The field of view of the camera
	* \param width : The width resolution of the screen
	* \param height : the height resolution of the screen
	* \param near : the min distance of field of view
	* \param far : the max distance of filed of view
	*/
	Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 upVector, float FOV, float width, float height, float near, float far);
	/*!
	* \brief Destructor of class Texture
	*/
	~Camera(){};
	/*!
	* \brief get the view matrix
	* \return glm::mat4 : View matrix
	*/
	glm::mat4 getViewMatrix();
	/*!
	* \brief get the projection matrix
	* \return glm::mat4 : Projection matrix
	*/
	glm::mat4 getProjectionMatrix();
	/*!
	* \brief set the view matrix
	* \param pos : reference to the position of the camera
	* \param dir : reference to the direction of the camera
	* \param up : reference to the vertical vector of the camera
	*/
	void setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &up);
	/*!
	* \brief set the projection matrix
	* \param Fov : reference to the field of view
	* \param width : reference to the width resolution
	* \param height : reference to the height resolution
	* \param near : reference to the min distance of field of view
	* \param far : reference to the max distance of field of view
	*/
	void setProjectionMatrix(const float &Fov, const int width, const int height, const float &near, const float &far);
private:
	/*!
	* \brief the view Matrix
	*/
	glm::mat4 View_matrix;
	/*!
	* \brief the projection matrix
	*/
	glm::mat4 Projection_matrix;
};

#endif