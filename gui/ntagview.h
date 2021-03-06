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

#ifndef NTAGVIEW_H
#define NTAGVIEW_H
#include "ntagviewitem.h"
#include "treewidgeteditor.h"

#include <QTreeWidget>
#include <QShortcut>
#include <QMenu>
#include <QAction>

class NTagView : public QTreeWidget
{
    Q_OBJECT
private:
    NTagViewItem *root;
    virtual void mousePressEvent(QMouseEvent *event);
    int filterPosition;
    void copyTreeItem(QTreeWidgetItem *source, QTreeWidgetItem *target);
    QMenu context;
    QAction *addAction;
    QAction *propertiesAction;
    QAction *deleteAction;
    QAction *renameAction;
    QAction *hideUnassignedAction;
    QShortcut *renameShortcut;
    QShortcut *addShortcut;
    QShortcut *deleteShortcut;
    TreeWidgetEditor *editor;
    qint32 accountFilter;

private slots:
    int calculateHeightRec(QTreeWidgetItem * item);
    void calculateHeight();
    void editComplete();

public:
    explicit NTagView(QWidget *parent = 0);
    ~NTagView();
    void resetSize();
    bool hideUnassigned;
    void updateSelection();
    void loadData();
    bool rebuildTagTreeNeeded;
    void dragEnterEvent(QDragEnterEvent *event);
    bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
    void mouseMoveEvent(QMouseEvent *event);
    void dropEvent(QDropEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    QHash<qint32, NTagViewItem*> dataStore;


signals:
    void updateSelectionRequested();
    void tagRenamed(qint32 lid, QString oldName, QString newName);
    void tagDeleted(qint32 lid, QString name);
    void tagAdded(qint32 lid);

public slots:
    void tagUpdated(qint32 lid, QString name);
    void rebuildTree();
    void buildSelection();
    void addNewTag(qint32 lid);
    void addRequested();
    void propertiesRequested();
    void deleteRequested();
    void renameRequested();
    void tagExpunged(qint32 lid);
    void updateTotals(qint32 lid, qint32 total);
    void hideUnassignedTags();
    void notebookSelectionChanged(qint32 notebookLid);

protected:
    void dragMoveEvent(QDragMoveEvent *event);

};

#endif // NTAGVIEW_H
