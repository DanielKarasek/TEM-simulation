#include "scene.h"
#include "myqslider.h"
Scene::Scene()
{
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x222222)));
    container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);
    view->defaultFrameGraph()->setFrustumCullingEnabled(false);


    widget = new QWidget;
    hLayout = new QHBoxLayout(widget);
    vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("MicroscopeBitchboi"));

    // Root entity
    m_rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 120.0f, 350.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 50.0f, 0));


    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(m_rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(2);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(0, 100.0f, 50.0f));
    lightEntity->addComponent(lightTransform);

    Qt3DCore::QEntity *lightEntity2 = new Qt3DCore::QEntity(m_rootEntity);
    Qt3DRender::QPointLight *light2 = new Qt3DRender::QPointLight(lightEntity);
    light2->setColor("white");
    light2->setIntensity(2);
    lightEntity2->addComponent(light);
    Qt3DCore::QTransform *lightTransform2 = new Qt3DCore::QTransform(lightEntity);
    lightTransform2->setTranslation(QVector3D(0, 100.0f, -50.0f));
    lightEntity2->addComponent(lightTransform2);

    // For camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(m_rootEntity);
    camController->setCamera(cameraEntity);
    camController->setLinearSpeed(500);
    camController->setAcceleration(10);

    // Set root object of the scene
    view->setRootEntity(m_rootEntity);

//    slider = new MyQSlider(Qt::Horizontal);
//    slider->setMinimum(0);
//    slider->setMaximum(100);

    //menu button
    button_menu = new QPushButton("Menu", widget);
    vLayout->addWidget(button_menu);

    //menu buttons
    button_add_lens = new QPushButton("Add lens", widget);
    vLayout->addWidget(button_add_lens);

    button_load = new QPushButton("Load && Save", widget);
    vLayout->addWidget(button_load);

    button_lens_update = new QPushButton("Lens update", widget);
    vLayout->addWidget(button_lens_update);

    button_detector_update = new QPushButton("Detector update", widget);
    vLayout->addWidget(button_detector_update);

    button_specimen_update = new QPushButton("Update specimen", widget);
    vLayout->addWidget(button_specimen_update);


    //add lens
    label_add_focal_power = new QLabel("Focal length: ");
    vLayout->addWidget(label_add_focal_power);
    slider_add_focal_power = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_add_focal_power);
    connect(slider_add_focal_power, &MyQSlider::valueChanged, this, [=] () {
        label_add_focal_power->setText("Focal length: " + QString::number(slider_add_focal_power->valueDouble()));
    });
    slider_add_focal_power->setDoubleMinimum(-20);
    slider_add_focal_power->setDoubleMaximum(20);

    label_add_x_deviation = new QLabel("X deviation: ");
    vLayout->addWidget(label_add_x_deviation);
    slider_add_x_deviation = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_add_x_deviation);
    connect(slider_add_x_deviation, &MyQSlider::valueChanged, this, [=] () {
        label_add_x_deviation->setText("X deviation: " + QString::number(slider_add_x_deviation->valueDouble()));
    });
    slider_add_x_deviation->setDoubleMinimum(-1);
    slider_add_x_deviation->setDoubleMaximum(1);

    label_add_y_deviation = new QLabel("Y deviation: ");
    vLayout->addWidget(label_add_y_deviation);
    slider_add_y_deviation = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_add_y_deviation);
    connect(slider_add_y_deviation, &MyQSlider::valueChanged, this, [=] () {
        label_add_y_deviation->setText("Y deviation: " + QString::number(slider_add_y_deviation->valueDouble()));
    });
    slider_add_y_deviation->setDoubleMinimum(-1);
    slider_add_y_deviation->setDoubleMaximum(1);

    label_add_z_deviation = new QLabel("Z position: ");
    vLayout->addWidget(label_add_z_deviation);
    slider_add_z_position = new MyQSlider(Qt::Horizontal);
    slider_add_z_position->setDoubleMinimum(0);
    vLayout->addWidget(slider_add_z_position);
    connect(slider_add_z_position, &MyQSlider::valueChanged, this, [=] () {
        label_add_z_deviation->setText("Z position: " + QString::number(slider_add_z_position->valueDouble()));
    });

    button_add_lens_apply = new QPushButton("Apply", widget);
    vLayout->addWidget(button_add_lens_apply);

    //Load
    file_dialog_rays = new QFileDialog(widget);
    vLayout->addWidget(file_dialog_rays);
    button_load_rays = new QPushButton("Load rays", widget);
    vLayout->addWidget(button_load_rays);

    file_dialog_specimen = new QFileDialog(widget);
    vLayout->addWidget(file_dialog_specimen);
    button_load_specimen = new QPushButton("Load specimen", widget);
    vLayout->addWidget(button_load_specimen);

    file_dialog_microscope = new QFileDialog(widget);
    vLayout->addWidget(file_dialog_microscope);
    button_load_microscope = new QPushButton("Load microscope", widget);
    vLayout->addWidget(button_load_microscope);

    file_dialog_save_microscope = new QFileDialog(widget);
    vLayout->addWidget(file_dialog_save_microscope);
    button_save_microscope = new QPushButton("Save microscope", widget);
    vLayout->addWidget(button_save_microscope);


