#include "ConfParser.hpp"

void	fill_field_value(std::string &token, Server &server, Location *location, t_token_append_function &token_var_function, unsigned int arg_counter)
{
	if (token_var_function == NULL)
		token_var_function = define_token_var_function(token);
	else
		token_var_function(token, server, location, arg_counter);
}

void	interpret_field_loop(std::string &token, std::queue<std::string> &tokens, Server &server, Location *location)
{
	t_token_append_function	token_identifier = NULL;
	unsigned int arg_counter = 0;

	while (!tokens.empty())
	{
		if (token[0] == '{' || token[0] == '}')
			continue;
		else if (token[0] == ';')
		{
			if (arg_counter == 1)
				ThrowMisc("missing argument for `" + func_ptr_to_str(token_identifier) + "`");
			break;
		}
		else
			fill_field_value(token, server, location, token_identifier, arg_counter);
		arg_counter++;
		token = extract_token(tokens);
	}
	void fill_error_page(std::string &token, Server &server, Location *location, unsigned int arg_counter);
	std::string		ft_itoa( uint64_t number );
	if (token_identifier == fill_error_page)
	{
		for (std::map<t_http_code, std::string>::iterator it = server.getErrorPagePath().begin(); it != server.getErrorPagePath().end(); it++)
		{
			if (it->second == "") // fill the second part of the map for each error code
				ThrowMisc(std::string("missing error path for error code `") + ft_itoa(it->first) + "' in field `error_page'");
		}
	}
}

void interpret_cgi_field_loop(std::string &token, std::queue<std::string> &tokens, CgiLocation &cgiObj)
{
	unsigned int arg_counter = 0;
	std::string identifier = "";

	(void)token;
	(void)tokens;
	(void)cgiObj;

	while (!tokens.empty())
	{
		if (token[0] == '{')
			continue ;
		else if (token[0] == '}')
			break ;
		else if (token[0] == ';')
		{
			if (arg_counter == 1)
				ThrowBadArgumentNumber(identifier, 1, false);
			break;
		}
		else
		{
			if (arg_counter == 0)
			{
				if (token != "cgi_path" && token != "path_info")
				{
					define_token_var_function(token); // will throw a unknown field, except if the token exists but is in the wrong location
					ThrowBadFieldLocation("cgi location", token);
				}
				identifier = token;
			}
			else if (arg_counter == 1)
			{
				if (identifier == "cgi_path")
					cgiObj.setExecPath(token);
				else if (identifier == "path_info")
				{
					if (token[0] != '/')
						ThrowMisc("field `path_info' must be an abolute path");
					cgiObj.setRootPath(token);
				}
			}
			else
				ThrowBadArgumentNumber(identifier, 1, true);
		};
		arg_counter++;
		token = extract_token(tokens);
	}
}

void	interpret_location_loop(std::queue<std::string> &tokens, Server &server)
{
	bool is_cgi_loc = false;
	Location location(server.getDefaultLocation());
	CgiLocation cgi_loc;

	std::string token = extract_token(tokens);
	location.setLocationPath(token);
	if (token[0] == '~') // can be a cgi location
	{
		token = extract_token(tokens);
		if (token[0] != '{') // is for sure a cgi location
			is_cgi_loc = true;
		cgi_loc.setExtension(token);
	}
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
		{
			if (is_cgi_loc)
			{
				cgi_loc.check_validity(); // can throw
				server.addCgiLocation(cgi_loc);
			}
			else if (!location.isEmptyLocation(server.getDefaultLocation()))
			{
				if (location.getDefaultIndexPath().empty())
					location.addDefaultIndexPath("index.html");
				server.addLocations(location);
			}
			break;
		}
		else
		{
			if (is_cgi_loc)
				interpret_cgi_field_loop(token, tokens, cgi_loc);
			else
				interpret_field_loop(token, tokens, server, &location);
		}
	}
}

// tokens is a copy
void	interpret_server_fields(Server &server, std::queue<std::string> tokens)
{
	bool	skipping_location = false;
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
		{
			if (skipping_location)
			{
				skipping_location = false;
				continue ;
			}
			break ;
		}
		else if (skipping_location)
			continue ;
		else if (token == "location")
			skipping_location = true;
		else
			interpret_field_loop(token, tokens, server, NULL);
	}
}

// tokens is a reference
void	interpret_location_fields(Server &server, std::queue<std::string> &tokens)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		if (token[0] == '{' || token[0] == ';')
			continue;
		else if (token[0] == '}')
			break ;
		else if (token == "location")
			interpret_location_loop(tokens, server);
	}
}


Server	interpret_server_loop(std::queue<std::string> &tokens)
{
	Server server;
	Location default_location;
	
	server.addLocations(default_location);

	interpret_server_fields(server, tokens);
	interpret_location_fields(server, tokens);
	if (server.getHost().empty())
		ThrowMisc("missing `listen` field");
	if (server.getDefaultLocation().getDefaultIndexPath().empty())
		server.getDefaultLocation().addDefaultIndexPath("index.html");
	server.removeDuplicatedLocation();
	return server;
}

bool	_remove_duplicated_server_names(Server &server, Server &new_serv)
{
	for (std::vector<std::string>::iterator it_serv = server.getServerName().begin(); it_serv != server.getServerName().end(); it_serv++)
	{
		for (std::vector<std::string>::iterator it_new = new_serv.getServerName().begin(); it_new != new_serv.getServerName().end(); )
		{
			if (*it_serv == *it_new)
			{
				it_new = new_serv.getServerName().erase(it_new);
			}
			else
				it_new++;
		}
	}
	if (new_serv.getServerName().empty())
		return true;
	return false;
}

void	_check_server_is_unique(std::vector<Server> &servers)
{
	Server &new_server = servers.back();

	for (std::vector<Server>::iterator it_serv = servers.begin(); it_serv != servers.end() - 1; it_serv++)
	{
		if (!new_server.isEqual(*it_serv))
			continue;
		if (new_server.getServerName() == it_serv->getServerName() || new_server.getServerName().size() == 0)
		{
			servers.erase(servers.end() - 1);
			protected_write_log(WARNING_MSG_DUPLICATE_SERV);
			return ;
		}
		if (it_serv->getServerName().size() == 0)
		{
			servers.erase(it_serv);
			protected_write_log(WARNING_MSG_DUPLICATE_SERV);
			return ;
		}
		if (_remove_duplicated_server_names(*it_serv, new_server))
		{
			servers.erase(servers.end() - 1);
			protected_write_log(WARNING_MSG_DUPLICATE_SERV);
			return ;
		}
	}
}


void	interpret_tokens(std::queue<std::string> &tokens, std::vector<Server> &servers)
{
	while (!tokens.empty())
	{
		std::string token = extract_token(tokens);
		servers.push_back(interpret_server_loop(tokens));
		_check_server_is_unique(servers);
	}
}