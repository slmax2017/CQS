#include "PicLinkTask.h"

PicLinkTask::PicLinkTask(SendThread *pSendThread):
    ITask(pSendThread)
{
}

void PicLinkTask::task(QString url)
{
    TiXmlPrinter  *xmlPri = new TiXmlPrinter;
    TiXmlDocument *doc = new TiXmlDocument;

    TiXmlElement *root = new TiXmlElement("root");
    root->SetAttribute("data", "DATA");

    TiXmlElement *type = new TiXmlElement("type");
    type->SetAttribute("msg", "1007");

    TiXmlElement *pic = new TiXmlElement("pic");
    TiXmlText *text = new TiXmlText(url.toUtf8().data());
    pic->LinkEndChild(text);

    doc->LinkEndChild(root);
    root->LinkEndChild(type);
    root->LinkEndChild(pic);

    doc->Accept(xmlPri);

    //压入传输数据队列
    QString xml = QString(QLatin1String(xmlPri->CStr()));
    m_pSendThread->pushData(xml);

    delete doc; doc = NULL;
    delete xmlPri; xmlPri = NULL;
}
