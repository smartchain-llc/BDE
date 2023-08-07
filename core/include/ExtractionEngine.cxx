#include <ExtractionEngine.h>

#include <cstdio>

#define DATA_BUFFER_LEN 8192

void Extractor::Extract(const TargetFile& target) {
    FILE* file = fopen(target.path_cstr(), "rw");

    fseek(file, target.start(), 0);
    
    uint8_t* dataBuffer = new uint8_t[DATA_BUFFER_LEN];
    auto bytesRead = fread(dataBuffer, 1, (target.end() - target.start()), file);

    fclose(file);

    char* outputPath = new char[256];
    sprintf(outputPath, "%s.bde1", target.path_cstr());
    
    file = fopen(outputPath, "w+");
    fwrite(dataBuffer, bytesRead, 1, file);

    fclose(file);
    delete[] dataBuffer;
}