#ifndef __TARGETFILE_H__
#define __TARGETFILE_H__

#include "ArgumentParser.h"

class TargetFile {
public:
    TargetFile(const char* filepath);
    TargetFile(const cli::ArgumentPack& cli_args);
    
    const char* path_cstr()         const { return _filepath.c_str(); }
    const auto  path()              const { return _filepath; }
    const uintmax_t& file_size()    const { return _filesize; }
    const uintmax_t& start()        const { return _startOffset; }
    const uintmax_t& end()          const { return _endOffset; }
private:
    std::filesystem::path   _filepath;

    uintmax_t _startOffset  { 0 };
    uintmax_t _endOffset    { 0 };
    uintmax_t _filesize     { 0 };
};

#endif // __TARGETFILE_H__