#ifndef CLUSTER_CLIENT_HPP
# define CLUSTER_CLIENT_HPP

# include "shared.hpp"
# include "Socket.class.hpp"
# include "HttpExchange.class.hpp"

# include <sys/time.h>
# include <vector>
# include <algorithm>


typedef std::vector<Socket>::const_iterator	t_const_iter_sockets;
typedef std::vector<Socket>::iterator		t_iter_sockets;
typedef std::vector<Socket>					t_sockets;

typedef std::map<int, HttpExchange>::const_iterator	t_const_iter_map_read;
typedef std::map<int, HttpExchange>::iterator		t_iter_map_read;
typedef std::map<int, HttpExchange>					t_map_read;

class Cluster
{
	private:
		// std::vector<Server>	_all_server;
		std::vector<Socket>				_sockets; //fd that are listening
		std::map<int, HttpExchange>		_map_sockets; //fd create with accept => read
		std::vector<int>				_fd_write; //fd create with accept => write
		int								_max_fd;
	public:
		// Cluster();
		Cluster(std::vector<Server>);
		// Cluster(Cluster const &copy);
		// ~Cluster();
		Socket const * find_same_config_server(Server const &server) const;
		void addServer(Server const & server);
		void init_set_fds(fd_set *readfds, fd_set *writefds, fd_set *exceptfds) const;
		void runServer();
		void print_set(fd_set *fds, std::string str) const;
		void acceptNewConnection(Socket const & socket);
		Socket const *get_matching_socket(int fd, std::string server_name) const;
		void switchHttpExchangeToWrite(int fd);

};

#endif