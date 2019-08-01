## PyQt 创建 Icon

#### Part 1 —— 使用 PaintEvent 事件创建 Icon QWidget
```python
import os
import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

QSS_Layout = """
* {
    font-weight:bold;
    color:white;
    font-style:Microsoft YaHei
}
QMainWindow {
    width: 1000;
    height: 800;
    border-image:url(background-color.jpg)
}
QLabel#titleLbl{
}
QPushButton#clickBtn{
    background-color:white;
    color:rgb(255,0,255);
    font-weight:bold;
    border-radius: 5;
    min-width: 600
}
"""


class Pentagram(QWidget):
    def __init__(self, fillup=False):
        super(QWidget, self).__init__()
        #self.setPalette(QPalette(Qt.white))
        self.setPalette(QPalette(Qt.transparent)) # 背景透明
        self.setAutoFillBackground(True)
        self.setMinimumSize(24,24)
        self.pen = QPen(Qt.white)
        self.brush = QBrush(Qt.white)
        self.fillup = fillup
        self.points = [QPoint(12,1),QPoint(15,9),QPoint(22,9),QPoint(17,15),QPoint(19,22),QPoint(12,18),QPoint(5,22),QPoint(7,15),QPoint(1,9),QPoint(9,9)]

    def paintEvent(self, QPaintEvent):
        painter = QPainter(self)
        painter.setPen(self.pen)
        painter.setBrush(self.brush)
        if False == self.fillup:
            painter.drawPolygon(QPolygon(self.points))
        else:
            painter.drawPolygon(QPolygon(self.points), Qt.WindingFill)


class MainWindow(QMainWindow):
    def __init__(self):
        super(QMainWindow, self).__init__()
        self.setWindowTitle('华志办公指数')
        #self.setStyleSheet(QSS_Layout)  # 添加式样表
        self.setWindowIcon(QIcon('background-color.jpg'))
        # Layout
        titleLbl = QLabel('华志办公综合指数', objectName='titleLbl')
        titleLbl.setAlignment(Qt.AlignCenter)
        scoreLbl = QLabel('123567'+ ' 分', objectName='scoreLbl')
        scoreLbl.setAlignment(Qt.AlignCenter)
        vdiv1 = QVBoxLayout()
        vdiv1.addWidget(titleLbl)
        vdiv1.addWidget(scoreLbl)
        vdiv2 = QVBoxLayout()
        for idx, score in (['流畅性：', 4.0],['安全性：', 5.0],['稳定性：', 3.0],['便携性：',4.0]):
            hdiv = QHBoxLayout()
            hdiv.addWidget(QLabel(idx))
            # 星级绘制
            fillStars = int(score)
            emptyStars = 5-fillStars
            for i in range(fillStars):
                hdiv.addWidget(Pentagram(True))
            for i in range(emptyStars):
                hdiv.addWidget(Pentagram(False))
            vdiv2.addLayout(hdiv)
        vdiv3 = QVBoxLayout()
        vdiv3.addWidget(QPushButton('测试', objectName='clickBtn'))
        # MainGroup
        mdiv = QVBoxLayout()
        mdiv.addLayout(vdiv1)
        mdiv.addLayout(vdiv2)
        mdiv.addLayout(vdiv3)
        div = QHBoxLayout()
        div.addStretch()
        div.addLayout(mdiv)
        div.addStretch()
        mgroup = QGroupBox()
        mgroup.setLayout(div)
        self.setCentralWidget(mgroup)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = MainWindow()
    # 添加式样表
    win.setStyleSheet(QSS_Layout)
    win.show()
    sys.exit(app.exec_())
```

refer: https://www.cnblogs.com/hhh5460/p/4273799.html
