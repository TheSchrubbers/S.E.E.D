#include <Seed/Graphics/imageLoading/tga.hpp>

bool TGA::Load(const char* filename)
{
	std::fstream filestr;

	filestr.open(filename, std::ios::in | std::ios::binary);								// Open file
	if (filestr.is_open())														// Do the following actions, if file is opened
	{
		// read TGA header
		filestr.read((char*)&tgaheader, sizeof(struct tga_header));			// Read tga header. For more information: see tga.h and link above
		printf("image type: %i n", tgaheader.imageTypeCode);

		// read pixel data
		int imageSize = tgaheader.width * tgaheader.height * tgaheader.bpp;		// Calculate image size

		this->imageData = (char*)malloc(imageSize);							// Reserve space in the memory to store our image data
		filestr.read((char*) this->imageData, imageSize);						// Read image data from file, into the reserved memory place


		/*
		* TGA is stored in BGR (Blue-Green-Red) format,
		* we need to convert this to Red-Green-Blue (RGB).
		* The following section does BGR to RGB conversion
		*/

		if (tgaheader.bpp == 24)
		{
			for (int i = 0; i < imageSize; i += 3)
			{
				char c = this->imageData[i];
				this->imageData[i] = this->imageData[i + 2];
				this->imageData[i + 2] = c;
			}
		}
		else
			if (tgaheader.bpp == 32)
			{
				for (int i = 0; i < imageSize; i += 4)
				{
					// 32 bits per pixel   =  4 byte per pixel			
					char c = this->imageData[i];
					this->imageData[i] = this->imageData[i + 2];
					this->imageData[i + 2] = c;
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