#include "CarPicTask.h"

CarPicTask::CarPicTask(SendThread *pSendThread)
    :ITask(pSendThread)
{

}

CarPicTask::~CarPicTask(){}

void CarPicTask::task(QString x)
{
    TiXmlPrinter  *xmlPri = new TiXmlPrinter;
    TiXmlDocument *doc = new TiXmlDocument;

    TiXmlElement *root = new TiXmlElement("root");
    root->SetAttribute("data", "DATA");

    TiXmlElement *type = new TiXmlElement("type");
    type->SetAttribute("msg", "1005");

    TiXmlElement *brand = new TiXmlElement("CarID");
    TiXmlText *text = new TiXmlText(x.toUtf8().data());
    brand->LinkEndChild(text);

    doc->LinkEndChild(root);
    root->LinkEndChild(type);
    root->LinkEndChild(brand);

    doc->Accept(xmlPri);

    //压入传输数据队列
    QString xml = QString(QLatin1String(xmlPri->CStr()));
    m_pSendThread->pushData(xml);

    delete doc; doc = NULL;
    delete xmlPri; xmlPri = NULL;
}
