#ifndef WIDGET_H
#define WIDGET_H

#define NUM_PATTERNS        11
#define NUM_VECTORS         12
#define ACTIVATION_RESPONSE 1.0
#define LEARNING_RATE       0.5
#define ERROR_THRESHOLD     0.003
#define NUM_HIDDEN_NEURONS  6

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QInputDialog>
#include "Neuralnet.h"

class Widget : public QWidget{
public:
    Widget();
    ~Widget();

    QPushButton* TrainButton;
    QPushButton* LearnButton;

    NeuralNet* Brain;
    Data* MyData;

    std::vector<QPointF> vecPath;
    std::vector<QPointF> vecSmoothPath;
    std::vector<double> vecVectors;

    bool IsDrawing;
    bool IsLearning;

    void Clear();
    void CreateVectors();
    bool Smooth();

    int TestForMatch();
    void Train();
    void Learn();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);
};

Widget::Widget(){
    MyData=new Data(NUM_PATTERNS,NUM_VECTORS);
    Brain=new NeuralNet(NUM_VECTORS*2,NUM_PATTERNS,NUM_HIDDEN_NEURONS,LEARNING_RATE);
    TrainButton=new QPushButton("开始训练",this);
    LearnButton=new QPushButton("进入学习模式",this);
    this->setFixedSize(400,400);
    TrainButton->setGeometry(0,0,100,30);
    LearnButton->setGeometry(this->width()-100,0,100,30);
    connect(TrainButton,&QPushButton::clicked,this,&Widget::Train);
    connect(LearnButton,&QPushButton::clicked,this,&Widget::Learn);
    IsLearning=0;
    IsDrawing=0;
}

Widget::~Widget(){

}

void Widget::Clear(){
    vecPath.clear();
    vecSmoothPath.clear();
    vecVectors.clear();
}

void Widget::CreateVectors(){
    for(int p=1;p<vecSmoothPath.size();p++){
    double x=vecSmoothPath[p].x()-vecSmoothPath[p-1].x();
    double y=vecSmoothPath[p].y()-vecSmoothPath[p-1].y();
    QVector2D v(x,y);
    v.normalize();
    vecVectors.push_back(v.x());
    vecVectors.push_back(v.y());
    }
}

bool Widget::Smooth(){
    if(vecPath.size()<NUM_VECTORS+1)
       return false;
    vecSmoothPath=vecPath;
    while(vecSmoothPath.size()>NUM_VECTORS+1){
        double shortest=999999;
        int PointMarker=0;
        for(int i=2;i<vecSmoothPath.size()-1;i++){
            double length=sqrt((vecSmoothPath[i-1].x()-vecSmoothPath[i].x())*(vecSmoothPath[i-1].x()-vecSmoothPath[i].x())+(vecSmoothPath[i-1].y()-vecSmoothPath[i].y())*(vecSmoothPath[i-1].y()-vecSmoothPath[i].y()));
            if(length<shortest){
                shortest=length;
                PointMarker=i;
            }
        }
        QPoint newPoint;
        newPoint.setX((vecSmoothPath[PointMarker-1].x()+vecSmoothPath[PointMarker].x())/2);
        newPoint.setY((vecSmoothPath[PointMarker-1].y()+vecSmoothPath[PointMarker].y())/2);
        vecSmoothPath[PointMarker-1]=newPoint;
        vecSmoothPath.erase(vecSmoothPath.begin()+PointMarker);
    }
    return true;
}

int Widget::TestForMatch(){
    std::vector<double> outputs=Brain->Update(vecVectors);
    double highest=0;
    int bestmatch=0;
    for(int i=0;i<outputs.size();i++)
        if(outputs[i]>highest){
        highest=outputs[i];
        bestmatch=i;
    }
    return bestmatch;
}

void Widget::Train(){
    TrainButton->setText("训练中");
    this->setEnabled(false);
    QMessageBox::information(this,"提示","点击确定后开始训练！请耐心等待");
    Brain->Train(MyData);
    this->setEnabled(true);
    TrainButton->setText("开始训练");
    QMessageBox::information(this,"提示","训练完成！");
}

void Widget::Learn(){
    if(IsLearning)
        LearnButton->setText("进入训练模式");
    else
        LearnButton->setText("进入测试模式");
    IsLearning=!IsLearning;
}

void Widget::mousePressEvent(QMouseEvent *e){
        IsDrawing=true;
}

void Widget::mouseReleaseEvent(QMouseEvent *e){
    if(!IsLearning){
        IsDrawing=false;
        if(Smooth()){
            CreateVectors();
            int i=TestForMatch();
            QMessageBox::information(this,"提示","你做的手势是  "+MyData->Name[i]);
        }
        else
            QMessageBox::information(this,"提示","手势太短！");
        Clear();
    }
    else{
        if(Smooth()){
            CreateVectors();
            QString newName=QInputDialog::getText(this,"提示","请输入新手势的名字：");
            if(newName!=NULL){
            MyData->AddData(vecVectors,newName);
            Brain=new NeuralNet(NUM_VECTORS*2,MyData->NumPatterns,NUM_HIDDEN_NEURONS,LEARNING_RATE);
            QMessageBox::information(this,"提示","记得重新训练！");

            }
        }
        else{
            QMessageBox::information(this,"提示","手势太短！");
        }
        Clear();
    }
}

void Widget::mouseMoveEvent(QMouseEvent* e){
    if(IsDrawing){
        vecPath.push_back(e->pos());
        update();
    }
}

void Widget::paintEvent(QPaintEvent *e){
    if(IsDrawing){
        QPainter paint;
        paint.begin(this);
        QPen a;
        a.setWidth(5);
        paint.setPen(a);
        for(int i=0;i<vecPath.size();i++)
            paint.drawPoint(vecPath[i]);
        paint.end();
    }
}

#endif // WIDGET_H
