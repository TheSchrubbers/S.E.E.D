#include "Outils.hpp"

void AfficherMatrices(glm::mat4 & mat)
{
	for (int i = 0; i < 4; i++)
	{
		printf("%f %f %f %f\n", mat[i].x, mat[i].y, mat[i].z, mat[i].w);
	}
}