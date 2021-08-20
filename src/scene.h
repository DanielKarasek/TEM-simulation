#ifndef SCENE_H
#define SCENE_H


#include <QObject>
#include <QVector>
#include <QEntity>
#include <QtWidgets/QHBoxLayout>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>
#include <QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QRenderSettings>
#include <QtWidgets/QSlider>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>
#include <string>
#include <iostream>
#include <QComboBox>


#include "presenter.h"
#include "myqslider.h"

using std::string;
/*!
 * \brief Třída pro View
 * \author Daniel Karásek xkaras38, Filip Osvald, xosval04
 * \details
 * <a href="classScene.html">Scéna</a>
 * Třída reprezentující View. Umožňuje uživateli zadávat změny pro mikroskop v uživatelském rozhraní. Obsahuje sloty a signály komunikující s třídou  Presenter pro aplikování změn
 * mikroskopu. Sloty reagující na signály z widgetu (tlačítek a sliderů). A jednotlivé widgety které se zobrazují podle toho kde se uživatel v menu nachází.
 */
class Scene : public QObject
{
    Q_OBJECT
public:
    QVector<Qt3DCore::QEntity *> m_micro;
    Qt3DCore::QEntity *m_rootEntity;

    explicit Scene();
    void updateMicro(QVector<Qt3DCore::QEntity *> new_micro);
    Qt3DCore::QEntity *getRoot();
    /*!
     * \brief signály propojující tuto třídu s třídou presenter
     */
public: signals:
    void signalAddLens(double z, double opticalPower, double xDev, double yDev);
    //load signals
    void signalLoadRays(string filename);
    void signalLoadSpecimen(string filename);
    void signalLoadMicroscope(string filename);
    void signalSaveMicro(string fileName);

    void signalUpdateLens(int i, double z, double focalPower, double xDev, double yDev);
    void signalRemoveLens(int i);
    void signalUpdateDetector(double absSize, int relSize, double zFromSrc);
    void signalUpdateSpecimen(double rotation, double z, double absSize);

    void signalUpdateMicroVector(Qt3DCore::QEntity *root, QVector<Qt3DCore::QEntity *> &microEntities);
    void signalGetLensCount();
    void signalChosenLensChanged(int index);
    void signalGetDetectorInfo();
    void signalGetSpecimenInfo();
    void signalAddLensViewChosen();

    /*!
     * \brief sloty chytající signály ze třídy Presenter
     */
public slots:
        void updateScene();
        void fillCombox(int i);
        void presetLensUpdate(double zRef, double opticalPowerRef, double xDevRef, double yDevRef, double maxHeightHandle);
        void presetDetectorUpdate(double zRef, double absSizeRef, int relSizeRef, double minHeightHandle, double maxHeightHandle);
        void presetSpecimenUpdate(double rotationRef, double zRef, double absSizeRef, double maxHeightHandle);
        void maxLensZ(double maxHeightHandle);

     /*!
      * \brief widgety  uživatelského rozhraní
      */
private:


    QWidget *container;
    QWidget *widget;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;

    //main menu button
    QPushButton *button_menu;
    //menu butons
    QPushButton *button_add_lens;
    QPushButton *button_load;
    QPushButton *button_lens_update;
    QPushButton *button_detector_update;
    QPushButton *button_specimen_update;
    //add lens
    QLabel *label_add_focal_power;
    MyQSlider *slider_add_focal_power;

    QLabel *label_add_x_deviation;
    MyQSlider *slider_add_x_deviation;

    QLabel *label_add_y_deviation;
    MyQSlider *slider_add_y_deviation;

    QLabel *label_add_z_deviation;
    MyQSlider *slider_add_z_position;
    QPushButton *button_add_lens_apply;

    //Save load
    QFileDialog *file_dialog_rays;
    QPushButton *button_load_rays;
    QFileDialog *file_dialog_specimen;
    QPushButton *button_load_specimen;

    QFileDialog *file_dialog_microscope;
    QPushButton *button_load_microscope;
    QFileDialog *file_dialog_save_microscope;
    QPushButton *button_save_microscope;

    //Lens update
    QLabel *label_choose_lens;
    QComboBox *combox_choose_lens;
    QLabel *label_update_focal_power;
    MyQSlider *slider_update_focal_power;

    QLabel *label_update_x_deviation;
    MyQSlider *slider_update_x_deviation;

    QLabel *label_update_y_deviation;
    MyQSlider *slider_update_y_deviation;

    QLabel *label_update_z_position;
    MyQSlider *slider_update_z_position;

    QPushButton *button_delete_lens;
    QPushButton *button_apply_lens_update;

    //detector update
    QLabel *label_detector_abs;
    MyQSlider *slider_detector_abs;
    QLabel *label_detector_rel;
    QSlider *slider_detector_rel;
    QLabel *label_detector_z;
    MyQSlider *slider_detector_z;
    QPushButton *button_apply_detector_update;

    //specimen update
    QLabel *label_specimen_rotation;
    MyQSlider *slider_specimen_rotation;
    QLabel *label_specimen_abs_size;
    MyQSlider *slider_specimen_abs_size;
    QLabel *label_specimen_z;
    MyQSlider *slider_specimen_z;

    QPushButton *button_apply_specimen_update;

    void deleteMicro();
    void deleteEntity(Qt3DCore::QEntity *);
    void exitMenu();

    /*!
     * \brief sloty  používané widgety této třídy
     */
private slots:
    void goToMenu();
    void goToAddLens();
    void goToSaveLoad();
    void goLensUpdate();
    void goToDetectorUpdate();
    void goToSpecimenUpdate();

    void loadRays();
    void loadSpecimen();
    void loadMicroscope();
    void saveMicroscope();

    void applyAddLens();
    void applyLensUpdate();
    void removeLens();
    void chosenLensChanged();

    void applyDetectorUpdate();
    void applySpecimenUpdate();



};

#endif // SCENE_H
