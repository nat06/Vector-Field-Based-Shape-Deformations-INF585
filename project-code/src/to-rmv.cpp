class Weighted_laplacian {
    // Some internal buffer for the computation
    std::vector<Vec3> _buffer_vertices;
    // Cache the cotangent weights
    std::vector< std::vector<float> > _cotan_weights;
    // Store the list of first ring neighbor for each vertex.
    // the first ring neighbor is the list of vertices directly adjacent to another vertex
    const Mesh_topology* _topo;
public:
    /// @note avoid odd numbers of iterations they imply a full copy of
    /// the vertex buffer at the end of the smoothing process.
    int _nb_iter;
 
    /// If the **rest** position of the mesh changes you need to update the
    /// pre-computation step.
    void update(const Mesh_topology& rest_pose_mesh)
    {
        _topo = &topo;
        mesh_utils::laplacian_cotan_weights(topo, _cotan_weights);
    }
 
    // -------------------------------------------------------------------------
 
    /// @param vertices : in place smoothing of this vertex buffer
    void process(Vec3 vertices[],
                 float smooth_factors[],
                 int nb_vertices)
    {       
        _buffer_vertices.resize( nb_vertices );
         
        if(_nb_iter == 0){
            return;
        }
         
        // first_ring[a_vertex][ith_neighbor of a_vertex] =
        // "index of a neighbor directly connected to a_vertex"
        const std::vector< std::vector< mesh_t::Vert_idx > >& first_ring = _topo->first_ring();
        Vec3* src_vertices = vertices;
        Vec3* dst_vertices = _buffer_vertices.data();
        for(int k = 0; k < _nb_iter; k++)
        {             
            for( int i = 0; i < nb_vertices; i++)
            {
                Vec3 cog(0.f);
                float sum = 0.f;
                size_t nb_neighs = first_ring[i].size();
                for(size_t n = 0; n < nb_neighs; n++)
                {
                    mesh_t::Vert_idx neigh = first_ring[i][n];
                    // Note: if we set w = 1.0f then the procedures operates
                    // an uniform smoothing, the initial distribution of
                    // the triangles will not be preserved
                    float w = _cotan_weights[i][n];
                    cog += src_vertices[neigh] * w;
                    sum += w;
                }
                // When using cotan weights smoothing may be unstable
                // in this case we need to set t < 1
                // sometimes you even need to get as low as t < 0.5
                float t = smooth_factors[i];               
                dst_vertices[i] = (cog / sum) * t + src_vertices[i] * (1.f - t);
            }
             
            std::swap(dst_vertices, src_vertices);
        }
         
        if(_nb_iter%2 == 1)           
            utils::copy( vertices/*destination*/, _buffer_vertices.data()/*source*/, nb_vertices );
    }
};
