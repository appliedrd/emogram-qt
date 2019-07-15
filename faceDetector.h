#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <QObject>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QDir>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QResource>
#include <opencv2/opencv.hpp>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */

class FaceDetector : public QObject
{
    Q_OBJECT
    QString facecascade_filename_;
    QString eyecascade_filename_;
    QBasicTimer timer_;
    cv::Mat frame_;
    bool processAll_;
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    time_t t;
    int snapshot_interval;


    void process(cv::Mat frame);
    void loadFiles(cv::String faceCascadeFilename, cv::String eyesCascadeFilename);
    void queue(const cv::Mat & frame);
    void timerEvent(QTimerEvent* ev);
    static void matDeleter(void* mat);

public:
    FaceDetector(QObject *parent=0) : QObject(parent), processAll_(true)
    {
       time(&t);
        snapshot_interval = 5;

        facecascade_filename_ = "resources/haarcascade_frontalface_default.xml";
        eyecascade_filename_ = "resources/haarcascade_eye.xml";
        loadFiles(facecascade_filename_.toStdString().c_str(),
                  eyecascade_filename_.toStdString().c_str());
    }
    void setProcessAll(bool all);
    ~FaceDetector();

signals:
    void image_signal(const QImage&);

public slots:
    void processFrame(const cv::Mat& frame);
    void facecascade_filename(QString filename);
};

#endif // FACEDETECTOR_H
