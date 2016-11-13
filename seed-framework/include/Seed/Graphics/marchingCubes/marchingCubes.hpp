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
* \file marchingCube.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef MARCHINGCUBES_HPP
#define MARCHINGCUBES_HPP

//SEED INCLUDES
#include "Seed/Graphics/engine/tools.hpp"
//OTHER INCLUDES
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


class ImplicitSphere;

/*! \class MarchingCube
* \brief MarchingCube class using marching cubes algorithm to modelize implicit surface
*/
class MarchingCubes
{
public:
	/*!
	* \brief Constructor of the MarchingCubes class, algorithm of the marching cubes
	* \param iSphere implicit surface
	* \param resolution the resolution the the grid of cubes -> 2^resolution
	*/
	MarchingCubes(ImplicitSphere *iSphere, const float resolution);
	/*!
	* \brief Constructor of the MarchingCubes class, algorithm of the marching cubes
	* \param isolevel
	* \param resolution the resolution the the grid of cubes -> 2^resolution
	*/
	MarchingCubes(float isolevel, const float resolution);
	/*!
	* \destructor of the MarchingCubes class
	*/
	~MarchingCubes();
	/*!
	* \brief Compute position and density of each point (xi,y,zi) of the row y
	* \param Vmin the beginning vertice for computing row
	* \param points an empty array of struct Point which gets data of the processed row
	*/
	void computeRow(const glm::vec3 &Vmin, std::vector<std::shared_ptr<Point>> &points);
	/*!
	* \brief Construct a mesh from a implicit surface
	* \return A vector of points of the mesh
	*/
	std::vector<std::shared_ptr<Point>> construcMesh();
	/*!
	* \brief Search which vertices are in or on implicit surface
	* \param d1 density of the vertice 0
	* \param d2 density of the vertice 1
	* \param d3 density of the vertice 2
	* \param d4 density of the vertice 3
	* \param d5 density of the vertice 4
	* \param d6 density of the vertice 5
	* \param d7 density of the vertice 6
	* \param d8 density of the vertice 7
	* \return the 8bits mask of vertices in implicit surface
	*/
	unsigned char computeCube(float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8);
	/*!
	* \brief Search the closest vertice of the implicit surface on the X axis of the cube
	* \param p0 One vertice of the current edge (X axis)
	* \param p1 The other vertice of the current edge (X axis)
	* \param out The result vertice
	*/
	void intersect_xaxis(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> out);
	/*!
	* \brief Search the closest vertice of the implicit surface on the Y axis of the cube
	* \param p0 One vertice of the current edge (Y axis)
	* \param p1 The other vertice of the current edge (Y axis)
	* \param out The result vertice
	*/
	void intersect_yaxis(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> out);
	/*!
	* \brief Search the closest vertice of the implicit surface on the Z axis of the cube
	* \param p0 One vertice of the current edge (Z axis)
	* \param p1 The other vertice of the current edge (Z axis)
	* \param out The result vertice
	*/
	void intersect_zaxis(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> out);
private:
	glm::vec3 sizeCube, Vmin, Vmax;
	glm::ivec3 size;
	ImplicitSphere *iSphere;

	static const int edgeTable[256];
	static const int triTable[256][16];
};


#endif