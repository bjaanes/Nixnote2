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

#ifndef NNOTEBOOKVIEW_H
#define NNOTEBOOKVIEW_H
#include "nnotebookviewitem.h"
#include "treewidgeteditor.h"
#include <QTreeWidget>
#include <QMenu>
#include <QShortcut>

class NNotebookView : public QTreeWidget
{
    Q_OBJECT
private:
    NNotebookViewItem *root;
    virtual void mousePressEvent(QMouseEvent *event);
    int filterPosition;
    QMenu context;
    QMenu *stackMenu;
    QAction *addAction;
    QAction *propertiesAction;
    QAction *deleteAction;
    QAction *renameAction;
    QAction *newStackAction;
    QAction *removeFromStackAction;
    QShortcut *renameShortcut;
    QShortcut *addShortcut;
    QShortcut *deleteShortcut;
    TreeWidgetEditor *editor;
    QHash<QString, QAction*> menuData;
    void sortStackMenu();


private slots:
    int calculateHeightRec(QTreeWidgetItem * item);
    void calculateHeight();
    void editComplete();

public:
    explicit NNotebookView(QWidget *parent = 0);
    ~NNotebookView();
    void resetSize();
    void updateSelection();
    void loadData();
    bool rebuildNotebookTreeNeeded;
    void contextMenuEvent(QContextMenuEvent *event);
    QHash<qint32, NNotebookViewItem*> dataStore;
    QHash<QString, NNotebookViewItem*> stackStore;

signals:
    void updateSelectionRequested();
    void notebookRenamed(qint32 lid, QString oldName, QString newName);
    void notebookDeleted(qint32 lid, QString name);
    void notebookAdded(qint32 lid);
    void stackAdded(QString name);
    void stackDeleted(QString name);
    void stackRenamed(QString oldName, QString newName);
    void notebookSelectionChanged(qint32);

public slots:
    void notebookUpdated(qint32 lid, QString name);
    void rebuildTree();
    void buildSelection();
    void addRequested();
    void propertiesRequested();
    void deleteRequested();
    void renameRequested();
    void moveToStackRequested();
    void moveToNewStackRequested();
    void removeFromStackRequested();
    void notebookExpunged(qint32 lid);
    void updateTotals(qint32 lid, qint32 total);
};

#endif // NNOTEBOOKVIEW_H
