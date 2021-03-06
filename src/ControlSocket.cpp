// =====================================================================================
// 
//       Filename:  ControlSocket.cpp
//
//    Description:  控制端套接字
//
//        Version:  1.0
//        Created:  2013年08月23日 22时08分08秒
//       Revision:  none
//       Compiler:  cl
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>
#include <QtNetwork>

#include "ControlSocket.h"
#include "MainWindow.h"
#include "FalconXml.h"

ControlSocket::ControlSocket(MainWindow *mainWindow)
{
	this->mainWindow = mainWindow;

	connect(this, SIGNAL(connected()), this, SLOT(sendRequest()));
	connect(this, SIGNAL(disconnected()), this, SLOT(connFailed()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connError()));
	connect(this, SIGNAL(readyRead()), this, SLOT(getServerInfo()));
}

ControlSocket::~ControlSocket()
{

}

void ControlSocket::sendRequest()
{
	mainWindow->setUIEnable(true);
	mainWindow->setStatusLabel(tr("  成功连接到服务器"));
	mainWindow->showTrayInfo(tr("成功连接到服务器"));

	QString strNowTime = QDateTime::currentDateTime().toString(Qt::ISODate);
	mainWindow->sendPhoneMessage(QString(tr("监控端于")) + strNowTime + tr("连接到服务器，") + tr("登录IP：") + getIP());

	qDebug() << strNowTime << "成功连接到服务器\r\n";

	if (mainWindow->settingInfo.isPlaySound) {
		QString dir = QCoreApplication::applicationDirPath(); 
		QString filename(dir + "/sound/ConnServerSucceed.wav"); 
		QSound::play(filename);
	}

	sendControlCommand(GetClientListInfo);
}

void ControlSocket::sendControlCommand(XmlInfoType type, QString strCmd)
{
	if (type != GetClientListInfo && mainWindow->strNowSelectIP.size() == 0) {
		QMessageBox::warning(NULL, tr("未找到操作IP"), tr("请在左侧用户列表栏目选择要操作的IP!"));
		return;
	}
	QByteArray sendCommand = FalconXml::CreaterControlCommandXml(type, mainWindow->strNowSelectIP, strCmd);

	qDebug() << "向服务端发送数据" << sendCommand << "\r\n";

	this->write(sendCommand);

	// 目测 write 是有缓冲的，避免沾包，立即刷新
	this->flush();
}

void ControlSocket::readFullXml(QByteArray &recvData)
{
	QByteArray tmpRecv;
	recvData = this->readAll();

	while (FalconXml::checkXmlData(recvData) == false) {
		
		qDebug() << "接收数据不完整。继续读取\r\n";
		
		tmpRecv = this->readAll();
		if (tmpRecv.size() == 0 && !this->waitForReadyRead()) {
			break;
		}
		recvData += tmpRecv;
	}
}

void ControlSocket::getServerInfo()
{
	QByteArray recvData;
	QString dir = QCoreApplication::applicationDirPath();
	QString filename; 

	this->readFullXml(recvData);

	QVector<GroupWithClient> vectClient;
	QVector<ClientProcessInfo> vectProcessInfo;
	ClientFluxInfo clientFluxInfo;
	QString strViewInfo;
	QString strIP;
	switch (FalconXml::getXmlInfoType(recvData)) {
		// 客户端列表信息包 1
		case ClientListInfo:
			qDebug() << "消息识别为客户端列表信息\r\n";
			FalconXml::ResolvingClientListInfoXml(recvData, vectClient);
			mainWindow->setClientList(vectClient);
			break;
		// 客户端上报处理结果信息包 2
		case ClientDisposeInfo:
			qDebug() << "消息识别为客户端上报处理结果信息\r\n";
			FalconXml::ResolvingClientDisposeInfoXml(recvData, strViewInfo, strIP);
			mainWindow->showTrayInfo(strViewInfo);
			addClientLog(strIP, strViewInfo);
			break;
		// 桌面截图信息包 3
		case DesktopScreen:
			qDebug() << "消息识别为桌面截图信息\r\n";
			FalconXml::ResolvingDesktopScreenXml(recvData);
			break;
		// 客户端网络流量信息包 4
		case FluxInfo:
			qDebug() << "消息识别为流量信息\r\n";
			FalconXml::ResolvingFluxInfoXml(recvData, clientFluxInfo);
			mainWindow->setClientFluxInfo(clientFluxInfo);
			break;
		// 进程信息包 5
		case ProcessInfo:
			qDebug() << "消息识别为客户端进程信息\r\n";
			FalconXml::ResolvingProcessInfoXml(recvData, vectProcessInfo);
			mainWindow->setClientProcessInfo(vectProcessInfo);
			break;
		// 硬件信息包 6
		case hardInfo:
			qDebug() << "消息识别为客户端硬件信息\r\n";
			FalconXml::ResolvingHardInfoXml(recvData, strViewInfo);
			mainWindow->setClientHardInfo(strViewInfo);
			break;
		// 日志信息包 7
		case LogInfo:
			qDebug() << "消息识别为客户端日志信息\r\n";
			FalconXml::ResolvingLogInfoXml(recvData, strViewInfo);
			mainWindow->setClientLogInfo(strViewInfo);
			break;
		// CMD 处理信息包 8
		case CmdInfo:
			qDebug() << "消息识别为CMD处理信息\r\n";
			FalconXml::ResolvingCmdInfoXml(recvData, strViewInfo);
			mainWindow->setClientCmdInfo(strViewInfo);
			break;
		// 黑名单信息包
		case BlackList:
			qDebug() << "消息识别为黑名单信息\r\n";
			mainWindow->vectBlackList.clear();
			FalconXml::ResolvingBlackListInfoXml(recvData, mainWindow->vectBlackList);
			mainWindow->setBlackList();
			break;
		// 新用户连接包 10
		case NewClientConn:
			qDebug() << "消息识别为新用户连接包信息\r\n";
			if (mainWindow->settingInfo.isPlaySound) {
				filename = dir + "/sound/ClientLogin.wav";
				QSound::play(filename);
			}
			FalconXml::ResolvingClientListInfoXml(recvData, vectClient);
			mainWindow->setClientList(vectClient);
			break;
		// 客户端离线信息 11
		case ClientOffLine:
			qDebug() << "消息识别为客户端离线包信息\r\n";
			FalconXml::ResolvingClientListInfoXml(recvData, vectClient);
			mainWindow->setClientOffList(vectClient);
			break;
		default:
			qDebug() << "收到无法识别的服务器消息\r\n";
	}
}

