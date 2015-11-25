// Logger.hpp

#if !defined(SVM_LOGGER_H_)
#define SVM_LOGGER_H_

#include "Includes.hpp"
#include "Types.hpp"
#include "Compiler/TokenList.hpp"

namespace svm
{

	class Logger
	{
	public:
		static void Log(std::string str);
	};


	class BaseError
	{
	protected:
		std::string m_message;

	public:
		BaseError(std::string message);
		virtual std::string GetMessage() const = 0;
		friend std::ostream& operator<<(std::ostream &os, const BaseError &e);
	};

	class GenericError: public BaseError
	{
	private:
		GenericError();

	public:
		GenericError(std::string message);
		std::string GetMessage() const;
	};

	class CompileError: public BaseError
	{
	public:
		enum class Error { Syntax, Compile };

	private:
		Error m_type;
		std::string m_filename;
		Length m_linenum;
		Length m_column;
		std::string m_data;

	private:
		CompileError();

	public:
		CompileError(Error errorType, std::string filename, Length linenum, Length column, std::string message, std::string data = "");
		std::string GetMessage() const;
	};

	class TokenError: public BaseError
	{
	private:
		Compiler::Token *m_token;
		std::string m_filename;

	private:
		TokenError();

	public:
		TokenError(Compiler::Token *token, std::string name, std::string message);
		std::string GetMessage() const;
	};

	class FileError : public BaseError
	{
	public:
		enum class Error { OpenFailure, ReadFailure, WriteFailure };

	private:
		Error m_type;
		std::string m_filename;

	private:
		FileError();

	public:
		FileError(Error errorType, std::string filename, std::string message = "");
		std::string GetMessage() const;
	};

}

#endif
 
