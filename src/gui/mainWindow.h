// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void openSecondaryWindow();   // Ouvre la fenêtre secondaire
    void launchConsoleApp();      // Lance l'application console
    void showGameRules();  // Lance l'affichage des règles du jeu

private:
    QPushButton *rulesButton;
    QPushButton *secondaryWindowButton;
    QPushButton *consoleAppButton;
};

#endif // MAINWINDOW_H
