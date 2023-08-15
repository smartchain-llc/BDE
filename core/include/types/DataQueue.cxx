#include <types/DataQueue.h>

#define DATAQUEUE_EXCEPTION_BASE_ERR 20

namespace types {

uint8_t& FileData::operator[](int i) {
    return _buffer[i];
}


auto DataQueue::get_free_block() -> FileData* {
    if(!is_full()){
        std::cout << "[DataQueue] Returning writable <" <<
            &(_storage[_i_writable]) << ">\n";
        return &(_storage[_i_writable]);
    }
    return nullptr;
}
auto DataQueue::get_next_read() -> FileData* {
    std::cout << "[DataQueue] Returning readable <" <<
        &(_storage[_i_readable]) << ">\n";
    return &(_storage[_i_readable]);
}
void DataQueue::update_free_index(){ _i_writable = (_i_writable+1) % MAX_QUEUE_LENGTH; _size++; }
void DataQueue::update_read_index(){ _i_readable = (_i_readable+1) % MAX_QUEUE_LENGTH; _size--; }
}