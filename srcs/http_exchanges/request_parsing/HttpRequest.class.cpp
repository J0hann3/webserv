#include "HttpRequest.class.hpp"

bool HttpRequest::isAcceptedMethod(Location const & location) const
{
	return (location.doesAcceptMethod(*getMethod()));
}

HttpRequest::HttpRequest ( void ) : HttpRequestLine()
{
}

HttpRequest::HttpRequest ( HttpRequest const & model ) :
	HttpRequestLine(model), _body(model._body), _fields(model._fields)
{
}

HttpRequest & HttpRequest::operator=(HttpRequest const & model)
{
	if (this != &model)
	{
		HttpRequestLine::operator=(model);
		_fields = model._fields;
		_body = model._body;
	}
	return (*this);
}

void	HttpRequest::init( std::stringstream &request_stream ) throw(ExceptionHttpStatusCode)
{
	_fill_request_line_with_stream(request_stream);
	HttpField::fill_fields(request_stream, _fields);
}

bool	HttpRequest::checkFieldExistence(std::string const & field_name) const
{
	return (HttpField::checkFieldExistence(_fields, field_name));
}

std::vector<std::string> const	&HttpRequest::getFieldValue(
	std::string const & field_name) const throw(ExceptionHttpStatusCode)
{
	return (HttpField::getFieldValue(_fields, field_name));
}

HttpRequest::~HttpRequest( void )
{
}

void	HttpRequest::displayRequest( void ) const
{

	protected_write_log("------------ HttpRequest ------------");
	displayRequest_line();
	for (std::vector<HttpField>::const_iterator it = _fields.begin();
			it != _fields.end(); ++it)
		it->display_field();
	if (!_body.empty() && g_err_log_fd)
	{
		write(g_err_log_fd, "\r\n", 2);
		write(g_err_log_fd, _body.c_str(), _body.size());
	}
	protected_write_log("");

}


std::string const &	HttpRequest::getBody( void ) const
{
	return (_body);
}

void	HttpRequest::addStringToBody( std::string const & extra_body_content)
{
	_body += extra_body_content;
}

std::string	HttpRequest::getUri(std::string root)
{
	std::string uri = root + getTarget();
	size_t pos = uri.find_first_of('?');
	if (pos != std::string::npos)
		uri = uri.substr(0, pos);
	return (uri);
}
