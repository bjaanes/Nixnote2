/*********************************************************************************
NixNote - An open-source client for the Evernote service.
Copyright (C) 2013 Randy Baumgarte

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
***********************************************************************************/

#include "notemodel.h"
#include "logger/qslog.h"
#include "global.h"

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql>


//****************************************************
//* This class is used to read & write information
//* to the NoteModel class.  This class holds what
//* a user sees in the table listing in the GUI.
//****************************************************


// Generic constructor
NoteModel::NoteModel(QObject *parent)
    :QSqlTableModel(parent)
{
    // Check if the table exists.  If not, create it.
    QSqlQuery sql;
    sql.exec("Select *  from sqlite_master where type='table' and name='NoteTable';");
    if (!sql.next())
        this->createTable();
    this->setEditStrategy(QSqlTableModel::OnFieldChange);

    this->setTable("NoteTable");
    this->setFilter("lid in (select lid from filter)");
}

// Destructor
NoteModel::~NoteModel() {
}


//* Create the NoteModel table.
void NoteModel::createTable() {
    QLOG_TRACE() << "Entering NoteModel::createTable()";

    QLOG_DEBUG() << "Creating table NoteTable";
    QSqlQuery sql;
    QString command("Create table if not exists NoteTable (" +
                  QString("lid integer primary key,") +
                  QString("dateCreated real default null,") +
                  QString("dateUpdated real default null,") +
                  QString("title text default null collate nocase,") +
                  QString("notebookLid integer default null,") +
                  QString("notebook text default null collate nocase,") +
                  QString("tags text default null collate nocase,") +

                  QString("author text default null collate nocase,") +
                  QString("dateSubject real default null,") +
                  QString("dateDeleted real default null,") +

                  QString("source text default null collate nocase,") +
                  QString("sourceUrl text default null collate nocase,") +
                  QString("sourceApplication text default null collate nocase,") +
                  QString("latitude real default null,") +
                  QString("longitude real default null,") +
                  QString("altitude real default null,") +
                  QString("hasEncryption integer default null,") +
                  QString("hasTodo integer default null,") +
                  QString("isDirty integer default null,") +
                  QString("size integer default null") +
                  QString(")"));
    if (!sql.exec(command) ||
            !sql.exec("CREATE INDEX NoteTable_Title_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Author_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Notebook_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Notebook_Lid_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_DateCreated_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_DateUpdated_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Date_Subject_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Date_Deleted_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Source_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Source_Url_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Source_Application_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Latitude_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Longitude_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Altitude_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Has_Encryption_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Has_Todo_Index on NoteTable (lid)") ||
            !sql.exec("CREATE INDEX NoteTable_Is_Dirty_Index on NoteTable (lid)")
       ) {
        QLOG_ERROR() << "Creation of NoteTable table failed: " << sql.lastError();
    }
}


//int NoteModel::rowCount(const QModelIndex & /*parent*/) const
// {
//    QLOG_TRACE() << "Entering NoteModel::rowCount()";

//    QSqlQuery sql;
//    sql.exec("Select count(lid) from NoteTable where lid in (select lid from filter)");
//    if (!sql.next()) {
//        QLOG_ERROR() << "Error retrieving NoteModel::rowCount: " << sql.lastError();
//        return 0;
//    }
//    int rowCount = QVariant(sql.value(0)).toInt();
//    return rowCount;

// }

int NoteModel::columnCount(const QModelIndex &parent) const {
//    parent.column();
    return NOTE_TABLE_COLUMN_COUNT;
}


Qt::ItemFlags NoteModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    // Make the table row non-editable
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}



QVariant NoteModel::data (const QModelIndex & index, int role) const {
//    if (index.column() != NOTE_TABLE_IS_DIRTY_POSITION || role != Qt::DisplayRole)
        return QSqlTableModel::data(index,role);

//    QVariant v = QSqlTableModel::data(index, role);
//    if (v.toBool())
//        return QVariant(QPixmap(":notebook_small.png"));
//    else
//        return QPixmap(":notebook_small.png");
}
