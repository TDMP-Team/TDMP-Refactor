#ifndef TDMP_TEARDOWN_CONTAINERS_STRING_H
#define TDMP_TEARDOWN_CONTAINERS_STRING_H

namespace td {

    class td_string {
    public:
        td_string() = default;

        td_string(const char* str) {
            size_t len = std::strlen(str);
            if (len < 32) {
                std::strncpy(_stack, str, len);
                _stack[len] = '\0';
            } else {
                _heap = new char[len + 1];
                std::strcpy(_heap, str);
            }
        }

        ~td_string() {
            if (_stack[31] != '\0') {
                delete[] _heap;
            }
        }

        td_string& operator+=(const td_string& other) {
            size_t thisLen = length();
            size_t otherLen = other.length();
            size_t newLen = thisLen + otherLen;

            if (newLen < 32) {
                std::strcat(_stack, other.c_str());
            } else {
                char* newHeap = new char[newLen + 1];
                std::strcpy(newHeap, c_str());
                std::strcat(newHeap, other.c_str());
            
                if (_stack[31] != '\0') {
                    delete[] _heap;
                }
                _heap = newHeap;
                _stack[31] = '\1';
            }

            return *this;
        }

        const char* c_str() const {
            return _stack[31] != '\0' ? _heap : _stack;
        }

        size_t length() const {
            return std::strlen(c_str());
        }

    private:
        union {
            char* _heap;
            char _stack[32] = { 0 };
        };
    };

}

#endif // TDMP_TEARDOWN_CONTAINERS_STRING_H