// =====================================================================================
// 
//       Filename:  LinkCmdWidget.h
//
//    Description:  cmd 管理标签 Widget
//
//        Version:  1.0
//        Created:  2013年08月21日 09时33分05秒
//       Revision:  none
//       Compiler:  cl
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#ifndef LINKCMDWIDGET_H_
#define LINKCMDWIDGET_H_

#include <QGroupBox>
#include <QStack>

class QTextBrowser;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class MainWindow;

class LinkCmdWidget : public QGroupBox
{
	Q_OBJECT

public:
	LinkCmdWidget(MainWindow *mainWindow);
	~LinkCmdWidget();

	void createMainWeiget();			// 创建主显示

protected:
	bool eventFilter(QObject *target, QEvent *event);

public slots:
	void resetCmdInfo();
	void cmdViewAddInfo(QString strInfo);
	void sendCMDCommand();

private:
	QTextBrowser *cmdText;				// cmd 输出显示窗口
	QLabel *cmdLabel;					// 命令输入标签
	QLineEdit *cmdInputEdit;			// cmd 命令输入框
	QPushButton *sendButton;			// 发送 cmd 命令按钮

	QVBoxLayout *cmdLayout;				// cmd 窗口控制
	QHBoxLayout *cmdButtonLayout;		// cmd 底部窗口控制
	MainWindow *mainWindow;
	QStack<QString> cmdUpStack;
	QStack<QString> cmdDownStack;
};

#endif	// LINKCMDWIDGET_H_
