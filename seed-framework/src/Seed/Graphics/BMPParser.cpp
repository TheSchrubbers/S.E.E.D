#include <Seed/Graphics/BMPParser.hpp>

bool readBMP(parserImage *pi, char* img, unsigned short type)
{
	switch(type)
	{
		case 1:
		{
			pi->setWidth((int)img[21]* 256 * 256 * 256 + (int)img[20] * 256 * 256 + (int)img[19] * 256 + (int)img[18]);
			pi->setHeight((int)img[25]* 256 * 256 * 256 + (int)img[24] * 256 * 256 + (int)img[23] * 256 + (int)img[22]);
			pi->setDepth((int)img[28] + (int)img[29] * 256);
			pi->setCompression((int)img[30] + (int)img[31] * 256 + (int)img[32] * 256 * 256 + (int)img[33] * 256 * 256 * 256);

			if(pi->getCompression() != 0)
			{
				printf("ERROR: Bitmap compressed\n");
				return false;
			}

			//get offset of beginning of datas
			int offset = (int)img[10] + (int)img[11] * 256 + (int)img[12] * 256 * 256 + (int)img[13] * 256 * 256 * 256;
			//get size of image
			int size = (int)img[34] + (int)img[35] * 256 + (int)img[36] * 256 * 256 + (int)img[37] * 256 * 256 * 256;
			//set datas in pixels array
			pi->setPixels(img, offset, size);

			return true;
		}
	}
	return false;
}