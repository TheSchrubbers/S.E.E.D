#include <Seed/Graphics/data_structure/KDtree.hpp>
#include <Seed/Graphics/Outils.hpp>
#include <iterator>
#include <stack>

#include <stdlib.h>
#include <time.h>
#include <Seed/Graphics/shader.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>

#define DIMENSION 3
KDtree::KDtree(std::vector<ParticleSPH*> pts, int depth)
{
	this->constructKDtree(pts, depth);
}

KDtree::~KDtree()
{
}

void KDtree::constructKDtree(std::vector<ParticleSPH*> pts, int depth)
{
	this->root = this->addKDnode(pts, depth);
	this->root->father = nullptr;
}

std::shared_ptr<KDnode> KDtree::addKDnode(std::vector<ParticleSPH*> pts, int depth)
{
	std::shared_ptr<KDnode> node(std::make_shared<KDnode>());
	node->left = nullptr;
	node->right = nullptr;
	node->deleted = false;
	int k = depth % DIMENSION;
	if (pts.size() > 1)
	{
		switch (k)
		{
		case 0:
			std::sort(pts.begin(), pts.end(), sortPointsX());
			break;
		case 1:
			std::sort(pts.begin(), pts.end(), sortPointsY());
			break;
		case 2:
			std::sort(pts.begin(), pts.end(), sortPointsZ());
		}
	}
	//root node
	int mid = pts.size() / 2;
	//random position of the node in the plan N.(axis k) = 0
	node->position = glm::vec3(pts[mid]->position);
	//node->position = glm::vec3(0.0);
	//node->position[k] = (pts[mid]->position[k]);
	//orientation of the vector of the node
	node->orientation = glm::vec3(0.0);
	node->orientation[k] = 1.0f;
	if (pts.size() > 1 && depth > 0)
	{
		node->list.push_back(pts[mid]);
		//left and right part of the vector
		std::vector<ParticleSPH*> left(pts.begin(), pts.begin() + mid), right(pts.begin() + mid + 1, pts.end());
		//recursive algorithm for left and right child
		if (left.size())
		{
			node->left = this->addKDnode(left, depth - 1);
			node->left->father = node;
		}
		if (right.size())
		{
			node->right = this->addKDnode(right, depth - 1);
			node->right->father = node;
		}
	}
	else
	{
		for (ParticleSPH* pttmp : pts)
		{
			node->list.push_back(pttmp);
		}
	}
	return node;
}



void KDtree::afficher()
{
	std::stack<std::shared_ptr<KDnode>> nodes;
	nodes.push(this->root);
	std::shared_ptr<KDnode> node;
	int k = 0;
	std::cout << "afficher" << std::endl;
	while (!nodes.empty())
	{
		k++;
		node = nodes.top();
		nodes.pop();
		if (node)
		{
			if (node->list.size())
				std::cout << node->list.at(0)->position.x << " " << node->list.at(0)->position.y << " " << node->list.at(0)->position.z << std::endl;
			if (node->left)
				nodes.push(node->left);
			if (node->right)
				nodes.push(node->right);
		}
	}
	std::cout << k << std::endl;
}




std::vector<ParticleSPH*> KDtree::radiusNeighbouring(ParticleSPH* pt, float &radius)
{
	int step = 0;
	std::vector<ParticleSPH*> pts;
	std::shared_ptr<KDnode> node;
	std::stack<std::shared_ptr<KDnode>> nodes;
	nodes.push(root);
	int k = 0;
	while (!nodes.empty())
	{
		step++;
		node = nodes.top();
		nodes.pop();
		//node is into the sphere?
		for (ParticleSPH* pttmp : node->list)
		{
			//if point in the node into the sphere of the current point
			if (pt->position != pttmp->position && glm::distance(pttmp->position, pt->position) <= radius)
			{
				pts.push_back(pttmp);
			}
		}

		//if it's a leaf
		if (node->left != nullptr || node->right != nullptr)
		{
			//look at if the median plan is intersecting by the sphere
			k = this->getOrientation(node->orientation);
			if (!intersectionSpherePlan(glm::vec3(pt->position.x, pt->position.y, pt->position.z), radius, node->orientation, node->position))
			{
				//if not intersecting, look at if the point is one or other side of the median plan
				if (pt->position[k] <= node->position[k])
				{
					if (node->left)
						nodes.push(node->left);
				}
				else
				{
					if (node->right)
						nodes.push(node->right);
				}
			}
			else
			{
				//intersecting, we don't know if a node from the both side can be into the sphere
				//we get the both children
				if (node->left)
					nodes.push(node->left);
				if (node->right)
					nodes.push(node->right);
			}
		}
	}
	//std::cout << "step : " << step << " nb particles : " << pts.size() << std::endl;
	return pts;
}

