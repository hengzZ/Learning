## 一个粗糙布局的主界面

##### 1. PyQt 安装
```bash
pip3 install --force-reinstall PyQt5==5.10.1
```

##### 组件模块查询
https://www.riverbankcomputing.com/static/Docs/PyQt5/module_index.html

#### 2. 布局策略
* Layout
    * QVBoxLayout
    * QHBoxLayout
    * QGridLayout
* GroupBox
    * QGroupBox
* Window
    * QMainWindow —— 带 Layout 布局的 QWidget。
    * QWidget

##### 特别提醒
* ***QLayout*** 与 ***QWidget*** 的继承树不同，因此两者添加到窗口的调用函数不同。
* *QLayout*
    * *QLayout.**addLayout***
    * *QLayout.addWidget*
* *QWidget*
    * *QWidget.**setLayout***
    * *QWidget.addWidget*

<br>

#### 3. 代码
```python
import os
import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtWebEngineWidgets import *


class MainWindow(QWidget):
    def __init__(self):
        super(QWidget, self).__init__()
        self.setWindowTitle('MainForm')
        # 主界面布局
        # group 1
        checkBox1 = QCheckBox('checkbox 1')
        checkBox2 = QCheckBox('checkbox 2')
        checkBox3 = QCheckBox('checkbox 3')
        checkBox4 = QCheckBox('checkbox 4')
        div1 = QVBoxLayout()
        div1.addWidget(checkBox1)
        div1.addWidget(checkBox2)
        div1.addWidget(checkBox3)
        div1.addWidget(checkBox4)
        group1 = QGroupBox('Service Type')
        group1.setLayout(div1)
        # div 2
        checkBox5 = QCheckBox('way 1')
        checkBox6 = QCheckBox('way 2')
        div2 = QHBoxLayout()
        div2.addWidget(checkBox5)
        div2.addWidget(checkBox6)
        group2 = QGroupBox('Access Type')
        group2.setLayout(div2)
        # div 3
        scanCodeArea = QWebEngineView()
        scanCodeArea.load(QUrl('http://www.baidu.com'))
        loginBtn = QPushButton('Login')
        div3 = QVBoxLayout()
        div3.addWidget(scanCodeArea)
        div3.addWidget(loginBtn)
        group3 = QGroupBox('User Login')
        group3.setLayout(div3)
        # MainForm
        leftDiv = QVBoxLayout()
        leftDiv.addWidget(group1)
        leftDiv.addWidget(group2)
        mainFormLayout = QGridLayout()
        mainFormLayout.addLayout(leftDiv, 1, 1)
        mainFormLayout.addWidget(group3, 1, 2)
        #
        self.setLayout(mainFormLayout)


if __name__=='__main__':
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())
```
