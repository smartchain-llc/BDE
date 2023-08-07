#ifndef __ARGUMENTPARSER_H__
#define __ARGUMENTPARSER_H__

#include <filesystem>

namespace cli {
    static const char usage_msg[256] = "\nusage: bde [-sSeElL] <file>\n[-s, -S]\tStart offset\n[-e, -E]\tEnd offset\n[-l, -L] Byte size from start\0";

    class ArgumentParseExecptionType;
    class ArgumentParseException {
    public:
        ArgumentParseException(ArgumentParseExecptionType, const char*);
        void what();

        const int8_t& errorCode() { return _errorCode; }
    private:
        int8_t _errorCode;
        const char* _msg;
    };

    struct ArgumentPack {
        const char* filepath;
        uintmax_t startOffset { 0 };
        uintmax_t endOffset { 0 };
    };
    const ArgumentPack& parse_arguments(int& argc, char** argv);
}
#endif // __ARGUMENTPARSER_H__