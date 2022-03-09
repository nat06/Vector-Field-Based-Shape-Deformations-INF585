#include "cgp/base/base.hpp"
#include "cgp/display/opengl/opengl.hpp"

#include "shading_parameters.hpp"



namespace cgp
{
	void opengl_uniform(GLuint shader, shading_parameters_phong const& shading, bool expected)
	{
		opengl_uniform(shader, "color", shading.color, expected);
		opengl_uniform(shader, "alpha", shading.alpha, expected);
		opengl_uniform(shader, "Ka", shading.phong.ambient, expected);
		opengl_uniform(shader, "Kd", shading.phong.diffuse, expected);
		opengl_uniform(shader, "Ks", shading.phong.specular, expected);
		opengl_uniform(shader, "specular_exp", shading.phong.specular_exponent, expected);
		opengl_uniform(shader, "use_texture", shading.use_texture, false);
		opengl_uniform(shader, "texture_inverse_y", shading.texture_inverse_y, false);
	}
}