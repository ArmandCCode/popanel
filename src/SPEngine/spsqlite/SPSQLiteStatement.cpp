#include "SPEngine/spsqlite/SPSQLiteStatement.h"

SPSQLiteStatement::SPSQLiteStatement(SPSQLiteDB* p_ParentDB, std::string p_SQLText) : m_ParentDB(p_ParentDB), m_sQuery(p_SQLText), m_StatementHandle(NULL), m_ErrCode(0), m_QueryStatus(0)
{

}

SPSQLiteStatement::~SPSQLiteStatement()
{
    //dtor
    this->CloseQuery();
    this->m_ErrCode = 0;
    this->m_QueryStatus = 0;
    this->m_ParentDB = NULL;
}

bool SPSQLiteStatement::CompileQuery()
{
    this->m_ErrCode = sqlite3_prepare_v2(this->GetParentDB()->GetSQLiteDB(), this->GetQuery().c_str(), -1, &this->m_StatementHandle, NULL);

    switch (this->m_ErrCode)
    {
    case SQLITE_OK:
        return true;
        break;
    default:
        break;
    }

    return false;
}

bool SPSQLiteStatement::IsCompiled()
{
    if(this->m_StatementHandle == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void SPSQLiteStatement::ExecuteQueryStep()
{
    this->m_QueryStatus = sqlite3_step(this->m_StatementHandle);
}

std::string SPSQLiteStatement::GetErrorInfo()
{
    return sqlite3_errstr(this->m_ErrCode);
}

std::string SPSQLiteStatement::GetQueryStatusInfo()
{
    return sqlite3_errstr(this->m_QueryStatus);
}

int SPSQLiteStatement::GetQueryStatus()
{
    return m_QueryStatus;
}

void SPSQLiteStatement::CloseQuery()
{
    if(this->IsCompiled())
    {
        sqlite3_finalize(this->m_StatementHandle);
        this->m_StatementHandle = NULL;
        this->SetQuery("");
    }
}

double SPSQLiteStatement::GetValueAsDouble(int ColumnCode)
{
    if (this->IsCompiled())
    {
        return sqlite3_column_double(this->m_StatementHandle, ColumnCode);
    }
    return 0.0;
}

int SPSQLiteStatement::GetValueAsInt(int ColumnCode)
{
    if (this->IsCompiled())
    {
        return sqlite3_column_int(this->m_StatementHandle, ColumnCode);
    }
    return 0;
}

std::string SPSQLiteStatement::GetValueAsText(int ColumnCode)
{
    std::string sTmp;
    if (this->IsCompiled())
    {
        sTmp = (const char*)sqlite3_column_text(this->m_StatementHandle, ColumnCode);
        return sTmp;
    }
    return "";
}

const void* SPSQLiteStatement::GetValueAsBlob(int ColumnCode) // Expérimentation !
{
    if (this->IsCompiled())
    {
        return sqlite3_column_blob(this->m_StatementHandle, ColumnCode);
    }
    return NULL;
}
