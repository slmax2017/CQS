#include "widget.h"
#include "ui_widget.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "TaskFactory.h"

#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_model(NULL),
    m_pManager(NULL)
{
    ui->setupUi(this);

    //连接服务器
    connectToHost();

    //启动发消息线程
    startSendThread();

    //启动接收消息线程
    startRecvThread();

    //界面初始化
    init();

    //接收线程信号
    connect(m_pRecvThread, &ReceThread::sendToMain, this, &Widget::recvReader);
    connect(m_pSendThread, &SendThread::sendToMain, this, &Widget::sended);

    //网络资料监控器
    m_pManager = new QNetworkAccessManager(this);
    connect(m_pManager, &QNetworkAccessManager::finished, this, &Widget::loadPixmaps);
}

Widget::~Widget()
{
   /* m_rThread->wait();
    m_rThread->terminate();
    m_sThread->wait();
    m_sThread->terminate();*/
    delete ui;
}

void Widget::init()
{
    //品牌
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        ui->brandCombo->addItem(QString(c));
    }
}

void Widget::handleXml(TiXmlDocument *xmlDoc)
{
    TiXmlElement *rootElem = xmlDoc->RootElement();
    TiXmlElement *typeElem = rootElem->FirstChildElement();
    const char *szType = typeElem->Attribute("msg");

    if (!memcmp("1002", szType, 4))
    {
        loadCarSeries(rootElem);
    }
    else if (!memcmp("1004", szType, 4))
    {
        loadCarData(rootElem);
    }
    else if (!memcmp("1006", szType, 4))
    {
        loadPicData(rootElem);
    }
    else if (!memcmp("1008", szType, 4))
    {
        loadPicLink(rootElem);
    }
}

void Widget::loadCarSeries(TiXmlElement *rootElem)
{
   m_MapCar.clear();

   TiXmlElement *brandElem = rootElem->FirstChildElement()->NextSiblingElement();
   for (; brandElem != NULL; brandElem = brandElem->NextSiblingElement())
   {
       list<CarData> lstData;

       QString brandName =  QString(brandElem->Attribute("name"));
       TiXmlElement *carElem = brandElem->FirstChildElement();
       for (; carElem != NULL; carElem = carElem->NextSiblingElement())
       {
            CarData data;

            data.id = QString(carElem->Attribute("id"));
            data.name = QString(carElem->GetText());
            lstData.push_back(data);
       }

       m_MapCar[brandName] = lstData;
   }

   loadComboBox();
}

void Widget::loadCarData(TiXmlElement *rootElem)
{
    TiXmlElement *onSaleElem = rootElem->FirstChildElement()->NextSiblingElement();
    for (; onSaleElem != NULL; onSaleElem = onSaleElem->NextSiblingElement())
    {
        TiXmlElement *carElem = onSaleElem->FirstChildElement();
        for (; carElem != NULL; carElem = carElem->NextSiblingElement())
        {
            ui->cbxSale->addItem(QString(carElem->Attribute("id")) + "," + QString(carElem->GetText()));
        }
    }

    if (ui->cbxSale->count())
    {
        ui->pushButton->setEnabled(true);
    }
}

void Widget::loadPicData(TiXmlElement *rootElem)
{
    m_MapPic.clear();

    TiXmlElement *kindElem = rootElem->FirstChildElement()->NextSiblingElement();
    for (; kindElem != NULL; kindElem = kindElem->NextSiblingElement())
    {
        list<QString> lstPic;

        QString kindName = QString(kindElem->Attribute("kind"));
        TiXmlElement *linkElem = kindElem->FirstChildElement();
        for (; linkElem != NULL; linkElem = linkElem->NextSiblingElement())
        {
            lstPic.push_back(QString(linkElem->GetText()));
        }

        m_MapPic[kindName] = lstPic;
    }

    loadTreeView();
}

void Widget::loadPicLink(TiXmlElement *rootElem)
{
     TiXmlElement *kindElem = rootElem->FirstChildElement()->NextSiblingElement();
     QString pic_link = kindElem->GetText();

     m_pManager->get(QNetworkRequest(QUrl(pic_link)));
}

void Widget::loadComboBox()
{
    map<QString, list<CarData> >::iterator it = m_MapCar.begin();
    for (; it != m_MapCar.end(); ++it)
    {
        ui->carBrand->addItem(it->first);
    }

    if (ui->carBrand->count() and ui->carName->count())
    {
        ui->btnQuery->setEnabled(true);
    }
}

