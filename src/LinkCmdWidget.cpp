// =====================================================================================
// 
//       Filename:  LinkCmdWidget.cpp
//
//    Description:  cmd 管理标签 Widget
//
//        Version:  1.0
//        Created:  2013年08月21日 09时34分08秒
//       Revision:  none
//       Compiler:  cl
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "LinkCmdWidget.h"

LinkCmdWidget::LinkCmdWidget()
{
	qDebug() << "LinkCmdWidget 控件初始化\r\n";
	setStyleSheet("QGroupBox {border-width:1px; border-style:solid; border-color:gray;}");
	createMainWeiget();
}

LinkCmdWidget::~LinkCmdWidget()
{

}

/*================================================================
*  函 数 名： LinkCmdWidget::createMainWeiget
*
*  功能描述： 创建主显示控件
*
*  参    数： 无
*			  
*  返 回 值： 无
*
*  作    者：刘欢 2013/8
================================================================*/
void LinkCmdWidget::createMainWeiget()
{
	cmdText = new QTextBrowser();
	cmdText->setReadOnly(true);
	cmdText->setStyleSheet("QTextBrowser{ background-color:black; }");
	cmdText->setTextColor(Qt::green);
	cmdText->setCurrentFont(QFont("新宋体", 10));

	resetCmdInfo();

	cmdLabel = new QLabel(tr("命令输入"));
	cmdInputEdit = new QLineEdit();
	sendButton = new QPushButton(tr("远程执行"));

	cmdLayout = new QVBoxLayout();
	cmdButtonLayout = new QHBoxLayout();

	cmdButtonLayout->addWidget(cmdLabel);
	cmdButtonLayout->addWidget(cmdInputEdit);
	cmdButtonLayout->addWidget(sendButton);

	cmdLayout->addWidget(cmdText);
	cmdLayout->addLayout(cmdButtonLayout);

	setLayout(cmdLayout);
}


/*================================================================
*  函 数 名： LinkCmdWidget::resetCmdInfo
*
*  功能描述： 重置显示控件
*
*  参    数： 无
*			  
*  返 回 值： 无
*
*  作    者：刘欢 2013/8
================================================================*/
void LinkCmdWidget::resetCmdInfo()
{
	cmdText->clear();
	cmdText->append(tr("Microsoft Windows \n版权所有 (c) Microsoft Corporation。保留所有权利。\n\nC:\\>_"));
}

