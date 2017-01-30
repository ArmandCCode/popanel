#ifndef SPSQLITESTATEMENT_H
#define SPSQLITESTATEMENT_H

#include <sqlite3.h>
#include <string>
#include "SPSQLiteDB.h"

class SPSQLiteStatement
{
    public:
        SPSQLiteStatement(SPSQLiteDB* p_ParentDB, std::string p_SQLText);
        virtual ~SPSQLiteStatement();

        // Paramétrage de la requête
        sqlite3_stmt* GetStatementHandle() { return m_StatementHandle; }
        std::string GetQuery() { return m_sQuery; }

        // Compilation requête
        bool CompileQuery();

        // Infos d'erreur
        std::string GetErrorInfo();
        std::string GetQueryStatusInfo();

        // Statut
        bool IsCompiled(); // La requête a t'elle pu être compilée ?

        // Exécution des étapes de la requête
        void ExecuteQueryStep(); // Exécuter un pas dans la requête
        int GetQueryStatus(); // SQLITE_ROW, SQLITE_DONE, SQLITE_ERROR, et autres...

        // Fermeture de la requête
        void CloseQuery();

        // Récupération des valeurs d'une requête SELECT
        double GetValueAsDouble(int ColumnCode);
        int GetValueAsInt(int ColumnCode);
        std::string GetValueAsText(int ColumnCode);
        const void* GetValueAsBlob(int ColumnCode); // Expérimentation !

    protected:
    private:
        SPSQLiteDB* m_ParentDB;
        std::string m_sQuery;
        sqlite3_stmt* m_StatementHandle;

        int m_ErrCode;
        int m_QueryStatus;

        // Accesseurs en interne
        SPSQLiteDB* GetParentDB() { return m_ParentDB; } // Accesseur pour l'objet base
        void SetQuery(std::string val) { m_sQuery = val; }

};

#endif // SPSQLITESTATEMENT_H
