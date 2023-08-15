#include <ExtractionEngine.h>
#include <types/DataQueue.h>
#include <thread>

#include <cstdio>

#define DATA_BUFFER_LEN 8192

void extraction_fn(FILE* inputFile, const TargetFile& target, types::DataQueue& dataQueue);
void output_fn(const TargetFile& target, types::DataQueue& dataQueue);


void Extractor::Extract(const TargetFile& target) {

    FILE* in_file = fopen(target.path_cstr(), "rw");
    
    types::DataQueue dataSegment_q{ 10 };

    // auto extraction_thread = std::thread( extraction_fn, in_file, std::ref(target), std::ref(dataSegment_q) );
    auto output_thread  = std::thread( output_fn, std::ref(target), std::ref(dataSegment_q));
    extraction_fn(in_file, target, dataSegment_q);

    // extraction_thread.join();
    output_thread.join();

    // fseek(file, target.start(), 0);
    
    // uint8_t* dataBuffer = new uint8_t[DATA_BUFFER_LEN];
    // auto bytesRead = fread(dataBuffer, 1, (target.end() - target.start()), file);

    // fclose(file);

    // char* outputPath = new char[256];
    // sprintf(outputPath, "%s.bde1", target.path_cstr());
    
    // file = fopen(outputPath, "w+");
    // fwrite(dataBuffer, bytesRead, 1, file);

    // fclose(file);
    // delete[] dataBuffer;
}

void extraction_fn(FILE* inputFile, const TargetFile& target, types::DataQueue& dataQueue) {
    types::FileData* write_block { nullptr };

    for(size_t i = target.start(); i <= target.end(); i += types::FILEDATA_BUFFER_LENGTH) {
    fseek(inputFile, i, 0);
        write_block = dataQueue.get_free_block();
        while( !write_block ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            write_block = dataQueue.get_free_block();
        }
        write_block->byteLength = fread(write_block->buffer(), 1, types::FILEDATA_BUFFER_LENGTH, inputFile);
        dataQueue.update_free_index();
    }
    std::cout << "[Queue Writer] Done\n";
}

void output_fn(const TargetFile& target, types::DataQueue& dataQueue) {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::cout << "[Queue Reader] Initiating Read...\n";
    std::string outputFilePath { target.path().string() +  ".bde" };
    types::FileData* read_block { nullptr };

    FILE* outputFile = fopen(outputFilePath.c_str(), "w+");
    fseek(outputFile, 0, 0);

    while(!dataQueue.is_empty()) {
        read_block = dataQueue.get_next_read();
        dataQueue.update_read_index();
        fwrite(read_block->buffer(), 1, read_block->byteLength, outputFile);
        fseek(outputFile, SEEK_END, 0);

    }
    std::cout << "[Queue Reader] Done\n";
    fclose(outputFile);
}