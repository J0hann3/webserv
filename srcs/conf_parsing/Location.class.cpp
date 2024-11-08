#include "Location.class.hpp"

Location::Location()
{
	_location_path = "";
	for (it_method it = g_http_methods.begin(); it != g_http_methods.end(); ++it)
		_accepted_methods.push_back(*it);
	_root_path = "/tmp/webserv/";
	_has_redirect = false;
	_redirect = std::pair<int, std::string>();
	_default_index_path.clear();
	_has_autoindex = false;
	_can_upload = false;
}

Location::Location(Location const &ref)
{
	*this = ref;
}

Location &Location::operator=(Location const &ref)
{
	if (ref == *this && ref._location_path == _location_path)
		return *this;
	_location_path = ref._location_path;
	_accepted_methods = ref._accepted_methods;
	_root_path = ref._root_path;
	_has_redirect = ref._has_redirect;
	_redirect = ref._redirect;
	_default_index_path = ref._default_index_path;
	_has_autoindex = ref._has_autoindex;
	_can_upload = ref._can_upload;
	return *this;
}

// do not check location path
bool	Location::operator==(Location const &rref) const
{
	if (this->_accepted_methods == rref._accepted_methods
		&& _root_path == rref._root_path
		&& this->_has_redirect == rref._has_redirect
		&& this->_redirect == rref._redirect
		&& this->_default_index_path == rref._default_index_path
		&& this->_has_autoindex == rref._has_autoindex
		&& this->_can_upload == rref._can_upload)
		return true;
	return false;
}

std::string const &							Location::getLocationPath() const {return this->_location_path;}
std::vector<std::string> const &			Location::getMethods() const {return this->_accepted_methods;}
std::vector<std::string> &					Location::getMethods() {return this->_accepted_methods;}
std::string const &							Location::getRootPath() const {return this->_root_path;}
bool const &								Location::getHasRedirect() const {return this->_has_redirect;}
std::pair<t_http_code, std::string> const &	Location::getRedirect() const {return this->_redirect;}
std::pair<t_http_code, std::string> &		Location::getRedirect() {return this->_redirect;}
std::vector<std::string> const &			Location::getDefaultIndexPath() const {return this->_default_index_path;}
bool const &								Location::getHasAutoindex() const {return this->_has_autoindex;}
bool const &								Location::getCanUpload() const {return this->_can_upload;}

void	Location::setLocationPath(std::string location_path) {this->_location_path = location_path;}
void	Location::setMethods(std::vector<std::string> methods) {this->_accepted_methods = methods;}
void	Location::addMethods(std::string method) {this->_accepted_methods.push_back(method);}
void	Location::setRootPath(std::string root_path) {this->_root_path = root_path;}
void	Location::setHasRedirect(bool has_redirect) {this->_has_redirect = has_redirect;}
void	Location::setRedirect(std::pair<t_http_code, std::string> redirect) {this->_redirect = redirect;}
void	Location::setDefaultIndexPath(std::vector<std::string> default_path) {this->_default_index_path = default_path;}
void	Location::addDefaultIndexPath(std::string default_path) {this->_default_index_path.push_back(default_path);}
void	Location::setHasAutoindex(bool has_auto_index){this->_has_autoindex = has_auto_index;}
void	Location::setCanUpload(bool can_upload) {this->_can_upload = can_upload;}

bool	Location::isEmptyLocation(Location &default_location) const
{
	if (*this == default_location)
		return true;
	return false;
}

bool	Location::doesAcceptMethod(std::string method) const
{
	if (std::find(this->getMethods().begin(), this->getMethods().end(), method) != this->getMethods().end())
		return true;
	return false;
}

bool	Location::hasDefaultIndex( void ) const
{
	return(_default_index_path.size() != 0);
}


e_status	Location::updateUriToIndex(std::string & uri) const
{
	std::vector<std::string> index = getDefaultIndexPath();
	for (unsigned int i = 0; i < index.size(); i++)
	{
		if (access((uri + index.at(i)).c_str(), F_OK) != -1)
		{
			uri = uri + index.at(i);
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

