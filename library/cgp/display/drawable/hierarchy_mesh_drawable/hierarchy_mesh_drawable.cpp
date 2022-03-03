#include "cgp/base/base.hpp"
#include "hierarchy_mesh_drawable.hpp"

namespace cgp
{

    static void assert_valid_hierarchy(hierarchy_mesh_drawable const& hierarchy);

    void hierarchy_mesh_drawable::add(hierarchy_mesh_drawable_node const& node)
    {
        name_map[node.drawable.name] = static_cast<int>(elements.size());
        elements.push_back(node);
        assert_valid_hierarchy(*this);
    }
    void hierarchy_mesh_drawable::add(mesh_drawable const& element, std::string const& name_parent, vec3 const& translation)
    {
        hierarchy_mesh_drawable_node const node = {element, name_parent, affine_rts(rotation_transform(),translation,1.0f)};
        add(node);
    }
    void hierarchy_mesh_drawable::add(mesh_drawable const& element, std::string const& name_parent, affine_rts const& transform)
    {
        hierarchy_mesh_drawable_node const node = {element, name_parent, transform};
        add(node);
    }

    hierarchy_mesh_drawable_node& hierarchy_mesh_drawable::operator[](const std::string& name)
    {
        auto it = name_map.find(name);
        if(it==name_map.end())
        {
            std::cerr<<"Error: cannot find element ["<<name<<"] in hierarchy_mesh_drawable"<<std::endl;
            std::cerr<<"Possibles element names are: ";
            for( auto const& s : name_map ) { std::cerr<<"["<<s.first<<"] "; }
            abort();
        }
        const size_t index = it->second;
        assert_cgp_no_msg(index<elements.size());

        return elements[index];
    }
    const hierarchy_mesh_drawable_node& hierarchy_mesh_drawable::operator[](const std::string& name) const
    {
        auto it = name_map.find(name);
        if(it==name_map.end())
        {
            std::cerr<<"Error: cannot find element ["<<name<<"] in hierarchy_mesh_drawable"<<std::endl;
            std::cerr<<"Possibles element names are: ";
            for( auto const& s : name_map ) { std::cerr<<"["<<s.first<<"] "; }
            abort();
        }

        const size_t index = it->second;
        assert_cgp_no_msg(index<elements.size());

        return elements[index];
    }


    void hierarchy_mesh_drawable::update_local_to_global_coordinates()
    {
        if(elements.size()==0)
            return ;

        assert_valid_hierarchy(*this);

        std::string const& name_root_parent = elements[0].name_parent;

        const size_t N = static_cast<int>(elements.size());
        for(size_t k=0; k<N; ++k)
        {
            hierarchy_mesh_drawable_node& element = elements[k];

            std::string const parent_name = element.name_parent;

            // Case of root element (or same parent) - local = global
            if( parent_name == name_root_parent ) {
                element.global_transform = element.transform;
            }
            // Else apply hierarchical transformation
            else
            {
                affine_rts const& local = element.transform;
                affine_rts const& global_parent = (*this)[parent_name].global_transform;

                element.global_transform = global_parent * local;
            }
        }
    }


    void assert_valid_hierarchy(hierarchy_mesh_drawable const& hierarchy)
{
    if(hierarchy.elements.size()==0)
        return ;

    {
        // Check that elements and name_map have the same size
        const size_t N1 = hierarchy.elements.size();
        const size_t N2 = hierarchy.name_map.size();
        if(N1!=N2) {
            std::cerr<<"Error: Hierarchy not valid - Number of element ("<<N1<<") != Name_map.size() ("<<N2<<")"<<std::endl;
            abort();
        }
    }

    {
        // Check that the name of the parent of root node doesn't belong to the hierarchy
        const std::string root_name_parent = hierarchy.elements[0].name_parent;
        const auto it = hierarchy.name_map.find(root_name_parent);
        if(it!=hierarchy.name_map.end()) {
            std::cerr<<"Error: Hierarchy not valid - name of the root node ("<<root_name_parent<<") cannot be an element of the hierarchy"<<std::endl;
            abort();
        }
    }

    {
        // Check that all names stored in map are designating the corresponding elements
        for(const auto& e : hierarchy.name_map)
        {
            const std::string& name = e.first;
            const int index = e.second;

            if(index>=int(hierarchy.elements.size()) || index<0)
            {
                std::cerr<<"Error: Hierarchy not valid - Incorrect index stored in name_map: "<<name<<";"<<index<<std::endl;
                abort();
            }

            const hierarchy_mesh_drawable_node& designated_element = hierarchy.elements[index];
            if(designated_element.drawable.name != name)
            {
                std::cerr<<"Error: Hierarchy not valid - Incoherent name between map ("<<name<<";"<<index<<") and element ("<<designated_element.drawable.name<<") "<<std::endl;
                abort();
            }
        }
    }

    {
        // Check that all parents are defined before their children (excepted for root parent)
        std::set<std::string> parent_names_visited;
        parent_names_visited.insert(hierarchy.elements[0].name_parent);
        const size_t N = hierarchy.elements.size();
        for(size_t k=0; k<N; ++k)
        {
            const std::string& name = hierarchy.elements[k].drawable.name;
            const std::string& parent_name = hierarchy.elements[k].name_parent;
            if(parent_names_visited.find(parent_name)==parent_names_visited.end())
            {

                std::cerr<<"Error: Hierarchy not valid"<<std::endl;
                std::cerr<<"Element ("<<name<<","<<k<<") has parent name ("<<parent_name<<") used before being defined"<<std::endl;
                abort();
            }
            else
                parent_names_visited.insert(name);
        }
    }


}

}