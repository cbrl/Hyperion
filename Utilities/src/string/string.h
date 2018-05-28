#pragma once

// Silence codecvt deprecation warning
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cwchar>
#include <locale>
#include <codecvt>
//#include <charconv>


using std::string;
using std::wstring;

using std::to_string;
using std::to_wstring;

// stringstream
using std::stringstream;
using std::istringstream;
using std::ostringstream;

// wstringstream
using std::wstringstream;
using std::wistringstream;
using std::wostringstream;




// Convert a string to a wide string
inline wstring str2wstr(const string& in) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;

	return wstring_converter.from_bytes(in);
}

// Convert a wide string to a string
inline string wstr2str(const wstring& in) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;

	return wstring_converter.to_bytes(in);
}


// Convert an integer to a hexadecimal string
template<typename T>
inline string int2hexstr(T i) {

	static_assert(std::is_integral_v<T>, "int2hexstr() input type is not an integral type");

	stringstream stream;
	stream << "0x"
	       << std::setfill('0') << std::setw(sizeof(T) * 2)
	       << std::hex << i;

	return stream.str();
}


// Trim whitespace at the beginning and end of a string
template<typename StringT>
inline StringT TrimWhiteSpace(StringT& in) {
	size_t text_start = in.find_first_not_of(L" \t");
	size_t text_end   = in.find_last_not_of(L" \t");

	if (text_start != StringT::npos && text_end != StringT::npos) {
		return in.substr(text_start, text_end - text_start + 1);
	}
	else if (text_start != StringT::npos) {
		return in.substr(text_start);
	}

	return StringT();
}



// Split a string by a specified token
template<typename StringT>
inline std::vector<StringT> Split(StringT& in, const typename StringT::value_type* token) {

	std::vector<StringT> out;

	StringT sToken(token);
	StringT temp;

	for (typename StringT::size_type i = 0; i < in.size(); ++i) {

		StringT substr = in.substr(i, sToken.size());

		if (substr == token) {
			if (!temp.empty()) {
				out.push_back(temp);
				temp.clear();
				i += sToken.size() - 1;
			}
			else {
				out.push_back(reinterpret_cast<const typename StringT::value_type*>(""));
			}
		}
		else if ((i + sToken.size()) >= in.size()) {
			temp += in.substr(i, sToken.size());
			out.push_back(temp);
			break;
		}
		else {
			temp += in[i];
		}
	}

	return out;
}
