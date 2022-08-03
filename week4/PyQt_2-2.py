import sys
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QLabel, QLineEdit, QTextEdit, QPushButton


class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        grid = QGridLayout()
        self.setLayout(grid)

        self.des()
        self.form()
        self.button()

        grid.addWidget(self.titleDes, 0, 0)
        grid.addWidget(self.authorDes, 1, 0)
        grid.addWidget(self.textDes, 2, 0)

        grid.addWidget(self.titleForm, 0, 1)
        grid.addWidget(self.authorForm, 1, 1)
        grid.addWidget(self.textForm, 2, 1)

        grid.addWidget(self.submit, 0, 2)
        grid.addWidget(self.cancel, 1, 2)

        self.setWindowTitle('QGridLayout')
        self.setGeometry(300, 300, 300, 200)
        self.show()
        
    def des(self):
        self.titleDes = QLabel("Title:")
        self.authorDes = QLabel("Author:")
        self.textDes = QLabel("Review:")

    def form(self):
        self.titleForm = QLineEdit()
        self.authorForm = QLineEdit()
        self.textForm = QTextEdit()

    def button(self):
        self.submit = QPushButton("submit")
        self.cancel = QPushButton("cancel")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())
