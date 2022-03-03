#include "texture.hpp"

#include "cgp/base/base.hpp"

namespace cgp
{
    GLuint opengl_load_texture_image(std::string const& filename, GLint wrap_s, GLint wrap_t)
    {
        image_raw const im = image_load_file(filename);
        return opengl_load_texture_image(im, wrap_s, wrap_t);
    }

    GLuint opengl_load_texture_image(image_raw const& im, GLint wrap_s, GLint wrap_t)
    {
        GLuint id = 0;
        glGenTextures(1,&id); opengl_check;
        glBindTexture(GL_TEXTURE_2D,id); opengl_check;

        // Send texture on GPU
        if(im.color_type==image_color_type::rgba){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, im.width, im.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr(im.data)); opengl_check;
        }
        if(im.color_type==image_color_type::rgb){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, im.width, im.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(im.data)); opengl_check;
        }
        glGenerateMipmap(GL_TEXTURE_2D); opengl_check;

        // Set default texture behavior
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s); opengl_check;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t); opengl_check;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); opengl_check;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); opengl_check;

        glBindTexture(GL_TEXTURE_2D,0); opengl_check;

        return id;
    }
    GLuint opengl_load_texture_image(grid_2D<vec3> const& im, GLint wrap_s, GLint wrap_t)
    {
        GLuint id = 0;
        glGenTextures(1,&id); opengl_check;
        glBindTexture(GL_TEXTURE_2D,id); opengl_check;

        // Send texture on GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GLsizei(im.dimension.x), GLsizei(im.dimension.y), 0, GL_RGB, GL_FLOAT, ptr(im.data)); opengl_check;
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set default texture behavior
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glBindTexture(GL_TEXTURE_2D,0);

        return id;
    }
    void opengl_update_texture_image(GLuint texture_id, grid_2D<vec3> const& im)
    {
        assert_cgp(glIsTexture(texture_id), "Incorrect texture id");

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, GLsizei(im.dimension.x), GLsizei(im.dimension.y), GL_RGB, GL_FLOAT, ptr(im.data));
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
    }

}