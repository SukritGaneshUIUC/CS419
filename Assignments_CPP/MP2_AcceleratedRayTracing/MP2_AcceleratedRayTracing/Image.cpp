#include "Image.h"

/*
* Initialize the image to contain rows * columns pixels
* Pixel values are undefined
*
* @param row The number of rows in the image
* @param cols The number of columns in the image
*
*/
void Image::initializeImage(const int& rows, const int& cols)
{
	pixels = new ColorRGB[rows * cols];

	this->rows = rows;
	this->cols = cols;
}

/*
* Deep copies the image from another image object
*
* @param other The image to be copied
*/
void Image::copyImage(const Image& other)
{
	this->rows = other.rows;
	this->cols = other.cols;
	initializeImage(rows, cols);
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			set(i, j, other.get(i, j));
		}
	}
	//setImage(other.rows, other.cols, other.pixels);
}

/*
* Sets every pixel in the image to a single color
*
* @param theColor The color to fill the image with
*/
void Image::singleColorFill(const ColorRGB& theColor)
{
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < rows; j++) {
			set(i, j, theColor);
		}
	}
}

/*
* Default constructor for image
*/
Image::Image() {
	initializeImage(2160, 3840);
}

/*
* Construct an image with a given number of rows and columns, pixel values remain undefined
*
* @param rows The number of rows in the image
* @param cols The number of columns in the image
*/
Image::Image(const int& rows, const int& cols)
{
	initializeImage(rows, cols);
}

/*
* Construct an image with a given number of rows and columns
* All pixels set to a single color
*
* @param rows The number of rows in the image
* @param cols The number of columns in the image
* @param theColor The color to set all pixels in the image to
*/
Image::Image(const int& rows, const int& cols, const ColorRGB& theColor)
{
	setImage(rows, cols, theColor);
}

/*
* Construct an image with a given number of rows and columns
* Pixel values set to a 1D array of pixels of length row * cols, with pixels stored in row-major order
*
* @param rows The number of rows in the image
* @param cols The number of columns in the image
* @param thePixels The 1D array of colors to set the image's pixels to, in row-major format
*/
Image::Image(const int& rows, const int& cols, const ColorRGB*& thePixels)
{
	setImage(rows, cols, thePixels);
}

/*
* Copy constructor for image
*
* @param other The image object to be copied
*/
Image::Image(const Image& other)
{
	std::cout << "COPY CONSTRUCTOR! AVOID!" << std::endl;
	if (this != &other) {
		copyImage(other);
	}
}

/*
* Copy assignment operator for image
*
* @param other The image object to be copied
*/
Image& Image::operator=(const Image& other)
{
	std::cout << "COPY ASSIGNMENT! AVOID!" << std::endl;
	if (this != &other) {
		copyImage(other);
	}
	return *this;
}

/*
* Move constructor for image
*
* @param other The image object to be moved
*/
Image::Image(Image&& other)
{
	std::cout << ":) Move Constructor. Good!" << std::endl;
	if (this != &other) {
		this->rows = other.rows;
		this->cols = other.cols;
		this->pixels = other.pixels;
		other.pixels = nullptr;
	}
}

/*
* Move assignment operator for image
*
* @param other The image object to be moved
*/
Image& Image::operator=(Image&& other)
{
	std::cout << ":) Move Assignment. Good!" << std::endl;
	if (this != &other) {
		this->rows = other.rows;
		this->cols = other.cols;
		this->pixels = other.pixels;
		other.pixels = nullptr;
	}
	return *this;
}

/*
* Image destructor
*/
Image::~Image()
{
	if (pixels) {
		delete[] pixels;
	}
}

/*
* Get the color of a pixel in the image
* Top-right is row, column (0,0)
*
* @param row The row of the pixel
* @param col The column of the pixel
*
* @return The color of the pixel located at the specified row and column
*/
const ColorRGB& Image::get(const int& row, const int& col) const
{
	return pixels[rows * row + col];
}

/*
* @return The number of rows in the image
*/
const int& Image::getRows() const
{
	return rows;
}

/*
* @return The number of columns in the image
*/
const int& Image::getCols() const
{
	return cols;
}

/*
* Set the color of a pixel in the image
* Top-right is row, column (0,0)
*
* @param row The row of the pixel
* @param col The column of the pixel
* @param color The color to set the pixel to
*/
void Image::set(const int& row, const int& col, const ColorRGB& color)
{
	pixels[rows * row + col] = color;
}

/*
* Set the contents of the image equal to the contents of another image
* Make sure rows and columns are set correctly
*
* @param theImage The image to set
*/
void Image::setImage(const ColorRGB*& theImage)
{
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			set(row, col, theImage[rows * row + col]);
		}
	}
}

/*
* Set the dimensions of the image, and set all pixels to a single color
*
* @param rows The number of rows
* @param columns The number of columns
* @param theColor The color to set all pixels to
*/
void Image::setImage(const int& rows, const int& cols, const ColorRGB& theColor)
{
	initializeImage(rows, cols);
	singleColorFill(theColor);
}

/*
* Set the dimensions of the image, and set all pixel colors
*
* @param rows The number of rows
* @param columns The number of columns
* @param theImage a 1D array containing the pixel colors, in row-major order
*/
void Image::setImage(const int& rows, const int& cols, const ColorRGB*& theImage)
{
	initializeImage(rows, cols);
	setImage(theImage);
}

/*
* Write the image to a .ppm file
*
* @param outputFilePath The path of the output file
*/
void Image::writeToFile(const std::string& outputFilePath) const {
	std::ofstream file;
	file.open(outputFilePath);

	// PPM Specification
	file << PPM_ID << "\n";

	// width and height
	file << cols << " " << rows << "\n";

	// max color value
	file << 255 << "\n";

	// pixels
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			file << get(row, col).renderString() << " ";
		}
		file << "\n";
	}
	file.close();
}