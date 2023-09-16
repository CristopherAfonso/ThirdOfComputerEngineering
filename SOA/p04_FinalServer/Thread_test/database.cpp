#include "database.h"
#include "ui_database.h"

database::database(QWidget* parent, std::mutex* extern_mutex) :
  QMainWindow(parent),
  ui(new Ui::database),
  mutex_db(extern_mutex) {
  ui->setupUi(this);
  mDatabase = QSqlDatabase::addDatabase("QSQLITE");
  startDataBase();
  ui->tableWidget->resizeRowsToContents();
  ui->tableWidget->resizeColumnsToContents();
  QHeaderView* header = ui->tableWidget->horizontalHeader();
  header->setSectionResizeMode(QHeaderView::Stretch);
  header->setStretchLastSection(true);
}

database::~database() {
  if (mDatabase.isOpen()) mDatabase.close();

  delete ui;
}

bool database::startDataBase(const QString& path) {
  if (mDatabase.isOpen()) {
    mDatabase.close();
    //mutex_db->lock();
    ui->tableWidget->setRowCount(0);
    pixmaps_assoc_.clear();
    ui->label_LoadImage->clear();
    //mutex_db->unlock();
  }

  mDatabase.setDatabaseName(path); //nameFile path of the database
  bool ok = mDatabase.open();

  if (!ok)
    qDebug() << "Problem with opening database";

  else {
    QSqlQuery query(mDatabase);
    QString create_table = "CREATE TABLE IF NOT EXISTS CHEDDARPP (ip TEXT, projectName TEXT, taskNumber INTEGER, schedulable TEXT, timestamp TEXT, graph BLOB, PRIMARY KEY (ip, projectName, timestamp));";
    query.prepare(create_table);
    ok = query.exec();

    if (!ok)
      qDebug() << "Error creating table";

    else {
      ok = loadTable();

      if (!ok) qDebug() << "Error loading previous data";
    }
  }

  return ok;
}

bool database::loadTable() {
  // Load data
  QSqlQuery qry(mDatabase);
  qry.prepare("SELECT * FROM CHEDDARPP");
  bool loaded = qry.exec();

  if (loaded) {
    while(qry.next()) {
      QImage img;
      QByteArray arr = qry.value(5).toByteArray();
      img.loadFromData(arr);
      QPixmap pix = QPixmap::fromImage(img);
      pixmaps_assoc_.push_back(pix);
      //ui->label_LoadImage->setScaledContents(true);
      //QPixmap image = ui->label_Image->pixmap();
      //ui->label_LoadImage->setPixmap(image.scaled(ui->label_LoadImage->size(), Qt::KeepAspectRatio));
      //mutex_db->lock();
      ui->tableWidget->insertRow(ui->tableWidget->rowCount());

      for (int i = 0; i < 6; ++i)
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, i, new QTableWidgetItem(qry.value(i).toString()));
    }

    ///Always have the minimum date in the filter when updating table
    std::list<QDateTime> dates_in_table;

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) dates_in_table.push_front(QDateTime::fromString(ui->tableWidget->item(i, 4)->text()));

    if (!dates_in_table.empty()) ui->dateTimeEdit->setDateTime(*(std::min_element(dates_in_table.begin(), dates_in_table.end())));

    else ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    //mutex_db->unlock();
  }

  return loaded;
}

void database::insertValues(QString ip, QString projectName, int taskNumber, QString plannable, QString dateRealization, QByteArray image) {
  QSqlQuery query(mDatabase);
  query.prepare("INSERT INTO CHEDDARPP (ip, projectName, taskNumber, schedulable, timestamp, graph) VALUES (?,?,?,?,?,?);");
  query.addBindValue(ip);
  query.addBindValue(projectName.toStdString().c_str());
  query.addBindValue(taskNumber);                         ///< To received
  query.addBindValue(plannable.toStdString().c_str());    ///< To received
  query.addBindValue(dateRealization);
  query.addBindValue(image);

  if(!query.exec()) {
    qDebug() << query.lastError();
    qDebug() << "Error adding values to database";

  } else {
    //mutex_db->lock();
    QImage img;
    img.loadFromData(image);
    QPixmap pix = QPixmap::fromImage(img);
    pixmaps_assoc_.push_back(pix);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(ip));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(projectName));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString(std::to_string(taskNumber).c_str())));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(plannable));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(dateRealization));

    ///Always have the minimum date in the filter when updating table
    if ((QDateTime::fromString(dateRealization) < ui->dateTimeEdit->dateTime())) ui->dateTimeEdit->setDateTime(QDateTime::fromString(dateRealization));

    //mutex_db->unlock();
  }
}

void database::on_tableWidget_cellClicked(int row, int column) {
  //mutex_db->lock();
  QPixmap& pix = pixmaps_assoc_[row];
  ui->label_LoadImage->resize(pix.width(), pix.height());
  ui->label_LoadImage->setPixmap(pix);
  //mutex_db->unlock();
}


void database::on_pushButton_LoadDB_clicked() {
  QString nameFile = QFileDialog::getOpenFileName(this, "Open DB", QDir::rootPath(), "");

  if (nameFile != "") startDataBase(nameFile);
}

void database::keyPressEvent( QKeyEvent* event ) {
  if(event->key() == Qt::Key_Delete) {
    if (!ui->tableWidget->selectedItems().empty()) {
      QString ip_to_remove = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
      QString name = ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text();
      QString date = ui->tableWidget->item(ui->tableWidget->currentRow(), 4)->text();
      QSqlQuery query(mDatabase);
      query.prepare(QString("DELETE FROM CHEDDARPP WHERE ip='") + ip_to_remove + QString("' AND projectName='") + name + QString("' AND timestamp='") + date + QString("';"));

      if (!query.exec()) qDebug() << query.lastError();

      else {
        //mutex_db->lock();
        pixmaps_assoc_.remove(ui->tableWidget->currentRow());
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
        //mutex_db->unlock();
      }
    }
  }
}


void database::on_pushButton_NewDB_clicked() {
  QString nameFile = QFileDialog::getSaveFileName(this, "Create DB", QDir::rootPath(), "");

  if (nameFile != "") startDataBase(nameFile);
}


void database::on_lineEdit_textChanged(const QString& new_text) {
  if (!(std::regex_match(new_text.toStdString(), std::regex("^[A-Za-z0-9]*$")))) ui->lineEdit->undo();

  else
    applyFilters();
}


void database::on_dateTimeEdit_dateTimeChanged(const QDateTime& dateTime) {
  applyFilters();
}


void database::on_spinBox_filter_num_tasks_textChanged(const QString& new_text) {
  applyFilters();
}

void database::applyFilters() {
  //mutex_db->lock();
  for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
    if (((QDateTime::fromString(ui->tableWidget->item(i, 4)->text())) >= ui->dateTimeEdit->dateTime()) && (std::regex_search((ui->tableWidget->item(i, 1)->text().toStdString()), std::regex(ui->lineEdit->text().toStdString()))) && (ui->tableWidget->item(i, 2)->text().toInt() >= ui->spinBox_filter_num_tasks->value())) ui->tableWidget->showRow(i);

    else ui->tableWidget->hideRow(i);
  }

  //mutex_db->unlock();
}

