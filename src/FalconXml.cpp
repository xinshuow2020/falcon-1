// =====================================================================================
// 
//       Filename:  FalconXml.cpp
//
//    Description:  XML 解析
//
//        Version:  1.0
//        Created:  2013年08月23日 21时08分08秒
//       Revision:  none
//       Compiler:  cl
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "FalconXml.h"
#include "FalconType.h"

// 构造控制指令XML包
QByteArray FalconXml::CreaterControlCommandXml(XmlInfoType strType, QString strIP, QString strCmd)
{
	QByteArray strXml;
	QXmlStreamWriter stream(&strXml);

	stream.setAutoFormatting(true);

	stream.writeStartDocument();
	stream.writeStartElement("falcon");
	stream.writeAttribute("type", QString::number(strType));

	stream.writeTextElement("ip", strIP);
	stream.writeTextElement("info", strCmd);

	stream.writeEndElement();
	stream.writeEndDocument();

	return strXml;
}

XmlInfoType FalconXml::getXmlInfoType(QByteArray &strXml)
{
	QXmlStreamReader reader(strXml);

	 while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "falcon" && reader.attributes().hasAttribute("type")) {
				return (XmlInfoType)(reader.attributes().value("type").toString().toInt());
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "检测类型错误 XML Error:" << reader.errorString() << "\r\n";
	}

	return NoType;
}

// 检查一个XML包是否接收完整
bool FalconXml::checkXmlData(QByteArray &strData)
{
	return (strData.indexOf("</falcon>") != -1);
}

/*
<!-- 服务端发送的客户端列表信息XML包 -->
<?xml version="1.0" encoding="UTF-8"?>
	<falcon type="具体包的类型">
	<version>客户端版本号</version>
</falcon>
*/
// 构造寻找服务端的UDP发送XML包
QByteArray FalconXml::CreateUdpFoundServerXml()
{
	QByteArray strXml;
	QXmlStreamWriter stream(&strXml);

	stream.setAutoFormatting(true);

	stream.writeStartDocument();
	stream.writeStartElement("falcon");
	stream.writeAttribute("type", QString::number(FoundServerInfo));

	stream.writeTextElement("version", "1.0");

	stream.writeEndElement();
	stream.writeEndDocument();

	return strXml;
}

/*
<!-- 服务端发送的客户端列表信息XML包 -->
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<group name="组名1">
		<ip>客户机IP地址 1 </ip>
		<ip>客户机IP地址 2 </ip>
		<ip>客户机IP地址 3 </ip>
		<ip>客户机IP地址 n </ip>
	</group>
	<group name="组名2">
		<ip>客户机IP地址 1 </ip>
		<ip>客户机IP地址 2 </ip>
		<ip>客户机IP地址 3 </ip>
		<ip>客户机IP地址 n </ip>
	</group>
</falcon>
*/
// 解析客户端列表XML包
void FalconXml::ResolvingClientListInfoXml(QByteArray&strXml, QVector<GroupWithClient> &vectClient)
{
	QXmlStreamReader reader(strXml);
	GroupWithClient tmpGroupInfo;

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "group" && reader.attributes().hasAttribute("name")) {
				tmpGroupInfo.strGroup = reader.attributes().value("name").toString();
			} else if (reader.name() == "ip") {
				tmpGroupInfo.vectClient.push_back(reader.readElementText(QXmlStreamReader::SkipChildElements));
			}
		}
		if (type == QXmlStreamReader::EndElement) {
			if (reader.name() == "group") {
				vectClient.push_back(tmpGroupInfo);
				tmpGroupInfo.vectClient.clear();
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "解析客户端列表 XML Error:" << reader.errorString() << "\r\n";
	}
}

/*
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<date>上报日期</date>
	<time>上报时间</time>
	<ip>客户端IP</ip>
    <info>具体上报信息</info>
</falcon>
*/
// 解析客户端上报处理结果XML包
void FalconXml::ResolvingClientDisposeInfoXml(QByteArray &strXml, QString &showInfo, QString &strIP)
{
	QXmlStreamReader reader(strXml);
	showInfo = "[";

	qDebug() << strXml << "\r\n";

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "date") {
				showInfo += reader.readElementText(QXmlStreamReader::SkipChildElements);
				showInfo += " ";
			}
			else if (reader.name() == "time") {
				showInfo += reader.readElementText(QXmlStreamReader::SkipChildElements);
				showInfo += "] 客户端";
			} else if (reader.name() == "ip") {
				strIP = reader.readElementText(QXmlStreamReader::SkipChildElements);
				showInfo += strIP;
				showInfo += " ";
			} else if (reader.name() == "info") {
				showInfo += reader.readElementText(QXmlStreamReader::SkipChildElements);
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "客户端上报处理结果 XML Error:" << reader.errorString() << "\r\n";;
	}
}

/*

*/
// 解析桌面截图XML包
void FalconXml::ResolvingDesktopScreenXml(QByteArray &strXml)
{
	strXml = "";
}

/*
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<ip>客户端IP</ip>
    <upload>上传速度</upload>
    <download>下载速度</download>
</falcon>
*/
// 解析客户端网络流量信息XML包
void FalconXml::ResolvingFluxInfoXml(QByteArray &strXml, ClientFluxInfo &clientFluxInfo)
{
	QXmlStreamReader reader(strXml);

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "ip") {
				clientFluxInfo.strClientIP = reader.readElementText(QXmlStreamReader::SkipChildElements);
			}
			if (reader.name() == "upload") {
				clientFluxInfo.uploadSpeed = reader.readElementText(QXmlStreamReader::SkipChildElements).toDouble();
			}
			else if (reader.name() == "download") {
				clientFluxInfo.downloadSpeed = reader.readElementText(QXmlStreamReader::SkipChildElements).toDouble();
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "客户端网络流量信息 XML Error:" << reader.errorString() << "\r\n";
	}
}

