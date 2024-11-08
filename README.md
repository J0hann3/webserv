# Webserv

[Subject Webserv Project 42](webserv.pdf)

## Config file
- Define a server scope

		Server { ... }

- Define a Location

		Location <path> { ... }

- Define a Cgi Location

		Location ~ .<extension> { ... }

- Define a field variable

		<field identifier> <args> ... ;

### Field identifiers :
#### Server only :
- listen [ \<host IP\> | localhost ][ : ]PORT<span style="color:gray"> -> define the IP and PORT of the current server scope</span>
- server_name \<server_name\> ...<span style="color:gray"> -> match for one or more server names</span>
- error_page \<code\> ... \<error_path_path\><span style="color:gray"> -> define a specific error page for one or more error codes</span>
- client_max_body_size \<value\>[ K | M ]<span style="color:gray"> -> define the maximum body size for the client in the current server scope</span>
#### Location / Server :
- root \<path\><span style="color:gray"> -> define the starting root of each request recieved by the server in this location</span>
- autoindex \<on | off\><span style="color:gray"> -> toggle if the request directory should list the contained files</span>
#### Location only :
- limit_except \<GET | POST | DELETE\> ...<span style="color:gray"> -> remove one or more methods in the current location by listing them</span>
- return \<error_code\> \<new_uri\><span style="color:gray"> -> redirect a location on a new given uri with an error code</span><span style="color:#666666"> - this field will discard all other fields</span>
- index \<file_name\><span style="color:gray"> -> if a directory is given in uri, redirect to the first found file from left to right</span>
- can_upload \<on | off\><span style="color:gray"> -> toggle if the current location can or can not accept file upload</span>
#### Cgi Location :
- cgi_path \<absolute_cgi_path\><span style="color:gray"> -> define the path of the binary that will execute the given file in uri</span>
- path_info \<absolute_cgi_root\><span style="color:gray"> -> define where the uri file name should be matched</span>

## New Notion

### HTTP Protocol

- Hyper Text Transfer Protocol
- Stateless => independent request
- Connnectionless

#### Request
- GET => fetch data from the server
	- don't use it for a form
- POST => add data to the server
- PUT => edit data already on the server
- DELETE => delete data from the server

#### Header
- ONE EMPTY LINE BEEETWEN HEADER AND CORE
- Host => IP or URL of the host
- User-Agent => navigator + OS version
- Accept-Language => client prefer language
- Accept-Encoding => compression treat by the client
- Connection => keep connection or waiting time

#### Status Code
- 1xx => Informational, it's processing
- 2xx => Success
- 3xx => Redirect
- 4xx => Client Error
- 5xx => Server Error

### Socket
- bidirectionnel communication
- communication if same type and domain of socket
- send() -> envoyer
- port (16 bits 0 à 65 535) -> 0 à 1023 réservé

#### client
- socket() -> create socket
	- socket type : stream sockets  + address domain : AT_INET + protocole : IPPROTO_TCP = TCP
- connect() -> connect to the address of the server
- read()/write()

#### Server
- socket()
- bind() ->Internet => port number on the machine
- listen()
- accept()
- send/receive data
