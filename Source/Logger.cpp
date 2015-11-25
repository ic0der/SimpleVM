// Logger.cpp

#include "Logger.hpp"

namespace svm
{

	void Logger::Log(std::string str)
	{
		std::cout << "[LOG] " << str << std::endl;
	}


	BaseError::BaseError(std::string message):
		m_message(message)
	{}

	std::ostream& operator<<(std::ostream &os, const BaseError &e)
	{
		os << e.GetMessage() << std::endl;
		return os;
	}


	GenericError::GenericError(std::string message):
		BaseError(message)
	{}

	std::string GenericError::GetMessage() const
	{
		std::stringstream ss;
		ss << "[Error] " << m_message;
		return ss.str();
	}


	CompileError::CompileError(Error errorType, std::string filename, Length linenum, Length column, std::string message, std::string data):
		BaseError(message), m_type(errorType), m_filename(filename), m_linenum(linenum), m_column(column), m_data(data)
	{}

	std::string CompileError::GetMessage() const
	{
		std::stringstream ss;

		std::string type;
		switch (m_type)
		{
		case Error::Compile:
			type = "[Compile ";
			break;
		case Error::Syntax:
			type = "[Syntax ";
			break;
		}
		type += "Error] ";

		std::string data;
		if (m_data.length() > 0)
			data = ": " + m_data;

		ss << type << m_filename << " @" << m_linenum << ":" << m_column << std::endl << "    " << m_message << data;
		return ss.str();
	}


	TokenError::TokenError(Compiler::Token *token, std::string name, std::string message):
		BaseError(message), m_token(token), m_filename(name)
	{}

	std::string TokenError::GetMessage() const
	{
		std::stringstream ss;
		ss << "[Syntax Error] " << m_filename << " @" << m_token->linenum << ":" << m_token->column << std::endl << "    " << m_message;
		return ss.str();
	}


	FileError::FileError(Error errorType, std::string filename, std::string message):
		BaseError(message), m_type(errorType), m_filename(filename)
	{}

	std::string FileError::GetMessage() const
	{
		std::stringstream ss;

		std::string message = "";
		if (m_message.length() > 0)
			message = ": " + m_message;

		std::string error;
		switch (m_type)
		{
		case Error::OpenFailure:
			error = "Cannot open ";
			break;
		case Error::ReadFailure:
			error = "Cannot write into ";
			break;
		case Error::WriteFailure:
			error = "Cannot read from ";
			break;
		}

		ss << "[File Error] " << error << m_filename << message;
		return ss.str();
	}

}
 