int KDtree::radiusNeighbouringNumber(glm::vec4 &pt, float &radius)
{
	int Neighbours = 0;
	std::vector<ParticleSPH*> pts;
	std::shared_ptr<KDnode> node;
	std::stack<std::shared_ptr<KDnode>> nodes;
	nodes.push(root);
	int k = 0;
	while (!nodes.empty())
	{
		node = nodes.top();
		nodes.pop();
		//node is into the sphere?
		for (ParticleSPH* pttmp : node->list)
		{
			//if point in the node into the sphere of the current point
			if (pt != pttmp->position && glm::distance(pttmp->position, pt) <= radius)
			{
				Neighbours++;
			}
		}

		//if it's a leaf
		if (node->left != nullptr || node->right != nullptr)
		{
			//look at if the median plan is intersecting by the sphere
			k = this->getOrientation(node->orientation);
			if (!intersectionSpherePlan(glm::vec3(pt), radius, node->orientation, node->position))
			{
				//if not intersecting, look at if the point is one or other side of the median plan
				if (pt[k] <= node->position[k])
				{
					if (node->left)
						nodes.push(node->left);
				}
				else
				{
					if (node->right)
						nodes.push(node->right);
				}
			}
			else
			{
				//intersecting, we don't know if a node from the both side can be into the sphere
				//we get the both children
				if (node->left)
					nodes.push(node->left);
				if (node->right)
					nodes.push(node->right);
			}
		}
	}
	return Neighbours;
}

std::vector<ParticleSPH*> KDtree::radiusNeighbouring(glm::vec4 &pt, float &radius)
{
	int step = 0;
	std::vector<ParticleSPH*> pts;
	std::shared_ptr<KDnode> node;
	std::stack<std::shared_ptr<KDnode>> nodes;
	nodes.push(root);
	int k = 0;
	while (!nodes.empty())
	{
		step++;
		node = nodes.top();
		nodes.pop();
		//node is into the sphere?
		for (ParticleSPH* pttmp : node->list)
		{
			//if point in the node into the sphere of the current point
			if (pt != pttmp->position && glm::distance(pttmp->position, pt) <= radius)
			{
				pts.push_back(pttmp);
			}
		}

		//if it's a leaf
		if (node->left != nullptr || node->right != nullptr)
		{
			//look at if the median plan is intersecting by the sphere
			k = this->getOrientation(node->orientation);
			if (!intersectionSpherePlan(glm::vec3(pt), radius, node->orientation, node->position))
			{
				//if not intersecting, look at if the point is one or other side of the median plan
				if (pt[k] <= node->position[k])
				{
					if (node->left)
						nodes.push(node->left);
				}
				else
				{
					if (node->right)
						nodes.push(node->right);
				}
			}
			else
			{
				//intersecting, we don't know if a node from the both side can be into the sphere
				//we get the both children
				if (node->left)
					nodes.push(node->left);
				if (node->right)
					nodes.push(node->right);
			}
		}
	}
	//std::cout << "step : " << step << " nb particles : " << pts.size() << std::endl;
	return pts;
}


void KDtree::addElement(ParticleSPH *p)
{
	std::shared_ptr<KDnode> n = root;
	int k = this->getOrientation(n->orientation);
	while (n->left || n->right)
	{
		if (n->position[k] > p->position[k])
		{
			if (n->right)
				n = n->right;
			else
			{
				k = (k + 1) % DIMENSION;
				n->right = std::shared_ptr<KDnode>(std::make_shared<KDnode>());
				n->right->father = n;
				n->right->list.push_back(p);
				n->right->position = glm::vec3(p->position);
				n->right->orientation = glm::vec3(0.0);
				n->right->orientation[k] = 1.0;
				break;
			}

		}
		else
		{
			if (n->left)
				n = n->left;
			else
			{

				k = (k + 1) % DIMENSION;
				n->left = std::shared_ptr<KDnode>(std::make_shared<KDnode>());
				n->left->father = n;
				n->left->list.push_back(p);
				n->left->position = glm::vec3(p->position);
				n->left->orientation = glm::vec3(0.0);
				n->left->orientation[k] = 1.0;
				break;
			}
		}
		k++;
	}
}

