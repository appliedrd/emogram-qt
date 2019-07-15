#include "gui/displaywidget.h"

DisplayWidget::DisplayWidget(QWidget *parent) : QWidget(parent)
{
   // QStringList cameraOptions;
   // cameraOptions << "0" << "1" << "2" << "3" << "4" << "5" << "6";
   // QComboBox* cameraComboBox = new QComboBox;
   // cameraComboBox->addItems(cameraOptions);

    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    runButton = new QPushButton("run", this);
    horizontalLayout->addWidget(runButton);

    QVBoxLayout *layout = new QVBoxLayout;
    image_viewer_ = new ImageViewer(this);

    layout->addWidget(image_viewer_);

    // add the controls to select output dirctory and snapshot rate
    QHBoxLayout* controlLayout = new QHBoxLayout();

    QVBoxLayout* dirLayout = new QVBoxLayout();
    QPushButton *dirSelector = new QPushButton(output_dir_);
    dirLayout->addWidget(dirSelector);
    controlLayout->addLayout(dirLayout);

    QVBoxLayout* rateLayout = new QVBoxLayout();
    QComboBox* combo = new QComboBox(this);
    combo->addItems(rate_);
    rateLayout->addWidget(combo);
    controlLayout->addLayout(rateLayout);

    layout->addLayout(controlLayout);
    layout->addLayout(horizontalLayout);

    QLabel *status = new QLabel();
    status->setText(status_);


    // create objects for the label and progress bar
    //QLabel  *statusLabel = new QLabel();
    QProgressBar *statusProgressBar = new QProgressBar();
    statusBar = new QStatusBar();

    // set text for the label
    //statusLabel->setText("Status:");
    statusBar->showMessage( tr("Ready"), 5000 );

    layout->addWidget(statusBar);
    setLayout(layout);

    camera_ = new Camera();
    faceDector_ = new FaceDetector();

    //faceDector_->setProcessAll(false);

    faceDetectThread_.start();
    cameraThread_.start();

    camera_->moveToThread(&cameraThread_);
    faceDector_->moveToThread(&faceDetectThread_);

    // TODO: Add in slot to turn off camera_, or something
    image_viewer_->connect(faceDector_,
                           SIGNAL(image_signal(QImage)),
                           SLOT(set_image(QImage)));

    faceDector_->connect(camera_, SIGNAL(matReady(cv::Mat)),
                         SLOT(processFrame(cv::Mat)));

    QObject::connect(runButton, SIGNAL(clicked()),
                     camera_, SLOT(runSlot()));

    //QObject::connect(cameraComboBox, SIGNAL(currentIndexChanged(int)),
    //                 camera_, SLOT(cameraIndexSlot(int)));

    QObject::connect(this, SIGNAL(videoFileNameSignal(QString)),
                     camera_, SLOT(videoFileNameSlot(QString)));

    faceDector_->connect(this, SIGNAL(facecascade_name_signal(QString)),
                         SLOT(facecascade_filename(QString)));

    //controls
    QObject::connect(dirSelector, SIGNAL(clicked()),
                     this,	SLOT(openDirDialog()));

    QObject::connect(this, SIGNAL(videoOutputDirSignal(QString)),
                     camera_, SLOT(videoOutputDirSlot(QString)));

    //QObject::connect(this, SIGNAL(setStatus(statusSignal(QString, int)),
    //                 camera_, SLOT(statusSlot(QString, int)));

    // todo set status
   // QObject::connect(this, SIGNAL(status_signal(QString)),
   //                  camera_, SLOT(videoOutputDirSlot(QString)));

}

DisplayWidget::~DisplayWidget()
{
    faceDector_->~FaceDetector();
    camera_->~Camera();
    faceDetectThread_.quit();
    cameraThread_.quit();
    faceDetectThread_.wait();
    cameraThread_.wait();
}

void DisplayWidget::change_face_cascade_filename(QString filename)
{
    emit facecascade_name_signal(filename);
}

void DisplayWidget::openFileDialog()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Video"));
    emit videoFileNameSignal(filename);
}

void DisplayWidget::openDirDialog()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    emit videoOutputDirSignal(dir);
}

void DisplayWidget::setStatus(QString status, int duration)
{
    statusBar->showMessage( tr("status"), duration );
    emit statusSignal(status, duration);
}

void DisplayWidget::cameraStart() {
     runButton->animateClick();
}
