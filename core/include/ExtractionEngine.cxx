#include <ExtractionEngine.h>
#include <types/DataQueue.h>
#include <thread>

#include <cstdio>

#define DATA_BUFFER_LEN 8192

void extraction_fn(FILE* inputFile, const TargetFile& target, types::DataQueue& dataQueue) {
    types::FileData* write_block { nullptr };

    for(size_t i = target.start(); i <= target.end(); i += types::FILEDATA_BUFFER_LENGTH) {
        fseek(inputFile, i, 0);
        write_block = dataQueue.get_free_block();
        write_block->byteLength = target.end() - i > types::FILEDATA_BUFFER_LENGTH
            ? types::FILEDATA_BUFFER_LENGTH
            : target.end() - i;

        while( !write_block ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            write_block = dataQueue.get_free_block();
        }
        fread(write_block->buffer(), 1, write_block->byteLength, inputFile);
        dataQueue.update_free_index();
    }
}

void output_fn(const TargetFile& target, types::DataQueue& dataQueue) {
    std::string outputFilePath { target.path().string() +  ".bde" };
    types::FileData* read_block { nullptr };

    FILE* outputFile = fopen(outputFilePath.c_str(), "w+");
    fseek(outputFile, 0, 0);

    dataQueue.is_readable.get();

    while(!dataQueue.is_empty()) {
        read_block = dataQueue.get_next_read();
        dataQueue.update_read_index();
        fwrite(read_block->buffer(), 1, read_block->byteLength, outputFile);

    }
    fclose(outputFile);
}


void Extractor::Extract(const TargetFile& target) {

    FILE* in_file = fopen(target.path_cstr(), "rw");
    
    types::DataQueue dataSegment_q{ 10 };

    auto output_thread  = std::thread( output_fn, std::ref(target), std::ref(dataSegment_q));
    extraction_fn(in_file, target, dataSegment_q);

    output_thread.join();
}
