#ifndef TDMP_TEARDOWN_CONTAINERS_VECTOR_H
#define TDMP_TEARDOWN_CONTAINERS_VECTOR_H

namespace td {

    // https://github.com/TTFH/TDLL/blob/main/teardown_structs.h & Modifications
    template<typename T>
    class td_vector {
    public:
        uint32_t size() const {
            return _size;
        }

        void remove(uint32_t index) {
            if (index < _size) {
                for (uint32_t i = index; i < _size - 1; i++) {
                    _data[i] = _data[i + 1];
                }

                _size--;
            }
        }

    public:
        T operator[](uint32_t index) const { return _data[index]; }
        T& operator[](uint32_t index) { return _data[index]; }

        inline void reset() { _size = 0; _capacity = 0; _data = nullptr; }

    private:
        uint32_t _size = 0;
        uint32_t _capacity = 0;
        T* _data = nullptr;
    };

}

#endif // TDMP_TEARDOWN_CONTAINERS_VECTOR_H