//    //Lens update
    label_choose_lens = new QLabel("Choose lens");
    vLayout->addWidget(label_choose_lens);
    combox_choose_lens = new QComboBox(widget);
    vLayout->addWidget(combox_choose_lens);


    label_update_focal_power = new QLabel("Focal length: ");
    vLayout->addWidget(label_update_focal_power);
    slider_update_focal_power = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_update_focal_power);
    connect(slider_update_focal_power, &MyQSlider::valueChanged, this, [=] () {
        label_update_focal_power->setText("Focal length: " + QString::number(slider_update_focal_power->valueDouble()));
    });

    slider_update_focal_power->setDoubleMinimum(-20);
    slider_update_focal_power->setDoubleMaximum(20);

    label_update_x_deviation = new QLabel("X deviation: ");
    vLayout->addWidget(label_update_x_deviation);
    slider_update_x_deviation = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_update_x_deviation);
    connect(slider_update_x_deviation, &MyQSlider::valueChanged, this, [=] () {
        label_update_x_deviation->setText("X deviation: " + QString::number(slider_update_x_deviation->valueDouble()));
    });
    slider_update_x_deviation->setDoubleMaximum(1);
    slider_update_x_deviation->setDoubleMinimum(-1);

    label_update_y_deviation = new QLabel("Y deviation: ");
    vLayout->addWidget(label_update_y_deviation);
    slider_update_y_deviation = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_update_y_deviation);
    connect(slider_update_y_deviation, &MyQSlider::valueChanged, this, [=] () {
        label_update_y_deviation->setText("Y deviation: " + QString::number(slider_update_y_deviation->valueDouble()));
    });
    slider_update_y_deviation->setDoubleMaximum(1);
    slider_update_y_deviation->setDoubleMinimum(-1);

    label_update_z_position = new QLabel("Z position: ");
    vLayout->addWidget(label_update_z_position);
    slider_update_z_position = new MyQSlider(Qt::Horizontal);
    slider_update_z_position->setDoubleMinimum(0);
    vLayout->addWidget(slider_update_z_position);
    connect(slider_update_z_position, &MyQSlider::valueChanged, this, [=] () {
        label_update_z_position->setText("Z position: " + QString::number(slider_update_z_position->valueDouble()));
    });


    button_delete_lens = new QPushButton("Delete lens", widget);
    vLayout->addWidget(button_delete_lens);
    button_apply_lens_update = new QPushButton("Apply", widget);
    vLayout->addWidget(button_apply_lens_update);

    //detector update_____________________________________________
    label_detector_abs =  new QLabel("Detector absolut: ");
    vLayout->addWidget(label_detector_abs);
    slider_detector_abs = new MyQSlider(Qt::Horizontal);
    slider_detector_abs->setDoubleMinimum(1);
    slider_detector_abs->setDoubleMaximum(150);
    vLayout->addWidget(slider_detector_abs);
    connect(slider_detector_abs, &MyQSlider::valueChanged, this, [=] () {
        label_detector_abs->setText("Detector absolut: " + QString::number(slider_detector_abs->valueDouble()));
    });

    label_detector_rel =  new QLabel("Detector relative: ");
    vLayout->addWidget(label_detector_rel);
    slider_detector_rel = new QSlider(Qt::Horizontal);
    vLayout->addWidget(slider_detector_rel);
    connect(slider_detector_rel, &QSlider::valueChanged, this, [=] () {
        label_detector_rel->setText("Detector relative: " + QString::number(slider_detector_rel->value()));
    });
    slider_detector_rel->setMinimum(1);
    slider_detector_rel->setMaximum(250);


    label_detector_z =  new QLabel("Detector change on z axes: ");
    vLayout->addWidget(label_detector_z);
    slider_detector_z = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_detector_z);
    connect(slider_detector_z, &MyQSlider::valueChanged, this, [=] () {
        label_detector_z->setText("Detector change on z axes: " + QString::number(slider_detector_z->valueDouble()));
    });

    button_apply_detector_update = new QPushButton("Apply", widget);
    vLayout->addWidget(button_apply_detector_update);
    //specimen update____________________________________________
    label_specimen_rotation = new QLabel("Specimen Rotation: ");
    vLayout->addWidget(label_specimen_rotation);
    slider_specimen_rotation = new MyQSlider(Qt::Horizontal);
    slider_specimen_rotation->setDoubleMaximum(359);
    slider_specimen_rotation->setDoubleMinimum(0);

    vLayout->addWidget(slider_specimen_rotation);
    connect(slider_specimen_rotation, &MyQSlider::valueChanged, this, [=] () {
        label_specimen_rotation->setText("Specimen Rotation: " + QString::number(slider_specimen_rotation->valueDouble()));
    });


    label_specimen_abs_size = new QLabel("Specimen position: ");
    vLayout->addWidget(label_specimen_abs_size);

    slider_specimen_abs_size = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_specimen_abs_size);
    slider_specimen_abs_size->setDoubleMinimum(0.05);
    slider_specimen_abs_size->setDoubleMaximum(10);

    connect(slider_specimen_abs_size, &MyQSlider::valueChanged, this, [=] () {
        label_specimen_abs_size->setText("Specimen absolut size: " + QString::number(slider_specimen_abs_size->valueDouble()));
    });

    label_specimen_z = new QLabel("Specimen z position: ");
    vLayout->addWidget(label_specimen_z);

    slider_specimen_z = new MyQSlider(Qt::Horizontal);
    vLayout->addWidget(slider_specimen_z);
    slider_specimen_z->setDoubleMinimum(0);
    connect(slider_specimen_z, &MyQSlider::valueChanged, this, [=] () {
        label_specimen_z->setText("Specimen z position: " + QString::number(slider_specimen_z->valueDouble()));
    });


    button_apply_specimen_update = new QPushButton("Apply", widget);
    vLayout->addWidget(button_apply_specimen_update);




    connect(button_menu, &QPushButton::released, this, &Scene::goToMenu);
    connect(button_add_lens, &QPushButton::released, this, &Scene::goToAddLens);
    connect(button_load, &QPushButton::released, this, &Scene::goToSaveLoad);
    connect(button_lens_update, &QPushButton::released, this, &Scene::goLensUpdate);

    connect(button_delete_lens, &QPushButton::released, this, &Scene::removeLens);


    connect(button_detector_update, &QPushButton::released, this, &Scene::goToDetectorUpdate);
    connect(button_specimen_update, &QPushButton::released, this, &Scene::goToSpecimenUpdate);

    connect(button_load_rays, &QPushButton::released, this, &Scene::loadRays);
    connect(button_load_specimen, &QPushButton::released, this, &Scene::loadSpecimen);
    connect(button_load_microscope, &QPushButton::released, this, &Scene::loadMicroscope);
    connect(button_save_microscope, &QPushButton::released, this, &Scene::saveMicroscope);



    connect(button_apply_specimen_update, &QPushButton::released, this, &Scene::applySpecimenUpdate);
    connect(button_add_lens_apply, &QPushButton::released, this, &Scene::applyAddLens);
    connect(button_apply_lens_update, &QPushButton::released, this, &Scene::applyLensUpdate);

    connect(button_apply_detector_update, &QPushButton::released, this, &Scene::applyDetectorUpdate);



    connect(combox_choose_lens, &QComboBox::currentTextChanged, this, &Scene::chosenLensChanged);

    // Show window
    widget->show();
    widget->resize(1200, 800);
    goToMenu();

}

