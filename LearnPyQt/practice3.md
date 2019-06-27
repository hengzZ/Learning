## Qt 式样表 （QSS）
—— http://thesmithfam.org/blog/2009/09/10/qt-stylesheets-tutorial/

Qt 样式表的思想很大程度上是来自于 HTML 的层叠式样式表(CSS)，通过调用 QWidget::setStyleSheet() 或 QApplication::setStyleSheet()，可以为一个独立的子部件、整个窗口，甚至是整个应用程序指定一个样式表。

* Qt 样式表与 CSS 的语法规则几乎完全相同
* selector { attribute: value }

<br>

##### 注意事项！
* pWidget->setStyleSheet("background-image:url(:/bmp/IMG_0345.JPG)");
  **假使这个 pWidget 继承自 QWidget，而且是顶层窗口，则不会显示图片。**
   解释如下：Qt 三大窗体部件 QMainwindow， QDialog， QWidget 之间的关系，QMainwindow， QDialog 继承于 QWidget，而 QWidget 没有父窗体。
* 设置背景图片的三种方式
    * QPalette 的方法
    ```c++
      QWidget *widget=new QWidget;
      widget->autoFillBackground(true);
      QPalette palette;
      palette.setColor(QPalette::Background,QColor(192,253,123));
      //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/background.png")));
      widget->setPalette(palette);
    ```
    * setStyleSheet 的方法
    ```c++
      QFrame *frame = new QFrame;
      frame->setObjectName("myframe");  // 为组件指定名字
      frame->setStyleSheet("QFrame#myframe{border-image:url(images/frame.png)}" );
    ```
    * paintEvent 的方法 (重写 QWidget 的 paintEvent 事件)
    ```c++
      class MyFrame : public QWidget
      {
      public:
          MyFrame();
          void paintEvent(QPaintEvent *event);
      };
      // cpp
      MyFrame::MyFrame()
      {
      }
      void MyFrame::paintEvent(QPaintEvent *event)
      {
          QPainter painter(this);
          painter.drawPixmap(0,0,400,700,QPixmap("images/frame.png"));
      }
    ```
    
* ***border-image*** vs ***background-image***
    * background-image 不能保证图片大小和控件大小一致。
    * border-image 保证图片大小和控件大小一致。

* 窗口置顶
```python
  Dialog.setWindowFlags(QtCore.Qt.WindowCloseButtonHint|QtCore.Qt.WindowStaysOnTopHint)
```

<br>

##### QSS 常用语法
—— https://www.cnblogs.com/findumars/p/10278961.html

https://doc.qt.io/qt-5/stylesheet-reference.html
https://www.cnblogs.com/wangqiguo/p/4960776.html



<br>

#### Part 1 —— 设置窗口背景图片的例子
```python
import os
import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *


#class MainWindow(QWidget):    # QWidget 的 setStyleSheet 方法不能设置背景图片！
class MainWindow(QMainWindow):
    def __init__(self):
        super(QMainWindow, self).__init__()
        self.setWindowTitle('MainForm')
        #self.setStyleSheet('background-color:black')
        self.setStyleSheet('border-image:url(image.jpg)')
        self.setWindowIcon(QIcon('image.jpg'))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())
```

<br>

#### Part 2 —— 一个 QSS 的配置案例
```python
import os
import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *


class MainWindow(QMainWindow):
    def __init__(self):
        super(QMainWindow, self).__init__()
        self.setWindowTitle('办公综合指数')
        self.setStyleSheet( """
                QMainWindow{border-image:url(background-image.jpg)}
                QLabel#titleLbl{}
                QPushButton#clickBtn{background-color:red}
                """)
        self.setWindowIcon(QIcon('icon.jpg'))
        # Layout
        titleLbl = QLabel('办公综合指数', objectName='titleLbl')
        scoreLbl = QLabel('123567'+ ' 分', objectName='scoreLbl')
        vdiv1 = QVBoxLayout()
        vdiv1.addWidget(titleLbl)
        vdiv1.addWidget(scoreLbl)
        vdiv2 = QVBoxLayout()
        for idx, score in (['流畅性：', 4.0],['安全性：', 5.0],['稳定性：', 3.0],['便携性：',4.0]):
            hdiv = QHBoxLayout()
            hdiv.addWidget(QLabel(idx))
            hdiv.addWidget(QLabel(str(score)))
            vdiv2.addLayout(hdiv)
        vdiv3 = QVBoxLayout()
        vdiv3.addWidget(QPushButton('测试', objectName='clickBtn'))
        # MainGroup
        mdiv = QVBoxLayout()
        mdiv.addLayout(vdiv1)
        mdiv.addLayout(vdiv2)
        mdiv.addLayout(vdiv3)
        mgroup = QGroupBox()
        mgroup.setLayout(mdiv)
        self.setCentralWidget(mgroup)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())
```
