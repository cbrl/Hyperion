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
#include <string_view>
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

#ifdef _WIN32
#include "os/windows/windows.h"
#endif


// String literal operators
using namespace std::string_literals;



//----------------------------------------------------------------------------------
// Functions - Conversion
//----------------------------------------------------------------------------------

// Convert a string to a wide string
inline std::wstring StrToWstr(std::string_view in) {
	if (in.empty()) return std::wstring{};
#ifdef _WIN32
	if (in.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
		return std::wstring{}; //return empty string if the size is greater than what MultiByteToWideChar can support
	}
	int count = MultiByteToWideChar(CP_UTF8, 0, in.data(), static_cast<int>(in.size()), NULL, 0);
	std::wstring out(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, in.data(), static_cast<int>(in.size()), out.data(), count);
	out.resize(count);
	return out;
#else
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;
	return wstring_converter.from_bytes(in);
#endif
}

// Convert a wide string to a string
inline std::string WstrToStr(std::wstring_view in) {
	if (in.empty()) return std::string{};
#ifdef _WIN32
	if (in.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
		return std::string{}; //return empty string if the size is greater than what WideCharToMultiByte can support
	}
	int count = WideCharToMultiByte(CP_UTF8, 0, in.data(), static_cast<int>(in.size()), NULL, 0, NULL, NULL);
	std::string out(count, 0);
	int written = WideCharToMultiByte(CP_UTF8, 0, in.data(), static_cast<int>(in.size()), out.data(), count, NULL, NULL);
	out.resize(count);
	return out;
#else
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_converter;
	return wstring_converter.to_bytes(in);
#endif
}




//----------------------------------------------------------------------------------
// Functions - ToStr
//----------------------------------------------------------------------------------

namespace detail {
template<typename T> requires std::is_arithmetic_v<T>
std::optional<std::string> ToStrImpl(T val, std::chars_format fmt) noexcept {
	// Determine maximum chars possible in string
	constexpr size_t max_chars = std::numeric_limits<T>::digits - std::numeric_limits<T>::min_exponent + 3;

	// Convert the string
	std::vector<char> chars(max_chars + 1, '\0');
	const std::to_chars_result result = std::to_chars(chars.data(), chars.data() + max_chars, val, fmt);

	if (result.ec != std::errc()) {
		return {};
	}

	return std::optional<std::string>{std::in_place, chars.data()};
}

template<typename T> requires std::is_arithmetic_v<T>
std::optional<std::string> ToStrImpl(T val, std::chars_format fmt, int precision) noexcept {
	// Determine maximum chars possible in string
	constexpr size_t max_chars = std::numeric_limits<T>::digits - std::numeric_limits<T>::min_exponent + 3;

	// Convert the string
	std::vector<char> chars(max_chars + 1, '\0');
	const std::to_chars_result result = std::to_chars(chars.data(), chars.data() + max_chars, val, fmt, precision);

	if (result.ec != std::errc()) {
		return {};
	}

	return std::optional<std::string>{std::in_place, chars.data()};
}
} //namespace detail

// Convert the specified arithemetic value to a string
template<typename T> requires std::is_arithmetic_v<T>
std::optional<std::string> ToStr(T val, int base = 10) noexcept {

	// Determine maximum chars possible in string
	size_t max_chars = 0;
	if constexpr (std::is_floating_point_v<T>)
		max_chars = std::numeric_limits<T>::digits - std::numeric_limits<T>::min_exponent + 3;
	else
		max_chars = std::numeric_limits<T>::digits + 1;

	// Convert the string
	std::vector<char> chars(max_chars + 1, '\0');
	const std::to_chars_result result = std::to_chars(chars.data(), chars.data() + max_chars, val, base);

	if (result.ec != std::errc()) {
		return {};
	}

	return std::optional<std::string>{std::in_place, chars.data()};
}

// Convert the specified arithemetic value to a string
template<typename T> requires std::is_arithmetic_v<T>
std::optional<std::string> ToStr(T val, std::chars_format fmt) noexcept {

	if constexpr (std::is_integral_v<T>) {
		if constexpr (sizeof(T) <= sizeof(float))
			return detail::ToStrImpl(static_cast<float>(val), fmt); //Formatted to_chars only accepts floating point values
		else
			return detail::ToStrImpl(static_cast<double>(val), fmt);
	}
	else {
		return detail::ToStrImpl(val, fmt);
	}
}

