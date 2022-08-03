import sys
import random
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QToolTip
from PyQt5.QtGui import QIcon, QPixmap, QFont
from PyQt5.QtCore import QCoreApplication

class Program(QWidget):
    def __init__(self):
        super().__init__()
        self.ui_init()

    def ui_init(self):
        self.image()
        self.button()
        self.tooltip()
        self.num()

        self.setWindowTitle("대표를 선출하라!")
        self.setWindowIcon(QIcon("/home/user/다운로드/home.webp"))
        self.setGeometry(0, 0, 400, 400)
        self.show()

    def image(self):
        self.IMAGE = QLabel(self)
        self.IMAGE.setPixmap(QPixmap("/home/user/다운로드/home.webp").scaled(35, 44))
        self.IMAGE.move(100, 10)

    def button(self):
        self.BUTTON = QPushButton("대표 선출", self)
        self.BUTTON.setFixedSize(340, 40)
        self.BUTTON.move(30, 150)

    def tooltip(self):
        pass

    def num(self):
        pass

loop = QApplication(sys.argv)
instance = Program()
loop.exec_()
