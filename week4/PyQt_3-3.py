import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
from pymongo import MongoClient  # pip install pymongo, pip install dnspython

client = MongoClient("<mongodb url>")

db = client['<데이터베이스 이름>']


class MyApp(QMainWindow):

  def __init__(self):
      super().__init__()

      self.main_widget = QWidget()
      self.setCentralWidget(self.main_widget)

      canvas = FigureCanvas(Figure(figsize=(4, 3)))
      vbox = QVBoxLayout(self.main_widget)
      vbox.addWidget(canvas)

      self.addToolBar(NavigationToolbar(canvas, self))

      self.ax = canvas.figure.subplots()
      self.x = list()
      self.y = list()
      for d, cnt in zip(db['sensors'].find(), range(10, 0, -1)):  # sensors 데이터베이스의 가장 최근 값 10개를 뒤집어서 가져옴 
          self.x.append(cnt)  # x축 좌표에는 카운트 값
          self.y.append(int(d['pm2']))  # y축 좌표에는 센서 값
      self.ax.plot(self.x, self.y, '-')  # 그래프 그리기

      self.setWindowTitle('Matplotlib in PyQt5')
      self.setGeometry(300, 100, 600, 400)
      self.show()

if __name__ == '__main__':
  app = QApplication(sys.argv)
  ex = MyApp()
  sys.exit(app.exec_())
