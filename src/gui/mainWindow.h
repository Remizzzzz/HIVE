// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include "hive.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void startNewGame();
    void resumeGame();
    void showTutorial();
    void changeSettings();
    void launchConsoleApp();  // Lance l'application console
    void quitMenu();

private:
    Mode hiveMode = PvP; int mode = 0;
    int levelIA = 0;
    int hiveNbRewind = 5;
    bool hasLadybug = false;
    bool hasMosquito = false;
    bool load = false;
    QString nomJ1;
    QString nomJ2 = "IA";

    // Boutons du menu
    QPushButton *startButton;
    QPushButton *tutorialButton;
    QPushButton *resumeButton;
    QPushButton *settingsButton;
    QPushButton *launchConsoleButton;
    QPushButton *quitButton;

    // Zone où s'affichera tous les trucs du menu
    QStackedWidget *stackedWidget;

    void initializeEmptyWidget();
    void initializeTutorialWidget();
    void initializeSettingsWidget();
};

#endif // MAINWINDOW_H
