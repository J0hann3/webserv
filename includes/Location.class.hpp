#ifndef LOCATION_HPP
#define LOCATION_HPP

# include "Shared.hpp"

#include <vector>
#include <utility>
#include <algorithm>

typedef uint16_t t_http_code;
typedef uint16_t t_port;

class Location
{
	public:
		Location();
		Location(Location const &ref);

		Location &operator=(Location const &ref);
		bool	operator==(Location const &rref) const;

		std::string const &							getLocationPath() const;
		std::vector<std::string> const &			getMethods() const;
		std::vector<std::string> &					getMethods();
		std::string const &							getRootPath() const;
		bool const &								getHasRedirect() const;
		std::pair<t_http_code, std::string> const &	getRedirect() const;
		std::pair<t_http_code, std::string> &		getRedirect();
		std::vector<std::string> const &			getDefaultIndexPath() const;
		bool const &								getHasAutoindex() const;
		bool const &								getCanUpload() const;

		void	setLocationPath(std::string location_path);
		void	setMethods(std::vector<std::string> methods);
		void	addMethods(std::string method);
		void	setRootPath(std::string root_path);
		void	setHasRedirect(bool has_redirect);
		void	setRedirect(std::pair<t_http_code, std::string> redirect);
		void	setDefaultIndexPath(std::vector<std::string> default_path);
		void	addDefaultIndexPath(std::string default_path);
		void	setHasAutoindex(bool has_auto_index);
		void	setCanUpload(bool can_upload);

		bool		isEmptyLocation(Location &default_location) const;
		bool		doesAcceptMethod(std::string method) const;
		bool		hasDefaultIndex(void) const;
		e_status	updateUriToIndex(std::string & uri) const;

	private:
		std::string							_location_path;
		std::vector<std::string>			_accepted_methods;
		std::string							_root_path;
		bool								_has_redirect;
		std::pair<t_http_code, std::string>	_redirect;
		std::vector<std::string>			_default_index_path;
		bool								_has_autoindex;
		bool								_can_upload;
};

#endif