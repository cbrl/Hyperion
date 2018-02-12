#pragma once

#include <string>
#include <vector>

using std::string;
using std::wstring;
using std::vector;


// Trim whitespace at the beginning and end of a string
inline wstring TrimWhiteSpace(wstring& in) {
	size_t textStart = in.find_first_not_of(L" \t");
	size_t textEnd = in.find_last_not_of(L" \t");

	if (textStart != wstring::npos && textEnd != wstring::npos) {
		return in.substr(textStart, textEnd - textStart + 1);
	}
	else if (textStart != wstring::npos) {
		return in.substr(textStart);
	}

	return L"";
}


// Trim whitespace at the beginning and end of a string
inline string TrimWhiteSpace(string& in) {
	size_t textStart = in.find_first_not_of(" \t");
	size_t textEnd = in.find_last_not_of(" \t");

	if (textStart != string::npos && textEnd != string::npos) {
		return in.substr(textStart, textEnd - textStart + 1);
	}
	else if (textStart != string::npos) {
		return in.substr(textStart);
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