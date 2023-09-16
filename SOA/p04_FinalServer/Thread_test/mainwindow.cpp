#include "mainwindow.h"
#include "ui_mainwindow.h"

std::mutex mutex_output, mutex_db;
MainWindow* outside = NULL;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->actionServer_On->setEnabled(true); ///< We turn on the On buttom
  ui->actionOff_Server->setEnabled(false); ///< We turn off the Off buttom
  server = NULL;
  select_port_ = new SelectPort(this);
  select_port_->setWindowTitle("Set port server");
  data_base_ = new database(this, &mutex_db);
  outside = this;
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event) {
  if (isEnabled()) event->accept();

  else event->ignore();
}

void MainWindow::on_actionServer_On_triggered() {
  server = new QTcpServer();

  if (server->listen(QHostAddress::Any, findChild<QSpinBox*>("spinBox_port")->value())) {
    connect(server, SIGNAL(newConnection()), this, SLOT(manageConnect()));
    ui->actionServer_On->setEnabled(false); ///< We turn off the On buttom
    ui->actionOff_Server->setEnabled(true); ///< We turn on the Off buttom
    QMessageBox::information(this, "Server is activated", ("Waiting connections in port " + std::to_string(findChild<QSpinBox*>("spinBox_port")->value())).c_str());

  } else QMessageBox::critical(this, "Listening error", server->errorString());
}

void MainWindow::manageConnect() {
  while (server->hasPendingConnections()) {
    SocketThread* new_thread = new SocketThread(server->nextPendingConnection(), this);
    //connection_list.push_front(new_thread);
    //connect(new_socket, SIGNAL(readyRead()), this, SLOT(clientInteraction()));
    mutex_output.lock();
    ui->clients_msgsEdit->insertPlainText("New client connected...\n");
    mutex_output.unlock();
  }
}

void SocketThread::clientInteraction(MainWindow* main_app) {
  QTcpSocket* client_conn = reinterpret_cast<QTcpSocket*>(sender());
  QByteArray message_from_client;

  while (client_conn->bytesAvailable() > 0)
    message_from_client = client_conn->readAll();

  if (message_from_client.toStdString() == "SEND_IMG") {
    client_conn->write("OK");
    client_conn->flush();
    client_conn->waitForBytesWritten();
    ///Read the image info
    message_from_client.clear();
    client_conn->waitForReadyRead();

    while (client_conn->bytesAvailable() > 0)
      message_from_client = client_conn->readAll();

    std::stringstream stream(message_from_client.toStdString());
    std::string arg;
    std::vector<std::string> info;

    while (std::getline(stream, arg, '|'))
      info.push_back(arg);

    ///Read the image
    message_from_client.clear();
    client_conn->waitForReadyRead();

    while (client_conn->bytesAvailable() > 0)
      message_from_client = client_conn->readAll();

    QImage image_from_client;
    bool delivery_success = image_from_client.loadFromData(message_from_client);

    if (delivery_success) {
      mutex_output.lock();
      main_app->ui->clients_msgsEdit->insertPlainText("Client " + QString::number(client_conn->socketDescriptor()) + " sent this image...\n");
      QPixmap pixmap_image_client = QPixmap::fromImage(image_from_client);
      main_app->ui->label_to_show_image->resize(pixmap_image_client.width(), pixmap_image_client.height());
      main_app->ui->label_to_show_image->setPixmap(pixmap_image_client);
      mutex_output.unlock();
      QByteArray byteArrayImage;
      QBuffer bufferImage(&byteArrayImage);
      bufferImage.open(QIODevice::WriteOnly);
      image_from_client.save(&bufferImage, "JPEG");
      //mutex_db.lock();
      main_app->data_base_->insertValues(client_conn->localAddress().toString(), info[0].c_str(), std::stoi(info[1]), info[2].c_str(), QDateTime::currentDateTime().toString(), byteArrayImage);
      //mutex_db.unlock();

    } else {
      mutex_output.lock();
      main_app->ui->clients_msgsEdit->insertPlainText("The image from client " + QString::number(client_conn->socketDescriptor()) + " could not be read\n");
      mutex_output.unlock();
    }

  } else if (message_from_client.toStdString() == "RECEIVE_IMG") {
    client_conn->write("OK");
    client_conn->flush();
    client_conn->waitForBytesWritten();
    ///Get the date, name and date desired
    message_from_client.clear();
    client_conn->waitForReadyRead();

    while (client_conn->bytesAvailable() > 0)
      message_from_client = client_conn->readAll();

    std::stringstream stream(message_from_client.toStdString());
    std::string arg;
    std::vector<std::string> info;

    while (std::getline(stream, arg, '|'))
      info.push_back(arg);

    ///Filter rows based on that
    if (info.size() == 3) {
      //mutex_db.lock();
      main_app->data_base_->findChild<QLineEdit*>("lineEdit")->setText(info[0].c_str());
      main_app->data_base_->findChild<QSpinBox*>("spinBox_filter_num_tasks")->setValue(QString(info[1].c_str()).toInt());
      main_app->data_base_->findChild<QDateTimeEdit*>("dateTimeEdit")->setDateTime(QDateTime::fromString(info[2].c_str()));
      ///Send the list of images identified by name and date where that IP appears
      std::string wishable_images = "";
      QTableWidget* table = main_app->data_base_->findChild<QTableWidget*>("tableWidget");

      for (int i = 0; i < table->rowCount(); ++i)
        if (table->item(i, 0)->text() == client_conn->localAddress().toString() && !(table->isRowHidden(i))) wishable_images += (std::to_string(i) + " - Name: " + table->item(i, 1)->text().toStdString() + " Date: " + table->item(i, 4)->text().toStdString() + "\n");

      //mutex_db.unlock();
      client_conn->write((wishable_images == "" ? "X" : wishable_images.c_str())); ///<Database is empty?
      client_conn->flush();
      client_conn->waitForBytesWritten();

      ///Receive the number indicating the row wanted
      if (wishable_images != "") {
        message_from_client.clear();
        client_conn->waitForReadyRead();

        while (client_conn->bytesAvailable() > 0)
          message_from_client = client_conn->readAll();

        if (message_from_client.toStdString() != "") {
          int client_wanted_row = std::stoi(message_from_client.toStdString());
          //mutex_db.lock();

          ///Send the image of that row
          if (client_wanted_row >= 0 && client_wanted_row < main_app->data_base_->pixmaps_assoc_.size() && table->item(client_wanted_row, 0)->text() == client_conn->localAddress().toString()) {
            QPixmap pix = main_app->data_base_->pixmaps_assoc_[client_wanted_row];
            QImage image_to_server = pix.toImage();
            QByteArray byteArrayImage;
            QBuffer bufferImage(&byteArrayImage);
            bufferImage.open(QIODevice::WriteOnly);
            image_to_server.save(&bufferImage, "JPEG"); ///< We store the image in the "bufferImage" as a "JPEG" to be sent to server
            client_conn->write(byteArrayImage);
            client_conn->flush();
            client_conn->waitForBytesWritten();
          }

          //mutex_db.unlock();

        } else {
          mutex_output.lock();
          main_app->ui->clients_msgsEdit->insertPlainText("The client " + QString::number(client_conn->socketDescriptor()) + " didn't ask for a row...\n");
          mutex_output.unlock();
        }

      } else {
        mutex_output.lock();
        main_app->ui->clients_msgsEdit->insertPlainText("The client " + QString::number(client_conn->socketDescriptor()) + " requested an invalid image\n");
        mutex_output.unlock();
      }

    } else {
      mutex_output.lock();
      main_app->ui->clients_msgsEdit->insertPlainText("Client " + QString::number(client_conn->socketDescriptor()) + " didn't send the filters...\n");
      mutex_output.unlock();
    }

  } else {
    mutex_output.lock();
    main_app->ui->clients_msgsEdit->insertPlainText("The client " + QString::number(client_conn->socketDescriptor()) + " wants to do an unknown action\n");
    mutex_output.unlock();
  }
}

