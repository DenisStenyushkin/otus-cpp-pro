#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace hw8 {

enum class HashAlgo {
    CRC32,
    MD5
};

class Parameters {
public:
    Parameters(int argc, char** argv);

    const std::vector<std::string>& GetIncludeDirs() const;
	const std::vector<std::string>& GetExcludeDirs() const;
	bool GetScanRecursive() const;
	size_t GetMinFileSize() const;
	const std::vector<std::string>& GetFileNameMasks() const;
	size_t GetBufferSize() const;
	HashAlgo GetHashAlgorithm() const;

private:
    std::vector<std::string> m_includeDirs;
	std::vector<std::string> m_excludeDirs;
	bool m_scanRecursive{ false };
	size_t m_minFileSize{ 1 };
	std::vector<std::string> m_fileNameMasks;
	size_t m_bufferSize{ 1 };
	HashAlgo m_hashAlgo{ HashAlgo::MD5 };
};

std::ostream& operator<<(std::ostream& s, const Parameters& p);

} // namespace hw8