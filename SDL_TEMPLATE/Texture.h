#pragma once

#include <iostream>

class Texture {
private:
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width;
	int height;
	int bpp;

public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
};