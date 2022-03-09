#include "image.hpp"

#include "cgp/base/base.hpp"
#include "cgp/files/files.hpp"
#include "third_party/src/lodepng/lodepng.h"
#include "third_party/src/jpeg/jpge.h"
#include "third_party/src/jpeg/jpgd.h"
#include "cgp/display/opengl/opengl.hpp"

namespace cgp
{
	image_raw::image_raw()
    :width(0), height(0), color_type(image_color_type::rgb), data()
    {}
    image_raw::image_raw(unsigned int width_arg, unsigned int height_arg, image_color_type color_type_arg, buffer<unsigned char> const& data_arg)
        :width(width_arg), height(height_arg), color_type(color_type_arg), data(data_arg)
    {}


    image_raw image_load_png(std::string const& filename, image_color_type color_type)
    {
        assert_file_exist(filename);

        LodePNGColorType lodepng_color_type;
        if(color_type==image_color_type::rgb)
            lodepng_color_type = LCT_RGB;
        else if(color_type==image_color_type::rgba)
            lodepng_color_type = LCT_RGBA;
        else
        {
            std::cerr<<"Unkown color type for file"<<filename<<std::endl;
            exit(1);
        }

        image_raw im;
        im.color_type = color_type;

        unsigned error = lodepng::decode(im.data.data, im.width, im.height, filename, lodepng_color_type);
        if ( error )
        {
            std::cerr<<"Error Loading png file "<<filename<<std::endl;
            std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
            exit(1);
        }

        return im;
    }

    void image_save_png(std::string const& filename, image_raw const& im)
    {
        LodePNGColorType lodepng_color_type;
        if(im.color_type==image_color_type::rgb)
            lodepng_color_type = LCT_RGB;
        else if(im.color_type==image_color_type::rgba)
            lodepng_color_type = LCT_RGBA;
        else
        {
            std::cerr<<"Unkown color type for file"<<filename<<std::endl;
            exit(1);
        }

        //std::vector<unsigned char> output;
        unsigned error = lodepng::encode(filename, im.data.data, im.width, im.height, lodepng_color_type);
        if ( error )
        {
            std::cerr<<"Error Loading png file "<<filename<<std::endl;
            std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
            exit(1);
        }
    }

    void convert(image_raw const& in, grid_2D<vec3>& out)
    {
        size_t const N = size_t(in.width) * size_t(in.height);
        out.resize(in.width, in.height);
        if (in.color_type == image_color_type::rgb)
        {
            for(size_t k=0; k<N; ++k)
                out.data[k] = vec3(in.data[3*k+0], in.data[3*k+1], in.data[3*k+2])/255.0f;
        }
        else if (in.color_type == image_color_type::rgba)
        {
            for(size_t k=0; k<N; ++k)
                out.data[k] = vec3(in.data[4*k+0], in.data[4*k+1], in.data[4*k+2])/255.0f;
        }
    }

    image_raw image_load_jpg(std::string const& filename)
    {
        int width = 0;
        int height = 0;
        int actual_comps = 0;

        unsigned char* p = jpgd::decompress_jpeg_image_from_file(filename.c_str(), &width, &height, &actual_comps, 3);

        image_raw im;
        im.color_type = image_color_type::rgb;
        im.width = width;
        im.height = height;

        int const N = im.width * im.height * 3;
        im.data.resize(N);
        for (int k = 0; k < N; ++k)
            im.data[k] = p[k];

        return im;
    }

    void image_save_jpg(std::string const& filename, image_raw const& im)
    {
        bool res = jpge::compress_image_to_jpeg_file( filename.c_str(), im.width, im.height, 3, ptr(im.data));
        assert_cgp(res == 1, "Failed to save jpg image " + filename);
    }


    image_raw image_load_file(std::string const& filename)
    {
        size_t const N = filename.size();
        if (N > 4) {
            std::string const ext_4 = filename.substr(N - 4, 4);
            if (ext_4 == ".png")
                return image_load_png(filename);
            else if (ext_4 == ".jpg")
                return image_load_jpg(filename);
        }
        if (N > 5) {
            std::string const ext_5 = filename.substr(N - 5, 5);
            if (ext_5 == ".jpeg")
                return image_load_jpg(filename);
        }

        error_cgp("Error image_load_file(" + filename + "), could not detect a valid extension while expecting .jpg or .png file\n");
        
    }


}