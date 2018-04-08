#pragma once

#include <string>
#include <vector>
#include <cwchar>
#include <locale>
#include <codecvt>


using std::string;
using std::wstring;

using std::to_string;
using std::to_wstring;



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


// Trim whitespace at the beginning and end of a string
template<typename StringT>
inline StringT TrimWhiteSpace(StringT& in) {
	size_t text_start = in.find_first_not_of(L" \t");
	size_t text_end = in.find_last_not_of(L" \t");

	if (text_start != StringT::npos && text_end != StringT::npos) {
		return in.substr(text_start, text_end - text_start + 1);
	}
	else if (text_start != StringT::npos) {
		return in.substr(text_start);
	}

	return StringT();
}



// Split a string by a specified token
template<typename StringT, typename CharT>
inline void Split(StringT& in, std::vector<StringT>& out, const CharT* token) {

	// Ensure the string's char type and token's type are the same
	if constexpr (is_same_v<string, StringT>)
		static_assert(is_same_v<string, StringT> && is_same_v<char, CharT>, "Split() string/token type mismatch");

	if constexpr (is_same_v<wstring, StringT>)
		static_assert(is_same_v<wstring, StringT> && is_same_v<wchar_t, CharT>, "Split() string/token type mismatch");


	out.clear();

	StringT sToken(token);
	StringT temp;

	for (size_t i = 0; i < in.size(); ++i) {

		StringT substr = in.substr(i, sToken.size());

		if (substr == token) {
			if (!temp.empty()) {
				out.push_back(temp);
				temp.clear();
				i += sToken.size() - 1;
			}
			else {
				out.push_back((CharT*)"");
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
}
