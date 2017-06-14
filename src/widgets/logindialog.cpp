#include "logindialog.h"
#include "ui_logindialog.h"
#include "../myHelper.h"
#include "../iconhelper.h"
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->InitStyle();
  	ui->listWidget_Theme->hide();
	isLeftPressDown = false; 
    isListWedgitThemeShow = false;
	this->dir = NONE;
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	ui->widget_title->setMouseTracking(true);
	ui->widget_2->setMouseTracking(true);
	ui->widget_Theme->setMouseTracking(true);
	ui->widget->setMouseTracking(true);
	ui->lab_Title->setMouseTracking(true);
	ui->lab_Ico->setMouseTracking(true);
	this->setMouseTracking(true);
    connect(ui->listWidget_Theme, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(themeClicked(QListWidgetItem *)));
}


LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
}

void LoginDialog::themeClicked(QListWidgetItem * item)
{
    myHelper::SetStyle(item->text());//天蓝色风格
}

void LoginDialog::on_btnMenu_clicked()
{
    if (isListWedgitThemeShow) {
        isListWedgitThemeShow = false;
        ui->listWidget_Theme->hide();
    } else {
        isListWedgitThemeShow = true;
        ui->listWidget_Theme->show();
    }
}


bool LoginDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}


void LoginDialog::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void LoginDialog::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void LoginDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}


void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		isLeftPressDown = false;
		this->releaseMouse();
		this->setCursor(QCursor(Qt::ArrowCursor));
		dir = NONE;
	}
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		isLeftPressDown = true;
		if (dir != NONE) {
			this->mouseGrabber();
		}
		else {
			dragPosition = event->globalPos() - this->frameGeometry().topLeft();
		}
		break;
	case Qt::RightButton:
		break;
	default:
		QDialog::mousePressEvent(event);
	}
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
	QPoint gloPoint = event->globalPos();
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());
	
	if (!isLeftPressDown) {
		this->region(gloPoint);	
	}
	else {	
		if (dir != NONE) {
			QRect rMove(tl, rb);
			switch (dir) {
			case LEFT:
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				break;
			case RIGHT:
				rMove.setWidth(gloPoint.x() - tl.x());
				break;
			case UP:
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case DOWN:
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case LEFTTOP:
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case RIGHTTOP:
				rMove.setWidth(gloPoint.x() - tl.x());
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case LEFTBOTTOM:
				rMove.setHeight(gloPoint.y() - tl.y());
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				break;
			case RIGHTBOTTOM:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			default:

				break;
			}
		
			this->setGeometry(rMove);
		}
		else {
			move(event->globalPos() - dragPosition);
			event->accept();
		}
	}

	QDialog::mouseMoveEvent(event);
}

void LoginDialog::region(const QPoint &cursorGlobalPoint)
{
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());
	int x = cursorGlobalPoint.x();
	int y = cursorGlobalPoint.y();
	qDebug() << "t1=" << tl << " rb=" << rb << " x=" << x << " y=" << y;
	if (tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
		// 左上角
		dir = LEFTTOP;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
		// 右下角
		dir = RIGHTBOTTOM;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
		//左下角
		dir = LEFTBOTTOM;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
		// 右上角
		dir = RIGHTTOP;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x <= tl.x() + PADDING && x >= tl.x()) {
		// 左边
		dir = LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (x <= rb.x() && x >= rb.x() - PADDING) {
		// 右边
		dir = RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (y >= tl.y() && y <= tl.y() + PADDING){
		// 上边
		dir = UP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (y <= rb.y() && y >= rb.y() - PADDING) {
		// 下边
		dir = DOWN;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else {
		// 默认
		dir = NONE;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}



