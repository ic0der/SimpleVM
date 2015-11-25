// Utils.cpp

#include "Utils.hpp"
#include "Logger.hpp"

namespace svm
{
	std::string& Util::File::ReadFile(std::string filePath)
	{
		std::string *str = new std::string; // it should be on heap beacuse we are returning a reference to it

		std::ifstream file(filePath, std::ios::in);
		if (!file)
			throw FileError(FileError::Error::OpenFailure, filePath);
		
		str->append(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		if (file.bad())
			throw FileError(FileError::Error::ReadFailure, filePath);

		file.close();
		return *str;
	}

	void Util::File::WriteFile(std::string filePath, const std::string &content)
	{
		std::ofstream file(filePath, std::ios::out);
		if (!file)
			throw FileError(FileError::Error::OpenFailure, filePath);
		file << content;
		if (file.bad())
			throw FileError(FileError::Error::WriteFailure, filePath);
		file.close();
	}
/*
	bool Util::File::Exists(std::string filePath)
	{
		std::experimental::filesystem::path p = filePath;
		return std::experimental::filesystem::exists(p);
	}
*/

	Util::HRT::HRT()
	{
		m_start = std::chrono::steady_clock::now();
	}

	double Util::HRT::Reset()
	{
		std::chrono::duration<double> diff = std::chrono::steady_clock::now() - m_start;
		m_start = std::chrono::steady_clock::now();
		return diff.count();
	}

	double Util::HRT::Elapsed()
	{
		std::chrono::duration<double> diff = std::chrono::steady_clock::now() - m_start;
		return diff.count();
	}


	Hash Util::HashFunc(std::string name)
	{
		Hash hash;
		for (Length i = 0; i < name.length(); i++)
			hash = (hash << 1) ^ name[i];
		return hash;
	}

	static std::unordered_map<Length, UID> uids_map_;
	UID Util::GenUID(Length seed)
	{
		return ++uids_map_[seed];
	}
}