// Convert the specified arithemetic value to a string
template<typename T> requires std::is_arithmetic_v<T>
std::optional<std::string> ToStr(T val, std::chars_format fmt, int precision) noexcept {

	if constexpr (std::is_integral_v<T>) {
		if constexpr (sizeof(T) <= sizeof(float))
			return detail::ToStrImpl(static_cast<float>(val), fmt, precision); //Formatted to_chars only accepts floating point values
		else
			return detail::ToStrImpl(static_cast<double>(val), fmt, precision);
	}
	else {
		return detail::ToStrImpl(val, fmt, precision);
	}
}




//----------------------------------------------------------------------------------
// Functions - StrTo
//----------------------------------------------------------------------------------

// Convert a string to the specified arithmetic type
template<typename T>
std::optional<T> StrTo(std::string_view in) noexcept {

	static_assert(std::is_arithmetic_v<T>, "StrTo called with non-arithmetic type");

	T out = {};
	const auto [ptr, ec] = std::from_chars(in.data(), in.data() + in.size(), out);

	if (ptr != (in.data() + in.size())
	    || ec == std::errc::invalid_argument
	    || ec == std::errc::result_out_of_range) {

		return {};
	}

	return out;
}

template<>
inline std::optional<bool> StrTo(std::string_view in) noexcept {

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




//----------------------------------------------------------------------------------
// Functions - Manipulation
//----------------------------------------------------------------------------------

// Trim whitespace at the beginning and end of a string
inline std::string TrimWhiteSpace(std::string_view in) {
	const size_t text_start = in.find_first_not_of(" \t");
	const size_t text_end   = in.find_last_not_of(" \t");

	if (text_start != std::string_view::npos && text_end != std::string_view::npos) {
		return std::string{in.substr(text_start, text_end - text_start + 1)};
	}
	if (text_start != std::string_view::npos) {
		return std::string{in.substr(text_start)};
	}

	return {};
}

// Trim whitespace at the beginning and end of a string
inline std::wstring TrimWhiteSpace(std::wstring_view in) {
	const size_t text_start = in.find_first_not_of(L" \t");
	const size_t text_end   = in.find_last_not_of(L" \t");

	if (text_start != std::wstring_view::npos && text_end != std::wstring_view::npos) {
		return std::wstring{in.substr(text_start, text_end - text_start + 1)};
	}
	if (text_start != std::wstring_view::npos) {
		return std::wstring{in.substr(text_start)};
	}

	return {};
}


namespace detail {
template<typename StringT, typename ReturnT>
std::vector<ReturnT> SplitImpl(StringT in, StringT token) {
	std::vector<ReturnT> out;

	size_t start = 0;
	size_t end = in.find(token);

	while (end != StringT::npos) {
		out.emplace_back(in.substr(start, end - start));
		start = end + token.length();
		end = in.find(token, start);
	}
	out.emplace_back(in.substr(start, end));

	return out;
}
} //namespace detail

inline std::vector<std::string> Split(std::string_view in, std::string_view token) {
	return detail::SplitImpl<std::string_view, std::string>(in, token);
}

inline std::vector<std::wstring> Split(std::wstring_view in, std::wstring_view token) {
	return detail::SplitImpl<std::wstring_view, std::wstring>(in, token);
}



//----------------------------------------------------------------------------------
// Functions - Miscellaneous
//----------------------------------------------------------------------------------

// Get the string representation of a type name
template<typename T>
constexpr std::string_view type_name() noexcept {
#if defined(__clang__)
	std::string_view p = __PRETTY_FUNCTION__;
	return std::string_view{p.data() + 34, p.size() - 34 - 1};
#elif defined(__GNUC__)
	std::string_view p = __PRETTY_FUNCTION__;
	return std::string_view{p.data() + 49, p.find(';', 49) - 49};
#elif defined(_MSC_VER)
	std::string_view p = __FUNCSIG__;
	return std::string_view{p.data() + 84, p.size() - 84 - 7};
#endif
}