#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QLabel>
#include <QStatusBar>
#include <QProgressBar>
#include <QFileDialog>
#include <QThread>

#include <iostream>
#include <stdio.h>

#include "gui/imageviewer.h"
#include "camera.h"
#include "faceDetector.h"

class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();
     QStatusBar *statusBar;
     QPushButton *runButton;
     void cameraStart();

signals:
    void videoFileNameSignal(QString videoFileName);
    void facecascade_name_signal(QString filename);
    void videoOutputDirSignal(QString dirName);
    void statusSignal(QString status, int duration);

public slots:
    void openFileDialog();
    void change_face_cascade_filename(QString filename);
    void openDirDialog();
    void setStatus(QString status, int duration);

private:
    FaceDetector* faceDector_;
    Camera* camera_;
    QThread faceDetectThread_;
    QThread cameraThread_;
    ImageViewer* image_viewer_;
    QString output_dir_ ="SET output Directory";
    QString sample_rate_ = "2s";
    QString status_ = "STATUS: set output directory then run...";
    QStringList rate_ = { "2s", "5s", "1m", "5m" };
};

#endif // DISPLAYWIDGET_H
