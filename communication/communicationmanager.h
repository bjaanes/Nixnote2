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

#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include "global.h"

#include <QString>
#include <evernote/NoteStore.h>
#include <evernote/UserStore.h>
#include "communication/communicationerror.h"

#include <inttypes.h>
#include <iostream>
#include <netinet/in.h>
#include <stdint.h>

#include <protocol/TBinaryProtocol.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TSSLServerSocket.h>
#include <transport/TSSLSocket.h>
#include <transport/THttpClient.h>
#include <server/TSimpleServer.h>
#include <QObject>
#include <QNetworkAccessManager>

using boost::shared_ptr;

using namespace std;
using namespace evernote::edam;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

//* Userid/password key & secret
//#define EDAM_CONSUMER_KEY "baumgarte"
//#define EDAM_CONSUMER_SECRET "eb8b5740e17cb55f"


//* OAuth key & secret
//#define EDAM_CONSUMER_KEY = "baumgarr"
#define EDAM_CONSUMER_KEY = "baumgarr-3523"
#define EDAM_CONSUMER_SECRET = "8d5ee175f8a5d3ec"


//#define EVERNOTE_HOST   "www.evernote.com"
//#define EDAM_USER_STORE_PATH "/edam/user"
//#define EDAM_NOTE_STORE_PATH "/edam/note/s1"
//#define EDAM_CLIENT_NAME "NixNote/Linux"

class CommunicationManager : public QObject
{
    Q_OBJECT

private:
    bool initComplete;
    bool initNoteStore();
    bool initUserStore();

    string userStorePath;
    string noteStorePath;
    string clientName;
    string evernoteHost;
    shared_ptr<TSocket> sslSocketNoteStore;
    shared_ptr<TTransport> noteStoreHttpClient;
    shared_ptr<NoteStoreClient> noteStoreClient;

    string linkedNoteStorePath;
    shared_ptr<TSocket> linkedSslSocketNoteStore;
    shared_ptr<TTransport> linkedNoteStoreHttpClient;
    shared_ptr<NoteStoreClient> linkedNoteStoreClient;
    AuthenticationResult linkedAuthToken;

    void downloadInkNoteImage(QString guid, Resource *r);

    shared_ptr<TSocket> sslSocketUserStore;
    shared_ptr<TTransport> userStoreHttpClient;
    shared_ptr<UserStoreClient> userStoreClient;
    //shared_ptr<AuthenticationResult> authenticationResult;
    string authToken;
    bool init();
    QNetworkAccessManager *networkAccessManager;
    void checkForInkNotes(vector<Resource> &resources);

public:
    CommunicationManager(QObject *parent = 0);
    ~CommunicationManager();
    CommunicationError error;
    bool connect();
    bool getSyncState(string authToken, SyncState &syncState);
    bool getSyncChunk(string token, SyncChunk &chunk, int start, int chunkSize, bool fullSync=false);
    bool authenticateToLinkedNotebookShard(LinkedNotebook book);
    bool getLinkedNotebookSyncState(SyncState &syncState, LinkedNotebook book);
    bool getLinkedNotebookSyncChunk(SyncChunk &chunk, LinkedNotebook book, int start, int chunkSize, bool fullSync=false);
    string getToken();
    void disconnect();
    void disconnectFromLinkedNotebook();
    bool authenticateToLinkedNotebook(AuthenticationResult &authResult, LinkedNotebook book);
    bool getUserInfo(User &user);
    QList< QPair<QString, QImage*>* > *inkNoteList;
    bool getSharedNotebookByAuth(SharedNotebook &sharedNotebook);

    qint32 uploadSavedSearch(SavedSearch &search);
    qint32 expungeSavedSearch(string guid);

    qint32 uploadTag(Tag &tag);
    qint32 expungeTag(string guid);

    qint32 uploadNotebook(Notebook &notebook);
    qint32 expungeNotebook(string guid);

    qint32 uploadNote(Note &note);

public slots:
    int inkNoteReady(QImage *newImage, QImage *replyImage, int position);
};

#endif // COMMUNICATIONMANAGER_H