void KDtree::delElement(ParticleSPH *p)
{
	std::shared_ptr<KDnode> n = root;
	int k = this->getOrientation(n->orientation);
	while (n->position == glm::vec3(p->position))
	{
		k = this->getOrientation(n->orientation);
		if (n->position == glm::vec3(p->position))
		{
			n->deleted = true;
			break;
		}
		if (n->position[k] > p->position[k])
		{
			n = n->right;
		}
		else
		{
			n = n->left;
		}
		k++;
	}
}


/*
std::vector<ParticleSPH*> KDtree::radiusNeighbouring(ParticleSPH* pt, float &radius)
{
	int step = 0;
	std::vector<ParticleSPH*> pts;
	std::shared_ptr<KDnode> node;
	std::stack<std::shared_ptr<KDnode>> nodes;
	nodes.push(root);
	int k = 0;
	while (!nodes.empty())
	{
		step++;
		node = nodes.top();
		nodes.pop();
		//node is into the sphere?
		for (ParticleSPH* pttmp : node->list)
		{
			//if point in the node into the sphere of the current point
			if (pt->position != pttmp->position && glm::distance(pttmp->position, pt->position) <= radius)
			{
				pts.push_back(pttmp);
			}
		}

		//if it's a node
		if (node->left != nullptr || node->right != nullptr)
		{
			//look at if the median plan is intersecting by the sphere
			k = this->getOrientation(node->orientation);
			//if (!intersectionSpherePlan(glm::vec3(pt->position.x, pt->position.y, pt->position.z), radius, node->orientation, node->position))
			//bool inter = intersectionSphereAlignedPlan(pt->position, radius, node->position, k);
			bool inter = intersectionSpherePlan(glm::vec3(pt->position), radius, node->orientation, node->position);
			glm::vec3 A = glm::vec3(pt->position);
			A[k] = node->position[k];

			int KFather = (k - 1);
			if (KFather < 0)
				KFather = DIMENSION - 1;

			std::cout << inter << std::endl;
			if (node->father)
				std::cout << A[KFather] << " " << node->position[KFather] << " " << node->father->position[KFather] << std::endl;

			//if the sphere intersect the root plan or a child plan and the intersection is on the side of the child median plan
			//we watch the both children
			if (
				inter && 
					(node->father == nullptr 
					|| 
						(	
							(A[KFather] <= node->father->position[KFather] && node->position[KFather] <= node->father->position[KFather]) 
							|| 
							(A[KFather] >= node->father->position[KFather] && node->position[KFather] >= node->father->position[KFather])
							||
							(glm::abs(pt->position[KFather] - node->father->position[KFather]) <= radius)
						)
					)
				)
			{
				//we look at the both sides of the median plan
				//intersecting, we don't know if a node from the both side can be into the sphere
				//we get the both children
				if (node->left)
					nodes.push(node->left);
				if (node->right)
					nodes.push(node->right);
			}
			else
			{
				if (pt->position[k] <= node->position[k] && node->left)
				{
					nodes.push(node->left);
				}
				else if (pt->position[k] >= node->position[k] && node->right)
				{
					nodes.push(node->right);
				}
			}
		}
	}
	std::cout << "step : " << step << std::endl;
	return pts;
}*/

int KDtree::getOrientation(glm::vec3 &orientation)
{
	for (int i = 0; i < 3; i++)
	{
		if (orientation[i] != 0)
		{
			return i;
		}
	}
	return -1;
}

bool intersectionSphereAlignedPlan(glm::vec4 &C, float r, glm::vec3 &P, int k, glm::vec3 Axis)
{
	glm::vec3 A = glm::vec3(C);
	A[k] = P[k];
	if (glm::abs(C[k] - P[k]) < r)
		return true;
	return false;
}