#include <map>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include <pybind11/pybind11.h>

typedef std::function< pybind11::module & (std::string const &) > ModuleGetter;

void bind_std_exception(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_locale_classes(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_ios_base(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_ostream_tcc(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_fstream_tcc(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_2(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_3(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_4(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_vector_5(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_sstream_tcc(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_msgpack_v1_detail_cpp11_zone(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_msgpack_object(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_msgpack_v1_object_fwd(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_msgpack_v2_object_fwd(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_std_stl_map(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_1(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_2(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_3(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_4(std::function< pybind11::module &(std::string const &namespace_) > &M);
void bind_unknown_unknown_5(std::function< pybind11::module &(std::string const &namespace_) > &M);


PYBIND11_MODULE(mmtf_cpp, root_module) {
	root_module.doc() = "mmtf_cpp module";

	std::map <std::string, pybind11::module> modules;
	ModuleGetter M = [&](std::string const &namespace_) -> pybind11::module & {
		auto it = modules.find(namespace_);
		if( it == modules.end() ) throw std::runtime_error("Attempt to access pybind11::module for namespace " + namespace_ + " before it was created!!!");
		return it->second;
	};

	modules[""] = root_module;

	std::vector< std::pair<std::string, std::string> > sub_modules {
		{"", "mmtf"},
		{"", "msgpack"},
		{"msgpack", "v1"},
		{"msgpack", "v2"},
		{"", "std"},
	};
	for(auto &p : sub_modules ) modules[p.first.size() ? p.first+"::"+p.second : p.second] = modules[p.first].def_submodule(p.second.c_str(), ("Bindings for " + p.first + "::" + p.second + " namespace").c_str() );

	//pybind11::class_<std::shared_ptr<void>>(M(""), "_encapsulated_data_");

	bind_std_exception(M);
	bind_std_locale_classes(M);
	bind_std_ios_base(M);
	bind_std_ostream_tcc(M);
	bind_std_fstream_tcc(M);
	bind_std_stl_vector(M);
	bind_std_stl_vector_1(M);
	bind_std_stl_vector_2(M);
	bind_std_stl_vector_3(M);
	bind_std_stl_vector_4(M);
	bind_std_stl_vector_5(M);
	bind_std_sstream_tcc(M);
	bind_msgpack_v1_detail_cpp11_zone(M);
	bind_msgpack_object(M);
	bind_msgpack_v1_object_fwd(M);
	bind_msgpack_v2_object_fwd(M);
	bind_std_stl_map(M);
	bind_unknown_unknown(M);
	bind_unknown_unknown_1(M);
	bind_unknown_unknown_2(M);
	bind_unknown_unknown_3(M);
	bind_unknown_unknown_4(M);
	bind_unknown_unknown_5(M);

}