#include "HttpRequestGet.class.hpp"
#include <sys/stat.h>
#include <fcntl.h>

HttpRequestGet::HttpRequestGet (std::string const & str_request)
	throw (ExceptionHttpStatusCode)
	: HttpRequest()
{
	std::stringstream	stream_request (str_request);

	if (stream_request.bad())
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPGET_SSTREAM_FAIL);
	HttpRequest::init(stream_request);
	if (*getMethod() != "GET")
		throw_http_err_with_log(HTTP_500, MSG_ERR_HTTPGET_WRONG_METHOD);
}

HttpRequestGet::HttpRequestGet ( HttpRequestGet const & model)
	: HttpRequest(model)
{
}

HttpRequestGet & HttpRequestGet::operator= (HttpRequestGet const & model)
{
	if (&model != this)
		HttpRequest::operator=(model);
	return (*this);
}

HttpRequestGet::HttpRequestGet( void ) //unused
{
}

HttpRequestGet::~HttpRequestGet( void )
{
}

void			HttpRequestGet::process_header( Socket const * const socket )
{
	(void)socket;

	//TODO
}

bool	HttpRequestGet::hasBody() const
{
	return (false);
}

std::string	getUri(std::string root, std::string target)
{
	return (root + target);
}

bool	handle_directory(std::string & uri, Location const & location, HttpResponse & response, Server const & server)
{
	//check if uri is a directory
	int fd = open(uri.c_str(), O_DIRECTORY);
	if (fd == -1)
		return false;
	std::cout << "is directory\n";
	close(fd);
	if (location.getHasAutoindex())
	{
		//add content-length flags
		//fill body auto index
		return true; //TODO
	}
	std::vector<std::string> index = location.getDefaultDirPath();
	for (unsigned int i = 0; i < index.size(); i++)
	{
		if (access((uri + index.at(i)).c_str(), F_OK) != -1)
		{
			uri = uri + index.at(i);
			return false;
		}
	}
	response.generateErrorResponse(HTTP_403, server);
	return true;
}

void	handle_file(std::string & uri, HttpResponse & response, Server const & server)
{
	if (response.openFstream(uri) == FAILURE)
	{
		response.generateErrorResponse(HTTP_403, server); //! not sure
		return ;
	}
}

void	HttpRequestGet::_initResponse( Socket const * const socket, HttpResponse &response )
{
	response.setVersion(getVersion());

	Location location = socket->getServer().searchLocation(getTarget());// get location path
	//get location cgi if cgi
	if (response.handle_redirect(location))
	{
		std::cout << "redirection\n";
		return ;
	}
	if (checkMethod(location) == false)
	{
		std::cout << "wrong method\n";
		response.addAllowMethod(location.getMethods());
		response.generateErrorResponse(HTTP_405, socket->getServer());//!send error to client with allow method
		return ;
	}
	std::string uri = getUri(location.getRootPath(), getTarget());
	if (handle_directory(uri, location, response, socket->getServer()))
	{
		std::cout << "return\n";
		return ;
	}
	handle_file(uri, response, socket->getServer());
	std::cout << "return file\n";
	return ;
}

void	HttpRequestGet::generate_response( Socket const * const socket, HttpResponse &response )
{
	_initResponse(socket, response);
	response.fillHeader();
	std::cout << "response\n";
}

void	HttpRequestGet::readBody(int fd, Socket const * const socket)
{
	(void)socket;
	(void)fd;
}