void Scene::applyAddLens()
{
    double z = slider_add_z_position->valueDouble();
    double opticalPower = slider_add_focal_power->valueDouble();
    double xDev = slider_add_x_deviation->valueDouble();
    double yDev = slider_add_y_deviation->valueDouble();
    emit signalAddLens(z, opticalPower, xDev, yDev);

    combox_choose_lens->addItem("1");
}

void Scene::applyLensUpdate()
{
    int i = combox_choose_lens->currentIndex();
    double z = slider_update_z_position->valueDouble();
    double focalPower = slider_update_focal_power->valueDouble();
    double xDev = slider_update_x_deviation->valueDouble();
    double yDev = slider_update_y_deviation->valueDouble();
    emit signalUpdateLens(i, z, focalPower, xDev, yDev);
}

void Scene::removeLens()
{
    int i = combox_choose_lens->currentIndex();
    emit signalRemoveLens(i);
}

void Scene::applyDetectorUpdate()
{
    double absSize = slider_detector_abs->valueDouble();
    int relSize = slider_detector_rel->value();
    double zFromSrc = slider_detector_z->valueDouble();
    emit signalUpdateDetector(absSize, relSize, zFromSrc);
    emit signalGetDetectorInfo();

}
void Scene::applySpecimenUpdate()
{
    double rotation = slider_specimen_rotation->valueDouble();
    double z = slider_specimen_z->valueDouble();
    double abSize = slider_specimen_abs_size->valueDouble();
    emit signalUpdateSpecimen(rotation, z, abSize);
    emit signalGetSpecimenInfo();

}
void Scene::loadRays()
{
//    file_dialog_rays->setVisible(true);
    QString filename = file_dialog_rays->getOpenFileName();
    emit signalLoadRays(filename.toUtf8().constData());
}

