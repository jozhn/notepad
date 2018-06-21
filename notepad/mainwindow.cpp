#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QClipboard>
#include <QFontDialog>
#include <QList>
#include <QString>
#include <QUrl>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->setAcceptDrops(false);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())//数据是否包含URls
    {
        e->acceptProposedAction();//接收动作
        //若不添加此函数，则外部文件无法添加到窗体中
    }
    else
        e->ignore();//忽略事件
}

void MainWindow::dropEvent(QDropEvent *e)
{
    const QMimeData *mimeData = e->mimeData();//获取mimie数据
    if(mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();//获取url列表
        QString fileName = urlList.at(0).toLocalFile();//将第一个URL作为本地文件路径
        if(!fileName.isEmpty()){
            readFile(fileName);
            return;
        }
        else
            return;
    }
}

bool MainWindow::readFile(const QString &fileName) //读文件
{
    QString displayString;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QString str = getUnicode(line);
        displayString.append(str);
    }
    flag = 1;//标记有文件
    filePath = fileName;//存储文件路径到全局变量

    QFileInfo inf;
    inf = QFileInfo(filePath);
    QString title = inf.fileName();
    ui->plainTextEdit->clear();
    this->setWindowTitle(title+QString::fromUtf8("- 记事本"));

    ui->plainTextEdit->setPlainText(displayString);
    file.close();
    return true;
}

QString MainWindow::getUnicode(const QByteArray &ba)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode( ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
        text = QTextCodec::codecForName( "GBK" )->toUnicode(ba);
    else
        text = QTextCodec::codecForName( "UTF-8" )->toUnicode(ba);
    return text;
}

void MainWindow::on_action_create_triggered()
{
    flag=0;
    ui->plainTextEdit->clear();
    setWindowTitle(QString::fromUtf8("无标题 - 记事本"));
}

void MainWindow::on_action_open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open text file"),
                "./", tr("Text files(*.txt);;All files (*.*)"));
    readFile(fileName);
}

void MainWindow::on_action_save_triggered()
{
    if( flag==1)//如果标记为1，证明有文件加载，不然没有不需要保存
    {
        QFile file(filePath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) //以文本文式写入
        {
            QTextStream out(&file);
            out <<ui->plainTextEdit->toPlainText()<< endl;
            file.close();
        }
        //QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("保存成功！"));
    }
    else
    {
        //QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("文件不存在！"));
        on_action_another_triggered();
    }
}

void MainWindow::on_action_another_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,tr("Open File"),
        "./",tr("Text File(*.txt)"));
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) //以文本文式写入
    {
        QTextStream out(&file);
        out <<ui->plainTextEdit->toPlainText()<< endl;
        file.close();
    }
    filePath = fileName;
    flag = 1;
    setWindowTitle(fileName);
    //QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("保存成功！"));
}

void MainWindow::on_action_paste_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    QString clipText = clipboard->text();         //获取剪贴板上文本信息
    ui->plainTextEdit->insertPlainText(clipText);
}

void MainWindow::on_action_copy_triggered()
{
    QString copyText = ui->plainTextEdit->textCursor().selectedText();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(copyText);
}

void MainWindow::on_action_del_triggered()
{
    ui->plainTextEdit->textCursor().deleteChar();
}

void MainWindow::on_action_undo_triggered()
{
    ui->plainTextEdit->undo();
}

void MainWindow::on_action_shear_triggered()
{
    QString copyText = ui->plainTextEdit->textCursor().selectedText();
    ui->plainTextEdit->textCursor().deleteChar();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(copyText);
}

void MainWindow::on_action_about_triggered()
{
    Dialog *dlg = new Dialog(this);
    dlg->setModal(true);
    dlg->setWindowTitle(QString::fromUtf8("关于“记事本”"));
    dlg->show();
}

void MainWindow::on_action_font_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok)
        ui->plainTextEdit->setFont(font);
    else{
        //qDebug() <<QString::fromUtf8("没有选择字体！");
    }
}
