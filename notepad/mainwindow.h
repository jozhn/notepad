#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int flag;//是否有打开文件的标记
    QString filePath;

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);


private slots:
    void on_action_create_triggered();

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_action_another_triggered();

    void on_action_paste_triggered();

    void on_action_copy_triggered();

    void on_action_del_triggered();

    void on_action_undo_triggered();

    void on_action_shear_triggered();

    void on_action_about_triggered();

    void on_action_font_triggered();

private:
    Ui::MainWindow *ui;
    bool readFile(const QString &fileName);
    QString getUnicode(const QByteArray &ba);
};

#endif // MAINWINDOW_H