void Scene::loadSpecimen()
{
    QString filename = file_dialog_specimen->getOpenFileName();
    emit signalLoadSpecimen(filename.toUtf8().constData());
}

void Scene::loadMicroscope()
{
    QString filename = file_dialog_microscope->getOpenFileName();
    emit signalLoadMicroscope(filename.toUtf8().constData());
}
void Scene::saveMicroscope()
{
    QString filename = QFileDialog::getSaveFileName(widget, tr("Save File"), "microscope");
    emit signalSaveMicro(filename.toUtf8().constData());
}

void Scene::goToMenu()
{
    button_menu->setVisible(false);
    button_add_lens->setVisible(true);
    button_load->setVisible(true);
    button_lens_update->setVisible(true);
    button_detector_update->setVisible(true);
    button_specimen_update->setVisible(true);
    //add lens dispose
    label_add_focal_power->setVisible(false);
    slider_add_focal_power->setVisible(false);
    label_add_x_deviation->setVisible(false);
    slider_add_x_deviation->setVisible(false);
    label_add_y_deviation->setVisible(false);
    slider_add_y_deviation->setVisible(false);
    label_add_z_deviation->setVisible(false);
    slider_add_z_position->setVisible(false);
    button_add_lens_apply->setVisible(false);

    //Load dispose
    file_dialog_rays->setVisible(false);
    button_load_rays->setVisible(false);
    button_load_microscope->setVisible(false);
    button_save_microscope->setVisible(false);
 
    button_load_specimen->setVisible(false);
    //Lens update dispose
    combox_choose_lens->setVisible(false);
    label_choose_lens->setVisible(false);

    label_update_focal_power->setVisible(false);
    slider_update_focal_power->setVisible(false);
    label_update_x_deviation->setVisible(false);
    slider_update_x_deviation->setVisible(false);
    label_update_y_deviation->setVisible(false);
    slider_update_y_deviation->setVisible(false);
    label_update_z_position->setVisible(false);
    slider_update_z_position->setVisible(false);
    button_delete_lens->setVisible(false);
    button_apply_lens_update->setVisible(false);

    //detector update dispose
    label_detector_abs->setVisible(false);
    slider_detector_abs->setVisible(false);
    button_apply_detector_update->setVisible(false);
    label_detector_rel->setVisible(false);
    slider_detector_rel->setVisible(false);
    label_detector_z->setVisible(false);
    slider_detector_z->setVisible(false);

    //specimen update dispose
    label_specimen_rotation->setVisible(false);
    slider_specimen_rotation->setVisible(false);
    label_specimen_abs_size->setVisible(false);
    slider_specimen_abs_size->setVisible(false);
    label_specimen_z->setVisible(false);
    slider_specimen_z->setVisible(false);
    button_apply_specimen_update->setVisible(false);

}
void Scene::exitMenu()
{
    button_menu->setVisible(true);

    button_add_lens->setVisible(false);
    button_load->setVisible(false);
    button_lens_update->setVisible(false);
    button_detector_update->setVisible(false);
    button_specimen_update->setVisible(false);
}
void Scene::goToAddLens()
{
    exitMenu();
    label_add_focal_power->setVisible(true);
    slider_add_focal_power->setVisible(true);
    slider_add_focal_power->setValueDouble(0);
    label_add_x_deviation->setVisible(true);
    slider_add_x_deviation->setVisible(true);
    slider_add_x_deviation->setValueDouble(0);
    label_add_y_deviation->setVisible(true);
    slider_add_y_deviation->setVisible(true);
    slider_add_y_deviation->setValueDouble(0);
    label_add_z_deviation->setVisible(true);
    slider_add_z_position->setVisible(true);
    button_add_lens_apply->setVisible(true);
    emit signalAddLensViewChosen();
}
void Scene::goToSaveLoad()
{
     exitMenu();
//     file_dialog_rays->setVisible(true);
     button_load_rays->setVisible(true);
     button_load_specimen->setVisible(true);
     button_load_microscope->setVisible(true);
     button_save_microscope->setVisible(true);
}

