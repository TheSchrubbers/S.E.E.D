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
* \file KDtree.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef KDTREE_HPP
#define KDTREE_HPP
#include <vector>
#include <Seed/Graphics/Outils.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <Seed/Graphics/particles/SPH.hpp>
#include <memory>

struct KDnode
{
	glm::vec3 orientation;
	glm::vec3 position;
	std::vector<ParticleSPH*> list;
	std::shared_ptr<KDnode> left, right;
};

/*! \class KDtree
* \brief KDtree class constructs KD-tree
*/
class Scene;
class Shader;
class KDtree
{
	
public:
	KDtree(std::vector<ParticleSPH*> pts, int depth);
	~KDtree();
	std::shared_ptr<KDnode> addKDnode(std::vector<ParticleSPH*> pts, int depth); 
	void constructKDtree(std::vector<ParticleSPH*> pts, int depth);
	std::vector<ParticleSPH*> radiusNeighbouring(ParticleSPH* pt, float &radius);
	int getOrientation(glm::vec3 &orientation);
	void afficher();
	void render(Scene* sce);
private:
	std::shared_ptr<KDnode> root;
};



#endif