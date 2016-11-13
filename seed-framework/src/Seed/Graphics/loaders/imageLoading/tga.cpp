#include <Seed/Graphics/loaders/imageLoading/tga.hpp>

bool TGA::Load(const char* filename)
{
	std::cout << "open tga file" << std::endl;
	std::fstream filestr;
	filestr.open(filename, std::ios::in | std::ios::binary);								// Open file
	std::cout << "fin open tga file" << std::endl;
	if (filestr.is_open())														// Do the following actions, if file is opened
	{
		std::cout << "read tga file" << std::endl;

		// read TGA header
		filestr.read((char*)&tgaheader, sizeof(struct tga_header));			// Read tga header. For more information: see tga.h and link above
		//printf("image type: %i n", tgaheader.imageTypeCode);

		std::cout << "fin read tga file" << std::endl;
		// read pixel data
		int imageSize = tgaheader.width * tgaheader.height * tgaheader.bpp;		// Calculate image size

		std::cout << "malloc memory : " << imageSize << std::endl;
		this->imageData = (char*)malloc(imageSize);							// Reserve space in the memory to store our image data
		std::cout << "fin malloc memory : " << imageSize << std::endl;
		std::cout << "read file" << std::endl;
		filestr.read((char*) this->imageData, imageSize);						// Read image data from file, into the reserved memory place
		std::cout << "fin read file" << std::endl;
		

		/*
		* TGA is stored in BGR (Blue-Green-Red) format,
		* we need to convert this to Red-Green-Blue (RGB).
		* The following section does BGR to RGB conversion
		*/
		if ((int)tgaheader.bpp == 24)
		{
			for (int i = 0; i < imageSize; i += 3)
			{
				char c = this->imageData[i];
				this->imageData[i] = this->imageData[i + 2];
				this->imageData[i + 2] = c;
			}
			if (tgaheader.xOrigin == 0 && tgaheader.yOrigin == 0)
			{
				int W = tgaheader.width * 3;
				int H = tgaheader.height;
				//flip image in y axis
				for (int i = 0; i < (H / 2); i++)
				{
					for (int j = 0; j < W; j++)
					{
						char c = this->imageData[j + i * W];
						this->imageData[j + i * W] = this->imageData[j + (H - i) * W];
						this->imageData[j + (H - i) * W] = c;
					}
				}
			}
		}
		else
			if ((int)tgaheader.bpp == 32)
			{
				for (int i = 0; i < imageSize; i += 4)
				{
					// 32 bits per pixel   =  4 byte per pixel			
					char c = this->imageData[i];
					this->imageData[i] = this->imageData[i + 2];
					this->imageData[i + 2] = c;
				}
				if (tgaheader.xOrigin == 0 && tgaheader.yOrigin == 0)
				{
					int W = tgaheader.width * 4;
					int H = tgaheader.height;
					//flip image in y axis
					for (int i = 0; i < (H / 2); i++)
					{
						for (int j = 0; j < W; j++)
						{
							char c = this->imageData[j + i * W];
							this->imageData[j + i * W] = this->imageData[j + (H - i) * W];
							this->imageData[j + (H - i) * W] = c;
						}
					}
				}
			}
		filestr.close();
	}
	else
	{
		return false;
	}

	return true;
}