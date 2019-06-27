## PyQt5 事件处理机制 （Signal-Slot）

##### 语法
* 完整版
```python
QtCore.QObject.connect(widget, QtCore.SIGNAL('signalname'), slot_function)
```
* 简化版
```python
widget.signal.connect(slot_function)
```

##### 各组件的默认事件列表 （Signal 列表）
例如： 
* QPushButton (继承自 QAbstractButton)
    * clicked
    * pressed
    * released
    * toggled
* QMainWindow (继承自 QWidget)
    * iconSizeChanged
    * tabifiedDockWidgetActivated
    * toolButtonStyleChanged
    * customContextMenuRequested
    * windowIconChanged
    * windowIconTextChanged
    * windowTitleChanged
    
refer <br>
https://www.riverbankcomputing.com/static/Docs/PyQt5/api/qtwidgets/qabstractbutton.html

##### 自定义 Signal
参考 ***Support for Signals and Slots*** <br>
https://www.riverbankcomputing.com/static/Docs/PyQt5/signals_slots.html

<br>

##### 实例 —— 单击按键更换按键显示文本
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


class EventsHandler():
    def __init__(self, mainWin):
        self.window = mainWin

    def linkSignals2Slots(self):
        pushBtn = self.window.findChild(QPushButton, 'clickBtn')
        pushBtn.clicked.connect(self.clickBtnClicked)

    def clickBtnClicked(self):
        pushBtn = self.window.findChild(QPushButton, 'clickBtn')
        pushBtn.setText('测试中...')
        

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
    event_handler = EventsHandler(win)
    event_handler.linkSignals2Slots()
    win.show()
    sys.exit(app.exec_())
```

<br>

#### Part 2 —— 未使用多线程技术的事件处理
```python
import os
import sys
import time

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

QSS_Layout = """
* {
    font-size: 25px;
    font-weight: bold;
    font-style: Microsoft YaHei;
    color: white
}
QMainWindow {
    min-width: 720px;
    max-width: 720px;
    min-height: 480px;
    max-height: 480px;
    border-image: url(background-color.jpg)
}
QLabel#titleLbl {
    min-height: 50px;
    max-height: 50px;
    font-size: 35px
}
QLabel#scoreLbl {
    min-height: 50px;
    max-height: 50px;
    font-size: 35px
}
QPushButton#clickBtn {
    min-width: 300px;
    max-width: 300px;
    min-height: 50px;
    max-height: 50px;
    border-radius: 25px;
    background-color: white;
    font-weight: bold;
    color: rgb(255,0,255)
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
        self.setWindowTitle('华志办公综合指数')
        self.setWindowIcon(QIcon('background-color.jpg'))
        # Layout
        titleLbl = QLabel('华志办公综合指数', objectName='titleLbl')
        titleLbl.setAlignment(Qt.AlignCenter)
        scoreLbl = QLabel(' 分', objectName='scoreLbl')
        scoreLbl.setAlignment(Qt.AlignCenter)
        vdiv1 = QVBoxLayout()
        vdiv1.addWidget(titleLbl)
        vdiv1.addWidget(scoreLbl)
        vdiv2 = QVBoxLayout(objectName='scoreList')
        for idx, score in (['流畅性：', 5],['安全性：', 5],['稳定性：', 5],['便携性：', 5]):
            hdiv = QHBoxLayout()
            hdiv.addWidget(QLabel(idx))
            # 星级绘制
            fillStars = int(score)
            for i in range(fillStars):
                pentagram = Pentagram(True)
                pentagram.setVisible(False)
                hdiv.addWidget(pentagram)
            # 添加弹簧
            hdiv.addStretch()
            vdiv2.addLayout(hdiv)
        vdiv3 = QVBoxLayout()
        vdiv3.addWidget(QPushButton('测试', objectName='clickBtn'))
        # MainGroup
        mdiv = QVBoxLayout()
        mdiv.addLayout(vdiv1)
        mdiv.addStretch()
        mdiv.addLayout(vdiv2)
        mdiv.addStretch()
        mdiv.addLayout(vdiv3)
        div = QHBoxLayout()
        div.addStretch()
        div.addLayout(mdiv)
        div.addStretch()
        mgroup = QGroupBox()
        mgroup.setLayout(div)
        self.setCentralWidget(mgroup)


class EventsHandler(QObject):
    test_trigger = pyqtSignal(name="execTestTask")
    test_complete_trigger = pyqtSignal(name="testTaskCompleted")

    def __init__(self, mainWin):
        super(QObject,self).__init__()
        self.window = mainWin
        self.test_trigger.connect(self.execTestTask)
        self.test_complete_trigger.connect(self.testTaskCompleted)

    def initEventList(self):
        pushBtn = self.window.findChild(QPushButton, 'clickBtn')
        pushBtn.clicked.connect(self.clickBtnClicked)

    def clickBtnClicked(self):
        pushBtn = self.window.findChild(QPushButton, 'clickBtn')
        pushBtn.setEnabled(False)
        pushBtn.setText('测试中...')
        QApplication.processEvents() # UI 及时刷新
        self.test_trigger.emit()

    def execTestTask(self):
        """
        跑分测试， TODO (使用多线程实现一个新版本)
        """
        time.sleep(5)
        scoreList = (12345, 3.0, 4.0, 5.0, 3.0)
        self.updateScores(scoreList)
        self.test_complete_trigger.emit()

    def testTaskCompleted(self):
        pushBtn = self.window.findChild(QPushButton, 'clickBtn')
        pushBtn.setEnabled(True)
        pushBtn.setText('测试')

    def updateScores(self, scoreList):
        scoreLbl = self.window.findChild(QLabel, 'scoreLbl')
        scoreListLayout = self.window.findChild(QVBoxLayout, 'scoreList')
        scoreLbl.setText('%s 分' % str(scoreList[0]))
        for i in range(scoreListLayout.count()):
            layoutitem = scoreListLayout.itemAt(i)
            starcount = int(scoreList[1+i])
            for j in range(starcount):
                item = layoutitem.itemAt(1+j)
                if isinstance(item, QWidgetItem):
                    item.widget().setVisible(True)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = MainWindow()
    # 添加式样表
    win.setStyleSheet(QSS_Layout)
    # 关联事件
    ehandler = EventsHandler(win)
    ehandler.initEventList()
    # 显示
    win.show()
    sys.exit(app.exec_())
```
