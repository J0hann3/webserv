#ifndef HTTPREQUESTPOST_CLASS_HPP
# define HTTPREQUESTPOST_CLASS_HPP

# include "HttpTools.hpp"
# include "HttpRequest.class.hpp"

# define MSG_ERR_HTTPPOST_SSTREAM_FAIL "ERROR: internal: stringstream failure in the post request construction"
# define MSG_ERR_HTTPPOST_WRONG_METHOD "ERROR: internal: call to the wrong HTTP method constructor (POST)"

class HttpRequestPost : public HttpRequest
{
	public:
		HttpRequestPost (std::string const & str_request, Cluster & cluster) throw (ExceptionHttpStatusCode);
		HttpRequestPost ( HttpRequestPost const & model);
		HttpRequestPost & operator= (HttpRequestPost const & model);
		~HttpRequestPost( void );

		void	processHeader( Socket const * const socket );
		void	generateResponse( Socket const * const socket, HttpResponse &response );
		bool	hasBody() const;
		void	readBody(int fd, Socket const * const socket, bool &end);
		bool	hasCgi() const;
		void	setCgi(bool has_cgi);
		Cgi		*getCgi();

		void	processBody(bool &end);
		bool	hasContentLength() const;

		static bool	isBusyFile(std::string filename);

	private:
		HttpRequestPost( void );

		void		_initResponse( Socket const * const socket, HttpResponse &response );
		void		_handleCgi(CgiLocation const & cgi_location, Server const &server);

		void		_setBodyReadType(uint64_t maxClientBody);
		uint64_t	_getSizeToReadBody(uint64_t maxClientBody) const;

		void		_openFile();
		void		_closeFile();

		void		_processBodyContentLength(bool &end);
		bool		_parseChunkBody();
		void		_parseChunkSize();

		std::string		_filename;
		std::ofstream	_file;

		uint64_t	_content_length;
		uint64_t	_chunk_read_size;
		uint64_t	_read_size;

		bool		_has_size_chunk;
		bool		_chunk_body_flags;
		bool		_content_length_flags;

		Cgi			_cgi;
		bool		_has_cgi;
		
		static std::vector<std::string> _busyFile;
};

#endif
