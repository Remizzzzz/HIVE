#include "hexagonalbutton.h"



HexagonalButton::HexagonalButton(int size, QWidget *parent) : QPushButton(parent) {
    // Créer l'hexagone
    createHexagon(size);
    // Créer un masque basé sur l'hexagone
    QRegion region(hexagon);  // Appliquer le polygone comme masque
    setMask(region);  // Le masque définit la zone cliquable du bouton
}

HexagonalButton& HexagonalButton::operator=(const HexagonalButton &other) {
    this->player=other.player;
    this->updateState(other.currentEvent);
    iT=other.iT;
    return *this;
}

void HexagonalButton::createHexagon(int size) {
    hexagon.clear();
    // Coordonnées des sommets d'un hexagone
    hexagon << QPoint(size / 2, 0)                        // Sommet haut
            << QPoint(size, size / 4)                    // Haut-droite
            << QPoint(size, 3 * size / 4)                // Bas-droite
            << QPoint(size / 2, size)                   // Sommet bas
            << QPoint(0, 3 * size / 4)                  // Bas-gauche
            << QPoint(0, size / 4);                     // Haut-gauche
}


void HexagonalButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen(Qt::black,2);
    painter.setPen(pen);
    // Définir la couleur en fonction de l'état
    switch (currentEvent) {
        case 0: // La case est un insecte
            if (player) {//Si c'est au joueur 1
                painter.setBrush(QBrush(Qt::red));
            } else {//Si c'est au joueur 2
                painter.setBrush(QBrush(Qt::white));
            }
        break;
        case 1: // Un insecte a été sélectionné
            painter.setBrush(QBrush(Qt::blue));
        break;
        case 2: // La case est vide
            painter.setBrush(QBrush(Qt::darkYellow));
        break;
        case 3: // La case est une destination possible
            //painter.setBrush(QBrush(Qt::cyan));
            painter.setBrush(Qt::cyan);
            break;
        case 4: //Debug
            painter.setBrush(QBrush(Qt::darkRed));
            break;
        case 5://La case est un insecte
            painter.setBrush(QBrush(Qt::white));
        default:
            break;
    }
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPolygon(hexagon);
    if (iT==ant) {
        QPixmap buttonImageAnt("../assets/ant.PNG");

        if (buttonImageAnt.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageAnt.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(3, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    } else if (iT==bee) {
        QPixmap buttonImageBee("../assets/bee.PNG");
        if (buttonImageBee.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageBee.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(1, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    } else if (iT==spider) {
        QPixmap buttonImageSpider("../assets/spider.PNG");
        if (buttonImageSpider.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageSpider.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(2, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    } else if (iT==beetle) {
        QPixmap buttonImageBeetle("../assets/beetle.PNG");
        if (buttonImageBeetle.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageBeetle.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(2, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    } else if (iT==grasshopper) {
        QPixmap buttonImageGrasshopper("../assets/grasshopper.png");
        if (buttonImageGrasshopper.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageGrasshopper.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(3, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    } else if (iT==mosquitoe) {
        QPixmap buttonImageGrasshopper("../assets/mosquitoe.png");
        if (buttonImageGrasshopper.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageGrasshopper.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(1, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    } else if (iT==ladybug) {
        QPixmap buttonImageGrasshopper("../assets/ladybug.png");
        if (buttonImageGrasshopper.isNull()) {
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(hexagon);
        } else {
            QSize newSize(25, 25);
            // Redimensionner l'image pour qu'elle s'adapte au bouton
            QPixmap scaledImage = buttonImageGrasshopper.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Calculer la position centrale pour afficher l'image
            QPoint center = rect().center();
            QPoint topLeft(0, center.y() - scaledImage.height() / 2);

            painter.drawPixmap(topLeft, scaledImage);
        }
    }
}

