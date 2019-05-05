#include "createmodel.h"
#include "ui_createmodel.h"
#include <QMessageBox>

CreateModel::CreateModel(QWidget *parent,QString *hivename) :
    QDialog(parent),
    ui(new Ui::CreateModel)
{
    ui->setupUi(this);
    dbtool=new Tool("dbTable");
    ui->label_9->setText(*hivename);
    hivemodel=hivename->toStdString();
    date=to_string(ui->dateEdit->date().year()).substr(2,4);
    if(ui->dateEdit->date().month()<10)
       date=date+ "0"+to_string(ui->dateEdit->date().month());
    else {
        date=date+ to_string(ui->dateEdit->date().month());
    }
    if(ui->dateEdit->date().day()<10)
       date=date+"0"+ to_string(ui->dateEdit->date().day());
    else {
        date= date+to_string(ui->dateEdit->date().day());
    }
    if(ui->timeEdit->time().hour()<10)
       times="0"+ to_string(ui->timeEdit->time().hour());
    else {
        times= to_string(ui->timeEdit->time().hour());
    }
    if(ui->timeEdit->time().minute()<10)
       times=times+"0"+ to_string(ui->timeEdit->time().minute());
    else {
        times= times+to_string(ui->timeEdit->time().minute());
    }
    cout<<"time: "<<times<<endl;
}

CreateModel::~CreateModel()
{
    delete ui;
}

void CreateModel::hiveSize(){
    if(ui->checkBox_3->isChecked()){
        size="small";

    }
    else if(ui->checkBox_4->isChecked()){
        size="medium";
    }
    else if(ui->checkBox_5->isChecked()){
        size="large";
    }
    else {
        size="medium";
    }
    cout<<"size: "<<size<<endl;
}

void CreateModel::on_pushButton_clicked()
{
    if((ui->checkBox_3->isChecked() && ui->checkBox_4->isChecked() && ui->checkBox_5->isChecked())||
            (ui->checkBox_3->isChecked() && ui->checkBox_4->isChecked()) || (ui->checkBox_4->isChecked() && ui->checkBox_5->isChecked())||
            (ui->checkBox_3->isChecked() && ui->checkBox_5->isChecked())){
         QMessageBox::warning(this, tr("ERROR MESSAGE"), tr("Check one box before continuing"));
    }
    else if((ui->lineEdit_3->text().isEmpty())){
         QMessageBox::warning(this, tr("ERROR MESSAGE"), tr("Fill in all Boxes before Continuing"));
    }else if((ui->lineEdit_3->text().toInt()>10 && ui->comboBox->itemText(0).toStdString().compare("Days"))){
         QMessageBox::warning(this, tr("ERROR MESSAGE"), tr("Max Duration is 9 days"));
    }
    else{
        duration=ui->lineEdit_3->text().toStdString()+ui->comboBox->itemText(0).toStdString();
        cout<<"duration: "<<duration<<endl;
        cout<<"date: "<<date<<endl;
        cout<<"time: "<<times<<endl;
        hiveSize();
        genModel = new GeneratedModel(this,&hivemodel,&size,&date,&times,&duration,dbtool);
        genModel->show();
    }
}

void CreateModel::on_lineEdit_3_textChanged(const QString &arg1)
{
    if(arg1.size()>2){
        ui->lineEdit_3->backspace();
    }
    for (int i=0;i<arg1.size();i++) {
        if(arg1.at(i).isNumber()==false){
            ui->lineEdit_3->setText("");
        }
    }
    duration=arg1.toStdString()+ui->comboBox->itemText(0).toStdString();
    cout<<"duration: "<<duration<<endl;
}

void CreateModel::on_dateEdit_dateChanged(const QDate &dates)
{
    date=to_string(dates.year()).substr(2,4);
    if(dates.month()<10)
       date=date+ "0"+to_string(dates.month());
    else {
        date=date+ to_string(dates.month());
    }
    if(dates.day()<10)
       date=date+"0"+ to_string(dates.day());
    else {
        date= date+to_string(dates.day());
    }
    cout<<"date: "<<date<<endl;

}

void CreateModel::on_dateEdit_userDateChanged(const QDate &dates)
{
    date=to_string(dates.year()).substr(2,4);
    if(dates.month()<10)
       date=date+ "0"+to_string(dates.month());
    else {
        date=date+ to_string(dates.month());
    }
    if(dates.day()<10)
       date=date+"0"+ to_string(dates.day());
    else {
        date= date+to_string(dates.day());
    }
    cout<<"date: "<<date<<endl;
}

void CreateModel::on_timeEdit_userTimeChanged(const QTime &time)
{
    if(time.hour()<10)
       times="0"+ to_string(time.hour());
    else {
        times= to_string(time.hour());
    }
    if(time.minute()<10)
       times=times+"0"+ to_string(time.minute());
    else {
        times= times+to_string(time.minute());
    }
    cout<<"time: "<<times<<endl;
}

void CreateModel::on_timeEdit_timeChanged(const QTime &time)
{
    if(time.hour()<10)
       times="0"+ to_string(time.hour());
    else {
        times= to_string(time.hour());
    }
    if(time.minute()<10)
       times=times+"0"+ to_string(time.minute());
    else {
        times= times+to_string(time.minute());
    }
    cout<<"time: "<<times<<endl;
}