void Widget::loadTreeView()
{
    DELPTR(m_model);

    m_model = new QStandardItemModel();
    m_model->setHorizontalHeaderLabels(QStringList() << QString("图片"));

    map<QString, list<QString> >::iterator it = m_MapPic.begin();
    for (; it != m_MapPic.end(); ++it)
    {
        QString kindName = it->first;
        list<QString> lstPic = it->second;

        QStandardItem *itemName = new QStandardItem(kindName + QString("(%1)").arg(lstPic.size()));
        m_model->appendRow(itemName);

        list<QString>::iterator itor = lstPic.begin();
        for (; itor != lstPic.end(); ++itor)
        {
            QString link = *itor;
            QStandardItem *itemPic = new QStandardItem(link);
            itemName->appendRow(itemPic);
        }
    }

    ui->treeView->setModel(m_model);
}

void Widget::connectToHost()
{
    //连接服务器
    m_pNet = new NetClient;
    m_pNet->connectHost(QString("127.0.0.1"), 9999);
}

void Widget::startSendThread()
{
    m_sThread = new QThread(this);
    m_pSendThread = new SendThread(m_pNet);
    m_pSendThread->moveToThread(m_sThread);
    connect(m_sThread, &QThread::started, m_pSendThread, &SendThread::running);

    m_sThread->start();
}

void Widget::startRecvThread()
{
    m_rThread = new QThread(this);
    m_pRecvThread = new ReceThread(m_pNet);
    m_pRecvThread->moveToThread(m_rThread);
    connect(m_rThread, &QThread::started, m_pRecvThread, &ReceThread::running);

    m_rThread->start();
}

void Widget::on_brandCombo_currentTextChanged(const QString &arg1)
{
    ui->carBrand->clear();
    ui->cbxSale->clear();

    if (!m_pSendThread)
    {
       QMessageBox::information(this, QString("提示"), QString("线程未启动"), QMessageBox::Yes);
       return;
    }

    IFactory *pFactory = new TaskFactory(m_pSendThread);
    ITask *pTask = pFactory->createTask(1);
    pTask->task(arg1);

    delete pFactory; pFactory = NULL;
}

void Widget::sended(const QString &x)
{
    cout << x;
}

void Widget::recvReader(const QString &xml)
{
    TiXmlDocument *xmlDoc = new TiXmlDocument;
    xmlDoc->Parse(xml.toUtf8().data());
    if (xmlDoc->Error())
    {
        cout << "获取xml内容有误";
        delete xmlDoc; xmlDoc = NULL;
        return;
    }

    handleXml(xmlDoc);

    delete xmlDoc; xmlDoc = NULL;
}

void Widget::on_carBrand_currentTextChanged(const QString &arg1)
{
    ui->carName->clear();

    list<CarData> lst = m_MapCar[arg1];
    list<CarData>::iterator it = lst.begin();
    for (; it != lst.end(); ++it)
    {
        CarData data = *it;
        ui->carName->addItem(QString(data.name + "," + data.id));
    }

    if (ui->carName->count())
    {
        ui->btnQuery->setEnabled(true);
    }
}

void Widget::on_btnQuery_clicked()
{
    ui->cbxSale->clear();

    if (!m_pSendThread)
    {
        QMessageBox::information(this, QString("提示"), QString("线程未启动"), QMessageBox::Yes);
        return;
    }

    QString item = ui->carName->currentText();
    if (!item.length())
    {
       QMessageBox::information(this, QString("提示"), QString("没有选择车系!"), QMessageBox::Yes);
       return;
    }

    ui->btnQuery->setEnabled(false);

    QStringList idList = item.split(",");
    QString id = idList.at(1);

    IFactory *pFactory = new TaskFactory(m_pSendThread);
    ITask *pTask = pFactory->createTask(2);

    pTask->task(id);

    delete pFactory; pFactory = NULL;
}

void Widget::on_pushButton_clicked()
{
    if (!m_pSendThread)
    {
        QMessageBox::information(this, QString("提示"), QString("线程未启动"), QMessageBox::Yes);
        return;
    }

    QString item = ui->cbxSale->currentText();
    if (!item.length())
    {
       QMessageBox::information(this, QString("提示"), QString("没有选择车系!"), QMessageBox::Yes);
       return;
    }

    ui->pushButton->setEnabled(false);

    QStringList idList = item.split(",");
    QString id = idList.at(0);

    IFactory *pFactory = new TaskFactory(m_pSendThread);
    ITask *pTask = pFactory->createTask(3);

    pTask->task(id);

    delete pFactory; pFactory = NULL;
}

void Widget::on_treeView_clicked(const QModelIndex &index)
{
    QString pic_page = index.data().toString();

    IFactory *pFactory = new TaskFactory(m_pSendThread);
    ITask *pTask = pFactory->createTask(4);

    pTask->task(pic_page);

    delete pFactory;
    pFactory = NULL;
}

void Widget::loadPixmaps(QNetworkReply *reply)
{
    QByteArray array = reply->readAll();

    cout << array.length();

    QPixmap pix;
    pix.loadFromData(array);

    ui->label_4->setPixmap(pix);
}