void ControlSocket::connFailed()
{
	qDebug() << "服务器连接套接字被关闭\r\n";
	this->close();
	mainWindow->setUIEnable(false);
	if (mainWindow->settingInfo.isPlaySound) {
		QString dir = QCoreApplication::applicationDirPath(); 
		QString filename(dir + "/sound/ConnServerFailed.wav"); 
		QSound::play(filename);
	}
	if (mainWindow->settingInfo.isReConn) {
		mainWindow->startUdpFoundServer();
	}
}

void ControlSocket::connError()
{
	qDebug() << "服务器套接字错误：" << this->errorString() << "\r\n";
	mainWindow->showTrayInfo(tr("连接服务器错误：") + this->errorString());
	mainWindow->setStatusLabel(QString(tr("  连接服务器错误：")) + this->errorString());
	mainWindow->setUIEnable(false);
	if (mainWindow->settingInfo.isPlaySound) {
		QString dir = QCoreApplication::applicationDirPath(); 
		QString filename(dir + "/sound/ConnServerFailed.wav"); 
		QSound::play(filename);
	}
	if (mainWindow->settingInfo.isReConn) {
		mainWindow->startUdpFoundServer();
	}
}

QString ControlSocket::getIP()
{
	QList<QHostAddress> list = QNetworkInterface::allAddresses();

	foreach (QHostAddress address, list) {
		// 只获取 IPv4 协议地址
		if (address.protocol() == QAbstractSocket::IPv4Protocol 
			&& address.toString() != "127.0.0.1") {
			return address.toString();
		}
	}

	return QString("");
}

void ControlSocket::addClientLog(QString strIP, QString strLogInfo)
{
	QString strNowDate = QDate::currentDate().toString("yyyy-MM-dd");
	QString strLogFileName = strNowDate + ".log";

	QFile logFile(strLogFileName);

	if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		return;
 	}

 	QTextStream out(&logFile);
 	out << strLogInfo << "\r\n";

	QVector<ClientLogInfo>::iterator index = vectClientInfo.begin();
	for (; index != vectClientInfo.end(); ++index) {
		if (index->strIP == strIP) {
			index->vectLogInfo.push_back(strLogInfo);
			return;
		}
	}
	ClientLogInfo tmpLogInfo;
	tmpLogInfo.strIP = strIP;
	tmpLogInfo.vectLogInfo.push_back(strLogInfo);
	
	vectClientInfo.push_back(tmpLogInfo);
}

void ControlSocket::getLogInfo(QString strIP, QString &strLogInfo)
{
	QVector<ClientLogInfo>::iterator index = vectClientInfo.begin();
	for (; index != vectClientInfo.end(); ++index) {
		if (index->strIP == strIP) {
			QVector<QString>::iterator info = index->vectLogInfo.begin();
			for (; info != index->vectLogInfo.end(); ++info) {
				strLogInfo += *info;
				strLogInfo += "\r\n";
			}
		}
	}
}
