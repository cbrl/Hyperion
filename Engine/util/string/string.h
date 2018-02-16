#pragma once

#include <string>
#include <vector>

using std::string;
using std::wstring;
using std::vector;


// Trim whitespace at the beginning and end of a string
inline wstring TrimWhiteSpace(wstring& in) {
	size_t text_start = in.find_first_not_of(L" \t");
	size_t text_end = in.find_last_not_of(L" \t");

	if (text_start != wstring::npos && text_end != wstring::npos) {
		return in.substr(text_start, text_end - text_start + 1);
	}
	else if (text_start != wstring::npos) {
		return in.substr(text_start);
	}

	return L"";
}


// Trim whitespace at the beginning and end of a string
inline string TrimWhiteSpace(string& in) {
	size_t text_start = in.find_first_not_of(" \t");
	size_t text_end = in.find_last_not_of(" \t");

	if (text_start != string::npos && text_end != string::npos) {
		return in.substr(text_start, text_end - text_start + 1);
	}
	else if (text_start != string::npos) {
		return in.substr(text_start);
	}

	return "";
}


// Split a string by a specified token
inline void Split(wstring& in, vector<wstring>& out, const wchar_t* token) {
	out.clear();

	wstring sToken(token);
	wstring temp;

	for (size_t i = 0; i < in.size(); ++i) {

		wstring substr = in.substr(i, sToken.size());

		if (substr == token) {
			if (!temp.empty()) {
				out.push_back(temp);
				temp.clear();
				i += sToken.size() - 1;
			}
			else {
				out.push_back(L"");
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


// Split a string by a specified token
inline void Split(string& in, vector<string>& out, const char* token) {
	out.clear();

	string sToken(token);
	string temp;

	for (size_t i = 0; i < in.size(); ++i) {

		string substr = in.substr(i, sToken.size());

		if (substr == token) {
			if (!temp.empty()) {
				out.push_back(temp);
				temp.clear();
				i += sToken.size() - 1;
			}
			else {
				out.push_back("");
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