/*
<!-- 监控端接收的进程信息XML包 -->
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<process name="进程名 1 ">
		<pid>进程PID</pid>
		<mem>使用内存</mem>
		<path>映像路径</path>
		<description>/进程描述</description>
	</process>
	<process name="进程名 2 ">
		<pid>进程PID</pid>
		<mem>使用内存</mem>
		<path>映像路径</path>
		<description>/进程描述</description>
	</process>
	<process name="进程名 n ">
		<pid>进程PID</pid>
		<mem>使用内存</mem>
		<path>映像路径</path>
		<description>/进程描述</description>
	</process>
</falcon>
*/
// 解析客户端进程信息XML包
void FalconXml::ResolvingProcessInfoXml(QByteArray &strXml, QVector<ClientProcessInfo> & vectProcessInfo)
{
	QXmlStreamReader reader(strXml);
	ClientProcessInfo tmpProcessInfo;

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "process" && reader.attributes().hasAttribute("name")) {
				tmpProcessInfo.process = reader.attributes().value("name").toString();
			} else if (reader.name() == "pid") {
				tmpProcessInfo.pid = reader.readElementText(QXmlStreamReader::SkipChildElements);
			} else if (reader.name() == "mem") {
				tmpProcessInfo.mem = reader.readElementText(QXmlStreamReader::SkipChildElements);
				tmpProcessInfo.mem += " K";
			} else if (reader.name() == "path") {
				tmpProcessInfo.path = reader.readElementText(QXmlStreamReader::SkipChildElements);
			} else if (reader.name() == "description") {
				tmpProcessInfo.description = reader.readElementText(QXmlStreamReader::SkipChildElements);
			}
		}
		if (type == QXmlStreamReader::EndElement) {
			if (reader.name() == "process") {
				vectProcessInfo.push_back(tmpProcessInfo);
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "客户端进程信息X XML Error:" << reader.errorString() << "\r\n";
	}
}

/*
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<info>CMD处理信息</info>
</falcon>
*/
// 解析CMD处理信息XML包
void FalconXml::ResolvingCmdInfoXml(QByteArray &strXml, QString &strCmd)
{
	QXmlStreamReader reader(strXml);

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "info") {
				strCmd = reader.readElementText(QXmlStreamReader::SkipChildElements);
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "解析CMD处理信息 XML Error:" << reader.errorString() << "\r\n";
	}
}

/*
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<os>操作系统</os>
	<hostname>计算机名</hostname>
	<cpu>CPU信息</cpu>
	<mem>内存信息</mem>
	<net>网卡信息</net>
	<harddisk>硬盘信息</harddisk>
	<starttime>开机时间<starttime>
</falcon>
*/
// 解析客户端硬件信息XML包
void FalconXml::ResolvingHardInfoXml(QByteArray &strXml, QString &strHard)
{
	QXmlStreamReader reader(strXml);

	strHard = "客户端硬件信息:\r\n\r\n";

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "os") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			}else if (reader.name() == "hostname") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			} else if (reader.name() == "cpu") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			} else if (reader.name() == "mem") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			} else if (reader.name() == "net") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			} else if (reader.name() == "harddisk") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			} else if (reader.name() == "starttime") {
				strHard += reader.readElementText(QXmlStreamReader::SkipChildElements);
				strHard += "\r\n";
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "XML Error:" << reader.errorString() << "\r\n";
	}
}

/*
<?xml version="1.0" encoding="UTF-8"?>
<falcon type="具体包的类型">
	<info>附加信息</info>
</falcon>
*/
// 解析客户端日志信息XML包
void FalconXml::ResolvingLogInfoXml(QByteArray &strXml, QString &strLog)
{
	QXmlStreamReader reader(strXml);

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "info") {
				strLog = reader.readElementText(QXmlStreamReader::SkipChildElements);
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "XML Error:" << reader.errorString() << "\r\n";
	}
}

/*
<!-- 服务端下发进程黑名单的XML包 -->
<?xml version="1.0" encoding="UTF-8"?>
	<falcon type="具体包的类型">
	<process>进程名 1 </process>
	<process>进程名 2 </process>
	<process>进程名 3 </process>
	……
	<process>进程名 n </process>
</falcon>
*/
// 解析服务端黑名单XML包
void FalconXml::ResolvingBlackListInfoXml(QByteArray &strXml, QVector<QString> &vectBlackList)
{
	QXmlStreamReader reader(strXml);

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType type = reader.readNext();

		if (type == QXmlStreamReader::StartElement) {
			if (reader.name() == "process") {
				vectBlackList.push_back(reader.readElementText(QXmlStreamReader::SkipChildElements));
			}
		}
	}

	if (reader.hasError()) {
		qDebug() << "XML Error:" << reader.errorString() << "\r\n";
	}
}

// 构造服务端黑名单包
QByteArray FalconXml::CreateBlackListInfoXml(QVector<QString> &vectBlackList)
{
	QByteArray strXml;
	QXmlStreamWriter stream(&strXml);

	stream.setAutoFormatting(true);

	stream.writeStartDocument();
	stream.writeStartElement("falcon");
	stream.writeAttribute("type", QString::number(ChangeBlackList));

	QVector<QString>::iterator index = vectBlackList.begin();
	for (; index != vectBlackList.end(); ++index) {
		stream.writeTextElement("process", *index);
	}

	stream.writeEndElement();
	stream.writeEndDocument();

	return strXml;
}