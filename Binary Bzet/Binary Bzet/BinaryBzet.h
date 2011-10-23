#include <stdint.h>

typedef uint8_t u8; // unsigned 8 bit int
typedef uint64_t u64; // unsigned 64 bit int

class BinaryBzet {
public:
	BinaryBzet();
	~BinaryBzet();
private:
	u8 GetNumEndingZero(u64 index);
};