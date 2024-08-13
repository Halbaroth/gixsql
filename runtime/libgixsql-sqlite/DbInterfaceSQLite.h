#pragma once

/*
This file is part of Gix-IDE, an IDE and platform for GnuCOBOL
Copyright (C) 2021 Marco Ridoni

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA.
*/


#include "sqlite3.h"

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "ICursor.h"
#include "IDbInterface.h"
#include "IDbManagerInterface.h"
#include "IDataSourceInfo.h"
#include "ISchemaManager.h"


#define DECODE_BINARY_ON		1
#define DECODE_BINARY_OFF		0
#define DECODE_BINARY_DEFAULT	DECODE_BINARY_ON

struct SQLiteStatementData : public IPrivateStatementData {

	SQLiteStatementData();
	~SQLiteStatementData();

	void resizeParams(int n);
	void resizeColumnData(int n);

	sqlite3_stmt *statement = nullptr;

	int params_count = 0;
	int coldata_count = 0;

	bool _on_first_row = false;

private:
	void cleanup();
};

class DbInterfaceSQLite : public IDbInterface, public IDbManagerInterface
{
public:
	DbInterfaceSQLite();
	~DbInterfaceSQLite();

	virtual int init(const GlobalEnv* genv, const std::shared_ptr<spdlog::logger>& _logger) override;
	virtual int connect(std::shared_ptr<IDataSourceInfo>, std::shared_ptr<IConnectionOptions>) override;
	virtual int reset() override;
	virtual int terminate_connection() override;
	virtual int exec(std::string) override;
	virtual int exec_params(const std::string& query, const std::vector<CobolVarType>& paramTypes, const std::vector<std_binary_data>& paramValues, const std::vector<unsigned long>& paramLengths, const std::vector<uint32_t>& paramFlags) override;
	virtual int cursor_declare(const std::shared_ptr<ICursor>& crsr) override;
	virtual int cursor_open(const std::shared_ptr<ICursor>& crsr) override;
	virtual int cursor_close(const std::shared_ptr<ICursor>& crsr) override;
	virtual int cursor_fetch_one(const std::shared_ptr<ICursor>& crsr, int) override;
	virtual bool get_resultset_value(ResultSetContextType resultset_context_type, const IResultSetContextData& context, int row, int col, char* bfr, uint64_t bfrlen, uint64_t* value_len, bool *is_db_null) override;
	virtual bool move_to_first_record(const std::string& stmt_name = "") override;
	virtual uint64_t get_native_features() override;
	virtual int get_num_rows(const std::shared_ptr<ICursor>& crsr) override;
	virtual int get_num_fields(const std::shared_ptr<ICursor>& crsr) override;
	virtual const char* get_error_message() override;
	virtual int get_error_code() override;
	virtual std::string get_state() override;
	virtual int prepare(const std::string& stmt_name, const std::string& query) override;
	virtual int exec_prepared(const std::string& stmt_name, std::vector<CobolVarType> paramTypes, std::vector<std_binary_data>& paramValues, std::vector<unsigned long> paramLengths, const std::vector<uint32_t>& paramFlags) override;
	virtual DbPropertySetResult set_property(DbProperty p, std::variant<bool, int, std::string> v) override;


	virtual bool getSchemas(std::vector<SchemaInfo*>& res) override;
	virtual bool getTables(std::string table, std::vector<TableInfo*>& res) override;
	virtual bool getColumns(std::string schema, std::string table, std::vector<ColumnInfo*>& columns) override;
	virtual bool getIndexes(std::string schema, std::string tabl, std::vector<IndexInfo*>& idxs) override;

private:

	sqlite3* connaddr = nullptr;

	std::shared_ptr<IDataSourceInfo> data_source_info;
	std::shared_ptr<IConnectionOptions> connection_opts;

	std::shared_ptr<SQLiteStatementData> current_statement_data;

	int last_rc = 0;
	std::string last_error;
	std::string last_state;

	std::map<std::string, std::shared_ptr<ICursor>> _declared_cursors;
	std::map<std::string, std::shared_ptr<SQLiteStatementData>> _prepared_stmts;

	int decode_binary = DECODE_BINARY_DEFAULT;

	int sqliteRetrieveError(int rc);
	void sqliteClearError();
	void sqliteSetError(int err_code, std::string sqlstate, std::string err_msg);

	int _sqlite_exec(const std::shared_ptr<ICursor>& crsr, const std::string&, std::shared_ptr<SQLiteStatementData> prep_stmt = nullptr);
	int _sqlite_exec_params(const std::shared_ptr<ICursor>& crsr, const std::string& query, const std::vector<CobolVarType>& paramTypes, const std::vector<std_binary_data>& paramValues, const std::vector<unsigned long>& paramLengths, const std::vector<uint32_t>& paramFlags, std::shared_ptr<SQLiteStatementData> prep_stmt = nullptr);

	int _sqlite_get_num_rows(sqlite3_stmt* r);

	std::shared_ptr<SQLiteStatementData> retrieve_prepared_statement(const std::string& prep_stmt_name);
	bool is_cursor_from_prepared_statement(ICursor* cursor);

	// Updatable cursor emulation
	bool updatable_cursors_emu = false;
	bool has_unique_key(std::string table_name, const std::shared_ptr<ICursor>& crsr, std::vector<std::string>& unique_key);
	bool prepare_updatable_cursor_query(const std::string& qry, const std::shared_ptr<ICursor>& crsr, const std::vector<std::string>& unique_key, sqlite3_stmt** update_stmt, std::vector<std::string>& key_params);
	std::vector<std::string> get_resultset_column_names(sqlite3_stmt* stmt);
};

