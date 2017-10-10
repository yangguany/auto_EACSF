#include "csfwindow.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#define JSON_FILE_PATH "../Desktop/test.json"
#define BUFFER_SIZE 5000
#define MAX_TOKEN_COUNT 128
#include <QDebug>

using namespace std;

CSFWindow::CSFWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setupUi(this);
    this->initializeMenuBar();
}

CSFWindow::~CSFWindow()
{

}


// File
void CSFWindow::initializeMenuBar(){
    //Load and Save
    connect( action_LoadData, SIGNAL( triggered() ), SLOT( OnLoadDataConfiguration() ) );
    connect( action_SaveData, SIGNAL( triggered() ), SLOT( OnSaveDataConfiguration() ) );

    connect( action_LoadParameter, SIGNAL( triggered() ), SLOT( OnLoadParameterConfiguration() ) );
    connect( action_SaveParameter, SIGNAL( triggered() ), SLOT( OnSaveParameterConfiguration() ) );

    connect( action_LoadSoftware, SIGNAL( triggered() ), SLOT( OnLoadSoftwareConfiguration() ) );
    connect( action_SaveSoftware, SIGNAL( triggered() ), SLOT( OnSaveSoftwareConfiguration() ) );
}

QString CSFWindow::OpenFile(){
    QString filename = QFileDialog::getOpenFileName(
        this,
                tr("Open File"),
                "C://",
                "All files (*.*);;Text File (*.txt);; Json File (*.json)"
    );
    return filename;
}

void CSFWindow::OnLoadDataConfiguration(){
    QString settings;
    QFile file;
    QJsonObject dataFile;
    QString filename= OpenFile();

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument data = QJsonDocument::fromJson(settings.toUtf8());
    dataFile = data.object();

    lineEdit_T1img->setText(dataFile.value(QString("T1img")).toString());
    lineEdit_T2img->setText(dataFile.value(QString("T2img")).toString());
    lineEdit_BrainMask->setText(dataFile.value(QString("BrainMask")).toString());
    lineEdit_VentriclMask->setText(dataFile.value(QString("VentricleMask")).toString());
    lineEdit_TissueSeg->setText(dataFile.value(QString("TissueSeg")).toString());
    lineEdit_OutputDir->setText(dataFile.value(QString("output_dir")).toString());
    lineEdit_OutputFolderName->setText(dataFile.value(QString("output_folderName")).toString());
}

void CSFWindow::OnLoadParameterConfiguration(){
    QString settings;
    QFile file;
    QJsonObject paramFile;
    QString filename= OpenFile();

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument data = QJsonDocument::fromJson(settings.toUtf8());
    paramFile = data.object();

    //PARAM
    spinBox_Index->setValue(paramFile.value(QString("ACPC_index")).toInt());
    doubleSpinBox_mm->setValue(paramFile.value(QString("ACPC_mm")).toDouble());
    lineEdit_ReferenceAtlasDir->setText(paramFile.value(QString("Reference_Atlas_dir")).toString());
    spinBox_CSFLabel->setValue(paramFile.value(QString("TissueSeg_csf")).toInt());
    lineEdit_TissueSegAtlas->setText(paramFile.value(QString("TissueSeg_Atlas_dir")).toString());

    lineEdit_ROIAtalsT1->setText(paramFile.value(QString("ROI_Atlas_T1")).toString());
    lineEdit_ROIAtalsLabel->setText(paramFile.value(QString("ROI_Atlas_label")).toString());
    spinBox_LeftVentricleLabel->setValue(paramFile.value(QString("left_vLabel")).toInt());
    spinBox_RightVentricleLabel->setValue(paramFile.value(QString("right_vLabel")).toInt());

    comboBox_RegType->setCurrentText(paramFile.value(QString("ANTS_reg_type")).toString());
    doubleSpinBox_TransformationStep->setValue(paramFile.value(QString("ANTS_transformation_step")).toDouble());
    lineEdit_Iterations->setText(paramFile.value(QString("ANTS_iterations_val")).toString());
    comboBox_Metric->setCurrentText(paramFile.value(QString("ANTS_sim_metric")).toString());
    spinBox_SimilarityParameter->setValue(paramFile.value(QString("ANTS_sim_param")).toInt());
    doubleSpinBox_GaussianSigma->setValue(paramFile.value("ANTS_gaussian_sig").toDouble());
    spinBox_T1Weight->setValue(paramFile.value(QString("ANTS_T1_weight")).toInt());
}

