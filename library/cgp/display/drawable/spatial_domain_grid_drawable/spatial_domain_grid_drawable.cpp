#include "spatial_domain_grid_drawable.hpp"
#include "cgp/base/base.hpp"


namespace cgp
{
	segments_drawable spatial_domain_grid_drawable(spatial_domain_grid_3D const& domain, GLuint shader)
	{
		vec3 const& u = domain.length;
		vec3 const p000 = domain.corner_min();
		vec3 const p001 = p000 + vec3(0, 0, 1) * u;
		vec3 const p010 = p000 + vec3(0, 1, 0) * u;
		vec3 const p011 = p000 + vec3(0, 1, 1) * u;
		vec3 const p100 = p000 + vec3(1, 0, 0) * u;
		vec3 const p101 = p000 + vec3(1, 0, 1) * u;
		vec3 const p110 = p000 + vec3(1, 1, 0) * u;
		vec3 const p111 = p000 + vec3(1, 1, 1) * u;

		buffer<vec3> box;
		box.push_back(p000).push_back(p001);
		box.push_back(p001).push_back(p011);
		box.push_back(p011).push_back(p010);
		box.push_back(p010).push_back(p000);

		box.push_back(p100).push_back(p101);
		box.push_back(p101).push_back(p111);
		box.push_back(p111).push_back(p110);
		box.push_back(p110).push_back(p100);

		box.push_back(p000).push_back(p100);
		box.push_back(p001).push_back(p101);
		box.push_back(p011).push_back(p111);
		box.push_back(p010).push_back(p110);

		segments_drawable drawable; 
		drawable.initialize(box, "Spatial Domain", shader);
		return drawable;		
	}

	segments_drawable spatial_domain_grid_drawable_voxels(spatial_domain_grid_3D const& domain, GLuint shader)
	{
		buffer<vec3> g;

		vec3 const p0 = domain.corner_min();
		vec3 const& L = domain.length;
		vec3 const dL = domain.voxel_length();
		int3 const& s = domain.samples;

		for (int kx = 0; kx < s.x; ++kx) {
			for (int ky = 0; ky < s.y; ++ky) {
				vec3 const p = p0 + dL * vec3(kx, ky, 0);
				g.push_back(p).push_back(p + L * vec3(0, 0, 1));
			}
		}

		for (int kx = 0; kx < s.x; ++kx) {
			for (int kz = 0; kz < s.z; ++kz) {
				vec3 const p = p0 + dL * vec3(kx, 0, kz);
				g.push_back(p).push_back(p + L * vec3(0, 1, 0));
			}
		}

		for (int ky = 0; ky < s.y; ++ky) {
			for (int kz = 0; kz < s.z; ++kz) {
				vec3 const p = p0 + dL * vec3(0, ky, kz);
				g.push_back(p).push_back(p + L * vec3(1, 0, 0));
			}
		}


		segments_drawable drawable;
		drawable.initialize(g, "Spatial Domain Voxels", shader);
		return drawable;
	}



}