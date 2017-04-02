#include <fstream>
#include <fstream>
#include <iostream>
#include <QApplication>
#include <QIcon>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <string>
#include <sys/stat.h>
#include "mainwindow.h"
#include "settings.h"
#include "game.h"

#define DATA_PATH  "data.bin"
#define SETTINGS_PATH "settings.bin"

using namespace std;

std::vector<Game*> game_list;
Settings *program_settings;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    struct stat buffer;
    if(stat (SETTINGS_PATH, &buffer) == 0) {
        // Settings exist. Read them in.
        std::ifstream ifs(SETTINGS_PATH);
        boost::archive::text_iarchive ia(ifs);
        ia >> program_settings;
    } else {
        program_settings = new Settings;
    }

    if(stat (DATA_PATH, &buffer) == 0) {
        // There is existing data.
        std::ifstream ifs(DATA_PATH);
        boost::archive::text_iarchive ia(ifs);
        ia >> game_list;
    } else {
        // There is no data. Create a placeholder game.
        Game *default_game = new Game;
        game_list = {default_game};
    }

    QListWidget *game_list_view = w.get_games_list_view();

    for(unsigned int i = 0; i < game_list.size(); i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(game_list.at(i)->name));
        if(game_list.at(i)->icon_path == ""){
            item->setIcon(QIcon(":/questionmark.png"));
        } else {
            item->setIcon(QIcon(QString::fromStdString(game_list.at(i)->icon_path)));
        }
        item->setSizeHint(QSize(250, 50));
        game_list_view->addItem(item);
    }
    game_list_view->setCurrentRow(0);
    return a.exec();
}
