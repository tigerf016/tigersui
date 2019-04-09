#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vtkAlgorithm.h>
#include <vtkSmartPointer.h>
#include <QMainWindow>
#include <QPushButton>
#include <QColor>
#include <QString>
#include <vtkDataSetMapper.h>
#include <string>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update();
    void changeColour();
    void intensityChanged(int value);
    void clipFilterChanged(int arg1);
    void on_actionOpen_triggered();
    void shrinkFilterChanged(int arg1);
    void resetView();
    void changeBackground();
    void changeEdges(int arg1);


   /* void on_pushButton_6_clicked();

    void on_pushButton_14_clicked();
    
    void on_pushButton_7_clicked();
    
    void on_edgesButton_clicked();
    */

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkAlgorithm> modelData;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkRenderer> renderer;
    QColor backColour = QColor(192, 192, 192);
    QColor modelColour = QColor(255, 0, 0);;
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    QString c_str;

      int clipTrue =0;
      int shrinkTrue =0;
      int edgesTrue = 0;
};

#endif // MAINWINDOW_H
