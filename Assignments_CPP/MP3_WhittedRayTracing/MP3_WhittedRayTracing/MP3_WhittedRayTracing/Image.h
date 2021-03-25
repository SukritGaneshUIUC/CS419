#pragma once

#include <fstream>
#include <iostream>

#include "Vec3D.h"


// INDEXED FROM TOP LEFT IN FORMAT (ROW, COL)

class Image
{
private:
	const char* PPM_ID = "P3";

	ColorRGB* pixels;
	int rows;
	int cols;

	void initializeImage(const int& rows, const int& cols);
	void copyImage(const Image& other);
	void singleColorFill(const ColorRGB& theColor);
public:
	Image();
	Image(const int& rows, const int& cols);
	Image(const int& rows, const int& cols, const ColorRGB& theColor);
	Image(const int& rows, const int& cols, const ColorRGB*& thePixels);
	Image(const Image& other);
	Image& operator=(const Image& other);
	Image(Image&& other);
	Image& operator=(Image&& other);
	~Image();

	const ColorRGB& get(const int& row, const int& col) const;
	const int& getRows() const;
	const int& getCols() const;

	void set(const int& row, const int& col, const ColorRGB& color);
	void setImage(const ColorRGB*& theImage);
	void setImage(const int& rows, const int& cols, const ColorRGB& theColor);
	void setImage(const int& rows, const int& cols, const ColorRGB*& theImage);

	/*
	* Write to PPM File
	* Remember to include .ppm file type in outputFilePath argument
	*/

	void writeToFile(const std::string& outputFilePath) const;
};

