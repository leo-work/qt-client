#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#define PADDING 0

namespace Ui {
class LoginDialog;
}

enum Direction{
	UP = 0,
	DOWN = 1,
	LEFT,
	RIGHT,
	LEFTTOP,
	LEFTBOTTOM,
	RIGHTBOTTOM,
	RIGHTTOP,
	NONE
};

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();

private:
    Ui::LoginDialog *ui;

    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    void InitStyle();
	void region(const QPoint &cursorPoint);
    bool isLeftPressDown;

    QPoint dragPosition;
    Direction dir;

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
};

#endif // LOGINDIALOG_H