void MainWindow::on_actionOff_Server_triggered() {
  /*
  for(auto i : connection_list) {
    i->disconnect(); ///< We disconnect each socket before deleting it
    delete i; ///< We delete each socket of server
  }

  connection_list.clear();
  delete server;
  server = NULL;

  if (!connection_list.empty())
    QMessageBox::critical(this, "Error: The task to clean the client's list failed", "The server had some problems to shut down, try again");

  ui->actionServer_On->setEnabled(true); ///< We turn on the On buttom
  ui->actionOff_Server->setEnabled(false); ///< We turn  the Off buttom
  QMessageBox::information(this, "Shutdown successful", "The server was turned off successful");
  */
}


void MainWindow::on_actionSelect_the_port_to_server_triggered() {
  select_port_->show();
  this->setEnabled(false);
  select_port_->setEnabled(true);
}


void MainWindow::on_actionData_Base_triggered() {
  data_base_->show();
}

void MainWindow::on_actionAbout_Cheddar_Server_triggered() {
  QMessageBox::information(this, "About Cheddar++ Server", "This is a real-time scheduler simulator inspired by Cheddar. Is done using QT Creator framework (version 6.5.1) to make this program. This program acts as a server, all the clients will connect (via TCP) to introduce data to the database and this will send the clients images and some data of the scheduling planning.");
}

void MainWindow::on_actionManual_triggered() {
  QString link = "https://github.com/feichay10/Server-Proyecto-Final-SOA/blob/main/README.md";
  QDesktopServices::openUrl(QUrl(link));
}

SocketThread::SocketThread(QTcpSocket* socket, QObject* parent)
  : QThread(parent)
  , socket_(socket) {
  connect(socket_, &QTcpSocket::readyRead, this, [this] {clientInteraction(outside);});
}

void SocketThread::run() {
  if (!socket_->waitForConnected()) {
    mutex_output.lock();
    std::cout << "Error with a socket thread:" << socket_->errorString().toStdString() << std::endl;
    mutex_output.unlock();
  }

  exec();
}
