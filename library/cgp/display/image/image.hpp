#pragma once

#include "cgp/containers/containers.hpp"

namespace cgp
{
	enum class image_color_type {rgb, rgba};
	struct image_raw
	{
		image_raw();
		image_raw(unsigned int width_arg, unsigned int height_arg, image_color_type color_type_arg, buffer<unsigned char> const& data_arg);

		unsigned int width;
		unsigned int height;
		image_color_type color_type;
		buffer<unsigned char> data;
	};

	image_raw image_load_png(std::string const& filename, image_color_type color_type = image_color_type::rgba);
	void image_save_png(std::string const& filename, image_raw const& im);
	image_raw image_load_jpg(std::string const& filename);
	void image_save_jpg(std::string const& filename, image_raw const& im);

	// Generic function to read an image file (expect .png or .jpg format)
	image_raw image_load_file(std::string const& filename);

	// Convert an image into a 2D grid structure 
	//  Each (r,g,b) component in [0,255] in the image is converted into a vec3 with component in [0,1]
	void convert(image_raw const& in, grid_2D<vec3>& out);

}