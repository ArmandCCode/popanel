#include "SPEngine/spsqlite/SPSQLiteDB.h"

SPSQLiteDB::SPSQLiteDB() : m_SQLiteDB(NULL), m_DBPath(""), m_ErrCode(0)
{
    //ctor
}

SPSQLiteDB::~SPSQLiteDB()
{
    //dtor
    this->CloseDB();
}

bool SPSQLiteDB::OpenDB(std::string p_sPath)
{
    m_ErrCode = sqlite3_open(p_sPath.c_str(), &this->m_SQLiteDB);

    switch (m_ErrCode)
    {
    case SQLITE_OK:
        this->SetDBPath(p_sPath);
        return true;
        break;
    default:
        // réinitialiser le pointeur à NULL
        this->SetDBPath("");
        this->SetSQLiteDB(NULL);
        break;
    }

    return false;
}

void SPSQLiteDB::CloseDB()
{
    // Le pointeur est null
    if(this->GetSQLiteDB() != NULL)
    {
        sqlite3_close(this->GetSQLiteDB());
        this->SetDBPath("");
        this->SetSQLiteDB(NULL);
        this->m_ErrCode = 0;
    }
}

bool SPSQLiteDB::IsOpen()
{
    if (this->GetSQLiteDB() == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string SPSQLiteDB::GetErrorInfo()
{
    return sqlite3_errstr(this->m_ErrCode);
}
