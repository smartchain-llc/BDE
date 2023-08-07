#include <ExtractionEngine.h>

int main(int argc, char** argv) {
    auto targetFilePath = cli::parse_arguments(argc, argv);
    TargetFile target{ targetFilePath };
    
    Extractor::Extract(target);
}