#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <boost/program_options.hpp>

#include "Parameters.h"

namespace hw8 {

namespace po = boost::program_options;

template<typename T>
void extractParameter(const po::variables_map& vm,
                   const std::string& key,
                   T& out) {
    if (vm.count(key) != 0) {
        out = vm[key].as<T>();
    }
}

template<typename T>
void extractParameter(const po::variables_map& vm,
                   const std::string& key,
                   T& out,
                   const std::string& errorMessage) {
    if (vm.count(key) != 0) {
        out = vm[key].as<T>();
        return;
    }
    throw std::runtime_error(errorMessage);
}

Parameters::Parameters(int argc, char** argv) {
    po::options_description desc{"Duplicate files finder"};
    desc.add_options()
        ("include,I", po::value<std::vector<std::string>>(), "Include dirs")
        ("exclude,E", po::value<std::vector<std::string>>(), "Exclude dirs")
        ("mask,M", po::value<std::vector<std::string>>(), "File name mask")
        ("recursive", po::value<size_t>()->default_value(0), "Is scan recursive (1) or not (0)")
        ("minFileSize", po::value<size_t>()->default_value(1), "Min file size to process, bytes")
        ("bufferSize", po::value<size_t>(), "File reading buffer size, bytes")
        ("hash,H", po::value<std::string>()->default_value("crc32"), "Hash func name: md5 or crc32")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    extractParameter(vm, "include", m_includeDirs, "At least one include dir is required");
    extractParameter(vm, "exclude", m_excludeDirs);
    extractParameter(vm, "mask", m_fileNameMasks);
    extractParameter(vm, "minFileSize", m_minFileSize);
    extractParameter(vm, "bufferSize", m_bufferSize, "Buffer size is required");

    size_t scanRecursive;
    extractParameter(vm, "recursive", scanRecursive);
    m_scanRecursive = ( scanRecursive != 0 );

    std::string hashAlgoName;
    extractParameter(vm, "hash", hashAlgoName);
    if (hashAlgoName == "crc32") {
        m_hashAlgo = HashAlgo::CRC32;
    }
    else if (hashAlgoName == "md5") {
        m_hashAlgo = HashAlgo::MD5;
    }
    else {
        throw std::runtime_error("Unknowh has algorithm " + hashAlgoName);
    }
}

const std::vector<std::string>& Parameters::GetIncludeDirs() const {
    return m_includeDirs;
}

const std::vector<std::string>& Parameters::GetExcludeDirs() const {
    return m_excludeDirs;
}

bool Parameters::GetScanRecursive() const {
    return m_scanRecursive;
}

size_t Parameters::GetMinFileSize() const {
    return m_minFileSize;
}

const std::vector<std::string>& Parameters::GetFileNameMasks() const {
    return m_fileNameMasks;
}

size_t Parameters::GetBufferSize() const {
    return m_bufferSize;
}

HashAlgo Parameters::GetHashAlgorithm() const {
    return m_hashAlgo;
}

std::ostream& operator<<(std::ostream& s, HashAlgo a) {
    s << (a == HashAlgo::CRC32 ? "crc32" : "md5") << std::endl;
    return s;
}

std::ostream& operator<<(std::ostream& s, const Parameters& p) {
    s << "Parameters:" << std::endl;
    
    s << "Include dirs: ";
    for (const auto& d : p.GetIncludeDirs()) {
        s << d << " ";
    }
    s << std::endl;

    s << "Exclude dirs: ";
    for (const auto& d : p.GetExcludeDirs()) {
        s << d << " ";
    }
    s << std::endl;

    s << "Scan recursive: " << p.GetScanRecursive() << std::endl;
    s << "Min file size: " << p.GetMinFileSize() << std::endl;
    
    s << "File masks: ";
    for (const auto& m : p.GetFileNameMasks()) {
        s << m << " ";
    }
    s << std::endl;
    
    s << "Buffer size: " << p.GetBufferSize() << std::endl;
    s << "Hash algo: " << p.GetHashAlgorithm() << std::endl;

    return s;
}

} // namespace hw8