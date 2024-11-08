#include "Socket.class.hpp"
#include "utils.hpp"

Socket::Socket(): _fd(-1), _addr(), _sizeaddr(sizeof(_addr)), _server(){}


Socket::Socket(Socket const &copy): _server(copy._server)
{
	*this = copy;
}

Socket::~Socket(){};

Socket &Socket::operator=(Socket const &copy)
{
	if (this != &copy)
	{
		_fd = copy._fd;
		_addr = copy._addr;
		_sizeaddr = copy._sizeaddr;
	}
	return *this;
}

Socket::Socket(Server const &server):_server(server)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
	{
		protected_write_log(error_message_server(server,
					std::string("ERROR: socket() ") + std::strerror(errno)));
		return;
	}
	const int tmp = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0)
	{
		protected_write_log(error_message_server(server,
					std::string("ERROR: setsockopt() ") + std::strerror(errno)));
		close(_fd);
		_fd = -1;
		return;
	}
	_sizeaddr = sizeof(_addr);
	memset(&_addr, 0, _sizeaddr);
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(server.getHostUint());
	_addr.sin_port = htons(server.getPort());
	if (bind(_fd, (const sockaddr*)&_addr, _sizeaddr) == -1)
	{
		protected_write_log(error_message_server(server,
					std::string("ERROR: bind() ") + std::strerror(errno)));
		close(_fd);
		_fd = -1;
		return;
	}
	if (listen(_fd, NB_QUEUE_PORT) == -1)
	{
		protected_write_log(error_message_server(server,
					std::string("ERROR: listen() ") + std::strerror(errno)));
		close(_fd);
		_fd = -1;
		return;
	}
}

Socket::Socket(Server const &server, Socket const & socket): _fd(socket._fd),
			_addr(socket._addr), _sizeaddr(socket._sizeaddr), _server(server) {}


int const & Socket::getFd() const
{
	return _fd;
}

struct sockaddr_in const & Socket::getAddress() const
{
	return _addr;
}

socklen_t const & Socket::getSizeAddress() const
{
	return _sizeaddr;
}

Server const & Socket::getServer() const
{
	return _server;
}
