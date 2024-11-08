#ifndef HTTPFIELD_CLASS_HPP
# define HTTPFIELD_CLASS_HPP

# include "HttpTools.hpp"

# include <vector>

# define MSG_ERR_FIELD_NO_SEPARATOR			"ERROR: HTTP request: a field does not contain a ':' separator"
# define MSG_ERR_FIELD_NO_VALUE				"ERROR: HTTP request: a field does not contain any value"
# define MSG_ERR_FIELD_NO_NAME				"ERROR: HTTP request: a field does not contain any name"
# define MSG_ERR_FIELD_QUOTE_NOT_CLOSED		"ERROR: HTTP request: quote note closed in a field"
# define MSG_ERR_FIELD_INVALID_CHARACTER	"ERROR: HTTP request: invalid character in a field"
# define MSG_ERR_FIELD_EMPTY_ADDED_VALUE	"ERROR: HTTP request: attempted to add an empty value to a field"


class HttpField
{
	public:
		HttpField( std::string const & whole_line ) throw (ExceptionHttpStatusCode);
		HttpField( std::string const & name, std::string const & values ) throw (ExceptionHttpStatusCode);
		HttpField( std::string const & name, std::vector<std::string> const & values );
		HttpField( HttpField const & model );
		HttpField & operator=( HttpField const & model );
		virtual ~HttpField( void );

		std::string const &					getName( void ) const;
		std::vector<std::string> const &	getValues( void ) const;
		std::string							getFields( void ) const;
		void								mergeFieldValues( HttpField &to_merge);

		static void 							fill_fields(
													std::stringstream &new_fields_stream,
													std::vector<HttpField> &existing_fields)
														throw (ExceptionHttpStatusCode);
		static bool								checkFieldExistence(std::vector<HttpField> const &fields,
													std::string const & field_name);
		static std::vector<std::string>			&getFieldValue(
													std::vector<HttpField> &fields,
													std::string const & field_name)
														throw(ExceptionHttpStatusCode);
		static std::vector<std::string>	const	&getFieldValue(
													std::vector<HttpField> const &fields,
													std::string const & field_name)
														throw(ExceptionHttpStatusCode);
		static void								erase_field(
													std::vector<HttpField> &fields,
													std::string const & field_name);
		static void								erase_field_value(
													HttpField &fields,
													std::string const & value);
		static e_status							extract_field(
													std::vector<HttpField> &fields,
													std::string const & value,
													std::vector<std::string> &dest_values);



		void	display_field ( void ) const;

	private:
		std::string					_name;
		std::vector<std::string>	_values;
		
		HttpField( void );
		void	_addNewValues( std::string str );
		void	_add_one_value( std::string const & str, size_t start_idx,
					size_t end_idx);
		void	_add_all_values( std::string & str);
		void	_setName( std::string const &str );

};

#endif