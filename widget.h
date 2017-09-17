#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <map>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QUrl>
#include <list>
#include "sendthread.h"
#include "ReceThread.h"
#include "netclient.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Define.h"
using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public:
    void init();
    void handleXml(TiXmlDocument *xmlDoc);
    void loadCarSeries(TiXmlElement *elem);
    void loadCarData(TiXmlElement *elem);
    void loadPicData(TiXmlElement *elem);
    void loadPicLink(TiXmlElement *elem);
    void loadComboBox();
    void loadTreeView();
    void connectToHost();
    void startSendThread();
    void startRecvThread();
private slots:
    void on_brandCombo_currentTextChanged(const QString &arg1);
    void sended(const QString &x);
    void recvReader(const QString &xml);

    void on_carBrand_currentTextChanged(const QString &arg1);

    void on_btnQuery_clicked();

    void on_pushButton_clicked();

    void on_treeView_clicked(const QModelIndex &index);
    void loadPixmaps(QNetworkReply *reply);
private:
    Ui::Widget *ui;
    SendThread *m_pSendThread;
    ReceThread *m_pRecvThread;
    QThread *m_sThread;
    QThread *m_rThread;
    NetClient *m_pNet;
    QStandardItemModel *m_model;
    QNetworkAccessManager *m_pManager;
private:
    map<QString, list<CarData> > m_MapCar;
    map<QString, list<QString> > m_MapPic;
};

#endif // WIDGET_H
