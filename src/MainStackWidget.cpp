// =====================================================================================
// 
//       Filename:  MainStackWidget.cpp
//
//    Description:  标签显示管理
//
//        Version:  1.0
//        Created:  2013年08月18日 14时05分08秒
//       Revision:  none
//       Compiler:  cl
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "MainStackWidget.h"
#include "MainListWidget.h"
#include "LinkProcessWidget.h"
#include "LinkCmdWidget.h"
#include "ScreenViewWidget.h"
#include "FluxViewWidget.h"
#include "InfoWidget.h"

MainStackWidget::MainStackWidget(MainWindow *mainWindow)
{
	qDebug() << "MainStackWidget 控件初始化\r\n";
	this->mainWindow = mainWindow;
	createMainWeiget();
	setLayout(stackedLayout);
}

MainStackWidget::~MainStackWidget()
{

}

/*================================================================
*  函 数 名： MainStackWidget::createMainWeiget
*
*  功能描述： 创建主显示控件
*
*  参    数： 无
*			  
*  返 回 值： 无
*
*  作    者：刘欢 2013/8
================================================================*/
void MainStackWidget::createMainWeiget()
{
	stackedLayout = new QStackedLayout(this);
	stackedLayout->setStackingMode(QStackedLayout::StackOne);

	mainListWidget = new MainListWidget();
	screenViewWidget = new ScreenViewWidget(mainWindow);
	fluxViewWidget = new FluxViewWidget(mainWindow);
	linkProcessWidget = new LinkProcessWidget(mainWindow);
	linkCmdWidget = new LinkCmdWidget(mainWindow);
	infoWidget = new InfoWidget(mainWindow);

	stackedLayout->addWidget(mainListWidget);
	stackedLayout->addWidget(screenViewWidget);
	stackedLayout->addWidget(fluxViewWidget);
	stackedLayout->addWidget(linkProcessWidget);
	stackedLayout->addWidget(linkCmdWidget);
	stackedLayout->addWidget(infoWidget);
}

/*================================================================
*  函 数 名： MainStackWidget::getStackLayout
*
*  功能描述： 获得当前的stackedLayout
*
*  参    数： 无
*			  
*  返 回 值：stackedLayout
*
*  作    者：刘欢 2013/8
================================================================*/
QStackedLayout *MainStackWidget::getStackLayout()
{
	return stackedLayout;
}