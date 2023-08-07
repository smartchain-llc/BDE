#include "TargetFile.h"
#include <iostream>

void _validate_file_path(std::filesystem::path& filepath);

class TargetFileExceptionType {
public:
    static TargetFileExceptionType Invalid_File() { return TargetFileExceptionType(1); }

    operator int8_t() const { return _type; }
private:
    TargetFileExceptionType(int8_t type): _type { type }{}

private:
    int8_t _type;

};

class TargetFileException {
public:
    TargetFileException(TargetFileExceptionType e, const char* msg):
        _errorCode{ _errorCode + e },
        _msg { msg }
        {}
    void what();

private:
    int8_t _errorCode;
    const char* _msg;
};

void TargetFileException::what() {
    std::cerr << _msg << std::endl;
    exit( _errorCode );
}




TargetFile::TargetFile(const char* filepath):
    _filepath{ std::filesystem::canonical(filepath) }
{
    try{
        _validate_file_path(_filepath);
    }
    catch(TargetFileException e){ e.what(); }
}


TargetFile::TargetFile(const cli::ArgumentPack& cli_args):
    _filepath{ cli_args.filepath } {
    try {
        _validate_file_path( _filepath );


        _filesize = std::filesystem::file_size( _filepath );
        _startOffset = (cli_args.startOffset > 0)
            ? cli_args.startOffset
            : 0 ;
            
        _endOffset = (cli_args.endOffset != 0)
            ? cli_args.endOffset
            : _filesize ;

        std::cout << "[" << _startOffset << " - " << _endOffset << "]\n";
    }
    catch(TargetFileException e){ e.what(); }
} 

void _validate_file_path(std::filesystem::path& filepath) {
    if( !std::filesystem::exists( filepath )) throw TargetFileException(TargetFileExceptionType::Invalid_File(), "bde: File does not exist");
    if( !std::filesystem::is_regular_file( filepath )) throw TargetFileException(TargetFileExceptionType::Invalid_File(), "bde: Invalid File");
}