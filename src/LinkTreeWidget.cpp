// =====================================================================================
// 
//       Filename:  LinkTreeWidget.cpp
//
//    Description:  自动上线主机显示
//
//        Version:  1.0
//        Created:  2013年08月20日 11时08分08秒
//       Revision:  none
//       Compiler:  cl
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "LinkTreeWidget.h"
#include "MainStackWidget.h"
#include "LinkProcessWidget.h"
#include "LinkCmdWidget.h"
#include "FluxViewWidget.h"
#include "InfoWidget.h"
#include "MainWindow.h"
#include "ControlSocket.h"
#include "FalconType.h"

LinkTreeWidget::LinkTreeWidget(MainWindow *mainWindow)
{
	qDebug() << "LinkTreeWidget 控件初始化\r\n";

	this->mainWindow = mainWindow;
	connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			 this, SLOT(changeNowSelectIP(QTreeWidgetItem *)));
	setHeaderLabel(QString(tr("局域网主机管理")));
}

LinkTreeWidget::~LinkTreeWidget()
{

}

void LinkTreeWidget::changeNowSelectIP(QTreeWidgetItem *current)
{
	QRegExp regExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");

	// 正则表达式匹配是否选择了 IP
	if (regExp.exactMatch(current->text(0))) {
		
		strNowSelectIP = current->text(0);
		mainWindow->strNowSelectIP = strNowSelectIP;
		
		qDebug() << "当前列表框选择的IP更新为：" << strNowSelectIP << "\r\n";

		// 当前选择的控件编号
		int index = mainWindow->getStackWidget()->getStackLayout()->currentIndex();

		qDebug() << "当前界面控件编号为：" << index << "\r\n";

		switch (index) {
			case 2:
				((FluxViewWidget *)mainWindow->getStackWidget()->getStackLayout()->widget(2))->resetFluxView();
				mainWindow->getConnSocket()->sendControlCommand(GetFluxInfo);
				qDebug() << "切换客户端自动触发发送获取流量信息命令" << "\r\n";
				break;
			case 3:
				((LinkProcessWidget *)mainWindow->getStackWidget()->getStackLayout()->widget(3))->resetInfo();
				mainWindow->getConnSocket()->sendControlCommand(GetProcessInfo);
				qDebug() << "切换客户端自动触发发送获取进程信息命令" << "\r\n";
				break;
			case 4:
				((LinkCmdWidget *)mainWindow->getStackWidget()->getStackLayout()->widget(4))->resetCmdInfo();
				break;
			case 5:
				((InfoWidget *)mainWindow->getStackWidget()->getStackLayout()->widget(5))->resetView();
				qDebug() << "切换客户端自动触发发送获取硬件信息命令" << "\r\n";
				mainWindow->getConnSocket()->sendControlCommand(GethardInfo);
				break;
			default:
				break;
		}
	}
}

/*================================================================
*  函 数 名： LinkTreeWidget::addGroupItem
*
*  功能描述： 添加一个组名
*
*  参    数： QString &strTitle   组名
*			  
*			  
*  返 回 值：bool always true
*
*  作    者：刘欢 2013/8
================================================================*/
bool LinkTreeWidget::addGroupItem(QString strTitle)
{
	QTreeWidgetItem *imageItem = new QTreeWidgetItem(this, QStringList(strTitle));
	imageItem->setIcon(0, QIcon(":/res/images/group.png"));

	return true;
}

/*================================================================
*  函 数 名： LinkTreeWidget::addComputerItem
*
*  功能描述： 列表增加一个连接计算机信息
*
*  参    数： QString &strGroup —— 要添加的组名
*			  QString &strTitle —— 要添加的计算机命名
*			  bool isOnline —— 该计算机是否连接上服务端
*  返 回 值： bool —— 成功或失败
*
*  作    者：刘欢 2013/8
================================================================*/
bool LinkTreeWidget::addComputerItem(QString strGroup, QString strTitle, bool isOnline)
{
	QTreeWidgetItemIterator it(this);

	while (*it) {
		if ((*it)->text(0) == strGroup) {
				QTreeWidgetItem *item = new QTreeWidgetItem(*it, QStringList(strTitle));
				if (isOnline) {
					item->setIcon(0, QIcon(":/res/images/computer_online.png"));
				} else {
					item->setIcon(0, QIcon(":/res/images/computer_offline.png"));
				}
				
				(*it)->addChild(item);
			return true;
		}
		++it;
	}

	return false;
}

/*================================================================
*  函 数 名： LinkTreeWidget::removeComputerItem
*
*  功能描述： 列表删除一个连接计算机信息
*
*  参    数： QString &strTitle —— 要添加的计算机命名
*			  
*  返 回 值： bool —— 成功或失败
*
*  作    者：刘欢 2013/8
================================================================*/
bool LinkTreeWidget::removeComputerItem(QString strComputer)
{
	QTreeWidgetItemIterator it(this);

	while (*it) {
		QTreeWidgetItemIterator child(it);
		while(*child) {
			if ((*child)->text(0) == strComputer) {
				
				// TODO BUGS
				//this->removeItemWidget(*child, 1);
				return true;
			}
			++child;
		}
		++it;
	}

	return false;
}
