#include "DefineVariableFields.hpp"

t_token_append_function define_token_var_function(std::string &token)
{
	// find function that matches token
	if (token == "listen")
		return fill_listen;
	if (token == "server_name")
		return fill_server_name;
	if (token == "error_page")
		return fill_error_page;
	if (token == "client_max_body_size")
		return fill_client_max_body_size;
	if (token == "root")
		return fill_root_path;
	if (token == "autoindex")
		return fill_autoindex;
	// at this point, these fields can only be declare in a location
	if (token == "limit_except")
		return fill_limit_except;
	if (token == "return")
		return fill_redirect;
	if (token == "index")
		return fill_default_dir_file;
	if (token == "can_upload")
		return fill_can_upload;
	ThrowMisc("unknown field `" + token + "`");
}

std::string func_ptr_to_str(t_token_append_function ptr)
{
	// find function that matches token
	if (ptr == fill_listen)
		return "listen";
	if (ptr == fill_server_name)
		return "server_name";
	if (ptr == fill_error_page)
		return "error_page";
	if (ptr == fill_client_max_body_size)
		return "client_max_body_size";
	if (ptr == fill_root_path)
		return "root";
	if (ptr == fill_autoindex)
		return "autoindex";
	// at this point, these fields can only be declare in a location
	if (ptr == fill_limit_except)
		return "limit_except";
	if (ptr == fill_redirect)
		return "return";
	if (ptr == fill_default_dir_file)
		return "index";
	if (ptr == fill_can_upload)
		return "can_upload";
	return "";
}
