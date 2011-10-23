#include "BinaryBzet.h"

BinaryBzet::BinaryBzet() {

}

BinaryBzet::~BinaryBzet() {

}

u8 BinaryBzet::GetNumEndingZero(u64 index)
{
	u8 c = 0;
	if (!(index % 2))
		c = u8(1) + GetNumEndingZero(index >> 1);
	return c;
}