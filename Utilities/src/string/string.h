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
#include <charconv>

#include <gsl/pointers>
#include <gsl/string_span>

#include <optional>
#include <type_traits>


//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

// Convert a string to a wide string
inline std::wstring StrToWstr(const std::string& in) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;
	return wstring_converter.from_bytes(in);
}

// Convert a wide string to a string
inline std::string WstrToStr(const std::wstring& in) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;
	return wstring_converter.to_bytes(in);
}


// Convert an integer to a hexadecimal string
template<typename T>
std::string IntToHexStr(T i) {

	static_assert(std::is_integral_v<T>, "int2hexstr() input type is not an integral type");

	std::stringstream stream;
	stream << "0x"
	       << std::setfill('0') << std::setw(sizeof(T) * 2)
	       << std::hex << i;

	return stream.str();
}


// Trim whitespace at the beginning and end of a string
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

// Trim whitespace at the beginning and end of a string
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


// Split a string by a specified token
template<typename StringT>
inline std::vector<StringT> Split(const StringT& in, const typename StringT::value_type* token) {

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


// Convert a string to the specified arithmetic type
template<typename T>
inline std::optional<T> StrTo(std::string_view in) {

	static_assert(std::is_arithmetic_v<T>, "StrTo called with non-arithmetic type");
	
	T out = {};
	const std::from_chars_result result = std::from_chars(in.data(), in.data() + in.size(), out);

	if (result.ptr != (in.data() + in.size())
	    || result.ec == std::errc::invalid_argument
	    || result.ec == std::errc::result_out_of_range) {
		
		return {};
	}

	return out;
}

template<>
inline std::optional<bool> StrTo(std::string_view in) {

	if (in.size() == 1) {
		if (in[0] == '0')
			return false;
		else
			return true;
	}
	if (in.size() == 4
	    && (in[0] == 't' || in[0] == 'T')
	    && (in[1] == 'r' || in[1] == 'R')
	    && (in[2] == 'u' || in[2] == 'U')
	    && (in[3] == 'e' || in[3] == 'E')) {

		return true;
	}
	if (in.size() == 5
	    && (in[0] == 'f' || in[0] == 'F')
	    && (in[1] == 'a' || in[1] == 'A')
	    && (in[2] == 'l' || in[2] == 'L')
	    && (in[3] == 's' || in[3] == 'S')
	    && (in[4] == 'e' || in[4] == 'E')) {
		
		return false;
	}

	return {};
}