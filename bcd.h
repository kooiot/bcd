// =====================================================================================
//
//       Filename:  bcd.h
//
//    Description:  bcd decode/encode helper
//
//        Version:  1.0
//        Created:  2015年04月03日 11时15分45秒
//       Revision:  none
//       Compiler:  gcc
//
//         Author:  Dirk Chang (RGlyay5DaGFuZwo=), changch84@gmail.com
//   Organization:  www.KooIoT.com
//
// =====================================================================================
#include <cstring>
#include <cassert>
#include <cmath>

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#include <typeinfo>
#endif

namespace bcd {
	/******************************************************************
	 *
	 * format: 
	 *		# for a digit
	 *		. for dot of float/double
	 *		0 for treat the digit as 0
	 ******************************************************************/
	template <class T>
		T decode(unsigned char* src, size_t count, bool reverse = false) {
			T val = 0;
			int i = 0;
			unsigned short digit = 0;
			for (; i < count; ++i) {
				// --- 
				val = val * 100;	
				if (!reverse) {
					digit =  (src[i] & 0xF ) + ( (src[i] & 0xF0) >> 4) * 10;
				} else {
					digit =  (src[count - i - 1] & 0xF ) * 10 + ((src[count - i - 1] & 0xF0) >> 4);
				}
				val += digit;
#ifdef DEBUG
				std::cout << "digit\t" << digit << std::endl;
				std::cout << "val\t" << std::setprecision(10) << val << std::endl;
#endif
			}
			return val;
		}

	template <class T>
		T decode(unsigned char* src, const char* format="####", bool reverse = false) {
			size_t fl = strlen(format);
			size_t fl2 = 0;

			const char* pch = strchr(format, '.');
			if (pch) {
				fl -= 1;
				fl2 = fl - (pch - format) + fl % 2;
			}

			// make a copy of the source bytes
			int count = (fl + 1) / 2;
			unsigned char* temp = new unsigned char[count];
			memcpy(temp, src, count);
			if (fl % 2 == 1) {
				if (!reverse) {
					temp[count - 1] = temp[count - 1] & 0xF0;
				} else {
					temp[0] = temp[0] & 0x0F;
				}
			}

			pch = strchr(format, '0');
			while(pch) {
				size_t i = pch - format;
				if (!reverse) {
					temp[i / 2] = temp[i / 2] & (i % 2 == 0 ? 0x0F : 0xF0);
				} else {
					temp[count - i / 2 - 1] = temp[count - i / 2 - 1] & (i % 2 == 0 ? 0xF0 : 0x0F);
				}
				pch = strchr(pch + 1, '0');
			}

			T val = decode<T>(temp, count, reverse);
#ifdef DEBUG
			std::cout << "val\t" << std::setprecision(10) << val << std::endl;
#endif
			if (fl2 != 0) {
				val = val / pow(10, fl2);
#ifdef DEBUG
				std::cout << "pow(10, fl2)\t" << std::setprecision(10) << pow(10, fl2) << std::endl;
#endif
			}
			return val;
		}

	template <class T>
		/******************************************************************
		 *
		 * format: 
		 *		# for a digit
		 *		. for dot of float/double
		 *		0 for fill with 0
		 ******************************************************************/
		size_t encode(unsigned char* buf, T src, const char* format, bool reverse = false) {
			size_t fl = strlen(format);
			size_t fl2 = 0;

			const char* pch = strchr(format, '.');
			if (pch) {
				fl -= 1;
				fl2 = fl - (pch - format) + fl % 2;
			}
			//std::cout << "src\t" << std::setprecision(10) << src << std::endl;
			long long val = floor(src * pow(10, fl2));

			// make a copy of the source bytes
			int count = (fl + 1) / 2;
			memset(buf, 0, count);

			unsigned short digit = 0;
			int i = 0;
			for (; i < count; ++i) {
				// --- 
				digit = val % 100;
				if (!reverse) {
					buf[count - i - 1] = digit % 10 + ((digit / 10) << 4);
				} else {
					buf[i] = ((digit % 10) << 4) + digit / 10;
				}

#ifdef DEBUG
				std::cout << "digit\t" << digit << std::endl;
				std::cout << "val\t" << std::setprecision(10) << val << std::endl;
#endif
				val = val / 100;	
			}

			// Make the zeros
			int j = 0;
			for (i = 0; j < fl; ++j, ++i) {
				if (format[j] == '.') {
					++j;
				}
				if (format[j] == '0') {
					if (!reverse) {
						buf[i / 2] &= (i % 2 == 0) ? 0x0F : 0xF0;
					} else {
						buf[count - (i / 2) - 1] &= (i % 2 == 1) ? 0x0F : 0xF0;
					}
				}
			}

			return count;
		}
}
