#ifndef SPSQLITEDB_H
#define SPSQLITEDB_H

#include <sqlite3.h>
#include <string>
#include <wx/msgdlg.h>

class SPSQLiteDB
{
    public:
        SPSQLiteDB();
        virtual ~SPSQLiteDB();
        bool OpenDB(std::string p_sPath);
        void CloseDB();

        // Utile pour les objets "requête", attention à ne pas en modifier le contenu !
        sqlite3* GetSQLiteDB() { return m_SQLiteDB; }

        // purement informatif
        std::string GetDBPath() { return m_DBPath; }

        // Obtenir la description du code d'erreur courant de l'objet
        std::string GetErrorInfo();

        // La base est elle ouverte ?
        bool IsOpen();

    protected:
    private:
        sqlite3* m_SQLiteDB;
        std::string m_DBPath;
        int m_ErrCode; // Code d'erreur d'ouverture de la base

        void SetSQLiteDB(sqlite3* val) { m_SQLiteDB = val; } // handle de la base SQLite3, n'utiliser qu'en interne
        void SetDBPath(std::string val) { m_DBPath = val; } // purement informatif

};

#endif // SPSQLITEDB_H
