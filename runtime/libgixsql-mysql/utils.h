/*
* Copyright (C) 2021 Marco Ridoni
* Copyright (C) 2013 Tokyo System House Co.,Ltd.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation; either version 3,
* or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; see the file COPYING.LIB.  If
* not, write to the Free Software Foundation, 51 Franklin Street, Fifth Floor
* Boston, MA 02110-1301 USA
*/

#ifndef OCDBUTIL_H
#define OCDBUTIL_H

#include <algorithm>
#include <string>
#include <vector>
#include <algorithm>

#define SIGN_LENGTH 1
#define TERMINAL_LENGTH 1
#define DECIMAL_LENGTH 1

#define BUFFSIZE 256

char* trim_end(char*);
int strim(char* buf);
char* safe_strdup(char* s);
bool is_commit_or_rollback_statement(std::string query);
bool is_dml_statement(std::string query);
bool is_begin_transaction_statement(std::string query);
bool is_update_or_delete_statement(const std::string& query);
bool is_update_or_delete_where_current_of(const std::string& query, std::string& table_name, std::string& cursor_name, bool* is_delete);
void ltrim(std::string& s);

// trim from end (in place)
void rtrim(std::string& s);

// trim from both ends (in place)
void trim(std::string& s);

// trim from start (copying)
std::string ltrim_copy(std::string s);

// trim from end (copying)
std::string rtrim_copy(std::string s);

// trim from both ends (copying)
std::string trim_copy(std::string s);

std::string string_replace(const std::string& subject, const std::string& search, const std::string& replace);

bool starts_with(std::string s, std::string s1);

bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2);

std::string to_lower(const std::string s);
std::string to_upper(const std::string s);

template<typename T>
inline bool vector_contains(const std::vector<T>& v, T item)
{
    return std::find(v.begin(), v.end(), item) != v.end();
}
template<typename T>
inline bool vector_contains_all(const std::vector<T>& v, const std::vector<T>& srch)
{
	for (int i = 0; i < srch.size(); i++) {
		if (!vector_contains(v, srch.at(i))) {
			return false;
			break;
		}
	}
	return true;
}

std::string vector_join(const std::vector<std::string>& v, char sep);

#endif
