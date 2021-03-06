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

#include "counterrunner.h"
#include "sql/notetable.h"
#include "sql/notebooktable.h"
#include "sql/tagtable.h"

#include <QtSql>

CounterRunner::CounterRunner(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
    keepRunning = true;
}


void CounterRunner::run() {
//    hammer = new Thumbnailer();
    QLOG_DEBUG() << "Starting CounterRunner";
    exec();
    QLOG_DEBUG() << "CounterRunner exiting.";
}


void CounterRunner::countAll() {
    this->countNotebooks();
    this->countTags();
    this->countTrash();
}


void CounterRunner::countTrash() {
    NoteTable ntable;
    QList<qint32> lids;
    emit trashTotals(ntable.getAllDeleted(lids));
}


void CounterRunner::countNotebooks() {

    // First get every possible tag
    NotebookTable nTable;
    QList<qint32> lids;
    nTable.getAll(lids);


    QSqlQuery query;
    query.exec("select notebooklid, count(notebooklid) from notetable where lid in (select lid from filter) group by notebooklid;");

    while(query.next()) {
        emit notebookTotals(query.value(0).toInt(), query.value(1).toInt());
        lids.removeAll(query.value(0).toInt());
    }

    // The ones that are left have a zero count
    for (int i=0; i<lids.size(); i++)
        emit(notebookTotals(lids[i], 0));
}


void CounterRunner::countTags() {
    // First get every possible tag
    TagTable tTable;
    QList<qint32> lids;
    tTable.getAll(lids);

    QSqlQuery query;
    query.prepare("select data, count(lid) from datastore where key=:key and lid in (select lid from filter) group by data;");
    query.bindValue(":key", NOTE_TAG_LID);
    query.exec();
    while(query.next()) {
        emit tagTotals(query.value(0).toInt(), query.value(1).toInt());
        lids.removeAll(query.value(0).toInt());
    }

    // The ones that are left have a zero count, so we reset them
    for (int i=0; i<lids.size(); i++)
        emit(tagTotals(lids[i], 0));

    // Finally, emit that we are done so unassigned tags can be hidden
    emit(tagCountComplete());
}
