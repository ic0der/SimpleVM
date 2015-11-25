// Bytecode.cpp

#include "Bytecode.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

namespace svm
{
	Bytecode::Bytecode():
		buffer(nullptr), len(0)
	{}

	Bytecode::Bytecode(const std::string &path):
		buffer(nullptr), len(0)
	{
		ReadFromFile(path);
	}

	Bytecode::~Bytecode()
	{
		try
		{
			delete[] buffer;
		}
		catch (...) {}
	}

	void Bytecode::WriteToFile(std::string filename)
	{
		std::ofstream file(filename, std::ofstream::binary);
		if (!file)
			throw FileError(FileError::Error::OpenFailure, filename);

		file.write((char*)buffer, len);
		if (file.bad())
			throw FileError(FileError::Error::WriteFailure, filename);

		file.close();
	}

	void Bytecode::ReadFromFile(std::string filename)
	{
		std::ifstream file(filename, std::ios::binary);
		if (!file)
			throw FileError(FileError::Error::OpenFailure, filename);

		file.seekg(0, std::ios::end);
		len = file.tellg();
		file.seekg(0);

		buffer = new Byte[len];
		if (!buffer)
			throw "Allocation failure";

		file.read((char*)buffer, len);
		if (file.bad())
			throw FileError(FileError::Error::ReadFailure, filename);

		file.close();
	}

	Byte* Bytecode::operator[] (Length index)
	{
		if (buffer != nullptr && index < len)
			return buffer + index;
		return new Byte();
	}
}
