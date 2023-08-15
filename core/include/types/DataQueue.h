#ifndef __DATAQUEUE_H__
#define __DATAQUEUE_H__

#include <cstdint>
#include <atomic>
#include <vector>
#include <iostream>

namespace types {

constexpr uint8_t MAX_QUEUE_LENGTH = 10;
constexpr uint16_t FILEDATA_BUFFER_LENGTH = 8192;

class DataQueueExceptionType;
class DataQueueException {
public:
    DataQueueException(DataQueueExceptionType, const char*);
    const char* what();
private:
    uint8_t _errorCode;
    const char* _msg;
};

class FileData {
public:
    FileData(): _buffer{ new uint8_t[FILEDATA_BUFFER_LENGTH] }
        {}
    ~FileData(){ delete[] _buffer; }
    uint8_t& operator[](int i);
    uint8_t* buffer(){ return _buffer; }
    size_t byteLength { 0 };
private:
    uint8_t *_buffer;
};

class DataQueue {
public:
    DataQueue(uint8_t size): 
        _size{ 0 },
        _storage{ MAX_QUEUE_LENGTH, FileData() }
        { 
            std::cout << "[DataQueue] Allocation <" <<
                &_storage[0] << " - " << 
                &_storage[MAX_QUEUE_LENGTH -1] << ">\n";
        }
    
    auto get_free_block() -> FileData*;
    auto get_next_read() -> FileData*;

    auto is_empty() { return _size == 0; }
    auto is_full() { return _size == 10; }

    void update_free_index();
    void update_read_index();
private:
    std::vector<FileData> _storage;
    std::atomic<uint8_t> 
        _i_writable {0},
        _i_readable {0},
        _size {0};
};

}


#endif // __DATAQUEUE_H__