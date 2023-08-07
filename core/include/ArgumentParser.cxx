#include "ArgumentParser.h"
#include <iostream>
#include <cstring>

#define ARGC_MIN 2
#define ARG_PARSE_BASE_ERR 10

cli::ArgumentPack ArgumentPackage;

class cli::ArgumentParseExecptionType {
public:
    static ArgumentParseExecptionType Insufficient_Argc(){ return ArgumentParseExecptionType(1); }
    static ArgumentParseExecptionType Invalid_Argv(){ return ArgumentParseExecptionType(2); }

    operator int8_t() { return _type; }
private:
    ArgumentParseExecptionType(int8_t type): _type{ type }{}
    int8_t _type { -1 };
};

cli::ArgumentParseException::ArgumentParseException(cli::ArgumentParseExecptionType type, const char* msg):
    _errorCode{ ARG_PARSE_BASE_ERR + type },
    _msg{ msg }
    {}

void cli::ArgumentParseException::what(){
    std::cerr << _msg << cli::usage_msg << std::endl;
    exit(_errorCode);
}

/* [ PRIVATE ] - LOCAL HELPER FUNCTION FORWARD DECLARATIONS */
void _validate_argc(int argc) {
    if( argc < ARGC_MIN ) {
        auto e = cli::ArgumentParseException(cli::ArgumentParseExecptionType::Insufficient_Argc(), "bde: Insufficient arguments");
        throw e;
    }
} 
void _validate_argv(char** argv) {
    ArgumentPackage.filepath = argv[1];
    if( !argv[2] )
        return;
    
    if ( strncmp(argv[2], "-s", 2) || strncmp(argv[2], "-S", 2) ) {
        if( !argv[3] ) throw cli::ArgumentParseException(cli::ArgumentParseExecptionType::Invalid_Argv(), "bde: Invalid argument [-s, -S]");
        ArgumentPackage.startOffset = strtoull(argv[3], NULL, 10);

        if( argv[4] ) {
            if( strncmp(argv[4], "-e", 2) || strncmp(argv[4], "-E", 2) ) {
                ArgumentPackage.endOffset = strtoll(argv[5], NULL, 10);
            }
        }
    }
}


const cli::ArgumentPack& cli::parse_arguments(int& argc, char** argv) {
    try{
        _validate_argc(argc);
        _validate_argv(argv);
    }
    catch (cli::ArgumentParseException e) { e.what(); }
    return ArgumentPackage;
}