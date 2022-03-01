#include "ffd.hpp"
#define SOLUTION


using namespace cgp;

// Helper to compute permutation(n, k) - k among n
//   avoids the recursive formula (too costly)
int binomial_coeff(int n, int k)
{
    int res = 1;
    if(k>n-k)
        k = n - k;
    for(int i=0; i<k; ++i) {
        res *= (n-i);
        res /= (i+1);
    }
    return res;
}


#ifdef SOLUTION
float bezier_coeff(const float t, int k, int N)
{
    float const Nk = float(binomial_coeff(N,k));
    return Nk * float(std::pow(t,k)) * float(std::pow(1-t, N-k));
}


void ffd_deform(buffer<vec3>& position, grid_3D<vec3> const& grid, buffer<grid_3D<float> > const& weights)
{
	int const Nx = grid.dimension.x;
	int const Ny = grid.dimension.y;
	int const Nz = grid.dimension.z;

	int const N_vertex = position.size();
	for (int k = 0; k < N_vertex; ++k)
	{
		vec3 p;
		for (int kx = 0; kx < Nx; ++kx) {
			for (int ky = 0; ky < Ny; ++ky) {
				for (int kz = 0; kz < Nz; ++kz) {
					p += weights[k](kx,ky,kz) * grid(kx,ky,kz);
				}
			}
		}
		
		position[k] = p;
	}

}

buffer<grid_3D<float> > precompute_weights(buffer<vec3>& position, int Nx, int Ny, int Nz)
{
	buffer<grid_3D<float>> weights;
	weights.resize(position.size());

	for(int k=0; k<weights.size(); ++k){
		vec3 const& p = position[k];
		weights[k].resize(Nx,Ny,Nz);
		for (int kx = 0; kx < Nx; ++kx) {
			for (int ky = 0; ky < Ny; ++ky) {
				for (int kz = 0; kz < Nz; ++kz) {
					
					float const alpha_x = bezier_coeff(p.x, kx, Nx-1 );
					float const alpha_y = bezier_coeff(p.y, ky, Ny-1 );
					float const alpha_z = bezier_coeff(p.z, kz, Nz-1 );

					weights[k](kx,ky,kz) = alpha_x * alpha_y * alpha_z;
				}
			}
		}
	}

	return weights;
}
#else

// Precompute the FFD elements that depends only on the initial position of the shape (its barycentric coordinates in the grid)
buffer<grid_3D<float> > precompute_weights(buffer<vec3>& position, int Nx, int Ny, int Nz)
{
	buffer<grid_3D<float>> weights;
	weights.resize(position.size());

	// TO DO: compute the weights such that
	//   weights[k](kx,ky,kz) = b_kx(x_k) * b_ky(y_k) * b_zk(z_k)
	//    with (x_k,y_k,z_k) <= position[k]
	//         (kx,ky,kz) varying on the grid dimension
	//         b_kx, b_ky, b_kz being the Bezier basis functions


	return weights;
}


// Computation of the FFD deformation on the position with respect to the grid
void ffd_deform(buffer<vec3>& position, grid_3D<vec3> const& grid, buffer<grid_3D<float> > const& weights)
{
	int const Nx = grid.dimension.x;
	int const Ny = grid.dimension.y;
	int const Nz = grid.dimension.z;

	int const N_vertex = position.size();

	// TO DO: compute the deformed position
	// General formulation:
	// For all position k of the shape to be deformed
	//     position[k] = sum_{x,y,z} weights[k] * grid(x,y,z)

}
#endif