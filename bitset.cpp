#include <iostream>
#include <utility>
#include <stdint.h>
#include <limits>
#include <type_traits>


/// <summary>
/// 具有运行时常量大小的位向量。
/// </summary>
class BitSet {
public:
	using buffer_type = uint8_t;
	static_assert(std::is_unsigned<buffer_type>::value, "unsigned buffer type must be used!!");

	BitSet(int size_);
	BitSet(const BitSet& bs);
	BitSet(BitSet&& bs)noexcept;
	~BitSet();

	BitSet& operator=(const BitSet& bs) = delete;
	BitSet& operator=(BitSet&& bs)noexcept = delete;

	inline bool test(int i)const {
		return buffer[get_buffer_pos(i)] & get_bit_one_buffer(i);
	}
	inline bool operator[](int i)const {
		//note: not assignmentable
		return test(i);
	}

	BitSet& set(int i);
	BitSet& reset(int i);
	BitSet& flip(int i);

	BitSet& set();
	BitSet& reset();
	BitSet& flip();

	inline int get_size()const {
		return size;
	}

	void show()const;
private:
	const int size, bufsize;
	buffer_type* buffer;

	static constexpr int buf_bits = 8 * static_cast<int>(sizeof(buffer_type));
	static constexpr buffer_type
		BUF_ONE = std::numeric_limits<buffer_type>::max(),
		BUF_ZERO = std::numeric_limits<buffer_type>::min();

	static constexpr int get_buffer_pos(int i);
	static constexpr buffer_type get_bit_one_buffer(int i);
	static constexpr buffer_type get_bit_zero_buffer(int i);
};

BitSet::BitSet(int size_) :size(size_), bufsize((size_ - 1) / buf_bits + 1)
{
	buffer = new buffer_type[bufsize];
}

BitSet::BitSet(const BitSet& bs) : size(bs.size), bufsize(bs.bufsize)
{
	buffer = new buffer_type[bufsize];
	std::copy(bs.buffer, bs.buffer + bufsize, buffer);
}

BitSet::BitSet(BitSet&& bs) noexcept :
	size(bs.size), bufsize(bs.bufsize), buffer(bs.buffer)
{
	bs.buffer = nullptr;
}

BitSet::~BitSet()
{
	if (buffer) {
		delete[] buffer;
		buffer = nullptr;
	}
}

BitSet& BitSet::set(int i)
{
	buffer[get_buffer_pos(i)] |= get_bit_one_buffer(i);
	return *this;
}

BitSet& BitSet::reset(int i)
{
	buffer[get_buffer_pos(i)] &= get_bit_zero_buffer(i);
	return *this;
}

BitSet& BitSet::flip(int i)
{
	buffer[get_buffer_pos(i)] ^= get_bit_one_buffer(i);
	return *this;
}

BitSet& BitSet::set()
{
	for (int i = 0; i < bufsize - 1; i++) {
		buffer[i] = BUF_ONE;
	}
	for (int i = (bufsize - 1) * buf_bits; i < size; i++) {
		set(i);
	}
	return *this;
}

BitSet& BitSet::reset()
{
	for (int i = 0; i < bufsize - 1; i++) {
		buffer[i] = BUF_ZERO;
	}
	for (int i = (bufsize - 1) * buf_bits; i < size; i++) {
		reset(i);
	}
	return *this;
}

BitSet& BitSet::flip()
{
	for (int i = 0; i < bufsize; i++) {
		buffer[i] = ~buffer[i];
	}
	return *this;
}

void BitSet::show() const
{
	for (int i = 0; i < size; i++) {
		std::cout << (int)test(i);
	}
	std::cout << std::endl;
}

constexpr int BitSet::get_buffer_pos(int i)
{
	return i / buf_bits;   //int div
}

constexpr typename BitSet::buffer_type BitSet::get_bit_one_buffer(int i)
{
	return static_cast<buffer_type>(1) << (buf_bits - 1 - (i % buf_bits));
}

constexpr typename BitSet::buffer_type BitSet::get_bit_zero_buffer(int i)
{
	return ~get_bit_one_buffer(i);
}