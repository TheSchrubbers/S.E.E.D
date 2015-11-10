#include <Seed/Graphics/data_structure/KDtree.hpp>
#include <Seed/Graphics/Outils.hpp>
#include <iterator>

#define DIMENSION 3

KDtree::KDtree(){}

KDtree::~KDtree(){}

void KDtree::constructKDtree(std::vector<Point*> pts, int depth)
{
	this->root = this->addKDnode(pts, depth);
}

KDnode* KDtree::addKDnode(std::vector<Point*> pts, int depth)
{
	KDnode *node;
	int k = depth % DIMENSION;
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

	//root node
	int mid = pts.size() / 2;
	node->position = glm::vec3(0.0);
	node->position[k] = (pts[mid]->p[k] + pts[mid + 1]->p[k]) / 2.0;
	node->orientation = glm::vec3(0.0);
	node->orientation[k] = 1.0f;
	std::vector<Point*> left(pts.begin(), pts.begin() + mid), right(pts.begin() + mid + 1, pts.end());
	if (depth > 0)
	{
		node->left = this->addKDnode(left, depth - 1);
		node->right = this->addKDnode(right, depth - 1);
	}
	return node;
}

