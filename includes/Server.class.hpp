#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.class.hpp"
#include "CgiLocation.class.hpp"
#include <stdint.h>
#include <map>

#define ONE_MEGABIT 1048576

#define WARNING_MSG_DUPLICATE_LOC "WARNING: Server configuration: duplicated location ignored"
#define WARNING_MSG_DUPLICATE_SERV "WARNING: Server configuration: duplicated server ignored"

class Server
{
	public:
		Server();
		std::string const &							getHost() const;
		uint32_t const &							getHostUint() const;
		t_port const &								getPort() const;
		std::vector<std::string> const &			getServerName() const;
		std::vector<std::string> &					getServerName();
		std::map<t_http_code, std::string> const &	getErrorPagePath() const;
		std::map<t_http_code, std::string> &		getErrorPagePath();
		uint64_t const &							getClientMaxBodySize() const;
		std::vector<Location> const &				getLocations() const;
		std::vector<CgiLocation> const &			getCgiLocations() const;
		Location &									getDefaultLocation();
		Location const &							getDefaultLocation() const;
		bool const &								getHasListen() const;

		void	setHost(std::string host);
		void	setHostUint(uint32_t host_uint);
		void	setServerName(std::vector<std::string> server_names);
		void	addServerName(std::string server_name);
		void	setPort(t_port port);
		void	setErrorPagePath(std::map<t_http_code, std::string> error_page_path);
		void	addErrorPagePath(t_http_code http_code, std::string path);
		void	setClientMaxBodySize(uint64_t client_max_body_size);
		void	setLocations(std::vector<Location> locations);
		void	addLocations(Location location);
		void	addCgiLocation(CgiLocation cgi_loc);
		void	setHasListen(bool has_listen);

		void	removeDuplicatedLocation();
		bool	isEqual(Server const &ref) const;

		Location const &searchLocation(std::string path) const;
		bool searchCgiLocation(std::string path, CgiLocation &cgi_loc) const;

		std::string getErrorPagePath(t_http_code error_code) const;

	private:
		std::string							_host;
		uint32_t							_host_uint;
		t_port								_port;
		std::vector<std::string>			_server_name;
		std::map<t_http_code, std::string>	_error_page_path;
		uint64_t							_client_max_body_size;
		std::vector<Location>				_locations;
		std::vector<CgiLocation>			_cgi_locations;
		bool								__has_listen;
};

#endif