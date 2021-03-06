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

#include "savedsearchproperties.h"
#include "global.h"
#include "sql/searchtable.h"

#include <QString>
#include <QIcon>
#include <QGridLayout>
#include <QLabel>

extern Global global;

SavedSearchProperties::SavedSearchProperties(QWidget *parent) :
    QDialog(parent)
{
    okPressed = false;
    setWindowTitle(tr("Saved Search"));
    setWindowIcon(QIcon(":search.png"));
    setLayout(&grid);

    connect(&name, SIGNAL(textChanged(const QString&)), this, SLOT(validateInput()));
    connect(&query, SIGNAL(textChanged(const QString&)), this, SLOT(validateInput()));

    nameLabel.setText(tr("Name"));
    queryLabel.setText(tr("Query"));
    queryGrid.addWidget(&nameLabel, 1,1);
    queryGrid.addWidget(&name, 1, 2);
    queryGrid.addWidget(&queryLabel, 2,1);
    queryGrid.addWidget(&query, 2, 2);
    queryGrid.setContentsMargins(10, 10,  -10, -10);
    grid.addLayout(&queryGrid,1,1);

    ok.setText(tr("OK"));
    connect(&ok, SIGNAL(clicked()), this, SLOT(okButtonPressed()));
    cancel.setText(tr("Cancel"));
    connect(&cancel, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    buttonGrid.addWidget(&ok, 1, 1);
    buttonGrid.addWidget(&cancel, 1,2);
    grid.addLayout(&buttonGrid,2,1);
}



void SavedSearchProperties::okButtonPressed() {
    okPressed = true;

    if (this->lid > 0) {
        SavedSearch s;
        SearchTable table;
        table.get(s, lid);
        s.name = this->name.text().trimmed().toStdString();
        s.__isset.name = true;
        s.query = this->query.text().trimmed().toStdString();
        s.__isset.query = true;
        table.update(lid, s, true);
        close();
        return;
    }

    // We have a new tag to add
    SavedSearch s;
    s.name = name.text().trimmed().toStdString();
    s.query = query.text().trimmed().toStdString();
    QUuid uuid;
    QString g =  uuid.createUuid().toString().replace("{","").replace("}","");
    s.guid = g.toStdString();
    s.__isset.name = true;
    s.__isset.query = true;
    s.__isset.guid = true;
    SearchTable t;
    t.add(0,s,true);
    close();
}

void SavedSearchProperties::cancelButtonPressed() {
    okPressed = false;
    close();
}

void SavedSearchProperties::setLid(qint32 lid) {
    if (lid > 0) {
        this->lid = lid;
        SavedSearch s;
        SearchTable table;
        table.get(s, lid);
        originalName = QString::fromStdString(s.name).trimmed();
        name.setText(originalName);
        query.setText(QString::fromStdString(s.query));
        return;
    }
    this->lid = 0;
    this->setWindowTitle(tr("Add Saved Search"));
    originalName = "";
}

void SavedSearchProperties::validateInput() {
    ok.setEnabled(true);
    if (name.text()=="") {
        ok.setEnabled(false);
        return;
    }
    if (query.text() == "") {
        ok.setEnabled(false);
        return;
    }
    SearchTable t;
    QString search = name.text().trimmed();
    if (t.findByName(search)>0 && name.text().trimmed() != originalName) {
        ok.setEnabled(false);
        return;
    }
}
