#pragma once

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Silence codecvt deprecation warning
#undef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING



//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cwchar>
#include <locale>
#include <codecvt>
//#include <charconv>

#include <gsl/pointers>
#include <gsl/string_span>



//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

// Convert a std::string to a wide std::string
inline std::wstring str2wstr(const std::string& in) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;

	return wstring_converter.from_bytes(in);
}

// Convert a wide std::string to a std::string
inline std::string wstr2str(const std::wstring& in) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;

	return wstring_converter.to_bytes(in);
}


// Convert an integer to a hexadecimal std::string
template<typename T>
std::string int2hexstr(T i) {

	static_assert(std::is_integral_v<T>, "int2hexstr() input type is not an integral type");

	std::stringstream stream;
	stream << "0x"
	       << std::setfill('0') << std::setw(sizeof(T) * 2)
	       << std::hex << i;

	return stream.str();
}


// Trim whitespace at the beginning and end of a std::string
inline std::string TrimWhiteSpace(const std::string& in) {
	const size_t text_start = in.find_first_not_of(" \t");
	const size_t text_end = in.find_last_not_of(" \t");

	if (text_start != std::string::npos && text_end != std::string::npos) {
		return in.substr(text_start, text_end - text_start + 1);
	}
	if (text_start != std::string::npos) {
		return in.substr(text_start);
	}

	return std::string();
}

// Trim whitespace at the beginning and end of a std::string
inline std::wstring TrimWhiteSpace(const std::wstring& in) {
	const size_t text_start = in.find_first_not_of(L" \t");
	const size_t text_end   = in.find_last_not_of(L" \t");

	if (text_start != std::wstring::npos && text_end != std::wstring::npos) {
		return in.substr(text_start, text_end - text_start + 1);
	}
	if (text_start != std::wstring::npos) {
		return in.substr(text_start);
	}

	return std::wstring();
}


// Split a std::string by a specified token
template<typename StringT>
std::vector<StringT> Split(const StringT& in, const typename StringT::value_type* token) {

	std::vector<StringT> out;
	StringT token_s(token);

	size_t start = 0;
	size_t end   = in.find(token_s);

	while (end != StringT::npos) {
		out.push_back(in.substr(start, end - start));
		start = end + token_s.length();
		end = in.find(token_s, start);
	}

	out.push_back(in.substr(start, end));

	return out;
}
