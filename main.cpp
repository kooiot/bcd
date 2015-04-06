// =====================================================================================
//
//       Filename:  main.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年04月03日 11时51分07秒
//       Revision:  none
//       Compiler:  gcc
//
//         Author:  Dirk Chang (RGlyay5DaGFuZwo=), changch84@gmail.com
//   Organization:  www.KooIoT.com
//
// =====================================================================================

#include <iostream>
#include <iomanip>
#include "bcd.h"
#include <cassert>

int main() {
	unsigned char temp[4] = {0};
	temp[0] = 0x12;
	temp[1] = 0x34;
	temp[2] = 0x56;
	temp[3] = 0x78;

	int v = bcd::decode<int>(temp, "########", true);
	assert(v == 87654321);
	v = bcd::decode<int>(temp, "########", false);
	assert(v == 12345678);

	double fv = bcd::decode<double>(temp, "0####.###");
	//std::cout << std::setprecision(10) << fv << std::endl;
	assert(fv - 2345.678 < 0.0001);
	fv = bcd::decode<double>(temp, "####.###");
	//std::cout << std::setprecision(10) << fv << std::endl;
	assert(fv - 1234.567 < 0.0001);

	fv = bcd::decode<double>(temp, "0#####.##", true);
	//std::cout << std::setprecision(10) << fv << std::endl;
	assert(fv - 76543.21 < 0.0001);
	fv = bcd::decode<double>(temp, "#####.##", true);
	//std::cout << std::setprecision(10) << fv << std::endl;
	assert(fv - 87654.32 < 0.0001);

	unsigned char buf[4] = {0};
	assert(4 == bcd::encode<int>(buf, 87654321, "########", true));
	assert(memcmp(temp, buf, 4) == 0);
	assert(4 == bcd::encode<double>(buf, 8765.4321, "####.####", true));
	assert(memcmp(temp, buf, 4) == 0);

	assert(4 == bcd::encode<int>(buf, 12345678, "########"));
	assert(memcmp(temp, buf, 4) == 0);
	assert(4 == bcd::encode<double>(buf, 12345.678, "#####.###"));
	assert(memcmp(temp, buf, 4) == 0);

	assert(4 == bcd::encode<int>(buf, 12345678, "#######0"));
	assert(memcmp(temp, buf, 3) == 0);
	assert(buf[3] == 0x70);

	assert(4 == bcd::encode<int>(buf, 87654321, "#######0", true));
	assert(memcmp(temp + 1, buf + 1, 3) == 0);
	assert(buf[0] == 0x02);


	/*
	std::cout << memcmp(temp, buf, 4) << std::endl;
	std::cout << std::hex;
	for (int i = 0; i < 4; ++i) {
		std::cout << (short)temp[i] << '\t' << (short)buf[i] << std::endl;
	}
	*/


	assert(4 == bcd::encode<double>(buf, 12345.678, "0####.###"));
	assert(memcmp(temp + 1, buf + 1, 3) == 0);
	//std::cout << std::hex << (short(buf[0]));
	assert(buf[0] == 0x02);

	assert(4 == bcd::encode<double>(buf, 87654.321, "0####.###", true));
	assert(memcmp(temp, buf, 3) == 0);
	assert(buf[3] == 0x70);

	std::cout << "--------------------------------------" << std::endl;
	std::cout << "| All tests run successfully" << std::endl;
	std::cout << "--------------------------------------" << std::endl;

	return 0;
}