void Scene::goLensUpdate()
{
     exitMenu();
     label_choose_lens->setVisible(true);
     combox_choose_lens->setVisible(true);

     label_update_focal_power->setVisible(true);
     slider_update_focal_power->setVisible(true);
     label_update_x_deviation->setVisible(true);
     slider_update_x_deviation->setVisible(true);
     label_update_y_deviation->setVisible(true);
     slider_update_y_deviation->setVisible(true);
     label_update_z_position->setVisible(true);
     slider_update_z_position->setVisible(true);
     button_delete_lens->setVisible(true);
     button_apply_lens_update->setVisible(true);

     emit signalGetLensCount();
     emit signalAddLensViewChosen();
}

void Scene::fillCombox(int i)
{
    combox_choose_lens->clear();
    for(int j = 1; j <= i; j++)
    {
        combox_choose_lens->addItem(QString::number(j));
    }

}
void Scene::goToDetectorUpdate()
{
     exitMenu();
     label_detector_abs->setVisible(true);
     slider_detector_abs->setVisible(true);
     button_apply_detector_update->setVisible(true);

     label_detector_rel->setVisible(true);
     slider_detector_rel->setVisible(true);
     label_detector_z->setVisible(true);
     slider_detector_z->setVisible(true);

    emit signalGetDetectorInfo();
}
 void Scene::presetDetectorUpdate(double zRef, double absSizeRef, int relSizeRef, double minHeightHandle, double maxHeightHandle)
 {
     slider_detector_abs->setValueDouble(absSizeRef);

     slider_detector_rel->setValue(relSizeRef);

     slider_detector_z->setValueDouble(zRef);
     slider_detector_z->setDoubleMinimum(minHeightHandle);
     slider_detector_z->setDoubleMaximum(maxHeightHandle);
 }

void Scene::goToSpecimenUpdate()
{
     exitMenu();
     label_specimen_rotation->setVisible(true);
     slider_specimen_rotation->setVisible(true);

     label_specimen_abs_size->setVisible(true);
     slider_specimen_abs_size->setVisible(true);

     label_specimen_z->setVisible(true);
     slider_specimen_z->setVisible(true);
     button_apply_specimen_update->setVisible(true);
     emit signalGetSpecimenInfo();
}

void Scene::presetSpecimenUpdate(double rotationRef, double zRef, double absSizeRef, double maxHeightHandle)
{
    slider_specimen_rotation->setValueDouble(rotationRef);

    slider_specimen_abs_size->setValueDouble(absSizeRef);

    slider_specimen_z->setDoubleMaximum(maxHeightHandle);
    slider_specimen_z->setValueDouble(zRef);
}

void Scene::chosenLensChanged()
{
    int index = combox_choose_lens->currentIndex();
    emit signalChosenLensChanged(index);
}
void Scene::maxLensZ(double maxHeightHandle)
{
    slider_add_z_position->setDoubleMaximum(maxHeightHandle);
    slider_update_z_position->setDoubleMaximum(maxHeightHandle);
    slider_add_z_position->setValueDouble(maxHeightHandle/2);
}
void Scene::presetLensUpdate(double zRef, double opticalPowerRef, double xDevRef, double yDevRef, double maxHeightHandle)
{
    slider_update_focal_power->setValueDouble(opticalPowerRef);
    slider_update_x_deviation->setValueDouble(xDevRef);
    slider_update_y_deviation->setValueDouble(yDevRef);

    slider_update_z_position->setDoubleMaximum(maxHeightHandle);
    slider_update_z_position->setValueDouble(zRef);
}

void Scene::updateScene()
{
    emit signalUpdateMicroVector(m_rootEntity, m_micro);
    emit signalGetLensCount();
}

Qt3DCore::QEntity *Scene::getRoot(){
    return m_rootEntity;
}