void CSFWindow::OnLoadSoftwareConfiguration(){
    QString settings;
    QFile file;
    QJsonObject swFile;
    QString filename= OpenFile();

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument data = QJsonDocument::fromJson(settings.toUtf8());
    swFile = data.object();

    //PARAM
    lineEdit_ABC->setText(swFile.value(QString("ABC")).toString());
    lineEdit_ANTS->setText(swFile.value(QString("ANTS")).toString());
    lineEdit_BRAINSFit->setText(swFile.value(QString("BRAINSFit")).toString());
    lineEdit_FSLBET->setText(swFile.value(QString("FSLBET")).toString());
    lineEdit_ImageMath->setText(swFile.value(QString("ImageMath")).toString());
    lineEdit_ITK->setText(swFile.value(QString("ITK")).toString());
    lineEdit_N4->setText(swFile.value(QString("N4")).toString());
    lineEdit_Python->setText(swFile.value(QString("Python")).toString());
}

bool CSFWindow::OnSaveDataConfiguration(){
    QFile saveFile(QStringLiteral("data_configuration.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject jsonObject;
    writeDataConfiguration_d(jsonObject);
    QJsonDocument saveDoc(jsonObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

bool CSFWindow::OnSaveParameterConfiguration(){
    QFile saveFile(QStringLiteral("parameter_configuration.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject jsonObject;
    writeDataConfiguration_p(jsonObject);
    QJsonDocument saveDoc(jsonObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

bool CSFWindow::OnSaveSoftwareConfiguration(){
    QFile saveFile(QStringLiteral("software_configuration.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject jsonObject;
    writeDataConfiguration_sw(jsonObject);
    QJsonDocument saveDoc(jsonObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void CSFWindow::writeDataConfiguration_d(QJsonObject &json)
{
    json["T1img"] = lineEdit_T1img->text();
    json["T2img"] = lineEdit_T2img->text();
    json["BrainMask"] = lineEdit_BrainMask->text();
    json["VentricleMask"] = lineEdit_VentriclMask->text();
    json["TissueSeg"] = lineEdit_TissueSeg->text();
    json["output_dir"] = lineEdit_OutputDir->text();
    json["output_folderName"] = lineEdit_OutputFolderName->text();

    cout<<"Save Data Configuration"<<endl;
}

void CSFWindow::writeDataConfiguration_p(QJsonObject &json)
{
    json["ACPC_index"] = spinBox_Index->value();
    json["ACPC_mm"] = doubleSpinBox_mm->value();

    json["Reference_Atlas_dir"] = lineEdit_ReferenceAtlasDir->text();
    json["TissueSeg_csf"] = spinBox_CSFLabel->value();
    json["TissueSeg_Atlas_dir"] = lineEdit_TissueSegAtlas->text();
    json["left_vLabel"] = spinBox_LeftVentricleLabel->value();
    json["right_vLabel"] = spinBox_RightVentricleLabel->value();

    json["ANTS_reg_type"] = comboBox_RegType->currentText();
    json["ANTS_transformation_step"] = doubleSpinBox_TransformationStep->value();
    json["ANTS_iterations_val"] = lineEdit_Iterations->text();
    json["ANTS_sim_metric"] = comboBox_Metric->currentText();
    json["ANTS_sim_param"] = spinBox_SimilarityParameter->value();
    json["ANTS_gaussian_sig"] = doubleSpinBox_GaussianSigma->value();
    json["ANTS_T1_weight"] = spinBox_T1Weight->value();

    cout<<"Save Parameter Configuration"<<endl;
}

void CSFWindow::writeDataConfiguration_sw(QJsonObject &json)
{
    json["ABC"] = lineEdit_ABC->text();
    json["ANTS"] = lineEdit_ANTS->text();
    json["BRAINSFit"] = lineEdit_BRAINSFit->text();
    json["FSLBET"] = lineEdit_FSLBET->text();
    json["ImageMath"] = lineEdit_ImageMath->text();
    json["ITK"] = lineEdit_ITK->text();
    json["N4"] = lineEdit_N4->text();
    json["Python"] = lineEdit_Python->text();
    cout<<"Save Software Configuration"<<endl;
}

// 1st Tab - Inputs
void CSFWindow::on_pushButton_T1img_clicked()
{
    lineEdit_T1img->setText(OpenFile());
}

void CSFWindow::on_pushButton_T2img_clicked()
{
    lineEdit_T2img->setText(OpenFile());
}

void CSFWindow::on_pushButton_BrainMask_clicked()
{
     lineEdit_BrainMask->setText(OpenFile());
}

void CSFWindow::on_pushButton_VentricleMask_clicked()
{
     lineEdit_VentriclMask->setText(OpenFile());
}

void CSFWindow::on_pushButton_TissueSeg_clicked()
{
    lineEdit_TissueSeg->setText(OpenFile());
}

void CSFWindow::on_pushButton_OutputDir_clicked()
{
     lineEdit_OutputDir->setText(OpenFile());
}

void CSFWindow::on_spinBox_Index_valueChanged(int arg1)
{
    indexVal=arg1;
}

void CSFWindow::on_doubleSpinBox_mm_valueChanged(double arg1)
{
    mm=arg1;
}

void CSFWindow::on_radioButton_Index_clicked(bool checked)
{
    if(!checked) indexVal=0;
}

void CSFWindow::on_radioButton_mm_clicked(bool checked)
{
    if(!checked) mm=0;
}

// 2nd Tab - Executables
void CSFWindow::on_pushButton_ABC_clicked()
{
     lineEdit_ABC->setText(OpenFile());
}

void CSFWindow::on_pushButton_ANTS_clicked()
{
     lineEdit_ANTS->setText(OpenFile());
}

void CSFWindow::on_pushButton_BRAINSFit_clicked()
{
     lineEdit_BRAINSFit->setText(OpenFile());
}

void CSFWindow::on_pushButton_FSLBET_clicked()
{
     lineEdit_FSLBET->setText(OpenFile());
}

void CSFWindow::on_pushButton_ImageMath_clicked()
{
     lineEdit_ImageMath->setText(OpenFile());
}

void CSFWindow::on_pushButton_ITK_clicked()
{
     lineEdit_ITK->setText(OpenFile());
}

void CSFWindow::on_pushButton_N4_clicked()
{
     lineEdit_N4->setText(OpenFile());
}

void CSFWindow::on_pushButton_Python_clicked()
{
     lineEdit_Python->setText(OpenFile());
}


// 3rd Tab - 1.Reference Alginment, 2.Skull Stripping
void CSFWindow::on_pushButton_ReferenceAtlasDir_clicked()
{
     lineEdit_ReferenceAtlasDir->setText(OpenFile());
}

void CSFWindow::on_radioButton_rigidRegistration_clicked(bool checked)
{
        preAlign=!preAlign;
        notPreAlign=!notPreAlign;
}

void CSFWindow::on_radioButton_preAligned_clicked(bool checked)
{
    preAlign=!preAlign;
    notPreAlign=!notPreAlign;
}

// 4th Tab - 3.Tissue Seg
void CSFWindow::on_pushButton_TissueSegAtlas_clicked()
{
     lineEdit_TissueSegAtlas->setText(OpenFile());
}


// 5th Tab - 4.Ventricle Masking

// 6th Tab - 5.ANTS Registration

// 7th Tab - 6.Execution

void CSFWindow::on_checkBox_SkullStripping_clicked(bool checked)
{
   // skullStr=!skullStr;
}

void CSFWindow::on_checkBox_SkullStripping_stateChanged(int arg1)
{

}

